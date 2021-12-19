#include <stdio.h>


#include <sys/types.h>

#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>

#include "texture.h"
#include "main.h"

#define PI 3.14159

void LoadTexture(u_int *tim, TIM_IMAGE *tparam, SPRITE *sprt)
{

    GetTimInfo(tim, tparam);

    LoadImage(tparam->prect, (u_int*)tparam->paddr);
    DrawSync(0);

    // Upload CLUT to framebuffer if present
    if( tparam->mode & 0x8 )
    {
        LoadImage(tparam->crect, (u_int*)tparam->caddr);
        DrawSync(0);
    }

    sprt->prect   = *tparam->prect;
    sprt->crect   = *tparam->crect;
    sprt->mode    = tparam->mode;

    sprt->uoffs = (sprt->prect.x%64)<<(2-(sprt->mode&0x3));
    sprt->voffs = (sprt->prect.y&0xff);

}


void RenderSprt(SPRT *sprt, int x, int y, int w, int h, int angle, RECT rect, int uoffs, int voffs)
{

    setSprt(sprt);
    setXY0(sprt, x, y);
    setWH(sprt, w, h);

    
    setUV0(sprt, uoffs, voffs);
    setClut(sprt, rect.x, rect.y);
    setRGB0(sprt, 128, 128, 128);
}


void RenderTile(TILE *tile, int x, int y, int w, int h, int r, int g, int b)
{

    setTile(tile);
    setXY0(tile, x, y);
    setWH(tile, w, h);
    setRGB0(tile, r, g, b);
}


