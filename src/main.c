#include "kbase.h"
#include "bios.h"
#include "game.h"

#define WIDTH (320)
#define HEIGHT (240)

// sdl vars
bios eys_bios;
game game_ram;

void init_eys();

int main(void)
{
	// init
	init_eys();
	// main
	// exit
	return 0;
}

void init_eys()
{
	bios_init(&eys_bios,WIDTH,HEIGHT);
	game_init(&game_ram,&eys_bios);
}

