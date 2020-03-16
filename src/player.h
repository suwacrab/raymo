#include "kbase.h"
#include "kappamap.h"
#include "bios.h"


#ifndef PLAYER_H
#define PLAYER_H

/*	--	structs	--	*/
typedef struct player_joyp {
	u8 up,down,left,right;
	u8 a,b,menu,cancel;
	u8 l,r;
} player_joyp;

typedef struct player {
	// backend
	bios *io;
	player_joyp joyp;
	// gameplay
	VEC2 pos,vel; // 20.12 fixed point
	s8 hp,maxhp; // health & max health
	s8 dir; // direction player's facing
	u32 walking;
	// animation
	u32 walkframe;
} player;

/*	--	main functions	--	*/
extern void player_init(player *plr,bios *io);
extern void player_updt(player *plr);
extern void player_draw(player *plr);

/*	--	update functions	--	*/
extern void player_updtjoyp(player *plr);
extern void player_updtmove(player *plr);

/*	--	draw functions	--	*/
extern void player_drawchar(player *plr);

#endif
