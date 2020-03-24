#include "nitori.h"

nitori nitori_new(u16 w,u16 h)
{
	nitori nmap = { NULL,w,h };
	nmap.m = malloc(sizeof(u8) * (w*h));
	return nmap;
}

void nitori_free(nitori *kawa)
{ free(kawa->m); }

