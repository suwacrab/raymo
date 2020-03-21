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
	for(u32 i=0; i<4; i++)
	{ player_init(&gram->plrs[i],gram); }
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
	player *plrs = gram->plrs;
	kanako *suwa_objs = &gram->suwa_objs;
	// player shit
	player_updt(&plrs[0]);
	// obj shit
	if(	(io->time&15) == 0 ) 
	{  
		for(u32 a=0; a<(1<<16); a += 0x4000)
		{
			u32 ra = a + (io->time<<8);
			suwako *nobj = kanako_add(suwa_objs);
			((FIXED*)nobj->data)[0] = (lu_cos(ra) * 2);
			((FIXED*)nobj->data)[1] = (lu_sin(ra) * 2);
			nobj->data[2] = 0;
			nobj->stat.type = 1;
			nobj->pos.y = int2fx(io->h/2,12);
			nobj->pos.x = int2fx(io->w/2,12);
		}
	}
	for(u32 i=0; i<suwa_objs->len; i++)
	{
		suwako *obj = &suwa_objs->objs[i];
		if(!obj->stat.dead)
		{
			FIXED *dat = (FIXED*)obj->data;
			obj->pos.x += dat[0];
			obj->pos.y += dat[1];
			dat[2]++;
			s32 x = fx2int(obj->pos.x,12);
			s32 y = fx2int(obj->pos.y,12);
			if( !in_range(y,-32,io->h+32) ) obj->stat.dead = true;
			if( !in_range(x,-32,io->w+32) ) obj->stat.dead = true;
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
	// drawin maps
	RGB16 screenblock[] =
	{
		RGB15( 0, 0, 0), RGB15(31, 0, 0),
		RGB15( 0,31, 0), RGB15( 0, 0,31)
	};
	gram->testmap[3][3] = 1;
	mokou_sprattr tileattr = { {0,0},0b00,0xFFFF, 0,0 };
	for(u32 y=0; y<0x80; y++)
	{
		tileattr.pos.y = y*16;
		for(u32 x=0; x<0x80; x++)
		{
			tileattr.pos.x = x*16;
			u8 tile = gram->testmap[y][x];
			if( (tile>0) && (tile<4) )
			{
				keine *tileimg = &gram->img_bank[GAME_IMG_TESTTILE];
				SDL_Rect srcrect = { (tile&15)*16,(tile>>4)*16,16,16 };
				mokou_spr16(tileimg,io->fb,srcrect,tileattr);
			}	
		}
	}
	// drawin players
	player_draw(&plrs[0]);
	// drawin objs
	for(u32 i=0; i<suwa_objs->len; i++)
	{
		suwako *obj = &suwa_objs->objs[i];
		if(!obj->stat.dead)
		{
			s32 x = fx2int(obj->pos.x,12);
			s32 y = fx2int(obj->pos.y,12);
			keine *raymo = &gram->img_bank[GAME_IMG_RAYMO];
			mokou_sprattr attr = { {x-12,y-12},0b00,lu_fade(obj->data[2]<<9), 0,0 };
			SDL_Rect src = { ((x>>4)&3)*24,0,24,24 };
			mokou_spr16(raymo,io->fb,src,attr);
		}
	}
}

void game_drawtestmap(game *gram)
{
	// vars
	mokou_sprattr attr = { {0,0},0,0xFFFF, 0,0 };
	u8 *testmap = gram->testmap;
	bios *io = gram->io;
	keine *tileimg = &gram->img_bank[GAME_IMG_TESTTILE];
	// drawin
	for(u32 y=0; y<0x80; y++)
	{
		attr.pos.y = y*16;
		for(u32 x=0; x<0x80; x++)
		{
			attr.pos.x = y*16;
			u8 tile = testmap[x + (y*0x80)];
			if(tile>0)
			{
				SDL_Rect srcrect = { (tile&15)*16,(tile>>4)*16,16,16 };
				mokou_spr16(tileimg,io->fb,srcrect,attr);
			}
		}
	}
}

