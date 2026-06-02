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

void free_config(config_t *cfg)
{
    size_t i;

    if (!cfg)
        return;
    if (cfg->ingredients) {
        for (i = 0; i < cfg->ingredient_count; i++) {
            if (cfg->ingredients[i]) {
                free(cfg->ingredients[i]->ingredient);
                free(cfg->ingredients[i]);
            }
        }
        free(cfg->ingredients);
    }
    if (cfg->kitchen) {
        if (cfg->kitchen->equipment) {
            for (i = 0; i < cfg->kitchen->equipment_count; i++)
                free(cfg->kitchen->equipment[i]);
            free(cfg->kitchen->equipment);
        }
        free(cfg->kitchen);
    }
    free(cfg);
}

void save_config(const char *path, const config_t *cfg)
{
    FILE *f = fopen(path, "w");
    size_t i;

    if (!f) {
        fprintf(stderr, "Error: cannot write config file '%s'\n", path);
        return;
    }
    fprintf(f, "{\n    \"stock\" : {\n");
    for (i = 0; i < cfg->ingredient_count; i++) {
        fprintf(f, "        \"%s\": %zu",
            cfg->ingredients[i]->ingredient,
            cfg->ingredients[i]->quantity);
        fprintf(f, "%s\n", (i + 1 < cfg->ingredient_count) ? "," : "");
    }
    fprintf(f, "    },\n    \"kitchen\" : {\n");
    fprintf(f, "        \"workers\" : %zu,\n", cfg->kitchen->workers_count);
    fprintf(f, "        \"equipement\" : [\n");
    for (i = 0; i < cfg->kitchen->equipment_count; i++) {
        fprintf(f, "            \"%s\"%s\n",
            cfg->kitchen->equipment[i],
            (i + 1 < cfg->kitchen->equipment_count) ? "," : "");
    }
    fprintf(f, "        ]\n    }\n}\n");
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

int add_ingredient(config_t *cfg, const char *name, size_t qty)
{
    stocks_t *existing;

    if (!cfg || !name)
        return FAIL;
    existing = find_ingredient(cfg, name);
    stocks_t *entry;
    stocks_t **tmp;

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
    tmp = realloc(cfg->ingredients,
        (cfg->ingredient_count + 1) * sizeof(stocks_t *));
    if (!tmp) {
        free(entry->ingredient);
        free(entry);
        return FAIL;
    }
    cfg->ingredients = tmp;
    cfg->ingredients[cfg->ingredient_count++] = entry;
    return SUCCESS;
}
