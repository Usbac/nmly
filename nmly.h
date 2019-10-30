#ifndef NMLY_H_
#define NMLY_H_

#define KBYTE 1024
#define EMPTY ""
#define SINGLE_MSG "%s\n"
#define PREVIEW_MSG "\n%i File(s) to be modified in %i folder(s)"
#define SUCCESS_MSG "\n%i File(s) modified in %i folder(s)"
#define FILES_ERROR_MSG "\n%i File(s) cannot be modified. Maybe check your permissions?"
#define DIR_ERROR_MSG "Cannot open directory %s\n"
#define SPLIT_DIR_ERROR_MSG "Cannot open directory %s\n\n"
#define DIR_CONFIRM_MSG "Apply the changes in the following directory '%s'? [Y/n] "
#define COMPARE_MSG "%s > %s \n"
#define SPLIT_COMPARE_MSG "%s\n%s\n\n"
#define TIME_MSG "\n%f Segs\n"
#define ARG_ERROR_MSG "Error: Invalid command\n"
#define VERSION_MSG "Nmly v0.9.10\n"
#define HELP_USAGE_MSG "USAGE \n\n" \
		"$ nmly [Arg] [options...]\n\n" \
		"ARGUMENTS\n\n" \
		"after [text]         Add text at the end of the filenames\n" \
		"before [text]        Add text at the begining of the filenames\n" \
		"lower                All filename characters to lowercase\n" \
		"remove [text]        Remove the specified text from the filename\n" \
		"replace [ori] [new]  Replace a text with a new one\n" \
		"reverse              Reverse the filename\n" \
		"switch [sep]         Switch the filename order based in a separator\n" \
		"upper                All filename characters to uppercase"
#define HELP_OPTIONS_MSG "\n\nOPTIONS\n\n" \
		"-d --directory [txt]  The target directory\n" \
		"-e --extension [txt]  Filter files based on theirs extension\n" \
		"-f --folders          Apply changes to the folders name too\n" \
		"-h --help             Get help and information\n" \
		"-l --locale           Process special characters (like latin ones)\n" \
		"-p --preview          Show the changes without applying them\n" \
		"-r --recursive        Work recursively\n" \
		"-s --size [size]      Filter files based on theirs size (+/-)(g/m/k)\n" \
		"   --split            Display the changes horizontally split\n" \
		"-u --unmodifiable     Show non-modifiable files\n" \
		"-v --version          Show the application version"
#define HELP_EXAMPLES_MSG "\n\nEXAMPLES\n\n" \
		"$ nmly switch -\n" \
		"  Author - Song.mp3 > Song - Author.mp3\n" \
		"$ nmly remove ' 2017' -d vacations -e mp4\n" \
		"  vacations/video 2017.mp4 > vacations/video.mp4\n" \
		"$ nmly reverse -d folder -s +1g\n" \
		"  folder/fileBiggerThan1GB.iso > folder/BG1nahTreggiBelif.iso\n" \
		"$ nmly replace jpeg jpg\n" \
		"  picture.jpeg > picture.jpg\n" \
		"$ nmly after world -r\n" \
		"  hello.pdf > helloworld.pdf\n" \
		"  subdir/file.txt > subdir/fileworld.txt\n" \
		"$ nmly upper -r --split\n" \
		"  hello.pdf\n" \
		"  HELLO.pdf\n" \
		"$ nmly -u -d folder -r\n" \
		"  folder/filewithpermissions.txt\n"

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
 * Stores in the specified path with the changes applied 
 * in the given directory.
 * @param new_path the new path.
 * @param file the file path.
 * @param argv the arguments array.
 */
void getChanges(char **new_path, char *file, char *argv[]);

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
 * Returns 1 if the given file matches the filters,
 * 0 otherwise
 * @param entpath the file path.
 */
int matchesFilters(char *entpath);

/**
 * Parse the file size based filter.
 * @param str the file size.
 */
void parseSizeArgs(char *str);

/**
 * Display the help messages.
 */
void printHelp(void);

/**
 * Map all the arguments.
 * @param argc the number of arguments.
 * @param argv the arguments array.
 * @return 1 if the normal program flow
 * must be stopped after this function, 0 otherwise.
 */
int mapArgs(int argc, char *argv[]);

#endif /* NMLY_H_ */
