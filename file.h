#ifndef _file_h
#define _file_h

#include "engine_meta.h"

#ifdef PSX
typedef int FILE;
#define MFILE FILE
#define FMEMOPEN fmemopen
#define FREAD fread
#define FWRITE fwrite
#define FCLOSE fclose
#else
typedef int FFILE;
#define MFILE FFILE
#define FMEMOPEN ffmemopen
#define FREAD ffread
#define FWRITE ffwrite
#define FCLOSE ffclose
#endif

MFILE *FMEMOPEN(void *, int, char *);
int FREAD(void *, int, int, const MFILE*);
int FWRITE(void *, int, int, const MFILE*);
int FCLOSE(MFILE*);

#endif
