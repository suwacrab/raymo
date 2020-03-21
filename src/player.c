#include "player.h"

/*	--	main functions	--	*/
void player_init(player *plr,game *gram)
{
	// clearing
	plr->gram = gram;
	plr->maxhp = 0x10;
	plr->hp = plr->maxhp;

	vec2_set(&plr->pos,0,0);
	vec2_set(&plr->vel,0,0);

	memset(&plr->joyp,0,sizeof(player_joyp));
}
void player_updt(player *plr)
{
	player_updtjoyp(plr);
	player_updtmove(plr);
}
void player_draw(player *plr)
{
	player_drawchar(plr);
}

/*	--	update functions	--	*/
void player_updtjoyp(player *plr)
{
	// vars
	game *gram = plr->gram;
	bios *io = gram->io;
	player_joyp *joyp = &plr->joyp;
	uint8_t *keystate = io->keystate;
	// update d-pad
	joyp->up = keystate[SDLK_UP];
	joyp->down = keystate[SDLK_DOWN];
	joyp->left = keystate[SDLK_LEFT];
	joyp->right = keystate[SDLK_RIGHT];
	// update face buttons
	joyp->a = keystate[SDLK_z];
	joyp->b = keystate[SDLK_x];
	joyp->menu = keystate[SDLK_c];
	joyp->cancel = keystate[SDLK_v];
	// update side buttons
	joyp->l = keystate[SDLK_a];
	joyp->r = keystate[SDLK_s];

}
void player_updtmove(player *plr)
{
	// vars
	player_joyp *joyp = &plr->joyp;
	u8 *testmap = plr->gram->testmap;
	VEC2 *pos = &plr->pos;
	VEC2 *vel = &plr->vel;
	FIXED *gsp = &plr->gsp;
	// get current tile
	VEC2 tilepos = *pos;
	vec2_shr(&tilepos,12); // get rid of fixed point (0xFFFFFFFF>0xFFFFF)
	vec2_shr(&tilepos,4); // divide by 16 for tile pos (0xFFFFF>0xFFFF)
	u8 curtile = testmap[tilepos.x + (tilepos.y*0x80)];
	
	// update velocity based on movement
	s32 spd = fix_mul(1<<12,0x1000,12);
	if(joyp->up) vel->y -= spd;
	if(joyp->down) vel->y += spd;
	if(joyp->left) *gsp -= spd;
	if(joyp->right) *gsp += spd;

	vec2_add(pos,vel);
	
	// update direction
	if(joyp->right) plr->dir = 1;
	if(joyp->left) plr->dir = 0;
	// update walk frame
	bool did_move = (vel->x != 0);
	if(did_move) plr->walkframe++;
	if(!did_move) plr->walkframe = 0;
	
	char *n = malloc(sizeof(char)*16);
}

/*	--	draw functions	--	*/
void player_drawchar(player *plr)
{
	// vars
	game *gram = plr->gram;
	bios *io = gram->io;
	VEC2 *pos = &plr->pos;
	VEC2 *vel = &plr->vel;
	s32 dx = pos->x>>12;
	s32 dy = pos->y>>12;
	keine *raymo = &gram->img_bank[GAME_IMG_RAYMO];
	u32 flip = (~plr->dir)&1;
	// drawing
	u32 frame = (plr->walkframe>>3)%4;
	mokou_sprattr attr = { {dx-12,dy-12},flip<<1,0xFFFF, 0,0 };
	SDL_Rect src = { frame*24,0,24,24 };
	mokou_spr16(raymo,io->fb,src,attr);
}



