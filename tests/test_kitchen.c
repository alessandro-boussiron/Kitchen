/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Unit tests — config, recipes, commands
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "kitchen.h"
#include <stdlib.h>
#include <string.h>

/* ── helpers ─────────────────────────────────────────────────────────────── */

static config_t *make_cfg(void)
{
    config_t *cfg = calloc(1, sizeof(config_t));
    cfg->kitchen = calloc(1, sizeof(kitchen_t));
    return cfg;
}

static config_t *make_full_cfg(void)
{
    config_t *cfg = make_cfg();

    cfg->kitchen->workers_count = 2;
    cfg->kitchen->equipment = malloc(3 * sizeof(char *));
    cfg->kitchen->equipment[0] = strdup("furnace");
    cfg->kitchen->equipment[1] = strdup("knife");
    cfg->kitchen->equipment_count = 2;
    add_ingredient(cfg, "dough",     10);
    add_ingredient(cfg, "tomato",    10);
    add_ingredient(cfg, "cheese",    10);
    add_ingredient(cfg, "pepperoni", 10);
    add_ingredient(cfg, "ham",       10);
    return cfg;
}

/* ── find_ingredient ─────────────────────────────────────────────────────── */

Test(find_ingredient, null_cfg)
{
    cr_assert_null(find_ingredient(NULL, "cheese"));
}

Test(find_ingredient, null_name)
{
    config_t *cfg = make_cfg();

    cr_assert_null(find_ingredient(cfg, NULL));
    free_config(cfg);
}

Test(find_ingredient, empty_stock)
{
    config_t *cfg = make_cfg();

    cr_assert_null(find_ingredient(cfg, "cheese"));
    free_config(cfg);
}

Test(find_ingredient, found)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, "cheese", 5), SUCCESS);
    cr_assert_not_null(find_ingredient(cfg, "cheese"));
    cr_assert_str_eq(find_ingredient(cfg, "cheese")->ingredient, "cheese");
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)5);
    free_config(cfg);
}

Test(find_ingredient, not_found_after_other_added)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, "cheese", 5), SUCCESS);
    cr_assert_null(find_ingredient(cfg, "dough"));
    free_config(cfg);
}

/* ── add_ingredient ──────────────────────────────────────────────────────── */

Test(add_ingredient, null_cfg)
{
    cr_assert_eq(add_ingredient(NULL, "cheese", 5), FAIL);
}

Test(add_ingredient, null_name)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, NULL, 5), FAIL);
    free_config(cfg);
}

Test(add_ingredient, adds_new_ingredient)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, "cheese", 10), SUCCESS);
    cr_assert_eq(cfg->ingredient_count, (size_t)1);
    cr_assert_str_eq(cfg->ingredients[0]->ingredient, "cheese");
    cr_assert_eq(cfg->ingredients[0]->quantity, (size_t)10);
    free_config(cfg);
}

Test(add_ingredient, accumulates_existing)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, "cheese", 5), SUCCESS);
    cr_assert_eq(add_ingredient(cfg, "cheese", 3), SUCCESS);
    cr_assert_eq(cfg->ingredient_count, (size_t)1);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)8);
    free_config(cfg);
}

Test(add_ingredient, multiple_distinct)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, "dough",  1), SUCCESS);
    cr_assert_eq(add_ingredient(cfg, "cheese", 2), SUCCESS);
    cr_assert_eq(add_ingredient(cfg, "tomato", 3), SUCCESS);
    cr_assert_eq(cfg->ingredient_count, (size_t)3);
    cr_assert_eq(find_ingredient(cfg, "dough")->quantity,  (size_t)1);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)2);
    cr_assert_eq(find_ingredient(cfg, "tomato")->quantity, (size_t)3);
    free_config(cfg);
}

Test(add_ingredient, zero_quantity)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(add_ingredient(cfg, "cheese", 0), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)0);
    free_config(cfg);
}

/* ── free_config ─────────────────────────────────────────────────────────── */

Test(free_config, null_safe)
{
    free_config(NULL);
    cr_assert(1);
}

Test(free_config, empty_cfg)
{
    config_t *cfg = make_cfg();

    free_config(cfg);
    cr_assert(1);
}

