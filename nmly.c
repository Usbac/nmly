#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include "nmly.h"
#include "helper.h"

#define BEFORE 0
#define AFTER 1
#define REPLACE 2
#define UPPER 3
#define LOWER 4
#define SWITCH 5
#define REVERSE 6
#define REMOVE 7

const char *preview_msg = "\n%i File(s) to be modified in %i folder(s)\n%f Segs\n";
const char *success_msg = "\n%i File(s) modified in %i folder(s)\n%f Segs\n";
const char *error_msg = "Error opening directory\n";
char *working_path = ".";
char *version = "version 0.9";
int files_n = 0;
int folders_n = 0;
int option = 0;
int preview = 0;
int recursive = 0;
int modify_folders = 0;


int is_file(const char* path) 
{
    struct stat buf;
    stat(path, &buf);

    return S_ISREG(buf.st_mode);
}


int is_dir(const char* path) 
{
    struct stat buf;
    stat(path, &buf);

    return S_ISDIR(buf.st_mode);
}


char *getChanges(char *path, char *argv[]) 
{
	char *dir = strBefore(path, '/');
	char *filename = strAfter(path, '/');

	switch (option) {
		case BEFORE: return before(dir, filename, argv[2]);
			break;
		case AFTER: return after(dir, filename, argv[2]);
			break;
		case UPPER: return upper(dir, filename);
			break;
		case LOWER: return lower(dir, filename);
			break;
		case SWITCH: return switchSides(dir, filename, argv[2][0]);
			break;
		case REVERSE: return reverse(dir, filename);
			break;
		case REPLACE: return replace(dir, filename, argv[2], argv[3]);
			break;
		case REMOVE: return replace(dir, filename, argv[2], "");
			break;
	}

	return path;
}


int listDir(char *basedir, char *argv[]) 
{
	DIR *dir;
	char b[512];
	struct dirent *ent;

	if((dir = opendir(basedir)) == NULL) {
		return -1;
	}

	while((ent = readdir(dir)) != NULL) {
		//Avoid current and previous folders
		if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
			continue;
		}

		char *entpath = malloc((strlen(basedir) + strlen(ent->d_name) + 2) * sizeof(char));
		strcpy(entpath, basedir);
		strcat(entpath, "/");
		strcat(entpath, ent->d_name);

		if (recursive && is_dir(entpath)) {
			if (listDir(entpath, argv) == -1) {
				free(entpath);
				return -1;
			}

			folders_n++;
		} else if (is_file(entpath) || (is_dir(entpath) && modify_folders)) {
			char *new_path = getChanges(entpath, argv);

			if (new_path == NULL) {
				continue;
			}

			files_n++;

			if (!preview) {
				rename(entpath, new_path);
			} else {
				printf("%s > %s \n", entpath, new_path);
			}	
		}
		
		free(entpath);
	}

	closedir(dir);

	return 0;
}


int mapArgs(int argc, char *argv[]) 
{
	size_t i;
	for (i = 0; i < argc; i++) {
		//Recursive
		if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--recursive") == 0) {
			recursive = 1;
		}

		//Preview
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--preview") == 0) {
			preview = 1;
		}
		
		//Modify folders
		if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--folders") == 0) {
			modify_folders = 1;
		}

		//Locale (special characters)
		if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--locale") == 0) {
			setlocale(LC_ALL, "");
		}

		//Help
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			help();
			return 1;
		}

		//Version
		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
			printf("%s\n", version);
			return 1;
		}

		if (i+1 > argc) {
			return 0;
		}

		//Working path
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--directory") == 0) {
			working_path = malloc(strlen(argv[++i]) * sizeof(char));
			strcpy(working_path, argv[i]);
		}
	}

	//Options
	if (strcmp(argv[1], "before") == 0) {
		option = BEFORE;
	}
	if (strcmp(argv[1], "after") == 0) {
		option = AFTER;
	}
	if (strcmp(argv[1], "replace") == 0) {
		option = REPLACE;
	}
	if (strcmp(argv[1], "upper") == 0) {
		option = UPPER;
	}
	if (strcmp(argv[1], "lower") == 0) {
		option = LOWER;
	}
	if (strcmp(argv[1], "switch") == 0) {
		option = SWITCH;
	}
	if (strcmp(argv[1], "reverse") == 0) {
		option = REVERSE;
	}
	if (strcmp(argv[1], "remove") == 0) {
		option = REMOVE;
	}

	return 0;
}


void help()
{
	printf(
		"USAGE \n\n"
		"$ nmly [Arg] [options...]\n\n"
		"ARGUMENTS\n\n"
		"after [text]         Add the text at the end of the filenames\n"
		"before [text]        Add the text at the begining of the filenames\n"
		"lower                All filename characters to lowercase\n"
		"remove [text]        Remove the specified text from the filename\n"
		"replace [ori] [new]  Replace a text with a new one\n"
		"reverse              Reverse the filename\n"
		"switch [sep]         Switch the filename order based in a separator\n"
		"upper                All filename characters to uppercase\n\n"
		"OPTIONS\n\n"
		"-d --directory  The directory where the changes will be applied\n"
		"-f --folders    Apply the changes to the folders name too\n"
		"-h --help       Get help and information about the application\n"
		"-l --locale     Accept special characters (like latin characters)\n"
		"-p --preview    Show the changes without applying them\n"
		"-r --recursive  Apply the changes recursively in the directory\n"
		"-v --version    Show the application version\n\n"
		"EXAMPLES\n\n"
		"$ nmly switch - -d ./\n"
		"Author - Song.mp3 > Song - Author.mp3\n\n"
		"$ nmly replace jpeg jpg -d ./\n"
		"picture.jpeg > picture.jpg\n\n"
		"$ nmly after world -d ./ -r\n"
		"hello.pdf > helloworld.pdf\n"
		"subfolder/file.txt > subfolder/fileworld.txt \n"
	);
}


int main(int argc, char *argv[]) 
{
	if (mapArgs(argc, argv) == 1) {
		return 0;
	}

	float start_time = (float)clock() / CLOCKS_PER_SEC;

	if (listDir(working_path, argv) == -1) {
		printf(error_msg);
		return -1;
	}

	float total_time = ((float)clock() / CLOCKS_PER_SEC) - start_time;

	const char *msg = (preview == 1)? preview_msg : success_msg;
	printf(msg, files_n, folders_n, total_time);
	
	return 0;
}
