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
	eys_bios = malloc( MBSIZE(2) );
	bios_init(eys_bios,WIDTH,HEIGHT);
	bios_loadimg(eys_bios,0x00,"gfx/testtex.png",KEINE_PIXELFMT_RGB15);
	bios_loadimg(eys_bios,0x01,"gfx/testtile.png",KEINE_PIXELFMT_RGB15);
	bios_loadimg(eys_bios,0x02,"gfx/raymo.png",KEINE_PIXELFMT_RGB15);
}

