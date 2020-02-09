#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iomanip>
#include "system.h"
#include "game.h"
#include "events.h"
#include "player.h"
#include "filesys.h"

#define USEABLE_ITEMS (HEAL|HEAL_ALL)

unsigned int GAME::KeyLoop(unsigned char *keys)
{
	bool exit=false;
	unsigned int key=0,x;
	do{
		key = getch();

		for(x=0; x < strlen((char*)keys); x++)
			if(keys[x] == key)
				exit = true;

	}while(!exit);

	return key;
}

// set active weapon and armor based on id given
bool GAME::LoadGame(PLAYER* plyr,EVENTSNODE g_Events [])
{
	system("CLS");
	char file[51];
	FILEOBJECT *data;
	ITEM i;
	INVENTORY *inv;
	int x;

	std::cout << "What is the name of your save file?" << std::endl;

	do{
		std::cin.getline(file, sizeof(file), '\n');
	}while(!strlen(file));

	if(data = ::LoadFile(file))
	{
		plyr->SetDexterity(data->body.idexterity);
		plyr->SetStrength(data->body.istrength);
		plyr->SetVitality(data->body.ivitality);
		plyr->SetHPMax(data->body.lhpmax);
		plyr->SetHP(data->body.lhpmax);
		plyr->SetIntelligence(data->body.iintelligence);
		plyr->SetExperience(data->body.lexperience);
		plyr->SetLevel(data->body.ilevel);
		plyr->SetName(data->body.name);
		plyr->SetFile(data->body.file);
		plyr->SetProficiency(data->body.lweaponproficiency);

		i = FItoI(data->footer.items_data[0]);
		plyr->GetInventory()->SetItem(i);
		inv = plyr->GetInventory();

		for(x=1; x < data->footer.sizeof_item_array; x++)
		{
			i = FItoI(data->footer.items_data[x]);
			inv->SetNext(&i);
			inv = inv->GetNext();
		}

		for(x=0; x < data->footer.sizeof_event_array; x++)
		{
			g_Events[x] = FEtoE(data->footer.events_data[x]);
		}

		plyr->SetCurrentArmor(*plyr->GetInventory()->FindID((ITEMID)data->body.armorID)->GetItem());
		plyr->SetCurrentWeapon(*plyr->GetInventory()->FindID((ITEMID)data->body.weaponID)->GetItem());
		delete [] data;

		return true;
	}

	return false;
}

bool GAME::SaveGame(PLAYER* plyr,EVENTSNODE g_Events [])
{
	system("CLS");
	char file[50];
	FILEOBJECT data;
	FILEITEM fi;
	INVENTORY *inv;
	int x,c=0;

	strcpy(file,plyr->GetFile());

	data.body.idexterity = plyr->GetDexterity();
	data.body.istrength = plyr->GetStrength();
	data.body.iintelligence = plyr->GetIntelligence();
	data.body.ilevel = plyr->GetLevel();
	data.body.ivitality = plyr->GetVitality();
	data.body.lexperience = plyr->GetExperience();
	data.body.lhpmax = plyr->GetMaxHP();
	data.body.lweaponproficiency = plyr->GetProficiency();
	strcpy(data.body.name,plyr->GetName());
	strcpy(data.body.file,plyr->GetFile());

	data.body.armorID = plyr->GetCurrentArmor().uiID;
	data.body.weaponID = plyr->GetCurrentWeapon().uiID;

	data.footer.items_data = new FILEITEM[plyr->GetInventory()->GetSize()];
	inv = plyr->GetInventory()->End();

	while(inv != 0)
	{
		fi = ItoFI(*(inv->GetItem()));
		data.footer.items_data[c] = fi;
		c++;
		inv = inv->GetPrevious();
	}
	data.footer.sizeof_item_array = c;

	data.footer.events_data = new FILEEVENT[5];
	for(x=0; x < 5; x++)
	{
		data.footer.events_data[x] = EtoFE(g_Events[x]);
	}

	data.footer.sizeof_event_array = 5;

	::SaveFile(file,data);

	system("CLS");
	std::cout << "Game succesfully saved under " << file << "!" << std::endl << std::endl;
	system("PAUSE");
	std::cout << std::endl;

	system("CLS");

	return true;
}

