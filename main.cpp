#include <iostream>
#include <iomanip>
#include "events.h"
#include "player.h"
#include "main.h"
#include "item.h"
#include "entity.h"
#include "game.h"
#include "system.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_POINTS 30

EVENTSNODE g_Events [] = {  {"TEMPLE",0,NOT_COMPLETE},
							{"MOUNTAIN",0,NOT_COMPLETE},
							{"CASTLE",0,NOT_COMPLETE},
							{"DWARVES",0,NOT_COMPLETE},
							{"FORTRESS",0,NOT_COMPLETE}};

PLAYER Paristan;

void main()
{
	PLAYER player;

	BeginMenu(&player);
}

void ResetEvents()
{
	memset(g_Events,0,sizeof(g_Events));
	g_Events[0].szName = "TEMPLE";
	g_Events[1].szName = "MOUNTAIN";
	g_Events[2].szName = "CASTLE";
	g_Events[3].szName = "DWARVES";
	g_Events[4].szName = "FORTRESS";
}

void InitNPCs()
{
	ITEM wItem = g_Items[(int)BLADE];
	wItem.ilevel = 3;

	Paristan.SetCurrentArmor(g_Items[(int)BALDRIC]);
	Paristan.SetCurrentWeapon(wItem);
	Paristan.SetDexterity(25);
	Paristan.SetExperience(0);
	Paristan.SetHP(105);
	Paristan.SetHPMax(105);
	Paristan.SetIntelligence(30);
	Paristan.SetLevel(8);
	Paristan.SetName("Paristan");
	Paristan.SetProficiency(0);
	Paristan.SetStrength(10);
	Paristan.SetVitality(20);
}

void MainMenu(PLAYER *plyr)
{
	char file[51];
	char options[23];
	unsigned char yesno[] = {'Y','N','n','y',0};
	unsigned int  result;
	unsigned int  lvlflag = (int)COMPLETE;
	INVENTORY *ptr;
	PLAYER before;

	do
	{
		memset(options,0,sizeof(options));
		memcpy((void*)&before,(const void *)plyr,sizeof(PLAYER));

		system("CLS");

		std::cout << "(E)xit Game" << std::endl;
		strcat(options,"eE");

		std::cout << "(S)ave Game" << std::endl;
		strcat(options,"sS");

		std::cout << "(I)nventory" << std::endl;
		strcat(options,"iI");

		std::cout << "(O)rganize Equipment" << std::endl;
		strcat(options,"oO");

		if(g_Events[(int)TEMPLE].uiValue & (int)DID_SPECIAL)
		{
			std::cout << "(A)ttributes" << std::endl;
			strcat(options,"aA");
		}

		if(g_Events[(int)MOUNTAIN].uiValue & (int)FOUND_SPECIAL)
		{
			std::cout << "(D)warves Town" << std::endl;
			strcat(options,"dD");
		}

		std::cout << "(T)emple" << std::endl;
		strcat(options,"tT");

		if(g_Events[(int)TEMPLE].uiValue & (int)LVL_COMPLETE)
		{
			if(!(g_Events[(int)MOUNTAIN].uiValue & (int)GOT_SPECIAL))
			{
				std::cout << "(M)ountain" << std::endl;
				strcat(options,"mM");
			}
		}

		if((g_Events[(int)MOUNTAIN].uiValue & (int)LVL_COMPLETE) || 
			(g_Events[(int)MOUNTAIN].uiValue & (int)GOT_SPECIAL))
		{
			std::cout << "(C)astle" << std::endl;
			strcat(options,"cC");
		}

		if(g_Events[(int)CASTLE].uiValue & (int)DID_SPECIAL)
		{
			std::cout << "(F)ortress" << std::endl;
			strcat(options,"fF");
		}

		std::cout << "(R)eset Level Data" << std::endl;
		strcat(options,"rR");

		result = g_Game.KeyLoop((unsigned char*)options);

		switch(result)
		{
		case 'r':
		case 'R':
			ResetEvents();
			break;
		case 'f':
		case 'F':
			lvlflag = Fortress(plyr);
			break;
		case 'c':
		case 'C':
			lvlflag = Castle(plyr);
			break;
		case 'i':
		case 'I':
			lvlflag = Inventory(plyr);
			break;
		case 'm':
		case 'M':
			lvlflag = Mountain(plyr);
			break;
		case 'a':
		case 'A':
			lvlflag = Attributes(plyr);
			break;
		case 'e':
		case 'E':
			// exit
			lvlflag = QUIT;
			break;
		case 'd':
		case 'D':
			lvlflag = Dwarves(plyr);
			break;
		case 't':
		case 'T':
			lvlflag = Temple(plyr);
			break;
		case 'o':
		case 'O':
			lvlflag = Equip(plyr);
			break;
		case 's':
		case 'S':
			if(!g_Game.SaveGame(plyr,g_Events))
			{
				std::cout << "Error saving file!" << std::endl;
				std::cout << "Would you like to change your save file name (Y/N)?";

				if( tolower( g_Game.KeyLoop( yesno ) ) == 'y' )
				{
					do{
						std::cin.getline(file, sizeof(file), '\n');
					}while(!strlen(file));

					plyr->SetFile(file);

					std::cout << std::endl << "Please try to resave the file." << std::endl;
					system("pause");
				}
			}
			break;
		}

		switch(lvlflag)
		{
		case CONTINUE:
			ptr = plyr->GetInventory()->Begining(); // keep inventory the same
			memcpy((void*)plyr,(const void*)&before,sizeof(PLAYER));
			plyr->SetInventory(ptr);
			break;
		}

	}while( lvlflag != QUIT );

	SafeMT(plyr->GetInventory()->End());
	memset(plyr,0,sizeof(PLAYER));
	ResetEvents();
}

ENDFLAG Equip(PLAYER* plyr)
{
	char name[50];
	INVENTORY *inv;
	int x=0,y=0;
	system("cls");

	while(1)
	{
		inv = plyr->GetInventory()->Begining();
		std::cout << "Armor in Inventory:" << std::endl;
		while(inv)
		{
			// add rune levels (defense)
			if(inv->GetItem()->efunction==ARMOR &&
				inv->GetItem()->uiID != plyr->GetCurrentArmor().uiID)
			{
				std::cout << inv->GetItem()->szname << " Strength: " 
					 << inv->GetItem()->istrength << " Level: " 
					 << inv->GetItem()->ilevel << " ";
				
				std::cout << std::endl;
			}
			inv = inv->GetNext();
		}

		inv = plyr->GetInventory()->Begining();
		std::cout << std::endl << "Weapons in Inventory:" << std::endl;
		while(inv)
		{
			// add rune levels (attack)
			if(inv->GetItem()->efunction==WEAPON && 
				inv->GetItem()->uiID != plyr->GetCurrentWeapon().uiID)
			{
				std::cout << inv->GetItem()->szname << " Strength: " 
					 << inv->GetItem()->istrength << " Level: " 
					 << inv->GetItem()->ilevel << ' ';
			
				for(x=0; x < 8; x++)
				{
					if(inv->GetItem()->srunes.level[x])
					{
						switch((ELEMENT)x)
						{
						case DARK:
							std::cout << "Dark: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						case HOLY:
							std::cout << "Holy: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						case FIRE:
							std::cout << "Fire: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						case ICE:
							std::cout << "Ice: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						case THUNDER:
							std::cout << "Thunder: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						case EARTH:
							std::cout << "Earth: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						case WIND:
							std::cout << "Wind: " << inv->GetItem()->srunes.level[x] << ' ';
							break;
						}
						if(x == 4)
							std::cout << std::endl;
					}
				}

				std::cout << std::endl;
			}
			inv = inv->GetNext();
		}


		std::cout << std::endl << "Current Weapon:" << std::endl;
		std::cout << plyr->GetCurrentWeapon().szname << " Strength: " 
			 << plyr->GetCurrentWeapon().istrength << " Level: " 
			 << plyr->GetCurrentWeapon().ilevel << std::endl;

		for(x=0; x < 8; x++)
		{
			if(plyr->GetCurrentWeapon().srunes.level[x])
			{
				switch((ELEMENT)x)
				{
				case DARK:
					std::cout << "Dark: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				case HOLY:
					std::cout << "Holy: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				case FIRE:
					std::cout << "Fire: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				case ICE:
					std::cout << "Ice: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				case THUNDER:
					std::cout << "Thunder: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				case EARTH:
					std::cout << "Earth: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				case WIND:
					std::cout << "Wind: " << plyr->GetCurrentWeapon().srunes.level[x] << ' ';
					break;
				}
			}
		}

		std::cout << std::endl << std::endl << "Current Armor:" << std::endl;
		std::cout << plyr->GetCurrentArmor().szname << " Strength: " 
			 << plyr->GetCurrentArmor().istrength << " Level: " 
			 << plyr->GetCurrentArmor().ilevel << std::endl;

		std::cout << std::endl << std::endl << "What would you like to equip? (End to go back)" << std::endl;
		std::cin.getline(name, sizeof(name), '\n');

		if(!strcmpi(name,"end"))
			break;

		system("cls");
		inv = plyr->GetInventory()->FindItem(name);

		if(!inv)
		{
			if(y)
				std::cout << "Item not found!" << std::endl << std::endl;
		}
		else
		{
			switch(inv->GetItem()->efunction)
			{
			case WEAPON:
				plyr->SetProficiency(0);
				plyr->GetInventory()->FindItem(plyr->GetCurrentWeapon().szname)->SetItem(plyr->GetCurrentWeapon());
				plyr->SetCurrentWeapon(*inv->GetItem());
				break;
			case ARMOR:
				plyr->SetProficiency(0);
				plyr->GetInventory()->FindItem(plyr->GetCurrentArmor().szname)->SetItem(plyr->GetCurrentArmor());
				plyr->SetCurrentArmor(*inv->GetItem());
				break;
			default:
				std::cout << "Cannot equip that Item!" << std::endl << std::endl;
				break;
			}
		}

		y=1;
	}
		
	return COMPLETE;
}

ENDFLAG Inventory(PLAYER* plyr)
{
	int c=0;
	system("cls");
	INVENTORY *inv;

	inv = plyr->GetInventory()->Begining();
	while(inv)
	{
		if(inv->GetItem()->iquantity)
		{
			switch(inv->GetItem()->efunction)
			{
			case HEAL:
				std::cout << '(' << (c+1) << ") " << inv->GetItem()->szname << 'x' 
					 << inv->GetItem()->iquantity << std::endl;
				c++;
				break;
			case DAMAGE:
				std::cout << '(' << (c+1) << ") " << inv->GetItem()->szname << 'x'
					 << inv->GetItem()->iquantity << " (Strength: " 
					 << inv->GetItem()->istrength << ')' << std::endl;
				c++;
				break;
			case RUNE:
				std::cout << '(' << (c+1) << ") " << inv->GetItem()->szname << 'x' 
					 << inv->GetItem()->iquantity << std::endl;
				c++;
				break;
			case MISC:
				std::cout << '(' << (c+1) << ") " << inv->GetItem()->szname << std::endl;
				c++;
				break;
			case PARTS:
				std::cout << '(' << (c+1) << ") " << inv->GetItem()->szname;
				std::cout << 'x' << inv->GetItem()->iquantity;
				std::cout << std::endl;
				c++;
				break;
			}
		}
		inv = inv->GetNext();
	}

	if(!c)
		std::cout << "No items are present.";

	std::cout << std::endl;
	system("PAUSE");
	return COMPLETE;
}

ENDFLAG Attributes(PLAYER* plyr)
{
	system("cls");
	std::cout << "Lord " << plyr->GetName() << " level " << plyr->GetLevel() 
		 << std::endl << "Hit Points of " << plyr->GetMaxHP()
		 << std::endl << "Strength of " << plyr->GetStrength() 
		 << std::endl << "Dexterity of " << plyr->GetDexterity()
		 << std::endl << "Intelligence of " << plyr->GetIntelligence()
		 << std::endl << "Vitality of " << plyr->GetVitality()
		 << std::endl << std::endl << "Weapon Proficiency of " << plyr->GetProficiency() 
		 << " with the " << plyr->GetCurrentWeapon().szname 
		 << std::endl << "Current experience: " << plyr->GetExperience()
		 << std::endl << "Experience till next level: " << abs((plyr->GetExperience() - ((long)ceil(100+(pow(10,plyr->GetLevel())/pow(2,plyr->GetLevel()+1))))))
		 << std::endl << "You have " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity << " gold!" << std::endl;
	
	system("pause");
	return COMPLETE;
}

void BeginMenu(PLAYER *plyr)
{
	unsigned char options[] = {'l','L','N','n','e','E',0};
	unsigned char result;
	bool exit = false;

	while(!exit)
	{
		InitNPCs();
		ResetEvents();

		system("CLS");
		std::cout << "Welcome to First Legacy! V4" << std::endl
			 << "Please choose an option:" << std::endl
			 << "(L)oad Game" << std::endl
			 << "(N)ew Game" << std::endl
			 << "(E)xit Game" << std::endl;

		result = g_Game.KeyLoop(options);

		switch(result)
		{
		case 'l':
		case 'L':
			if(!g_Game.LoadGame(plyr,g_Events))
			{
				std::cout << "Couldn't open specified save file..." << std::endl;
				system("pause");
			}
			else
			{
				MainMenu(plyr);
			}
			break;
		case 'N':
		case 'n':
			CreateCharacter(plyr);
			MainMenu(plyr);
			break;
		case 'E':
		case 'e':
			exit = true;
			break;
		}
	}
}

void CreateCharacter(PLAYER *plyr)
{
	system("CLS");
	unsigned int result;
	unsigned char yesno[] = {'Y','N','n','y',0};
	int points=0;
	int choice;
	int vitality;
	ITEM gold;
	char name[20];
	char file[50];
	srand((unsigned int)time(NULL));

	std::cout << "What is your adventurer\'s name? (20 characters)" << std::endl;
	do{
		std::cin.getline(name, 20, '\n');
	}while(!strlen(name));

	std::cout << std::endl << "What should your save file be named? (50 characters)" << std::endl;
	do{
		std::cin.getline(file, 50, '\n');
	}while(!strlen(file));

	plyr->SetName(name);
	plyr->SetFile(file);
	plyr->SetCurrentArmor(g_Items[(int)CAPE]);
	plyr->SetCurrentWeapon(g_Items[(int)FOIL]);
	plyr->SetExperience(0);
	plyr->SetLevel(1);
	plyr->SetVitality(0);
	plyr->SetProficiency(0);
	gold = g_Items[(int)GOLD];
	gold.iquantity = 125;
	plyr->GetInventory()->SetItem(gold);
	gold = g_Items[(int)FOIL];
	plyr->GetInventory()->SetNext(&gold);
	gold = g_Items[(int)CAPE];
	plyr->GetInventory()->GetNext()->SetNext(&gold);
	plyr->SetHPMax(20);

	do
	{
		system("CLS");
		points = 0;

		std::cout << "You have a total of " << MAX_POINTS << " points to distribute" << std::endl
			<< "among your four abilities. Choose Well!" << std::endl << std::endl;

		std::cout << std::endl << "Points to distribute: " << MAX_POINTS - points << std::endl
			 << "Strength: ";
		std::cin >> choice;
		points += choice;
		plyr->SetStrength(choice);

		if(MAX_POINTS-points <= 0 || points >= MAX_POINTS)
			continue;

		std::cout << std::endl << "Points to distribute: " << MAX_POINTS - points << std::endl
			 << "Vitality: ";
		std::cin >> choice;
		points += choice;
		vitality = choice;

		if(MAX_POINTS-points <= 0 || points >= MAX_POINTS)
			continue;

		std::cout << std::endl << "Points to distribute: " << MAX_POINTS - points << std::endl
			 << "Dexterity: ";
		std::cin >> choice;
		points += choice;
		plyr->SetDexterity(choice);

		if(MAX_POINTS-points <= 0 || points >= MAX_POINTS)
			continue;

		std::cout << std::endl << "Intelligence: " << MAX_POINTS - points;
		plyr->SetIntelligence(MAX_POINTS-points);

		std::cout << std::endl << "Are these ok? (Y/N)" << std::endl;

		result = g_Game.KeyLoop(yesno);
	}while(result != 'Y' && result != 'y');

	plyr->AddVitality(vitality);
	plyr->SetHP(plyr->GetMaxHP());
}

