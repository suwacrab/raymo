#include "kbase.h"
#include "keine.h"
#include "mokou.h"
#include "nitori.h"
#include "game.h"

#ifndef HINA_H
#define HINA_H

/* -- structs -- */
typedef struct hina
{
	game *gram; // game RAM
	keine *img; // image used for map
	u32 size; // size ( in pixels ) per tile
	nitori kmap; // the map itself
} hina;



#endif
