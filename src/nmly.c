#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <locale.h>
#include "nmly.h"
#include "helper.h"

char *working_path = ".";
char *filter = "";
int files_n = 0, folders_n = 1, files_error_n = 0;
enum OPTION option;
int split_view = 0;
int preview = 0, preview_unmodifiable = 0;
int verbose = 1;
int recursive = 0;
int modify_folders = 0;
unsigned long size_filter = 0;
struct timeval start_time, end_time;
enum SIZE_TYPE {
    LT,
    GT,
    EQ
} size_type_filter;


static int isFile(const char* path)
{
    struct stat buf;
    if (stat(path, &buf) < 0) {
        return 0;
    }

    return S_ISREG(buf.st_mode);
}


static int isDir(const char* path)
{
    struct stat buf;
    if (stat(path, &buf) < 0) {
        return 0;
    }

    return S_ISDIR(buf.st_mode);
}


static unsigned long getFileSize(const char* path)
{
    struct stat buf;
    if (stat(path, &buf) < 0) {
        return 0;
    }

    return buf.st_size;
}


static int sizeFilter(char *path) {
    unsigned long file_size = getFileSize(path);

    switch (size_type_filter) {
        case GT:
            return file_size >= size_filter;
        case LT:
            return file_size < size_filter;
        case EQ:
            return file_size == size_filter;
    }

    return 0;
}


static void getChanges(char **new_path, char *file, char *argv[])
{
    switch (option) {
        case op_before: before(&*new_path, file, argv[2]);
            break;
        case op_after: after(&*new_path, file, argv[2]);
            break;
        case op_upper: upper(&*new_path, file);
            break;
        case op_lower: lower(&*new_path, file);
            break;
        case op_switch: switchSides(&*new_path, file, argv[2][0]);
            break;
        case op_reverse: reverse(&*new_path, file);
            break;
        case op_replace: replace(&*new_path, file, argv[2], argv[3]);
            break;
        case op_remove: replace(&*new_path, file, argv[2], "");
    }
}


static int matchesFilters(char *entpath)
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


static void processFile(char *entpath, char *argv[])
{
    char *new_path;
    int len;

    if (!matchesFilters(entpath)) {
        return;
    }

    len = 0;
    switch (option) {
        case op_before:
        case op_after:
            len = strlen(entpath) + strlen(argv[2]) + 2;
            break;
        case op_upper:
        case op_lower:
        case op_switch:
        case op_reverse:
            len = strlen(entpath) + 2;
            break;
        case op_replace:
        case op_remove:
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
            printf(split_view ? MSG_SPLIT_COMPARE : MSG_COMPARE, entpath, new_path);
        }
    }

    free(new_path);
}


static void listDir(char *basedir, char *argv[])
{
    DIR *dir;
    struct dirent *ent;
    char *entpath;
    int len;

    if (!(dir = opendir(basedir))) {
        if (verbose) {
            printf(preview_unmodifiable ? "%s" :
                   split_view ? MSG_SPLIT_DIR_ERROR : MSG_DIR_ERROR, basedir);
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


static void parseSizeArgs(char *str) {
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
    switch (*suffix) {
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


static void printHelp(void)
{
    printf(MSG_HELP);
    printf(MSG_HELP_OPTIONS);
    printf(MSG_HELP_EXAMPLES);
}


static void printFinishedMsg(void)
{
    if (!preview_unmodifiable) {
        printf(preview ? MSG_PREVIEW : MSG_SUCCESS, files_n, folders_n);
    }

    if (files_error_n > 0 || preview_unmodifiable) {
        printf(MSG_FILES_ERROR, files_error_n);
    }

    double elapsed = (end_time.tv_sec - start_time.tv_sec)
        + (end_time.tv_usec - start_time.tv_usec) / BILLION;

    printf(MSG_TIME, elapsed);
}


static int mapArgs(int argc, char *argv[])
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
            setlocale(LC_ALL, "");
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
            printf(MSG_VERSION);
            return 1;
        }

        if (i+1 > argc) {
            return 0;
        }

        /* Working path */
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--directory")) {
            if (argv[i+1] == NULL) {
                printf(MSG_UNDEFINED_DIR_ERROR);
                return 1;
            }

            working_path = argv[++i];
        }

        /* Extension filter */
        if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extension")) {
            if (argv[i+1] == NULL) {
                printf(MSG_EXTENSION_ERROR);
                return 1;
            }

            filter = argv[++i];
        }

        /* Size */
        if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
            if (argv[i+1] == NULL) {
                printf(MSG_SIZE_ERROR);
                return 1;
            }

            parseSizeArgs(argv[++i]);
        }
    }

    /* Options */
    if (!strcmp(argv[1], "before")) {
        option = op_before;
    } else if (!strcmp(argv[1], "after")) {
        option = op_after;
    } else if (!strcmp(argv[1], "replace")) {
        option = op_replace;
    } else if (!strcmp(argv[1], "upper")) {
        option = op_upper;
    } else if (!strcmp(argv[1], "lower")) {
        option = op_lower;
    } else if (!strcmp(argv[1], "switch")) {
        option = op_switch;
    } else if (!strcmp(argv[1], "reverse")) {
        option = op_reverse;
    } else if (!strcmp(argv[1], "remove")) {
        option = op_remove;
    } else if (!preview_unmodifiable) {
        printf("%s", MSG_ARG_ERROR);
        return 1;
    }

    return 0;
}


static int confirm()
{
    char confirm;

    if (preview || preview_unmodifiable) {
        return 1;
    }

    printf(MSG_DIR_CONFIRM, working_path);
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
