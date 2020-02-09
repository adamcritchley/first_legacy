#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

enum ITEMID{AXE,KNIFE,FOIL,BLADE,BROADSWORD,CLUB,
			SCIMITAR,HAMMER,CAPE,BREASTPLATE,CHAINMAIL,BALDRIC,
			POTION,GOLD,ICE_RUNE,FIRE_RUNE,
			THUNDER_RUNE,WIND_RUNE,EARTH_RUNE,DARK_RUNE,HOLY_RUNE,
			MAX_POTION,HAND,HILT,STAFF,HAND_COVER,SWORD};
struct ITEM;
typedef void (*LVLFUNCTION)(ITEM*);

struct RUNEDATA{
	unsigned int level[7];
};

struct ITEM{
	char		 szname[26];
	unsigned int uiID;			
	int			ilevel;
	FUNCTION	efunction;
	int			iquantity;
	int			istrength;
	RUNEDATA    srunes;
	LVLFUNCTION levelup;
};

void Axe(ITEM*);
void Blade(ITEM*);
void Broadsword(ITEM*);
void Club(ITEM*);
void Knife(ITEM*);
void Foil(ITEM*);
void Scimitar(ITEM*);
void Hammer(ITEM*);

void Breastplate(ITEM*);
void Baldric(ITEM*);
void Chainmail(ITEM*);
void Cape(ITEM*);

static ITEM g_Items[] = {{"Axe",AXE,1,WEAPON,0,1,{0,0,0,0,0,0,0},Axe}, // 0
	{"Knife",KNIFE,1,WEAPON,0,2,{0,0,0,0,0,0,0},Knife}, // 1
	{"Foil",FOIL,1,WEAPON,0,1,{0,0,0,0,0,0,0},Foil}, // 2
	{"Blade",BLADE,1,WEAPON,0,2,{0,0,0,0,0,0,0},Blade}, // 3
	{"Broadsword",BROADSWORD,1,WEAPON,0,3,{0,0,0,0,0,0,0},Broadsword}, // 4
	{"Club",CLUB,1,WEAPON,0,2,{0,0,0,0,0,0,0},Club}, // 5
	{"Scimitar",SCIMITAR,1,WEAPON,0,1,{0,0,0,0,0,0,0},Scimitar}, // 6
	{"Hammer",HAMMER,1,WEAPON,0,3,{0,0,0,0,0,0,0},Hammer}, // 7

	{"Cape",CAPE,1,ARMOR,0,1,{0,0,0,0,0,0,0},Cape}, // 8
	{"Breastplate",BREASTPLATE,1,ARMOR,0,3,{0,0,0,0,0,0,0},Breastplate}, // 9
	{"Chainmail",CHAINMAIL,1,ARMOR,0,2,{0,0,0,0,0,0,0},Chainmail}, // 10
	{"Baldric",BALDRIC,1,ARMOR,0,1,{0,0,0,0,0,0,0},Baldric}, // 11

	{"Potion",POTION,10,HEAL,0,10,{0,0,0,0,0,0,0},(LVLFUNCTION)0}, // 6 * rand(2) + 3 // 12
	{"Gold",GOLD,0,MONEY,0,0,{0,0,0,0,0,0,0},(LVLFUNCTION)0}, // 13

	{"Fire Rune",FIRE_RUNE,0,RUNE,0,200,{0,0,0,0,0,0,0},(LVLFUNCTION)0}, // 14
	{"Ice Rune",ICE_RUNE,0,RUNE,0,250,{0,0,0,0,0,0,0},(LVLFUNCTION)0}, // 250 * lvl to fasten
	{"Thunder Rune",THUNDER_RUNE,0,RUNE,0,310,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Earth Rune",EARTH_RUNE,0,RUNE,0,330,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Wind Rune",WIND_RUNE,0,RUNE,0,350,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Dark Rune",DARK_RUNE,0,RUNE,0,370,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Holy Rune",HOLY_RUNE,0,RUNE,0,400,{0,0,0,0,0,0,0},(LVLFUNCTION)0},

	{"Max Potion",MAX_POTION,0,HEAL_ALL,0,0,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Hand",HAND,0,WEAPON,0,0,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Hilt",HILT,0,PARTS,0,3,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Staff",STAFF,0,PARTS,0,2,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Hand Cover",HAND_COVER,0,PARTS,0,1,{0,0,0,0,0,0,0},(LVLFUNCTION)0},
	{"Sword Blade",SWORD,0,PARTS,0,4,{0,0,0,0,0,0,0},(LVLFUNCTION)0}}; // rand(lvl)%(lvl/3) + lvl

// HILT,SWORD,HAND_COVER,STAFF
#endif