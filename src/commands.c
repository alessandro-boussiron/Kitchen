/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Shell command implementations
*/

#include "kitchen.h"
#include <stdio.h>

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
    if (recipe->apply(cfg) != SUCCESS)
        return FAIL;
    printf("'%s' successfully cooked!\n", recipe->name);
    if (g_recipe_ingredients[recipe->recipe].store_made_product) {
        if (add_ingredient(cfg, recipe->name, 1) != SUCCESS) {
            printf("Error: could not store '%s' in stock.\n", recipe->name);
            return FAIL;
        }
        printf("Stored 1 x '%s' in stock.\n", recipe->name);
    }
    return SUCCESS;
}
