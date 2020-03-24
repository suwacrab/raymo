#include "kbase.h"
#include "suwako.h"

#ifndef KANAKO_H
#define KANAKO_H

/*	--	obj yasas	--	*/
typedef struct kanako 
{
	suwako *objs;
	u16 first,len,alive;
} kanako;

/*	--	yasa funcs	--	*/
extern kanako *kanako_init(kanako *yasa,suwako *objs,u16 len);
extern suwako *kanako_add(kanako *yasa);
extern void kanako_del(kanako *yasa,u16 id);

#endif