ENDFLAG Temple(PLAYER* plyr)
{
	srand((unsigned int)time(NULL));
	BATTLEINFO bi;
	ITEM weapon,wItem;
	unsigned int flags = g_Events[(int)TEMPLE].uiValue;
	int x;
	ENDFLAG resultf;
	unsigned int result;
	unsigned char *keys;
	unsigned char yesno[] = {'Y','N','n','y',0};
	system("cls");

	std::cout << std::setw(5) << "" << "Welcome to the Temple of Peril!" << std::endl;
	std::cout << "As you stand in front of small temple" << std::endl;
	std::cout << "you can\'t help but notice it\'s majestic" << std::endl;
	std::cout << "beauty, and stand to gaze at it in awe." << std::endl;
	std::cout << "Then you happen to notice the goblin" << std::endl;
	std::cout << "anxiously shaking his finger at the crude" << std::endl;
	std::cout << "and highly unnoticeable toll sign which" << std::endl;
	std::cout << "reads (110 GolD peIcES to pAss). Why" << std::endl;
	std::cout << "even bother with it thinking to yourself?" << std::endl << std::endl;

	std::cout << "(P)ay or (F)ight the goblin?" << std::endl;
	keys = new unsigned char[5];
	keys[0] = 'f';
	keys[1] = 'F';
	keys[2] = 'P';
	keys[3] = 'p';
	keys[4] = 0;
	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
	case 'f':
	case 'F':
		bi.num_monsters = 1;
		bi.monsters = new ENTITY;
		bi.monsters[0] = g_Creatures[(int)GOBLIN];
		resultf = g_Game.Battle(plyr,&bi);
		delete [] bi.monsters;

		if(resultf != COMPLETE)
			return resultf;
		break;
	case 'p':
	case 'P':
		if(plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity < 110)
		{
			std::cout << "Where\'s the rest of me GOLD!" << std::endl << std::endl;
			bi.num_monsters = 1;
			bi.monsters = new ENTITY;
			bi.monsters[0] = g_Creatures[(int)GOBLIN];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;
		}
		else
		{
			plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity -= 110;
			std::cout << "Thanks for my gold." << std::endl;
			std::cout << "Go ahead, GO!" << std::endl << std::endl;
		}
		break;
	}

	if(!(flags & (int)GOT_ITEM))
	{
		std::cout << std::setw(5) << "" << "You stand in the beautiful archway." << std::endl;
		std::cout << std::setw(5) << "" << "You happen to notice a rune stuck" << std::endl;
		std::cout << std::setw(5) << "" << "in one of the archway\'s many cracks!" << std::endl;
		std::cout << std::setw(5) << "" << "This is a rare chance!!" << std::endl << std::endl;

		std::cout <<  "Do you take it (Y/N)?" << std::endl;
		result = g_Game.KeyLoop(yesno);
	
		switch(result)
		{
		case 'y':
		case 'Y':
			std::cout << std::setw(5) << "" << "No one will notice if you take it." << std::endl;
			std::cout << std::setw(5) << "" << "You reach up and take the rune." << std::endl;
			std::cout << std::setw(5) << "" << "A voice from behind you gets your" << std::endl;
			std::cout << std::setw(5) << "" << "attention. You turn around to find" << std::endl;
			std::cout << std::setw(5) << "" << "a well suited goblin heading your" << std::endl;
			std::cout << std::setw(5) << "" << "way, and from the look of it he " << std::endl;
			std::cout << std::setw(5) << "" << "doesn't want to talk." << std::endl;
			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)GOBLIN_ELITE];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			if(plyr->GetInventory()->FindID(HOLY_RUNE))
			{
				plyr->GetInventory()->FindID(HOLY_RUNE)->GetItem()->iquantity++;
			}
			else
			{
				wItem = g_Items[(int)HOLY_RUNE];
				wItem.iquantity = 1;

				plyr->GetInventory()->End()->SetNext(&wItem);
			}

			std::cout << std::setw(5) << "" << "Barely surviving that battle you" << std::endl;
			std::cout << std::setw(5) << "" << "smile with self satisfaction." << std::endl;
			std::cout << std::setw(5) << "" << "You pocket the rune and head under" << std::endl;
			std::cout << std::setw(5) << "" << "the archway ready for anything." << std::endl << std::endl;
			flags |= (int)GOT_ITEM;
			break;
		case 'n':
		case 'N':
			std::cout << std::setw(5) << "" << "You decide that it is too easy." << std::endl;
			std::cout << std::setw(5) << "" << "It\'s probably a trick. So you" << std::endl;
			std::cout << std::setw(5) << "" << "leave it be and head on your way." << std::endl;
			break;
		}
		
		std::cout << std::setw(5) << "" << "Until you find two trolls ready" << std::endl;
		std::cout << std::setw(5) << "" << "to make your acquaintance." << std::endl << std::endl;
		bi.num_monsters = 2;
		bi.monsters = new ENTITY[bi.num_monsters];
		bi.monsters[0] = g_Creatures[(int)GOBLIN];
		bi.monsters[1] = g_Creatures[(int)GOBLIN];
		resultf = g_Game.Battle(plyr,&bi);
		delete [] bi.monsters;

		if(resultf != COMPLETE)
			return resultf;

		if(flags & (int)GOT_ITEM)
		{
			std::cout << std::setw(5) << "" << "Ever since you took the rune it" << std::endl;
			std::cout << std::setw(5) << "" << "seems everything wants to kill you." << std::endl;
		}
	}
	else
	{
		std::cout << std::setw(5) << "" << "You pass under the archway only to" << std::endl;
		std::cout << std::setw(5) << "" << "find a troll sleepingly soundly " << std::endl;
		std::cout << std::setw(5) << "" << "against the wall. Maybe you can try" << std::endl;
		std::cout << std::setw(5) << "" << "your luck and sneak past it? " << std::endl << std::endl;

		std::cout << "(S)neak or (W)ake it?" << std::endl;
		keys = new unsigned char[5];
		keys[0] = 's';
		keys[1] = 'S';
		keys[2] = 'w';
		keys[3] = 'W';
		keys[4] = 0;
		result = g_Game.KeyLoop(keys);
		delete [] keys;

		switch(result)
		{
		case 'W':
		case 'w':
			std::cout << std::setw(5) << "" << "You have no idea where the thought" << std::endl;
			std::cout << std::setw(5) << "" << "came from, but you decided to wake" << std::endl;
			std::cout << std::setw(5) << "" << "the troll out of it\'s sleep. " << std::endl;
			std::cout << std::setw(5) << "" << "Needless to say the troll wasn\'t happy." << std::endl;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)TROLL];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;
			
			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "In the near future let\'s hope you" << std::endl;
			std::cout << std::setw(5) << "" << "don\'t get anymore ideas like that." << std::endl;
			std::cout << std::setw(5) << "" << "Sighing you continue down the hallway." << std::endl;
     		break;
		case 's':
		case 'S':
			if(plyr->GetDexterity() > 5)
			{
				std::cout << std::setw(5) << "" << "You slowly tiptoe your way across" << std::endl;
				std::cout << std::setw(5) << "" << "the hallway carefull not to wake " << std::endl;
				std::cout << std::setw(5) << "" << "the slumbering troll. " << std::endl;
				std::cout << std::setw(5) << "" << "You almost trip over the sleeping" << std::endl;
				std::cout << std::setw(5) << "" << "hulk\'s huge legs. But you recover" << std::endl;
				std::cout << std::setw(5) << "" << "from the fall two inches from the" << std::endl;
				std::cout << std::setw(5) << "" << "troll\'s chest. With a sigh you step" << std::endl;
				std::cout << std::setw(5) << "" << "over the troll and continue down the" << std::endl;
				std::cout << std::setw(5) << "" << "hallway with slow steps." << std::endl << std::endl;
			}
			else
			{
				std::cout << std::setw(5) << "" << "Being the butter fingers you are" << std::endl;
				std::cout << std::setw(5) << "" << "you let the hilt of your " << plyr->GetCurrentWeapon().szname << std::endl;
				std::cout << std::setw(5) << "" << "fall and land on the troll\'s head" << std::endl;
				std::cout << std::setw(5) << "" << "giving it a very rude awakening." << std::endl;

				weapon = plyr->GetCurrentWeapon();
				plyr->SetCurrentWeapon(g_Items[HAND]);
				bi.num_monsters = 1;
				bi.monsters = new ENTITY[bi.num_monsters];
				bi.monsters[0] = g_Creatures[(int)TROLL];
				resultf = g_Game.Battle(plyr,&bi);
				delete [] bi.monsters;
				plyr->SetCurrentWeapon(weapon);

				if(resultf != COMPLETE)
					return resultf;

				std::cout << std::setw(5) << "" << "Picking up your " << plyr->GetCurrentWeapon().szname << " from the ground" << std::endl;
				std::cout << std::setw(5) << "" << "you remind yourself to keep a better grip" << std::endl;
				std::cout << std::setw(5) << "" << "on it next time." << std::endl;
			}
			break;
		}
	}

	std::cout << std::setw(5) << "" << "Further down the corridor you happen to" << std::endl;
	std::cout << std::setw(5) << "" << "glance out of the corner of your eye and" << std::endl;
	std::cout << std::setw(5) << "" << "happen to notice a wooden door bordered" << std::endl;
	std::cout << std::setw(5) << "" << "with steel plates, and an intricate " << std::endl;
	std::cout << std::setw(5) << "" << "design of a book engraved on the front." << std::endl;
	std::cout << std::setw(5) << "" << "It looks interesting and helpfull." << std::endl;

	std::cout << "(G)o in or (C)ontinue?" << std::endl;
	keys = new unsigned char[5];
	keys[0] = 'c';
	keys[1] = 'C';
	keys[2] = 'g';
	keys[3] = 'G';
	keys[4] = 0;
	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
	case 'G':
	case 'g':
		std::cout << std::setw(5) << "" << "You push on the jagged surface of the" << std::endl;
		std::cout << std::setw(5) << "" << "door, and amazingly is opens quite " << std::endl;
		std::cout << std::setw(5) << "" << "smoothly. You stare in amazement at the room." << std::endl;
		std::cout << std::setw(5) << "" << "There is a bed, and a dresser and in the upper" << std::endl;
		std::cout << std::setw(5) << "" << "right corner of the room there is a skeleton," << std::endl;
		std::cout << std::setw(5) << "" << "a potion, and a book covered with a nice" << std::endl;
		std::cout << std::setw(5) << "" << "amount of dust, probably enough to choke" << std::endl;
		std::cout << std::setw(5) << "" << "a elephant or two. The bed looks tempting." << std::endl << std::endl;

		std::cout << "(R)est, take (P)otion, or check the" << std::endl;

		if(flags & OTHER_1)
		{
			if(flags & OTHER_2)
			{
				std::cout << "(S)keleton?" << std::endl;
			}
			else
			{
				std::cout << "(D)resser, or (S)keleton?" << std::endl;
			}
		}
		else
		{
			if(flags & OTHER_2)
			{
				std::cout << "(B)ook, or (S)keleton?" << std::endl;
			}
			else
			{
				std::cout << "(D)resser, (B)ook, or (S)keleton?" << std::endl;
			}
		}

		keys = new unsigned char[11];
		keys[0] = 'r';
		keys[1] = 'R';
		keys[2] = 'p';
		keys[3] = 'P';

		if(flags & OTHER_2)
		{
			if(flags & OTHER_1)
			{
				keys[4] = 's';
				keys[5] = 'S';
				keys[6] = 0;
			}
			else
			{
				keys[4] = 'B';
				keys[5] = 'b';
				keys[6] = 's';
				keys[7] = 'S';
				keys[8] = 0;
			}
		}
		else
		{
			keys[4] = 'd';
			keys[5] = 'D';

			if(flags & OTHER_1)
			{
				keys[6] = 's';
				keys[7] = 'S';
				keys[8] = 0;
			}
			else
			{
				keys[6] = 'B';
				keys[7] = 'b';
				keys[8] = 's';
				keys[9] = 'S';
				keys[10] = 0;
			}
		}

		result = g_Game.KeyLoop(keys);
		delete [] keys;

		switch(result)
		{
		case 'r':
		case 'R':
			std::cout << std::setw(5) << "" << "" << std::endl;
			std::cout << std::setw(5) << "" << "All adventurers need a little rest." << std::endl;
			std::cout << std::setw(5) << "" << "So you lay down and fall asleep. When" << std::endl;
			std::cout << std::setw(5) << "" << "you awake you feel relaxed and renewed." << std::endl;
			std::cout << std::setw(5) << "" << "In fact if it wasn\'t for the skeleton" << std::endl;
			std::cout << std::setw(5) << "" << "holding a knife to your throat it might" << std::endl;
			std::cout << std::setw(5) << "" << "have been the best rest you\'ve had in" << std::endl;
			std::cout << std::setw(5) << "" << "weeks!" << std::endl;

			plyr->SetHP(plyr->GetMaxHP());
			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)SKELETON];

			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "You decide to get the hell out of the"  << std::endl;
			std::cout << std::setw(5) << "" << "room before the skeleton decides to"  << std::endl;
			std::cout << std::setw(5) << "" << "pull itself back together."  << std::endl;
			std::cout << std::setw(5) << "" << "So once again you find yourself walking"  << std::endl;
			std::cout << std::setw(5) << "" << "in the hallway to an unknown destination."  << std::endl << std::endl;
			break;
		case 'p':
		case 'P':
			std::cout << std::setw(5) << "" << "The potion looks awfully tempting."   << std::endl;
			std::cout << std::setw(5) << "" << "You snatch the potion from the table,"   << std::endl;
			std::cout << std::setw(5) << "" << "however the skeleton doesn\'t like you taking"  << std::endl;
			std::cout << std::setw(5) << "" << "things that aren't yours."  << std::endl;

			plyr->GetInventory()->FindItem("POTION")->GetItem()->iquantity++;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)SKELETON];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "You decide to get the hell out of the"  << std::endl;
			std::cout << std::setw(5) << "" << "room before the skeleton decides to"  << std::endl;
			std::cout << std::setw(5) << "" << "pull itself back together."  << std::endl;
			std::cout << std::setw(5) << "" << "So once again you find yourself walking"  << std::endl;
			std::cout << std::setw(5) << "" << "in the hallway to an unknown destination."  << std::endl << std::endl;
			break;
		case 'd':
		case 'D':
			// add item instead of gold
			std::cout << std::setw(5) << "" << "You check the dresser and find a Frosty Blade." << std::endl;
			std::cout << std::setw(5) << "" << "It wasn\'t until the point"  << std::endl;
			std::cout << std::setw(5) << "" << "of a knife stabbed you in the back did"  << std::endl;
			std::cout << std::setw(5) << "" << "you realize the skeleton wasn\'t dead."  << std::endl;

			flags |= (int)OTHER_2;
			memset(&wItem,0,sizeof(wItem));
			wItem = g_Items[(int)SWORD];
			wItem.istrength = 321;
			wItem.iquantity = 1;
			wItem.ilevel = 2;
			wItem.srunes.level[(int)ICE] = 4;
			strcpy(wItem.szname,"Frosty Blade");

			if(plyr->GetInventory()->FindItem(wItem.szname))
			{
				plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
			}
			else
			{
				plyr->GetInventory()->End()->SetNext(&wItem);
			}

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)SKELETON];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "You decide to get the hell out of the"  << std::endl;
			std::cout << std::setw(5) << "" << "room before the skeleton decides to"  << std::endl;
			std::cout << std::setw(5) << "" << "pull itself back together."  << std::endl;
			std::cout << std::setw(5) << "" << "So once again you find yourself walking"  << std::endl;
			std::cout << std::setw(5) << "" << "in the hallway to an unknown destination."  << std::endl << std::endl;
			break;

		case 'b':
		case 'B':
			std::cout << std::setw(5) << "" << "The book looks more tempting than any"  << std::endl;
			std::cout << std::setw(5) << "" << "other of the rooms accomadations."  << std::endl;
			std::cout << std::setw(5) << "" << "You carefully open the book and recognize"  << std::endl;
			std::cout << std::setw(5) << "" << "the ancient writings as spells. Unfortunately,"  << std::endl;
			std::cout << std::setw(5) << "" << "if you would have listened to your grandfathers"  << std::endl;
			std::cout << std::setw(5) << "" << "teachings you would have been able to cast"  << std::endl;
			std::cout << std::setw(5) << "" << "them. You decifered ancient knowledge to"  << std::endl;
			std::cout << std::setw(5) << "" << "transfer strength into dexterity."  << std::endl;
			std::cout << std::setw(5) << "" << "You chant the spell and fell a rush of"  << std::endl;
			std::cout << std::setw(5) << "" << "speed and agility flow through your body."  << std::endl;
			plyr->AddDexterity((int)ceil(1.5 * (rand()%2) + 1));
			plyr->AddStrength(2 * (rand()%2) + 1);
			
			std::cout << std::setw(5) << "" << "Looks like the skeleton wants you"  << std::endl;
			std::cout << std::setw(5) << "" << "to try it out on him first."  << std::endl;
			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)SKELETON];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "You decide to get the hell out of the"  << std::endl;
			std::cout << std::setw(5) << "" << "room before the skeleton decides to"  << std::endl;
			std::cout << std::setw(5) << "" << "pull itself back together."  << std::endl;
			std::cout << std::setw(5) << "" << "So once again you find yourself walking"  << std::endl;
			std::cout << std::setw(5) << "" << "in the hallway to an unknown destination."  << std::endl << std::endl;
			flags |= (int)OTHER_1;
			break;
		case 's':
		case 'S':
			std::cout << std::setw(5) << "" << "You decide to look at the poor fellow"  << std::endl;
			std::cout << std::setw(5) << "" << "that used to be living, and if it wasn\'t"  << std::endl;
			std::cout << std::setw(5) << "" << "for him getting up and attacking you, you"  << std::endl;
			std::cout << std::setw(5) << "" << "just might have felt sorry for him."  << std::endl;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)SKELETON];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "You decide to get the hell out of the"  << std::endl;
			std::cout << std::setw(5) << "" << "room before the skeleton decides to"  << std::endl;
			std::cout << std::setw(5) << "" << "pull itself back together."  << std::endl;
			std::cout << std::setw(5) << "" << "So once again you find yourself walking"  << std::endl;
			std::cout << std::setw(5) << "" << "in the hallway to an unknown destination."  << std::endl << std::endl;
			break;
		}
	}

	system("PAUSE");
	std::cout << std::endl;
    system("CLS");

	std::cout << std::setw(5) << "" << "You come to a small open area with a covered"   << std::endl;
	std::cout << std::setw(5) << "" << "coffin on several steps. Oddly enough the room"  << std::endl;
	std::cout << std::setw(5) << "" << "is covered with marble and highly expensive"  << std::endl;
	std::cout << std::setw(5) << "" << "objects. Though the coffin looks more interesting."  << std::endl;
	std::cout << std::setw(5) << "" << "You move slowly toward the coffin aware of your"  << std::endl;
	std::cout << std::setw(5) << "" << "surroundings."  << std::endl;

	if(plyr->GetStrength() <= 20)
	{
		std::cout << std::setw(5) << "" << "You push but it won\'t budge looks like"  << std::endl;
		std::cout << std::setw(5) << "" << "you ended up at a dead end after all."  << std::endl;
		std::cout << std::setw(5) << "" << "You take all the gold you can carry,"  << std::endl;
		std::cout << std::setw(5) << "" << "however a group of angry trolls block the"  << std::endl;
		std::cout << std::setw(5) << "" << "entrance. One signals for a challenge, so you accept."   << std::endl;

		bi.num_monsters = 3;
		bi.monsters = new ENTITY[bi.num_monsters];
		bi.monsters[0] = g_Creatures[(int)TROLL];
		bi.monsters[1] = g_Creatures[(int)TROLL];
		bi.monsters[2] = g_Creatures[(int)TROLL];
		resultf = g_Game.Battle(plyr,&bi);
		delete [] bi.monsters;

		if(resultf != COMPLETE)
			return resultf;

		std::cout << std::setw(5) << "" << "Although disappointed because of the lack of "  << std::endl;
	    std::cout << std::setw(5) << "" << "treasure you proudly walk out of the temple"  << std::endl;
		std::cout << std::setw(5) << "" << "and await a new quest to embark on."  << std::endl;

		plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity += 30;
		plyr->SetHP(plyr->GetMaxHP());
		
		flags |= LVL_COMPLETE;
		g_Events[(int)TEMPLE].uiValue = (LVLFLAGS)flags;

		system("PAUSE");
		std::cout << std::endl;
	    system("CLS");
		return COMPLETE;
	}

	std::cout << std::setw(5) << "" << "You push with a giant\'s power, and the"  << std::endl;
	std::cout << std::setw(5) << "" << "huge lid topples off the coffin and lands"  << std::endl;
	std::cout << std::setw(5) << "" << "with a gigantic smash. It landed so hard"  << std::endl;
	std::cout << std::setw(5) << "" << "the marble floor crumpled and broke."  << std::endl;
	std::cout << std::setw(5) << "" << "Under the lid of the coffin was a hidden"  << std::endl;
	std::cout << std::setw(5) << "" << "passageway, darkness now lies before you."  << std::endl;
	std::cout << std::setw(5) << "" << "You hold your breath and leap into the dark"  << std::endl;
	std::cout << std::setw(5) << "" << "tunnel below. You land with a hard thud!"  << std::endl;
	x = (rand()%4)+1;
	plyr->AddHP(-x);

	std::cout << std::setw(11) << "" << x << " damage was dealt to you!"  << std::endl;

	if(plyr->GetHP() <= 0)
	{
		std::cout << "Unfortunately that fall happened to kill you...";
		result = g_Game.KeyLoop(yesno);

		switch(result)
		{
		case 'y':
		case 'Y'://21: // 'Y'
			return CONTINUE;
			break;
		case 'N'://49: // 'N'
		case 'n':
			return QUIT;
			break;
		}
	}

	std::cout << std::setw(5) << "" << "After the fall you head down the corridor."  << std::endl;
	std::cout << std::setw(5) << "" << "You finnaly make it to a large steel door."  << std::endl;
	std::cout << std::setw(5) << "" << "You push open the steel door with a loud"   << std::endl;
	std::cout << std::setw(5) << "" << "echo that carries down the corridor."  << std::endl   << std::endl;

	system("PAUSE");
	std::cout << std::endl;

	std::cout << std::setw(5) << "" << "Beyond the archway you see countless treasures."  << std::endl;
	std::cout << std::setw(5) << "" << "Ranging from gold swords to runes of all kinds."  << std::endl;
	std::cout << std::setw(5) << "" << "Then your eye catches a glimpse of a very pissed"  << std::endl;
	std::cout << std::setw(5) << "" << "ogre. You don\'t think he likes you very much."  << std::endl;

	bi.num_monsters = 1;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)OGRE];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(5) << "" << "As you lay over the gigantic monstrosity you"  << std::endl;
	std::cout << std::setw(5) << "" << "remember the gold and jewels."  << std::endl;
	std::cout << std::setw(5) << "" << "Thump!"  << std::endl << std::endl;

	system("PAUSE");
	std::cout << std::endl;

	std::cout << std::setw(5) << "" << "You land on the gold and jewels, when you wake"  << std::endl;
	std::cout << std::setw(5) << "" << "your head throbes. Worst yet you glance and find"  << std::endl;
	std::cout << std::setw(5) << "" << "that your beloved treasure is gone. All that is"  << std::endl;

	if(plyr->GetInventory()->FindID(KNIFE))
	{
		std::cout << std::setw(5) << "" << "left is a few gold pieces, and a knife you already have!"  << std::endl;
		std::cout << std::setw(5) << "" << "Fooey! Dissapointed you exit the chamber, and await"  << std::endl;
		std::cout << std::setw(5) << "" << "your next challenge."  << std::endl;
	}
	else
	{
		ITEM *knife = new ITEM;
		knife[0] = g_Items[KNIFE];
		plyr->GetInventory()->End()->SetNext(knife);
		std::cout << std::setw(5) << "" << "left is a few gold pieces, and a knife! Fooey!"  << std::endl;
		std::cout << std::setw(5) << "" << "Dissapointed you exit the chamber, and await your"  << std::endl;
		std::cout << std::setw(5) << "" << "next challenge."  << std::endl;
	}

	plyr->SetHP(plyr->GetMaxHP());
	flags |= DID_SPECIAL;
	flags |= LVL_COMPLETE;
	g_Events[(int)TEMPLE].uiValue = (LVLFLAGS)flags;
	plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity += (rand()%90)+10;
	system("PAUSE");
	std::cout << std::endl;
    system("CLS");
	
	return COMPLETE;
}

