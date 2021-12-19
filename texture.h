#ifndef _texture_h
#define _texture_h

#include "main.h"

void LoadTexture(u_int *, TIM_IMAGE *, SPRITE*);

void RenderTile(TILE *tile, int x, int y, int w, int h, int r, int g, int b);

void RenderSprt(SPRT *sprite, int x, int y, int w, int h, int angle, RECT rect, int uoffs, int voffs);

#endif