ENDFLAG GAME::Battle(PLAYER *plyr,BATTLEINFO *bi,PLAYER *npc,bool prompt)
{
	int x=0,y=0,c=0;
	int damage=0;
	long turns=0;
	bool first=false;
	bool again=false;
	ITEM *item;
	INVENTORY *inv;
	unsigned int result=0;
	unsigned char keys[] = {'F','f','I','i',0};
	unsigned char yesno[] = {'Y','N','n','y',0};
	unsigned char *creatures;

	srand((unsigned int)time(NULL));

	for(x=0; x < bi->num_monsters; x++)
	{
		std::cout << bi->monsters[x].threat << std::endl;
	}

	x=rand()%bi->num_monsters;
	first = plyr->GetDexterity() >= (int)floor(bi->monsters[x].fdexteritymultiplier * 
		(rand()%bi->monsters[x].idexterity) + bi->monsters[x].idexteritybase);

	while((bi->num_monsters > 0) && (plyr->GetHP() > 0))
	{
		if(!first)
		{
			for(x=0; x < bi->num_monsters; x++)
			{
				damage = (int)floor(bi->monsters[x].fdamagemultiplier * 
					(rand()%bi->monsters[x].idamage) + 
					bi->monsters[x].idamagebase);

				if(npc)
				{
					if((rand() % 3) < 1)
					{
						switch(rand() % 2)
						{
						case 0:
							damage -= (int)floor((rand()%npc->GetCurrentArmor().istrength) +
								npc->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt " 
								 << damage << " to " << npc->GetName() << "!" << std::endl;

							npc->AddHP(-damage);
							break;

						case 1:
							damage -= (int)floor((rand()%plyr->GetCurrentArmor().istrength) +
								plyr->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt " 
								 << damage << " to you!" << std::endl;

							plyr->AddHP(-damage);
							break;
						}
					}
					else
					{
						if(plyr->GetHP() > npc->GetHP())
						{
							damage -= (int)floor((rand()%npc->GetCurrentArmor().istrength) +
								npc->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt " 
								 << damage << " to " << npc->GetName() << "!" << std::endl;

							npc->AddHP(-damage);
						}
						else
						{
							damage -= (int)floor((rand()%plyr->GetCurrentArmor().istrength) +
								plyr->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt " 
								 << damage << " to you!" << std::endl;

							plyr->AddHP(-damage);
						}
					}
				}
				else
				{
					damage -= (int)floor((rand()%plyr->GetCurrentArmor().istrength) +
						plyr->GetCurrentArmor().ilevel);

					if(damage <= 0)
						damage = 1;

					std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt " 
						 << damage << " to you!" << std::endl;

					plyr->AddHP(-damage);
				}
			}

			if(plyr->GetHP() <= 0)
				break;

			if(npc)
			{
				if(npc->GetHP() <= 0)
				{
					break;
				}
			}

			std::cout << "Your life ratio is " << plyr->GetHP() << '/' << plyr->GetMaxHP()
				 << std::endl << "(F)ight or (I)tem?" << std::endl;

			do{
				result = g_Game.KeyLoop(keys);
				switch(result)
				{
				case 'F': 
				case 'f':
					again = false;
					damage = (rand()%plyr->GetStrength()) + 
						plyr->GetCurrentWeapon().ilevel +
						plyr->GetCurrentWeapon().istrength;

					if(bi->num_monsters > 1)
					{
						creatures = new unsigned char[bi->num_monsters];
						for(y=0; y < bi->num_monsters; y++)
							creatures[y] = '0' + y + 1; // creatures[y] = 2 + y;

						std::cout << "Which creature? ";
						for(y=0; y < bi->num_monsters; y++)
							std::cout << bi->monsters[y].szName << " (" << y+1 << ") ";
						std::cout << std::endl;

						result = (g_Game.KeyLoop(creatures) - '0' - 1);

						delete [] creatures;
					}
					else
					{
						result = 0;
					}

					if(damage <= 0)
						damage = 1;

					if(bi->monsters[result].eweakness != NONE)
					{
						if(plyr->GetCurrentWeapon().srunes.level[bi->monsters[result].eweakness])
						{
							std::cout << std::setw(5) << "" << "The ";
							switch(bi->monsters[result].eweakness)
							{
							case ICE:
								std::cout << "Ice Rune";
								break;
							case FIRE:
								std::cout << "Fire Rune";
								break;
							case THUNDER:
								std::cout << "Thunder Rune";
								break;
							case HOLY:
								std::cout << "Holy Rune";
								break;
							case DARK:
								std::cout << "Dark Rune";
								break;
							case WIND:
								std::cout << "Wind Rune";
								break;
							case EARTH:
								std::cout << "Earth Rune";
								break;
							}

							std::cout << " glowed!" << std::endl;
							damage += plyr->GetCurrentWeapon().srunes.level[bi->monsters[result].eweakness];
						}
					}

					bi->monsters[result].lhp -= damage;
				
					std::cout << "You dealt " << damage << " to " 
						 << bi->monsters[result].szName << '!' << std::endl;

					if(bi->monsters[result].lhp <= 0)
					{
						std::cout << std::endl << bi->monsters[result].szName
				             << " fell under your " << plyr->GetCurrentWeapon().szname << '.' << std::endl;
						y=(rand()%bi->monsters[result].goldbase)+5;
						c=((rand()%bi->monsters[result].lexperience)*.5) +  bi->monsters[result].lexperience*.50;
						std::cout << std::setw(5) << "" << "You gained " << y << " gold and " 
							 << c << " experience!"<< std::endl;
						plyr->GetInventory()->FindItem("GOLD")->GetItem()->iquantity+=y;

						// Add random item from treasure class
						y = (rand()%(bi->monsters[result].treasure+1))+11;
						if(y != 11)
						{
							if(inv = plyr->GetInventory()->FindItem(g_Items[y].szname))
							{
								inv->GetItem()->iquantity++;
							}
							else
							{
								item = new ITEM;
								*item = g_Items[y];
								item->iquantity=1;
								plyr->GetInventory()->End()->SetNext(item);
							}

							std::cout << std::setw(5) << "" << "You've acquired a " << g_Items[y].szname
								 << '.' << std::endl;
						}

						plyr->AddExperience(bi->monsters[result].lexperience);
					
						if(result != (bi->num_monsters-1))				
							swap(bi->monsters[result], bi->monsters[bi->num_monsters-1]);

						bi->num_monsters--;
					}

					break;
				case 'I': // 'I'
				case 'i':
					y=0;
					inv = plyr->GetInventory()->Begining();
					creatures = new unsigned char[50];

					while(inv)
					{
						if(inv->GetItem()->iquantity && (inv->GetItem()->efunction&(USEABLE_ITEMS)))
						{
							std::cout << '(' << y + 1 << ") " << inv->GetItem()->szname << 'x' << inv->GetItem()->iquantity << std::endl;
							creatures[y] = '0' + y + 1;
							y++;
						}
						inv = inv->GetNext();
					}

					if(!y)
					{
						std::cout << "No useable items in inventory!" << std::endl;
						again = true;
						break;
					}

					y=0;
					result = (g_Game.KeyLoop(creatures) - '0');
					inv = plyr->GetInventory()->Begining();
	
					do
					{
						if(inv->GetItem()->iquantity && (inv->GetItem()->efunction&(USEABLE_ITEMS)))
						{
							y++;
						}

						if(y < result)
							inv = inv->GetNext();	
					}
					while(y < result);

					switch(inv->GetItem()->efunction)
					{
					case HEAL_ALL:
						y = (plyr->GetMaxHP() - plyr->GetHP());
						plyr->AddHP(y);
						std::cout << std::setw(5) << "" << "You recovered " << y << " hit points!" << std::endl;
						inv->GetItem()->iquantity--;
						break;

					case HEAL:
						y = ((rand()%inv->GetItem()->ilevel) * ((rand()%2)+1)) + inv->GetItem()->istrength;

						if( y > plyr->GetMaxHP() )
							y = plyr->GetMaxHP();

						plyr->AddHP(y);
						std::cout << std::setw(5) << "" << "You recovered " << y << " hit points!" << std::endl;
						inv->GetItem()->iquantity--;
						break;
					}

					delete [] creatures;
					break;
				}
			}while(again);

			if(!bi->num_monsters)
				break;
		
			if(npc)
			{
				if(npc->GetHP() < (npc->GetMaxHP()/3))
				{
					std::cout << std::setw(5) << "" << npc->GetName() << " used a potion on himself!" << std::endl;
					y = ((rand()%10) * ((rand()%2)+1)) + 10;

					npc->AddHP(y);
					std::cout << std::setw(5) << "" << npc->GetName() << " recovered " << y << " hit points!" << std::endl;

					// heal himself
				}
				else if(plyr->GetHP() < (plyr->GetMaxHP()/3))
				{
					std::cout << std::setw(5) << "" << npc->GetName() << " used a potion on you!" << std::endl;

					y = ((rand()%10) * ((rand()%2)+1)) + 10;
					plyr->AddHP(y);

					std::cout << std::setw(5) << "" << "You recovered " << y << " hit points!" << std::endl;

					// heal
				}
				else
				{
					// damage monster
					result = rand() % bi->num_monsters;

					damage = (rand()%plyr->GetStrength()) + 
						plyr->GetCurrentWeapon().ilevel +
						plyr->GetCurrentWeapon().istrength;

					bi->monsters[result].lhp -= damage;
	
					std::cout << npc->GetName() << " dealt " << damage << " to "
						<< bi->monsters[result].szName << '!' << std::endl;

					if(bi->monsters[result].lhp <= 0)
					{
						std::cout << std::endl << bi->monsters[result].szName
							<< " fell under " << npc->GetName() << "'s " << npc->GetCurrentWeapon().szname << '.' << std::endl;

						y=(rand()%bi->monsters[result].goldbase)+5;
						c=((rand()%bi->monsters[result].lexperience)*.5) +  bi->monsters[result].lexperience*.5;

						std::cout << std::setw(5) << "" << "You gained " << y << " gold and "
							<< c << " experience!" << std::endl;

						plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity+=y;

						// Add random item from treasure class
						y = (rand()%(bi->monsters[result].treasure+1))+11;

						if(y != 11)
						{
							if(inv = plyr->GetInventory()->FindItem(g_Items[y].szname))
							{
								inv->GetItem()->iquantity++;
							}
							else
							{
								item = new ITEM;
								*item = g_Items[y];
								item->iquantity=1;
								plyr->GetInventory()->End()->SetNext(item);
							}

							std::cout << std::setw(5) << "" << "You've acquired a " << g_Items[y].szname
								<< '.' << std::endl;
						}

						plyr->AddExperience(bi->monsters[result].lexperience);

						if(result != (bi->num_monsters-1))
							swap(bi->monsters[result], bi->monsters[bi->num_monsters-1]);

						bi->num_monsters--;
					}
				}
			}
		}
		else
		{
			std::cout << "Your life ratio is " << plyr->GetHP() << '/' << plyr->GetMaxHP()
				<< std::endl << "(F)ight or (I)tem?" << std::endl;
			
			do
			{
				result = g_Game.KeyLoop(keys);
				switch(result)
				{
				case 'F'://33: // 'F'
				case 'f':
					again = false;
					damage = (rand()%plyr->GetStrength()) + 
						(rand()%plyr->GetCurrentWeapon().istrength) +
						plyr->GetCurrentWeapon().ilevel;

					if(bi->num_monsters > 1)
					{
						creatures = new unsigned char[bi->num_monsters];
						for(y=0; y < bi->num_monsters; y++)
							creatures[y] = '0' + y + 1; // 2 + y;

						std::cout << "Which creature? ";
						for(y=0; y < bi->num_monsters; y++)
							std::cout << bi->monsters[y].szName << " (" << y + 1 << ") ";
						std::cout << std::endl;

						result = (g_Game.KeyLoop(creatures) - '0' - 1);
						delete [] creatures;
					}
					else
					{
						result = 0;
					}

					if(damage <= 0)
						damage = 1;

					if(bi->monsters[result].eweakness != NONE)
					{
						if(plyr->GetCurrentWeapon().srunes.level[bi->monsters[result].eweakness])
						{
							std::cout << std::setw(5) << "" << "The ";
							switch(bi->monsters[result].eweakness)
							{
							case ICE:
								std::cout << "Ice Rune";
								break;
							case FIRE:
								std::cout << "Fire Rune";
								break;
							case THUNDER:
								std::cout << "Thunder Rune";
								break;
							case HOLY:
								std::cout << "Holy Rune";
								break;
							case DARK:
								std::cout << "Dark Rune";
								break;
							case WIND:
								std::cout << "Wind Rune";
								break;
							case EARTH:
								std::cout << "Earth Rune";
								break;
							}

							std::cout << " glowed!" << std::endl;
							damage += plyr->GetCurrentWeapon().srunes.level[bi->monsters[result].eweakness];
						}
					}

					bi->monsters[result].lhp -= damage;
	
					std::cout << "You dealt " << damage << " to "
						<< bi->monsters[result].szName << '!' << std::endl;

					if(bi->monsters[result].lhp <= 0)
					{
						std::cout << std::endl << bi->monsters[result].szName
							<< " fell under your " << plyr->GetCurrentWeapon().szname << '.' << std::endl;
						y=(rand()%bi->monsters[result].goldbase)+5;
						c=((rand()%bi->monsters[result].lexperience)*.5) +  bi->monsters[result].lexperience*.5;
						std::cout << std::setw(5) << "" << "You gained " << y << " gold and "
							<< c << " experience!" << std::endl;

						plyr->AddExperience(c);
						plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity+=y;

						// Add random item from treasure class
						y = (rand()%(bi->monsters[result].treasure+1))+11;
						if(y != 11)
						{
							if(inv = plyr->GetInventory()->FindItem(g_Items[y].szname))
							{
								inv->GetItem()->iquantity++;
							}
							else
							{
								item = new ITEM;
								*item = g_Items[y];
								item->iquantity=1;
								plyr->GetInventory()->End()->SetNext(item);
							}

							std::cout << std::setw(5) << "" << "You've acquired a " << g_Items[y].szname
								<< '.' << std::endl;
						}

						if((rand()%4) == 0)
						{
							if(inv = plyr->GetInventory()->FindID(POTION))
							{
								inv->GetItem()->iquantity++;
							}
							else
							{
								item = new ITEM;
								*item = g_Items[(int)POTION];
								item->iquantity=1;
								plyr->GetInventory()->End()->SetNext(item);
							}

							std::cout << std::setw(5) << "" << "You've acquired a " << g_Items[(int)POTION].szname
								<< '.' << std::endl;
						}

						if(result != (bi->num_monsters-1))
							swap(bi->monsters[result], bi->monsters[bi->num_monsters-1]);

						bi->num_monsters--;
					}
					break;

				case 'I':
				case 'i':// 'I'
					y=0;
					inv = plyr->GetInventory()->Begining();
					creatures = new unsigned char[50];
	
					while(inv)
					{
						if(inv->GetItem()->iquantity && (inv->GetItem()->efunction&(USEABLE_ITEMS)))
						{
							std::cout << '(' << y + 1 << ") " << inv->GetItem()->szname << 'x' << inv->GetItem()->iquantity << std::endl;
							creatures[y] = '0' + y + 1;
							y++;
						}
						inv = inv->GetNext();
					}

					if(!y)
					{
						std::cout << "No useable items in inventory!" << std::endl;
						again = true;
						break;
					}

					y=0;
					result = (g_Game.KeyLoop(creatures) - '0');
					inv = plyr->GetInventory()->Begining();
	
					do
					{
						if(inv->GetItem()->iquantity && (inv->GetItem()->efunction&(USEABLE_ITEMS)))
						{
							y++;
						}

						if(y < result)
							inv = inv->GetNext();	
					}
					while(y < result);

					switch(inv->GetItem()->efunction)
					{
					case HEAL_ALL:
						y = (plyr->GetMaxHP() - plyr->GetHP());
						plyr->AddHP(y);
						std::cout << std::setw(5) << "" << "You recovered " << y << " hit points!" << std::endl;
						inv->GetItem()->iquantity--;
						break;

					case HEAL:
						y = ((rand()%inv->GetItem()->ilevel) * ((rand()%2)+1)) + inv->GetItem()->istrength;

						if( y > plyr->GetMaxHP() )
							y = plyr->GetMaxHP();

						plyr->AddHP(y);
						std::cout << std::setw(5) << "" << "You recovered " << y << " hit points!" << std::endl;
						inv->GetItem()->iquantity--;
						break;
					}
					delete [] creatures;
					break;
				}
			}while(again);

			if(!bi->num_monsters)
				break;
	
			// npc does damage or heal
			if(npc)
			{
				if(npc->GetHP() < (npc->GetMaxHP()/3))
				{
					std::cout << std::setw(5) << "" << npc->GetName() << " used a potion on himself!" << std::endl;
					y = ((rand()%10) * ((rand()%2)+1)) + 10;

					npc->AddHP(y);
					std::cout << std::setw(5) << "" << npc->GetName() << " recovered " << y << " hit points!" << std::endl;

					// heal himself
				}
				else if(plyr->GetHP() < (plyr->GetMaxHP()/3))
				{
					std::cout << std::setw(5) << "" << npc->GetName() << " used a potion on you!" << std::endl;

					y = ((rand()%10) * ((rand()%2)+1)) + 10;
					plyr->AddHP(y);

					std::cout << std::setw(5) << "" << "You recovered " << y << " hit points!" << std::endl;

					// heal
				}
				else
				{
					// damage monster
					result = rand() % bi->num_monsters;

					damage = (rand()%plyr->GetStrength()) + 
						plyr->GetCurrentWeapon().ilevel +
						plyr->GetCurrentWeapon().istrength;

					bi->monsters[result].lhp -= damage;
	
					std::cout << npc->GetName() << " dealt " << damage << " to "
						<< bi->monsters[result].szName << '!' << std::endl;

					if(bi->monsters[result].lhp <= 0)
					{
						std::cout << std::endl << bi->monsters[result].szName
							<< " fell under " << npc->GetName() << "'s " << npc->GetCurrentWeapon().szname << '.' << std::endl;

						y=(rand()%bi->monsters[result].goldbase)+5;
						c=((rand()%bi->monsters[result].lexperience)*.5) +  bi->monsters[result].lexperience*.5;

						std::cout << std::setw(5) << "" << "You gained " << y << " gold and "
							<< c << " experience!" << std::endl;

						plyr->GetInventory()->FindID(GOLD)->GetItem()->iquantity+=y;
						plyr->AddExperience(c);

						// Add random item from treasure class
						y = (rand()%(bi->monsters[result].treasure+1))+11;

						if(y != 11)
						{
							if(inv = plyr->GetInventory()->FindItem(g_Items[y].szname))
							{
								inv->GetItem()->iquantity++;
							}
							else
							{
								item = new ITEM;
								*item = g_Items[y];
								item->iquantity = 1;
								plyr->GetInventory()->End()->SetNext(item);
							}

							std::cout << std::setw(5) << "" << "You've acquired a " << g_Items[y].szname
								<< '.' << std::endl;
						}

						if(result != (bi->num_monsters-1))
							swap(bi->monsters[result], bi->monsters[bi->num_monsters-1]);

						bi->num_monsters--;
					}
				}
			}
					
			if(!bi->num_monsters)
				break;
	
			for(x=0; x < bi->num_monsters; x++)
			{
				damage = (int)floor(bi->monsters[x].fdamagemultiplier * 
					(rand()%bi->monsters[x].idamage) + 
					bi->monsters[x].idamagebase);

				if(npc)
				{
					if((rand() % 3) < 1)
					{
						switch(rand() % 2)
						{
						case 0:
							damage -= (int)floor((rand()%npc->GetCurrentArmor().istrength) +
								npc->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt "
								<< damage << " to " << npc->GetName() << "!" << std::endl;

							npc->AddHP(-damage);
							break;

						case 1:
							damage -= (int)floor((rand()%plyr->GetCurrentArmor().istrength) +
								plyr->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt "
								<< damage << " to you!" << std::endl;

							plyr->AddHP(-damage);
							break;
						}
					}
					else
					{
						if(plyr->GetHP() > npc->GetHP())
						{
							damage -= (int)floor((rand()%npc->GetCurrentArmor().istrength) +
								npc->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt "
								<< damage << " to " << npc->GetName() << "!" << std::endl;

							npc->AddHP(-damage);
						}
						else
						{
							damage -= (int)floor((rand()%plyr->GetCurrentArmor().istrength) +
								plyr->GetCurrentArmor().ilevel);

							if(damage <= 0)
								damage = 1;

							std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt "
								<< damage << " to you!" << std::endl;

							plyr->AddHP(-damage);
						}
					}
				}
				else
				{
					damage -= (int)floor((rand()%plyr->GetCurrentArmor().istrength) +
						plyr->GetCurrentArmor().ilevel);

					if(damage <= 0)
						damage = 1;

					std::cout << std::setw(5) << "" << bi->monsters[x].szName << " dealt "
						<< damage << " to you!" << std::endl;

					plyr->AddHP(-damage);
				}
			}

			if(plyr->GetHP() <= 0)
				break;

			if(npc)
			{
				if(npc->GetHP() <= 0)
				{
					break;
				}
			}
		}
		
		turns++;
	}

	if(bi->num_monsters == 0)
	{
		plyr->AddProficiency((long)ceil(turns*2));

		return COMPLETE;
	}
	else
	{
		if(npc)
		{
			if(npc->GetHP() <= 0)
			{
				std::cout << bi->monsters[(x - 1)].szName << " struck down " << npc->GetName() << std::endl
					<< " with a heafty blow!" << std::endl;
			}
			else
			{
				std::cout << bi->monsters[(x - 1)].szName << " struck you down"
					<< " with a heafty blow!" << std::endl;
			}
		}
		else
		{
			std::cout << bi->monsters[(x - 1)].szName << " struck you down"
				<< " with a heafty blow!" << std::endl;
		}

		if(prompt)
		{
			std::cout << std::endl << "Load Game? (Y/N)" << std::endl;

			result = g_Game.KeyLoop(yesno);
			switch(result)
			{
			case 'Y'://21: // 'Y'
			case 'y':
				return QUIT;
				break;
			case 'N'://49: // 'N'
			case 'n':
				return CONTINUE;
				break;
			}
		}

		return CONTINUE;
	}

	return COMPLETE;
}