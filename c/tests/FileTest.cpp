//#include <iostream>
//using namespace std;

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(File)
{
	void setup()
	{
		//File_Create();
	}

	void teardown()
	{
		//File_Destroy();
	}

	int write_original(char* fname)
	{
		int flag = 1;
		FILE* fp = NULL;
		unlink (fname);
		fp = fopen(fname, "w");
		if (fp) {
			fputs ( "/* file.h\n" \
				" */\n" \
				"#ifndef __FILE_H__\n" \
				"#define __FILE_H__\n" \
				"\n" \
				"/* Global count = 1 */\n" \
				"int file(int a);\n" \
				"#endif /* __FILE_H__ */\n",
				fp);
			fclose (fp);
		} else
			flag = 0;
		return flag;
	}
};

TEST(File, FileExists)
{
	char fn[32] = "/tmp/myfile.txt";
	FILE* fp = fopen(fn, "w");
	int flag = -1;
	if (fp) {
		fprintf (fp, "File exists.\n");
		fclose (fp);
	} else {
		fprintf (stderr, "Failed to setup test.\n");
	}

	flag = file_exists(fn);
	CHECK_TRUE(flag);

	unlink(fn);
}

TEST(File, FileAbsent)
{
	char fn[32] = "/tmp/no_file.txt";

	CHECK_FALSE(file_exists(fn));
}

TEST(File, CopyUntil)
{
	char fname[15] = "/tmp/file.h";
	char orig[20];
	char actual[84] = "";
	unsigned n = 0;
	char expectations[][64] = {
		{"/* file.h\n"},
		{" */\n"},
		{"#ifndef __FILE_H__\n"},
		{"#define __FILE_H__\n"},
		{"\n"}
	};
	CHECK_TRUE (write_original (fname));
	
	sprintf (orig, "%s.orig", fname);
	rename (fname, orig);

	FILE* fin = fopen (orig, "r");
	FILE* fout = fopen (fname, "w");

	//   T e s t   r u n
	if( fin && fout ) {
		file_copyUntil ((char*)"/* Global count = 1 */\n", fin, fout);
	}
	if( fin )
		fclose (fin);
	if( fout )
		fclose (fout);

	//   V a l i d a t i o n
	CHECK_TRUE (file_exists (fname));
	fin = fopen (fname, "r");
	CHECK_TRUE (fin);

	while( fgets(actual, sizeof(actual), fin) != NULL) {
		STRCMP_EQUAL (expectations[n], actual);
		n++;
	}

	size_t expect = sizeof(expectations)/sizeof(expectations[0]);
	if(  n != expect )
		fprintf (stderr, "+%d %s  n=%d, expect=%lu\n", __LINE__, __FILE__, n, expect);
	CHECK_TRUE (n == expect);

	//   c l e a n   u p
	fclose(fin);
	unlink (fname);
}

TEST(File, ScanFor)
{
	char fname[15] = "/tmp/file.h";
	char actual[80];
	char expect[80] = "/* Global count = 1 */\n";
	FILE* fin = NULL;

	CHECK_TRUE (write_original (fname));
	fin = fopen (fname, "r");
	if (fin) {
		file_scanFor((char*)"\n", fin);
		fgets (actual, sizeof (actual), fin);
		STRCMP_EQUAL (expect, actual);
	} else {
		fprintf (stderr, "FAILURE: Unable to read %s.\n", fname);
	}
}
