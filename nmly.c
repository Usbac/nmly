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

const char *single_str = "%s\n";
const char *preview_msg = "\n%i File(s) to be modified in %i folder(s)";
const char *success_msg = "\n%i File(s) modified in %i folder(s)";
const char *files_error_msg = "\n%i File(s) cannot be modified. Maybe check your permissions?";
const char *dir_error_msg = "Cannot open directory %s\n";
const char *dir_confirm_msg = "Apply the changes in the following directory '%s'? [Y/n] ";
const char *compare_msg = "%s > %s \n";
const char *time_msg = "\n%f Segs\n";
const char *arg_error_msg = "Error: Invalid command\n";
const char *version_msg = "Nmly v0.9.6\n";
char *working_path = ".";
char *filter = "";
int files_n = 0, folders_n = 0, files_error_n = 0;
int option = 0;
int preview = 0;
int preview_unmodifiable = 0;
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
	char *new_path = NULL;
	
	//Matches filter by extension
	char *extension = strAfter(filename, '.');
	if (filter[0] != '\0' && (extension == NULL || strcmp(extension, filter))) {
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
		case REMOVE: new_path = replace(dir, filename, argv[2], "");
			break;
	}

	free(dir);
	free(filename);

	return new_path;
}


void listDir(char *basedir, char *argv[])
{
	DIR *dir;
	char b[BUFFER];
	struct dirent *ent;

	if ((dir = opendir(basedir)) == NULL) {
		printf(preview_unmodifiable ? single_str : dir_error_msg, basedir);
		files_error_n++;
		return;
	}

	while ((ent = readdir(dir)) != NULL) {
		//Avoid current and previous folders
		if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
			continue;
		}

		char *entpath = malloc((strlen(basedir) + strlen(ent->d_name) + 2) * sizeof(char));
		strcpy(entpath, basedir);
		strcat(entpath, "/");
		strcat(entpath, ent->d_name);

		if (is_file(entpath) || (is_dir(entpath) && modify_folders)) {
			processFile(entpath, argv);
		}

		if (recursive && is_dir(entpath)) {
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
	
		printf(compare_msg, entpath, new_path);
	}

	free(new_path);
}


int mapArgs(int argc, char *argv[]) 
{
	size_t i;
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
			setlocale(LC_ALL, "");
		}

		//List unmodifiable files
		if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--unmodifiable")) {
			preview_unmodifiable = 1;
		}

		//Help
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			help();
			return 1;
		}

		//Version
		if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
			printf("%s", version_msg);
			return 1;
		}

		if (i+1 > argc) {
			return 0;
		}

		//Working path
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--directory")) {
			working_path = malloc(strlen(argv[++i]) * sizeof(char));
			strcpy(working_path, argv[i]);
		}

		//Extension filter
    		if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extension")) {
			filter = malloc(strlen(argv[++i]) * sizeof(char));
			strcpy(filter, argv[i]);
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
		printf("%s\n", arg_error_msg);
		return 1;
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
		"-d --directory [text]  The directory where the changes will be applied\n"
		"-e --extension [text]  Apply the changes only to the files with that extension\n"
		"-f --folders           Apply the changes to the folders name too\n"
		"-h --help              Get help and information about the application\n"
		"-l --locale            Accept special characters (like latin characters)\n"
		"-p --preview           Show the changes without applying them\n"
		"-r --recursive         Apply the changes recursively in the directory\n"
		"-u --unmodifiable      Show the files that cannot be modified\n"
		"-v --version           Show the application version\n\n"
		"EXAMPLES\n\n"
		"$ nmly switch - -d ./\n"
		"Author - Song.mp3 > Song - Author.mp3\n\n"
		"$ nmly remove ' 2017' -d ./vacations -e mp4\n"
		"./vacations/video 2017.mp4 > ./vacations/video.mp4\n\n"
		"$ nmly replace jpeg jpg -d ./\n"
		"picture.jpeg > picture.jpg\n\n"
		"$ nmly after world -d ./ -r\n"
		"hello.pdf > helloworld.pdf\n"
		"subfolder/file.txt > subfolder/fileworld.txt\n\n"
		"$ nmly -u -d ./folder -r\n"
		"./folder/filewithpermissions.txt\n"
	);
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

		printf(dir_confirm_msg, working_path);
		scanf("%c", &confirm);

		if (confirm != 'Y' && confirm != 'y') {
			return 0;
		}
	}

	listDir(working_path, argv);

	float total_time = ((float) clock() / CLOCKS_PER_SEC) - start_time;

	if (!preview_unmodifiable) {
		const char *msg = (preview) ? preview_msg : success_msg;
		printf(msg, files_n, folders_n);
	}

	//Files error message
	if (files_error_n > 0 || preview_unmodifiable) {
		printf(files_error_msg, files_error_n);
	}

	printf(time_msg, total_time);

	return 0;
}
