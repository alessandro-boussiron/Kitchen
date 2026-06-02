/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Entry point — shell loop, input tokenizer, command dispatch
*/

#include "kitchen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const command_t commands[] = {
    {"exit", cmd_exit},
    {"help", cmd_help},
    {"recipes", cmd_recipes},
    {"ingredients", cmd_ingredients},
    {"make", cmd_make},
    {"add", cmd_add},
    {NULL, NULL},
};

static void free_tokens(char **tokens)
{
    size_t i;

    if (!tokens)
        return;
    for (i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
}

static char *extract_token(char **p)
{
    char *start;
    size_t len;
    char *tok;

    if (**p == '"') {
        (*p)++;
        start = *p;
        while (**p && **p != '"')
            (*p)++;
        len = *p - start;
        if (**p == '"')
            (*p)++;
    } else {
        start = *p;
        while (**p && !isspace((unsigned char)**p))
            (*p)++;
        len = *p - start;
    }
    tok = malloc(len + 1);
    if (!tok)
        return NULL;
    memcpy(tok, start, len);
    tok[len] = '\0';
    return tok;
}

static char **append_token(char **tokens, size_t count, char *tok)
{
    char **tmp;

    tmp = realloc(tokens, (count + 2) * sizeof(char *));
    if (!tmp)
        return NULL;
    tmp[count] = tok;
    tmp[count + 1] = NULL;
    return tmp;
}

static char **tokenize(char *line)
{
    char **tokens = NULL;
    char **tmp;
    size_t count = 0;
    char *p = line;
    size_t len;
    char *tok;

    len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    while (*p) {
        while (*p && isspace((unsigned char)*p))
            p++;
        if (!*p)
            break;
        tok = extract_token(&p);
        if (!tok) {
            free_tokens(tokens);
            return NULL;
        }
        tmp = append_token(tokens, count, tok);
        if (!tmp) {
            free(tok);
            free_tokens(tokens);
            return NULL;
        }
        tokens = tmp;
        count++;
    }
    if (!tokens)
        tokens = calloc(1, sizeof(char *));
    return tokens;
}

static int dispatch(config_t *cfg, char **tokens)
{
    int i;

    if (!tokens || !tokens[0])
        return SUCCESS;
    for (i = 0; commands[i].name; i++)
        if (strcmp(tokens[0], commands[i].name) == 0)
            return commands[i].fn(cfg, tokens);
    printf("Unknown command: '%s' (type 'help' for a list)\n", tokens[0]);
    return FAIL;
}

static void run_shell(config_t *cfg)
{
    char line[LINE_MAX_LEN];
    char **tokens;
    int ret;

    while (1) {
        printf("kitchen> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin))
            break;
        tokens = tokenize(line);
        if (!tokens)
            break;
        ret = dispatch(cfg, tokens);
        free_tokens(tokens);
        if (ret == SHELL_EXIT)
            break;
    }
}

int main(void)
{
    config_t *cfg = load_config(CONFIG_PATH);

    if (!cfg)
        return 84;
    run_shell(cfg);
    save_config(CONFIG_PATH, cfg);
    free_config(cfg);
    return 0;
}
