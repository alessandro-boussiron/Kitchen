/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Low-level JSON primitive helpers
*/

#ifndef JSON_UTILS_H_
    #define JSON_UTILS_H_

    #include <stddef.h>

char *read_file(const char *path);
void skip_ws(const char *s, size_t *i);
char *parse_string(const char *s, size_t *i);
long parse_number(const char *s, size_t *i);
int expect_char(const char *s, size_t *i, char c);

#endif /* !JSON_UTILS_H_ */
