/* conf.h				DATE: 2022-06-14
 * PURPOSE: Process argsv for project.
 */
#ifndef __CONF_H__
#define __CONF_H__
typedef struct conf_T {
	char* cfile;
	char* hfile;
	char* purpose;
	int testing;
} conf_t;

/* Global count = 3 */
conf_t* conf_create (void);
conf_t* conf_new (int argc, char* argv[]);
void conf_free (conf_t** confPtr);
#endif /* __CONF_H__ */
