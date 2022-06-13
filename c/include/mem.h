#ifndef __MEM_H__
#define __MEM_H__

#define ALLOC(x)                mem_alloc((x), __func__)
#define MALLOC(x)               (char*)mem_alloc((x), __func__)
#define NEW(p)                  ((p) = (void*)MALLOC(sizeof *(p)))
#define NEW0(p)                 ((p) = CALLOC(1, (long)sizeof *(p)))
#define CALLOC(count, nbytes)   mem_calloc((count), (nbytes), __func__)
#define FREE(x)                 if((x)){free((x)); x=NULL;}
#define RESIZE(ptr, nbytes)     ((ptr) = mem_resize((ptr), (nbytes), __func__))

void mem_init(void(*errlog)(const char*, ...));
void* mem_alloc(size_t size, const char*func_name);
void* mem_calloc(size_t count, size_t size, const char *func_name);
void* mem_resize(void *ptr, size_t size, const char *func_name);
char* mem_stringDuplicate (const char *src);
char* mem_grow(char*str, size_t *str_size, long min);
#endif /* __MEM_H__ */