Test(free_config, full_cfg)
{
    config_t *cfg = make_cfg();

    add_ingredient(cfg, "dough",  5);
    add_ingredient(cfg, "cheese", 3);
    cfg->kitchen->workers_count = 2;
    cfg->kitchen->equipment = malloc(2 * sizeof(char *));
    cfg->kitchen->equipment[0] = strdup("furnace");
    cfg->kitchen->equipment_count = 1;
    free_config(cfg);
    cr_assert(1);
}

/* ── find_recipe ─────────────────────────────────────────────────────────── */

Test(find_recipe, null_name)
{
    cr_assert_null(find_recipe(NULL));
}

Test(find_recipe, unknown_name)
{
    cr_assert_null(find_recipe("sushi"));
}

Test(find_recipe, pepperoni_pizza)
{
    const recipe_t *r = find_recipe("pepperoni_pizza");

    cr_assert_not_null(r);
    cr_assert_str_eq(r->name, "pepperoni_pizza");
    cr_assert_eq(r->recipe, PEPPERONI_PIZZA);
}

Test(find_recipe, cheese_pizza)
{
    const recipe_t *r = find_recipe("cheese_pizza");

    cr_assert_not_null(r);
    cr_assert_eq(r->recipe, CHEESE_PIZZA);
}

Test(find_recipe, ham_pizza)
{
    const recipe_t *r = find_recipe("ham_pizza");

    cr_assert_not_null(r);
    cr_assert_eq(r->recipe, HAM_PIZZA);
}

/* ── apply — success cases ───────────────────────────────────────────────── */

Test(apply_pepperoni, success_deducts_ingredients)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("pepperoni_pizza");

    cr_assert_eq(r->apply(cfg), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "dough")->quantity,     (size_t)9);
    cr_assert_eq(find_ingredient(cfg, "tomato")->quantity,    (size_t)8);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity,    (size_t)8);
    cr_assert_eq(find_ingredient(cfg, "pepperoni")->quantity, (size_t)7);
    free_config(cfg);
}

Test(apply_cheese, success_deducts_ingredients)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("cheese_pizza");

    cr_assert_eq(r->apply(cfg), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "dough")->quantity,  (size_t)9);
    cr_assert_eq(find_ingredient(cfg, "tomato")->quantity, (size_t)8);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)7);
    free_config(cfg);
}

Test(apply_ham, success_deducts_ingredients)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("ham_pizza");

    cr_assert_eq(r->apply(cfg), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "dough")->quantity,  (size_t)9);
    cr_assert_eq(find_ingredient(cfg, "tomato")->quantity, (size_t)8);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)8);
    cr_assert_eq(find_ingredient(cfg, "ham")->quantity,    (size_t)8);
    free_config(cfg);
}

/* ── apply — failure cases ───────────────────────────────────────────────── */

Test(apply_pepperoni, no_workers)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("pepperoni_pizza");

    cfg->kitchen->workers_count = 0;
    cr_assert_eq(r->apply(cfg), FAIL);
    free_config(cfg);
}

Test(apply_pepperoni, missing_equipment)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("pepperoni_pizza");

    free(cfg->kitchen->equipment[1]);
    cfg->kitchen->equipment[1] = NULL;
    cfg->kitchen->equipment_count = 1;
    cr_assert_eq(r->apply(cfg), FAIL);
    free_config(cfg);
}

Test(apply_pepperoni, not_enough_ingredient)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("pepperoni_pizza");

    find_ingredient(cfg, "pepperoni")->quantity = 1; /* needs 3 */
    cr_assert_eq(r->apply(cfg), FAIL);
    cr_assert_eq(find_ingredient(cfg, "dough")->quantity, (size_t)10);
    free_config(cfg);
}

Test(apply_pepperoni, exactly_enough_succeeds)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("pepperoni_pizza");

    find_ingredient(cfg, "dough")->quantity     = 1;
    find_ingredient(cfg, "tomato")->quantity    = 2;
    find_ingredient(cfg, "cheese")->quantity    = 2;
    find_ingredient(cfg, "pepperoni")->quantity = 3;
    cr_assert_eq(r->apply(cfg), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "dough")->quantity,     (size_t)0);
    cr_assert_eq(find_ingredient(cfg, "pepperoni")->quantity, (size_t)0);
    free_config(cfg);
}

Test(apply_cheese, no_furnace)
{
    config_t *cfg = make_full_cfg();
    const recipe_t *r = find_recipe("cheese_pizza");

    free(cfg->kitchen->equipment[0]);
    cfg->kitchen->equipment[0] = strdup("knife");
    free(cfg->kitchen->equipment[1]);
    cfg->kitchen->equipment[1] = NULL;
    cfg->kitchen->equipment_count = 1;
    cr_assert_eq(r->apply(cfg), FAIL);
    free_config(cfg);
}