ENDFLAG Mountain(PLAYER* plyr)
{
	srand((unsigned int)time(NULL));
	BATTLEINFO bi;
	unsigned int flags = g_Events[(int)MOUNTAIN].uiValue;
	ENDFLAG resultf;
	unsigned int result;
	unsigned char *keys;
	unsigned char yesno[] = {'Y','N','n','y',0};
	system("cls");
	ITEM wItem;

	std::cout << std::setw(15) << "" << "Welcome to Mountain of Dread!" << std::endl;
	std::cout << "As you stand in the mountain\'s gigantic" << std::endl;
	std::cout << "shadow, you happen to recall the reason" << std::endl;
	std::cout << "you even became a adventurer. Your dad," << std::endl;
	std::cout << "his dad, and your great grandfather had" << std::endl;
	std::cout << "been the greatest adventurers, and the" << std::endl;
	std::cout << "only powers greater enough to even challenge" << std::endl;
	std::cout << "each of them were there own. Your dad was" << std::endl;
	std::cout << "the greatest adventurer ever known, once" << std::endl;
	std::cout << "he took out a dragon with one swipe of his" << std::endl;
	std::cout << "mighty Zheiwhander. The zheiwhander your dad" << std::endl;
	std::cout << "weilded was gigantic, powerful, and very" << std::endl;
	std::cout << "heavy. You remember once sparing your dad," << std::endl;
	std::cout << "needless to say that was the first and last" << std::endl;
	std::cout << "time you ever took up arms against your dad." << std::endl;
	std::cout << std::endl;
	system("PAUSE");
	std::cout << "You remember your dad taking up a exhibition" << std::endl;
	std::cout << "at the same mountain your standing in front of" << std::endl;
	std::cout << "now. He told you stories of mountain dwarfs," << std::endl;
	std::cout << "a rare breed of dwarf specializing in blacksmithy." << std::endl;
	std::cout << "The dwarves are harmless unless provoked to fight." << std::endl;
	std::cout << "And when provoked the dwarves used magically " << std::endl;
	std::cout << "encumbered swords, that were brilliantly made " << std::endl;
	std::cout << "with intricate designs of magical powers. You" << std::endl;
	std::cout << "decide if you find any dwarves maybe they can " << std::endl;
	std::cout << "fasten you a new sword for a price of course." << std::endl;
	std::cout << "Well you better hurry up before dark, because" << std::endl;
	std::cout << "after dark most people don\'t make it back." << std::endl;
	std::cout << std::endl;
	system("pause");

	if(!(flags & (int)DID_SPECIAL))
	{
		std::cout << "You face two paths either (C)limb or (E)nter the cave?" << std::endl;

		keys = new unsigned char[5];
		keys[0] = 'c';
		keys[1] = 'C';
		keys[2] = 'e';
		keys[3] = 'E';
		keys[4] = 0;
		result = g_Game.KeyLoop(keys);
		delete [] keys;

		switch(result)
		{
		case 'e':
		case 'E':
			std::cout << std::setw(11) << "" << "You decide it would be easier to enter the" << std::endl;
			std::cout << std::setw(11) << "" << "cave rather than climbing to the mountain" << std::endl;
			std::cout << std::setw(11) << "" << "peak. The inside looks as though as if it" << std::endl;
			std::cout << std::setw(11) << "" << "were carved out almost as though it were" << std::endl;
			std::cout << std::setw(11) << "" << "some sort of mine though no minerals are" << std::endl;
			std::cout << std::setw(11) << "" << "visible, and the walls are amazingly smooth." << std::endl;
			std::cout << std::setw(11) << "" << "The further you walk down the tunnel the " << std::endl;
			std::cout << std::setw(11) << "" << "darker it gets. Your eyes just now seem to" << std::endl;
			std::cout << std::setw(11) << "" << "be adjusting to the darkness of the cave." << std::endl;
			std::cout << std::setw(11) << "" << "You can make out a dark figure or maybe two" << std::endl;
			std::cout << std::setw(11) << "" << "dark figures? The closer you get the more" << std::endl;
			std::cout << std::setw(11) << "" << "abstract they get. Suddenly one looks as" << std::endl;
			std::cout << std::setw(11) << "" << "though it moved! You carefully watch as the" << std::endl;
			std::cout << std::setw(11) << "" << "figure slowly moves toward you then it breaks" << std::endl;
			std::cout << std::setw(11) << "" << "into a jog. It\'s feet make loud noises when" << std::endl;
			std::cout << std::setw(11) << "" << "they hit the ground. What is THIS!!!" << std::endl;
			std::cout << std::endl;
			system("PAUSE");

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)GOLEM];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "You managed to slay the stone figure." << std::endl; 
			std::cout << std::setw(5) << "" << "Being very tired you sit to rest but" << std::endl; 
			std::cout << std::setw(5) << "" << "unfortunately the second golem didn\'t." << std::endl; 

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)GOLEM];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(5) << "" << "Just once can\'t you rest without any" << std::endl;
			std::cout << std::setw(5) << "" << "distractions, or at least weaker enemies." << std::endl;
			std::cout << std::setw(5) << "" << "You\'d better get to walking before they" << std::endl;
			std::cout << std::setw(5) << "" << "decide to get back up, and spoil your rest." << std::endl;
			std::cout << std::setw(5) << "" << "Along the walls you finally come across some" << std::endl;
			std::cout << std::setw(5) << "" << "torches. It would be nice to have some light." << std::endl << std::endl;

			std::cout << "(G)rab the torch or (L)eave it?" << std::endl;
			keys = new unsigned char[5];
			keys[0] = 'g';
			keys[1] = 'G';
			keys[2] = 'l';
			keys[3] = 'L';
			keys[4] = 0;
			result = g_Game.KeyLoop(keys);
			delete [] keys;
		
			switch(result) 
			{
			case 'G':
			case 'g':
				std::cout << std::setw(11) << "" << "You decide to take one besides who\'s" << std::endl;
				std::cout << std::setw(11) << "" << "going to miss one lousey little torch." << std::endl;
				std::cout << std::setw(11) << "" << "Apparently one ogre did care about you" << std::endl;
				std::cout << std::setw(11) << "" << "borrowing his torch!" << std::endl;
				std::cout << std::setw(11) << "" << "\'Give back me special glowing stick!\'" << std::endl << std::endl;

				bi.num_monsters = 1;
				bi.monsters = new ENTITY[bi.num_monsters];
				bi.monsters[0] = g_Creatures[(int)OGRE];
				resultf = g_Game.Battle(plyr,&bi);
				delete [] bi.monsters;

				if(resultf != COMPLETE)
					return resultf;

				std::cout << std::setw(11) << "" << "After that extremely weird experience" << std::endl;
				std::cout << std::setw(11) << "" << "you look at the torch, and ask yourself" << std::endl;
				std::cout << std::setw(11) << "" << "was it worth it?" << std::endl << std::endl;
				system("PAUSE");
				flags |= ((int)GOT_ITEM);
				break;

			case 'L':
			case 'l':
				std::cout << std::setw(11) << "" << "You\'d better leave them alone, and" << std::endl;
				std::cout << std::setw(11) << "" << "you continue on without the torches." << std::endl << std::endl;
				system("PAUSE");
				std::cout << std::setw(11) << "" << "You come across a slim crossing ledge, unfortunately," << std::endl;
				std::cout << std::setw(11) << "" << "you can\'t cross because you can\'t see where" << std::endl;
				std::cout << std::setw(11) << "" << "you\'re going. There is nothing left but to go" << std::endl;
				std::cout << std::setw(11) << "" << "back the way you came." << std::endl;
				system("PAUSE");

				std::cout << std::setw(11) << "" << "The vast mountain towers above you." << std::endl << std::endl;
				std::cout << "(C)limb or (L)eave the Mountain?" << std::endl;

				keys = new unsigned char[5];
				keys[0] = 'c';
				keys[1] = 'C';
				keys[2] = 'l';
				keys[3] = 'L';
				keys[4] = 0;
				result = g_Game.KeyLoop(keys);
				delete [] keys;

				if(tolower(result) == 'l')
					return CONTINUE;

				break;
			} // end torch switch


		if(flags & (int)GOT_ITEM)
		{
			flags &= ((int)GOT_ITEM^0xFFFF);
			std::cout << std::setw(11) << "" << "You come across a slim crossing ledge, and" << std::endl;
			std::cout << std::setw(5) << "" << "thanks to the torch you can now see where" << std::endl;
			std::cout << std::setw(5) << "" << "you\'re going in the extremely dark tunnel." << std::endl;
			std::cout << std::setw(5) << "" << "Unfortunately the fun hasn't stopped yet." << std::endl << std::endl;
			system("pause");
			std::cout << std::setw(11) << "" << "After crossing the ledge you continue your" << std::endl;
			std::cout << std::setw(11) << "" << "way down the tunnel with your torch still lit." << std::endl;
			std::cout << std::setw(11) << "" << "Further down the tunnel you get a whiff of a" << std::endl;
			std::cout << std::setw(11) << "" << "truly strong scent. Sort of familiar to the" << std::endl;
			std::cout << std::setw(11) << "" << "smell of fresh dew. As you make your way the" << std::endl;
			std::cout << std::setw(11) << "" << "smell gets even stronger, and almost puts you" << std::endl;
			std::cout << std::setw(11) << "" << "in a very relaxing trance. You quickly shake" << std::endl;
			std::cout << std::setw(11) << "" << "off the trance and continue. You come to a " << std::endl;
			std::cout << std::setw(11) << "" << "giant lake in one of the cave\'s chambers." << std::endl;
			std::cout << std::setw(11) << "" << "The water\'s stillness looks very inviting," << std::endl;
			std::cout << std::setw(11) << "" << "and restfull. It wouldn\'t hurt just to take" << std::endl;
			std::cout << std::setw(11) << "" << "a quick dip in the relaxing water. But then" << std::endl;
			std::cout << std::setw(11) << "" << "again you should hurry to the summit." << std::endl << std::endl;
			std::cout << "(T)ake a dip or (C)ontinue to the summit?" << std::endl;

			keys = new unsigned char[5];
			keys[0] = 't';
			keys[1] = 'T';
			keys[2] = 'c';
			keys[3] = 'C';
			keys[4] = 0;
			result = g_Game.KeyLoop(keys);
			delete [] keys;		

			switch(result)
			{
			case 't':
			case 'T':
				std::cout << std::setw(11) << "" << "You deserve to take a nice relaxing dip" << std::endl;
				std::cout << std::setw(11) << "" << "in the cool clear water. So you do." << std::endl;
				std::cout << std::setw(11) << "" << "But the giant had other plans for you" << std::endl;
				std::cout << std::setw(11) << "" << "since you busted into his house, and" << std::endl;
				std::cout << std::setw(11) << "" << "started swimming in his bathwater." << std::endl;
				plyr->SetHP(plyr->GetMaxHP());

				bi.num_monsters = 1;
				bi.monsters = new ENTITY[bi.num_monsters];
				bi.monsters[0] = g_Creatures[(int)GIANT];
				resultf = g_Game.Battle(plyr,&bi);
				delete [] bi.monsters;

				if(resultf != COMPLETE)
					return resultf;

				std::cout << std::setw(11) <<  "" << "As you put your clothes back on you" << std::endl;
				std::cout << std::setw(11) <<  "" << "wonder where to go now?" << std::endl;

	///***********CHECK EVENT NODES FOR THIS EVENT***************///
				if(!(flags & (int)FOUND_SPECIAL))
				{
					std::cout << std::setw(11) << "" << "You spot a shadow scurrying in the corner." << std::endl << std::endl;
					std::cout << "(I)nvestigate or (D)isregard?" << std::endl;

					keys = new unsigned char[5];
					keys[0] = 'i';
					keys[1] = 'I';
					keys[2] = 'd';
					keys[3] = 'D';
					keys[4] = 0;
					result = g_Game.KeyLoop(keys);
					delete [] keys;

					switch(result)
					{
					case 'd':
					case 'D':
						std::cout << std::setw(11) << "" <<  "Whatever it was, it was probably unimportant." << std::endl;
						std::cout << std::setw(11) << "" <<  "Besides you have a quest to finish." << std::endl;
						break;
					case 'I':
					case 'i':
						std::cout << std::setw(11) << "" << "You decide to spot whats going on and find" << std::endl;
						std::cout << std::setw(11) << "" << "that a dwarven child is being attacked by" << std::endl;
						std::cout << std::setw(11) << "" << "a group of Ogres!" << std::endl << std::endl;

						std::cout << "(H)elp or (C)ontinue to the summit" << std::endl;
						keys = new unsigned char[5];
						keys[0] = 'c';
						keys[1] = 'C';
						keys[2] = 'h';
						keys[3] = 'H';
						keys[4] = 0;
						result = g_Game.KeyLoop(keys);
						delete [] keys;

						switch(result)
						{
							case 'c':
							case 'C':
								std::cout << std::setw(11) << "" << "You decide it\'s their own problem." << std::endl;
								break;

							case 'h':
							case 'H':
								std::cout << std::setw(5) << "" << "You\'ve got to help it!" << std::endl << std::endl;

								bi.num_monsters = 2;
								bi.monsters = new ENTITY[bi.num_monsters];
								bi.monsters[0] = g_Creatures[(int)OGRE];
								bi.monsters[1] = g_Creatures[(int)OGRE];
								resultf = g_Game.Battle(plyr,&bi);
								delete [] bi.monsters;

								if(resultf != COMPLETE)
									return resultf;

								std::cout << std::setw(11) << "" << "\'Thank you for helping me but you\'ve" << std::endl;
								std::cout << std::setw(13) << "" << "got to save my town. Hold on and I\'ll " << std::endl;
								std::cout << std::setw(13) << "" << "heal your battle wounds.\'" << std::endl;
								plyr->SetHP(plyr->GetMaxHP());						
								std::cout << std::setw(5) << "" << "\'There now follow me!\'" << std::endl;
								std::cout << std::setw(5) << "" << "As you run behind the child you\'re wondering" << std::endl;
								std::cout << std::setw(5) << "" << "how you got yourself in this mess!" << std::endl;
								std::cout << std::setw(5) << "" << "The child suddenly stops, and starts to back" << std::endl;
								std::cout << std::setw(5) << "" << "up, and you see why. Two golems and a leading" << std::endl;
								std::cout << std::setw(5) << "" << "ogre are slowly walking towards you!" << std::endl << std::endl;

								bi.num_monsters = 3;
								bi.monsters = new ENTITY[bi.num_monsters];
								bi.monsters[0] = g_Creatures[(int)GOLEM];
								bi.monsters[1] = g_Creatures[(int)GOLEM];
								bi.monsters[2] = g_Creatures[(int)OGRE];
								resultf = g_Game.Battle(plyr,&bi);
								delete [] bi.monsters;

								if(resultf != COMPLETE)
									return resultf;

								std::cout << std::setw(11) << "" << "\'I\'ll heal your wounds!\'" << std::endl;
								plyr->SetHP(plyr->GetMaxHP());						
									
								std::cout << std::setw(5) << "" << "\'Come on follow me!\'" << std::endl;
								std::cout << std::setw(5) << "" << "\'Hurry up!\'" << std::endl << std::endl;
								system("pause");
								
								std::cout << std::setw(11) << "" <<  "There are the Ogres and Golems everywhere!" << std::endl << std::endl;
								std::cout << "(F)ight them, (B)ribe them, or (R)un away from it all?" << std::endl;
								keys = new unsigned char[8];
								keys[0] = 'f';
								keys[1] = 'F';
								keys[2] = 'b';
								keys[3] = 'B';
								keys[4] = 'r';
								keys[5] = 'R';
								keys[6] = 0;
								result = g_Game.KeyLoop(keys);
								delete [] keys;

								switch(result)
								{
								case 'r':
								case 'R':
									std::cout << std::setw(11) << "" <<  "You run away as fast as you can to get" << std::endl;
									std::cout << std::setw(11) << "" <<  "away from the horrid scene. In the " << std::endl;
									std::cout << std::setw(11) << "" <<  "distance you can faintly hear the dwarven" << std::endl;
									std::cout << std::setw(11) << "" <<  "child yell for your help right before" << std::endl;
									std::cout << std::setw(11) << "" <<  "you hear a swing of a blade and the gurgle" << std::endl;
									std::cout << std::setw(11) << "" <<  "of fresh blood running down his throat." << std::endl;
									break;

								case 'B':
								case 'b':
									if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity < 800)
									{
										std::cout << std::setw(11) << "" << "\'Trying to bribe with invisible gold" << std::endl;
										std::cout << std::setw(11) << "" << "is not smart\' \'GET HIM!!!\'" << std::endl;
									}
									else
									{
										std::cout << std::setw(11) << "" << "Good you pay money!" << std::endl << std::endl;
										system("pause");
										plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -=800;
										std::cout << std::endl << "Now we FIGHT!!!" << std::endl << std::endl;

										bi.num_monsters = 1;
										bi.monsters = new ENTITY[bi.num_monsters];
										bi.monsters[0] = g_Creatures[(int)OGRE_LEADER];
										resultf = g_Game.Battle(plyr,&bi);
										delete [] bi.monsters;

										if(resultf != COMPLETE)
											return resultf;

										flags |= FOUND_SPECIAL;

										std::cout << std::setw(11) << "" << "The rest of the monsters flee from" << std::endl;
										std::cout << std::setw(11) << "" << "your grasp for fear of being killed!" << std::endl;
										std::cout << std::setw(11) << "" << "\'Thank you for saving my town!\'" << std::endl;
										std::cout << std::setw(11) << "" << "\'Please stay and look around before" << std::endl;
										std::cout << std::setw(12) << "" << "you go. Feel free to come back at" << std::endl;
										std::cout << std::setw(12) << "" << "any time you feel like it!\'" << std::endl;
										std::cout << std::setw(11) << "" << "You wave goodbye and thank them for" << std::endl;
										std::cout << std::setw(11) << "" << "their hospitality, and continue down" << std::endl;
										std::cout << std::setw(11) << "" << "the way you came. You come back to the" << std::endl;
										std::cout << std::setw(11) << "" << "giant underground lake where you fought" << std::endl;
										std::cout << std::setw(11) << "" << "the Giant, and continue down the tunnel" << std::endl;
										std::cout << std::setw(11) << "" << "you didn\'t venture into before." << std::endl << std::endl;
										break;
									}

								case 'f':
								case 'F':
									std::cout << std::setw(11) << "" << "\'You think you can take on clan!\'" << std::endl;
									std::cout << std::setw(11) << "" << "\'Bring it ON!!!\'" << std::endl << std::endl;

									bi.num_monsters = 4;
									bi.monsters = new ENTITY[bi.num_monsters];
									bi.monsters[0] = g_Creatures[(int)OGRE];
									bi.monsters[1] = g_Creatures[(int)OGRE];
									bi.monsters[2] = g_Creatures[(int)GOLEM];
									bi.monsters[3] = g_Creatures[(int)GOLEM];
									resultf = g_Game.Battle(plyr,&bi);
									delete [] bi.monsters;

									if(resultf != COMPLETE)
										return resultf;

									std::cout << std::setw(5) << "" << "I\'ll heal you now." << std::endl;
									plyr->SetHP(plyr->GetMaxHP());						

									bi.num_monsters = 4;
									bi.monsters = new ENTITY[bi.num_monsters];
									bi.monsters[0] = g_Creatures[(int)OGRE];
									bi.monsters[1] = g_Creatures[(int)OGRE];
									bi.monsters[2] = g_Creatures[(int)OGRE];
									bi.monsters[3] = g_Creatures[(int)GOLEM];
									resultf = g_Game.Battle(plyr,&bi);
									delete [] bi.monsters;

									if(resultf != COMPLETE)
										return resultf;

									std::cout << std::setw(5) << "" << "\'Be careful!\'. I\'ll heal you now." << std::endl << std::endl;
									plyr->SetHP(plyr->GetMaxHP());						
									std::cout << std::setw(11) << "\'Now you have to face ME!!!!\'" << std::endl << std::endl;
			
									bi.num_monsters = 3;
									bi.monsters = new ENTITY[bi.num_monsters];
									bi.monsters[0] = g_Creatures[(int)OGRE];
									bi.monsters[1] = g_Creatures[(int)OGRE_LEADER];
									bi.monsters[2] = g_Creatures[(int)OGRE];
									resultf = g_Game.Battle(plyr,&bi);
									delete [] bi.monsters;

									if(resultf != COMPLETE)
										return resultf;

									flags |= FOUND_SPECIAL;

									std::cout << std::setw(11) << "" << "The rest of the monsters flee from" << std::endl;
									std::cout << std::setw(11) << "" << "your grasp for fear of being killed!" << std::endl;
									std::cout << std::setw(11) << "" << "Thank you for saving my town!" << std::endl;
									std::cout << std::setw(11) << "" << "Please stay and look around before" << std::endl;
									std::cout << std::setw(11) << "" << "you go. Feel free to come back at" << std::endl;
									std::cout << std::setw(11) << "" << "any time you feel like it!" << std::endl;
									std::cout << std::setw(11) << "" << "You wave goodbye and thank them for" << std::endl;
									std::cout << std::setw(11) << "" << "their hospitality, and continue down" << std::endl;
									std::cout << std::setw(11) << "" << "the way you came. You come back to the" << std::endl;
									std::cout << std::setw(11) << "" << "giant underground lake where you fought" << std::endl;
									std::cout << std::setw(11) << "" << "the Giant, and continue down the tunnel" << std::endl;
									std::cout << std::setw(11) << "" << "you didn\'t venture into before." << std::endl << std::endl;
									std::cout << std::setw(11) << "" << "Darkness is becoming very homey lately." << std::endl;
									break;
								} // seige switch
								break;
							} // help switch
							break;
						}// child switch
					}// town found
					break;

				case 'c':
				case 'C':
					std::cout << std::setw(11) << "" << "You don\'t have enough time to take a" << std::endl;
					std::cout << std::setw(11) << "" << "swim so you continue on your journey." << std::endl;
					break;
				}// pond switch

				std::cout << std::setw(11) << "" <<  "Your torch is running awfully low. You spot" << std::endl;
				std::cout << std::setw(11) << "" <<  "a door marked with the letters s and r. S..R.." << std::endl;
				std::cout << std::setw(11) << "" <<  "You might be able to find another torch." << std::endl;

				std::cout << std::endl << std::endl << "Will you (G)o in or (L)eave?" << std::endl;
				keys = new unsigned char[5];
				keys[0] = 'g';
				keys[1] = 'G';
				keys[2] = 'l';
				keys[3] = 'L';
				keys[4] = 0;
				result = g_Game.KeyLoop(keys);
				delete [] keys;

				switch(result)
				{
				case 'G':
				case 'g':
					std::cout << std::setw(5) << "" << "You decide you might find another torch" << std::endl;
					std::cout << std::setw(5) << "" << "or some other type of supplies. You put" << std::endl;
					std::cout << std::setw(5) << "" << "your ear to the door. Surprisingly sounds" << std::endl;
					std::cout << std::setw(5) << "" << "like nothings in the room. You reach for" << std::endl;
					std::cout << std::setw(5) << "" << "the knob, and as your turning it you realize" << std::endl;
					std::cout << std::setw(5) << "" << "what is a door doing in a mountain?" << std::endl;
					std::cout << std::setw(5) << "" << "You turn it and swing the door open to" << std::endl;
					std::cout << std::setw(5) << "" << "reveal a supplies room?" << std::endl;
					std::cout << std::setw(5) << "" << "There is a blade, a bag of gold, and a potion." << std::endl << std::endl;

					std::cout << "Take the (B)lade, bag of (G)old, (P)otion, or (L)eave?" << std::endl;
					keys = new unsigned char[9];
					keys[0] = 'g';
					keys[1] = 'G';
					keys[2] = 'b';
					keys[3] = 'B';
					keys[4] = 'p';
					keys[5] = 'P';
					keys[6] = 'l';
					keys[7] = 'L';
					keys[8] = 0;
					result = g_Game.KeyLoop(keys);
					delete [] keys;

					switch(result)
					{
					case 'b':
					case 'B':
						std::cout << std::setw(11) << "" <<  "A blade to an unfinished sword occupies a nearby" << std::endl;
						std::cout << std::setw(11) << "" <<  "shelf you decide nobody\'s gonna miss it. You" << std::endl;
						std::cout << std::setw(11) << "" <<  "quickly pull it from the shelf. Nothing happened.";
						std::cout << std::endl << std::endl;
						system("PAUSE");

						memset(&wItem,0,sizeof(wItem));
						wItem = g_Items[(int)SWORD];
						wItem.istrength = 321;
						wItem.iquantity = 1;
						wItem.ilevel = 4;
						strcpy(wItem.szname, "Smiting Blade");

						if(plyr->GetInventory()->FindItem(wItem.szname))
						{
							plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
						}
						else
						{
							plyr->GetInventory()->End()->SetNext(&wItem);
						}

						std::cout << std::setw(11) << "" <<  "It\'s completely quiet except for the sound of" << std::endl;
						std::cout << std::setw(11) << "" <<  "a very noisy alarm that went of a few seconds" << std::endl;
						std::cout << std::setw(11) << "" <<  "after you took the blade. You quickly hide behind" << std::endl;
						std::cout << std::setw(11) << "" <<  "a nearby barrel. You spot two Ogres talking in" << std::endl;
						std::cout << std::setw(11) << "" <<  "front of the doorway. \'Someone spot a weird" << std::endl;
						std::cout << std::setw(11) << "" <<  "man in the Grand Hall\'. \'COME ON, I hope king" << std::endl;
						std::cout << std::setw(11) << "" <<  "it all right\'. You guess it wasn\'t you." << std::endl << std::endl;

						std::cout << "Follow them (Y or N)?" << std::endl;
						result = g_Game.KeyLoop(yesno);

						switch (result)
						{
							case 'n':
							case 'N':
							std::cout << std::setw(11) << "" <<  "You decide not to follow them for fear" << std::endl;
							std::cout << std::setw(11) << "" <<  "of getting caught. You get out of the " << std::endl;
							std::cout << std::setw(11) << "" <<  "storage room in fear of tripping an alarm." << std::endl;
							std::cout << std::setw(11) << "" <<  "Back in the hallway you figure you\'d better" << std::endl;
							std::cout << std::setw(11) << "" <<  "follow back down the way you didn\'t come." << std::endl;
							break;

							case 'y':
							case 'Y':
								std::cout << std::setw(11) << "" <<  "You decide to follow the Ogres in hope of" << std::endl;
								std::cout << std::setw(11) << "" <<  "being lead to even better treasures. The" << std::endl;
								std::cout << std::setw(11) << "" <<  "Ogres take off down the hallway, and swiftly," << std::endl;
								std::cout << std::setw(11) << "" <<  "but carefully, you trail them to a metal doorway." << std::endl;
								std::cout << std::setw(11) << "" <<  "You wonder why they stopped, but then you see" << std::endl;
								std::cout << std::setw(11) << "" <<  "what they saw. One man standing alone against" << std::endl;
								std::cout << std::setw(11) << "" <<  "five Ogres. The man doesn\'t even have a chance." << std::endl;
								std::cout << std::setw(11) << "" <<  "Then the Ogres grunt loud and blood starts" << std::endl;
								std::cout << std::setw(11) << "" <<  "oozing from their invisible wounds. Then one by" << std::endl;
								std::cout << std::setw(11) << "" <<  "one their heads start tumbling off their bulky" << std::endl;
								std::cout << std::setw(11) << "" <<  "shoulders. The other Ogres you followed ran" << std::endl;
								std::cout << std::setw(11) << "" <<  "in the direction in which you followed them." << std::endl;
								std::cout << std::setw(11) << "" << "The man glares at you menacingly and retreats" << std::endl;
								std::cout << std::setw(11) << "" << "down the hall behind him." << std::endl;
								break;
						}
						break;
					case 'g':
					case 'G':
						std::cout << std::setw(11) << "" <<  "A bag of gold sitting on a barrel temps" << std::endl;
						std::cout << std::setw(11) << "" <<  "your eyes. You looked around and snatched" << std::endl;
						std::cout << std::setw(11) << "" <<  "the bag then quickly taking your leave into" << std::endl;
						std::cout << std::setw(11) << "" <<  "the hallway. Their were 100 gold coins inside!" << std::endl;
						plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity+=100;
					break;
					case 'p':
					case 'P':
						std::cout << std::setw(11) << "" <<  "Nobodys going to miss a potion. SNATCH!" << std::endl;
						std::cout << std::setw(11) << "" <<  "HE HE HE." << std::endl;

						if(plyr->GetInventory()->FindItem("Potion"))
						{
							plyr->GetInventory()->FindItem("Potion")->GetItem()->iquantity++;
						}
						else
						{
							ITEM *potion = new ITEM;
							potion[0] = g_Items[POTION];

							plyr->GetInventory()->End()->SetNext(potion);
						}
						break;

					case 'l':
					case 'L':
						std::cout << std::setw(11) << "" <<  "There\'s nothing in the room that you want." << std::endl;
						break;
					}
					break;

				case 'l':
				case 'L':
					std::cout << std::setw(11) << "" <<  "There\'s probably nothing in the room." << std::endl;
					break;
				}

				std::cout << std::setw(11) << "" <<  "You continue to walk and come to a small" << std::endl;
				std::cout << std::setw(11) << "" <<  "metal door. You could try to push it open." << std::endl;
				std::cout << std::setw(11) << "" <<  "So with all your strength you push and" << std::endl;
				std::cout << std::setw(11) << "" <<  "the door finally gives way and swings" << std::endl;
				std::cout << std::setw(11) << "" <<  "wide open. You look through the doorway" << std::endl;
				std::cout << std::setw(11) << "" <<  "it looks like a sort of throne room." << std::endl;
				std::cout << std::setw(11) << "" <<  "A red carpet lines the floor and meets" << std::endl;
				std::cout << std::setw(11) << "" <<  "the throne, marble pillars hold up the" << std::endl;
				std::cout << std::setw(11) << "" <<  "ceiling. You think you can see something" << std::endl;
				std::cout << std::setw(11) << "" <<  "on the throne. It looks like a human " << std::endl;
				std::cout << std::setw(11) << "" <<  "wearing a red silk robe suited for a king." << std::endl;
				std::cout << std::setw(11) << "" <<  "He would look alive if it wasn\'t for" << std::endl;
				std::cout << std::setw(11) << "" <<  "blood trickling down his neck and spattering" << std::endl;
				std::cout << std::setw(11) << "" <<  "his red robe, giving it a sparkly shine." << std::endl;
				std::cout << std::setw(11) << "" <<  "You see somebody come out of nowhere." << std::endl;
				std::cout << std::setw(11) << "" <<  "He\'s blurred, but you can make out that he\'s" << std::endl;
				std::cout << std::setw(11) << "" <<  "wearing chain mail, and weilding a long" << std::endl;
				std::cout << std::setw(11) << "" <<  "slim bladed sword resembling a Rapier." << std::endl << std::endl; 
				system("PAUSE");
				
				std::cout << std::setw(7) << "" << "\'If you think you can take me on. Have at YOU!!!\'" << std::endl;
				std::cout << std::setw(5) << "" << "That was all you needed to hear. You lift your" << std::endl;
				std::cout << std::setw(5) << "" << plyr->GetCurrentWeapon().szname << " and prepare yourself for battle." << std::endl;

				bi.num_monsters = 1;
				bi.monsters = new ENTITY[bi.num_monsters];
				bi.monsters[0] = g_Creatures[(int)MORDRED];
				bi.monsters[0].szName = "A Shady Figure";
				bi.monsters[0].threat = "\'So you think you can fulfill your father\'s legacy?\'";
				g_Game.Battle(plyr,&bi,NULL,false);
				delete [] bi.monsters;

				std::cout << std::setw(5) << "" << "\'Did you think you could beat me!\'" << std::endl;
				std::cout << std::setw(5) << "" << "\'When you get stronger we can battle" << std::endl;
				std::cout << std::setw(5) << "" << "again, but until then this is all" << std::endl;
				std::cout << std::setw(7) << "" << "you\'re worth!\'" << std::endl;
				std::cout << std::setw(5) << "" << "You\'ve received 1 gold peice, and an Axe!" << std::endl;

				plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity++;

				if(!(plyr->GetInventory()->FindID(AXE)))
				{
					ITEM *axe = new ITEM;
					axe[0] = g_Items[AXE];

					plyr->GetInventory()->End()->SetNext(axe);
				}

				std::cout << std::setw(11) << "" <<  "In a blinding instant he ran through" << std::endl;
				std::cout << std::setw(11) << "" <<  "the door leaving you to bleed. With" << std::endl;
				std::cout << std::setw(11) << "" <<  "the last strength you have you crawl" << std::endl;
				std::cout << std::setw(11) << "" <<  "to a chair nearby and throw it at one" << std::endl;
				std::cout << std::setw(11) << "" <<  "of the windows shattering it to pieces." << std::endl;
				
				std::cout << std::endl;
				system("PAUSE");
				
				std::cout << std::setw(11) << "" <<  "You then crawl to the window and fall" << std::endl;
				std::cout << std::setw(11) << "" <<  "to your death, so you close your eyes," << std::endl;
				std::cout << std::setw(11) << "" <<  "and visualize playing with your dad" << std::endl;
				std::cout << std::setw(11) << "" <<  "when you were a kid. You also see a" << std::endl;
				std::cout << std::setw(11) << "" <<  "faint glimpse of your mother. When" << std::endl;
				std::cout << std::setw(11) << "" <<  "you open your eyes you find yourself" << std::endl;
				std::cout << std::setw(11) << "" <<  "flowing in a shallow stream. Regaining" << std::endl;
				std::cout << std::setw(11) << "" <<  "consciousness you grab hold of the ground," << std::endl;
				std::cout << std::setw(11) << "" <<  "and pull yourself free of the flowing" << std::endl;
				std::cout << std::setw(11) << "" <<  "water. You lay on the grass and rest." << std::endl;
				std::cout << std::setw(11) << "" <<  "You decide its time to go for now." << std::endl << std::endl;

				plyr->SetHP(plyr->GetMaxHP());
				flags |= DID_SPECIAL;
				flags |= LVL_COMPLETE;
				g_Events[(int)MOUNTAIN].uiValue = (LVLFLAGS)flags;
				system("PAUSE");
				std::cout << std::endl;
				system("CLS");
				
				return COMPLETE;
			}// torch if
			break;
		}// cave switch
	}else{
		std::cout << std::setw(11) << "" << "The cave entrance has been sealed the" << std::endl;
		std::cout << std::setw(11) << "" << "only way to go is up." << std::endl;
	}

	std::cout << std::setw(11) << "" << "You cling onto the rocky hillside, and slowly"<< std::endl;
	std::cout << std::setw(11) << "" << "make your way up the side of the mountain."<< std::endl;
	std::cout << std::setw(11) << "" << "Ow! That is the third time a good sized rock"<< std::endl;
	std::cout << std::setw(11) << "" << "hit you in the head. You look up but it is "<< std::endl;
	std::cout << std::setw(11) << "" << "still too far to see a ledge. You continue"<< std::endl;
	std::cout << std::setw(11) << "" << "to climb stopping on small holes in the side"<< std::endl;
	std::cout << std::setw(11) << "" << "of the mountain to rest and take a break."<< std::endl;
	std::cout << std::setw(11) << "" << "You see another person climbing below your"<< std::endl;
	std::cout << std::setw(11) << "" << "ledge. Though he is climbing awfully fast"<< std::endl;
	std::cout << std::setw(11) << "" << "for the mountain side going straight up."<< std::endl;
	std::cout << std::setw(11) << "" << "You feel something brush your shoulder."<< std::endl;
	std::cout << std::setw(11) << "" << "Probably just a mosquito. It wasn\'t till"<< std::endl;
	std::cout << std::setw(11) << "" << "the mosquito knocked you out that you figured"<< std::endl;
	std::cout << std::setw(11) << "" << "out it wasn\'t a mosquito on your shoulder." << std::endl << std::endl;
	system("PAUSE");

	std::cout << std::setw(11) << "" << "You wake up to find yourself in some kind"<< std::endl;
	std::cout << std::setw(11) << "" << "of prison. The bars are made of rusted iron."<< std::endl;
	std::cout << std::setw(11) << "" << "A stupid ogre was assigned to guard your cell."<< std::endl;
	std::cout << std::setw(11) << "" << "There are a few books and some kind of jewelry"<< std::endl;
	std::cout << std::setw(11) << "" << "chest with a lock on it. Your cell is very crude"<< std::endl;
	std::cout << std::setw(11) << "" << "with straw in one corner for the bed and a"<< std::endl;
	std::cout << std::setw(11) << "" << "hole in the wall for the facilities. It is so"<< std::endl;
	std::cout << std::setw(11) << "" << "bad not even a rat would like it here."<< std::endl;
	std::cout << std::setw(11) << "" << "Luckily the ogre is taking a snooze." << std::endl;
	std::cout << std::setw(11) << "" << "You have got to think of a way out."<< std::endl << std::endl;

	do{
	result = 0;

	if(flags & OTHER_2)
	{
		std::cout << "(S)teal the keys, or (B)ust your way out?" << std::endl;
	}
	else
	{
		std::cout << "(S)teal the keys, (C)heck the room, or (B)ust your way out?" << std::endl;
	}

	keys = new unsigned char[7];
	keys[0] = 's';
	keys[1] = 'S';
	keys[2] = 'b';
	keys[3] = 'B';

	if(!(flags & OTHER_2))
	{
		keys[4] = 'c';
		keys[5] = 'C';
		keys[6] = 0;
	}
	else
	{
		keys[4] = 0;
	}

	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
       case 'c':
	   case 'C':
		flags |= (int)OTHER_2;

		if(plyr->GetIntelligence() < 20)
		{
			std::cout << "Nothing useful." << std::endl;
			break;
		}

		std::cout << std::setw(11) << "" << "You decide to try to open the chest, but what is there" << std::endl;
		std::cout << std::setw(11) << "" << " to use? You pry at the nearby chest with a piece of straw." << std::endl;

		if(plyr->GetDexterity() < 15)
		{
			std::cout << std::setw(11) << "" << "It didn\'t budge." << std::endl;
		}
		else
		{
			std::cout << std::setw(11) << "" << "You really didn\'t think it was going to work." << std::endl;
			std::cout << std::setw(11) << "" << "You found a holy rune, a Blazing Hilt, and a thunder" << std::endl;
			std::cout << std::setw(11) << "" << "rune. Now how to get out of here?" << std::endl;

			memset(&wItem,0,sizeof(wItem));
			wItem = g_Items[(int)HILT];
			wItem.istrength = 238;
			wItem.iquantity = 1;
			wItem.ilevel = 3;
			wItem.srunes.level[(int)FIRE] = 2;
			strcpy(wItem.szname,"Blazing Hilt");

			if(plyr->GetInventory()->FindItem(wItem.szname))
			{
				plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
			}
			else
			{
				plyr->GetInventory()->End()->SetNext(&wItem);
			}

			if(plyr->GetInventory()->FindItem("Thunder Rune"))
			{
				plyr->GetInventory()->FindItem("Thunder Rune")->GetItem()->iquantity++;
			}
			else
			{
				ITEM *rune = new ITEM;
				rune[0] = g_Items[THUNDER_RUNE];
				rune[0].iquantity = 1;

				plyr->GetInventory()->End()->SetNext(rune);
				delete [] rune;
			}

  			if(plyr->GetInventory()->FindItem("Holy Rune"))
			{
				plyr->GetInventory()->FindItem("Holy Rune")->GetItem()->iquantity++;
			}
			else
			{
				ITEM *rune = new ITEM;
				rune[0] = g_Items[HOLY_RUNE];
				rune[0].iquantity = 1;

				plyr->GetInventory()->End()->SetNext(rune);
				delete [] rune;
			}
		}
		break;

	  case 's':
	  case 'S':
		if(plyr->GetDexterity() < 20)
		{
			std::cout << std::setw(11) << "" << "You reach out slowly and yank the troll\'s" << std::endl;
			std::cout << std::setw(11) << "" << "keys. You got them but you also woke the" << std::endl;
			std::cout << std::setw(11) << "" << "ogre. Whoops!" << std::endl;
			
			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)OGRE];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::endl << "After that close encounter you raid the Ogre\'s" << std::endl;
			std::cout << "belt for the key and unlock yourself from the prison." << std::endl;
		}
		else
		{
			std::cout << std::setw(11) << "" << "You gracefully pull the keys from the" << std::endl;
			std::cout << std::setw(11) << "" << "guard\'s belt, and open your cell." << std::endl;
		}
		break;

	  case 'b':
	  case 'B':
		std::cout << std::setw(11) << "" << "You grab the iron bars and yank them." << std::endl;

		if(plyr->GetStrength() < 30)
		{
			std::cout << std::setw(11) << "" << "The bars didn\'t even budge. So you fall asleep." << std::endl;
			system("PAUSE");
			std::cout << std::setw(11) << "" << "When you wake the ogre is dead and the gate open." << std::endl;
		}	
		else
		{
			std::cout << std::setw(11) << "" << "The bars crumble under your strength." << std::endl;
			std::cout << std::setw(11) << "" << "Unfortunately the loud noise of your" << std::endl;
			std::cout << std::setw(11) << "" << "escape awoke the ogre!." << std::endl;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)OGRE];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;
		}
		break;
	}

	}while(result == 'c' || result == 'C');

	std::cout << std::setw(11) << "" << "You make your way down the vacant hallway."<< std::endl;
	std::cout << std::setw(11) << "" << "It looks as though you\'re in some kind of "<< std::endl;
	std::cout << std::setw(11) << "" << "dungeon. Some prisoners are choking for your"<< std::endl;
	std::cout << std::setw(11) << "" << "help, but you have enough time only for your"<< std::endl;
	std::cout << std::setw(11) << "" << "own problems. One person does catch your eye."<< std::endl;
	std::cout << std::setw(11) << "" << "Wearing a leather vest, some gloves, and some boots."<< std::endl;
	std::cout << std::setw(11) << "" << "His face consealed in some kind of leather helm."<< std::endl;
	std::cout << std::setw(11) << "" << "You recognized him as one of your father\'s"<< std::endl;
	std::cout << std::setw(11) << "" << "old adventurer pals. But he looks different."<< std::endl << std::endl;

	std::cout << "(H)elp or (L)eave?" << std::endl;

	keys = new unsigned char[5];
	keys[0] = 'h';
	keys[1] = 'H';
	keys[2] = 'l';
	keys[3] = 'L';
	keys[4] = 0;
	result = g_Game.KeyLoop(keys);
	delete [] keys;

  switch(result)
  {
	case 'h':
	case 'H':
		std::cout << std::setw(11) << "" << "You decide to help. He doesn\'t talk much" << std::endl;
		std::cout << std::setw(11) << "" << "at all. In fact he looks almost in a coma." << std::endl << std::endl;
		std::cout << "Give him a Potion (Y or N)?" << std::endl;

		result = g_Game.KeyLoop(yesno);

		switch(result)
		{
		case 'y':
		case 'Y':
			if(plyr->GetInventory()->End()->FindID(POTION))
			{
				if(plyr->GetInventory()->End()->FindID(POTION)->GetItem()->iquantity)
				{
					flags |= OTHER_3;

					std::cout << std::setw(5) << "" << "You give him a potion."<< std::endl;
					std::cout << std::setw(11) << "" << "He is almost instantly revived."<< std::endl;
					std::cout << std::setw(11) << "" << "\'Thank you, you... must... be... " << plyr->GetName() << "."<< std::endl;
					std::cout << std::setw(11) << "" << "\'The son of Cervax!\' \'What are you talking about?\'"<< std::endl;
					std::cout << std::setw(11) << "" << "\'Your father was a great man by heart. My name is"<< std::endl;
					std::cout << std::setw(11) << "" << "Paristan. I must accompany you on your journey.\'" << std::endl << std::endl;
					std::cout << "Will you take him with you (Y or N)?"<< std::endl;

  					result = g_Game.KeyLoop(yesno);

					switch(result)
					{
					case 'y':
					case 'Y':
						std::cout << std::setw(5) << "\'I\'m thrilled at your decision.\'" << std::endl;
						std::cout << std::setw(5) << "\'Let us continue your journey\'" << std::endl;
						flags |= OTHER_4;
						break;

					case 'n':
					case 'N':
						std::cout << std::setw(5) << "\'Well I trust you know what is best.\'" << std::endl;
						std::cout << std::setw(5) << "\'If that is the way you feel I must take" << std::endl;
						std::cout << std::setw(7) << "my leave as of now.\'" << std::endl;
						std::cout << std::setw(5) << "\'I wish you luck my Lord.\'" << std::endl;
						break;
					}
				}
				else
				{
					std::cout << std::setw(11) << "" << "The thought was nice, but you have no potions"<< std::endl;
					std::cout << std::setw(11) << "" << "to give him, and thoughts don\'t heal the hurt."<< std::endl;
					std::cout << std::setw(11) << "" << "You\'ll have to carry him the rest of the way"<< std::endl;
					std::cout << std::setw(11) << "" << "until you can get him to get proper treatment."<< std::endl;
				}
			}
			else
			{
				std::cout << std::setw(11) << "" << "The thought was nice, but you have no potions"<< std::endl;
				std::cout << std::setw(11) << "" << "to give him, and thoughts don\'t heal the hurt."<< std::endl;
				std::cout << std::setw(11) << "" << "You\'ll have to carry him the rest of the way"<< std::endl;
				std::cout << std::setw(11) << "" << "until you can get him to get proper treatment."<< std::endl;
			}
			break;

		case 'n':
		case 'N':
			std::cout << std::setw(11) << "" << "You wouldn\'t want to waste a potion on him." << std::endl;
			std::cout << std::setw(11) << "" << "You turn away and continue your journey." << std::endl;
			break;
		}
		break;

	case 'l':
	case 'L':
		std::cout << std::setw(11) << "" << "It\'s probably just some bum, so you continue." << std::endl;
		break;
	}

	std::cout << std::setw(11) << "" << "You spy a stone door on the far side of the wall." << std::endl;
	std::cout << std::setw(11) << "" << "It\'s the only way out so you decide to go through" << std::endl;
	std::cout << std::setw(11) << "" << "it. As you walk through it you spot four ogres on" << std::endl;
	std::cout << std::setw(11) << "" << "patrol. You decide it\'s time to have a little fun." << std::endl;

	bi.num_monsters = 4;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)OGRE];
	bi.monsters[1] = g_Creatures[(int)OGRE];
	bi.monsters[2] = g_Creatures[(int)OGRE];
	bi.monsters[3] = g_Creatures[(int)OGRE];

	if(flags & (int)OTHER_4)
		resultf = g_Game.Battle(plyr,&bi,&Paristan);
	else
		resultf = g_Game.Battle(plyr,&bi);

	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(11) << "" << "You decide that\'s just practice for now." << std::endl;
	std::cout << std::setw(11) << "" << "You walk down the newly vacant hall." << std::endl;
	std::cout << std::setw(11) << "" << "You come to a metal studded door. You" << std::endl;
	std::cout << std::setw(11) << "" << "study it closely and find no traps, so" << std::endl;
	std::cout << std::setw(11) << "" << "you guess it\'s okay to go through. You" << std::endl;
	std::cout << std::setw(11) << "" << "grab the knob, turn it, and push it in." << std::endl;
	std::cout << std::setw(11) << "" << "It opens with a loud squeal. You walk" << std::endl;
	std::cout << std::setw(11) << "" << "inside the vacant room filled with gears." << std::endl;
	std::cout << std::setw(11) << "" << "Cranks and Axles fill the room with loud" << std::endl;
	std::cout << std::setw(11) << "" << "clanks and ticks. You see a tall figure in" << std::endl;
	std::cout << std::setw(11) << "" << "the corner of the room. You start to walk" << std::endl;
	std::cout << std::setw(11) << "" << "toward it then you stop, it doesn't look right." << std::endl;

	std::cout << std::setw(8) << "" << "\'Don\'t be afraid I won\'t hurt you.\'" << std::endl;
	std::cout << std::setw(8) << "" << "\'I'm a mechanical mishap put at everybody\'s" << std::endl;
	std::cout << std::setw(8) << "" << "disposal. Brought to life using magic.\'" << std::endl;
	std::cout << std::setw(8) << "" << "\'I was tired of people telling me what to do.\'" << std::endl;
	std::cout << std::setw(8) << "" << "\'Hold on and I\'ll get my creator!\'" << std::endl;

	system("PAUSE");

	std::cout << std::setw(11) << "" << "He comes back with a small dwarf\'s head " << std::endl;
	std::cout << std::setw(11) << "" << "skewered on his mechanical hand." << std::endl;
	std::cout << std::setw(8) << "" << "\'He\'s lonely, will you join him? I\'m not" << std::endl;
	std::cout << std::setw(8) << "" << "asking you, I\'m telling YOU!!\'" << std::endl;

	if(flags & OTHER_4)
		Paristan.SetHP(Paristan.GetMaxHP());

	bi.num_monsters = 1;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)MECHAZOID];

	if(flags & (int)OTHER_4)
		resultf = g_Game.Battle(plyr,&bi,&Paristan);
	else
		resultf = g_Game.Battle(plyr,&bi);

	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	memset(&wItem,0,sizeof(wItem));
	wItem = g_Items[(int)HAND_COVER];
	wItem.istrength = 172;
	wItem.iquantity = 1;
	wItem.ilevel = 2;
	wItem.srunes.level[(int)THUNDER] = 1;
	strcpy(wItem.szname, "Mechanized Hand Cover");

	if(plyr->GetInventory()->FindItem(wItem.szname))
	{
		plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
	}
	else
	{
		plyr->GetInventory()->End()->SetNext(&wItem);
	}

	std::cout << std::setw(5) << "" << "You've acquired a Mechanized Hand Cover!" << std::endl << std::endl;

	std::cout << std::setw(11) << "" << "It\'s over you took down the mighty creature." << std::endl;
	std::cout << std::setw(11) << "" << "Or at least you thought it was the end..." << std::endl;
	system("PAUSE");

	std::cout << std::setw(11) << "" << "Suddenly, the mountain shakes with incredible" << std::endl;
	std::cout << std::setw(11) << "" << "violence. The gears and cranks stop moving." << std::endl;
	std::cout << std::setw(11) << "" << "You\'ve got to get out now! You run back through" << std::endl;
	std::cout << std::setw(11) << "" << "the door pass the jail cells and into a separate" << std::endl;
	std::cout << std::setw(11) << "" << "door down the hall. You open the door to a dining" << std::endl;
	std::cout << std::setw(11) << "" << "room. Only one window. Here we go! The sound of" << std::endl;
	std::cout << std::setw(11) << "" << "breaking glass rings through the air, and the " << std::endl;
	std::cout << std::setw(11) << "" << "sting of the sharp edges shearing your skin makes" << std::endl;
	std::cout << std::setw(11) << "" << "the ordeal even more unpleasant. You fall and land" << std::endl;
	std::cout << std::setw(11) << "" << "in a small moat. You wonder how you survived the" << std::endl;
	std::cout << std::setw(11) << "" << "great fall. You manage to get out of the moat," << std::endl;
	std::cout << std::setw(11) << "" << "and brush yourself off. The window left a scar on" << std::endl;
	std::cout << std::setw(11) << "" << "your cheek, and it is bleeding alot. You praise" << std::endl;
	std::cout << std::setw(11) << "" << "the gods and pick your things up nearby where " << std::endl;
	std::cout << std::setw(11) << "" << "they were thrown during the fall. You make your" << std::endl;
	std::cout << std::setw(11) << "" << "way out into a nearby forest searching for your" << std::endl;
	std::cout << std::setw(11) << "" << "next adventure." << std::endl;

	if(flags & (int)OTHER_4)
	{
		std::cout << std::setw(15) << "" << "Hold on! Where is Paristan!" << std::endl;
	}

	system("PAUSE");

	flags |= (int)GOT_SPECIAL;
	plyr->SetHP(plyr->GetMaxHP());

	g_Events[(int)MOUNTAIN].uiValue = (LVLFLAGS)flags;
	std::cout << std::endl;
	system("CLS");

	return COMPLETE;
}

