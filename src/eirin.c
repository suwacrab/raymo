#include "keine.h"

/*	--	init funcs -- */
size_t keine_initPAL4(eirin *yago) { return sizeof(u8) * (yago->w>>1) * yago->h; }
size_t keine_initPAL8(eirin *yago) { return sizeof(u8) * yago->w * yago->h; }
size_t keine_initRGB565(eirin *yago) { return sizeof(u8) * (yago->w<<1) * yago->h; }

keine_initfunc eirin_initfmts[] = {
	&keine_initPAL4,&eirin_initPAL8,
	&keine_initRGB565
};

/*	--	main funcs	--	*/
keine *eirin_init(eirin *yago,u32 w,u32 h,eirin_pixelfmt fmt)
{
	// initializin
	yago->w = w;
	yago->h = h;
	yago->fmt = fmt;
	yago->palette = NULL;
	yago->m = NULL;
	
	// allocatin memory for pixels
	yago->m = calloc( 1,keine_initfmts[fmt](yago) );
	return yago;
}
keine *eirin_loadimg(eirin *yago,const char *fname,eirin_pixelfmt fmt)
{
	// vars n shit
	printf("loading image '%s'...   ",fname);
	SDL_Surface *loadimg = NULL; // loaded img
	SDL_Surface *convimg = NULL; // converted img
	// loadin
	loadimg = IMG_Load(fname);
	if(loadimg == NULL)
	{ printf("loading failed: %s\n",IMG_GetError()); exit(-1); }	
	else {
		if(fmt == EIRIN_PIXELFMT_RGB565)
		{
			SDL_PixelFormat sdlfmt = { 
				NULL,16,2, // pal,bpp,bytes
				3,2,3,0, // loss (rgba)
				11,5,0,0, // shift (rgba)
				31<<11,63<<5,31,0, // masks (rgba)
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
				yagopix[i] = RGB565(r>>3,g>>2,b>>3);
			}
			SDL_FreeSurface(loadimg);
			SDL_FreeSurface(convimg);
		}
	}
	
	printf("image loaded! [%p]\n",yago);
	// return the loaded img
	return yago;
}
keine *eirin_free(eirin *yago)
{
	if(yago->m != NULL) { free(yago->m); yago->m = NULL; }
	if(yago->palette != NULL) { free(yago->palette); yago->palette = NULL; }
}

