#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "engine.h"

/*************************
 * Engine Initialization *
 *************************/

int engine_def_init(engine_def *def)
{
    def = malloc(sizeof(engine_def));
    def->indices = malloc(0);
    def->indices_len = 0;
    def->chunks = malloc(0);
    def->chunks_len = 0;
    return 0;
}

/* for psx implementation */
int engine_def_parse_from_memory(engine_def *def, unsigned char *buffer, int length)
{
    int ret;
    FILE *mock_file = fmemopen(buffer, length, "r");
    ret = engine_def_parse(def, mock_file);
    fclose(mock_file);
    return ret;
}

int engine_def_parse(engine_def *def, FILE *f)
{
    int offset = offsetof(engine_def, pos_x);
    def = malloc(sizeof(engine_def));
    fread((void*)def, sizeof(char), offset, f);
    fread(&(def->indices_len), sizeof(int), 1, f);
    fread(def->indices, sizeof(chunk_index), def->indices_len, f);
    fread(&(def->chunks_len), sizeof(int), 1, f);
    fread(def->chunks, sizeof(chunk), def->chunks_len, f);
    return 0;
}

#ifdef EDIT_MODE
int engine_def_write(engine_def *def, FILE *f)
{
    int offset = offsetof(engine_def, pos_x);
    fwrite(def, sizeof(char), offset, f);
    fwrite(def->indices_len, sizeof(int), 1, f);
    fwrite(def->indices, sizeof(chunk_index), def->indices_len, f);
    fwrite(def->chunks_len, sizeof(int), 1, f);
    fwrite(def->chunks, sizeof(chunk), def->chunks_len, f);
}


#endif
