/* $Id: arrayrep.h 6 2007-01-22 00:45:22Z drhanson $ */
#ifndef ARRAYREP_INCLUDED
#define ARRAYREP_INCLUDED
#define T Array_T
struct array_t {
	size_t length;
	size_t size;
	char *array;
};
extern void ArrayRep_init(T array, size_t length, size_t size, void *ary);
#undef T
#endif
