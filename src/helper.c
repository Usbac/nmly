#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helper.h"
#include "string.h"


static char *changeCases(const char *file,
                         bool upper)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name = strBefore(filename, '.');
    char *ext = strAfter(filename, '.');
    char *result = malloc_(strlen(file) + 1);
    char *cases;

    snprintf(result, strlen(dir) + 2, "%s/", dir);

    /* Without extension */
    if (!name) {
        cases = strCases(filename, upper);
        strCpy(result + strlen(result), cases);
        goto end;
    }

    /* With extension */
    cases = strCases(name, upper);
    snprintf(result + strlen(result),
        strlen(cases) + strlen(ext) + 2,
        "%s.%s",
        cases,
        ext);

    end:
        free(cases);
        free(ext);
        free(name);
        free(filename);
        free(dir);

        return result;
}


static void doSwitch(char *new,
                     const char *sep,
                     const char *part_one,
                     const char *part_two)
{
    size_t len_one = strlen(part_one);
    size_t len_two = strlen(part_two);
    size_t len = len_one + strlen(sep) + len_two + 1;
    char *one = malloc_(len_one + 1);
    char *two = malloc_(len_two + 1);
    char *format;

    strCpy(one, part_one);
    strCpy(two, part_two);

    /* Leave whitespace between separator */
    if (one[len_one - 1] == ' ' && two[0] == ' ') {
        len += 2;
        format = "%s %s %s";
    } else {
        format = "%s%s%s";
    }

    strTrim(two);
    strTrim(one);
    snprintf(new, len, format, two, sep, one);
    free(one);
    free(two);
}


void *malloc_(size_t size)
{
    void *alloc_mem = malloc(size);
    if (!alloc_mem) {
        printf(MSG_MEM_ERROR);
        exit(-1);
    }

    return alloc_mem;
}


char *joinPaths(const char *path_one, const char *path_two)
{
    size_t len_one = strlen(path_one);
    size_t len_two = strlen(path_two);
    size_t len = len_one + len_two + 2;
    char *result;

    result = malloc_(len);
    snprintf(result, len, "%s/%s", path_one, path_two);

    return result;
}


char *before(const char *file, const char *text)
{
    size_t len = strlen(file) + strlen(text) + 2;
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *result = malloc_(len);

    snprintf(result,
        len,
        "%s/%s%s",
        dir,
        text,
        filename);

    free(dir);
    free(filename);

    return result;
}


char *after(const char *file, const char *text)
{
    size_t len = strlen(file) + strlen(text) + 2;
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name = strBefore(filename, '.');
    char *result = malloc_(len);
    char *ext;
    size_t dir_len;

    snprintf(result, strlen(dir) + 2, "%s/", dir);
    dir_len = strlen(result);

    /* Without extension */
    if (!name) {
        snprintf(result + dir_len,
            strlen(filename) + strlen(text) + 1,
            "%s%s",
            filename,
            text);
        goto end;
    }

    /* With extension */
    ext = strAfter(filename, '.');
    snprintf(result + dir_len,
        strlen(name) + strlen(text) + strlen(ext) + 2,
        "%s%s.%s",
        name,
        text,
        ext);
    free(ext);

    end:
        free(name);
        free(filename);
        free(dir);

        return result;
}


char *reverse(const char *file)
{
    char *result = malloc_(strlen(file));
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name;

    snprintf(result, strlen(dir) + 2, "%s/", dir);
    name = strBefore(filename, '.');

    /* Without extension */
    if (!name) {
        strReverse(filename);
        strCpy(result + strlen(result), filename);
    /* With extension */
    } else {
        char *ext = strAfter(filename, '.');
        strReverse(name);
        snprintf(result + strlen(result),
            strlen(name) + strlen(ext) + 2,
            "%s.%s",
            name,
            ext);
        free(name);
        free(ext);
    }

    free(filename);
    free(dir);

    return result;
}


char *upper(const char *file)
{
    return changeCases(file, true);
}


char *lower(const char *file)
{
    return changeCases(file, false);
}


char *replace(const char *file, const char *ori, const char *rep)
{
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *replaced, *result = NULL;

    if (strstr(filename, ori) != NULL) {
        replaced = strReplace(filename, ori, rep);
        result = joinPaths(dir, replaced);
        free(replaced);
    }

    free(filename);
    free(dir);

    return result;
}


char *switchSides(const char *file, char sep)
{
    char *part_one, *part_two;
    char *dir = strBefore(file, '/');
    char *filename = strAfter(file, '/');
    char *name = strBefore(filename, '.');
    char *switched = malloc_(strlen(filename) + 1);
    char *result = NULL;
    char tmp[2];
    tmp[0] = sep;
    tmp[1] = '\0';

    if (name == NULL || strrchr(name, sep) == NULL) {
        goto end;
    }

    part_one = strBefore(name ? name : filename, sep);
    part_two = strAfter(name ? name : filename, sep);

    if (part_one && sep != '.') {
        result = malloc_(strlen(file) + 1);
        doSwitch(switched, tmp, part_one, part_two);
        snprintf(result, strlen(dir) + 2, "%s/", dir);
        strCpy(result + strlen(result), switched);

        if (name) {
            char *ext = strAfter(filename, '.');
            snprintf(result + strlen(result), strlen(ext) + 2, ".%s", ext);
            free(ext);
        }
    }

    free(part_two);
    free(part_one);

    end:
        free(dir);
        free(filename);
        free(name);
        free(switched);

        return result;
}