/* ── cmd_exit ────────────────────────────────────────────────────────────── */

Test(cmd_exit, returns_shell_exit)
{
    cr_assert_eq(cmd_exit(NULL, NULL), SHELL_EXIT);
}

/* ── cmd_help ────────────────────────────────────────────────────────────── */

Test(cmd_help, returns_success, .init = cr_redirect_stdout)
{
    cr_assert_eq(cmd_help(NULL, NULL), SUCCESS);
}

/* ── cmd_recipes ─────────────────────────────────────────────────────────── */

Test(cmd_recipes, returns_success, .init = cr_redirect_stdout)
{
    cr_assert_eq(cmd_recipes(NULL, NULL), SUCCESS);
}

/* ── cmd_ingredients ─────────────────────────────────────────────────────── */

Test(cmd_ingredients, empty_stock, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();

    cr_assert_eq(cmd_ingredients(cfg, NULL), SUCCESS);
    free_config(cfg);
}

Test(cmd_ingredients, with_stock, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"ingredients", NULL};

    add_ingredient(cfg, "cheese", 5);
    cr_assert_eq(cmd_ingredients(cfg, args), SUCCESS);
    free_config(cfg);
}

/* ── cmd_make ────────────────────────────────────────────────────────────── */

Test(cmd_make, no_recipe_arg, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"make", NULL};

    cr_assert_eq(cmd_make(cfg, args), FAIL);
    free_config(cfg);
}

Test(cmd_make, unknown_recipe, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"make", "sushi", NULL};

    cr_assert_eq(cmd_make(cfg, args), FAIL);
    free_config(cfg);
}

Test(cmd_make, success_pepperoni, .init = cr_redirect_stdout)
{
    config_t *cfg = make_full_cfg();
    char *args[] = {"make", "pepperoni_pizza", NULL};

    cr_assert_eq(cmd_make(cfg, args), SUCCESS);
    free_config(cfg);
}

Test(cmd_make, fail_not_enough_stock, .init = cr_redirect_stdout)
{
    config_t *cfg = make_full_cfg();
    char *args[] = {"make", "pepperoni_pizza", NULL};

    find_ingredient(cfg, "pepperoni")->quantity = 0;
    cr_assert_eq(cmd_make(cfg, args), FAIL);
    free_config(cfg);
}

/* ── cmd_add ─────────────────────────────────────────────────────────────── */

Test(cmd_add, no_args, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"add", NULL};

    cr_assert_eq(cmd_add(cfg, args), FAIL);
    free_config(cfg);
}

Test(cmd_add, missing_qty, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"add", "cheese", NULL};

    cr_assert_eq(cmd_add(cfg, args), FAIL);
    free_config(cfg);
}

Test(cmd_add, invalid_qty_alpha, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"add", "cheese", "abc", NULL};

    cr_assert_eq(cmd_add(cfg, args), FAIL);
    cr_assert_eq(cfg->ingredient_count, (size_t)0);
    free_config(cfg);
}

Test(cmd_add, invalid_qty_mixed, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"add", "cheese", "5abc", NULL};

    cr_assert_eq(cmd_add(cfg, args), FAIL);
    free_config(cfg);
}

Test(cmd_add, success_new_ingredient, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"add", "cheese", "7", NULL};

    cr_assert_eq(cmd_add(cfg, args), SUCCESS);
    cr_assert_eq(cfg->ingredient_count, (size_t)1);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)7);
    free_config(cfg);
}

Test(cmd_add, success_accumulates, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args1[] = {"add", "cheese", "5", NULL};
    char *args2[] = {"add", "cheese", "3", NULL};

    cr_assert_eq(cmd_add(cfg, args1), SUCCESS);
    cr_assert_eq(cmd_add(cfg, args2), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)8);
    free_config(cfg);
}

Test(cmd_add, zero_quantity, .init = cr_redirect_stdout)
{
    config_t *cfg = make_cfg();
    char *args[] = {"add", "cheese", "0", NULL};

    cr_assert_eq(cmd_add(cfg, args), SUCCESS);
    cr_assert_eq(find_ingredient(cfg, "cheese")->quantity, (size_t)0);
    free_config(cfg);
}
