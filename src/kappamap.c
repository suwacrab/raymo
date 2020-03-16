#include "kappamap.h"

kappamap kmap_new(u16 w,u16 h)
{
	kappamap nmap = { NULL,w,h };
	nmap.m = malloc(sizeof(u8) * (w*h));
	return nmap;
}

void kmap_free(kappamap *kmap)
{ free(kmap->m); }

