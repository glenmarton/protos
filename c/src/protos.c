#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "protos.h"
#include "Seq.h"

/*
 * prototypes
 */
static void read_file (FILE* fin);
static int a_local_prototype (char* prototype);
static int is_function_declaration (char* line);
static int has_end_of_function_decl (char* decl);
static int generate_prototype (char* line, char* decl);
static void record_multiline_declaration (char* line, char* decl);
static void filter_from_end (char *exclude, char* line);
static int exclude_this_char (char* exclude, char ch);
static int decl_continued_from_previous_line (char*decl);

/*
 * global
 * functions
 */
#ifndef CPPUTESTING
int main(int argc, char** argv)
{
	if (argc > 1) {
		FILE* fin = fopen (argv[1], "r");
		fprintf(stderr, "+%d %s - reading file %s\n", __LINE__,
				__FILE__, argv[1]);
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
	char prototype[512] = "";
	Seq_T local_list = Seq_new(15);
	Seq_T global_list = Seq_new(15);

	read_past_function_prototype_list (fin);

	while (fgets(line, sizeof(line), fin) != NULL) {
		int found_one = extract_from_line_to_prototype(line, prototype);
		if (found_one) {
			printf("Found prototype: '%s'\n", prototype);
			if (a_local_prototype(prototype)) {
				Seq_addlo(local_list, prototype);
			} else {
				Seq_addlo(global_list, prototype);
			}
			*prototype = '\0';
		}
	}
	fprintf (stderr, "+%d %s  Counts: local = %lu, global = %lu\n", __LINE__,
			__FILE__, Seq_length(local_list),
			Seq_length(global_list));
}

void read_past_function_prototype_list (FILE* fin)
{
	char line[256];
	enum read_states {
		STOP_READING, SCAN_FOR_BLANK_LINE, SCAN_FOR_PROTOTYPES
	} keep_going = SCAN_FOR_PROTOTYPES;

	while (keep_going && fgets(line, sizeof(line), fin) != NULL) {
		if (keep_going == SCAN_FOR_PROTOTYPES &&
				strstr (line, "* prototypes") != NULL ) {
			keep_going = SCAN_FOR_BLANK_LINE;
		} else if( keep_going == SCAN_FOR_BLANK_LINE &&
				strstr (line, "") != NULL) {
			keep_going = STOP_READING;
		}
	}
}

/*
 *   d e c l a r a t i o n
 */
/* A function prototype consists of a function declaration then a semicollon. */
int extract_from_line_to_prototype(char* line, char* decl)
{
	static char blank = '\0';
	int flag = 0;

	if( is_function_declaration(line) && has_end_of_function_decl(line) )
		flag = generate_prototype (line, decl);
	else if( is_function_declaration(line) )
		record_multiline_declaration(line, decl);
	else if( decl_continued_from_previous_line(decl) && ! has_end_of_function_decl(line) )
		record_multiline_declaration (line, decl);
	else if( decl_continued_from_previous_line(decl) )
		flag = generate_prototype (line, decl);
	else
		*decl = blank;
	return flag;
}

static int a_local_prototype(char* prototype)
{
	return strstr(prototype, "static") != NULL;
}

static int is_function_declaration(char* line)
{
	return (isalpha(*line) && !strchr(line, ';'));
}

static int has_end_of_function_decl(char* decl)
{
	return decl != NULL && strrchr(decl, ')') != NULL;
}

static int generate_prototype(char* line, char* decl)
{
	filter_from_end (" {\n", line);
	strcat(decl, line);
	strcat(decl, ";"); // Convert function declaration to prototype.
	return 1;
}

static void record_multiline_declaration(char* line, char* decl)
{
		filter_from_end (" {", line);
		strcat(decl, line);
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

static int decl_continued_from_previous_line(char*decl)
{
	return *decl != '\0';
}
