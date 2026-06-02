/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Config section parsers — stock and kitchen
*/

#ifndef CFG_SECTIONS_H_
    #define CFG_SECTIONS_H_

    #include "kitchen.h"
    #include <stddef.h>

int parse_stock_section(const char *json, size_t *i, config_t *cfg);
int parse_kitchen_section(const char *json, size_t *i, config_t *cfg);

#endif /* !CFG_SECTIONS_H_ */
