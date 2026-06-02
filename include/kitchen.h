/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Main types and prototypes for the kitchen simulator
*/

#ifndef KITCHEN_H_
    #define KITCHEN_H_

    #include <stddef.h>
    #include <stdbool.h>

    #define SUCCESS    (int)(0)
    #define FAIL       (int)(1)
    #define SHELL_EXIT (int)(-1)

    #define RECIPE_QUANTITY (int)(4)

    #define CONFIG_PATH  "config.json"
    #define LINE_MAX_LEN 1024

typedef struct stocks {
    char *ingredient;
    size_t quantity;
} stocks_t;

typedef struct kitchen {
    char **equipment;
    size_t equipment_count;
    size_t workers_count;
} kitchen_t;

typedef struct config {
    stocks_t **ingredients;
    size_t ingredient_count;
    kitchen_t *kitchen;
} config_t;

typedef int (*apply_recipe_t)(config_t *);

typedef enum {
    PEPPERONI_PIZZA,
    CHEESE_PIZZA,
    HAM_PIZZA,
    DOUGH,
    UNDEFINED,
} recipe_type_t;

typedef struct recipe {
    char *name;
    apply_recipe_t apply;
    recipe_type_t recipe;
} recipe_t;

typedef struct recipe_ingredients {
    const char **ingredients;
    size_t *quantity;
    size_t count;
    bool store_made_product;
    const char **equipment;
} recipe_ingredients_t;

typedef int (*cmd_fn_t)(config_t *, char **);

typedef struct command {
    const char *name;
    cmd_fn_t fn;
} command_t;

// Config I/O
config_t *load_config(const char *path);
void save_config(const char *path, const config_t *cfg);
void free_config(config_t *cfg);

// Stock helpers
stocks_t *find_ingredient(config_t *cfg, const char *name);
int add_ingredient(config_t *cfg, const char *name, size_t qty);

// Recipes
extern const recipe_t g_recipes[RECIPE_QUANTITY];
extern const recipe_ingredients_t g_recipe_ingredients[RECIPE_QUANTITY];
const recipe_t *find_recipe(const char *name);

// Commands
int cmd_exit(config_t *cfg, char **args);
int cmd_help(config_t *cfg, char **args);
int cmd_recipes(config_t *cfg, char **args);
int cmd_ingredients(config_t *cfg, char **args);
int cmd_make(config_t *cfg, char **args);
int cmd_add(config_t *cfg, char **args);

#endif /* !KITCHEN_H_ */
