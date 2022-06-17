#include <stdio.h>
#include <string.h>
#define MATCH 0

int file_exists(char* fname)
{
	int flag = 1;
	FILE* fp = fopen (fname, "r");

	if (fp)
		fclose (fp);
	else
		flag = 0;
	return flag;
}

void file_copyUntil (char* search, FILE* fin, FILE* fout)
{
	char line[120];

	while( fgets(line, sizeof(line), fin) != NULL) {
		if( strstr (line, search) ) {
			break;
		} else {
			fputs (line, fout);
			*line = '\0';
		}
	}
}

void file_scanFor (char* search, FILE* fin)
{
	char line[256];

	while (fgets(line, sizeof(line), fin) != NULL) {
		if( strcmp (line, search) == MATCH ) {
			break;
		}
	}
}
