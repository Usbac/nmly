#ifndef NMLY_H_
#define NMLY_H_

/**
 * Returns 1 if the specified path is a file, 0 otherwise.
 * @param path the path.
 * @return 1 if the specified path is a file, 0 otherwise.
 */
int is_file(const char* path);

/**
 * Returns 1 if the specified path is a directory, 0 otherwise.
 * @param path the path.
 * @return 1 if the specified path is a directory, 0 otherwise.
 */
int is_dir(const char* path);

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
 */
void processFile(char *entpath, char *argv[]);

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
 * @param test blah blah parameter.
 * @return 0 if blah blah passed.
 */
void help();

#endif // NMLY_H_