ENDFLAG Dwarves(PLAYER *plyr)
{
	srand((unsigned int)time(NULL));
	unsigned int result,x,y,c;
	unsigned char *keys;
	unsigned char yesno[] = {'Y','N','n','y',0};
	char name[26],name2[50],name3[50],name4[50];
	ITEM iItem;
	INVENTORY *inv;

	do{
		memset(name,0,sizeof(name));
		system("cls");

		std::cout << "(L)eave, (B)uy, (R)est, or bl(A)cksmith?" << std::endl;

		keys = new unsigned char[9];
		keys[0] = 'l';
		keys[1] = 'L';
		keys[2] = 'r';
		keys[3] = 'R';
		keys[4] = 'b';
		keys[5] = 'B';
		keys[6] = 'a';
		keys[7] = 'A';
		keys[8] = 0;
		result = g_Game.KeyLoop(keys);
		delete [] keys;

		switch(result)
		{
		case 'r':
		case 'R':
			std::cout << "Welcome! It\'ll cost ya 50 gold to stay." << std::endl;

			std::cout << "(S)tay or (L)eave?" << std::endl;

			keys = new unsigned char[5];
			keys[0] = 'l';
			keys[1] = 'L';
			keys[2] = 's';
			keys[3] = 'S';
			keys[4] = 0;
			result = g_Game.KeyLoop(keys);
			delete [] keys;

			switch(result)
			{
			case 's':
			case 'S':
				if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= 50)
				{
					plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= 50;

					plyr->SetHP(plyr->GetMaxHP());
					std::cout << std::setw(11) << "" << "Thank you for staying." << std::endl;
					std::cout << std::setw(11) << "" << "You feel refreshed and relaxed." << std::endl;
					system("PAUSE");
				}
				else
				{
					std::cout << std::setw(11) << "" << "You don't have enough gold!" << std::endl;
					std::cout << std::setw(11) << "" << "Leave now!" << std::endl;
					system("PAUSE");
				}
				break;
			}

			result = 0;
			break;
		case 'b':
		case 'B':
			do{
				std::cout << std::setw(5) << "" << "Welcome what would you like!" << std::endl << std::endl;
				std::cout << "(R)unes, (P)otions, (W)eapons, (A)rmor, or (L)eave?" << std::endl;

				keys = new unsigned char[11];
				keys[0] = 'r';
				keys[1] = 'R';
				keys[2] = 'p';
				keys[3] = 'P';
				keys[4] = 'l';
				keys[5] = 'L';
				keys[6] = 'w';
				keys[7] = 'W';
				keys[8] = 'A';
				keys[9] = 'a';
				keys[10] = 0;
				result = g_Game.KeyLoop(keys);
				delete [] keys;

				switch(result)
				{
				case 'r':
				case 'R':
					std::cout << "(F)ire, (I)ce, or (T)hunder?" << std::endl;

					keys = new unsigned char[7];
					keys[0] = 'f';
					keys[1] = 'F';
					keys[2] = 'I';
					keys[3] = 'i';
					keys[4] = 'T';
					keys[5] = 't';
					keys[6] = 0;
					result = g_Game.KeyLoop(keys);
					delete [] keys;

					switch(result)
					{
					case 'f':
					case 'F':
						std::cout << std::setw(5) << "" << "Fire runes are 1000 gold each." << std::endl;
						std::cout << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity << "." << std::endl;

						std::cout << "Buy a rune(Y or N)?" << std::endl;
						result = g_Game.KeyLoop(yesno);

						switch(result)
						{
						case 'y':
						case 'Y':
							std::cout << std::endl << "How many would you like?" << std::endl;
							std::cin >> c;

							if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (1000 * c))
							{
								plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (1000 * c);

								if(plyr->GetInventory()->FindID(FIRE_RUNE))
								{
									plyr->GetInventory()->FindID(FIRE_RUNE)->GetItem()->iquantity += c;
								}
								else
								{
									iItem = g_Items[(int)FIRE_RUNE];
									iItem.iquantity = c;

									plyr->GetInventory()->End()->SetNext(&iItem);
								}
								std::cout << std::setw(5) << "" << "Thank you!" << std::endl;
							}
							else
							{
								std::cout << std::setw(5) << "" << "Don't have enough gold!" << std::endl;
							}

							system("PAUSE");
							break;

						case 'n':
						case 'N':
							break;
						}
						break;

					case 'i':
					case 'I':
						std::cout << std::setw(5) << "" << "Ice runes are 1100 gold each." << std::endl;
						std::cout << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity <<  "." << std::endl;

						std::cout << "Buy a rune(Y or N)?" << std::endl;
						result = g_Game.KeyLoop(yesno);

						switch(result)
						{
						case 'y':
						case 'Y':
							std::cout << std::endl << "How many would you like?" << std::endl;
							std::cin >> c;

							if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (1100 * c))
							{
								plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (1100 * c);
								if(plyr->GetInventory()->FindID(ICE_RUNE))
								{
									plyr->GetInventory()->FindID(ICE_RUNE)->GetItem()->iquantity += c;
								}
								else
								{
									iItem = g_Items[(int)ICE_RUNE];
									iItem.iquantity = c;

									plyr->GetInventory()->End()->SetNext(&iItem);
								}

								std::cout << std::setw(5) << "" << "Thank you!" << std::endl;
							}
							else
							{
								std::cout << std::setw(5) << "" << "Don't have enough gold!" << std::endl;
							}

							system("PAUSE");
							break;

						case 'n':
						case 'N':
							break;
						}
						break;				

					case 't':
					case 'T':
						std::cout << std::setw(5) << "" << "Thunder runes are 1400 gold each." << std::endl;
						std::cout << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity <<  "." << std::endl;

						std::cout << "Buy a rune(Y or N)?" << std::endl;
						result = g_Game.KeyLoop(yesno);

						switch(result)
						{
						case 'y':
						case 'Y':
							std::cout << std::endl << "How many would you like?" << std::endl;
							std::cin >> c;

							if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (1400 * c))
							{
								plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (1400 * c);
								if(plyr->GetInventory()->FindID(THUNDER_RUNE))
								{
									plyr->GetInventory()->FindID(THUNDER_RUNE)->GetItem()->iquantity += c;
								}
								else
								{
									iItem = g_Items[(int)THUNDER_RUNE];
									iItem.iquantity = c;

									plyr->GetInventory()->End()->SetNext(&iItem);
								}

								std::cout << std::setw(5) << "" << "Thank you!" << std::endl;
							}
							else
							{
								std::cout << std::setw(5) << "" << "Don't have enough gold!" << std::endl;
							}

							system("PAUSE");
							break;

						case 'n':
						case 'N':
							break;
						} // end rune switch
						break;
					}
					break;

				case 'p':
				case 'P':
					std::cout << std::setw(5) << "" << "Potions are 150 gold each." << std::endl;
					std::cout << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity <<  "." << std::endl;

					std::cout << "Buy a potion(Y or N)?" << std::endl;
					result = g_Game.KeyLoop(yesno);

					switch(result)
					{
					case 'y':
					case 'Y':
						std::cout << std::endl << "How many would you like?" << std::endl;
						std::cin >> c;

						if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (150 * c))
						{
							plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (150 * c);
							if(plyr->GetInventory()->FindID(POTION))
							{
								plyr->GetInventory()->FindID(POTION)->GetItem()->iquantity += c;
							}
							else
							{
								iItem = g_Items[(int)POTION];
								iItem.iquantity = c;

								plyr->GetInventory()->End()->SetNext(&iItem);
							}

							std::cout << std::setw(5) << "" << "Thank you!" << std::endl;
						}
						else
						{
							std::cout << std::setw(5) << "" << "Don't have enough gold!" << std::endl;
						}

						system("PAUSE");
						break;

					case 'n':
					case 'N':
						break;
					}
					break;

				case 'w':
				case 'W':
					do
					{
						system("cls");
						std::cout << "Welcome! Choose a weapon!" << std::endl;

						if(strlen(name))
						{
							for(x = BLADE; x <= HAMMER; x++)
							{
								if(!strcmpi(name,g_Items[(int)x].szname))
								{
									if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (g_Items[(int)x].istrength*750))
									{
										plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (g_Items[(int)x].istrength*750);
										plyr->GetInventory()->End()->SetNext(&g_Items[(int)x]);

										std::cout << std::setw(5) << "" << "Thank you!" << std::endl << std::endl;
									}
									else
									{
										std::cout << std::setw(5) << "" << "Don't have enough gold!" << std::endl << std::endl;
									}
								}
							}
						}

						for(x = BLADE; x <= HAMMER; x++)
						{
							if(!(plyr->GetInventory()->FindID((ITEMID)g_Items[(int)x].uiID)))	
							{
								std::cout << g_Items[(int)x].szname << ": " << (g_Items[(int)x].istrength*750) << " Gold" << std::endl;
							}
						}

						std::cout << std::endl << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
							 <<  "." << std::endl << std::endl << "Type end to leave." << std::endl;
						std::cin.getline(name, sizeof(name));
					}
					while(strcmpi(name,"end"));

					memset(name,0,sizeof(name));
					break;

				case 'a':
				case 'A':
					do
					{
						system("cls");
						std::cout << "Welcome! Choose an armor!" << std::endl;

						if(strlen(name))
						{
							for(x = BREASTPLATE; x <= BALDRIC; x++)
							{
								if(!strcmpi(name,g_Items[(int)x].szname))
								{
									if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (g_Items[(int)x].istrength*750))
									{
										plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (g_Items[(int)x].istrength*750);
										plyr->GetInventory()->End()->SetNext(&g_Items[(int)x]);

										std::cout << std::setw(5) << "" << "Thank you!" << std::endl << std::endl;
									}
									else
									{
										std::cout << std::setw(5) << "" << "Don't have enough gold!" << std::endl << std::endl;
									}
								}
							}
						}

						for(x = BREASTPLATE; x <= BALDRIC; x++)
						{
							if(!(plyr->GetInventory()->FindID((ITEMID)g_Items[(int)x].uiID)))	
							{
								std::cout << g_Items[(int)x].szname << ": " << (g_Items[(int)x].istrength*750) << " Gold" << std::endl;
							}
						}

						std::cout << std::endl << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
							 <<  "." << std::endl << std::endl << "Type end to leave." << std::endl;
						std::cin.getline(name, sizeof(name));
					}
					while(strcmpi(name,"end"));

					memset(name,0,sizeof(name));
					break;

			}
		}while(result != 'l' && result != 'L');

		result = 0;
		break;

		case 'a':
		case 'A':
			do
			{
				system("CLS");

				std::cout << std::setw(13) << "" << "Welcome to blacksmith shop!" << std::endl;
				std::cout << std::setw(11) << "" << "Upgrade (W)eapons, (F)asten a rune, " << std::endl;
				std::cout << std::setw(11) << "" << "Upgrade (A)rmor, (S)ynthesize, or (L)eave shop?" << std::endl;
				std::cout << std::setw(16) << "" << "What\'ll it be?" << std::endl;

				keys = new unsigned char[11];
				keys[0] = 'w';
				keys[1] = 'W';
				keys[2] = 'f';
				keys[3] = 'F';
				keys[4] = 'a';
				keys[5] = 'A';
				keys[6] = 's';
				keys[7] = 'S';
				keys[8] = 'l';
				keys[9] = 'L';
				keys[10] = 0;
				result = g_Game.KeyLoop(keys);
				delete [] keys;

				switch(result)
				{
				case 'w':
				case 'W':
					memset(name,0,sizeof(name));

					do
					{
						system("CLS");
						std::cout << "Please choose a weapon to upgrade." << std::endl << std::endl;

						if(strlen(name))
						{
							if(plyr->GetInventory()->FindItem(name))
							{
								if(plyr->GetInventory()->FindItem(name)->GetItem()->efunction == WEAPON)
								{
									if( (plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity) >=
										(((plyr->GetInventory()->FindItem(name)->GetItem()->ilevel+1) *
										  (plyr->GetInventory()->FindItem(name)->GetItem()->istrength) * 87)))
									{
										plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= 
											((plyr->GetInventory()->FindItem(name)->GetItem()->ilevel+1) *
											(plyr->GetInventory()->FindItem(name)->GetItem()->istrength) * 87);

										plyr->GetInventory()->FindItem(name)->GetItem()->ilevel++;

										x = plyr->GetInventory()->FindItem(name)->GetItem()->uiID;

										if(plyr->GetInventory()->FindID((ITEMID)x)->GetItem()->levelup)
											plyr->GetInventory()->FindID((ITEMID)x)->GetItem()->levelup(plyr->GetInventory()->FindID((ITEMID)x)->GetItem());

										std::cout << "Thank you!" << std::endl;

										if(plyr->GetCurrentWeapon().uiID ==
											plyr->GetInventory()->FindID((ITEMID)x)->GetItem()->uiID)
										{
											plyr->SetCurrentWeapon(*plyr->GetInventory()->FindID((ITEMID)x)->GetItem());
										}
									}
									else
									{
										std::cout << "Not enough money!" << std::endl;
									}
								}
								else
								{
									std::cout << "Item not found!" << std::endl;
								}
							}
							else
							{
								std::cout << "Item not found!" << std::endl;
							}
						}

						inv = plyr->GetInventory();

						while(inv)
						{
							if(inv->GetItem()->efunction == WEAPON)
							{
								std::cout << inv->GetItem()->szname << ": " 
									 << ((inv->GetItem()->ilevel+1) *
										(inv->GetItem()->istrength) * 87)
									 << " Gold" << std::endl;
							}

							inv = inv->GetNext();
						}

						std::cout << std::endl << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
							 <<  "." << std::endl << std::endl << "Type end to leave." << std::endl;
						std::cin.getline(name, sizeof(name));

					}while(strcmpi(name,"end"));

					memset(name,0,sizeof(name));
					break;

				case 'f':
				case 'F':
					memset(name,0,sizeof(name));

					do
					{
						if(strlen(name))
						{
							if(plyr->GetInventory()->FindItem(name))
							{
								if(plyr->GetInventory()->FindItem(name)->GetItem()->efunction == RUNE)
								{
									do{
										system("cls");
										std::cout << "How many would you like to fasten?" << std::endl;
										std::cin >> y;
									}while(plyr->GetInventory()->FindItem(name)->GetItem()->iquantity < y);

									if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >=
										(plyr->GetInventory()->FindItem(name)->GetItem()->istrength * y))
									{
										plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -=
											(plyr->GetInventory()->FindItem(name)->GetItem()->istrength * y);

										plyr->GetInventory()->FindItem(name)->GetItem()->iquantity -= y;

										memset(name2,0,sizeof(name2));

										do
										{
											system("cls");
											std::cout << "Please choose a weapon to fasten it to." << std::endl << std::endl;

											inv = plyr->GetInventory();

											if(strlen(name2))
											{
												if(plyr->GetInventory()->FindItem(name2))
												{
													if(plyr->GetInventory()->FindItem(name2)->GetItem()->efunction == WEAPON)
													{
														switch(plyr->GetInventory()->FindItem(name)->GetItem()->uiID)
														{
														case FIRE_RUNE:
															c = FIRE;
															break;
														case ICE_RUNE:
															c = ICE;
															break;
														case THUNDER_RUNE:
															c = THUNDER;
															break;
														case WIND_RUNE:
															c = WIND;
															break;
														case EARTH_RUNE:
															c = EARTH;
															break;
														case DARK_RUNE:
															c = DARK;
															break;
														case HOLY_RUNE:
															c = HOLY;
															break;
														}

														plyr->GetInventory()->FindItem(name2)->GetItem()->srunes.level[c] += y;

														std::cout << "Elemental level raised by " << y << '!' << std::endl;
														system("PAUSE");

														if(plyr->GetCurrentWeapon().uiID ==
															plyr->GetInventory()->FindItem(name2)->GetItem()->uiID)
														{
															plyr->SetCurrentWeapon(*plyr->GetInventory()->FindItem(name2)->GetItem());
														}

														break;
													}
													else
													{
														std::cout << "Weapon not found!" << std::endl;
														system("PAUSE");
													}
												}
												else
												{
													std::cout << "Weapon not found!" << std::endl;
													system("PAUSE");
												}
											}

											while(inv)
											{
												if(inv->GetItem()->efunction == WEAPON)
												{
													std::cout << inv->GetItem()->szname << std::endl;

													for(x=0; x < 8; x++)
													{
														switch((ELEMENT)x)
														{
														case DARK:
															std::cout << "Dark: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														case HOLY:
															std::cout << "Holy: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														case FIRE:
															std::cout << "Fire: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														case ICE:
															std::cout << "Ice: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														case THUNDER:
															std::cout << "Thunder: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														case EARTH:
															std::cout << "Earth: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														case WIND:
															std::cout << "Wind: " << inv->GetItem()->srunes.level[x] << ' ';
															break;
														}

														if(x == 4)
															std::cout << std::endl;
													}

													std::cout << std::endl;
												}

												inv = inv->GetNext();
											}

											std::cout << std::endl << "Choose a weapon." << std::endl;
											std::cin.getline(name2, sizeof(name2));

										}while(strcmpi(name2,"end"));

										memset(name2,0,sizeof(name2));
									}
									else
									{
										std::cout << "Not Enough Gold!" << std::endl;
										system("PAUSE");
									}
								}
								else
								{
									std::cout << "Item not found!" << std::endl;
									system("PAUSE");
								}
							}
							else
							{
								std::cout << "Item not found!" << std::endl;
								system("PAUSE");
							}
						}

						system("CLS");
						std::cout << "Please choose a rune to fasten." << std::endl << std::endl;

						inv = plyr->GetInventory();

						while(inv)
						{
							if(inv->GetItem()->efunction == RUNE)
							{
								if(inv->GetItem()->iquantity)
								{
									std::cout << inv->GetItem()->szname << 'x' << inv->GetItem()->iquantity 
										 << ": " << inv->GetItem()->istrength << " Gold" << std::endl;
								}
							}

							inv = inv->GetNext();
						}

						std::cout << std::endl << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
							 <<  "." << std::endl << std::endl << "Type full name (Fire Rune) or end to leave." << std::endl;
						std::cin.getline(name, sizeof(name));

					}while(strcmpi(name,"end"));
					memset(name,0,sizeof(name));
					break;

				case 'a':
				case 'A':
					memset(name,0,sizeof(name));

					do
					{
						system("CLS");

						std::cout << "Please choose an armor to upgrade." << std::endl << std::endl;

						if(strlen(name))
						{
							if(plyr->GetInventory()->FindItem(name))
							{
								if(plyr->GetInventory()->FindItem(name)->GetItem()->efunction == ARMOR)
								{
									if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >=
										((plyr->GetInventory()->FindItem(name)->GetItem()->ilevel+1) *
										(plyr->GetInventory()->FindItem(name)->GetItem()->istrength) * 87))
									{
										plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= 
											((plyr->GetInventory()->FindItem(name)->GetItem()->ilevel+1) *
											(plyr->GetInventory()->FindItem(name)->GetItem()->istrength) * 87);

										x = plyr->GetInventory()->FindItem(name)->GetItem()->uiID;

										plyr->GetInventory()->FindID((ITEMID)x)->GetItem()->ilevel++;
										plyr->GetInventory()->FindID((ITEMID)x)->GetItem()->levelup(plyr->GetInventory()->FindID((ITEMID)x)->GetItem());
										std::cout << "Thank you!" << std::endl;

										if(plyr->GetCurrentArmor().uiID ==
											plyr->GetInventory()->FindID((ITEMID)x)->GetItem()->uiID)
										{
											plyr->SetCurrentArmor(*plyr->GetInventory()->FindID((ITEMID)x)->GetItem());
										}
									}
									else
									{
										std::cout << "Not enough money!" << std::endl;
									}
								}
								else
								{
									std::cout << "Item not found!" << std::endl;
								}
							}
							else
							{
								std::cout << "Item not found!" << std::endl;
							}
						}

						inv = plyr->GetInventory();

						while(inv)
						{
							if(inv->GetItem()->efunction == ARMOR)
							{
								std::cout << inv->GetItem()->szname << ": " 
									 << ((inv->GetItem()->ilevel+1) *
										(inv->GetItem()->istrength) * 87) << " Gold" << std::endl;
							}

							inv = inv->GetNext();
						}

						std::cout << std::endl << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
							 <<  "." << std::endl << std::endl << "Type end to leave." << std::endl;
						std::cin.getline(name, sizeof(name));

					}while(strcmpi(name,"end"));

					memset(name,0,sizeof(name));
					break;

				case 's':
				case 'S':
					memset(name,0,sizeof(name));
					memset(name2,0,sizeof(name2));
					memset(name3,0,sizeof(name3));
					memset(name4,0,sizeof(name4));

					do
					{
						if(!strcmpi("assemble",name))
						{
							if(strlen(name2) && strlen(name3) && strlen(name4))
							{
								plyr->GetInventory()->FindItem(name2)->GetItem()->iquantity--;
								plyr->GetInventory()->FindItem(name3)->GetItem()->iquantity--;
								plyr->GetInventory()->FindItem(name4)->GetItem()->iquantity--;

								y = plyr->GetInventory()->FindItem(name2)->GetItem()->istrength +
									plyr->GetInventory()->FindItem(name3)->GetItem()->istrength +
									plyr->GetInventory()->FindItem(name4)->GetItem()->istrength;

								// rand(lvl)%(lvl/3) + lvl
								c = plyr->GetInventory()->FindItem(name2)->GetItem()->ilevel +
									plyr->GetInventory()->FindItem(name3)->GetItem()->ilevel +
									plyr->GetInventory()->FindItem(name4)->GetItem()->ilevel;

								x = ((rand()%c) % (c/3)) + c;

								memset(&iItem,0,sizeof(iItem));
								iItem.efunction = WEAPON;
								iItem.ilevel = 1;
								iItem.istrength = x;

								c = SWORD + 1;

								while( plyr->GetInventory()->FindID((ITEMID)c) != NULL )
								{
									c++;
								}

								iItem.uiID = c;

								for(c = (int)WIND; c <= (int)DARK; c++)
									iItem.srunes.level[c] = 
									(plyr->GetInventory()->FindItem(name2)->GetItem()->srunes.level[c] +
									plyr->GetInventory()->FindItem(name3)->GetItem()->srunes.level[c] +
									plyr->GetInventory()->FindItem(name4)->GetItem()->srunes.level[c]) / 2;

								if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= y)
								{
									std::cout << "Here is the final product." << std::endl;
									std::cout << "Strength: " << iItem.istrength << std::endl;

									for(c = (int)WIND; c <= (int)DARK; c++)
									{
										switch((ELEMENT)c)
										{
										case DARK:
											std::cout << "Dark: " << iItem.srunes.level[c] << ' ';
											break;
										case HOLY:
											std::cout << "Holy: " << iItem.srunes.level[c] << ' ';
											break;
										case FIRE:
											std::cout << "Fire: " << iItem.srunes.level[c] << ' ';
											break;
										case ICE:
											std::cout << "Ice: " << iItem.srunes.level[c] << ' ';
											break;
										case THUNDER:
											std::cout << "Thunder: " << iItem.srunes.level[c] << ' ';
											break;
										case EARTH:
											std::cout << "Earth: " << iItem.srunes.level[c] << ' ';
											break;
										case WIND:
											std::cout << "Wind: " << iItem.srunes.level[c] << ' ';
											break;
										}

										if(c == 4)
											std::cout << std::endl;
									}

									std::cout << std::endl;
									plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= y;

									// do the naming etc... here
									std::cout << "Please choose a name for this weapon (" 
										 << sizeof(name) << " chars)." << std::endl;
									std::cin.getline(name, sizeof(name));

									strcpy(iItem.szname,name);

									plyr->GetInventory()->End()->SetNext(&iItem);
									std::cout << std::endl << "Thank you!" << std::endl << std::endl;
									memset(name,0,sizeof(name));
									memset(name2,0,sizeof(name2));
									memset(name3,0,sizeof(name3));
									memset(name4,0,sizeof(name4));
								}
								else
								{
									std::cout << "Not enough money!" << std::endl;
								}
							}
							else
							{
								std::cout << "Not enough parts!" << std::endl;
							}

							system("PAUSE");
						}

						system("CLS");
						std::cout << "Please choose some components to assemble." << std::endl << std::endl;

						if(strlen(name))
						{
							if(plyr->GetInventory()->FindItem(name))
							{
								if(plyr->GetInventory()->FindItem(name)->GetItem()->efunction == PARTS)
								{
									switch(plyr->GetInventory()->FindItem(name)->GetItem()->uiID)
									{
									case HILT:
										strcpy(name2,name);
										memset(name,0,sizeof(name));
										break;
									case HAND_COVER:
										strcpy(name3,name);
										memset(name,0,sizeof(name));
										break;
									case SWORD:
										strcpy(name4,name);
										memset(name,0,sizeof(name));
										break;
									}
								}
								else
								{
									std::cout << "Part not found!" << std::endl;
								}
							}
							else
							{
								std::cout << "Part not found!" << std::endl;
							}
						}

						if(strlen(name2))
							std::cout << "Hilt: " << name2 << std::endl;
						else
							std::cout << "Hilt: None" << std::endl;

						if(strlen(name3))
							std::cout << "Hand Cover: " << name3 << std::endl;
						else
							std::cout << "Hand Cover: None" << std::endl;

						if(strlen(name4))
							std::cout << "Blade: " << name4 << std::endl << std::endl;
						else
							std::cout << "Blade: None" << std::endl;

						inv = plyr->GetInventory();

						y = 0;

						if(plyr->GetInventory()->FindItem(name2))
							y += plyr->GetInventory()->FindItem(name2)->GetItem()->istrength;
						
						if(plyr->GetInventory()->FindItem(name3))
							y += plyr->GetInventory()->FindItem(name3)->GetItem()->istrength;

						if(plyr->GetInventory()->FindItem(name4))
							y += plyr->GetInventory()->FindItem(name4)->GetItem()->istrength;

						std::cout << "Gold Available: " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity << std::endl;
						std::cout << "Cost: " << y << " Gold" << std::endl;
						std::cout << std::endl;

						while(inv)
						{
							if(inv->GetItem()->efunction == PARTS)
							{
								if(inv->GetItem()->iquantity)
								{
									std::cout << inv->GetItem()->szname << ": " 
										 << inv->GetItem()->istrength << " Gold / ";
									switch(inv->GetItem()->uiID)
									{
									case HILT:
										std::cout << "Hilt";
										break;
									case HAND_COVER:
										std::cout << "Hand Cover";
										break;
									case SWORD:
										std::cout << "Sword Blade";
										break;
									}

									std::cout << std::endl;
								}
							}

							inv = inv->GetNext();
						}

						std::cout << std::endl << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
							<< "." << std::endl << std::endl << "Type end to leave or assemble to make a weapon." << std::endl;
						std::cin.getline(name, sizeof(name));

					}while(strcmpi(name,"end"));

					memset(name,0,sizeof(name));
					break;
				}
			}while(result != 'l' && result != 'L');

			result = 0;
			break;
		}
	}while(result != 'l' && result != 'L');
	return COMPLETE;
}

