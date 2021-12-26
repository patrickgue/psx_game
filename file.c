#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************
 * Local Definitions *
 *********************/

#ifdef PSX
struct fake_file *find_fake_file(const FILE *);
#else
struct fake_file *find_fake_file(const FFILE *);
#endif

/*************************
 * mock files and struct *
 *************************/

struct fake_file
{
#ifdef PSX
    FILE       index;
#else
    FFILE      index;
#endif
    char       *buffer;
    int        buffer_len;
    int        buffer_pos;
};


struct fake_file *fake_files = NULL;
int files_count = 0;




/***********************
 * Fake File Functions *
 ***********************/


MFILE *FMEMOPEN(void *buff, int len, char *mode)
{
#ifdef PSX
    if (files_count == 0)
        fake_files = malloc(sizeof(struct fake_file) * 16);
#else
    if (files_count == 0)
        fake_files = malloc(sizeof(struct fake_file));
    else
        fake_files = realloc(fake_files, sizeof(struct fake_file) * (files_count + 1));
#endif

    fake_files[files_count].buffer_pos = 0;
    fake_files[files_count].buffer_len = len;
    fake_files[files_count].buffer = (char*) buff;
    fake_files[files_count].index = files_count;

    return &(fake_files[files_count++].index);
}

int FREAD(void *buff, int size, int len, const MFILE *f)
{
    struct fake_file *current_file = find_fake_file(f);
    int buff_size = 0;

    if (size * len < current_file->buffer_len - current_file->buffer_pos)
    {
        buff_size = size * len;
        memcpy(buff, current_file->buffer + current_file->buffer_pos, buff_size);
    }
    else if (current_file->buffer_len - current_file->buffer_pos > 0)
    {
        buff_size = current_file->buffer_len - current_file->buffer_pos;
        memcpy(buff, current_file->buffer + current_file->buffer_pos, buff_size);
    }
    else
    {
        buff = 0;
    }
    current_file->buffer_pos += buff_size;
    return buff_size;
}


int FWRITE(void *buff, int size, int len, const MFILE *f)
{
    struct fake_file *current_file = find_fake_file(f);
    int buff_size = size * len;
    if (current_file->buffer_len == 0)
    {
#ifdef PSX
        /* limit buffer size in PSX to 4096 (or write length if greater) bytes because no realloc is available and memory is limited */
        current_file->buffer = malloc(buff_size > 4096 ? buff_size : 4096);
#else
        current_file->buffer = malloc(buff_size);
#endif
    }
    else
    {
        /* writing to buffer may fail on PSX if buffer exceeds 4096 bytes, only reallocating on other platforms */
#ifndef PSX
        current_file->buffer = realloc(current_file->buffer, current_file->buffer_len + buff_size);
#endif
    }

    memcpy(current_file->buffer + current_file->buffer_len, buff, buff_size);
    current_file->buffer_len = current_file->buffer_pos = current_file->buffer_len + buff_size;

    return current_file->buffer_len;
}

int FCLOSE(MFILE *file)
{
    // TODO: to be implemented
    // struct fake_file *current_file = find_fake_file(file);
    // if (current_file->buffer != NULL)
    //     free(current_file->buffer);
}




/************************
* Local Implementations *
*************************/

struct fake_file *find_fake_file(const MFILE *f)
{
    struct fake_file *current_file;
    int i;
    for (i = 0; i < files_count; i++)
    {
        if (fake_files[i].index == *f)
            current_file = &fake_files[i];
    }
    return current_file;
}