The recipes are defined by the program and have this behaviour :

```c
typedef int (*apply_recipe) (config_t *) // return value is a SUCCESS or a FAIL (look macros) | A recipe should triggers only if the kitchen has enough workers, the requested equipement and enough ingredients in fridge

#define RECIPE_QUANTITY (int)(3) // Change in function of recipes

typedef enum {
    PEPPERONI_PIZZA,
    CHEESE_PIZZA,
    HAM_PIZZA,
    DOUGH,
    UNDEFINED,
} recipe_type_t; // Add more recipes (before the undefined)

typedef struct recipe {
    char *name;
    apply_recipe *apply;
    recipe_type_t recipe;
} recipe_t;

// In a recipe.c file, we'll have an array of ingredients name with quantitys

typedef struct recipe_ingredients {
    char **ingredients; // List of ingredients
    size_t *quantity; // Amount of ingredients in the same index
    size_t count; // How much different ingredients
    bool store_made_product;
} recipe_ingredients_t

// It should look like

recipe_ingredients_t ingredients[] = {
    {{"ingredient 1", "ingredient 2", "ingredient 3", NULL}, {9, 5, 1}, 3, true}
}

// ingredients[DOUGH] should return the ingredients of dough

```