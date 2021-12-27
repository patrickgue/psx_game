#include <stdbool.h>

#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>

#include "main.h"
#include "texture.h"
#include "engine.h"

#define PAL 1
#define OTLEN 4

#ifdef PAL
#define VH 256
#else
#define VH 240
#endif

DISPENV disp[2];
DRAWENV draw[2];
int db;

u_int ot[2][OTLEN];
char pribuff[2][32768];
char *nextpri;

extern u_long street_straight_img[];
SPRITE street_straight_s = {};

extern u_long street_t_img[];
SPRITE street_t_s = {};

extern u_long street_crosssection_img[];
SPRITE street_crosssection_s = {};

extern u_long trees_img[];
SPRITE trees_s = {};

extern u_char engine_data[];

u_char padbuff[2][34];


void load_sprites(void)
{
    TIM_IMAGE street_crosssection;
    TIM_IMAGE street_t;
    TIM_IMAGE street_straight;
    TIM_IMAGE trees;


    LoadTexture((u_int*)street_crosssection_img, &street_crosssection, &street_crosssection_s);
    LoadTexture((u_int*)street_t_img, &street_t, &street_t_s);
    LoadTexture((u_int*)street_straight_img, &street_straight, &street_straight_s);
    LoadTexture((u_int*)trees_img, &trees, &trees_s);
}

void init(void)
{
    ResetGraph(0);

#ifdef PAL

    SetDefDispEnv(&disp[0], 0, 0, 320, 256);
    SetDefDispEnv(&disp[1], 0, 256, 320, 256);

    disp[0].screen.y = 24;
    disp[1].screen.y = disp[0].screen.y;

    SetVideoMode(MODE_PAL);

    SetDefDrawEnv(&draw[0], 0, 256, 320, 256);
    SetDefDrawEnv(&draw[1], 0, 0, 320, 256);

#else

    SetDefDispEnv(&disp[0], 0, 0, 320, 240);
    SetDefDispEnv(&disp[1], 0, 240, 320, 240);
    
    SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
    SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

#endif
    
    setRGB0(&draw[0], 10,10,10); // 63, 0, 127
    setRGB0(&draw[1], 10,10,10);
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    
    db = 0;

    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);

    nextpri = pribuff[0];

    load_sprites();

    /* load any (first) sprite  */
    draw[0].tpage = getTPage(trees_s.mode&0x3, 0, trees_s.prect.x, trees_s.prect.y);
    draw[1].tpage = getTPage(trees_s.mode&0x3, 0, trees_s.prect.x, trees_s.prect.y);

    InitPAD((char*)padbuff[0], 34, (char*)padbuff[1], 34);
    StartPAD();
    ChangeClearPAD( 1 );

    FntLoad(960, 0);
    FntOpen(0, 8, 320, 224, 0, 100);
}

void display(void)
{
    DrawSync(0);
    VSync(0);

    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);

    SetDispMask(1);
    DrawOTag((u_long *) (ot[db] + OTLEN - 1));
    
    db = !db;
    nextpri = pribuff[db];
}

