#ifndef NMLY_H_
#define NMLY_H_

#define BILLION 1e9
#define KBYTE 1024
#define PREVIEW_MSG "\n%i File(s) to be modified in %i folder(s)"
#define SUCCESS_MSG "\n%i File(s) modified in %i folder(s)"
#define FILES_ERROR_MSG "\n%i File(s) cannot be modified. Maybe check your permissions?"
#define DIR_ERROR_MSG "Cannot open directory %s\n"
#define SPLIT_DIR_ERROR_MSG "Cannot open directory %s\n\n"
#define DIR_CONFIRM_MSG "Apply the changes in the following directory '%s'? [Y/n] "
#define COMPARE_MSG "%s > %s\n"
#define SPLIT_COMPARE_MSG "%s\n%s\n\n"
#define TIME_MSG "\n%lf Segs\n"
#define ARG_ERROR_MSG "Error: Invalid command\n"
#define DIRECTORY_ERROR_MSG "Error: Undefined directory\n"
#define EXTENSION_ERROR_MSG "Error: Undefined extension filter\n"
#define SIZE_ERROR_MSG "Error: Undefined size filter\n"
#define VERSION_MSG "NMLY v1.0.3\n"
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
		"-nv --no-verbose      No verbose mode (no files will be listed)\n" \
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

#endif /* NMLY_H_ */
