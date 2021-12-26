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


typedef struct _chunk_index
{
    uint32_t    id, x, y;
} chunk_index;

typedef struct _chunk
{
    uint32_t    id;
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
    /* static */
    char        name[64];
    mov_type    movement_type;
    tile_type   tile_types[256];
    int         tile_types_used;
    
    /* dynamic */
    int         pos_x, pos_y, direction;
    int         indices_len;
    chunk_index *indices;
    int         chunks_len;
    chunk       *chunks;

 
} engine_def;

int engine_def_parse(engine_def *, FILE *);
int engine_def_parse_from_memory(engine_def *, unsigned char *, int);
int engine_def_init(engine_def *);

#ifdef EDIT_MODE
int engine_def_write(engine_def *, FILE *);

#endif

#endif
