#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"
#include "file.h"
#include "global.h"
#include "local.h"
#include "mem.h"
#include "protos.h"
#include "Seq.h"

/*
 * prototypes
 */
static Seq_T* read_file (FILE* fin);
static int a_local_prototype (char* prototype);
static int is_function_declaration (char* line);
static int has_end_of_function_decl (char* decl);
static int generate_prototype (char* line, char* decl);
static void record_multiline_declaration (char* line, char* decl);
static void filter_from_end (char *exclude, char* line);
static int exclude_this_char (char* exclude, char ch);
static int decl_continued_from_previous_line (char*decl);

/*
 * local
 * variables
 */
static int testing = -1;


/*
 * global
 * functions
 */
#ifndef CPPUTESTING
int main(int argc, char** argv)
{
	conf_t *conf = conf_new (argc, argv);
	Seq_T* prototypes = NULL;
	FILE* fin = NULL;

	testing = conf->testing;
	if (*conf->cfile == '\0') {
		printf("Usage: %s C-filename\n", argv[0]);
		return 1;
	}
	fin = fopen (conf->cfile, "r");

	if (fin) {
		fprintf(stderr, "Read file: %s, ", conf->cfile);

		prototypes = read_file (fin);
		fclose (fin);

		fprintf (stderr, "Counts: global=%lu, local=%lu\n",
				Seq_length (prototypes[0]),
				Seq_length (prototypes[1]));

		global_writeFile (conf, prototypes[0]);
		local_writeFile (conf, prototypes[1]);
		Seq_free (&prototypes[0]);
		Seq_free (&prototypes[1]);
		free (prototypes);
	}
}
#else
void mock(void) { read_file(stdin); }
#endif /* CPPUTESTING */
static Seq_T* read_file (FILE* fin)
{
	char line[256];
	char prototype[512] = "";
	Seq_T local_list = Seq_new(65);
	Seq_T global_list = Seq_new(15);

	read_past_function_prototype_list (fin);

	while (fgets(line, sizeof(line), fin) != NULL) {
		int found_one = extract_from_line_to_prototype(line, prototype);
		if (found_one) {
			if (testing)
				printf("Found prototype: '%s'\n", prototype);
			if (a_local_prototype(prototype)) {
				Seq_addhi(local_list, mem_stringDuplicate (prototype));
			} else {
				Seq_addhi(global_list, mem_stringDuplicate (prototype));
			}
			*prototype = '\0';
		}
	}
	Seq_T * prototypes = (Seq_T*)malloc (sizeof(Seq_T) * 2);
	prototypes[0] = global_list;
	prototypes[1] = local_list;
	/*
	write_prototypes (stderr, "Global", global_list);
	write_prototypes (stderr, "Local", local_list);
	*/
	return prototypes;
}

void read_past_function_prototype_list (FILE* fin)
{
	char line[80];

	fgets(line, sizeof(line), fin);
	file_scanFor (" * prototypes\n", fin);
	file_scanFor ("\n", fin);
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
