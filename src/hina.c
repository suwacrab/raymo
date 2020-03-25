#include "hina.h"

/* -- main funcs -- */
hina *hina_init(hina *kagi,game *gram,HINA_SIZE size)
{
	// prop init
	kagi->size = size;
	kagi->gram = gram;
	kagi->img = NULL;
	// nitohina ON
	kagi->kmap.w = 0;
	kagi->kmap.h = 0;
	kagi->kmap.m = NULL;
}

void hina_drawmap(hina *kagi)
{
	// vars
	HINA_SIZE size = kagi->size;
	nitori *kmap = &kagi->kmap;
	game *gram = kagi->gram;
	bios *io = gram->io;

	u8 tile;
	SDL_Rect srcrect = { 0,0,hina_wide(size),hina_wide(size) };
	mokou_sprattr attr = { {0,0},0b00,0xFFFF, 0,0 };
	// drawin
	for( s32 y=0; y<(s32)kmap->h; y++)
	{
		for( s32 x=0; x<(s32)kmap->w; x++)
		{
			tile = nitori_get(kmap,x,y);
			if(tile>0)
			{
				attr.pos.x = hina_pos(size,x);
				attr.pos.y = hina_pos(size,y);
				srcrect.x = (tile&15) * hina_wide(size);
				srcrect.y = (tile>>4) * hina_wide(size);
				mokou_spr16(kagi->img,io->fb,srcrect,attr);
			}
		}
	}	
}
