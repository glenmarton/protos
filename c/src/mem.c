#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"

/*
 * local
 * variables
 */
#define FILENAME_LEN (size_t)75

static void (*logerr)(const char*, ...) = NULL;

/*
 * local
 * functions
 */
static void mem_error(size_t size, const char* alloc_name, const char*func_name)
{
	const char fmt[] = "%s() Failure: Could not allocate %zu bytes for %s:%s";

	if (logerr)
		logerr (fmt, alloc_name, size, func_name, strerror(errno));
	else
		fprintf (stderr, fmt, func_name, size, func_name, strerror(errno));
}

/*
 * global
 * functions
 */
void mem_init(void(*errlog)(const char*, ...))
{
	logerr = errlog;
}
void* mem_alloc(size_t size, const char*func_name)
{
	char* cp = NULL;

	if (func_name == NULL)
		logerr ("%s() Warning: No calling function name provided.", __func__);

	cp = (char*) malloc (size);
	if (cp == NULL)
		mem_error(size, __func__, func_name);

	return cp;
}
void* mem_calloc(size_t count, size_t size, const char *func_name)
{
	void *ptr;
	assert(count > 0);
	assert(size > 0);
	ptr = calloc(count, size);
	if (ptr == NULL) {
		mem_error(count, __func__, func_name);
	}
	return ptr;
}
void *mem_resize(void *ptr, size_t size, const char *func_name)
{
	assert(ptr);
	assert(size > 0);
	ptr = realloc(ptr, size);
	if (ptr == NULL) {
		mem_error(size, __func__, func_name);
	}
	return ptr;
}
char* mem_stringDuplicate (const char *src)
{
	char * cp;
	size_t size;

	if (!src)
		return NULL;

       size = strlen (src) + 1; /* +1 for '\0' */

	cp = mem_alloc (size, __func__);
	if (cp) {
		strcpy (cp, src);
	}

	return cp;
}

char* mem_grow(char*str, size_t *str_len, long min)
{
	char *cp = NULL;

	if (str == NULL)
		cp = MALLOC (*str_len);	/* first time through */

	else if (min < 0) {
		*str_len *= 2;				/* glibc 2.0 */
		cp = RESIZE(str, *str_len);

	} else {
		*str_len = (size_t)min +1;	/* glibc 2.1 */
		cp = RESIZE(str, *str_len);
	}

	if (0 && cp == NULL)
		FREE(str);	/* RESIZE failed so free str and return NULL. */

	return cp;
}
