#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "protos.h"

/*
 * prototypes
 */
static void read_file (FILE* fin);
static char* last3chars (char* line);
static int exclude_this_char(char* exclude, char ch);
static void filter_from_end(char *exclude, char* line);

/*
 * global
 * functions
 */
#ifndef CPPUTESTING
int main(int argc, char** argv)
{
	if (argc > 1) {
		FILE* fin = fopen (argv[1], "r");
		if (fin) {
			read_file (fin);
			fclose (fin);
		}
	} else {
		read_file (stdin);
	}
}
#else
void mock(void) { read_file(stdin); }
#endif /* CPPUTESTING */

static void read_file (FILE* fin)
{
	char line[256];
	char declaration[512] = "";

	while (fgets(line, sizeof(line), fin) != NULL) {
		extract_from_line_to_declaration(line, declaration);
#ifdef NOT_YET
		if (is_complete(declaration)) {
			printf("Found declaration: '%s'\n", declaration);
		}
#endif /* NOT_YET */
	}
}

void extract_from_line_to_declaration(char* line, char* decl)
{
	static char blank = '\0';

	if (*line == '\0')
		*decl = blank;

	else if (isalpha(*line)) {
		char* end = last3chars (line);
		if (end[2] != ';') {
			filter_from_end (" {", end);
			strcpy(decl, line);
		}
	}
}

static char* last3chars (char* line)
{
	size_t len = strlen (line);
	const size_t LAST_3_CHARS = 3;

	if (len > LAST_3_CHARS)
		return &line[len - LAST_3_CHARS];
	else
		return line;
}


static int exclude_this_char(char* exclude, char ch)
{
	int flag = 0;
	while (*exclude != '\0') {
		if (*exclude == ch) {
			flag = 1;
			break;
		}
		exclude++;
	}
	return flag;
}

static void filter_from_end(char *exclude, char* line)
{
	char* cp = line + strlen(line) - 1 /* NULL_C */;

	while (cp >= line) {
		if (exclude_this_char(exclude, *cp)) {
			*cp = '\0';
			cp--;
		} else
			break;
	}
}

