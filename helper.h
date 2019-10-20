#ifndef HELPER_H_
#define HELPER_H_
#define BUFFER 512

/**
 * Copy a portion of the specified string into a new one.
 * @param sub the array where the substring will be stored.
 * @param str the original string.
 * @param start the starting position.
 * @param len the number of characters to copy.
 */
void substr(char *sub, const char *str, const int start, const int len);

/**
 * Remove extra whitespaces at the beginning and end of a string.
 * @param str the string.
 */
void *trim(char *str);

/**
 * Returns the string reversed.
 * @param str the string.
 * @return the string reversed.
 */
char *strrev(char *str);

/**
 * Returns a substring from the beginning to the last specified character.
 * @param str the original string.
 * @param character the specified character.
 * @return a substring from the beginning to the last specified character.
 */
char *strBefore(const char *str, const char character);

/**
 * Returns a substring from the last specified character to the end of the given string.
 * @param str the original string.
 * @param character the specified character.
 * @return a substring from the last specified character to the end of the given string.
 */
char *strAfter(const char *str, const char character);

/**
 * Stores in the source string the filename with the specified text appended at the beginning.
 * @param src the source string.
 * @param str the file directory.
 * @param filename the filename.
 */
void before(char **src, const char *dir, const char *filename, const char *text);

/**
 * Stores in the source string the filename with the specified text appended at the end.
 * @param dir the file directory.
 * @param filename the filename.
 */
void after(char **src, const char *dir, const char *filename, const char *text);

/**
 * Stores in the source string the filename with the text reversed.
 * @param src the source string.
 * @param dir the file directory.
 * @param filename the filename.
 */
void reverse(char **src, const char *dir, char *filename);

/**
 * Change a string to lowercase or uppercase.
 * @param dest the destiny string.
 * @param str the original string.
 * @param upper 1 for uppercase, any other int for lowercase.
 */
void strCases(char *dest, const char *str, const int upper);

/**
 * Stores in the source string the filename with all in lowercase or uppercase.
 * @param src the source string.
 * @param dir the file directory.
 * @param filename the filename.
 * @param upper 1 for uppercase, any other int for lowercase.
 */
void changeCases(char **src, const char *dir, const char *filename, const int upper);

/**
 * Stores in the source string the filename with the text in uppercase.
 * @param src the source string.
 * @param dir the file directory.
 * @param filename the filename.
 */
void upper(char **src, const char *dir, const char *filename);

/**
 * Stores in the source string the filename with the text in lowercase.
 * @param src the source string.
 * @param dir the file directory.
 * @param filename the filename.
 */
void lower(char **src, const char *dir, const char *filename);

/**
 * Stores in the source string the filename with the specified text replaced by a new one.
 * @param src the source string.
 * @param dir the file directory.
 * @param filename the filename.
 * @param ori the text to replace.
 * @param rep the new text.
 */
void replace(char **src, const char *dir, const char *filename, const char *ori, const char *rep);

/**
 * Stores in the source string a new string with the specified text replaced by a new one.
 * @param str the string.
 * @param ori the text to replace.
 * @param rep the new text.
 */
int strReplace(char **src, const char *str, const char *ori, const char *rep);

/**
 * Save into a string two switched parts separated by a separator. 
 * @param new the new string.
 * @param sep the separator.
 * @param part_one the first part.
 * @param part_two the second part.
 */
void doSwitch(char *new, const char *sep, char *part_one, char *part_two);

/**
 * Stores in the source string a switched filename based in a separator.
 * @param src the source string.
 * @param dir the file directory.
 * @param filename the filename.
 * @param sep the separator.
 */
void switchSides(char **src, const char *dir, const char *filename, const char sep);

#endif // HELPER_H_
