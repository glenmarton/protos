#include <stdio.h>
#include <unistd.h>
#include "file.h"
#include "local.h"

/*
 * prototypes
 */
static void write_prototype_section (FILE* fp, Seq_T locals);

/*
 * global
 * functions
 */
void local_writeFile (conf_t* conf, Seq_T locals)
{
	if (conf->testing)
		fprintf (stderr, "Writing %s with %lu prototypes.\n", conf->cfile,
				Seq_length (locals));
	fprintf (stderr, "Write: %s\n", conf->cfile);

	local_generateFile (conf->cfile, locals);
}

void local_generateFile (char* cfile, Seq_T locals)
{
	char original[80];
	FILE *fin = NULL;
	FILE *fout = NULL;
	sprintf (original, "%s.orig", cfile);
	rename (cfile, original);
	fin = fopen (original, "r");
	fout = fopen (cfile, "w");

	if( fin && fout ) {
		char line[120];
		file_copyUntil(" * prototypes", fin, fout);
		fputs (" * prototypes\n */\n", fout);
		file_scanFor ("\n", fin);
		write_prototype_section (fout, locals);
		while(fgets(line, sizeof(line), fin))
			fputs (line, fout);
	} else {
		rename (original, cfile);
	}
	if (fin)
		fclose (fin);
	if (fout)
		fclose (fout);
	unlink (original);
}

static void write_prototype_section (FILE* fp, Seq_T locals)
{
	size_t n, stop = Seq_length (locals);

	for (n=0; n<stop; n++) {
		fprintf (fp, "%s\n", (char*)Seq_get(locals, n));
	}
	fputs ("\n", fp);
}
