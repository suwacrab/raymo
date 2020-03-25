#include "player.h"

/*	--	LUTs	--	*/
const u8 height_lut[0x10][0x10] =
{
	{ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 },
	{ 16,16,16,16, 16,16,16,16, 16,16,16,16, 16,16,16,16 },
	{ 16,16,16,16, 16,16,16,16, 16,16,16,16, 16,16,16,16 },
	{ 16,16,16,16, 16,16,16,16, 16,16,16,16, 16,16,16,16 },
	{ 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15 },
	{ 12,12,12,12, 11,11,10,9, 9,10,11,11, 12,12,12,12 }
};

const u16 ang_lut[0x10] =
{
	0,
	0,
	0,
	0,
	0xE000, // -45.0
	0
};

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
	VEC2 *pos = &plr->pos;
	VEC2 *vel = &plr->vel;
	FIXED *gsp = &plr->gsp;
	
	game *gram = plr->gram;
	hina *hmap = gram->hmap;
	nitori *kmap = &hmap->kmap;
	// get current tile
	VEC2 rpos = *pos; // rounded plr pos
	vec2_shr(&rpos,12); // $FFFFF.FFF > $FFFFF
	bool midair = false;
	u8 hitA,hitB; // bottom left,bottom right
	u8 hitC,hitD; // mid left, mid right
	u8 hitE,hitF; // top left, top right
	s32 Bwid = 2; // __B__ottom width.
	s32 Bhei = 8; // __B__ottom height.
	VEC2 posA = { rpos.x-Bwid,rpos.y+Bhei };
	VEC2 posB = { rpos.x+Bwid,rpos.y+Bhei };
	hitA = nitori_get(kmap,posA.x>>4,(rpos.y+Bhei)>>4);
	hitB = nitori_get(kmap,posB.x>>4,posB.y>>4);
	
	u16 tileang = 0;
	// update velocity based on movement
	s32 spd = PLR_ACC;
	{
		if( !(joyp->left && joyp->right) )
		{
			if(joyp->left) 
			{
				if(*gsp > 0)
				{ // if going in opposite dir, go back
					*gsp -= PLR_DEC;
					if(*gsp <= 0) *gsp = -0x800;
				} else if(*gsp > -PLR_TOP)
				{ // if going under top spd, speed up
					*gsp -= PLR_ACC;
					if(*gsp <= -PLR_TOP) *gsp = -PLR_TOP;
				}
			}
			if(joyp->right) 
			{
				if(*gsp < 0)
				{
					*gsp += PLR_DEC;
					if(*gsp > 0) *gsp = 0x800;
				} else if(*gsp < PLR_TOP)
				{
					*gsp += PLR_ACC;
					if(*gsp >= PLR_TOP) *gsp = PLR_TOP;
				}
			}
		}
	}
	if( (!joyp->right) && (!joyp->left) )
	{
		*gsp -= MIN(abs(*gsp),PLR_FRC) * SIGN(*gsp);
	}
	
	// A&B handling
	midair = !((hitA>0) || (hitB>0));
	if( !midair )
	{ // if A or B was hit (grounded)...
		s32 hitindA = 0; s32 hitindB = 0;
		s32 heightA = 0; s32 heightB = 0;
		bool hitgrnd = false;
		// moving
		if( hitA>0 )
		{
			hitindA = (posA.x) - ((posA.x>>4)<<4);
			heightA = height_lut[hitA][hitindA];
			tileang = ang_lut[hitA];
			// tile settin
			s32 tpos = (posA.y>>4)<<4; // round to tile pos
			s32 fpos = ((tpos+16)-heightA); // convert to foot pos
			if( posA.y >= fpos ) // if in height...
			{ hitgrnd = true; pos->y = int2fx(fpos-Bhei,12); }
			else // if not in height...
			{ }//vel->y += PLR_GRV; }
		}
		if( 0 )
		{
			hitindB = (posB.x) - ((posB.x>>4)<<4);
			heightB = height_lut[hitB][hitindB];
			tileang = ang_lut[hitB];
			// tile settin
			s32 tpos = (posB.y>>4)<<4;
			s32 fpos = ((tpos+16)-heightB);
			// if posB is in tile range...
			//if(posB.y > fpos)
			//{ pos->y = int2fx(fpos-Bhei-2,12); }
		}
		
		if( hitgrnd )
		{
			vel->x = fix_mul(lu_cos(tileang),*gsp,12);
			vel->y = fix_mul(lu_sin(tileang),*gsp,12);
			if( joyp->a ) vel->y -= PLR_JMP*8;
		}
		// drawing
		char hittxt[0x100];
		sprintf(hittxt,"indA: %d\nindB: %d\nhiA: %d\nhiB: %d\n",
			hitindA,hitindB,heightA,heightB
		);
		game_drawdebugtxt(gram,"ground",0,8);
		game_drawdebugtxt(gram,hittxt,0,16);
	} else { // if neither A nor B was hit (midair)...
		game_drawdebugtxt(gram,"midair",0,8);
		vel->y += PLR_GRV;
		if( joyp->right ) vel->x += PLR_ACC;
	}
	
	vec2_add(pos,vel);
	
	// update direction
	if(joyp->right) plr->dir = 1;
	if(joyp->left) plr->dir = 0;
	// update walk frame
	bool did_move = (vel->x != 0);
	if(did_move) plr->walkframe++;
	if(!did_move) plr->walkframe = 0;

	// pos hitbox drawing
	mokou_pset16(gram->io->fb,posA.x,posA.y,RGB15(31, 0, 0));
	mokou_pset16(gram->io->fb,posB.x,posB.y,RGB15( 0,31, 0));
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
	mokou_pset16(io->fb,dx,dy,RGB15(0,31,0));
}



