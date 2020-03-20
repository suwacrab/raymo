#include "kbase.h"
#include "suwako.h"

#ifndef KANAKO_H
#define KANAKO__H

/*	--	obj yasas	--	*/
typedef struct kanako 
{
	suwako *objs;
	u16 first,len;
} kanako;

/*	--	yasa funcs	--	*/
kanako *kanako_init(kanako *yasa,suwako *objs,u16 len);
suwako *kanako_add(kanako *yasa);
void kanako_updt(kanako *yasa);

#endif