ENDFLAG Castle(PLAYER* plyr)
{
	srand((unsigned int)time(NULL));
	BATTLEINFO bi;
	unsigned int flags = g_Events[(int)CASTLE].uiValue;
	ENDFLAG resultf;
	unsigned int result,x;
	unsigned char *keys;
	unsigned char yesno[] = {'Y','N','n','y',0};
	system("cls");
	ITEM wItem;

	std::cout << std::setw(15) << "" << "You have chosen the Castle of Solitude!" << std::endl;
    std::cout << std::setw(15) << "" << "Difficulty: Hard" << std::endl;
    std::cout << "You remember this castle from a old tale" << std::endl;
    std::cout << "that your grandfather told you. The only" << std::endl;
    std::cout << "part of the story that you remember is that" << std::endl;
    std::cout << "in the castle treasury room lies gold and" << std::endl;
    std::cout << "jewels, and something else about a beast," << std::endl;
    std::cout << "but as strong as you are your not worried." << std::endl << std::endl;
	system("PAUSE");

    std::cout << "Your grandfather was a strong man as well" << std::endl;
    std::cout << "as a expert blacksmith training under the" << std::endl;
    std::cout << "very best as a apprentice. In fact the" << std::endl;
    std::cout << "sword your father carried was crafted by" << std::endl;
    std::cout << "your grandfather using a very rare metal" << std::endl;
    std::cout << "for the blade, and the finest ivory for" << std::endl;
    std::cout << "the handle. No doubt your grandfather was" << std::endl;
    std::cout << "very skilled in his profession, and also" << std::endl;
    std::cout << "very serious in his work. You remember so" << std::endl;
    std::cout << "little about your great grandfather. Your" << std::endl;
    std::cout << "grandfather died while in battle, and sadly" << std::endl;
    std::cout << "your father followed in his footsteps. Soon" << std::endl;
    std::cout << "you will find their killers and avenge their" << std::endl;
    std::cout << "unjustifiable death with the blade of your " 
		 << plyr->GetCurrentWeapon().szname << "." << std::endl << std::endl;
	system("PAUSE");

    std::cout << std::setw(5) << "" << "You make for the castle bridge, and to" << std::endl;
    std::cout << std::setw(5) << "" << "your displeasure an ogre is standing" << std::endl;
    std::cout << std::setw(5) << "" << "guard. \'I\'ll let you pass for a fee of 500 gold.\'" << std::endl;
	std::cout << "(P)ay or (F)ight to enter?" << std::endl;

	keys = new unsigned char[5];
	keys[0] = 'p';
	keys[1] = 'P';
	keys[2] = 'f';
	keys[3] = 'F';
	keys[4] = 0;
	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
	case 'p':
	case 'P':
		if( plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= 500 )
		{
			plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= 500;
			std::cout << std::setw(11) << "" << "You may now enter." << std::endl;
		}
        else
		{
			std::cout << std::setw(5) << "" << "You don't have money. Then DIE!!!" << std::endl;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)OGRE];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;
		}
		break;
	case 'f':
	case 'F':
		std::cout << std::setw(5) << "" << "Get ready to die for making a fool of me." << std::endl;

		bi.num_monsters = 1;
		bi.monsters = new ENTITY[bi.num_monsters];
		bi.monsters[0] = g_Creatures[(int)OGRE];
		resultf = g_Game.Battle(plyr,&bi);
		delete [] bi.monsters;

		if(resultf != COMPLETE)
			return resultf;
		break;
	}

	std::cout << std::setw(5) << "" << "As you enter you think to yourself..." << std::endl;
    std::cout << std::setw(5) << "" << "that was too easy." << std::endl;
    system("PAUSE");

	std::cout << std::setw(11) << "" << "For being 1500 years old the tile floor" << std::endl;
    std::cout << std::setw(11) << "" << "looks surprisingly new, and if it wasn\'t" << std::endl;
    std::cout << std::setw(11) << "" << "for the moaning of three zombies," << std::endl;
    std::cout << std::setw(11) << "" << "you could say it was quiet." << std::endl;
 
	bi.num_monsters = 3;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)ZOMBIE];
	bi.monsters[1] = g_Creatures[(int)ZOMBIE];
	bi.monsters[2] = g_Creatures[(int)ZOMBIE];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(5) << "" << "A little exhausted from the skeletons" << std::endl;
    std::cout << std::setw(5) << "" << "you slowly make your way down the hall." << std::endl;
    system("PAUSE");

	if(!(flags & OTHER_1))
	{
		std::cout << std::setw(5) << "" << "You see a door marked with faded wording." << std::endl;
		std::cout << std::setw(5) << "" << "You could get closer to read it." << std::endl;
  
		std::cout << "Get (C)loser or (I)gnore it?" << std::endl;

		keys = new unsigned char[5];
		keys[0] = 'c';
		keys[1] = 'C';
		keys[2] = 'i';
		keys[3] = 'I';
		keys[4] = 0;
		result = g_Game.KeyLoop(keys);
		delete [] keys;

		switch(result)
		{
		case 'c':
		case 'C':
			std::cout << std::setw(11) << "" << "As you get closer you can make out the" << std::endl;
			std::cout << std::setw(11) << "" << "letters L....a....b carved on the door." << std::endl;
			std::cout << std::setw(11) << "" << "Again turning your back gave the ogres" << std::endl;
			std::cout << std::setw(11) << "" << "just enough time to attack you." << std::endl;
       
			bi.num_monsters = 2;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)OGRE];
			bi.monsters[1] = g_Creatures[(int)OGRE];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(11) << "" << "Now that the ogres are out of your way" << std::endl;
			std::cout << std::setw(11) << "" << "you can make out they say the word \'Lab\'!" << std::endl;
			
			std::cout << "(G)o in or (I)gnore?" << std::endl;
			keys = new unsigned char[5];
			keys[0] = 'g';
			keys[1] = 'G';
			keys[2] = 'i';
			keys[3] = 'I';
			keys[4] = 0;
			result = g_Game.KeyLoop(keys);
			delete [] keys;

		   switch(result)
		   {
			case 'g':
			case 'G':
				std::cout << std::setw(5) << "" << "As you walk in you notice a weapon piece." << std::endl;
				std::cout << std::setw(5) << "" << "So being the adventurous one you grab them." << std::endl;
				std::cout << std::setw(5) << "" << "And as usual you neglected to notice the" << std::endl;
				std::cout << std::setw(5) << "" << "three zombies lurking near the entrance!" << std::endl;

				bi.num_monsters = 3;
				bi.monsters = new ENTITY[bi.num_monsters];
				bi.monsters[0] = g_Creatures[(int)ZOMBIE];
				bi.monsters[1] = g_Creatures[(int)ZOMBIE];
				bi.monsters[2] = g_Creatures[(int)ZOMBIE];
				resultf = g_Game.Battle(plyr,&bi);
				delete [] bi.monsters;

				if(resultf != COMPLETE)
					return resultf;

				memset(&wItem,0,sizeof(wItem));
				wItem = g_Items[(int)HILT];
				wItem.istrength = 572;
				wItem.iquantity = 1;
				wItem.ilevel = 5;
				wItem.srunes.level[(int)DARK] = 3;
				strcpy(wItem.szname,"Death Touch");

				if(plyr->GetInventory()->FindItem(wItem.szname))
				{
					plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
				}
				else
				{
					plyr->GetInventory()->End()->SetNext(&wItem);
				}

				std::cout << std::setw(11) << "" << "When you defeated the second zombie" << std::endl;
				std::cout << std::setw(11) << "" << "a thought went through your head where" << std::endl;
				std::cout << std::setw(11) << "" << "the hell are all these monsters coming" << std::endl;
				std::cout << std::setw(11) << "" << "from!? So again you make your way into" << std::endl;
				std::cout << std::setw(11) << "" << "the hallway corridor." << std::endl << std::endl;
				flags |= (int) OTHER_1;

				system("PAUSE");
				break;

			case 'i':
			case 'I':
				std::cout << std::setw(5) << "" << "Some things are better left alone so" << std::endl;
				std::cout << std::setw(5) << "" << "again you start down the corridor." << std::endl;
				break;
		   }
		case 'i':
		case 'I':
			std::cout << std::setw(5) << "" << "You again find yourself walking down" << std::endl;
			std::cout << std::setw(5) << "" << "the incredibly quiet hallway." << std::endl;
			break;
		}
	}

	std::cout << std::setw(5) << "" << "During your stroll you spot a courtyard from" << std::endl;
    std::cout << std::setw(5) << "" << "inside the hallway through the archway entrance." << std::endl << std::endl;

	std::cout << "(G)o in or (I)gnore?" << std::endl;
	keys = new unsigned char[5];
	keys[0] = 'g';
	keys[1] = 'G';
	keys[2] = 'i';
	keys[3] = 'I';
	keys[4] = 0;
	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
	case 'g':
	case 'G':
		std::cout << std::setw(11) << "" << "You decide to stop in the garden for" << std::endl;
		std::cout << std::setw(11) << "" << "a quick rest, and a short investigation." << std::endl;
		std::cout << std::setw(11) << "" << "Besides you have had enough of walking" << std::endl;
		std::cout << std::setw(11) << "" << "down this goddamn neverending hall." << std::endl << std::endl;
		system("PAUSE");

		std::cout << std::setw(5) << "" << "You come into the courtyard. It looks" << std::endl;
		std::cout << std::setw(5) << "" << "like it was tended to not too long ago." << std::endl;
		std::cout << std::setw(5) << "" << "There is also a working fountain in" << std::endl;
		std::cout << std::setw(5) << "" << "the center of the courtyard. The group" << std::endl;
		std::cout << std::setw(5) << "" << "of bushes in the courtyard are trimmed" << std::endl;
		std::cout << std::setw(5) << "" << "in the shape of a dragon. They are " << std::endl;
		std::cout << std::setw(5) << "" << "frighteningly realistic, and extremely" << std::endl;
		std::cout << std::setw(5) << "" << "tall. There are four each surrounding" << std::endl;
		std::cout << std::setw(5) << "" << "the working fountain holding the marble" << std::endl;
		std::cout << std::setw(5) << "" << "statue of a dragon slayor." << std::endl << std::endl;

		std::cout << "Examine the (F)ountain or (B)ushes?" << std::endl;
		keys = new unsigned char[5];
		keys[0] = 'b';
		keys[1] = 'B';
		keys[2] = 'f';
		keys[3] = 'F';
		keys[4] = 0;
		result = g_Game.KeyLoop(keys);
		delete [] keys;

		switch(result)
		{
		case 'f':
		case 'F':
			std::cout << std::setw(11) << "" << "Having getting close enough to the" << std::endl;
			std::cout << std::setw(11) << "" << "fountain you realize that it is in" << std::endl;
			std::cout << std::setw(11) << "" << "fact a wishing well. Nobody will" << std::endl;
			std::cout << std::setw(11) << "" << "miss a few gold coins so you help" << std::endl;
			std::cout << std::setw(11) << "" << "yourself to the riches. It wasn\'t" << std::endl;
			std::cout << std::setw(11) << "" << "till after you got the coins that" << std::endl;
			std::cout << std::setw(11) << "" << "you realized why everythings so big." << std::endl;
			std::cout << std::setw(11) << "" << "The garden is tended by a giant that" << std::endl;
			std::cout << std::setw(11) << "" << "doesn\'t seem to like visitors." << std::endl;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY;
			bi.monsters[0] = g_Creatures[(int)GIANT];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;
			
			if(resultf != COMPLETE)
				return resultf;

			x = (rand() % 250);
			plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity += x;
			std::cout << std::setw(11) << "" << "You found " << x << " gold pieces!" << std::endl;
			break;
		case 'b':
		case 'B':
			if(flags & OTHER_2)
			{
				std::cout << std::setw(11) << "" << "Nothing has changed from your last visit." << std::endl;
			}
			else
			{
				std::cout << std::setw(11) << "" << "The dragons are a good seven feet tall," << std::endl;
				std::cout << std::setw(11) << "" << "and four feet wide. Everything to the" << std::endl;
				std::cout << std::setw(11) << "" << "tooth to the scales are done with" << std::endl;
				std::cout << std::setw(11) << "" << "unimaginable detail and percision." << std::endl;
				std::cout << std::setw(11) << "" << "After another close inspection you" << std::endl;
				std::cout << std::setw(11) << "" << "find a weapon piece stuck in one of" << std::endl;
				std::cout << std::setw(11) << "" << "the dragons bushy stomachs. Probably" << std::endl;
				std::cout << std::setw(11) << "" << "dropped by the caretaker. You guessed" << std::endl;
				std::cout << std::setw(11) << "" << "he wouldn\'t mind if you borrow it." << std::endl;
				std::cout << std::setw(11) << "" << "Unfortunately he did mind, and the" << std::endl;
				std::cout << std::setw(11) << "" << "garden\'s caretaker is in fact a giant." << std::endl;
				std::cout << std::setw(11) << "" << "He doesn\'t look to happy either." << std::endl << std::endl;

				bi.num_monsters = 1;
				bi.monsters = new ENTITY;
				bi.monsters[0] = g_Creatures[(int)GIANT];
				resultf = g_Game.Battle(plyr,&bi);
				delete [] bi.monsters;
				
				if(resultf != COMPLETE)
					return resultf;

				memset(&wItem,0,sizeof(wItem));
				wItem = g_Items[(int)HAND_COVER];
				wItem.istrength = 361;
				wItem.iquantity = 1;
				wItem.ilevel = 4;
				strcpy(wItem.szname, "Resillient Killer");

				if(plyr->GetInventory()->FindItem(wItem.szname))
				{
					plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
				}
				else
				{
					plyr->GetInventory()->End()->SetNext(&wItem);
				}

				std::cout << std::setw(11) << "" << "Sitting over the dead body of a huge" << std::endl;
				std::cout << std::setw(11) << "" << "giant you wonder to yourself. How " << std::endl;
				std::cout << std::setw(11) << "" << "many are left? Since your done in the" << std::endl;
				std::cout << std::setw(11) << "" << "garden you decide to head back to the" << std::endl;
				std::cout << std::setw(11) << "" << "acursed hallway." << std::endl;

				flags |= (int)OTHER_2;
			}
			break;
		}
	case 'i':
	case 'I':
		std::cout << std::setw(11) << "" << "You decide there\'s nothing to do in" << std::endl;
		std::cout << std::setw(11) << "" << "the courtyard so you decide to start" << std::endl;
		std::cout << std::setw(11) << "" << "back down the hallway." << std::endl;
		break;
	}

	std::cout << std::setw(11) << "" << "Walk, walk, walk and walk some more. " << std::endl;
	std::cout << std::setw(11) << "" << "You\'re really getting tired of walking" << std::endl;
	std::cout << std::setw(11) << "" << "so you decide to stop and rest for a" << std::endl;
	std::cout << std::setw(11) << "" << "second against the wall." << std::endl;
	std::cout << std::setw(11) << "" << "As you lean against the wall you" << std::endl;
	std::cout << std::setw(11) << "" << "remember how good it feels to stop" << std::endl;
	std::cout << std::setw(11) << "" << "and rest for a second." << std::endl;
	std::cout << std::setw(11) << "" << "You shut your eyes for a second." << std::endl;
	std::cout << std::setw(11) << "" << "And the wall gives way." << std::endl;
	std::cout << std::setw(11) << "" << "You fall for a good five minutes then" << std::endl;
	std::cout << std::setw(11) << "" << "you hit the floor." << std::endl << std::endl;

	x = ((rand() % 10) + 5);

	plyr->AddHP(0-x);

    if(plyr->GetHP() <= 0)
	{
		system("cls");
		std::cout << std::setw(11) << "" << "Unfortunately, the fall happened to kill you." << std::endl << std::endl;

		std::cout << std::endl << "Load Game? (Y/N)" << std::endl;

		result = g_Game.KeyLoop(yesno);
		switch(result)
		{
		case 'Y'://21: // 'Y'
		case 'y':
			return CONTINUE;
			break;
		case 'N'://49: // 'N'
		case 'n':
			return QUIT;
			break;
		}
	}

	std::cout << std::setw(11) << "" << "You were dealt " << x << " damage!" << std::endl;
	std::cout << std::setw(11) << "" << "You look around, and see a strange" << std::endl;
	std::cout << std::setw(11) << "" << "light coming from the darkness of" << std::endl;
	std::cout << std::setw(11) << "" << "the cave." << std::endl << std::endl;

	std::cout << "(I)nvestigate of i(G)nore?" << std::endl;

	keys = new unsigned char[5];
	keys[0] = 'i';
	keys[1] = 'I';
	keys[2] = 'g';
	keys[3] = 'G';
	keys[4] = 0;
	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
	case 'i':
	case 'I':
		std::cout << std::setw(5) << "" << "You decide to find out what the light" << std::endl;
		std::cout << std::setw(5) << "" << "coming from the distance is." << std::endl;
		system("PAUSE");

		std::cout << std::setw(5) << "" << "You spot a old man wearing the priests" << std::endl;
		std::cout << std::setw(5) << "" << "robes and chanting near a fire." << std::endl;
		std::cout << std::setw(5) << "" << "\'Come near and rest, or I can also sell" << std::endl;
		std::cout << std::setw(5) << "" << "thee potions for 100 gold pieces each.\'" << std::endl << std::endl;

		do{
			std::cout << "(R)est or (B)uy or (L)eave?" << std::endl;
			keys = new unsigned char[7];
			keys[0] = 'r';
			keys[1] = 'R';
			keys[2] = 'b';
			keys[3] = 'B';
			keys[4] = 'l';
			keys[5] = 'L';
			keys[6] = 0;

			result = g_Game.KeyLoop(keys);
			delete [] keys;

		   switch(result)
		   {
			case 'r':
			case 'R':
				plyr->SetHP(plyr->GetMaxHP());
			    std::cout << "You feel relaxed and refreshed." << std::endl;
				x = 1;
				break;
			case 'b':
			case 'B':
				std::cout << std::setw(20) << "" << "Potions 100 gold pieces each." << std::endl;
				std::cout << "Available gold: " 
					 << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity << " ." << std::endl;
				std::cout << "How many?" << std::endl;
				std::cin >> x;

				if( plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= ( x * 100) )
				{
					plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (x * 100);
					std::cout << "Thank you." << std::endl;
				}
				else
				{
					std::cout << "Don't have enough gold." << std::endl;
				}

				x = 1;
				break;
			case 'l':
			case 'L':
				x = 0;
				break;
		   }
		}while(x);

		std::cout << std::setw(11) << "" << "You bid farewell to the old man" << std::endl;
		std::cout << std::setw(11) << "" << "and start back on your way." << std::endl;
		std::cout << std::setw(11) << "" << "The old man told you when you" << std::endl;
		std::cout << std::setw(11) << "" << "were resting to head west down the" << std::endl;
		std::cout << std::setw(11) << "" << "darkest tunnel the reach the riches." << std::endl;

		std::cout << "(T)ake his advice, or (I)gnore his advice?" << std::endl;

		keys = new unsigned char[5];
		keys[0] = 't';
		keys[1] = 'T';
		keys[2] = 'i';
		keys[3] = 'I';
		keys[4] = 0;

		result = g_Game.KeyLoop(keys);
		delete [] keys;

	   switch(result)
	   {
		case 't':
		case 'T':
			std::cout << std::setw(11) << "" << "You take his advice and decide" << std::endl;
			std::cout << std::setw(11) << "" << "to trust him. As you make your" << std::endl;
			std::cout << std::setw(11) << "" << "way down the tunnel you notice" << std::endl;
			std::cout << std::setw(11) << "" << "it gets hotter and hotter the" << std::endl;
			std::cout << std::setw(11) << "" << "further you go." << std::endl;
			std::cout << std::setw(5) << "" << "You turn right going through the" << std::endl;
			std::cout << std::setw(5) << "" << "tunnel and instead of more walking" << std::endl;
			std::cout << std::setw(5) << "" << "you are blessed with pounds of gold" << std::endl;
			std::cout << std::setw(5) << "" << "and jewelry. You quickly dive in the" << std::endl;
			std::cout << std::setw(5) << "" << "gold coins covered by wealth you" << std::endl;
			std::cout << std::setw(5) << "" << "forget about the beast, and just your" << std::endl;
			std::cout << std::setw(5) << "" << "luck it happens to be a angry dragon." << std::endl;

			bi.num_monsters = 1;
			bi.monsters = new ENTITY;
			bi.monsters[0] = g_Creatures[(int)DRAGON];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(11) << "" << "You won you finally won all the gold" << std::endl;
			std::cout << std::setw(11) << "" << "is yours, unfortunately with the dragons" << std::endl;
			std::cout << std::setw(11) << "" << "last breath he melted the gold together!" << std::endl;
			std::cout << std::setw(11) << "" << "You sigh and take the gold he didn\'t" << std::endl;
			std::cout << std::setw(11) << "" << "melt, and a few potions that weren\'t" << std::endl;
			std::cout << std::setw(11) << "" << "stuck in the metal. Before you leave" << std::endl;
			std::cout << std::setw(11) << "" << "you chop off one of the dragons teeth." << std::endl;
			std::cout << std::setw(11) << "" << "You slowly but proudly make your way" << std::endl;
			std::cout << std::setw(11) << "" << "out of the chamber." << std::endl << std::endl;
      
			x = (rand() % 200) + 120;
			plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity += x;

			plyr->SetHP(plyr->GetMaxHP());
			flags |= LVL_COMPLETE;
			flags |= DID_SPECIAL;
			g_Events[(int)CASTLE].uiValue = (LVLFLAGS)flags;
			system("PAUSE");
			std::cout << std::endl;
			system("CLS");
			return COMPLETE;
			break;

		case 'i':
		case 'I':
			std::cout << std::setw(5) << "" << "You decide not to heed the advice of a" << std::endl;
			std::cout << std::setw(5) << "" << "stranger, and continue on the next route." << std::endl;
			break;
	   }

   	case 'g':
	case 'G':
		std::cout << std::setw(5) << "" << "Having decided you had better stick" << std::endl;
		std::cout << std::setw(5) << "" << "to your quest. You decide it isn\'t real" << std::endl;
		std::cout << std::setw(5) << "" << "important so you decide to go on your" << std::endl;
		std::cout << std::setw(5) << "" << "way instead of worrying about lights." << std::endl;
		break;
	}

	std::cout << std::setw(5) << "" << "You can barely see where you\'re going," << std::endl;
	std::cout << std::setw(5) << "" << "but you can see the wall\'s outlines in" << std::endl;
	std::cout << std::setw(5) << "" << "the dark. You make your way down the" << std::endl;
	std::cout << std::setw(5) << "" << "dark tunnel then suddenly the tunnel " << std::endl;
	std::cout << std::setw(5) << "" << "lights up incredibly bright." << std::endl;
	std::cout << std::setw(5) << "" << "When your eyes adjust you find yourself" << std::endl;
	std::cout << std::setw(5) << "" << "face to face with a cyclops! Whoops!!!" << std::endl;

	bi.num_monsters = 1;
	bi.monsters = new ENTITY;
	bi.monsters[0] = g_Creatures[(int)CYCLOPS];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(11) << "" << "Surprisingly you strike down the huge" << std::endl;
	std::cout << std::setw(11) << "" << "cyclops, and swarm through his pouch." << std::endl;
	std::cout << std::setw(11) << "" << "You continue your way down the tunnel" << std::endl;
	std::cout << std::setw(11) << "" << "and spot light further down the tunnel." << std::endl;
	std::cout << std::setw(11) << "" << "Starting to a jog you gladly welcome" << std::endl;
	std::cout << std::setw(11) << "" << "the light when you exit the dark abyss." << std::endl;
	std::cout << std::setw(11) << "" << "Wondering what to do now you wonder" << std::endl;
	std::cout << std::setw(11) << "" << "if you should retire? Naaahhhh." << std::endl;

	plyr->SetHP(plyr->GetMaxHP());
	flags |= LVL_COMPLETE;
	g_Events[(int)CASTLE].uiValue = (LVLFLAGS)flags;
	system("PAUSE");
	std::cout << std::endl;
	system("CLS");

	return COMPLETE;
}

