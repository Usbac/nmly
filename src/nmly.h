#ifndef NMLY_H_
#define NMLY_H_

#define KBYTE 1024
#define MBYTE 1024 * 1024
#define GBYTE 1024 * 1024 * 1024
#define MSG_PREVIEW "\n%i File(s) to be modified in %i folder(s)"
#define MSG_SUCCESS "\n%i File(s) modified in %i folder(s)"
#define ERROR_FILES "\n%i File(s) cannot be modified. Maybe check your permissions?"
#define ERROR_DIR "Cannot open directory %s\n"
#define ERROR_SPLIT_DIR "Cannot open directory %s\n\n"
#define ERROR_ARG "Error: Invalid command\n"
#define ERROR_UNDEFINED_DIR "Error: Undefined directory\n"
#define ERROR_EXTENSION "Error: Undefined extension filter\n"
#define ERROR_SIZE "Error: Undefined size filter\n"
#define MSG_DIR_CONFIRM "Apply the changes in the following directory '%s'? [Y/n] "
#define MSG_COMPARE "%s > %s\n"
#define MSG_SPLIT_COMPARE "%s\n%s\n\n"
#define MSG_VERSION "NMLY v1.1.1\n"
#define MSG_HELP "USAGE \n\n" \
        "$ nmly [Function] [options...]\n\n" \
        "FUNCTIONS\n\n" \
        "after [text]         Add text at the end of the filenames\n" \
        "before [text]        Add text at the beginning of the filenames\n" \
        "lower                All filename characters to lowercase\n" \
        "remove [text]        Remove the specified text from the filename\n" \
        "replace [ori] [new]  Replace a text with a new one\n" \
        "reverse              Reverse the filename\n" \
        "switch [sep]         Switch the filename order based in a separator\n" \
        "upper                All filename characters to uppercase"
#define MSG_HELP_OPTIONS "\n\nOPTIONS\n\n" \
        "-d --directory [txt]  The target directory\n" \
        "-e --extension [txt]  Filter files based on theirs extension\n" \
        "-f --folders          Apply changes to the folders name too\n" \
        "-h --help             Get help and information\n" \
        "-l --locale           Process special characters (like latin ones)\n" \
        "-nv --no-verbose      No verbose mode (no files will be listed)\n" \
        "-p --preview          Show the changes without applying them\n" \
        "-r --recursive        Work recursively\n" \
        "-s --size [size]      Filter files based on theirs size (+/-)(g/m/k)\n" \
        "   --split            Display the changes horizontally split\n" \
        "-u --unmodifiable     Show non-modifiable files\n" \
        "-v --version          Show the application version"
#define MSG_HELP_EXAMPLES "\n\nEXAMPLES\n\n" \
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

#endif /* NMLY_H_ */
