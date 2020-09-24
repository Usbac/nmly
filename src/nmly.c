#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <locale.h>
#include "nmly.h"
#include "helper.h"

char *working_path = ".";
char *filter = "";
int files_n = 0, folders_n = 1, files_error_n = 0;
bool split_view = false;
bool preview = false, preview_unmodifiable = false;
bool verbose = true;
bool recursive = false;
bool modify_folders = false;
unsigned long size_filter = 0;

enum SIZE_TYPE {
    LT,
    GT,
    EQ
} size_type_filter;

enum OPTION {
    op_before,
    op_after,
    op_replace,
    op_upper,
    op_lower,
    op_switch,
    op_reverse,
    op_remove
} option;


static bool isFile(const char* path)
{
    struct stat info;
    if (stat(path, &info) < 0) {
        return false;
    }

    return S_ISREG(info.st_mode);
}


static bool isDir(const char* path)
{
    struct stat info;
    if (stat(path, &info) < 0) {
        return false;
    }

    return S_ISDIR(info.st_mode);
}


static unsigned long getFileSize(const char* path)
{
    struct stat buf;
    if (stat(path, &buf) < 0) {
        return 0;
    }

    return buf.st_size;
}


static bool sizeFilter(const char *path)
{
    unsigned long file_size = getFileSize(path);

    switch (size_type_filter) {
        case GT: return file_size >= size_filter;
        case LT: return file_size < size_filter;
        case EQ: return file_size == size_filter;
        default: return false;
    }
}


static char *getChanges(const char *file, char *argv[])
{
    switch (option) {
        case op_before: return before(file, argv[2]);
        case op_after: return after(file, argv[2]);
        case op_upper: return upper(file);
        case op_lower: return lower(file);
        case op_switch: return switchSides(file, argv[2][0]);
        case op_reverse: return reverse(file);
        case op_replace: return replace(file, argv[2], argv[3]);
        case op_remove: return replace(file, argv[2], "");
        default: return NULL;
    }
}


static bool matchesFilters(char *path)
{
    bool matches = true;
    char *file = strAfter(path, '/');
    char *ext = strAfter(file, '.');

    /* Matches filter by extension */
    if (filter[0] != '\0' && (ext == NULL || strcmp(ext, filter) != 0)) {
        matches = false;
    }

    /* Matches filter by file size */
    if (size_filter != 0 && !sizeFilter(path)) {
        matches = false;
    }

    free(file);
    free(ext);

    return matches;
}


static void processFile(char *path, char *argv[])
{
    char *new_path;

    if (!matchesFilters(path)) {
        return;
    }

    new_path = getChanges(path, argv);
    if (new_path == NULL) {
        goto end;
    }

    files_n++;

    if (!preview_unmodifiable) {
        if (!preview) {
            rename(path, new_path);
        }

        if (verbose) {
            printf(split_view ?
                MSG_SPLIT_COMPARE :
                MSG_COMPARE,
                path, new_path);
        }
    }

    end: free(new_path);
}


static void listDir(char *basedir, char *argv[])
{
    DIR *dir = opendir(basedir);
    struct dirent *ent;
    char *path;

    if (!dir) {
        if (verbose) {
            printf(preview_unmodifiable ?
                "%s" :
                split_view ?
                MSG_SPLIT_DIR_ERROR :
                MSG_DIR_ERROR,
                basedir);
        }

        files_error_n++;
        return;
    }

    while ((ent = readdir(dir))) {
        /* Avoid current and previous folders */
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
            continue;
        }

        path = joinPaths(basedir, ent->d_name);

        if (isFile(path) || (isDir(path) && modify_folders)) {
            processFile(path, argv);
        }

        if (recursive && isDir(path)) {
            listDir(path, argv);
            folders_n++;
        }

        free(path);
    }

    closedir(dir);
}


static void parseSizeArgs(char *str)
{
    char sign = str[0];
    char *suffix;

    switch (sign) {
        case '+': size_type_filter = GT; break;
        case '-': size_type_filter = LT; break;
        default: size_type_filter = EQ;
    }

    if (size_type_filter != EQ) {
        str++;
    }

    size_filter = strtoul(str, &suffix, 10);
    switch (*suffix) {
        case 'G':
        case 'g':
            size_filter = size_filter * GBYTE;
            break;
        case 'M':
        case 'm':
            size_filter = size_filter * MBYTE;
            break;
        case 'K':
        case 'k':
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

    printf("\n");
}


static bool mapArgs(int argc, char *argv[])
{
    int i;
    /* Show the help by default */
    if (argc == 1) {
        printHelp();
        return true;
    }

    for (i = 0; i < argc; i++) {
        /* Recursive */
        if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--recursive")) {
            recursive = true;
        }

        /* Preview */
        if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--preview")) {
            preview = true;
        }

        /* No Verbose */
        if (!strcmp(argv[i], "-nv") || !strcmp(argv[i], "--no-verbose")) {
            verbose = false;
        }

        /* Modify folders */
        if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--folders")) {
            modify_folders = true;
        }

        /* Locale (special characters) */
        if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--locale")) {
            setlocale(LC_ALL, "");
        }

        /* Split view */
        if (!strcmp(argv[i], "--split")) {
            split_view = true;
        }

        /* List unmodifiable files */
        if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--unmodifiable")) {
            preview_unmodifiable = true;
        }

        /* Help */
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            printHelp();
            return true;
        }

        /* Version */
        if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            printf(MSG_VERSION);
            return true;
        }

        if (i+1 > argc) {
            return false;
        }

        /* Working path */
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--directory")) {
            if (argv[i+1] == NULL) {
                printf(MSG_UNDEFINED_DIR_ERROR);
                return true;
            }

            working_path = argv[++i];
        }

        /* Extension filter */
        if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--extension")) {
            if (argv[i+1] == NULL) {
                printf(MSG_EXTENSION_ERROR);
                return true;
            }

            filter = argv[++i];
        }

        /* Size */
        if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
            if (argv[i+1] == NULL) {
                printf(MSG_SIZE_ERROR);
                return true;
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
        printf(MSG_ARG_ERROR);
        return true;
    }

    return false;
}


static bool confirm()
{
    char confirm;

    if (preview || preview_unmodifiable) {
        return true;
    }

    printf(MSG_DIR_CONFIRM, working_path);
    scanf("%c", &confirm);

    if (confirm != 'Y' && confirm != 'y' && confirm != '\n') {
        return false;
    }

    return true;
}


int main(int argc, char *argv[])
{
    if (mapArgs(argc, argv) || !confirm()) {
        return 0;
    }

    listDir(working_path, argv);
    printFinishedMsg();

    return 0;
}
