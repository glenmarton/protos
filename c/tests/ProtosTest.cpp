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
	char actual[9] = "";

	extract_from_line_to_prototype((char*)"", actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationGood)
{
	char expect[32] = "float add(float a, float b);";
	char actual[64] = "";

	extract_from_line_to_prototype((char*)"float add(float a, float b)", actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationPartial)
{
	char input[32] = "float add(float a,\n";
	char expect[32] = "float add(float a,\n";
	char actual[64] = "";

	extract_from_line_to_prototype(input, actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationMultiLine)
{
	char input[32] = "\tfloat b)\n";
	char expect[32] = "float add(float a,\n\tfloat b);";
	char actual[64] = "float add(float a,\n";

	extract_from_line_to_prototype(input, actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationTrippleLine)
{
	char input1[32] = "char* merge(char* a,\n";
	char input2[32] = "\tchar* b,\n";
	char input3[32] = "\tchar* c)\n";
	char expect[64] = "char* merge(char* a,\n\tchar* b,\n\tchar* c);";
	char actual[64] = "";

	extract_from_line_to_prototype(input1, actual);
	extract_from_line_to_prototype(input2, actual);
	extract_from_line_to_prototype(input3, actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclaration)
{
	char input[64] = "float add(float a, float b) { ";
	char expect[32] = "float add(float a, float b);";
	char actual[64] = "";
	extract_from_line_to_prototype((char*)input, actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationWithSpaces)
{
	char input[32] = "int inc(int a)   ";
	char expect[32] = "int inc(int a);";
	char actual[64] = "";

	extract_from_line_to_prototype(input, actual);
	STRCMP_EQUAL (expect, actual);
}

TEST(Protos, ExtractDeclarationWithClosingBrace)
{
	char input[32] = "int inc(int a){";
	char expect[32] = "int inc(int a);";
	char actual[64] = "";

	extract_from_line_to_prototype(input, actual);
	STRCMP_EQUAL (expect, actual);
}
/* Tests for extract_from_line_to_prototype.
float add(float a, float b){ 
int inc(int a) {
int dec(int a) 
	add(1.0,
int process(float f,
	int i,
	char c) {
*/
