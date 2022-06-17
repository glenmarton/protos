#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "conf.h"

/*
 * local
 * variables
 */
#define VERSION "0.0.1"
static char blank = '\0';
static char *alt_dir = "";
static char *purpose = "Fill in purpose.";
static int testing = false;

/*
 * prototypes
 */
static void alloc_all_empty (conf_t* conf);
static char* duplicate_str (char* str);
static char* build_header_filename (char* alt_dir, char* file);
static char* conf2str (conf_t* conf);
static int process_flags (int argc, char* argv[]);
static void fwrite_args (FILE* fp, int argc, char* argv[]);
static void replace_file_extension (char* fname, char ext);
static void usage (char *progname, char *flag_list);

/*
 * global
 * functions
 */

conf_t* conf_create(void)
{
	conf_t * ptr = (conf_t*) malloc (sizeof(conf_t));
	alloc_all_empty(ptr);
	return ptr;
}

conf_t* conf_new(int argc, char* argv[])
{
	conf_t* conf = NULL;
	int n = process_flags(argc, argv);

	/* Adjust for processed flags. */
	argc -= n;
	argv += n;

	if( argc > 0 ) {
		conf = (conf_t*) malloc(sizeof(conf_t));

		conf->cfile = duplicate_str (argv[0]);

		conf->hfile = build_header_filename (alt_dir, conf->cfile);

		conf->purpose = duplicate_str (purpose);
		conf->testing = testing;
	} else {
		conf = conf_create();
	}
if(testing) fprintf(stderr, "+%d %s  count=%d, configuration: %s\n", __LINE__, __FILE__, n, conf2str(conf));
	return conf;
}

void conf_free(conf_t** confPtr)
{
	if (confPtr && *confPtr) {
		conf_t* ptr = *confPtr;
		if( ptr->cfile )
			free (ptr->cfile);

		if( ptr->hfile )
			free (ptr->hfile);

		if( ptr->purpose )
			free (ptr->purpose);
		free (ptr);
		*confPtr = NULL;
	}
	alt_dir = (char*)&blank;
	purpose = "Fill in purpose.";
}

/*
 * local
 * functions
 */
static void alloc_all_empty(conf_t* conf)
{
	conf->cfile = duplicate_str (&blank);
	conf->hfile = duplicate_str (&blank);
	conf->purpose = duplicate_str (&blank);
}

static char* duplicate_str(char* str)
{
	size_t sz = strlen (str);
	char* newstr = (char*)malloc(sz + 1 /* NULL_C */);
	if( newstr ) {
		strcpy (newstr, str);
	} else {
		newstr = (char*) malloc(1);
		*newstr = '\0';
	}
	return newstr;
}


static char* build_header_filename (char* alt_dir, char* file)
{
	char* hfile = NULL;
	if (!file) {
		return (char*)&blank;
	} else if (testing) {
		fprintf (stderr, "%s(%s, %s)\n", __func__, alt_dir, file);
	}

	if (*alt_dir == '\0') {
		hfile = duplicate_str (file);
	} else {
		size_t size = strlen (alt_dir) + strlen (file) + 2 /* / and NULL_C */;
		hfile = (char*) malloc (size);
		sprintf (hfile, "%s/%s", alt_dir, file);
	}
	replace_file_extension(hfile, 'h');
	return hfile;
}

static char* conf2str(conf_t* conf)
{
	static char string[128];
	sprintf (string, "cfile:%s, hfile:%s, Purpose:'%s'\n", conf->cfile, conf->hfile, conf->purpose);
	return string;
}

static int process_flags(int argc, char* argv[])
{
	int ch;
	char flag_list[] = "H:hp:tv";

	if (testing)
		fwrite_args (stderr, argc, argv);

	while ((ch = getopt(argc, argv, flag_list)) != -1) {
		if(testing) fprintf (stderr, "+%d %s   Processing option '%c'\n", __LINE__, __FILE__, ch);
		switch (ch) {
			case 'H':
				alt_dir = optarg;
		if(testing) fprintf (stderr, "+%d %s   \talternate dir='%s'\n", __LINE__, __FILE__, alt_dir);
				break;
			case 'p':
				purpose = optarg;
		if(testing) fprintf (stderr, "+%d %s   \tpurpose='%s'\n", __LINE__, __FILE__, purpose);
				break;
			case 't':
				testing = true;
				break;
			case 'v':
				printf("%s %s\n", argv[0], VERSION);
				exit(0);
			case 'h':
				/* fall through */
			default:
				usage(argv[0], flag_list);
				exit(0);
		}
	}
	return optind;
}

static void fwrite_args (FILE* fp, int argc, char* argv[])
{
	int i;
	fprintf (fp, "+%d %s  argc=%d\n", __LINE__, __FILE__, argc);
	for( i=0; i<argc; i++) {
		fprintf (fp, "+%d %s  argv[%d]='%s'\n", __LINE__, __FILE__, i, argv[i]);
	}
}

static void replace_file_extension(char* fname, char ext)
{
	char* cp = strrchr(fname, '.');
	if (cp) {
		cp[1] = ext;
	}
}

static void usage(char *progname, char *flag_list)
{
	fprintf(stderr, "\n"
			"usage: %s [-%s]\n"
			"	-H alternate header dir\n"
			"	-h help, this message\n"
			"	-t testing\n"
			"	-v version\n\n", progname, flag_list);
}



