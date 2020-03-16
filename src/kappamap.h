#include "kbase.h"

#ifndef KMAP_H
#define KMAP_H

typedef struct kappamap
{
	u8 *m;
	u16 w,h;
} kappamap;

/*	--	functions	--	*/
extern kappamap kmap_new(u16 w,u16 h);
extern void kmap_free(kappamap *kmap);

/*	--	inline functions	--	*/
INLINE u8 *kmap_read(kappamap kmap,u32 x,u32 y)
{ return kmap.m + (x + (y*kmap.w)); }
INLINE u8 kmap_get(kappamap kmap,u32 x,u32 y)
{ return *kmap_read(kmap,x,y); }
INLINE void kmap_set(kappamap kmap,u32 x,u32 y,u8 d)
{ *kmap_read(kmap,x,y) = d; }

#endif
