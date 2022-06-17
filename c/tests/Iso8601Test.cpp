#include <iostream>
using namespace std;

extern "C"
{
#include <stdio.h>
#include <time.h>
#include "iso8601.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(Iso8601)
{
    void setup()
    {
      //iso8601_Create();
    }

    void teardown()
    {
       //iso8601_Destroy();
    }
};

TEST(Iso8601, date)
{
	time_t now = time(NULL);
	struct tm* ptr = localtime(&now);
	char expect[12] = "YYYY-MM-DD";
	strftime(expect, sizeof(expect), "%Y-%m-%d", ptr);

	char* actual = iso8601_date();

	STRCMP_EQUAL (expect, actual);
}


