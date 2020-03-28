#include "mokou.h"

/*	--	pset functions	--	*/
RGB8 *mokou_pread4(keine *yago,u32 x,u32 y)
{ return &((RGB8*)yago->m)[ (x>>1) + (y*(yago->w>>1)) ]; }
RGB8 mokou_pget4(keine *yago,s32 x,s32 y)
{ 
	if( !keine_clip(yago,x,y) ) return 0;
	RGB8 pix = *mokou_pread4(yago,x,y) >> (4*(x&1));
	return pix & 0xF; 
}
void mokou_pset4(keine *yago,s32 x,s32 y,RGB8 c)
{ 
	if( keine_clip(yago,x,y) && keine_fillp(yago,x,y))
	{
		*mokou_pread4(yago,x,y) = c;
	}
}

RGB16 *mokou_pread16(keine *yago,u32 x,u32 y)
{ return ((RGB16*)yago->m) + (x + (y*yago->w)); }
RGB16 mokou_pget16(keine *yago,s32 x,s32 y)
{ return keine_clip(yago,x,y) ? (*mokou_pread16(yago,x,y)) : 0; }
void mokou_pset16(keine *yago,s32 x,s32 y,RGB16 c)
{
	if(keine_clip(yago,x,y) && keine_fillp(yago,x,y)) {
		*mokou_pread16(yago,x,y) = c;
	}
}
void mokou_rect16(keine *yago,s32 px,s32 py,s32 dx,s32 dy,RGB16 c)
{
	if(dx > (s32)yago->w) dx = yago->w+1;
	if(dy > (s32)yago->h) dy = yago->h+1;
	for(s32 y=py; y<dy; y++)
		for(s32 x=px; x<dx; x++)
				*mokou_pread16(yago,x,y) = c;
}


/*	--	fill functions	--	*/
void mokou_spr16(
	keine *src,keine *dst,SDL_Rect srcrect,
	mokou_sprattr attr
) {
	// flipping
	u8 xf = (attr.flip>>1)&1;
	u8 yf = (attr.flip&1);
	s32 srcW = srcrect.w; s32 srcH = srcrect.h;
	s32 srcX = srcrect.x; s32 srcY = srcrect.y;
	s32 dx = attr.pos.x;
	s32 dy = attr.pos.y;
	
	RGB16 *srcpal = src->pal1;
	u16 old_fillp = dst->fillp;
	dst->fillp = attr.fillp;

	for(s32 y=0; y<srcH; y++)
	{
		s32 ly = dy+y;
		for(s32 x=0; x<srcW; x++)
		{
			s32 lx = dx+x;
			if(in_range(lx,0,dst->w-1) && in_range(ly,0,dst->h-1))
			{
				s32 gx = xf ? ((srcW-1)-x) : x;
				s32 gy = yf ? ((srcH-1)-y) : y;
				// pixel setting
				RGB16 pix;
				switch(src->fmt)
				{
					case KEINE_PIXELFMT_PAL4: pix = srcpal[mokou_pget4(src,gx+srcX,gy+srcY)]; break;
					case KEINE_PIXELFMT_RGB15: pix = mokou_pget16(src,gx+srcX,gy+srcY); break;
				}
				if(pix!=0) mokou_pset16(dst,lx,ly,pix);
			}
		}
	}

	dst->fillp = old_fillp;
}

