/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Recipe definitions and apply logic
*/

#include "kitchen.h"
#include <stdio.h>
#include <string.h>

static const char *pepperoni_ingr[] = {"dough", "tomato", "cheese", "pepperoni"};
static size_t pepperoni_qty[] = {1, 2, 2, 3};

static const char *cheese_ingr[] = {"dough", "tomato", "cheese"};
static size_t cheese_qty[] = {1, 2, 3};

static const char *ham_ingr[] = {"dough", "tomato", "cheese", "ham"};
static size_t ham_qty[] = {1, 2, 2, 2};

static const recipe_ingredients_t pepperoni_def = {pepperoni_ingr, pepperoni_qty, 4};
static const recipe_ingredients_t cheese_def    = {cheese_ingr, cheese_qty, 3};
static const recipe_ingredients_t ham_def       = {ham_ingr, ham_qty, 4};

static const char *pepperoni_equip[] = {"furnace", "knife", NULL};
static const char *cheese_equip[]    = {"furnace", NULL};
static const char *ham_equip[]       = {"furnace", "knife", NULL};

static int kitchen_has_equipment(const kitchen_t *k, const char *item)
{
    size_t i;

    for (i = 0; i < k->equipment_count; i++)
        if (strcmp(k->equipment[i], item) == 0)
            return 1;
    return 0;
}

static int do_apply(config_t *cfg, const recipe_ingredients_t *def,
    const char **required_equip)
{
    stocks_t *slots[8];
    size_t i;

    if (cfg->kitchen->workers_count == 0) {
        printf("Error: no workers available.\n");
        return FAIL;
    }
    for (i = 0; required_equip[i]; i++) {
        if (!kitchen_has_equipment(cfg->kitchen, required_equip[i])) {
            printf("Error: missing equipment '%s'.\n", required_equip[i]);
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
    return do_apply(cfg, &pepperoni_def, pepperoni_equip);
}

static int apply_cheese_pizza(config_t *cfg)
{
    return do_apply(cfg, &cheese_def, cheese_equip);
}

static int apply_ham_pizza(config_t *cfg)
{
    return do_apply(cfg, &ham_def, ham_equip);
}

const recipe_t g_recipes[RECIPE_QUANTITY] = {
    {"pepperoni_pizza", apply_pepperoni_pizza, PEPPERONI_PIZZA},
    {"cheese_pizza",    apply_cheese_pizza,    CHEESE_PIZZA},
    {"ham_pizza",       apply_ham_pizza,        HAM_PIZZA},
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
