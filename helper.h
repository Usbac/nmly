#ifndef HELPER_H_
#define HELPER_H_
#define BUFFER 512
#define ERROR_MEM_MSG "Could not allocate memory!"

/**
 * Just a safer malloc function.
 */
static void *malloc_(size_t size)
{
    void *alloc_mem = malloc(size);
    if (alloc_mem == NULL) {
        printf(ERROR_MEM_MSG);
        exit(-1);
    }

    return alloc_mem;
}

/**
 * Reference to the snprintf function of the stdio library.
 * Because you know, some compilers are not compliant with
 * the C standard.
 */
int snprintf(char *buf, size_t size, const char *fmt, ...);

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
void trim(char *str);

/**
 * Returns the string reversed.
 * @param str the string.
 * @return the string reversed.
 */
char *strrev(char *str);

/**
 * Returns a substring from the beginning to the last specified character.
 * @param str the original string.
 * @param ch the specified character.
 * @return a substring from the beginning to the last specified character.
 */
char *strBefore(const char *str, const char ch);

/**
 * Returns a substring from the last specified character to the end of the given string.
 * @param str the original string.
 * @param ch the specified character.
 * @return a substring from the last specified character to the end of the given string.
 */
char *strAfter(const char *str, const char ch);

/**
 * Stores in the source string the filename with the specified 
 * text appended at the beginning.
 * @param src the source string.
 * @param file the file path.
 * @param text the text to append.
 */
void before(char **src, const char *file, const char *text);

/**
 * Stores in the source string the filename with the specified text appended at the end.
 * @param src the source string.
 * @param file the file path.
 * @param text the text to append.
 */
void after(char **src, const char *file, const char *text);

/**
 * Stores in the source string the filename with the text reversed.
 * @param src the source string.
 * @param file the file path.
 */
void reverse(char **src, const char *file);

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
 * @param filename the file path.
 * @param upper 1 for uppercase, any other int for lowercase.
 */
void changeCases(char **src, const char *file, const int upper);

/**
 * Stores in the source string the filename with the text in uppercase.
 * @param src the source string.
 * @param dir the file path.
 */
void upper(char **src, const char *file);

/**
 * Stores in the source string the filename with the text in lowercase.
 * @param src the source string.
 * @param dir the file path.
 */
void lower(char **src, const char *file);

/**
 * Stores in the source string the filename with the specified 
 * text replaced by a new one.
 * @param src the source string.
 * @param file the file path.
 * @param ori the text to replace.
 * @param rep the new text.
 */
void replace(char **src, const char *file, const char *ori, const char *rep);

/**
 * Stores in the source string a new string with the specified 
 * text replaced by a new one.
 * @param src the source string.
 * @param str the original string.
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
 * @param file the file path.
 * @param sep the separator.
 */
void switchSides(char **src, const char *file, const char sep);

#endif /* HELPER_H_ */
