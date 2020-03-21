#include "bios.h"
#include "suwako.h"
#include "kanako.h"

struct player;

#ifndef GAME_H
#define GAME_H

/*	--	structs	--	*/
typedef enum game_img {
	GAME_IMG_TESTTEX = 0,
	GAME_IMG_TESTTILE = 1,
	GAME_IMG_RAYMO = 2,
} game_img;

typedef struct game
{
	// main
	bios *io;
	// assets
	keine *img_bank;
	// objects
	u32 plrmem[4 * 0x30];
	struct player *plrs;
	suwako objmem[0x400];
	kanako suwa_objs;
	// maps
	u8 testmap[0x80 * 0x80]; // in [Y][X] order!!!
} game;

/*	--	LUTs	--	*/
extern const char *game_img_lut[0x100];

/*	--	main funcs	--	*/
extern void game_init(game *gram,bios *io);
extern void game_run(game *gram);
extern keine *game_loadimg(game *gram,u32 id,const char *fname,keine_pixelfmt fmt);

/*	--	update funcs	--	*/
extern void game_updt(game *gram);

/*	--	draw funcs	--	*/
extern void game_draw(game *gram);
extern void game_drawtestmap(game *gram);

#endif

