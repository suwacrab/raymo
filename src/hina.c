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
	
}
