#include "file.h"
#include <stdio.h>
#include <stdlib.h>


/*************************
 * mock files and struct *
 *************************/

struct fake_file
{
    FILE       index;
    char       *buffer;
    int        buffer_len;
    int        buffer_pos;
};

struct fake_file *files = NULL;
int files_count = 0;



FILE *ffmemopen(void *buff, int leng, char *mode)
{
    if (files == NULL)
        files = malloc(sizeof(struct fake_file));
}

void ffread(void *, int, int, FILE*);
void ffwrite(void *, int, int, FILE*);
void ffclose(FILE *file)
{
    if (file != NULL)
        free(file);
}