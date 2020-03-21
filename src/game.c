#include "game.h"
#include "player.h"

/*	--	LUTs	--	*/
const char *game_img_lut[0x100] = 
{
	/*	00	*/ "gfx/testtex.png",
	/*	01	*/ "gfx/testtile.png",
	/*	02	*/ "gfx/raymo.png"
};

/*	--	main funcs	--	*/
void game_init(game *gram,bios *io)
{
	// bios
	gram->io = io;
	// asset init
	gram->img_bank = malloc(sizeof(keine) * 0x10);
	keine *img_bank = gram->img_bank;
	// player init
	printf("<player size> $%04lX\n",sizeof(player));
	gram->plrs = (player*)gram->plrmem;
	// obj init
	kanako *suwa_objs = kanako_init(&gram->suwa_objs,gram->objmem,0x400);

	// asset loading
	game_loadimg(gram,0,game_img_lut[0],KEINE_PIXELFMT_RGB15);
	game_loadimg(gram,1,game_img_lut[1],KEINE_PIXELFMT_RGB15);
	game_loadimg(gram,2,game_img_lut[2],KEINE_PIXELFMT_RGB15);
}

keine *game_loadimg(game *gram,u32 id,const char *fname,keine_pixelfmt fmt)
{
	keine *curimg = &gram->img_bank[id];
	keine_loadimg(curimg,fname,fmt);
	printf("<image loaded> [$%04X] '%s'\n",id,fname);
	return curimg;
}

void game_run(game *gram)
{
	bios *io = gram->io;
	while( !io->quit )
	{
		// update
		bios_updt(io);
		game_updt(gram);
		// draw
		bios_clearscreen(io);
		game_draw(gram);
		bios_draw(io);
	}
}

/*	--	update funcs	--	*/
void game_updt(game *gram)
{
	// vars
	bios *io = gram->io;
	kanako *suwa_objs = &gram->suwa_objs;
	// obj shit
	if(	(io->time&31) == 0 ) 
	{ 
		suwako *nobj = kanako_add(suwa_objs); 
		((s8*)nobj->data)[0] = -1;
		nobj->stat.type = 1;
		nobj->pos.y = int2fx(io->h,12);
		nobj->pos.x = int2fx(io->w/2,12);
	}
	for(u32 i=0; i<suwa_objs->len; i++)
	{
		suwako *obj = &suwa_objs->objs[i];
		if(!obj->stat.dead)
		{
			s8* dat = (s8*)obj->data;
			obj->pos.y += int2fx((FIXED)dat[0],12);
			if( fx2int(obj->pos.y,12) < -8 ) obj->stat.dead = true;
		}
	}
	u32 last_alive = suwa_objs->alive;
	kanako_updt(suwa_objs);
	if(last_alive != suwa_objs->alive)
	printf("<suwa_obj alive> $%04X\n",suwa_objs->alive);
}

/*	--	draw funcs	--	*/
void game_draw(game *gram)
{
	// vars
	bios *io = gram->io;
	player *plrs = gram->plrs;
	kanako *suwa_objs = &gram->suwa_objs;
	uint32_t time = io->time;
	// drawin
	for(u32 i=0; i<suwa_objs->len; i++)
	{
		suwako *obj = &suwa_objs->objs[i];
		if(!obj->stat.dead)
		{
			s32 x = fx2int(obj->pos.x,12);
			s32 y = fx2int(obj->pos.y,12);
			keine *raymo = &gram->img_bank[GAME_IMG_RAYMO];
			mokou_sprattr attr = { mokou_sprpos(x-12,y-12),0b00,0xFFFF, 0,0 };
			SDL_Rect src = { ((y>>4)&3)*24,0,24,24 };
			mokou_spr16(raymo,io->fb,src,attr);
		}
	}
}

