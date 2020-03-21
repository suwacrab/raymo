#include "kbase.h"
#include "kappamap.h"
#include "bios.h"
#include "game.h"

#ifndef PLAYER_H
#define PLAYER_H

/*	--	defs	--	*/

// acceleration (0.046875)
#define PLR_ACC (0x000000C0)
// decceleration (0.5)
#define PLR_DEC (0x00000800)

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
