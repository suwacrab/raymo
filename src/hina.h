#include "kbase.h"
#include "keine.h"
#include "mokou.h"
#include "nitori.h"
#include "game.h"

#ifndef HINA_H
#define HINA_H

/* -- enums -- */
typedef enum HINA_SIZE
{
	HINA_SIZE_4x4,
	HINA_SIZE_8x8,
	HINA_SIZE_16x16,
	HINA_SIZE_32x32
} HINA_SIZE;

/* -- structs -- */
typedef struct hina
{
	game *gram; // game RAM
	keine *img; // image used for map
	HINA_SIZE size; // size ( in pixels ) per tile
	nitori kmap; // the map itself
} hina;

/* -- main funcs -- */
extern hina *hina_init(hina *kagi,game *gram,HINA_SIZE size);
extern void hina_drawtile(hina *kagi,mokou_sprattr *attr,u8 tile,s32 dx,s32 dy);
extern void hina_drawmap(hina *kagi);

/* -- inline funcs -- */
INLINE s32 hina_wide(HINA_SIZE size)
{ return 4<<size; }
INLINE s32 hina_pos(HINA_SIZE size,s32 n)
{ return n * hina_wide(size); }

#endif
