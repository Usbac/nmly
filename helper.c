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


char *fullFilename(const char *dir, const char *filename)
{
	int length = (strlen(dir) + strlen(filename) + 2) * sizeof(char);
	char *path = malloc(length);
	snprintf(path, length, "%s/%s", dir, filename);

	return path;
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


char *before(const char *dir, const char *filename, const char *text)
{
	int len = (strlen(dir) + strlen(filename) + strlen(text) + 2) * sizeof(char);
	char *new = malloc(len);
	snprintf(new, len, "%s/%s%s", dir, text, filename);

	return new;
}


char *after(const char *dir, const char *filename, const char *text)
{
	char *name = strBefore(filename, '.');
	int malloc_size = (strlen(dir) + strlen(filename) + strlen(text) + 2) * sizeof(char);
	char *new = malloc(malloc_size);

	snprintf(new, strlen(dir) + 2, "%s/", dir);
	int len = strlen(new);

	//Without extension
	if (name == NULL) {
		snprintf(new + len, malloc_size - len, "%s%s", filename, text);
	//With extension
	} else {
		char *ext = strAfter(filename, '.');
		snprintf(new + len, malloc_size - len, "%s%s.%s", name, text, ext);
	}

	return new;
}


char *reverse(const char *dir, char *filename)
{
	char *name = strBefore(filename, '.');
	int malloc_size = (strlen(dir) + strlen(filename) + 2) * sizeof(char);
	char *new = malloc(malloc_size);
	snprintf(new, strlen(dir) + 2, "%s/", dir);
	int len = strlen(new);

	//Without extension
	if (name == NULL) {
		strrev(filename);
		snprintf(new + len, malloc_size - len, "%s", filename);
	//With extension
	} else {
		char *ext = strAfter(filename, '.');
		strrev(name);
		snprintf(new + len, malloc_size - len, "%s.%s", name, ext);
	}

	return new;
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


char *changeCases(const char *dir, const char *filename, const int upper)
{
	char *name = strBefore(filename, '.');
	char *ext = strAfter(filename, '.');
	char *new = malloc((strlen(dir) + strlen(filename) + 2) * sizeof(char));
	char *cases;

	snprintf(new, strlen(dir) + 2, "%s/", dir);

	//Without extension
	if (name == NULL) {
		cases = malloc(strlen(filename) + 1 * sizeof(char));
		strCases(cases, filename, upper);
		snprintf(new + strlen(new), strlen(cases) + 1, "%s", cases);
	//With extension
	} else {
		cases = malloc(strlen(name) + 1 * sizeof(char));
		strCases(cases, name, upper);
		snprintf(new + strlen(new), strlen(cases) + strlen(ext) + 2, "%s.%s", cases, ext);
	}

	free(cases);
	free(name);
	free(ext);

	return new;
}


char *upper(const char *dir, const char *filename)
{
	return changeCases(dir, filename, 1);
}


char *lower(const char *dir, const char *filename)
{
	return changeCases(dir, filename, 0);
}


char *replace(const char *dir, const char *filename, const char *ori, const char *rep)
{
	char *replaced = strReplace(filename, ori, rep);

	if (replaced == NULL) {
		return NULL;
	}

	int len = (strlen(dir) + strlen(replaced) + 2) * sizeof(char);
	char *new = malloc(len);
	snprintf(new, len, "%s/%s", dir, replaced);

	free(replaced);

	return new;
}


char *strReplace(const char *str, const char *ori, const char *rep)
{
	char *new = malloc(BUFFER * sizeof(char));
	new[0] = '\0';
	char *remaining_ref = malloc(strlen(str) + 1 * sizeof(char));
	char *remaining = remaining_ref;
	char *pos;
	
	snprintf(remaining, strlen(str) + 1, "%s", str);

	if (strstr(remaining, ori) == NULL) {
		return NULL;
	}

	while ((pos = strstr(remaining, ori)) != NULL) {
		strncat(new + strlen(new), remaining, pos - remaining);
		strcat(new, rep);
		remaining += pos - remaining + strlen(ori);
	}

	snprintf(new + strlen(new), strlen(remaining) + 1, "%s", remaining);
	free(remaining_ref);

	return new;
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


char *switchSides(const char *dir, const char *filename, const char sep)
{
	char *name = strBefore(filename, '.');
	char *new = malloc((strlen(dir) + strlen(filename) + 2) * sizeof(char));
	char tmp[2] = {sep, '\0'};

	snprintf(new, strlen(dir) + 2, "%s/", dir);
	
	//Without extension
	if (name == NULL) {
		char *part_one = strBefore(filename, sep);
		char *part_two = strAfter(filename, sep);

		//Return null if no separator is found or if it's a dot
		if (part_one == NULL || sep == '.') {
			return NULL;
		}

		doSwitch(new, tmp, part_one, part_two);
		
		free(name);
		free(part_one);
		free(part_two);

		return new;
	}

	//With extension
	char *extension = strAfter(filename, '.');
	char *part_one = strBefore(name, sep);
	char *part_two = strAfter(name, sep);

	//Return null if no separator is found or if it's a dot
	if (part_one == NULL || sep == '.') {
		return NULL;
	}
	
	doSwitch(new, tmp, part_one, part_two);
	snprintf(new + strlen(new), strlen(extension) + 2, ".%s", extension);
	
	free(extension);
	free(name);
	free(part_one);
	free(part_two);

	return new;
}
