#include "bios.h"
#include "player.h"

/*	--	 main functions	--	*/
void bios_init(bios *kernel,u32 w,u32 h)
{
	/*	--	sdl init	--	*/
	// main
	s32 sdlflags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	if( SDL_Init(sdlflags) < 0 )
	{ printf("couldn't init sdl: '%s'\n",SDL_GetError()); exit(-1); }
	// video
	s32 vidflags = SDL_SWSURFACE;
	kernel->window = SDL_SetVideoMode(w,h,16,vidflags);
	kernel->w = w; kernel->h = h;
	// event
	kernel->quit = false;
	kernel->time = 0;
	kernel->keystate = SDL_GetKeyState(NULL);

	/*	--	mokokene init	--	*/
	// fb
	kernel->fb = malloc(sizeof(keine));
	kernel->img_bank = malloc(sizeof(keine*)*0x10);
	keine_init(kernel->fb,w,h,KEINE_PIXELFMT_RGB15);

	/*	--	game init	--	*/
	kernel->plrs = malloc(sizeof(player)*4);
	kernel->bgs = malloc(sizeof(kappamap)*4);
	// player
	player *plrs = kernel->plrs;
	for(u32 i=0; i<4; i++) player_init(&plrs[i],kernel);
	// maps
	kappamap *bgs = kernel->bgs;
	bgs[0] = kmap_new(32,32);
}
void bios_loadimg(bios *kernel,u32 index,char *fname,keine_pixelfmt fmt)
{
	// load image
	uint32_t start = SDL_GetTicks();
	kernel->img_bank[index] = malloc(sizeof(keine));
	keine *curimg = kernel->img_bank[index];
	keine_loadimg(curimg,fname,fmt);
	// print info
	u32 loadtime = SDL_GetTicks() - start;
	u64 addr = (u64)curimg;
	printf("<image loaded> [%04X] [%04lX:%04lX] [load time: %04X] '%s'\n",
		index,addr>>16,addr&65535,loadtime,fname
	);
}

/*	--	update funcs	--	*/
void bios_boot(bios *kernel)
{
	kernel->lasttick = SDL_GetTicks();
	while( !kernel->quit )
	{
		// update & draw
		bios_update(kernel);
		bios_draw(kernel);
		// vsync
		bios_blitkene(kernel);
		bios_flip(kernel);
	}
}
void bios_update(bios *kernel)
{
	// vars
	player *plrs = kernel->plrs;
	// update input
	SDL_PumpEvents();
	bios_checkquit(kernel);
	// update player
	player_updt(&plrs[0]);
}
void bios_checkquit(bios *kernel)
{
	kernel->quit = kernel->keystate[SDLK_ESCAPE];
}

/*	--	draw funcs	--	*/
void bios_draw(bios *kernel)
{
	// vars
	keine *fb = kernel->fb;
	uint32_t time = kernel->time;
	keine **img_bank = kernel->img_bank;

	player *plrs = kernel->plrs;
	// clearing
	bios_clearscreen(kernel);
	// player drawin
	player_draw(&plrs[0]);
}
void bios_blitkene(bios *kernel)
{
	keine *curfb = kernel->fb;
	SDL_Rect drect = { 0,0,1,1 };
	for(u32 y=0; y<curfb->h; y++)
	{
		drect.y = y;
		for(u32 x=0; x<curfb->w; x++)
		{
			drect.x = x;
			u32 px = mokou_pget16(curfb,x,y);
			u32 r = (px&31); r <<= 3;
			u32 g = (px>>5)&31; g <<= 3;
			u32 b = (px>>10)&31; b <<= 3;
			SDL_FillRect(kernel->window,&drect,
				SDL_MapRGB(kernel->window->format,r,g,b)
			);
		}
	}
}
void bios_clearscreen(bios *kernel)
{
	SDL_Surface *window = kernel->window;
	// clear window
	SDL_Rect windowrect = { 0,0,kernel->w,kernel->h };
	SDL_FillRect(window,&windowrect,
		SDL_MapRGB(window->format,0,0,0)
	);
	// clear fb
	keine_clear(kernel->fb);
}
void bios_flip(bios *kernel)
{
	SDL_Flip(kernel->window);
	SDL_Delay( SDL_SECOND/50 );
	kernel->lasttick = SDL_GetTicks();
	kernel->time++;
}



