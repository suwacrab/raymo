#include "bios.h"

struct player;

#ifndef GAME_H
#define GAME_H

/*	--	structs	--	*/
typedef struct game
{
	bios *io;
	u32 plrmem[4 * 0x30];
	struct player *plrs;
} game;

/*	--	funcs	--	*/
extern void game_init(game *gram,bios *io);

#endif

