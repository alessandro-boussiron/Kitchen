/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Stock commands — ingredients, add
*/

#include "kitchen.h"
#include <stdio.h>
#include <stdlib.h>

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
