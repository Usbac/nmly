#ifndef HELPER_H_
#define HELPER_H_
#define BUFFER 512
#define MSG_MEM_ERROR "Could not allocate memory!"

#if defined(_WIN32) || defined(WIN32)
/**
 * Reference to the snprintf function of the stdio library.
 * Because you know, some compilers are not compliant with
 * the C standard.
 */
int snprintf(char *buf, size_t size, const char *fmt, ...);
#endif

/**
 * Just a safer malloc function.
 * @param size the memory to allocate.
 */
void *malloc_(size_t size);

/**
 * Just a safer strcpy function.
 * @param src the source string.
 * @param ori the original string.
 */
void strcpy_(char *src, const char *ori);

/**
 * Concatenate two paths into one.
 * @param src the source string.
 * @param path_one the first path.
 * @param path_two the second path.
 */
void concatPath(char *src, const char *path_one, const char *path_two);

/**
 * Stores in the source string the filename with the specified
 * text appended at the beginning.
 * @param src the source string.
 * @param file the file path.
 * @param text the text to append.
 */
void before(char **src, const char *file, const char *text);

/**
 * Returns a substring from the last specified character to the end of the given string.
 * @param str the original string.
 * @param ch the specified character.
 * @return a substring from the last specified character to the end of the given string.
 */
char *strAfter(const char *str, char ch);

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
 * Stores in the source string a switched filename based in a separator.
 * @param src the source string.
 * @param file the file path.
 * @param sep the separator.
 */
void switchSides(char **src, const char *file, char sep);

#endif /* HELPER_H_ */
