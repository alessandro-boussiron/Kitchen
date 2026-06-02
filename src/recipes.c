/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Recipe definitions and apply logic
*/

#include "kitchen.h"
#include <stdio.h>
#include <string.h>

const recipe_ingredients_t g_recipe_ingredients[RECIPE_QUANTITY] = {
    [PEPPERONI_PIZZA] = {
        (const char *[]){"dough", "tomato", "cheese", "pepperoni"},
        (size_t[]){1, 2, 2, 3},
        4, false,
        (const char *[]){"furnace", "knife", NULL}
    },
    [CHEESE_PIZZA] = {
        (const char *[]){"dough", "tomato", "cheese"},
        (size_t[]){1, 2, 3},
        3, false,
        (const char *[]){"furnace", NULL}
    },
    [HAM_PIZZA] = {
        (const char *[]){"dough", "tomato", "cheese", "ham"},
        (size_t[]){1, 2, 2, 2},
        4, false,
        (const char *[]){"furnace", "knife", NULL}
    },
    [DOUGH] = {
        (const char *[]){"flour", "water"},
        (size_t[]){2, 1},
        2, true,
        (const char *[]){NULL}
    },
};

static int kitchen_has_equipment(const kitchen_t *k, const char *item)
{
    size_t i;

    for (i = 0; i < k->equipment_count; i++)
        if (strcmp(k->equipment[i], item) == 0)
            return 1;
    return 0;
}

static int do_apply(config_t *cfg, recipe_type_t type)
{
    const recipe_ingredients_t *def = &g_recipe_ingredients[type];
    stocks_t *slots[8];
    size_t i;

    if (cfg->kitchen->workers_count == 0) {
        printf("Error: no workers available.\n");
        return FAIL;
    }
    for (i = 0; def->equipment[i]; i++) {
        if (!kitchen_has_equipment(cfg->kitchen, def->equipment[i])) {
            printf("Error: missing equipment '%s'.\n", def->equipment[i]);
            return FAIL;
        }
    }
    for (i = 0; i < def->count; i++) {
        slots[i] = find_ingredient(cfg, def->ingredients[i]);
        if (!slots[i] || slots[i]->quantity < def->quantity[i]) {
            printf("Error: not enough '%s' (need %zu).\n",
                def->ingredients[i], def->quantity[i]);
            return FAIL;
        }
    }
    for (i = 0; i < def->count; i++)
        slots[i]->quantity -= def->quantity[i];
    return SUCCESS;
}

static int apply_pepperoni_pizza(config_t *cfg)
{
    return do_apply(cfg, PEPPERONI_PIZZA);
}

static int apply_cheese_pizza(config_t *cfg)
{
    return do_apply(cfg, CHEESE_PIZZA);
}

static int apply_ham_pizza(config_t *cfg)
{
    return do_apply(cfg, HAM_PIZZA);
}

static int apply_dough(config_t *cfg)
{
    return do_apply(cfg, DOUGH);
}

const recipe_t g_recipes[RECIPE_QUANTITY] = {
    {"pepperoni_pizza", apply_pepperoni_pizza, PEPPERONI_PIZZA},
    {"cheese_pizza",    apply_cheese_pizza,    CHEESE_PIZZA},
    {"ham_pizza",       apply_ham_pizza,       HAM_PIZZA},
    {"dough",           apply_dough,           DOUGH},
};

const recipe_t *find_recipe(const char *name)
{
    int i;

    if (!name)
        return NULL;
    for (i = 0; i < RECIPE_QUANTITY; i++)
        if (strcmp(g_recipes[i].name, name) == 0)
            return &g_recipes[i];
    return NULL;
}
