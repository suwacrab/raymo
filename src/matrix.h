#include "kbase.h"

#ifndef MATRIX_H
#define MATRIX_H

/*	--	structs	--	*/
typedef struct MAT2 { FIXED m[2][2]; } MAT2;
typedef struct MAT3 { FIXED m[3][3]; } MAT3;

typedef struct MAT2f { float m[2][2]; } MAT2f;
typedef struct MAT3f { float m[3][3]; } MAT3f;

/*	--	inline funcs	--	*/

// multiplies a * b, stores result in C. dec is fixed point decimal
INLINE MAT2 mat2_mul(MAT2 *a,MAT2 *b,u32 dec)
{
	MAT2 c = { 0,0,0,0 };
	for(u32 y = 0; y<2; y++)
		for(u32 x = 0; x<2; x++)
			for(u32 z = 0; z<2; z++)
			{
				c.m[y][x] += fix_mul(a->m[y][z],b->m[z][x],dec);
			}

	return c;
}

// same as as mat2_mul(), except b & c are vec2 structs
INLINE VEC2 mat2_mulv(MAT2 *a,VEC2 *b,u32 dec)
{
	MAT2 tmpB = {b->x,0,b->y,0}; // convert vec2 to mat2
	MAT2 tmpC = mat2_mul(a,&tmpB,dec);
	VEC2 c = { tmpC.m[0][0],tmpC.m[1][0] };
	return c;
}

//
INLINE MAT2 mat2_rot(u16 ang)
{
	MAT2 nmat = {
		(FIXED)lu_cos(ang)>>4,-((FIXED)lu_sin(ang)),
		(FIXED)lu_sin(ang)>>4,(FIXED)lu_cos(ang)>>4
	};
	return nmat;
}


#endif
