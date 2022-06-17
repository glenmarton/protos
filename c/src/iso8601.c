#include <time.h>

char* iso8601_date(void)
{
	static char _8601[12];
	time_t now = time (NULL);
	struct tm* ptr = localtime (&now);
	strftime (_8601, sizeof(_8601), "%Y-%m-%d", ptr);
	return _8601;
}
