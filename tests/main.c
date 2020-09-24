#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../src/helper.h"

size_t failed = 0;


static void assertEqual(const char *expected, char *result)
{
    bool success = !strcmp(expected, result);

    printf("%s -> %s\n", success ? "success" : "failure", result);

    if (!success) {
        printf("  Expected '%s' got '%s'.\n", expected, result);
        failed++;
    }
}


void test(void)
{
    char *result;

    printf("switch:\n");

    result = switchSides("./music/song - artist.mp3", '-');
    assertEqual("./music/artist - song.mp3", result);
    free(result);

    result = switchSides("./music/song-artist.mp3", '-');
    assertEqual("./music/artist-song.mp3", result);
    free(result);

    result = switchSides("./music/city-state", '-');
    assertEqual("./music/state-city", result);
    free(result);

    printf("\nupper:\n");

    result = upper("./files/uppercase");
    assertEqual("./files/UPPERCASE", result);
    free(result);

    result = upper("./files/uppercase.txt");
    assertEqual("./files/UPPERCASE.txt", result);
    free(result);

    result = upper("./files/Hello world.txt");
    assertEqual("./files/HELLO WORLD.txt", result);
    free(result);

    printf("\nlower:\n");

    result = lower("./folder/lowercase");
    assertEqual("./folder/lowercase", result);
    free(result);

    result = lower("./folder/lowercase.txt");
    assertEqual("./folder/lowercase.txt", result);
    free(result);

    result = lower("./folder/Hello world.txt");
    assertEqual("./folder/hello world.txt", result);
    free(result);

    printf("\nbefore:\n");

    result = before("./sub/pictures.png", "prefix-");
    assertEqual("./sub/prefix-pictures.png", result);
    free(result);

    result = before("./sub/text", "001");
    assertEqual("./sub/001text", result);
    free(result);

    result = before("./sub/1234.md", "0");
    assertEqual("./sub/01234.md", result);
    free(result);

    printf("\nafter:\n");

    result = after("./docs/pictures.png", "-sufix");
    assertEqual("./docs/pictures-sufix.png", result);
    free(result);

    result = after("./docs/text", "001");
    assertEqual("./docs/text001", result);
    free(result);

    result = after("./docs/1234.md", "0");
    assertEqual("./docs/12340.md", result);
    free(result);

    printf("\nreverse: \n");

    result = reverse("./docs/hello");
    assertEqual("./docs/olleh", result);
    free(result);

    result = reverse("./docs/music.mp3");
    assertEqual("./docs/cisum.mp3", result);
    free(result);

    result = reverse("./docs/File.zip");
    assertEqual("./docs/eliF.zip", result);
    free(result);

    printf("\nreplace: \n");

    result = replace("./folder/hello world", "world", "hello");
    assertEqual("./folder/hello hello", result);
    free(result);

    result = replace("./folder/user-%s.png", "%s", "alex");
    assertEqual("./folder/user-alex.png", result);
    free(result);

    result = replace("./folder/people.png", "p", "ppp");
    assertEqual("./folder/pppeoppple.pppng", result);
    free(result);

    printf("\nremove: \n");

    result = replace("/abs/hello world", "o", "");
    assertEqual("/abs/hell wrld", result);
    free(result);

    result = replace("/abs/filename.svg", "name", "");
    assertEqual("/abs/file.svg", result);
    free(result);

    result = replace("/abs/main.c", "main", "");
    assertEqual("/abs/.c", result);
    free(result);
}


int main(int argc, char* argv[])
{
    test();

    if (failed >= 1) {
        printf("\nFailed tests: %zu.\n", failed);
        return 1;
    }

    printf("\nAll tests have succeeded.\n");
    return 0;
}
