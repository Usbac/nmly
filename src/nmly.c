#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include "nmly.h"
#include "helper.h"

char *working_path = ".";
char *filter = EMPTY;
int files_n = 0, folders_n = 1, files_error_n = 0;
int option = 0;
int split_view = 0;
int preview = 0, preview_unmodifiable = 0;
int verbose = 1;
int recursive = 0;
int modify_folders = 0;
unsigned long size_filter = 0;
struct timeval start_time, end_time, elapsed_time;
enum SIZE_TYPE {
	LT,
	GT,
	EQ
} size_type_filter;


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


void getChanges(char **new_path, char *file, char *argv[]) 
{
	switch (option) {
		case BEFORE: before(&*new_path, file, argv[2]);
			break;
		case AFTER: after(&*new_path, file, argv[2]);
			break;
		case UPPER: upper(&*new_path, file);
			break;
		case LOWER: lower(&*new_path, file);
			break;
		case SWITCH: switchSides(&*new_path, file, argv[2][0]);
			break;
		case REVERSE: reverse(&*new_path, file);
			break;
		case REPLACE: replace(&*new_path, file, argv[2], argv[3]);
			break;
		case REMOVE: replace(&*new_path, file, argv[2], EMPTY);
	}
}


void listDir(char *basedir, char *argv[])
{
	DIR *dir;
	struct dirent *ent;
	char *entpath;
	int len;

	if (!(dir = opendir(basedir))) {
		if (verbose) {
			printf(preview_unmodifiable ? "%s" : 
				split_view ? SPLIT_DIR_ERROR_MSG : DIR_ERROR_MSG, basedir);
		}

		files_error_n++;
		return;
	}

	while ((ent = readdir(dir))) {
		/* Avoid current and previous folders */
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
			continue;
		}

		len = strlen(basedir) + strlen(ent->d_name) + 2;
		entpath = malloc_(len * sizeof(char));
		memset(entpath, 0, len);
		concatPath(entpath, basedir, ent->d_name);

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
}


void processFile(char *entpath, char *argv[])
{
	char *new_path;
	int len;

	if (!matchesFilters(entpath)) {
		return;
	}
	
	len = 0;
	switch (option) {
		case BEFORE: case AFTER:
			len = strlen(entpath) + strlen(argv[2]) + 2;
			break;
		case UPPER: case LOWER: case SWITCH: case REVERSE:
			len = strlen(entpath) + 2;
			break;
		case REPLACE: case REMOVE:
			len = BUFFER;
	}

	new_path = malloc_(len * sizeof(char));
	memset(new_path, 0, len);
	getChanges(&new_path, entpath, argv);

	if (!strlen(new_path)) {
		free(new_path);
		return;
	}

	files_n++;

	if (!preview_unmodifiable) {
		if (!preview) {
			rename(entpath, new_path);
		}

		if (verbose) {
			printf(split_view ? SPLIT_COMPARE_MSG : COMPARE_MSG, entpath, new_path);
		}
	}

	free(new_path);
}


int matchesFilters(char *entpath)
{
	int matches = 1;
	char *file = strAfter(entpath, '/');

	/* Matches filter by extension */
	char *ext = strAfter(file, '.');
	if (filter[0] != '\0' && (ext == NULL || strcmp(ext, filter))) {
		matches = 0;
	}

	/* Matches filter by file size */
	if (size_filter != 0 && !sizeFilter(entpath)) {
		matches = 0;
	}

	free(file);
	free(ext);

	return matches;
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
		case 'G': case 'g':
			size_filter = size_filter * KBYTE * KBYTE * KBYTE;
			break;
		case 'M': case 'm':
			size_filter = size_filter * KBYTE * KBYTE;
			break;
		case 'K': case 'k':
			size_filter = size_filter * KBYTE;
			break;
	}
}


void printHelp(void)
{
	printf(HELP_USAGE_MSG);
	printf(HELP_OPTIONS_MSG);
	printf(HELP_EXAMPLES_MSG);
}


void printFinishedMsg(void)
{
	if (!preview_unmodifiable) {
		printf(preview ? PREVIEW_MSG : SUCCESS_MSG, files_n, folders_n);
	}

	if (files_error_n > 0 || preview_unmodifiable) {
		printf(FILES_ERROR_MSG, files_error_n);
	}

	double elapsed = (end_time.tv_sec - start_time.tv_sec)
		+ (end_time.tv_usec - start_time.tv_usec) / BILLION;

	printf(TIME_MSG, elapsed);
}


int mapArgs(int argc, char *argv[]) 
{
	int i;
	/* Show the help by default */
	if (argc == 1) {
		printHelp();
		return 1;
	}

	for (i = 0; i < argc; i++) {
		/* Recursive */
		if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--recursive")) {
			recursive = 1;
		}

		/* Preview */
		if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--preview")) {
			preview = 1;
		}

		/* No Verbose */
		if (!strcmp(argv[i], "-nv") || !strcmp(argv[i], "--no-verbose")) {
			verbose = 0;
		}
		
		/* Modify folders */
		if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--folders")) {
			modify_folders = 1;
		}

		/* Locale (special characters) */
		if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--locale")) {
			setlocale(LC_ALL, EMPTY);
		}

		/* Split view */
		if (!strcmp(argv[i], "--split")) {
			split_view = 1;
		}

		/* List unmodifiable files */
		if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--unmodifiable")) {
			preview_unmodifiable = 1;
		}

		/* Help */
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			printHelp();
			return 1;
		}

		/* Version */
		if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf(VERSION_MSG);
			return 1;
		}

		if (i+1 > argc) {
			return 0;
		}

		/* Working path */
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--directory")) {
			working_path = argv[++i];
		}

		/* Extension filter */
    		if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extension")) {
			filter = argv[++i];
		}

		/* Size */
		if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
			parseSizeArgs(argv[++i]);
		}
	}

	/* Options */
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
		printf("%s", ARG_ERROR_MSG);
		return 1;
	}

	return 0;
}


int confirm()
{
	char confirm;

	if (preview || preview_unmodifiable) {
		return 1;
	}

	printf(DIR_CONFIRM_MSG, working_path);
	scanf("%c", &confirm);

	if (confirm != 'Y' && confirm != 'y' && confirm != '\n') {
		return 0;
	}

	return 1;
}


int main(int argc, char *argv[]) 
{
	if (mapArgs(argc, argv) || !confirm()) {
		return 0;
	}

	gettimeofday(&start_time, NULL);
	listDir(working_path, argv);
 	gettimeofday(&end_time, NULL);

	printFinishedMsg();

	return 0;
}
