/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** JSON config file parser — load_config and all parsing helpers
*/

#include "kitchen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *read_file(const char *path)
{
    FILE *f = fopen(path, "r");
    long size;
    char *buf;

    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    buf = malloc(size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    if ((long)fread(buf, 1, size, f) != size) {
        free(buf);
        fclose(f);
        return NULL;
    }
    buf[size] = '\0';
    fclose(f);
    return buf;
}

static void skip_ws(const char *s, size_t *i)
{
    while (s[*i] && isspace((unsigned char)s[*i]))
        (*i)++;
}

static char *parse_string(const char *s, size_t *i)
{
    size_t start;
    size_t len;
    char *result;

    skip_ws(s, i);
    if (s[*i] != '"')
        return NULL;
    (*i)++;
    start = *i;
    while (s[*i] && s[*i] != '"')
        (*i)++;
    len = *i - start;
    result = malloc(len + 1);
    if (!result)
        return NULL;
    memcpy(result, s + start, len);
    result[len] = '\0';
    if (s[*i] == '"')
        (*i)++;
    return result;
}

static long parse_number(const char *s, size_t *i)
{
    long val = 0;

    skip_ws(s, i);
    while (s[*i] >= '0' && s[*i] <= '9') {
        val = val * 10 + (s[*i] - '0');
        (*i)++;
    }
    return val;
}

static int expect_char(const char *s, size_t *i, char c)
{
    skip_ws(s, i);
    if (s[*i] != c)
        return 0;
    (*i)++;
    return 1;
}

static int parse_equipment_array(const char *json, size_t *i, kitchen_t *k)
{
    char *item;
    char **tmp;

    if (!expect_char(json, i, '['))
        return 0;
    while (1) {
        skip_ws(json, i);
        if (json[*i] == ']')
            break;
        if (json[*i] == ',') {
            (*i)++;
            continue;
        }
        item = parse_string(json, i);
        if (!item)
            return 0;
        tmp = realloc(k->equipment, (k->equipment_count + 2) * sizeof(char *));
        if (!tmp) {
            free(item);
            return 0;
        }
        k->equipment = tmp;
        k->equipment[k->equipment_count++] = item;
        k->equipment[k->equipment_count] = NULL;
        skip_ws(json, i);
        if (json[*i] == ',')
            (*i)++;
    }
    (*i)++;
    return 1;
}

static int parse_stock_section(const char *json, size_t *i, config_t *cfg)
{
    char *key;
    long qty;

    if (!expect_char(json, i, '{'))
        return 0;
    while (1) {
        skip_ws(json, i);
        if (json[*i] == '}')
            break;
        if (json[*i] == ',') {
            (*i)++;
            continue;
        }
        key = parse_string(json, i);
        if (!key)
            return 0;
        if (!expect_char(json, i, ':')) {
            free(key);
            return 0;
        }
        skip_ws(json, i);
        qty = parse_number(json, i);
        if (add_ingredient(cfg, key, (size_t)qty) != SUCCESS) {
            free(key);
            return 0;
        }
        free(key);
        skip_ws(json, i);
        if (json[*i] == ',')
            (*i)++;
    }
    (*i)++;
    return 1;
}

static int parse_kitchen_section(const char *json, size_t *i, config_t *cfg)
{
    char *key;
    kitchen_t *k = cfg->kitchen;

    if (!expect_char(json, i, '{'))
        return 0;
    while (1) {
        skip_ws(json, i);
        if (json[*i] == '}')
            break;
        if (json[*i] == ',') {
            (*i)++;
            continue;
        }
        key = parse_string(json, i);
        if (!key)
            return 0;
        if (!expect_char(json, i, ':')) {
            free(key);
            return 0;
        }
        skip_ws(json, i);
        if (strcmp(key, "workers") == 0) {
            k->workers_count = (size_t)parse_number(json, i);
        } else if (strcmp(key, "equipement") == 0) {
            if (!parse_equipment_array(json, i, k)) {
                free(key);
                return 0;
            }
        }
        free(key);
        skip_ws(json, i);
        if (json[*i] == ',')
            (*i)++;
    }
    (*i)++;
    return 1;
}

static int parse_config_object(const char *json, config_t *cfg)
{
    char *key;
    size_t i = 0;

    if (!expect_char(json, &i, '{'))
        return 0;
    while (1) {
        skip_ws(json, &i);
        if (json[i] == '}')
            break;
        if (json[i] == ',') {
            i++;
            continue;
        }
        key = parse_string(json, &i);
        if (!key)
            return 0;
        if (!expect_char(json, &i, ':')) {
            free(key);
            return 0;
        }
        skip_ws(json, &i);
        if (strcmp(key, "stock") == 0) {
            if (!parse_stock_section(json, &i, cfg)) {
                free(key);
                return 0;
            }
        } else if (strcmp(key, "kitchen") == 0) {
            if (!parse_kitchen_section(json, &i, cfg)) {
                free(key);
                return 0;
            }
        }
        free(key);
        skip_ws(json, &i);
        if (json[i] == ',')
            i++;
    }
    return 1;
}

config_t *load_config(const char *path)
{
    char *json = read_file(path);
    config_t *cfg;

    if (!json) {
        fprintf(stderr, "Error: cannot open config file '%s'\n", path);
        return NULL;
    }
    cfg = calloc(1, sizeof(config_t));
    if (!cfg) {
        free(json);
        return NULL;
    }
    cfg->kitchen = calloc(1, sizeof(kitchen_t));
    if (!cfg->kitchen) {
        free(json);
        free(cfg);
        return NULL;
    }
    if (!parse_config_object(json, cfg)) {
        fprintf(stderr, "Error: failed to parse '%s'\n", path);
        free(json);
        free_config(cfg);
        return NULL;
    }
    free(json);
    return cfg;
}
