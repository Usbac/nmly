#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"


void substr(char *sub, const char *str, const int start, const int len) 
{
	memcpy(sub, &str[start], len);
}


void trim(char *str) 
{
	size_t len = strlen(str) - 1;

	if (str[0] == ' ') {
		memmove(str, str + 1, strlen(str));
	}

	if (str[len] == ' ') {
		str[len] = '\0';
	}
}


char *strrev(char *str)
{
	size_t i, j;
	char tmp;

	if (!str || !*str) {
		return str;
	}

	i = strlen(str) - 1;
	j = 0;

	while (i > j) {
		tmp = str[i];
		str[i--] = str[j];
		str[j++] = tmp;
	}

	return str;
}


char *strBefore(const char *str, const char ch) 
{
	char *pos = strrchr(str, ch);
	char *part;
	size_t size;
	
	if (pos == NULL) {
		return NULL;
	}

	size = pos - str;
	part = malloc_(size + 1 * sizeof(char));
	substr(part, str, 0, size);

	if (part[size] != '\0') {
		part[size] = '\0';
	}
	
	return part;
}


char *strAfter(const char *str, const char ch) 
{
	char *pos = strrchr(str, ch);
	char *part;
	size_t size;
	
	if (pos == NULL) {
		return NULL;
	}

	size = strlen(str) - (pos - str);
	part = malloc_(size * sizeof(char));
	substr(part, str, pos - str + 1, size);

	return part;
}


void before(char **src, const char *file, const char *text)
{
	char *dir = strBefore(file, '/');
	char *filename = strAfter(file, '/');
	snprintf(*src, strlen(file) + strlen(text) + 2, "%s/%s%s", dir, text, filename);
	free(dir);
	free(filename);
}


void after(char **src, const char *file, const char *text)
{
	char *dir = strBefore(file, '/');
	char *filename = strAfter(file, '/');
	char *name = strBefore(filename, '.');
	char *ext;
	int len;

	snprintf(*src, strlen(dir) + 2, "%s/", dir);
	len = strlen(*src);

	/* Without extension */
	if (name == NULL) {
		snprintf(*src + len, strlen(filename) + strlen(text) + 1, "%s%s", filename, text);
	/* With extension */
	} else {
		ext = strAfter(filename, '.');
		snprintf(*src + len, strlen(name) + strlen(text) + strlen(ext) + 2, 
			"%s%s.%s", name, text, ext);
		free(ext);
	}

	free(name);
	free(filename);
	free(dir);
}


void reverse(char **src, const char *file)
{
	char *dir = strBefore(file, '/');
	char *filename = strAfter(file, '/');
	char *name, *ext;
	int len;
	snprintf(*src, strlen(dir) + 2, "%s/", dir);
	len = strlen(*src);
	name = strBefore(filename, '.');

	/* Without extension */
	if (name == NULL) {
		strrev(filename);
		snprintf(*src + len, strlen(filename) + 1, "%s", filename);
	/* With extension */
	} else {
		ext = strAfter(filename, '.');
		strrev(name);
		snprintf(*src + len, strlen(name) + strlen(ext) + 2, "%s.%s", name, ext);
		free(name);
		free(ext);
	}

	free(filename);
	free(dir);
}


void strCases(char *dest, const char *str, const int upper)
{
	size_t len = strlen(str);
	size_t i;

	for (i = 0; i < len; i++) {
		dest[i] = upper ? toupper(str[i]) : tolower(str[i]);
	}

	dest[i] = '\0';
}


void changeCases(char **src, const char *file, const int upper)
{
	char *dir = strBefore(file, '/');
	char *filename = strAfter(file, '/');
	char *name = strBefore(filename, '.');
	char *ext = strAfter(filename, '.');
	char *cases;

	snprintf(*src, strlen(dir) + 2, "%s/", dir);

	/* Without extension */
	if (name == NULL) {
		cases = malloc_(strlen(filename) + 1 * sizeof(char));
		strCases(cases, filename, upper);
		snprintf(*src + strlen(*src), strlen(cases) + 1, "%s", cases);
	/* With extension */
	} else {
		cases = malloc_(strlen(name) + 1 * sizeof(char));
		strCases(cases, name, upper);
		snprintf(*src + strlen(*src), strlen(cases) + strlen(ext) + 2, "%s.%s", cases, ext);
	}

	free(cases);
	free(ext);
	free(name);
	free(filename);
	free(dir);
}


void upper(char **src, const char *file)
{
	changeCases(&*src, file, 1);
}


void lower(char **src, const char *file)
{
	changeCases(&*src, file, 0);
}


void replace(char **src, const char *file, const char *ori, const char *rep)
{
	char *dir = strBefore(file, '/');
	char *filename = strAfter(file, '/');
	char *replaced = malloc_(BUFFER * sizeof(char));
	int len;
	
	if (strReplace(&replaced, filename, ori, rep)) {
		free(replaced);
		free(filename);
		free(dir);
		*src = NULL;
		return;
	}

	len = (strlen(dir) + strlen(replaced) + 2) * sizeof(char);
	snprintf(*src, len, "%s/%s", dir, replaced);

	free(replaced);
	free(filename);
	free(dir);
}


int strReplace(char **src, const char *str, const char *ori, const char *rep)
{
	char *remaining_ref = malloc_(strlen(str) + 1 * sizeof(char));
	char *remaining = remaining_ref;
	char *pos;
	*src[0] = '\0';
	
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
	int len = strlen(part_two) + strlen(sep) + strlen(part_one) + 1;
	char *format;

	/* Leave whitespace betweeen separator */
	if (part_one[strlen(part_one) - 1] == ' ' && part_two[0] == ' ') {
		len += 2;
		format = "%s %s %s";
	} else {
		format = "%s%s%s";
	}

	trim(part_two);
	trim(part_one);

	snprintf(new, len, format, part_two, sep, part_one);
}


void switchSides(char **src, const char *file, const char sep)
{
	char *part_one, *part_two, *ext;
	char *dir = strBefore(file, '/');
	char *filename = strAfter(file, '/');
	char *name = strBefore(filename, '.');
	char *switched = malloc_(strlen(filename) * sizeof(char));
	char tmp[2];
	tmp[0] = sep;
	tmp[1] = '\0';

	snprintf(*src, strlen(dir) + 2, "%s/", dir);
	
	/* Without extension */
	if (name == NULL) {
		part_one = strBefore(filename, sep);
		part_two = strAfter(filename, sep);

		/* If no separator is found or if it's a dot */
		if (part_one != NULL && sep != '.') {
			doSwitch(switched, tmp, part_one, part_two);
			snprintf(*src + strlen(*src), strlen(switched) + 1, "%s", switched);
		} else {
			*src = NULL;
		}
	/* With extension */
	} else {
		ext = strAfter(filename, '.');
		part_one = strBefore(name, sep);
		part_two = strAfter(name, sep);

		/* If no separator is found or if it's a dot */
		if (part_one != NULL && sep != '.') {
			doSwitch(switched, tmp, part_one, part_two);
			snprintf(*src + strlen(*src), strlen(switched) + 1, "%s", switched);
			snprintf(*src + strlen(*src), strlen(ext) + 2, ".%s", ext);
		} else {
			*src = NULL;
		}

		free(ext);
	}
	
	free(name);
	free(part_one);
	free(part_two);
	free(filename);
	free(dir);
	free(switched);
}
