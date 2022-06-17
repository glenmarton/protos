/* global.h				DATE: 2022-06-16
 * PURPOSE: Handle writing global prototypes to header file.
 */
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "conf.h"
#include "Seq.h"

/* Global count = 3 */
void global_writeFile (conf_t* conf, Seq_T globals);
void global_updateFile (char* hfile, Seq_T globals);
void global_generateFile (char* hfile, char* purpose, Seq_T globals);
#endif /* __GLOBAL_H__ */
