//#include <iostream>
//using namespace std;

extern "C"
{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file.h"
#include "global.h"
#include "iso8601.h"
#include "Seq.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Global)
{
	void setup()
	{
		//Global_Create();
	}

	void teardown()
	{
		//Global_Destroy();
	}
	int write_original(char* fname)
	{
		int flag = 1;
		FILE* fp = NULL;
		unlink (fname);
		fp = fopen(fname, "w");
		if (fp) {
			fputs ("/* inc.h\n" \
				" */\n" \
				"#ifndef __INC_H__\n" \
				"#define __INC_H__\n" \
				"#define MY_MAX_INT 65535\n" \
				"\n" \
				"/* Global count = 1 */\n" \
				"int inc(int a);\n" \
				"#endif /* __INC_H__ */\n",
				fp);
			fclose (fp);
		} else
			flag = 0;
		return flag;
	}
};

TEST(Global, GenerateFile)
{
	//   S e t u p
	char hfile[] = "/tmp/inc.h";
	char purpose[] = "Increment ints";
	char actual[84] = "";
	int n = 0;
	char expectations[][64] = {
		{""},
		{" * PURPOSE: Increment ints\n"},
		{" */\n"},
		{"#ifndef __INC_H__\n"},
		{"#define __INC_H__\n"},
		{"\n"},
		{"/* Global count = 1 */\n"},
		{"int inc(int a);\n"},
		{"#endif /* __INC_H__ */\n"}
	};
	sprintf(expectations[0], "/* inc.h\t\t\t\tDATE: %s\n", iso8601_date());

	Seq_T globals = Seq_new(5);
	char prototype[32] = "int inc(int a);";

	Seq_addlo(globals, prototype);

	//   T e s t   r u n
	global_generateFile (hfile, purpose, globals);

	//   V a l i d a t i o n
	CHECK_TRUE (file_exists (hfile));
	FILE* fp = fopen (hfile, "r");
	CHECK_TRUE (fp);

	while( fgets(actual, sizeof(actual), fp) != NULL) {
		STRCMP_EQUAL (expectations[n], actual);
		n++;
	}

	//   c l e a n   u p
	fclose(fp);
	Seq_free (&globals);
	unlink (hfile);
}

TEST(Global, UpdateFile)
{
	//   S e t u p
	char hfile[] = "/tmp/inc.h";
	char actual[84] = "";
	unsigned n = 0;
	char expectations[10][64] = {
		{"/* inc.h\n"},
		{" */\n"},
		{"#ifndef __INC_H__\n"},
		{"#define __INC_H__\n"},
		{"#define MY_MAX_INT 65535\n"},
		{"\n"},
		{"/* Global count = 2 */\n"},
		{"int inc(int a);\n"},
		{"int dec(int a);\n"},
		{"#endif /* __INC_H__ */\n"}
	};

	Seq_T globals = Seq_new(4);
	char prototype_1[32] = "int inc(int a);";
	char prototype_2[32] = "int dec(int a);";

	Seq_addhi(globals, prototype_1);
	Seq_addhi(globals, prototype_2);

	CHECK_TRUE (write_original (hfile));

	//   T e s t   r u n
	global_updateFile (hfile, globals);

	//   V a l i d a t i o n
	CHECK_TRUE (file_exists (hfile));

	FILE* fp = fopen (hfile, "r");
	CHECK_TRUE (fp);

	while( fgets(actual, sizeof(actual), fp) != NULL) {
		STRCMP_EQUAL (expectations[n], actual);
		n++;
	}

	size_t expect = sizeof(expectations)/sizeof(expectations[0]);
	if( n == expect )
	CHECK_TRUE (n == expect);

	//   c l e a n   u p
	fclose(fp);
	Seq_free (&globals);
	unlink (hfile);
}

IGNORE_TEST(Global, GenerateFile)
{
	char expect[64] = "char* merge(char* a,\n\tchar* b,\n\tchar* c);";
	char actual[64] = "";

	STRCMP_EQUAL (expect, actual);
}

IGNORE_TEST(Global, Macro)
{
	char expect[32] = "float add(float a, float b);";
	char actual[64] = "";
	STRCMP_EQUAL (expect, actual);
}
