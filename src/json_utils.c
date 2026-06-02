/*
** EPITECH PROJECT, 2026
** Kitchen
** File description:
** Low-level JSON primitive helpers
*/

#include "json_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *read_buf(FILE *f, size_t size)
{
    char *buf;

    buf = malloc(size + 1);
    if (!buf)
        return NULL;
    if (fread(buf, 1, size, f) != size) {
        free(buf);
        return NULL;
    }
    buf[size] = '\0';
    return buf;
}

char *read_file(const char *path)
{
    FILE *f = fopen(path, "r");
    long raw_size;
    char *buf;

    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    raw_size = ftell(f);
    rewind(f);
    if (raw_size < 0) {
        fclose(f);
        return NULL;
    }
    buf = read_buf(f, (size_t)raw_size);
    fclose(f);
    return buf;
}

void skip_ws(const char *s, size_t *i)
{
    while (s[*i] && isspace((unsigned char)s[*i]))
        (*i)++;
}

char *parse_string(const char *s, size_t *i)
{
    size_t start;
    size_t len;
    char *result;

    skip_ws(s, i);
    if (s[*i] != '"')
        return NULL;
    (*i)++;
    start = *i;
    while (s[*i] && s[*i] != '"')
        (*i)++;
    len = *i - start;
    result = malloc(len + 1);
    if (!result)
        return NULL;
    memcpy(result, s + start, len);
    result[len] = '\0';
    if (s[*i] == '"')
        (*i)++;
    return result;
}

long parse_number(const char *s, size_t *i)
{
    long val = 0;

    skip_ws(s, i);
    while (s[*i] >= '0' && s[*i] <= '9') {
        val = val * 10 + (s[*i] - '0');
        (*i)++;
    }
    return val;
}

int expect_char(const char *s, size_t *i, char c)
{
    skip_ws(s, i);
    if (s[*i] != c)
        return 0;
    (*i)++;
    return 1;
}
