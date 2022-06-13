/* $Id: array.c 6 2007-01-22 00:45:22Z drhanson $ */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Array.h"
#include "Arrayrep.h"
#include "mem.h"
#define T Array_T
T Array_new(size_t length, size_t size)
{
	T array;
	NEW(array);
	if (length > 0)
		ArrayRep_init(array, length, size,
			CALLOC(length, size));
	else
		ArrayRep_init(array, length, size, NULL);
	return array;
}
void ArrayRep_init(T array, size_t length, size_t size, void *ary)
{
	assert(array);
	assert((ary && length>0) || (length==0 && ary==NULL));
	assert(size > 0);
	array->length = length;
	array->size   = size;
	if (length > 0)
		array->array = ary;
	else
		array->array = NULL;
}
void Array_free(T *array) {
	assert(array && *array);
	FREE((*array)->array);
	FREE(*array);
}
void *Array_get(T array, size_t i) {
	assert(array);
	assert( i < array->length );
	return array->array + i*array->size;
}
void *Array_put(T array, size_t i, void *elem) {
	assert(array);
	assert( i < array->length );
	assert(elem);
	memcpy(array->array + i*array->size, elem,
		array->size);
	return elem;
}
size_t Array_length(T array) {
	assert(array);
	return array->length;
}
size_t Array_size(T array) {
	assert(array);
	return array->size;
}
void Array_resize(T array, size_t length)
{
	assert(array);
	//assert(length >= 0);
	if (length == 0) {
		FREE(array->array);
	} else if (array->length == 0) {
		array->array = ALLOC(length*array->size);
	} else {
		RESIZE(array->array, length*array->size);
	}
	array->length = length;
}
T Array_copy(T array, size_t length) {
	T copy;
	assert(array);
	//assert(length >= 0);
	copy = Array_new(length, array->size);
	if (copy->length >= array->length
	&& array->length > 0)
		memcpy(copy->array, array->array,
			array->length*array->size);
	else if (array->length > copy->length
	&& copy->length > 0)
		memcpy(copy->array, array->array,
			copy->length*array->size);
	return copy;
}
