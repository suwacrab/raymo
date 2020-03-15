#include "bios.h"
#include "kbase.h"

#define WIDTH (320)
#define HEIGHT (240)

// sdl vars
bios *eys_bios = NULL;

void init_eys();

int main(void)
{
	// init
	init_eys();
	// main
	bios_boot(eys_bios);
	// exit
	return 0;
}

void init_eys()
{
	eys_bios = (bios*)malloc(sizeof(bios));
	bios_init(eys_bios,WIDTH,HEIGHT);
	bios_loadimg(eys_bios,0x00,"gfx/testtex.png",KEINE_PIXELFMT_RGB15);
	bios_loadimg(eys_bios,0x01,"gfx/testtile.png",KEINE_PIXELFMT_RGB15);
}

