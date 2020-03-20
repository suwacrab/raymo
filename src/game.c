#include "game.h"
#include "player.h"

void game_init(game *gram,bios *io)
{
	// bios
	gram->io = io;
	// player init
	gram->plrs = (player*)gram->plrmem;
}

