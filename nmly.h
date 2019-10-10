#ifndef NMLY_H_
#define NMLY_H_

#define KBYTE 1024
#define SINGLE_MSG "%s\n"
#define PREVIEW_MSG "\n%i File(s) to be modified in %i folder(s)"
#define SUCCESS_MSG "\n%i File(s) modified in %i folder(s)"
#define FILES_ERROR_MSG "\n%i File(s) cannot be modified. Maybe check your permissions?"
#define DIR_ERROR_MSG "Cannot open directory %s\n"
#define DIR_CONFIRM_MSG "Apply the changes in the following directory '%s'? [Y/n] "
#define COMPARE_MSG "%s > %s \n"
#define TIME_MSG "\n%f Segs\n"
#define ARG_ERROR_MSG "Error: Invalid command\n"
#define VERSION_MSG "Nmly v0.9.7\n"
#define BEFORE 0
#define AFTER 1
#define REPLACE 2
#define UPPER 3
#define LOWER 4
#define SWITCH 5
#define REVERSE 6
#define REMOVE 7

/**
 * Returns 1 if the specified path is a file, 0 otherwise.
 * @param path the path.
 * @return 1 if the specified path is a file, 0 otherwise.
 */
int isFile(const char* path);

/**
 * Returns 1 if the specified path is a directory, 0 otherwise.
 * @param path the path.
 * @return 1 if the specified path is a directory, 0 otherwise.
 */
int isDir(const char* path);

/**
 * Returns the size of the given file path, 
 * or 0 in case of errors.
 * @param path the file path.
 * @return the size of the given file path, 
 * or 0 in case of errors.
 */
unsigned long getFileSize(const char* path);

/**
 * Returns 1 if the given file matches the file size filter,
 * 0 otherwise.
 * @param path the file path.
 * @return 1 if the given file matches the file size filter,
 * 0 otherwise.
 */
int sizeFilter(char *path);

/**
 * Returns the specified path with the changes applied.
 * @param path the path.
 * @param argv the arguments array.
 * @return the specified path with the changes applied.
 */
char *getChanges(char *path, char *argv[]);

/**
 * Apply or preview the changes over the specified directory.
 * @param basedir the directory.
 * @param argv the arguments array.
 */
void listDir(char *basedir, char *argv[]);

/**
 * Process the given file 
 * @param entpath the file path.
 * @param argv the arguments array.
 */
void processFile(char *entpath, char *argv[]);

/**
 * Parse the file size based filter.
 * @param str the file size.
 */
void parseSizeArgs(char *str);

/**
 * Map all the arguments.
 * @param argc the number of arguments.
 * @param argv the arguments array.
 * @return 1 if the normal program flow
 * must be stopped after this function, 0 otherwise.
 */
int mapArgs(int argc, char *argv[]);

/**
 * Display the help text.
 */
void help();

#endif // NMLY_H_
