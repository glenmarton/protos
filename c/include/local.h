/* local.h				DATE: 2022-06-16
 * PURPOSE: Process prototypes inside C file.
 */
#ifndef __LOCAL_H__
#define __LOCAL_H__
#include "conf.h"
#include "Seq.h"

/* Global count = 2 */
void local_writeFile (conf_t* conf, Seq_T locals);
void local_generateFile (char* cfile, Seq_T locals);
#endif /* __LOCAL_H__ */
