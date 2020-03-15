#include "mokou.h"

/*	--	pset functions	--	*/
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
	s16 dx = (s16)(attr.pos>>16);
	s16 dy = (s16)(attr.pos&0xFFFF);
	dst->fillp = attr.fillp;

	for(s32 y=0; y<srcH; y++)
	{
		s32 ly = dy+y;
		for(s32 x=0; x<srcW; x++)
		{
			s32 lx = dx+x;
			s32 gx = xf ? ((srcW-1)-x) : x;
			s32 gy = yf ? ((srcH-1)-y) : y;
			RGB16 pix = mokou_pget16(src,gx+srcX,gy+srcY);
			if(pix!=0) mokou_pset16(dst,lx,ly,pix);
		}
	}
}

