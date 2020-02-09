#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "main.h"

struct ENTITY{
	char    *szName;
	int     idamagebase;
	int		idamage;
	float	fdamagemultiplier;
	
	int     idexteritybase;
	int		idexterity;
	float   fdexteritymultiplier;
	int		goldbase;

	long    lhpbase;
	long	lhp;
	float   fhpmultiplier;
	long    lexperience;
	long	treasure;
	ELEMENT eweakness;
	char    *threat;
};

enum CREATURES {GOBLIN,ORC,OGRE,MECHAZOID,DRAGON,ZOMBIE,SKELETON,GHAST,
	WRAITH,GIANT,CYCLOPS,VAMPIRE,MINOTAUR,MORDRED,TROLL,
	MORDRED_EVOLVED,GOBLIN_ELITE,OGRE_LEADER,GOLEM,DRAGON_SLAYER,DRAGOON_ELITE,BIOMECH};

static ENTITY g_Creatures[] = {
	{"Goblin"   ,1 ,3 ,2  ,1 ,3 ,2  ,10 ,15 ,5 ,2  ,10 ,1 ,NONE,"The goblin charges in a fiery rage."},
	{"Orc"      ,2 ,5 ,2  ,1 ,2 ,2  ,20 ,15 ,20,2  ,25 ,2 ,FIRE,"The orc awaits your move."},
	{"Ogre"     ,3 ,10,1.5,3 ,2 ,2  ,30 ,40 ,37,1  ,35 ,5 ,NONE,"The slobbering ogre walks toward you."},
	{"Mechazoid",10,20,1  ,2 ,5 ,2  ,40 ,60 ,65,2  ,250,4 ,THUNDER,"The Mechazoid wants to play!"},
	{"Dragon"   ,20,30,1  ,5 ,5 ,2  ,110,95 ,50,1.5,150,7 ,ICE,"The dragon reveals his wings."},
	{"Zombie"   ,10,10,1.5,1 ,2 ,1  ,50 ,20 ,15,1  ,30 ,2 ,FIRE,"The zombie slowly limps towards you."},
	{"Skeleton" ,9 ,10,1.5,1 ,2 ,1  ,20 ,8  ,14,1  ,40 ,1 ,HOLY,"The skeleton lunges at you."},
	{"Ghast"    ,10,21,1.5,2 ,5 ,1  ,60 ,30 ,20,1.5,60 ,1 ,DARK,"The ghast licks his lips."},
	{"Wraith"   ,10,10,1.5,2 ,5 ,2  ,65 ,40 ,25,1.5,70 ,3 ,HOLY,"The wraith moans sadly."},
	{"Giant"    ,7 ,12,2 ,5 ,2 ,1  ,90 ,50 ,50,1  ,80 ,4 ,NONE,"The giant looks down and smiles."},
	{"Cyclops"  ,13,15,2  ,6 ,2 ,1  ,95 ,85 ,25,2  ,75 ,6 ,ICE,"The cyclops looks at you with a hungry glare."},
	{"Vampire"  ,7 ,9 ,2  ,1 ,10,3  ,70 ,30 ,25,1  ,65 ,8 ,HOLY,"The vampire wants your blood."},
	{"Minotaur" ,13,4 ,1.5,2 ,2 ,1.5,85 ,80 ,60,1  ,85 ,2 ,WIND,"The minotaur looks at you ferociously."},
	{"Mordred"  ,25,35,1  ,5 ,3 ,1.5,160,160,150,1  ,215,8 ,DARK,"Mordred wants to see how strong you've become!"}, // form one
	{"Troll"    ,1 ,5 ,1.5,1 ,2 ,2  ,20 ,15 ,15,2  ,20 ,2 ,FIRE,"The troll viciously leaps at you."},

	{"Evolved Mordred",35,45,2  ,9 ,2 ,2  ,250,170,100,2  ,370,9,HOLY,"He's pissed!"}, // form two
	{"Goblin Elite"   ,3 ,10,1.5,2 ,5 ,2  ,85 ,20 ,35 ,1.5,75 ,2,NONE,"Goblin Elite is ready to fight."},
	{"Ogre Clan Leader",5 ,20,1.5,3 ,2 ,2 ,120,55 ,70 ,1.5,75 ,7,NONE,"Ogre Leader wants a snack!"},
	{"Golem",5,10,1.5,1,1,2,20,50,50,1.5,45,2,EARTH,"You hear a loud thud."},
	{"Dragon Slayer",5,10,1.5,1,1,2,20,50,50,1.5,65,2,EARTH,"'\'No talk. Just Die!\'"},
	{"Dragoon Elite",5,10,1.5,1,1,2,20,50,50,1.5,70,2,EARTH,"'\'The incompetent will die!\'"},
	{"Biomech",15,20,2,10,20,1,100,50,50,2,75,2,EARTH,"\'You have no future!!!\'"}};

#endif
