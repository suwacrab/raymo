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
	plr->maxhp = 0x200;
	plr->hp = plr->maxhp;
	plr->jumped = false;
	plr->canjmp = true;

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
	u8 hitA,hitB; // bottom left,bottom right
	u8 hitC,hitD; // mid left, mid right
	u8 hitE,hitF; // top left, top right
	s32 Bwid = 2; // __B__ottom width.
	s32 Bhei = 8; // __B__ottom height.
	VEC2 posA = { rpos.x-Bwid,rpos.y+Bhei };
	VEC2 posB = { rpos.x+Bwid,rpos.y+Bhei };
	VEC2 posC = { rpos.x-Bwid,rpos.y };
	VEC2 posD = { rpos.x+Bwid,rpos.y };
	hitA = nitori_get(kmap,posA.x>>4,posA.y>>4);
	hitB = nitori_get(kmap,posB.x>>4,posB.y>>4);
	hitC = nitori_get(kmap,posC.x>>4,posC.y>>4);
	hitD = nitori_get(kmap,posD.x>>4,posD.y>>4);

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
	bool AorB = ((hitA>0) || (hitB>0));
	bool CorD = ((hitC>0) || (hitD>0));
	bool midair = false;
	if( AorB )
	{ // if A or B was hit (grounded)...
			vel->x = *gsp;
			vel->y = 0;
			// pos setting
			s32 tilepos = ((posA.y>>4)<<4) - Bhei;
			pos->y = int2fx(tilepos,12);
			// jumping
			plr->jumped = false;
			if( (!joyp->a) && (!plr->canjmp) ) plr->canjmp = true;
			if( joyp->a && plr->canjmp )
			{ vel->y -= PLR_JMP; plr->canjmp = false; plr->jumped = true; }
	} else { // if neither A nor B was hit (midair)...
		if( (!joyp->a) && plr->jumped )
		{ // if not holding A & player jumped...
			if(vel->y < (-2*int2fx(1,12)))
			{ // if y < -4...
				vel->y = ( -2*int2fx(1,12));
				plr->jumped  = false;
			}
		}
		vel->y += PLR_GRV;
		if( joyp->right ) vel->x += PLR_ACC;
		if( joyp->left ) vel->x -= PLR_ACC;
		*gsp = vel->x;
	}
	if( CorD )
	{
		if( hitC>0 ) // if C was hit...
		{ // push right
			s32 tilepos = ((posC.x>>4)<<4) + 17 + Bwid;
			pos->x = int2fx(tilepos,12);
			vel->x = 0;
			*gsp = 0;
		}
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
	mokou_sprattr attr = { {dx-16,dy-16},flip<<1,0xFFFF, 0,0 };
	SDL_Rect src = { frame*32,0,32,32 };
	mokou_spr16(raymo,io->fb,src,attr);
	mokou_pset16(io->fb,dx,dy,RGB15(0,31,0));
	// text drawin
	char plrtxt0[0x20];
	char plrtxt1[] = "  [    ]\n  [    ]";
	sprintf(plrtxt0,"HP[%04d]\nMP[%04d]",
		plr->hp,plr->mp
	);
	
	// --> palette shiftin & draw
	keine *font = &gram->img_bank[GAME_IMG_BOREFONT];
	game_drawdebugtxt(gram,plrtxt0,16,0);
	font->pal1[1] = RGB15(22,15,15);
	game_drawdebugtxt(gram,plrtxt1,16,0);
	memcpy(font->pal1,font->pal0,sizeof(RGB16)*0x10);
}



