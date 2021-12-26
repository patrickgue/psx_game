#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "engine.h"

/***********
 * Globals *
 ***********/

chunk *mock_chunk = NULL;

/*************************
 * Engine Initialization *
 *************************/

int engine_def_init(engine_def *def)
{
    def = malloc(sizeof(engine_def));
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
    int offset = offsetof(engine_def, chunks_len);
    fread((void*)def, sizeof(char), offset, f);
    fread(&(def->chunks_len), sizeof(int), 1, f);
    def->chunks = malloc(sizeof(chunk) * def->chunks_len);
    fread(def->chunks, sizeof(chunk), def->chunks_len, f);
    return 0;
}

chunk *engine_get_chunk(engine_def *def, int x, int y)
{
    int i;

    for (i = 0; i < def->chunks_len; i++)
    {
        if (def->chunks[i].x == x && def->chunks[i].y == y)
        {
            return &def->chunks[i];
        }
    }

    /* if not found, return mock chunk */
    if (mock_chunk == NULL) {
        mock_chunk = malloc(sizeof(chunk));
        mock_chunk->x = x;
        mock_chunk->y = y;
        for (i = 0; i < CHUNK_SIZE; i++)
            mock_chunk->set[i] = 0;
    }
    return mock_chunk;
}

tile_type engine_chunk_get_tile(engine_def *def, chunk c, int index_x, int index_y)
{
    int index = c.set[(index_y << 5) + index_x];
    return def->tile_types[index];
}

#ifdef EDIT_MODE
int engine_def_write(engine_def *def, FILE *f)
{
    int offset = offsetof(engine_def, chunks_len);
    fwrite(def, sizeof(char), offset, f);
    fwrite(&(def->chunks_len), sizeof(int), 1, f);
    fwrite(def->chunks, sizeof(chunk), def->chunks_len, f);
}

void engine_set_chunk(engine_def  *def, chunk *c)
{
    int index = -1, i;
    bool is_new = true;
    for (i = 0; i < def->chunks_len; i++)
    {
        if (def->chunks[i].x == c->x && def->chunks[i].y == c->y)
        {
            memcpy(def->chunks, c, sizeof(chunk));
            is_new = false;
            break;
        }
    }

    if (is_new)
    {
        def->chunks = realloc(def->chunks, sizeof(chunk) * (def->chunks_len + 1));
        memcpy(&(def->chunks[def->chunks_len++]), c, sizeof(chunk));
    }

}

#endif
