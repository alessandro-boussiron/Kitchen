/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Shell command implementations
*/

#include "kitchen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmd_exit(config_t *cfg, char **args)
{
    (void)cfg;
    (void)args;
    return SHELL_EXIT;
}

int cmd_help(config_t *cfg, char **args)
{
    (void)cfg;
    (void)args;
    printf("Available commands:\n");
    printf("  exit                       Quit the kitchen shell\n");
    printf("  help                       Show this help\n");
    printf("  recipes                    List all available recipes\n");
    printf("  ingredients                List all ingredients in stock\n");
    printf("  make <recipe_name>         Cook a dish\n");
    printf("  add \"<ingredient>\" <qty>   Add stock to an ingredient\n");
    return SUCCESS;
}

int cmd_recipes(config_t *cfg, char **args)
{
    int i;

    (void)cfg;
    (void)args;
    for (i = 0; i < RECIPE_QUANTITY; i++)
        printf("%s\n", g_recipes[i].name);
    return SUCCESS;
}

int cmd_ingredients(config_t *cfg, char **args)
{
    size_t i;

    (void)args;
    if (!cfg->ingredients || cfg->ingredient_count == 0) {
        printf("No ingredients in stock.\n");
        return SUCCESS;
    }
    for (i = 0; i < cfg->ingredient_count; i++)
        printf("%s: %zu\n",
            cfg->ingredients[i]->ingredient,
            cfg->ingredients[i]->quantity);
    return SUCCESS;
}

int cmd_make(config_t *cfg, char **args)
{
    const recipe_t *recipe;

    if (!args[1]) {
        printf("Usage: make <recipe_name>\n");
        return FAIL;
    }
    recipe = find_recipe(args[1]);
    if (!recipe) {
        printf("Error: recipe '%s' not found.\n", args[1]);
        return FAIL;
    }
    if (recipe->apply(cfg) == SUCCESS) {
        printf("'%s' successfully cooked!\n", recipe->name);
        return SUCCESS;
    }
    return FAIL;
}

int cmd_add(config_t *cfg, char **args)
{
    char *endptr;
    size_t qty;

    if (!args[1] || !args[2]) {
        printf("Usage: add \"<ingredient>\" <qty>\n");
        return FAIL;
    }
    qty = (size_t)strtoul(args[2], &endptr, 10);
    if (endptr == args[2] || *endptr != '\0') {
        printf("Error: invalid quantity '%s'.\n", args[2]);
        return FAIL;
    }
    if (add_ingredient(cfg, args[1], qty) != SUCCESS) {
        printf("Error: could not add ingredient.\n");
        return FAIL;
    }
    printf("Added %zu x '%s'.\n", qty, args[1]);
    return SUCCESS;
}
