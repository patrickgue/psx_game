#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>

#include "main.h"
#include "texture.h"

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

    InitPAD(padbuff[0], 34, padbuff[1], 34);
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
    DrawOTag((u_int *) ot[db]+OTLEN-1);
    
    db = !db;
    nextpri = pribuff[db];
}

int main(int argc, char **argv)
{
    int pos_x, pos_y, button_up, counter, screen, enemy_x, enemy_y, enemy_direction, game_active, gi1, gi2, v;
    PADTYPE *pad;

    TILE *tile;
    SPRT *sprt, *sprt2;
    DR_TPAGE *tpage;

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

    while (1)
    {
        pad = (PADTYPE*)padbuff[0];
        if (pad->stat == 0)
        {
            if ((pad->type == 0x4) || (pad->type == 0x5) || (pad->type == 0x7))
            {
		if (!(pad->btn&PAD_UP))
		    pos_y--;
		else if (!(pad->btn&PAD_DOWN))
		    pos_y++;
		    
		if (!(pad->btn&PAD_LEFT) )
		    pos_x--;
		else if(!(pad->btn&PAD_RIGHT))
		    pos_x++;

		if (!(pad->btn&PAD_CROSS))
		    game_active = 1;
		else
		    button_up = -1;

		if (!(pad->btn&PAD_CROSS))
		    v += 512;
		
	    }
	   
	    
        }


	/* Rendering */

	ClearOTagR(ot[db], OTLEN);

	FntPrint(-1, "X: %d Y: %d\n", pos_x, pos_y);
	FntPrint(-1, "Counter: %d", counter);


	   

	// SortRotSprite(enemy_x, enemy_y, 64,64, 2048, 1, johnson_s);
	// SortRotSprite(60,60, 32, 32, 2048 + (counter * 8), 10, shoe_s);
        
	
	
	
	// Brick Wall
	for (gi1 = -1; gi1 < 12; gi1++)
	{
	    for (gi2 = -1; gi2 < 9; gi2++)
	    {
		if (gi1 == 3 - (pos_x / 32) && gi2 == 3 - (pos_y / 32))
		    SortRotSprite((gi1 * 32) - (pos_x % 32), (gi2 * 32) - (pos_y % 32), 32, 32, 0, 2, street_crosssection_s);
		else if (gi1 == 4 - (pos_x / 32) && gi2 == 3 - (pos_y / 32))
		    SortRotSprite((gi1 * 32) - (pos_x % 32), (gi2 * 32) - (pos_y % 32), 32, 32, v, 2, street_t_s);
		else
		    SortRotSprite((gi1 * 32) - (pos_x % 32), (gi2 * 32) - (pos_y % 32), 32, 32, 0, 2, trees_s);		    
	    }
	}

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
    
    return 0;
}


void SortRotSprite( int x, int y, int pw, int ph, int angle, int scale, SPRITE spr)
{
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
        v[i].vx = (((s[i].vx*cx)
            -(s[i].vy*cy))>>12)+x;
        v[i].vy = (((s[i].vy*cx)
            +(s[i].vx*cy))>>12)+y;
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
