/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Config management — free, save, stock helpers
*/

#include "kitchen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_kitchen(kitchen_t *k)
{
    size_t i;

    if (!k)
        return;
    if (k->equipment) {
        for (i = 0; i < k->equipment_count; i++)
            free(k->equipment[i]);
        free(k->equipment);
    }
    free(k);
}

void free_config(config_t *cfg)
{
    size_t i;

    if (!cfg)
        return;
    if (cfg->ingredients) {
        for (i = 0; i < cfg->ingredient_count; i++) {
            free(cfg->ingredients[i]->ingredient);
            free(cfg->ingredients[i]);
        }
        free(cfg->ingredients);
    }
    free_kitchen(cfg->kitchen);
    free(cfg);
}

static void save_stock(FILE *f, const config_t *cfg)
{
    size_t i;

    fprintf(f, "{\n    \"stock\" : {\n");
    for (i = 0; i < cfg->ingredient_count; i++) {
        fprintf(f, "        \"%s\": %zu",
            cfg->ingredients[i]->ingredient,
            cfg->ingredients[i]->quantity);
        fprintf(f, "%s\n", (i + 1 < cfg->ingredient_count) ? "," : "");
    }
}

static void save_kitchen(FILE *f, const kitchen_t *k)
{
    size_t i;

    fprintf(f, "    },\n    \"kitchen\" : {\n");
    fprintf(f, "        \"workers\" : %zu,\n", k->workers_count);
    fprintf(f, "        \"equipement\" : [\n");
    for (i = 0; i < k->equipment_count; i++) {
        fprintf(f, "            \"%s\"%s\n",
            k->equipment[i],
            (i + 1 < k->equipment_count) ? "," : "");
    }
    fprintf(f, "        ]\n    }\n}\n");
}

void save_config(const char *path, const config_t *cfg)
{
    FILE *f = fopen(path, "w");

    if (!f) {
        fprintf(stderr, "Error: cannot write config file '%s'\n", path);
        return;
    }
    save_stock(f, cfg);
    save_kitchen(f, cfg->kitchen);
    fclose(f);
}

stocks_t *find_ingredient(config_t *cfg, const char *name)
{
    size_t i;

    if (!cfg || !name)
        return NULL;
    for (i = 0; i < cfg->ingredient_count; i++)
        if (strcmp(cfg->ingredients[i]->ingredient, name) == 0)
            return cfg->ingredients[i];
    return NULL;
}

static int append_stock_entry(config_t *cfg, stocks_t *entry)
{
    stocks_t **tmp;

    tmp = realloc(cfg->ingredients,
        (cfg->ingredient_count + 1) * sizeof(stocks_t *));
    if (!tmp) {
        free(entry->ingredient);
        free(entry);
        return FAIL;
    }
    cfg->ingredients = tmp;
    cfg->ingredients[cfg->ingredient_count] = entry;
    cfg->ingredient_count++;
    return SUCCESS;
}

int add_ingredient(config_t *cfg, const char *name, size_t qty)
{
    stocks_t *existing;
    stocks_t *entry;

    if (!cfg || !name)
        return FAIL;
    existing = find_ingredient(cfg, name);
    if (existing) {
        existing->quantity += qty;
        return SUCCESS;
    }
    entry = malloc(sizeof(stocks_t));
    if (!entry)
        return FAIL;
    entry->ingredient = strdup(name);
    if (!entry->ingredient) {
        free(entry);
        return FAIL;
    }
    entry->quantity = qty;
    return append_stock_entry(cfg, entry);
}
