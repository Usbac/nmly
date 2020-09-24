#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "string.h"


void strReverse(char *str)
{
    size_t i, j;
    char tmp;

    if (!str || !*str) {
        return;
    }

    i = strlen(str) - 1;
    j = 0;

    while (i > j) {
        tmp = str[i];
        str[i--] = str[j];
        str[j++] = tmp;
    }
}


char *strReplace(const char *str,
                 const char *old,
                 const char *rep)
{
    char *result;
    size_t i, counter = 0;
    size_t old_len = strlen(old);
    size_t rep_len = strlen(rep);

    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], old) == &str[i]) {
            counter++;
            i += old_len - 1;
        }
    }

    result = malloc(i + (counter * (rep_len - old_len)) + 1);

    i = 0;
    while (*str) {
        if (strstr(str, old) == str) {
            strCpy(&result[i], rep);
            i += rep_len;
            str += old_len;
        } else {
            result[i++] = *str++;
        }
    }

    result[i] = '\0';
    return result;
}


char *strCases(const char *str, bool upper)
{
    size_t len = strlen(str), i;
    char *result = malloc(len + 1);

    for (i = 0; i <= len; i++) {
        result[i] = upper ? toupper(str[i]) : tolower(str[i]);
    }

    return result;
}


char *strBefore(const char *str, char ch)
{
    char *pos, *part;
    size_t size;

    if (str == NULL) {
        return NULL;
    }

    pos = strrchr(str, ch);

    if (!pos) {
        return NULL;
    }

    size = pos - str + 1;
    part = malloc(size + 1);
    snprintf(part, size, "%s", str);
    part[size] = '\0';

    return part;
}


char *strAfter(const char *str, char ch)
{
    char *pos, *part;
    size_t size, len;

    if (str == NULL) {
        return NULL;
    }

    pos = strrchr(str, ch);

    if (!pos) {
        return NULL;
    }

    size = pos - str;
    len = strlen(str) - size;
    part = malloc(len);
    memcpy(part, &str[size + 1], len);

    return part;
}


void strTrim(char *str)
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


void strCpy(char *src, const char *ori)
{
    snprintf(src, strlen(ori) + 1, "%s", ori);
}
