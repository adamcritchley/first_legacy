
#ifndef _EVENTS_H_
#define _EVENTS_H_

enum LVLFLAGS{NOT_COMPLETE=0,LVL_COMPLETE=2,GOT_ITEM=4,DID_SPECIAL=8,
				FOUND_SPECIAL=16,GOT_SPECIAL=32,OTHER_1=64,
				OTHER_2=128,OTHER_3=256,OTHER_4=512};

enum LEVELS{TEMPLE,MOUNTAIN,CASTLE,DWARVES,FORTRESS};
struct EVENTSNODE{
	char			  *szName;
	unsigned int	  uiID;
	LVLFLAGS		  uiValue;
};

#endif
