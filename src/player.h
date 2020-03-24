#include "kbase.h"
#include "nitori.h"
#include "bios.h"
#include "game.h"

#ifndef PLAYER_H
#define PLAYER_H

/*	--	defs	--	*/

// acceleration & friction (0.046875)
#define PLR_ACC (0x000000C0)
#define PLR_FRC (PLR_ACC)
// decceleration (0.5)
#define PLR_DEC (0x00000800)
// gravity (0.21875)
#define PLR_GRV (0x00000380)
// jump speed (7.5)
#define PLR_JMP (0x00000780)
// top speed (40)
#define PLR_TOP (0x00002800)

// convert pixel to map pos
#define PLRMAP(x,y) ( ((x)>>4) + ( ((y)>>4) * 0x80 ) )

/*	--	structs	--	*/
typedef struct player_joyp {
	// first nybble
	u8 up :1; u8 down :1;
	u8 left :1; u8 right :1;
	// 2nd nybble
	u8 a :1; u8 b :1;
	u8 menu :1; u8 cancel :1;
	u8 l :1; u8 r :1;
} player_joyp;

typedef struct player {
	// backend
	game *gram;
	player_joyp joyp;
	// gameplay
	VEC2 pos,vel; // pos & x/y speed, 20.12 fixed point
	FIXED gsp; // ground speed, 20.12
	u16 ang; // angle in radians
	s8 hp,maxhp; // health & max health
	s8 dir; // direction player's facing
	u32 walking;
	// animation
	u32 walkframe;
} player;

/*	--	LUTs	--	*/
extern const u8 height_lut[0x10][0x10];
extern const u16 ang_lut[0x10];
/*	--	main functions	--	*/
extern void player_init(player *plr,game *gram);
extern void player_updt(player *plr);
extern void player_draw(player *plr);

/*	--	update functions	--	*/
extern void player_updtjoyp(player *plr);
extern void player_updtmove(player *plr);

/*	--	draw functions	--	*/
extern void player_drawchar(player *plr);

#endif
