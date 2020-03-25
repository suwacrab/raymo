#include "bios.h"
#include "suwako.h"
#include "kanako.h"
#include "nitori.h"

struct hina;
struct player;

#ifndef GAME_H
#define GAME_H
/* -- defs -- */
#define HMAP_W (0x80)
#define HMAP_H (0x80)

/*	--	structs	--	*/
typedef enum game_img {
	GAME_IMG_TESTTEX = 0,
	GAME_IMG_TESTTILE = 1,
	GAME_IMG_RAYMO = 2,
	GAME_IMG_BOREFONT = 3
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
	suwako obj_mem[0x200];
	kanako suwa_objs;
	// maps
	u32 hmap_mem[0x20];
	struct hina *hmap;
	u8 map_mem[4][ HMAP_W * HMAP_H ];
} game;

/*	--	LUTs	--	*/
extern const char *game_img_lut[];

/*	--	main funcs	--	*/
extern void game_init(game *gram,bios *io);
extern void game_run(game *gram);
extern keine *game_loadimg(game *gram,u32 id,const char *fname,keine_pixelfmt fmt);

/*	--	update funcs	--	*/
extern void game_updt(game *gram);

/*	--	draw funcs	--	*/
extern void game_draw(game *gram);
extern void game_drawdebugtxt(game *gram,const char *txt,s32 x,s32 y);
extern void game_drawmap(game *gram);

#endif

