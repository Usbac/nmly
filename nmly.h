#ifndef NMLY_H_
#define NMLY_H_

#define KBYTE 1024
#define EMPTY ""
#define SINGLE_MSG "%s\n"
#define PREVIEW_MSG "\n%i File(s) to be modified in %i folder(s)"
#define SUCCESS_MSG "\n%i File(s) modified in %i folder(s)"
#define FILES_ERROR_MSG "\n%i File(s) cannot be modified. Maybe check your permissions?"
#define DIR_ERROR_MSG "Cannot open directory %s\n"
#define DIR_CONFIRM_MSG "Apply the changes in the following directory '%s'? [Y/n] "
#define COMPARE_MSG "%s > %s \n"
#define SPLIT_COMPARE_MSG "%s\n%s\n\n"
#define TIME_MSG "\n%f Segs\n"
#define ARG_ERROR_MSG "Error: Invalid command\n"
#define VERSION_MSG "Nmly v0.9.8\n"
#define HELP_MSG "USAGE \n\n" \
		"$ nmly [Arg] [options...]\n\n" \
		"ARGUMENTS\n\n" \
		"after [text]         Add text at the end of the filenames\n" \
		"before [text]        Add text at the begining of the filenames\n" \
		"lower                All filename characters to lowercase\n" \
		"remove [text]        Remove the specified text from the filename\n" \
		"replace [ori] [new]  Replace a text with a new one\n" \
		"reverse              Reverse the filename\n" \
		"switch [sep]         Switch the filename order based in a separator\n" \
		"upper                All filename characters to uppercase\n\n" \
		"OPTIONS\n\n" \
		"-d --directory [text]  The directory where the changes will be applied\n" \
		"-e --extension [text]  Apply changes only to the files with that extension\n" \
		"-f --folders           Apply changes to the folders name too\n" \
		"-h --help              Get help and information about the application\n" \
		"-l --locale            Accept special characters (like latin characters)\n" \
		"-p --preview           Show the changes without applying them\n" \
		"-r --recursive         Apply the changes recursively in the directory\n" \
		"-s --size [size]       Apply changes only to the files with specified filesize (+/-)(g/m/k)\n" \
        "   --split             Display the changes horizontally split\n" \
		"-u --unmodifiable      Show the files that cannot be modified\n" \
		"-v --version           Show the application version\n\n" \
		"EXAMPLES\n\n" \
		"$ nmly switch - -d ./\n" \
		"Author - Song.mp3 > Song - Author.mp3\n\n" \
		"$ nmly remove ' 2017' -d ./vacations -e mp4\n" \
		"./vacations/video 2017.mp4 > ./vacations/video.mp4\n\n" \
		"$ nmly -d ./folder -s +1g\n" \
		"./folder/fileBiggerThan1GB.iso\n\n" \
		"$ nmly replace jpeg jpg -d ./\n" \
		"picture.jpeg > picture.jpg\n\n" \
		"$ nmly after world -d ./ -r\n" \
		"hello.pdf > helloworld.pdf\n" \
		"subfolder/file.txt > subfolder/fileworld.txt\n\n" \
        "$ nmly upper -d ./ -r --split\n" \
        "hello.pdf\n" \
        "HELLO.pdf\n\n" \
        "subfolder/file.txt\n" \
        "subfolder/FILE.txt\n\n" \
		"$ nmly -u -d ./folder -r\n" \
		"./folder/filewithpermissions.txt\n"
        
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

#endif // NMLY_H_
