#include "kbase.h"

#ifndef SINLUT_H
#define SINLUT_H

extern const u16 sinlut[0x400]; // 4.12 fixed point sin table.

INLINE FIXED lu_sin(u16 a)
{ return (FIXED)( (s16)sinlut[a>>6] ); }
INLINE FIXED lu_cos(u16 a)
{ return lu_sin(a+0x4000); }

#endif
