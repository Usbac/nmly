#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

/**
 * Reverses the given string.
 * @param str the string.
 */
void strReverse(char *str);

/**
 * Returns the string with the specified substring
 * replaced.
 * @param str the string.
 * @param old the original substring.
 * @param rep the replacement substring.
 * @return the string with the specified substring
 * replaced.
 */
char *strReplace(const char *str,
                 const char *old,
                 const char *rep);

/**
 * Returns all the string characters to
 * uppercase or lowercase.
 * @param str the string.
 * @param upper true for uppercase, false for lowercase.
 * @return all the string characters to
 * uppercase or lowercase.
 */
char *strCases(const char *str, bool upper);

/**
 * Returns everything before the given char
 * in the string.
 * @param str the string.
 * @param ch the character.
 * @return everything before the given char.
 */
char *strBefore(const char *str, char ch);

/**
 * Returns everything after the given char
 * in the string.
 * @param str the string.
 * @param ch the character.
 * @return everything after the given char.
 */
char *strAfter(const char *str, char ch);

/**
 * Removes the whitespaces at both sides
 * of the given string.
 * @param src the string.
 */
void strTrim(char *str);

/**
 * Just a safer strcpy function.
 * @param src the source string.
 * @param ori the original string.
 */
void strCpy(char *src, const char *ori);

#endif /* STRING_H */
