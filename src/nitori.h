#include "kbase.h"

#ifndef KMAP_H
#define KMAP_H

typedef struct nitori
{
	u8 *m;
	u16 w,h;
} nitori;

/*	--	functions	--	*/
extern nitori nitori_new(u16 w,u16 h);
extern void nitori_free(nitori *kawa);

/*	--	inline functions	--	*/
INLINE u8 *nitori_read(nitori *kawa,u32 x,u32 y)
{ return kawa->m + (x + (y*kawa->w)); }

INLINE u8 nitori_get(nitori *kawa,u32 x,u32 y)
{ return *nitori_read(kawa,x,y); }

INLINE void nitori_set(nitori *kawa,u32 x,u32 y,u8 d)
{ *nitori_read(kawa,x,y) = d; }

#endif
