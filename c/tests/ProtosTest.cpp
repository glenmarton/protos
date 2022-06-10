#include <iostream>
using namespace std;

extern "C"
{
#include <stdbool.h>
#include <string.h>
#include "protos.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Protos)
{
    void setup()
    {
      //Protos_Create();
    }

    void teardown()
    {
       //Protos_Destroy();
    }
};

TEST(Protos, ExtractDeclarationBlankLine)
{
	char expect[2] = "";
	char actual[9] = "Unset";

	extract_from_line_to_declaration((char*)"", actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationGood)
{
	char expect[32] = "float add(float a, float b)";
	char actual[64] = "Unset";

	extract_from_line_to_declaration((char*)"float add(float a, float b)", actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclaration)
{
	char input[64] = "float add(float a, float b) { ";
	char expect[32] = "float add(float a, float b)";
	char actual[64] = "Unset";
	extract_from_line_to_declaration((char*)input, actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationWithSpaces)
{
	char input[32] = "int inc(int a)   ";
	char expect[32] = "int inc(int a)";
	char actual[64] = "Unset";

	extract_from_line_to_declaration(input, actual);
	STRCMP_EQUAL (expect, actual);
}
/* Tests for extract_from_line_to_declaration.
float add(float a, float b){ 
int inc(int a) {
int dec(int a) 
	add(1.0,
int process(float f,
	int i,
	char c) {
*/
