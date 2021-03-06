#include "kbase.h"

#ifndef SUWAKO_H
#define SUWAKO_H

#define POOL_NULL (0xFFFF)

/*	--	suwa objs	--	*/
typedef uint32_t suwa_data[4];
typedef struct suwa_stat {
	uint8_t type,dead;
	uint16_t next,id; // next index, current index*
	// 0xFFFF is treated as NULL.
} suwa_stat;

typedef struct suwako
{
	VEC2 pos; // $04 (fixed point 20.12)
	suwa_stat stat; // $04
	suwa_data data; // $0C
	// size = $14
} suwako;

#endif
