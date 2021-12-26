#ifndef _ENGINE_H
#define _ENGINE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "engine_meta.h"

#ifdef PSX
#include "file.h"
#endif

#define CHUNK_SIZE 32 * 32

typedef enum _en_mov_type
{
    MOV_BLOCK,
    MOV_TANK,
    MOV_GLIDER
} mov_type;

typedef struct _chunk
{
    uint32_t    x, y;
    uint8_t     set[CHUNK_SIZE];
} chunk;

typedef struct _tile_type
{
    uint8_t     flags;         /* x x x x x x x x */
                               /*               | */
                               /*           solid */
    uint16_t    rotation;
    uint8_t     texture;
    
} tile_type;

typedef struct _engine_def
{
    /* predefined static */
    char        name[32];
    char        version[16];
    char        engine_version[16];
    mov_type    movement_type;
    tile_type   tile_types[64];
    uint8_t     tile_types_used;

    /* predefined dynamic */
    int         pos_x, pos_y, direction;

    /* dynamic */
    int         chunks_len;
    chunk       *chunks;
} engine_def;

int engine_def_parse(engine_def *, FILE *);
int engine_def_parse_from_memory(engine_def *, unsigned char *, int);
int engine_def_init(engine_def *);
chunk *engine_get_chunk(engine_def *, int x, int y);
tile_type engine_chunk_get_tile(engine_def *, chunk, int, int);

#ifdef EDIT_MODE

#ifdef PSX /* mock realloc */
void *realloc(void*, int);
#endif

int engine_def_write(engine_def *, FILE *);
void engine_set_chunk(engine_def*,chunk*);

#endif

#endif
