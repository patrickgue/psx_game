#ifndef _main_h
#define _main_h

#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768


typedef struct _PADTYPE
{
    unsigned char	stat;
    unsigned char	len:4;
    unsigned char	type:4;
    unsigned short	btn;
    unsigned char	rs_x,rs_y;
    unsigned char	ls_x,ls_y;
} PADTYPE;


typedef struct _SPRITE
{
    RECT prect;
    RECT crect;
    int uoffs;
    int voffs;
    int mode;
} SPRITE;


typedef struct _rect
{
    int x, y, w, h;
} rect;

void SortRotSprite( rect r, int angle, int scale, SPRITE spr);

rect r(int,int,int,int);
#endif /* _main_h */
