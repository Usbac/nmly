#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"


void substr(char *sub, const char *str, const int start, const int len) 
{
	memcpy(sub, &str[start], len);
}


void *trim(char *str) 
{
	size_t length = strlen(str) - 1;

	if (str[0] == ' ') {
		memmove(str, str + 1, strlen(str));
	}

	if (str[length] == ' ') {
		str[length] = '\0';
	}
}


char *strrev(char *str)
{
	if (!str || !*str) {
		return str;
	}

	size_t i = strlen(str) - 1;
	size_t j = 0;

	char ch;
	while (i > j) {
		ch = str[i];
		str[i] = str[j];
		str[j] = ch;
		i--;
		j++;
	}

	return str;
}


char *strBefore(const char *str, const char character) 
{
	char *pos = strrchr(str, character);
	
	if (pos == NULL) {
		return NULL;
	}

	size_t size = pos - str;

	char *part = malloc(size + 1 * sizeof(char));
	substr(part, str, 0, size);

	if (part[size] != '\0') {
		part[size] = '\0';
	}
	
	return part;
}


char *strAfter(const char *str, const char character) 
{
	char *pos = strrchr(str, character);
	
	if (pos == NULL) {
		return NULL;
	}

	size_t size = strlen(str) - (pos - str);
	
	char *part = malloc(size * sizeof(char));
	substr(part, str, pos - str + 1, size);

	return part;
}


void before(char **src, const char *dir, const char *filename, const char *text)
{
	snprintf(*src, strlen(dir) + strlen(filename) + strlen(text) + 2, "%s/%s%s", dir, text, filename);
}


void after(char **src, const char *dir, const char *filename, const char *text)
{
	char *name = strBefore(filename, '.');
	snprintf(*src, strlen(dir) + 2, "%s/", dir);
	int len = strlen(*src);

	//Without extension
	if (name == NULL) {
		snprintf(*src + len, strlen(filename) + strlen(text) + 1, "%s%s", filename, text);
	//With extension
	} else {
		char *ext = strAfter(filename, '.');
		snprintf(*src + len, strlen(name) + strlen(text) + strlen(ext) + 2, "%s%s.%s", name, text, ext);
		free(ext);
	}
}


void reverse(char **src, const char *dir, char *filename)
{
	snprintf(*src, strlen(dir) + 2, "%s/", dir);
	int len = strlen(*src);

	char *name = strBefore(filename, '.');

	//Without extension
	if (name == NULL) {
		strrev(filename);
		snprintf(*src + len, strlen(filename) + 1, "%s", filename);
	//With extension
	} else {
		char *ext = strAfter(filename, '.');
		strrev(name);
		snprintf(*src + len, strlen(name) + strlen(ext) + 2, "%s.%s", name, ext);
		free(name);
		free(ext);
	}
}


void strCases(char *dest, const char *str, const int upper)
{
	size_t length = strlen(str);
	size_t i;

	for (i = 0; i < length; i++) {
		dest[i] = upper ? toupper(str[i]) : tolower(str[i]);
	}

	dest[i] = '\0';
}


void changeCases(char **src, const char *dir, const char *filename, const int upper)
{
	char *name = strBefore(filename, '.');
	char *ext = strAfter(filename, '.');
	char *cases;

	snprintf(*src, strlen(dir) + 2, "%s/", dir);

	//Without extension
	if (name == NULL) {
		cases = malloc(strlen(filename) + 1 * sizeof(char));
		strCases(cases, filename, upper);
		snprintf(*src + strlen(*src), strlen(cases) + 1, "%s", cases);
	//With extension
	} else {
		cases = malloc(strlen(name) + 1 * sizeof(char));
		strCases(cases, name, upper);
		snprintf(*src + strlen(*src), strlen(cases) + strlen(ext) + 2, "%s.%s", cases, ext);
	}

	free(cases);
	free(name);
	free(ext);
}


void upper(char **src, const char *dir, const char *filename)
{
	changeCases(&*src, dir, filename, 1);
}


void lower(char **src, const char *dir, const char *filename)
{
	changeCases(&*src, dir, filename, 0);
}


void replace(char **src, const char *dir, const char *filename, const char *ori, const char *rep)
{
	char *replaced = malloc(BUFFER * sizeof(char));
	
	if (strReplace(&replaced, filename, ori, rep)) {
		free(replaced);
		*src = NULL;
		return;
	}

	snprintf(*src, (strlen(dir) + strlen(replaced) + 2) * sizeof(char), "%s/%s", dir, replaced);

	free(replaced);
}


int strReplace(char **src, const char *str, const char *ori, const char *rep)
{
	*src[0] = '\0';
	char *remaining_ref = malloc(strlen(str) + 1 * sizeof(char));
	char *remaining = remaining_ref;
	char *pos;
	
	snprintf(remaining, strlen(str) + 1, "%s", str);

	if (strstr(remaining, ori) == NULL) {
		free(remaining_ref);
		return 1;
	}

	while ((pos = strstr(remaining, ori)) != NULL) {
		strncat(*src + strlen(*src), remaining, pos - remaining);
		strcat(*src, rep);
		remaining += pos - remaining + strlen(ori);
	}

	snprintf(*src + strlen(*src), strlen(remaining) + 1, "%s", remaining);
	free(remaining_ref);

	return 0;
}


void doSwitch(char *new, const char *sep, char *part_one, char *part_two)
{
	//Leave whitespace betweeen separator
	if (part_one[strlen(part_one) - 1] == ' ' && part_two[0] == ' ') {
		trim(part_two);
		trim(part_one);
		int len = strlen(part_two) + strlen(sep) + 3;
		snprintf(new + strlen(new), len, "%s %s ", part_two, sep);
	} else {
		trim(part_two);
		trim(part_one);
		int len = strlen(part_two) + strlen(sep) + 1;
		snprintf(new + strlen(new), len, "%s%s", part_two, sep);
	}

	snprintf(new + strlen(new), strlen(part_one) + 1, "%s", part_one);
}


void switchSides(char **src, const char *dir, const char *filename, const char sep)
{
	char *name = strBefore(filename, '.');
	char tmp[2] = {sep, '\0'};

	snprintf(*src, strlen(dir) + 2, "%s/", dir);
	
	//Without extension
	if (name == NULL) {
		char *part_one = strBefore(filename, sep);
		char *part_two = strAfter(filename, sep);

		//If no separator is found or if it's a dot
		if (part_one == NULL || sep == '.') {
			free(part_two);
			*src = NULL;
			return;
		}

		doSwitch(*src, tmp, part_one, part_two);
		
		free(part_one);
		free(part_two);
		return;
	}

	//With extension
	char *ext = strAfter(filename, '.');
	char *part_one = strBefore(name, sep);
	char *part_two = strAfter(name, sep);

	//If no separator is found or if it's a dot
	if (part_one == NULL || sep == '.') {
		free(ext);
		free(name);
		free(part_two);
		*src = NULL;
		return;
	}
	
	doSwitch(*src, tmp, part_one, part_two);
	snprintf(*src + strlen(*src), strlen(ext) + 2, ".%s", ext);
	
	free(ext);
	free(name);
	free(part_one);
	free(part_two);
}
