#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * prototypes
 */
static void read_file(FILE* fin);
static char* last3chars (char* line);
static void remove_from_end_of_line(char* line, char ch);

/*
 * global
 * functions
 */
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

static void read_file (FILE* fin)
{
	char line[256];

	while (fgets(line, sizeof(line), fin) != NULL) {
		if (isalpha(*line)) {
			char* cp = last3chars (line);
			if (cp[2] == ';') {
				/* end with ;, not a function declaration */
			} else 	{
				remove_from_end_of_line (cp, '{');
				remove_from_end_of_line (cp, ' ');
				printf("%s\n", line);
			}
		}
	}
}

static char* last3chars (char* line)
{
	size_t len = strlen (line);
	return &line[len - 4];
}

static void remove_from_end_of_line(char* line, char ch)
{
	char* cp = line;

	while( *cp != '\0' ) {
		if( *cp == ch ) {
			*cp = '\0';
		}
		cp++;
	}
}
