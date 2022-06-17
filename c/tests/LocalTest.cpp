//#include <iostream>
//using namespace std;

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
#include "local.h"
#include "Seq.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Local)
{
	void setup()
	{
		//Local_Create();
	}

	void teardown()
	{
		//Local_Destroy();
	}
	int write_original(char* fname)
	{
		int flag = 1;
		FILE* fp = NULL;
		unlink (fname);
		fp = fopen(fname, "w");
		if (fp) {
			fputs ("\n" \
				"/*\n" \
				" * prototypes\n" \
				" */\n" \
				"static int bad(int a);\n" \
				"static int ignore(int b);\n" \
				"\n" \
				"int main(void)\n" \
				"{return 0;}\n" \
				"\n" \
				"static int inc(int a)\n" \
				"{return ++a;}\n" \
				"static int dec(int b)\n" \
				"{return --b;}\n",
				fp);
			fclose (fp);
		} else
			flag = 0;
		return flag;
	}
};

TEST(Local, GenLocalFile)
{
	//   S e t u p
	char cfile[] = "/tmp/int.c";
	char actual[84] = "";
	size_t n = 0;
	char expectations[][64] = {
		{"\n"},
		{"/*\n"},
		{" * prototypes\n"},
		{" */\n"},
		{"static int inc(int a);\n"},
		{"static int dec(int b);\n"},
		{"\n"},
		{"int main(void)\n"},
		{"{return 0;}\n"},
		{"\n"},
		{"static int inc(int a)\n"},
		{"{return ++a;}\n"},
		{"static int dec(int b)\n"},
		{"{return --b;}\n"}
	};

	Seq_T locals = Seq_new(5);
	char prototype_1[32] = "static int inc(int a);";
	char prototype_2[32] = "static int dec(int b);";

	Seq_addhi(locals, prototype_1);
	Seq_addhi(locals, prototype_2);
	CHECK_TRUE (write_original (cfile));

	//   T e s t   r u n
	local_generateFile (cfile, locals);

	//   V a l i d a t i o n
	CHECK_TRUE (file_exists (cfile));

	FILE* fp = fopen (cfile, "r");
	CHECK_TRUE (fp);

	while( fgets(actual, sizeof(actual), fp) != NULL) {
		STRCMP_EQUAL (expectations[n], actual);
		n++;
	}
	size_t expect = sizeof (expectations) / sizeof (expectations[0]);
	if (n != expect)
		fprintf (stderr, "\n%s:%d  FAILURE: n=%lu, expect=%lu", __FILE__,
				__LINE__, n, expect);
	CHECK_TRUE (n == expect);

	//   c l e a n   u p
	fclose(fp);
	Seq_free (&locals);
	unlink (cfile);
}


