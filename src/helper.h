#ifndef HELPER_H_
#define HELPER_H_
#define ERROR_ALLOC_MEM "Could not allocate memory!"

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
 * Returns two paths joined into one.
 * @param path_one the first path.
 * @param path_two the second path.
 * @return two paths joined into one.
 */
char *joinPaths(const char *path_one, const char *path_two);

/**
 * Returns the filename with the specified
 * text prepended.
 * @param file the file path.
 * @param text the text to append.
 * @return the filename with the specified
 * text prepended.
 */
char *before(const char *file, const char *text);

/**
 * Returns a substring from the last specified character to the end of the given string.
 * @param str the original string.
 * @param ch the specified character.
 * @return a substring from the last specified character to the end of the given string.
 */
char *strAfter(const char *str, char ch);

/**
 * Returns the filename with the specified text appended.
 * @param file the file path.
 * @param text the text to append.
 * @return the filename with the specified text appended.
 */
char *after(const char *file, const char *text);

/**
 * Returns the filename with the text reversed.
 * @param file the file path.
 * @return the filename with the text reversed.
 */
char *reverse(const char *file);

/**
 * Returns the filename with the text in uppercase.
 * @param dir the file path.
 * @return the filename with the text in uppercase.
 */
char *upper(const char *file);

/**
 * Returns the filename with the text in lowercase.
 * @param dir the file path.
 * @return the filename with the text in lowercase.
 */
char *lower(const char *file);

/**
 * Returns the filename with the specified
 * text replaced by a new one.
 * @param file the file path.
 * @param ori the text to replace.
 * @param rep the new text.
 * @return the filename with the specified
 * text replaced by a new one.
 */
char *replace(const char *file, const char *ori, const char *rep);

/**
 * Returns the switched filename based in a separator.
 * @param file the file path.
 * @param sep the separator.
 * @return the switched filename based in a separator.
 */
char *switchSides(const char *file, char sep);

#endif /* HELPER_H_ */
