#include "kbase.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef KEINE_H
#define KEINE_H

// quick inlines & defs
INLINE u16 BGR565(u32 r,u32 g,u32 b) { return (r<<11) | (g<<5) | b; }
INLINE u16 RGB565(u32 r,u32 g,u32 b) { return (b<<11) | (g<<5) | r; }
INLINE u16 RGB15(u32 r,u32 g,u32 b) { return (b<<10) | (g<<5) | r; }

typedef struct RGB24 { u8 r,g,b; } RGB24;
typedef u16 RGB16;
typedef u8 RGB8;

/*	--	enums	--	*/
typedef enum keine_pixelfmt
{
	KEINE_PIXELFMT_PAL4, // 16-color palette, first pixel == first nybble
	KEINE_PIXELFMT_PAL8, // 256-color palette, each pixel == 1 byte
	KEINE_PIXELFMT_RGB565, // 16-bit high clr (BBBBBGGGGGGRRRRR in bits)
	KEINE_PIXELFMT_RGB15, // 15-bit high clr (BBBBBGGGGGRRRRR in bits)
	KEINE_PIXELFMT_RGB888 // 24-bit high clr ( [R,G,B] in bytes )
} keine_pixelfmt;

/*	--	structs	--	*/
typedef struct keine
{
	u32 w,h; // width,height
	u16 fillp; // fill pattern
	keine_pixelfmt fmt; // pixel format
	RGB16 *pal0; // color palette ( should b readonly )
	RGB16 *pal1; // alternate palette ( for drawing )
	void *m; // pixel matrix
} keine;

/*	--	main functions	--	*/
extern keine *keine_init(keine *yago,u32 w,u32 h,keine_pixelfmt fmt);
extern void keine_free(keine *yago);
extern void keine_clear(keine *yago);
extern keine *keine_loadimg(keine *yago,const char *fname,keine_pixelfmt fmt);

/*	--	init functions	--	*/
typedef size_t (*keine_initfunc)(keine*);
extern keine_initfunc keine_initfmts[];
extern size_t keine_initPAL4(keine *yago);
extern size_t keine_initPAL8(keine *yago);
extern size_t keine_initRGB16(keine *yago);

/*	--	inlines	--	*/
INLINE bool keine_clip(keine *yago,s32 x,s32 y)
{ return (in_range(x,0,yago->w) && in_range(y,0,yago->h)); }

INLINE u32 keine_fillp(keine *yago,u32 x,u32 y)
{
	u32 ind = (x&3) + ((y&3)*4);
	u32 tbit = (yago->fillp>>ind)&1;
	return tbit;
}
INLINE size_t keine_imgsize(keine *yago)
{ return keine_initfmts[yago->fmt](yago); }

#endif
