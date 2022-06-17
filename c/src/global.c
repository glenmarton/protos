#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include "file.h"
#include "global.h"
#include "iso8601.h"
#include "Seq.h"

/*
 * prototypes
 */
static char* filename_to_macro (char* base);
static void write_boilerplate (FILE* fp, char* base, char* purpose, char* macro);
static void write_prototype_section (FILE* fp, Seq_T globals);
static void replace_char (char* str, char from, char to);
static void to_uppercase (char* str);

/*
 * global
 * functions
 */
void global_writeFile (conf_t* conf, Seq_T globals)
{
	int flag = file_exists (conf->hfile);

	fprintf (stderr, "Write: %s\n", conf->hfile);
	if (conf->testing)
		fprintf (stderr, "+%d %s  %s %s with %lu prototypes.\n",
				__LINE__, __FILE__, flag?"Update":"Generate", conf->hfile,
				Seq_length (globals));
	if( flag )
		global_updateFile (conf->hfile, globals);
	else
		global_generateFile (conf->hfile, conf->purpose, globals);
}

void global_updateFile (char* hfile, Seq_T globals)
{
	char original[120];
	FILE *newfp = NULL, *origfp = NULL;

	sprintf (original, "%s.orig", hfile);
	rename (hfile, original);

	newfp = fopen(hfile, "w");
	origfp = fopen (original, "r");

	if( newfp && origfp ) {
		file_copyUntil ("/* Global count = ", origfp, newfp);

		write_prototype_section (newfp, globals);

		char* macro = filename_to_macro (basename (hfile));
		fprintf (newfp, "#endif /* %s */\n", macro);
	}

	if( newfp )
		fclose (newfp);
	if( origfp )
		fclose (origfp);
	unlink (original);
}

void global_generateFile (char* hfile, char* purpose, Seq_T globals)
{
	char* base = basename (hfile);
	char* macro = filename_to_macro (base);
	FILE* fp = fopen(hfile, "w");

	if( fp ) {
		write_boilerplate(fp, base, purpose, macro);
		write_prototype_section (fp, globals);

		fprintf (fp, "#endif /* %s */\n", macro);

		fclose (fp);
	}
}

/*
 * local
 * functions
 */
static char* filename_to_macro (char* base)
{
	static char macro[64];

	sprintf(macro, "__%s__", base);
	replace_char(macro, '.', '_');
	to_uppercase(macro);
	return macro;
}

static void write_boilerplate(FILE* fp, char* base, char* purpose, char* macro)
{
	fprintf (fp, "/* %s\t\t\t\tDATE: %s\n", base, iso8601_date());
	fprintf (fp, " * PURPOSE: %s\n", purpose);
	fputs (" */\n", fp);
	fprintf (fp,"#ifndef %s\n", macro);
	fprintf (fp,"#define %s\n", macro);
	fputs ("\n", fp);
}

static void write_prototype_section (FILE* fp, Seq_T globals)
{
	fprintf (fp, "/* Global count = %lu */\n", Seq_length (globals));

	size_t n, stop = Seq_length (globals);

	for (n=0; n<stop; n++) {
		fprintf (fp, "%s\n", (char*)Seq_get(globals, n));
	}
}

static void replace_char(char* str, char from, char to)
{
	while (*str != '\0') {
		if (*str == from)
			*str = to;
		str++;
	}
}

static void to_uppercase(char* str)
{
	while (*str != '\0') {
		*str = (char)toupper(*str);
		str++;
	}
}
