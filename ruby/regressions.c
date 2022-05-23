#include "regressions.h"

/*
 * prototypes
 */
static int withspacesbeforelocalfunction (void);
static int localfunction (void);

/*
 * global
 * functions
 */
int nospace(void)
{ return 0; }

int withbrace(void ){
	return 0;
}

int withspacesaroundvoid( void )
{ return 0; }

int withspaceafterfunction (void)
{ return 0; }

int  withspacesbeforefunction(void)
{ return 0; }

char* alongfunctionwith_many_paramaters_across_multiple_lines(int a, int b,
		int c, int d, char* str1, char* str2, char* str3, char* str4,
		double d1, double d2,
		float f1, float f2)
{
	char string[256];
	int isum = a + b + c + d;
	double dproduct = d1 * d2;
	float fsum = f1 + f2;

	sprintf (string, "%d, %c, %c, %c, %c, %f, %f", isum, *str1, *str2, \
			*str3, *str4, dproduct, fsum);
	return string;
}

/*
 * private
 * functions
 */
static   int     withspacesbeforelocalfunction ( void )
{ return 0; }

static int localfunction ( void )
{ return 0; }
