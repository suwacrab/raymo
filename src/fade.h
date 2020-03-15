#include "kbase.h"

#ifndef FADE_H
#define FADE_H

/*	--	LUTs	--	*/
extern u16 fadelut[0x10]; // LUT for fillp fading

/*	--	inlines	--	*/
INLINE u16 lu_fade(u16 i)
{ return fadelut[i>>12]; }

#endif
