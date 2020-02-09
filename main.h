#ifndef _MAIN_H_
#define _MAIN_H_

#include "events.h"

#define TOTAL_POINTS  25

enum ENDFLAG{COMPLETE,CONTINUE,QUIT};
class PLAYER;

void MainMenu(PLAYER*);
void BeginMenu(PLAYER*);
void CreateCharacter(PLAYER*);
void InitNPCs();
void ResetEvents();
ENDFLAG Temple(PLAYER*);
ENDFLAG Equip(PLAYER*);
ENDFLAG Mountain(PLAYER*);
ENDFLAG Castle(PLAYER*);
ENDFLAG Dwarves(PLAYER*);
ENDFLAG Fortress(PLAYER*);
ENDFLAG Attributes(PLAYER*);
ENDFLAG Inventory(PLAYER*);

enum ELEMENT  {WIND,FIRE,ICE,THUNDER,EARTH,HOLY,DARK,NONE};
enum FUNCTION {ARMOR=1,WEAPON=2,HEAL=4,HEAL_ALL=8,MONEY=16,DAMAGE=32,RUNE=64,MISC=128,PARTS=256};

template < class T >
void swap( T &a, T &b )
{
	T c = a;
	a = b;
	b = c;
}

#endif