/* $Id: array.h 6 2007-01-22 00:45:22Z drhanson $ */
#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED
#define T Array_T
typedef struct array_t *T;
extern T    Array_new (size_t length, size_t size);
extern void Array_free(T *array);
extern size_t Array_length(T array); /* how many elements. */
extern size_t Array_size  (T array); /* how long an elemnt is. */
extern void *Array_get(T array, size_t i);
extern void *Array_put(T array, size_t i, void *elem);
extern void Array_resize(T array, size_t length);
extern T    Array_copy  (T array, size_t length);
#undef T
#endif
