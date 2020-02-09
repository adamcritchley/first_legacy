#ifndef _GAME_H_
#define _GAME_H_

#include "entity.h"
#include "player.h"
enum ENDFLAG;

struct BATTLEINFO{
	int		num_monsters;
	ENTITY *monsters;
};

class GAME{
public:
	bool LoadGame(PLAYER*,EVENTSNODE *);
	bool SaveGame(PLAYER*,EVENTSNODE *);

	unsigned int KeyLoop(unsigned char *);
	
	ENDFLAG Battle(PLAYER*,BATTLEINFO*,PLAYER* npc = NULL,bool prompt = true);
private:
	unsigned int GetKey();

};

static GAME g_Game;

#endif