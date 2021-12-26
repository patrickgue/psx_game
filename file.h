#ifndef _file_h
#define _file_h

typedef int FILE;

FILE *ffmemopen(void *, int, char *);
void ffread(void *, int, int, FILE*);
void ffwrite(void *, int, int, FILE*);
void ffclose(FILE*);

#endif