ENDFLAG Fortress(PLAYER* plyr)
{
	srand((unsigned int)time(NULL));
	BATTLEINFO bi;
	unsigned int flags = g_Events[(int)FORTRESS].uiValue;
	ENDFLAG resultf;
	unsigned int result,x;
	unsigned char *keys;
	unsigned char yesno[] = {'Y','N','n','y',0};
	system("cls");
	ITEM wItem;

	std::cout << std::setw(15) << "" << "Welcome to The Unforgiving Fortress!" << std::endl;
	std::cout << std::setw(15) << "" << "Difficulty: Nightmare!" << std::endl;
	std::cout << std::setw(11) << "" << "Here you are at your final destination." << std::endl;
	std::cout << std::setw(11) << "" << "What will happen now depends on you, your" << std::endl;
	std::cout << std::setw(11) << "" << "training, and your skill. Once you enter" << std::endl;
	std::cout << std::setw(11) << "" << "you can not go back until you win." << std::endl;
	std::cout << "Are you sure you want to enter (Y/N)?" << std::endl;

	result = g_Game.KeyLoop(yesno);

	if((result == 'n') || (result == 'N'))
		return COMPLETE;

	std::cout << std::setw(11) << "" << "As you walk in front of the huge dwelling." << std::endl;
	std::cout << std::setw(11) << "" << "You recall your father\'s, and grandfather\'s" << std::endl;
	std::cout << std::setw(11) << "" << "last wish was to avenge their unworthy death." << std::endl;
	std::cout << std::setw(11) << "" << "You raise your " << plyr->GetCurrentWeapon().szname << " and prepare for the" << std::endl;
	std::cout << std::setw(11) << "" << "ultimate battle. All the last few levels were" << std::endl;
	std::cout << std::setw(11) << "" << "in preparation for this one. You feel confident" << std::endl;
	std::cout << std::setw(11) << "" << "and sure of yourself to make it this far. If it" << std::endl;
	std::cout << std::setw(11) << "" << "wasn\'t for your father\'s lessons, and grandfather\'s" << std::endl;
	std::cout << std::setw(11) << "" << "knowledge of weaponry and armory you would not" << std::endl;
	std::cout << std::setw(11) << "" << "have made it this far in your journey." << std::endl << std::endl;

	std::cout << std::setw(11) << "" << "Their death will be avenged by your " << plyr->GetCurrentWeapon().szname << ", and" << std::endl;
	std::cout << std::setw(11) << "" << "none guilty shall surpass your great strength." << std::endl;
	std::cout << std::setw(11) << "" << "With full confidence in yourself you slowly walk" << std::endl;
	std::cout << std::setw(11) << "" << "into the towering fortress to confront the greatest" << std::endl;
	std::cout << std::setw(11) << "" << "challenge and enemie ever fought. If he bested" << std::endl;
	std::cout << std::setw(11) << "" << "your father and grandfather this shall be a battle" << std::endl;
	std::cout << std::setw(11) << "" << "remembered in history by even the best." << std::endl;
	system("PAUSE");

	if(g_Events[(int)MOUNTAIN].uiValue & OTHER_4) // if Peristan is alive
	{
		do{
			system("cls");
			std::cout << std::setw(11) << "" << "As you enter through the arch a mysterious man" << std::endl;
			std::cout << std::setw(11) << "" << "wearing a robe stops you to talk. \'Stop my friend.\'" << std::endl;
			std::cout << std::setw(11) << "" << "\'You must buy supplies before venturing in the" << std::endl;
			std::cout << std::setw(11) << "" << "evil\'s stronghold.\'" << std::endl << std::endl;

			std::cout << "Buy (M)ax potions, (P)otions, or (L)eave?" << std::endl;

			keys = new unsigned char[5];
			keys[0] = 'm';
			keys[1] = 'M';
			keys[2] = 'P';
			keys[3] = 'p';
			keys[5] = 'l';
			keys[6] = 'L';
			keys[7] = 0;

			result = g_Game.KeyLoop(keys);
			delete [] keys;

		   switch(result)
		   {
		   case 'm':
		   case 'M':
				system("cls");
				std::cout << std::setw(5) << "" << "Gold: " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity
					 << "  /  " << "Potions: " << plyr->GetInventory()->FindID(POTION)->GetItem()->iquantity << std::endl;
				std::cout << std::setw(5) << "" << "It requires 15 potions and 350 gold to" << std::endl;
				std::cout << std::setw(5) << "" << "create a max potion." << std::endl << std::endl;

				std::cout << "Create (Y or N)?" << std::endl;

				result = g_Game.KeyLoop(yesno);

				if(result == 'Y' || result == 'y')
				{
					std::cout << "How many?" << std::endl;
					std::cin >> x;

					if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (x * 350))
					{
						if(plyr->GetInventory()->FindID(POTION)->GetItem()->iquantity >= (x * 15))
						{
							plyr->GetInventory()->FindID(POTION)->GetItem()->iquantity -= (x * 15);
							plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (x * 350);

							if(plyr->GetInventory()->FindID(MAX_POTION))
							{
								plyr->GetInventory()->FindID(MAX_POTION)->GetItem()->iquantity += x;
							}
							else
							{
								wItem = g_Items[(int)MAX_POTION];

								plyr->GetInventory()->End()->SetNext(&wItem);
								plyr->GetInventory()->FindID(MAX_POTION)->GetItem()->iquantity += x;
							}

							std::cout << std::setw(5) << "" << "Wait while I make it for you." << std::endl;
							std::cout << std::setw(5) << "" << "There you go a max potion refills all your life!" << std::endl;
							std::cout << std::setw(11) << "" << "Anything else?" << std::endl;
						}
						else
						{
							std::cout << "Sorry, but you don\'t have enough potions." << std::endl;
							system("PAUSE");
						}
					}
					else
					{
						std::cout << "Sorry, but you don\'t have enough gold." << std::endl;
						system("PAUSE");
					}
				}
				break;

			case 'p':
			case 'P':
				system("cls");
				std::cout << std::setw(5) << "" << "One potion is 75 gold" << std::endl;
				std::cout << std::setw(5) << "" << "Available gold is " << plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity << "." << std::endl;

				std::cout << "How many do you want?" << std::endl;
				std::cin >> x;

				if(plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity >= (x * 75))
				{
					plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity -= (x * 75);

					if(plyr->GetInventory()->FindID(POTION))
					{
						plyr->GetInventory()->FindID(POTION)->GetItem()->iquantity += x;
					}
					else
					{
						wItem = g_Items[(int)POTION];

						plyr->GetInventory()->End()->SetNext(&wItem);
						plyr->GetInventory()->FindID(POTION)->GetItem()->iquantity += x;
					}

					std::cout << std::setw(5) << "" << "Wait while I make it for you." << std::endl;
					std::cout << std::setw(11) << "" << "Anything else?" << std::endl;
				}
				else
				{
					std::cout << "Sorry, but you don\'t have enough gold." << std::endl;
					system("PAUSE");
				}
				break;

			case 'l':
			case 'L':
				system("cls");
				std::cout << std::setw(11) << "" << "You wave bye to the old man, and thank him" << std::endl;
				std::cout << std::setw(11) << "" << "for his hospitality and kindness." << std::endl;
				break;
		   }
	   }while((result != 'L') && (result != 'l'));
	}

	std::cout << std::setw(11) << "" << "You pass under the archway and into the dark" << std::endl;
	std::cout << std::setw(11) << "" << "courtyard. Walking silently among dead roses" << std::endl;
	std::cout << std::setw(11) << "" << "and other such once beautiful things. You" << std::endl;
	std::cout << std::setw(11) << "" << "stop to look at the eerie settings, and notice" << std::endl;
	std::cout << std::setw(11) << "" << "that this place doesn\'t look friendly at all." << std::endl;
	std::cout << std::setw(11) << "" << "Especially when the dragon popped out of nowhere." << std::endl;

	bi.num_monsters = 1;
	bi.monsters = new ENTITY;
	bi.monsters[0] = g_Creatures[(int)DRAGON];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(11) << "" << "As strong as you are that was child\'s play." << std::endl;
	std::cout << std::setw(11) << "" << "It wasn\'t until the rest of the family came" << std::endl;
	std::cout << std::setw(11) << "" << "to avenge it's death, that you realized their mad." << std::endl;

	bi.num_monsters = 2;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)DRAGON];
	bi.monsters[1] = g_Creatures[(int)DRAGON];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(11) << "" << "You make your way past the dragon\'s dead " << std::endl;
	std::cout << std::setw(11) << "" << "bodies and make your way out of the courtyard." << std::endl;
	std::cout << std::setw(11) << "" << "You arrive in sort of a dining room hall." << std::endl;
	std::cout << std::setw(11) << "" << "You seemed to disturbed some cyclops having" << std::endl;
	std::cout << std::setw(11) << "" << "their dragon dinner. The ogres didn\'t get really" << std::endl;
	std::cout << std::setw(11) << "" << "mad until they saw that you killed their main" << std::endl;
	std::cout << std::setw(11) << "" << "course. Now they want you to be the main course." << std::endl;

	bi.num_monsters = 3;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)CYCLOPS];
	bi.monsters[1] = g_Creatures[(int)CYCLOPS];
	bi.monsters[2] = g_Creatures[(int)CYCLOPS];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(11) << "" << "After you finished the cyclops off. You neglected" << std::endl;
	std::cout << std::setw(11) << "" << "to notice one cyclop that just went through one of the" << std::endl;
	std::cout << std::setw(11) << "" << "nearby doors to escape. You dash through the door" << std::endl;
	std::cout << std::setw(11) << "" << "to follow him, but it doesn\'t help you at all with" << std::endl;
	std::cout << std::setw(11) << "" << "him screaming. \'Help, " << plyr->GetName() << " is here.\'" << std::endl;
	std::cout << std::setw(11) << "" << "Before you could get to him a couple of mechazoids" << std::endl;
	std::cout << std::setw(11) << "" << "block your path to the cyclops. You guess they " << std::endl;
	std::cout << std::setw(11) << "" << "have a death wish. \'Time to play HUMAN!\'" << std::endl;

	bi.num_monsters = 2;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)MECHAZOID];
	bi.monsters[1] = g_Creatures[(int)MECHAZOID];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	std::cout << std::setw(11) << "" << "Those mechazoids were a little bit harder than" << std::endl;
	std::cout << std::setw(11) << "" << "the last time you fought one. There is a door to" << std::endl;
	std::cout << std::setw(11) << "" << "your left, where the mechazoids came out of." << std::endl << std::endl;

	std::cout << "(C)ontinue after cyclops, or (G)o into door?" << std::endl;

	keys = new unsigned char[5];
	keys[0] = 'c';
	keys[1] = 'C';
	keys[2] = 'g';
	keys[3] = 'G';
	keys[4] = 0;

	result = g_Game.KeyLoop(keys);
	delete [] keys;

	switch(result)
	{
	case 'g':
	case 'G':
		std::cout << std::setw(11) << "" << "Curiously you walk into the door. Not surprisingly" << std::endl;
		std::cout << std::setw(11) << "" << "you spot a few dwarfs working diligently to make" << std::endl;
		std::cout << std::setw(11) << "" << "mechazoids. You signal for the dwarfs escape, but" << std::endl;
		std::cout << std::setw(11) << "" << "they don\'t move. You soon find out why." << std::endl << std::endl;
		system("PAUSE");

		std::cout << std::setw(11) << "" << "After they lift their heads you find out that " << std::endl;
		std::cout << std::setw(11) << "" << "they aren't dwarfs they're mechazoids!" << std::endl;

		bi.num_monsters = 2;
		bi.monsters = new ENTITY[bi.num_monsters];
		bi.monsters[0] = g_Creatures[(int)MECHAZOID];
		bi.monsters[1] = g_Creatures[(int)MECHAZOID];
		resultf = g_Game.Battle(plyr,&bi);
		delete [] bi.monsters;

		if(resultf != COMPLETE)
			return resultf;

		std::cout << std::setw(11) << "" << "Three are left but one looks different. Huh?" << std::endl;

		bi.num_monsters = 2;
		bi.monsters = new ENTITY[bi.num_monsters];
		bi.monsters[0] = g_Creatures[(int)MECHAZOID];
		bi.monsters[1] = g_Creatures[(int)BIOMECH];
		resultf = g_Game.Battle(plyr,&bi);
		delete [] bi.monsters;

		if(resultf != COMPLETE)
			return resultf;

		std::cout << std::setw(11) << "" << "One dwarf still hasn\'t moved then he gets up" << std::endl;
		std::cout << std::setw(11) << "" << "walks slowly toward you and gives you......." << std::endl << std::endl;
		system("PAUSE");

		if(flags & FOUND_SPECIAL)
		{
			std::cout << std::setw(11) << "" << "Two max potions!" << std::endl;
		}
		else
		{
			std::cout << std::setw(11) << "" << "Two max potions and a weapon piece!" << std::endl;

			memset(&wItem,0,sizeof(wItem));
			wItem = g_Items[(int)HILT];
			wItem.istrength = 821;
			wItem.iquantity = 1;
			wItem.ilevel = 5;
			wItem.srunes.level[(int)EARTH] = 4;
			strcpy(wItem.szname,"Earth Crusher");

			if(plyr->GetInventory()->FindItem(wItem.szname))
			{
				plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
			}
			else
			{
				plyr->GetInventory()->End()->SetNext(&wItem);
			}

			flags |= FOUND_SPECIAL;
		}

		std::cout << std::setw(11) << "" << "\'Thank.. You...\' He runs frantically out of the room." << std::endl;

		if(plyr->GetInventory()->FindID(MAX_POTION))
		{
			plyr->GetInventory()->FindID(MAX_POTION)->GetItem()->iquantity += 2;
		}
		else
		{
			wItem = g_Items[(int)MAX_POTION];

			plyr->GetInventory()->End()->SetNext(&wItem);
			plyr->GetInventory()->FindID(MAX_POTION)->GetItem()->iquantity += 2;
		}
		break;

	case 'c':
	case 'C':
		std::cout << std::setw(11) << "" << "You decide to worry about the cyclop first." << std::endl;
		std::cout << std::setw(11) << "" << "So you continue to run after the cyclops. Well" << std::endl;
		std::cout << std::setw(11) << "" << "you find the cyclops, but he's not alone." << std::endl;

		if((g_Events[(int)MOUNTAIN].uiValue & OTHER_4) && !(flags & LVL_COMPLETE))
		{
			std::cout << std::setw(11) << "" << "Then Paristan makes his grand appearance," << std::endl;
			std::cout << std::setw(11) << "" << "and neither are you." << std::endl;

			Paristan.SetCurrentArmor(g_Items[(int)BREASTPLATE]);
			Paristan.SetCurrentWeapon(g_Items[(int)SCIMITAR]);
			Paristan.SetHPMax((long)(plyr->GetMaxHP() / 1.5));
			Paristan.SetHP(Paristan.GetMaxHP());
			Paristan.AddStrength(16);
			Paristan.AddDexterity(12);
			Paristan.AddIntelligence(15);

			bi.num_monsters = 5;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)MECHAZOID];
			bi.monsters[1] = g_Creatures[(int)CYCLOPS];
			bi.monsters[2] = g_Creatures[(int)MECHAZOID];
			bi.monsters[3] = g_Creatures[(int)CYCLOPS];
			bi.monsters[4] = g_Creatures[(int)ORC];
			resultf = g_Game.Battle(plyr,&bi,&Paristan);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			std::cout << std::setw(11) << "" << "Hey where did Paristan go?" << std::endl;
			std::cout << std::setw(11) << "" << "You wish he\'d quit doing that." << std::endl;
		}
		else
		{
			bi.num_monsters = 5;
			bi.monsters = new ENTITY[bi.num_monsters];
			bi.monsters[0] = g_Creatures[(int)MECHAZOID];
			bi.monsters[1] = g_Creatures[(int)CYCLOPS];
			bi.monsters[2] = g_Creatures[(int)MECHAZOID];
			bi.monsters[3] = g_Creatures[(int)CYCLOPS];
			bi.monsters[4] = g_Creatures[(int)ORC];
			resultf = g_Game.Battle(plyr,&bi);
			delete [] bi.monsters;

			if(resultf != COMPLETE)
				return resultf;

			if(flags & DID_SPECIAL)
			{
				memset(&wItem,0,sizeof(wItem));
				wItem = g_Items[(int)SWORD];
				wItem.istrength = 869;
				wItem.iquantity = 1;
				wItem.ilevel = 6;
				strcpy(wItem.szname,"Blood Letter");

				if(plyr->GetInventory()->FindItem(wItem.szname))
				{
					plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
				}
				else
				{
					plyr->GetInventory()->End()->SetNext(&wItem);
				}

				std::cout << std::setw(11) << "" << "Luckily, one of the creatures dropped a weapon piece." << std::endl;
				system("PAUSE");
				system("CLS");
				flags |= DID_SPECIAL;
			}
		}
		break;
	}
	
	system("CLS");

	std::cout << std::setw(11) << "" << "You hear a voice that sounds familiar." << std::endl;
	std::cout << std::setw(11) << "" << "\'Ha, Ha, Ha, You did well great grandson\'" << std::endl;
	std::cout << std::setw(11) << "" << "What are you talking about!" << std::endl;
	std::cout << std::setw(11) << "" << "\'Don't tell me you don't remember child.\' " << std::endl;
	std::cout << std::setw(11) << "" << "Who are you?" << std::endl;
	std::cout << std::setw(11) << "" << "\'I am Mordred! You my child are a very talented" << std::endl;
	std::cout << std::setw(11) << "" << "warrior. You have grown since our last battle.\'" << std::endl;
	std::cout << std::setw(11) << "" << "\'That's right you remember in the mountain!\'" << std::endl;
	std::cout << std::setw(11) << "" << "\'You Lord " << plyr->GetName() << " are the strongest of our family.\'" << std::endl;
	std::cout << std::setw(11) << "" << "Great Grandfather!?" << std::endl;
	std::cout << std::setw(11) << "" << "\'Yes now you remember.\'" << std::endl;
	std::cout << std::setw(11) << "" << "\'I killed my son and my grandson, because they" << std::endl;
	std::cout << std::setw(11) << "" << "were turning weak since you were born.\'" << std::endl;
	std::cout << std::setw(11) << "" << "\'For your sake I hope they trained you well!\'" << std::endl;

	std::cout << std::setw(11) << "" << "\'Now is time for your ultimate test! Have At YOU!\'" << std::endl;

	bi.num_monsters = 1;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)MORDRED];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	system("cls");

    std::cout << std::setw(11) << "" << "With your last bit of strength you deal the final" << std::endl;
    std::cout << std::setw(11) << "" << "blow. You mourn at the last remnants of your family." << std::endl;
    std::cout << std::setw(11) << "" << "Saying a few words of prayer you turn and recall on" << std::endl;
    std::cout << std::setw(11) << "" << "your adventure. Glad it\'s over you sigh and rest." << std::endl << std::endl;

	system("PAUSE");
	system("cls");

	std::cout << std::setw(11) << "" << "\'You have... not... defeated... me yet!!!\'" << std::endl;
    std::cout << std::setw(11) << "" << "With mordreds last bit of strength he chants" << std::endl;
    std::cout << std::setw(11) << "" << "a few words." << std::endl;
    std::cout << std::setw(11) << "" << "Suddenly he is overcomed with a magnific aura of magic." << std::endl;
    std::cout << std::setw(11) << "" << "\'Get ready for the ultimate challenge that not even" << std::endl;
    std::cout << std::setw(11) << "" << "your father and grandfather could overcome!!!!\'" << std::endl;
    std::cout << std::setw(11) << "" << "\'Get ready to die Lord " << plyr->GetName() << "!\'" << std::endl;

	bi.num_monsters = 1;
	bi.monsters = new ENTITY[bi.num_monsters];
	bi.monsters[0] = g_Creatures[(int)MORDRED_EVOLVED];
	resultf = g_Game.Battle(plyr,&bi);
	delete [] bi.monsters;

	if(resultf != COMPLETE)
		return resultf;

	system("PAUSE");
	system("cls");
	
    std::cout << std::setw(11) << "" << "\'Ahhhh! HoW did You DefEaTe Me! Nooooooooo!!!\'" << std::endl;
    std::cout << std::setw(11) << "" << "In a blinding flash of light he dissapears from your sight." << std::endl;
    std::cout << std::setw(11) << "" << "Releaved its over, you wonder, where is the exit?" << std::endl << std::endl;

	if(!(flags & LVL_COMPLETE))
	{
		memset(&wItem,0,sizeof(wItem));
		wItem = g_Items[(int)HAND_COVER];
		wItem.istrength = 753;
		wItem.iquantity = 1;
		wItem.ilevel = 4;
		wItem.srunes.level[(int)HOLY] = 8;
		strcpy(wItem.szname,"Blessed Destruction");

		if(plyr->GetInventory()->FindItem(wItem.szname))
		{
			plyr->GetInventory()->FindItem(wItem.szname)->GetItem()->iquantity++;
		}
		else
		{
			plyr->GetInventory()->End()->SetNext(&wItem);
		}

		std::cout << std::setw(11) << "" << "You\'ve received a special weapon piece!!!" << std::endl;
	}

	flags |= LVL_COMPLETE;
	plyr->SetHP(plyr->GetMaxHP());
	g_Events[(int)FORTRESS].uiValue = (LVLFLAGS)flags;
	system("PAUSE");
	system("CLS");

	return COMPLETE;
}