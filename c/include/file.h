#ifndef __FILE_H__
#define __FILE_H__

/* Global count = 2 */
int file_exists(char* fname);
void file_copyUntil (char* search, FILE* fin, FILE* fout);
void file_scanFor (char* search, FILE* fp);
#endif /* __FILE_H__ */
