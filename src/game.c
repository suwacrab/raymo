#include "game.h"
#include "player.h"
#include "hina.h"

/*	--	LUTs	--	*/
const char *game_img_lut[] = 
{
	/* 00 */ "gfx/testtex.png",
	/* 01 */ "gfx/testtile.png",
	/* 02 */ "gfx/raymo.png",
	/* 03 */ "gfx/borefont.png",
	NULL
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
	kanako *suwa_objs = kanako_init(&gram->suwa_objs,gram->obj_mem,0x200);
	// hina init
	gram->hmap = (hina*)gram->hmap_mem;
	hina *hmap = hina_init(gram->hmap,gram,HINA_SIZE_16x16);
	hmap->kmap.m = gram->map_mem[0];
	hmap->kmap.w = HMAP_W;
	hmap->kmap.h = HMAP_H;
	hmap->img = &gram->img_bank[GAME_IMG_TESTTILE];
	for(u32 x=0; x<0x20; x++)
		nitori_set(&hmap->kmap,x,6,(x>>1)+1);
	nitori_set(&hmap->kmap,4,5,4);
	// asset loading
	game_loadimg(gram,0,game_img_lut[0],KEINE_PIXELFMT_RGB15);
	game_loadimg(gram,1,game_img_lut[1],KEINE_PIXELFMT_RGB15);
	game_loadimg(gram,2,game_img_lut[2],KEINE_PIXELFMT_RGB15);
	game_loadimg(gram,3,game_img_lut[3],KEINE_PIXELFMT_RGB15);
	// come on, you got this!
	printf("GRAM usage: $%08lX\n",sizeof(game));
	printf("GRAM limit: $%08lX\n",sizeof(io->ram));
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
	u32 last_alive = suwa_objs->alive;
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
			if( !in_range(y,-32,io->h+32) ) kanako_del(suwa_objs,i);
			if( !in_range(x,-32,io->w+32) ) kanako_del(suwa_objs,i);
		}
	}
	if(last_alive != suwa_objs->alive)
	{ /* printf("<suwa_obj alive> $%04X\n",suwa_objs->alive); */ }
}

/*	--	draw funcs	--	*/
void game_draw(game *gram)
{
	// vars
	bios *io = gram->io;
	player *plrs = gram->plrs;
	kanako *suwa = &gram->suwa_objs;
	uint32_t time = io->time;
	// drawin maps
	game_drawmap(gram);
	// drawin players
	player_draw(&plrs[0]);
	// drawin objs
	for(u32 i=0; i<suwa->len; i++)
	{
		suwako *obj = &suwa->objs[i];
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
	// text drawin
	char alivestr[0x20],spdstr[0x20];
	sprintf(alivestr,"alive: $%04X/$%04X",suwa->alive,suwa->len);
	sprintf(spdstr,"gsp: $%08X\n",plrs[0].gsp);
	game_drawdebugtxt(gram,alivestr,0,0);
	game_drawdebugtxt(gram,spdstr,0,8);
}
void game_drawdebugtxt(game *gram,const char *txt,s32 x,s32 y)
{
	// vars
	keine *borefont = &gram->img_bank[GAME_IMG_BOREFONT];
	s32 ox = 0;
	s32 oy = 0;
	u32 len = strlen(txt);
	// drawin
	for(u32 i=0; i<len; i++)
	{
		char c = txt[i];
		if(c == '\n')
		{ ox = 0; oy++; }
		else {
			s32 dx = x + (ox*8);
			s32 dy = y + (oy*8);
			mokou_sprattr attr = { {dx,dy},0b00,0xFFFF, 0,0 };
			SDL_Rect srcrect = { (c&0xF)*8,(c>>4)*8,8,8 };
			mokou_spr16( borefont,gram->io->fb,srcrect,attr );
		
			ox++;
		}
	}
}
void game_drawmap(game *gram)
{
	hina *hmap = gram->hmap;
	hina_drawmap(&hmap[0]);
}

