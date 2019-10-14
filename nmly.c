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

char *working_path = ".";
char *filter = EMPTY;
int files_n = 0, folders_n = 0, files_error_n = 0;
int option = 0;
int split_view = 0;
int preview = 0, preview_unmodifiable = 0;
int recursive = 0;
int modify_folders = 0;
long size_filter = -1;
enum SIZE_TYPE {
	LT,
	GT,
	EQ
};
enum SIZE_TYPE size_type_filter;


int isFile(const char* path) 
{
	struct stat buf;
	if (stat(path, &buf) < 0) {
		return 0;
	}

	return S_ISREG(buf.st_mode);
}


int isDir(const char* path) 
{
	struct stat buf;
	if (stat(path, &buf) < 0) {
		return 0;
	}

	return S_ISDIR(buf.st_mode);
}


unsigned long getFileSize(const char* path)
{
	struct stat buf;
	if (stat(path, &buf) < 0) {
		return 0;
	}

	return buf.st_size;
}


int sizeFilter(char *path) {
	unsigned long filesize = getFileSize(path);

	switch(size_type_filter) {
		case GT:
			return filesize >= size_filter;
			break;
		case LT:
			return filesize < size_filter;
			break;
		case EQ:
			return filesize == size_filter;
	}

	return 0;
}


char *getChanges(char *path, char *argv[]) 
{
	char *dir = strBefore(path, '/');
	char *filename = strAfter(path, '/');
	char *new_path = NULL;
	
	//Matches filter by extension
	char *extension = strAfter(filename, '.');
	if (filter[0] != '\0' && (extension == NULL || strcmp(extension, filter))) {
		return NULL;
	}

	//Matches filter by file size
	if (size_filter != -1 && !sizeFilter(path)) {
		return NULL;
	}

	switch (option) {
		case BEFORE: new_path = before(dir, filename, argv[2]);
			break;
		case AFTER: new_path = after(dir, filename, argv[2]);
			break;
		case UPPER: new_path = upper(dir, filename);
			break;
		case LOWER: new_path = lower(dir, filename);
			break;
		case SWITCH: new_path = switchSides(dir, filename, argv[2][0]);
			break;
		case REVERSE: new_path = reverse(dir, filename);
			break;
		case REPLACE: new_path = replace(dir, filename, argv[2], argv[3]);
			break;
		case REMOVE: new_path = replace(dir, filename, argv[2], EMPTY);
			break;
	}

	free(dir);
	free(filename);

	return new_path;
}


void listDir(char *basedir, char *argv[])
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(basedir)) == NULL) {
		printf(preview_unmodifiable ? SINGLE_MSG : DIR_ERROR_MSG, basedir);
		files_error_n++;
		return;
	}

	while ((ent = readdir(dir)) != NULL) {
		//Avoid current and previous folders
		if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
			continue;
		}

		int length = (strlen(basedir) + strlen(ent->d_name) + 2) * sizeof(char);
		char *entpath = malloc(length);
		snprintf(entpath, length, "%s/%s", basedir, ent->d_name);

		if (isFile(entpath) || (isDir(entpath) && modify_folders)) {
			processFile(entpath, argv);
		}

		if (recursive && isDir(entpath)) {
			listDir(entpath, argv);
			folders_n++;
		}
		
		free(entpath);
	}

	closedir(dir);

	return;
}


void processFile(char *entpath, char *argv[])
{
	char *new_path = getChanges(entpath, argv);

	if (new_path == NULL) {
		free(new_path);
		
		return;
	}

	files_n++;

	if (!preview_unmodifiable) {
		if (!preview) {
			rename(entpath, new_path);
		}
	
		printf(split_view ? SPLIT_COMPARE_MSG : COMPARE_MSG, entpath, new_path);
	}

	free(new_path);
}


void parseSizeArgs(char *str) {
	char sign = str[0];
	char *suffix;

	switch (sign) {
		case '+':
			size_type_filter = GT;
			break;
		case '-':
			size_type_filter = LT;
			break;
		default:
			size_type_filter = EQ;
	}

	if (size_type_filter != EQ) {
		str++;
	}

	size_filter = strtoul(str, &suffix, 10);
	switch(*suffix) {
		case 'G':
		case 'g':
			size_filter = size_filter * KBYTE * KBYTE * KBYTE;
			break;
		case 'M':
		case 'm':
			size_filter = size_filter * KBYTE * KBYTE;
			break;
		case 'K':
		case 'k':
			size_filter = size_filter * KBYTE;
			break;
	}
}


int mapArgs(int argc, char *argv[]) 
{
	size_t i;
	
	//Show the help by default
	if (argc == 1) {
		printf(HELP_MSG);
		return 1;
	}

	for (i = 0; i < argc; i++) {
		//Recursive
		if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--recursive")) {
			recursive = 1;
		}

		//Preview
		if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--preview")) {
			preview = 1;
		}
		
		//Modify folders
		if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--folders")) {
			modify_folders = 1;
		}

		//Locale (special characters)
		if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--locale")) {
			setlocale(LC_ALL, EMPTY);
		}

		//Split view
		if (!strcmp(argv[i], "--split")) {
			split_view = 1;
		}

		//List unmodifiable files
		if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--unmodifiable")) {
			preview_unmodifiable = 1;
		}

		//Help
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			printf(HELP_MSG);
			return 1;
		}

		//Version
		if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf("%s", VERSION_MSG);
			return 1;
		}

		if (i+1 > argc) {
			return 0;
		}

		//Working path
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--directory")) {
			working_path = argv[++i];
		}

		//Extension filter
    		if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extension")) {
			filter = argv[++i];
		}

		//Size
		if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
			parseSizeArgs(argv[++i]);
		}
	}

	//Options
	if (!strcmp(argv[1], "before")) {
		option = BEFORE;
	} else if (!strcmp(argv[1], "after")) {
		option = AFTER;
	} else if (!strcmp(argv[1], "replace")) {
		option = REPLACE;
	} else if (!strcmp(argv[1], "upper")) {
		option = UPPER;
	} else if (!strcmp(argv[1], "lower")) {
		option = LOWER;
	} else if (!strcmp(argv[1], "switch")) {
		option = SWITCH;
	} else if (!strcmp(argv[1], "reverse")) {
		option = REVERSE;
	} else if (!strcmp(argv[1], "remove")) {
		option = REMOVE;
	} else if (!preview_unmodifiable) {
		printf(SINGLE_MSG, ARG_ERROR_MSG);
		return 1;
	}

	return 0;
}


int main(int argc, char *argv[]) 
{
	if (mapArgs(argc, argv)) {
		return 0;
	}
	float start_time = (float) clock() / CLOCKS_PER_SEC;

	//Confirmation
	if (!preview && !preview_unmodifiable) {
		char confirm;

		printf(DIR_CONFIRM_MSG, working_path);
		scanf("%c", &confirm);

		if (confirm != 'Y' && confirm != 'y' && confirm != '\n') {
			return 0;
		}
	}

	listDir(working_path, argv);

	float total_time = ((float) clock() / CLOCKS_PER_SEC) - start_time;

	//Messages
	if (!preview_unmodifiable) {
		const char *msg = (preview) ? PREVIEW_MSG : SUCCESS_MSG;
		printf(msg, files_n, folders_n);
	}

	if (files_error_n > 0 || preview_unmodifiable) {
		printf(FILES_ERROR_MSG, files_error_n);
	}

	printf(TIME_MSG, total_time);

	return 0;
}