int main(int argc, char **argv)
{
    int pos_x, pos_y,
        button_up, counter, screen,
        enemy_x, enemy_y, enemy_direction,
        game_active, gi1, gi2, gi3, gi4, v,
        chunk_x, chunk_y,
        offset_x, offset_y;
    bool rotate_pressed = false;
    PADTYPE *pad;
    chunk chunks[9];

    TILE *tile;
    SPRT *sprt, *sprt2;
    DR_TPAGE *tpage;

    engine_def *def = malloc(sizeof(engine_def));
    engine_def_parse_from_memory(def, engine_data, 1504);

    pos_x = pos_y = 0;
    button_up = 0;
    counter = 0;

    enemy_x = 10;
    enemy_y = 100;
    enemy_direction = 1;

    game_active = 0;

    screen = 0;

    v = 0;
    
    init();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1)
    {
        pad = (PADTYPE*)padbuff[0];
        if (pad->stat == 0)
        {
            if ((pad->type == 0x4) || (pad->type == 0x5) || (pad->type == 0x7))
            {
                if (!(pad->btn&PAD_UP))
                    def->pos_y+=3;
                else if (!(pad->btn&PAD_DOWN))
                    def->pos_y-=3;

                if (!(pad->btn&PAD_LEFT) )
                    def->pos_x+=3;
                else if(!(pad->btn&PAD_RIGHT))
                    def->pos_x-=3;

                if (!(pad->btn&PAD_CROSS))
                    game_active = 1;
                else
                    button_up = -1;

                if (!(pad->btn&PAD_CROSS)) {
                    if (!rotate_pressed) {
                        v += 512;
                    }
                    rotate_pressed = true;
                } else {
                    rotate_pressed = false;
                }
            }
	   
	    
        }


        /* Rendering */

        ClearOTagR((u_long *) ot[db], OTLEN);

        FntPrint(-1, "X: %d Y: %d %s %d %d\n", def->pos_x, def->pos_y, engine_data, chunk_x, chunk_y);
        FntPrint(-1, "Counter: %d %d %lu o: %d %d\n", counter, csin(v), sizeof(enum e), offset_x, offset_y);

        chunk_x = (def->pos_x - def->pos_x % 1024);
        chunk_y = (def->pos_y - def->pos_y % 1024);


        chunks[0] = *engine_get_chunk(def, chunk_x - 1024, chunk_y - 1024);
        chunks[1] = *engine_get_chunk(def, chunk_x, chunk_y - 1024);
        chunks[2] = *engine_get_chunk(def, chunk_x + 1024, chunk_y - 1024);

        chunks[3] = *engine_get_chunk(def, chunk_x - 1024, chunk_y);
        chunks[4] = *engine_get_chunk(def, chunk_x, chunk_y);
        chunks[5] = *engine_get_chunk(def, chunk_x + 1024, chunk_y);

        chunks[6] = *engine_get_chunk(def, chunk_x - 1024, chunk_y + 1024);
        chunks[7] = *engine_get_chunk(def, chunk_x, chunk_y + 1024);
        chunks[8] = *engine_get_chunk(def, chunk_x + 1024, chunk_y + 1024);

        offset_x = chunk_x + (def->pos_x % 1024);
        offset_y = chunk_y + (def->pos_y % 1024);

        FntPrint(-1, "index: %d %d | %d %d",  gi1 - (offset_x / 32), gi2 - (offset_y / 32), gi3, gi4);

        // Brick Wall
        for (gi1 = -1; gi1 < 12; gi1++)
        {
            for (gi2 = -1; gi2 < 9; gi2++)
            {

                gi3 = gi1 - ((def->pos_x % 1024) / 32);
                gi4 = gi2 - ((def->pos_y % 1024) / 32);
                tile_type t = engine_chunk_get_tile(def, chunks[4], gi3, gi4);


                if (t.texture == 0)
                    SortRotSprite(r((def->pos_x % 32) + (gi1 * 32), (def->pos_y % 32) + (gi2 * 32), 32, 32), 2048, 2, trees_s);
                else if (t.texture == 1)
                    SortRotSprite(r((def->pos_x % 32) + (gi1 * 32), (def->pos_y % 32) + (gi2 * 32), 32, 32), 2048, 2, street_t_s);
                /* if (gi1 == 3 - (def->pos_x / 32) && gi2 == 3 - (pos_y / 32))
                    SortRotSprite(r((gi1 * 32) - (pos_x % 32), (gi2 * 32) - (pos_y % 32), 32, 32), 0, 2, street_crosssection_s);
                else if (gi1 == 4 - (pos_x / 32) && gi2 == 3 - (pos_y / 32))
                    SortRotSprite(r((gi1 * 32) - (pos_x % 32), (gi2 * 32) - (pos_y % 32), 32, 32), v, 0, street_t_s);
                else
                    */
            }
        }


        /* gi1 = 1, gi2 = 1;
        FntPrint(-1, "%d %d", offset_x, offset_y);

        for (gi3 = 0; gi3 < 32; gi3++)
        {
            for (gi4 = 0; gi4 < 1; gi4++)
            {
                tile_type type = engine_chunk_get_tile(def, chunks[(gi2 * 3) + gi1], gi3, gi4);
                if (type.texture == 0)
                    SortRotSprite(r(offset_x + (gi3 * 32), offset_y + (gi4 * 32), 32, 32), 2048, 0, trees_s); // type.rotation * 11.3777, 0, trees_s);
                else if (type.texture == 1)
                    SortRotSprite(r(offset_x + (gi3 * 32), offset_y + (gi4 * 32), 32, 32), type.rotation * 11.3777, 0, street_straight_s);
            }
        }*/


        counter ++;

        /* Game logic */
        enemy_x += enemy_direction;
        if (enemy_x > 280)
            enemy_direction = -1;

        if (enemy_x < 0)
            enemy_direction = 1;


        FntFlush(-1);
        display();
    }
#pragma clang diagnostic pop
    
    return 0;
}


void SortRotSprite( rect r, int angle, int scale, SPRITE spr)
{
    int x = r.x, y = r.y, pw = r.w, ph = r.h;
    POLY_FT4 *quad;
    SVECTOR	s[4];
    SVECTOR	v[4];

    int i,cx,cy;

    cx = pw>>1;
    cy = ph>>1;

    s[0].vx = -(((pw*scale)>>12)-cx);
    s[0].vy = -(((ph*scale)>>12)-cy);

    s[1].vx = (((pw*scale)+2048)>>12)-cx;
    s[1].vy = s[0].vy;

    s[2].vx = -(((pw*scale)>>12)-cx);
    s[2].vy = (((ph*scale)+2048)>>12)-cy;

    s[3].vx = (((pw*scale)+2048)>>12)-cx;
    s[3].vy = s[2].vy;
    
    cx = ccos( angle );
    cy = csin( angle );
    
    for( i=0; i<4; i++ )
    {
        v[i].vx = (((s[i].vx*cx) - (s[i].vy*cy))>>12)+x;
        v[i].vy = (((s[i].vy*cx) + (s[i].vx*cy))>>12)+y;
    }

    quad = (POLY_FT4*)nextpri;
    setPolyFT4( quad );

    // set CLUT and tpage to the primitive
    setTPage( quad, spr.mode&0x3, 0, spr.prect.x, spr.prect.y );
    setClut( quad, spr.crect.x, spr.crect.y );

    setRGB0( quad, 128, 128, 128 );
    setXY4( quad,
        v[0].vx, v[0].vy,
        v[1].vx, v[1].vy,
        v[2].vx, v[2].vy,
        v[3].vx, v[3].vy );
    setUVWH( quad, spr.uoffs, spr.voffs, pw, ph );

    addPrim( ot[db], quad );
    nextpri += sizeof(POLY_FT4);
}

rect r(int x, int y, int w, int h)
{
    return (rect) {x,y,w,h};
}