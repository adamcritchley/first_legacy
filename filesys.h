#ifndef _FILESYS_H_
#define _FILESYS_H_

#include "item.h"
#include "events.h"

struct FILEEVENT{
	char			  szName[25];
	unsigned int	  uiID;
	unsigned int	  uiValue;
};

struct FILEITEM{
	char			szname[26];
	unsigned int	uiID;			
	int				ilevel;
	unsigned int	efunction;
	int				iquantity;
	int				istrength;
	int				levelup;
	unsigned int	runelevels[7];
};

// contains version and the size and offsets of the inventory and events
struct FILEHEADER{
	char signature[3];
	unsigned int size_of_inv;
	unsigned int offset_to_inv;
	unsigned int size_of_events;
	unsigned int offset_to_events;
	unsigned int offset_to_stats;
};

// contains player stats
struct FILEBODY{
	char		  name[20];
	char		  file[50];
	unsigned long lhpmax;
	unsigned int  iintelligence;
	unsigned int  idexterity;
	unsigned int  ivitality;
	unsigned int  istrength;
	unsigned int  ilevel;
	unsigned long lexperience;
	unsigned long lweaponproficiency;
	unsigned int  armorID;
	unsigned int  weaponID;
};

// contains inventory and events
struct FILEFOOTER{
	unsigned int  sizeof_event_array;
	FILEEVENT	  *events_data;
	unsigned int  sizeof_item_array;
	FILEITEM	  *items_data;
};

struct FILEOBJECT{
FILEHEADER header;
FILEBODY   body;
FILEFOOTER footer;
};

FILEOBJECT *LoadFile(char *);
bool SaveFile(char *,FILEOBJECT);
ITEM FItoI(FILEITEM);
FILEITEM ItoFI(ITEM);
FILEEVENT EtoFE(EVENTSNODE);
EVENTSNODE FEtoE(FILEEVENT);

#endif