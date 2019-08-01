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
	char *path = malloc(strlen(dir) + strlen(filename) + 2);
	strcpy(path, dir);
	strcat(path, "/");
	strcat(path, filename);

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
	char *new = malloc((strlen(dir) + strlen(filename) + strlen(text) + 2) * sizeof(char));
	
	strcpy(new, dir);
	strcat(new, "/");
	strcat(new, text);
	strcat(new, filename);

	return new;
}


char *after(const char *dir, const char *filename, const char *text)
{
	char *name = strBefore(filename, '.');
	char *new = malloc((strlen(dir) + strlen(filename) + strlen(text) + 2) * sizeof(char));

	strcpy(new, dir);
	strcat(new, "/");

	//Without extension
	if (name == NULL) {
		strcat(new, filename);
		strcat(new, text);
	//With extension
	} else {
		char *extension = strAfter(filename, '.');
		strcat(new, name);
		strcat(new, text);
		strcat(new, ".");
		strcat(new, extension);
	}

	return new;
}


char *reverse(const char *dir, char *filename)
{
	char *name = strBefore(filename, '.');
	char *new = malloc((strlen(dir) + strlen(filename) + 2) * sizeof(char));
	strcpy(new, dir);
	strcat(new, "/");

	//Without extension
	if (name == NULL) {
		strrev(filename);
		strcat(new, filename);
	//With extension
	} else {
		char *extension = strAfter(filename, '.');
		strrev(name);
		strcat(new, name);
		strcat(new, ".");
		strcat(new, extension);
	}

	return new;
}


void strCases(char *dest, const char *str, const int upper)
{
	size_t length = strlen(str);
	size_t i;

	for (i = 0; i < length; i++) {
		if (upper) {
			dest[i] = toupper(str[i]);
		} else {
			dest[i] = tolower(str[i]);
		}
	}

	dest[i] = '\0';
}


char *changeCases(const char *dir, const char *filename, const int upper)
{
	char *name = strBefore(filename, '.');
	char *extension = strAfter(filename, '.');
	char *new = malloc((strlen(dir) + strlen(filename) + 2) * sizeof(char));
	char *cases = malloc(strlen(filename) * sizeof(char));

	strcpy(new, dir);
	strcat(new, "/");

	//Without extension
	if (name == NULL) {
		strCases(cases, filename, upper);
		strcat(new, cases);
	//With extension
	} else {
		strCases(cases, name, upper);
		strcat(new, cases);
		strcat(new, ".");
		strcat(new, extension);
	}

	free(cases);

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

	char *new = malloc(strlen(dir) + strlen(replaced) + 2);
	strcpy(new, dir);
	strcat(new, "/");
	strcat(new, replaced);

	return new;
}


char *strReplace(const char *str, const char *ori, const char *rep)
{
	char *new = malloc(4096);
	new[0] = '\0';
	char *remaining = malloc(strlen(str) + 1 * sizeof(char));
	char *pos;
	
	strcpy(remaining, str);

	if (strstr(remaining, ori) == NULL) {
		return NULL;
	}

	while ((pos = strstr(remaining, ori)) != NULL) {
		strncat(new + strlen(new), remaining, pos - remaining);
		strcat(new, rep);
		remaining += pos - remaining + strlen(ori);
	}

	strcat(new, remaining);

	return new;
}


void doSwitch(char *new, const char *sep, char *part_one, char *part_two)
{
	//Leave whitespace betweeen separator
	if (part_one[strlen(part_one)-1] == ' ' && part_two[0] == ' ') {
		trim(part_two);
		trim(part_one);
		strcat(new, part_two);
		strcat(new, " ");
		strcat(new, sep);
		strcat(new, " ");
	} else {
		trim(part_two);
		trim(part_one);
		strcat(new, part_two);
		strcat(new, sep);
	}

	strcat(new, part_one);
}


char *switchSides(const char *dir, const char *filename, const char sep)
{
	char *name = strBefore(filename, '.');
	char *new = malloc((strlen(dir) + strlen(filename) + 2) * sizeof(char));
	char tmp[2] = {sep, '\0'};

	strcpy(new, dir);
	strcat(new, "/");
	
	//Without extension
	if (name == NULL) {
		char *part_one = strBefore(filename, sep);
		char *part_two = strAfter(filename, sep);

		//Return null if no separator is found or if it's a dot
		if (part_one == NULL || sep == '.') {
			return NULL;
		}

		doSwitch(new, tmp, part_one, part_two);
		
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
	strcat(new, ".");
	strcat(new, extension);
	
	free(part_one);
	free(part_two);

	return new;
}
