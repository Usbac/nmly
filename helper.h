#ifndef HELPER_H_
#define HELPER_H_

/**
 * Copy a portion of the specified string into a new one.
 * @param sub the array where the substring will be stored.
 * @param str the original string.
 * @param start the starting position.
 * @param len the number of characters to copy.
 */
void substr(char *sub, char *str, int start, int len);

/**
 * Returns a substring from the beginning to the last specified character.
 * @param str the original string.
 * @param character the specified character.
 * @return a substring from the beginning to the last specified character.
 */
char *strBefore(char *str, char character);

/**
 * Returns a substring from the last specified character to the end of the given string.
 * @param str the original string.
 * @param character the specified character.
 * @return a substring from the last specified character to the end of the given string.
 */
char *strAfter(char *str, char character);

/**
 * Returns the filename with the specified text appended at the beginning.
 * @param str the file directory.
 * @param filename the filename.
 * @return the filename with the specified text appended at the beginning.
 */
char *before(char *dir, char *filename, char *text);

/**
 * Returns the filename with the specified text appended at the end.
 * @param dir the file directory.
 * @param filename the filename.
 * @return the filename with the specified text appended at the end.
 */
char *after(char *dir, char *filename, char *text);

/**
 * Returns the filename with the text reversed.
 * @param dir the file directory.
 * @param filename the filename.
 * @return the filename with the text reversed.
 */
char *reverse(char *dir, char *filename);

/**
 * Returns the filename with all in lowercase or uppercase.
 * @param dir the file directory.
 * @param filename the filename.
 * @param upper 1 for uppercase, any other int for lowercase.
 * @return the filename with all in lowercase or uppercase.
 */
char *changeCases(char *dir, char *filename, int upper);

/**
 * Returns the filename with the text in uppercase.
 * @param dir the file directory.
 * @param filename the filename.
 * @return the filename with the text in uppercase.
 */
char *upper(char *dir, char *filename);

/**
 * Returns the filename with the text in lowercase.
 * @param dir the file directory.
 * @param filename the filename.
 * @return the filename with the text in lowercase.
 */
char *lower(char *dir, char *filename);

/**
 * Returns the filename with the specified text replaced by a new one.
 * @param dir the file directory.
 * @param filename the filename.
 * @param ori the text to replace.
 * @param rep the new text.
 * @return the filename with the specified text replaced by a new one.
 */
char *replace(char *dir, char *filename, char *ori, char *rep);

/**
 * Returns a new string with the specified text replaced by a new one.
 * @param str the string.
 * @param ori the text to replace.
 * @param rep the new text.
 * @return a string with the specified text replaced by a new one.
 */
char *strReplace(char *str, char *ori, char *rep);

/**
 * Save into a string two switched parts separated by a separator. 
 * @param new the new string.
 * @param sep the separator.
 * @param part_one the first part.
 * @param part_two the second part.
 */
void doSwitch(char *new, char *sep, char *part_one, char *part_two);

/**
 * Returns a switched filename based in a separator.
 * @param dir the file directory.
 * @param filename the filename.
 * @param sep the separator.
 * @return a switched filename based in a separator.
 */
char *switchSides(char *dir, char *filename, char sep);

#endif // HELPER_H_
