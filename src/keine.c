#include "keine.h"

/*	--	init funcs -- */
size_t keine_initPAL4(keine *yago) { return (yago->w>>1) * yago->h; }
size_t keine_initPAL8(keine *yago) { return yago->w * yago->h; }
size_t keine_initRGB16(keine *yago) { return (yago->w<<1) * yago->h; }

keine_initfunc keine_initfmts[] = {
	&keine_initPAL4,&keine_initPAL8,
	&keine_initRGB16,&keine_initRGB16
};

/*	--	main funcs	--	*/
keine *keine_init(keine *yago,u32 w,u32 h,keine_pixelfmt fmt)
{
	// initializin
	yago->fillp = 0xFFFF;
	yago->w = w;
	yago->h = h;
	yago->fmt = fmt;
	yago->pal0 = NULL;
	yago->pal1 = NULL;
	yago->m = NULL;
	
	// allocatin memory for pixels
	yago->m = malloc(keine_initfmts[fmt](yago));
	return yago;
}
keine *keine_loadimg(keine *yago,const char *fname,keine_pixelfmt fmt)
{
	// vars n shit
	SDL_Surface *loadimg = NULL; // loaded img
	SDL_Surface *convimg = NULL; // converted img
	// loadin
	loadimg = IMG_Load(fname);
	if(loadimg == NULL)
	{ printf("loading failed: %s\n",IMG_GetError()); exit(-1); }	
	else {
		switch(fmt)
		{
			case KEINE_PIXELFMT_RGB565:
			{
				SDL_PixelFormat sdlfmt = { 
					NULL,16,2, // pal,bpp,bytes
					3,2,3,0, // loss (rgba)
					0,5,11,0, // shift (rgba)
					31,63<<5,31<<11,0, // masks (rgba)
					0,0xFF // colorkey,alpha
				};
				convimg = SDL_ConvertSurface(loadimg,&sdlfmt,SDL_SWSURFACE);
				SDL_LockSurface(convimg);
				u32 w = convimg->w;
				u32 h = convimg->h;
				// ???
				keine_init(yago,w,h,fmt);
				u16 *convpix = (u16*)convimg->pixels;
				u16 *yagopix = (u16*)yago->m;
				for(u32 i=0; i<(w*h); i++)
				{
					u8 r,g,b;
					SDL_GetRGB(convpix[i],&sdlfmt,&r,&g,&b);
					yagopix[i] = convpix[i]; //BGR565(r>>3,g>>2,b>>3);
				}
				SDL_FreeSurface(loadimg);
				SDL_FreeSurface(convimg);
				break;
			}
			case KEINE_PIXELFMT_PAL4:
			{
				convimg = loadimg;
				u32 w = convimg->w;
				u32 h = convimg->h;
				u32 hw = convimg->w>>1;
				keine_init(yago,w,h,fmt);
				keine_clear(yago);
				yago->pal0 = malloc(sizeof(RGB16) * 16);
				yago->pal1 = malloc(sizeof(RGB16) * 16);
				// palette loadin
				for(u32 c=0; c<16; c++) // ha
				{
					SDL_Color clr = convimg->format->palette->colors[c];
					yago->pal0[c] = RGB15( clr.r>>3,clr.g>>3,clr.b>>3 );
					yago->pal1[c] = yago->pal0[c];
				}
				
				// fb loadin
				for(u32 y=0; y<h; y++)
				{
					for(u32 x=0; x<w; x++)
					{
						u32 ind = (x>>1) + (y*hw);
						u32 opix = ((RGB8*)yago->m)[ind];
						u32 npix = ((RGB8*)convimg->pixels)[x + (y*w)];
						npix &= 0xF;
						u32 b0 = npix << (4*(x&1)); // if odd, shift left
						u32 b1 = opix & (0xF0>>(4*(x&1))); // if odd, shift right
						((RGB8*)yago->m)[ind] = b0 | b1;
					}
				}

				//SDL_FreeSurface(convimg);
				break;
			}
			case KEINE_PIXELFMT_RGB15:
			{
				SDL_PixelFormat sdlfmt = {
					NULL,32,4,
					0,0,0,0, // loss(rgba)
					0,8,16,24, // shift (rgba)
					255,255<<8,255<<16,255<<24, // masks (rgba)
					0,0xFF
				};

				convimg = SDL_ConvertSurface(loadimg,&sdlfmt,SDL_SWSURFACE);
				SDL_LockSurface(convimg);
				u32 w = convimg->w;
				u32 h = convimg->h;
				keine_init(yago,w,h,fmt);
				memset(yago->m,0,keine_imgsize(yago));
				u32 *convpix = (u32*)convimg->pixels;
				u16 *yagopix = (u16*)yago->m;
				for(u32 i=0; i<(w*h); i++)
				{ 
					u8 r,g,b;
					SDL_GetRGB(convpix[i],&sdlfmt,&r,&g,&b);
					yagopix[i] = RGB15(r>>3,g>>3,b>>3);
				}
				SDL_FreeSurface(loadimg);
				SDL_FreeSurface(convimg);
				break;
			}
		}
	}
	
	// return the loaded img
	return yago;
}
void keine_free(keine *yago)
{
	if(yago->m != NULL) { free(yago->m); yago->m = NULL; }
	if(yago->pal0 != NULL) { free(yago->pal0); yago->pal0 = NULL; }
	if(yago->pal1 != NULL) { free(yago->pal1); yago->pal1 = NULL; }
}

void keine_clear(keine *yago)
{
	memset(yago->m,0,keine_imgsize(yago));
}

