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
	keine_init(kernel->fb,w,h,KEINE_PIXELFMT_RGB15);
}

/*void bios_loadimg(bios *kernel,u32 index,char *fname,keine_pixelfmt fmt)
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
}*/

void bios_updt(bios *kernel)
{
	// update ticks
	kernel->lasttick = SDL_GetTicks();
	kernel->ftimer = 0;
	// update input
	SDL_PumpEvents();
	bios_checkquit(kernel);
}
void bios_checkquit(bios *kernel)
{
	kernel->quit = kernel->keystate[SDLK_ESCAPE];
}
/*	--	draw funcs	--	*/
void bios_draw(bios *kernel)
{
	bios_blitkene(kernel);
	bios_flip(kernel);
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
	uint32_t fps = 50;
	uint32_t ftime = SDL_GetTicks() - kernel->lasttick;
	uint32_t ftimer = kernel->ftimer + ftime;
	if( ftimer < (SDL_SECOND/fps) )
	{
		// scene: lasttick is 0 and curtick is 2
		// frames are 10, so u ned to wait (frame-curtick), or 8
		u32 ftime = (SDL_SECOND/fps) - ftimer;
		//printf("%02d %02d\n",ftime,SDL_SECOND/fps);
		SDL_Delay(ftime);
	}
	SDL_Flip(kernel->window);
	kernel->time++;
}

