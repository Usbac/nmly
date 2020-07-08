#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"


static void trim(char *str)
{
    size_t len = strlen(str) - 1;

    while (str[0] == ' ') {
        memmove(str, str + 1, strlen(str));
        len--;
    }

    while (str[len] == ' ') {
        str[len] = '\0';
        len--;
    }
}


static void substr(char *sub, const char *str, const int start, const int len)
{
    memcpy(sub, &str[start], len);
}


static char *strBefore(const char *str, const char ch)
{
    char *pos = strrchr(str, ch);
    char *part;
    size_t size;

    if (!pos) {
        return NULL;
    }

    size = pos - str;
    part = malloc_(size + 1 * sizeof(char));
    memset(part, 0, size + 1);
    substr(part, str, 0, size);

    if (part[size] != '\0') {
        part[size] = '\0';
    }

    return part;
}


static void strCases(char *dest, const char *str, const int upper)
{
    size_t len = strlen(str);
    size_t i;

    for (i = 0; i < len; i++) {
        dest[i] = upper ? toupper(str[i]) : tolower(str[i]);
    }

    dest[i] = '\0';
}


static void changeCases(char **src,
                        const char *file,
                        const int upper)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name = strBefore(filename, '.');
    char *ext = strAfter(filename, '.');
    char *cases;

    snprintf(*src, strlen(dir) + 2, "%s/", dir);

    /* Without extension */
    if (!name) {
        cases = malloc_(strlen(filename) + 1 * sizeof(char));
        strCases(cases, filename, upper);
        strcpy_(*src + strlen(*src), cases);
        /* With extension */
    } else {
        cases = malloc_(strlen(name) + 1 * sizeof(char));
        strCases(cases, name, upper);
        snprintf(*src + strlen(*src), strlen(cases) + strlen(ext) + 2, "%s.%s", cases, ext);
    }

    free(cases);
    free(ext);
    free(name);
    free(filename);
    free(dir);
}


static int strReplace(char **src,
                      const char *str,
                      const char *ori,
                      const char *rep)
{
    char *remaining_ref = malloc_(strlen(str) + 1 * sizeof(char));
    char *remaining = remaining_ref;
    char *pos;

    strcpy_(remaining, str);

    if (!strstr(remaining, ori)) {
        free(remaining_ref);
        return 1;
    }

    while ((pos = strstr(remaining, ori))) {
        strncat(*src + strlen(*src), remaining, pos - remaining);
        strcat(*src, rep);
        remaining += pos - remaining + strlen(ori);
    }

    strcpy_(*src + strlen(*src), remaining);
    free(remaining_ref);

    return 0;
}


static void doSwitch(char *new,
                     const char *sep,
                     const char *part_one,
                     const char *part_two)
{
    int len_one = strlen(part_one);
    int len_two = strlen(part_two);
    int len = len_one + strlen(sep) + len_two + 1;
    char *one = malloc_(len_one + 1 * sizeof(char));
    char *two = malloc_(len_two + 1 * sizeof(char));
    char *format;

    memset(one, 0, len_one + 1);
    memset(two, 0, len_two + 1);
    strcpy_(one, part_one);
    strcpy_(two, part_two);

    /* Leave whitespace between separator */
    if (one[len_one - 1] == ' ' && two[0] == ' ') {
        len += 2;
        format = "%s %s %s";
    } else {
        format = "%s%s%s";
    }

    trim(two);
    trim(one);
    snprintf(new, len, format, two, sep, one);
    free(one);
    free(two);
}


static char *strrev(char *str)
{
    size_t i, j;
    char tmp;

    if (!str || !*str) {
        return str;
    }

    i = strlen(str) - 1;
    j = 0;

    while (i > j) {
        tmp = str[i];
        str[i--] = str[j];
        str[j++] = tmp;
    }

    return str;
}


void *malloc_(size_t size)
{
    void *alloc_mem = malloc(size);
    if (!alloc_mem) {
        printf(ERROR_MEM_MSG);
        exit(-1);
    }

    return alloc_mem;
}


void strcpy_(char *src, const char *ori)
{
    snprintf(src, strlen(ori) + 1, "%s", ori);
}


void concatPath(char *src, const char *path_one, const char *path_two)
{
    snprintf(src, strlen(path_one) + strlen(path_two) + 2, "%s/%s", path_one, path_two);
}


void before(char **src, const char *file, const char *text)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    snprintf(*src, strlen(file) + strlen(text) + 2, "%s/%s%s", dir, text, filename);
    free(dir);
    free(filename);
}


char *strAfter(const char *str, const char ch)
{
    char *pos = strrchr(str, ch);
    char *part;
    size_t size;
    size_t len;

    if (!pos) {
        return NULL;
    }

    size = pos - str;
    len = strlen(str) - size;
    part = malloc_(len * sizeof(char));
    memset(part, 0 , len);
    substr(part, str, size + 1, len);

    return part;
}


void after(char **src, const char *file, const char *text)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name = strBefore(filename, '.');
    char *ext;
    int len;

    snprintf(*src, strlen(dir) + 2, "%s/", dir);
    len = strlen(*src);

    /* Without extension */
    if (!name) {
        snprintf(*src + len,
            strlen(filename) + strlen(text) + 1,
            "%s%s",
            filename,
            text);
    /* With extension */
    } else {
        ext = strAfter(filename, '.');
        snprintf(*src + len,
            strlen(name) + strlen(text) + strlen(ext) + 2,
            "%s%s.%s",
            name,
            text,
            ext);
        free(ext);
    }

    free(name);
    free(filename);
    free(dir);
}


void reverse(char **src, const char *file)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name, *ext;
    int len;
    snprintf(*src, strlen(dir) + 2, "%s/", dir);
    len = strlen(*src);
    name = strBefore(filename, '.');

    /* Without extension */
    if (!name) {
        strrev(filename);
        strcpy_(*src + len, filename);
    /* With extension */
    } else {
        ext = strAfter(filename, '.');
        strrev(name);
        snprintf(*src + len, strlen(name) + strlen(ext) + 2, "%s.%s", name, ext);
        free(name);
        free(ext);
    }

    free(filename);
    free(dir);
}


void upper(char **src, const char *file)
{
    changeCases(&*src, file, 1);
}


void lower(char **src, const char *file)
{
    changeCases(&*src, file, 0);
}


void replace(char **src, const char *file, const char *ori, const char *rep)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *replaced = malloc_(BUFFER * sizeof(char));
    memset(replaced, 0, BUFFER);

    if (!strReplace(&replaced, filename, ori, rep)) {
        concatPath(*src, dir, replaced);
    }

    free(replaced);
    free(filename);
    free(dir);
}


void switchSides(char **src, const char *file, const char sep)
{
    char *part_one, *part_two, *ext;
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name = strBefore(filename, '.');
    char *switched = malloc_(strlen(filename) + 1 * sizeof(char));
    char tmp[2];
    tmp[0] = sep;
    tmp[1] = '\0';

    part_one = strBefore(name ? name : filename, sep);
    part_two = strAfter(name ? name : filename, sep);

    if (part_one && sep != '.') {
        doSwitch(switched, tmp, part_one, part_two);
        snprintf(*src, strlen(dir) + 2, "%s/", dir);
        strcpy_(*src + strlen(*src), switched);

        if (name) {
            ext = strAfter(filename, '.');
            snprintf(*src + strlen(*src), strlen(ext) + 2, ".%s", ext);
            free(ext);
        }
    }

    free(part_two);
    free(part_one);
    free(name);
    free(filename);
    free(dir);
    free(switched);
}
