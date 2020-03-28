#include "kbase.h"
#include "bios.h"
#include "game.h"

const u32 WIDTH = 320;
const u32 HEIGHT = 192;

// sdl vars
bios eys_bios;
game *game_ram = NULL;

void init_eys();

int main(void)
{
	// init
	init_eys();
	// main
	game_run(game_ram);
	// exit
	return 0;
}

void init_eys()
{
	bios_init(&eys_bios,WIDTH,HEIGHT);
	game_ram = (game*)eys_bios.ram;
	game_init(game_ram,&eys_bios);
}

