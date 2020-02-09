#include "player.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void SafeMT(INVENTORY *inv)
{
	char		 szname[26];
	unsigned int uiID;
	int			ilevel;
	FUNCTION	efunction;
	int			iquantity;
	int			istrength;
	RUNEDATA    srunes;
	LVLFUNCTION levelup;
	ITEM blank = {
		"",
		(ITEMID)0, 
		0, 
		(FUNCTION)0, 
		(int)0, 
		(int)0, 
		{ 0, 0, 0, 0, 0, 0, 0 }, 
		(LVLFUNCTION)0 
	};

	while(inv->GetPrevious() != 0)
	{
		inv = inv->GetPrevious();
		inv->DeleteNext();
	}

	inv->SetItem(blank);
}

unsigned int INVENTORY::GetSize()
{
	unsigned int c=0;
	INVENTORY *inv = this->End();

	while(inv != 0)
	{
		c++;
		inv = inv->GetPrevious();
	}
	
	return c;
}

void INVENTORY::DeletePrev()
{
	if(prev)
	{
		if(prev->GetPrevious() != 0)
			prev->GetPrevious()->SetNextPtr(this);

		prev = prev->GetPrevious();

		delete [] prev;
	}
}

void INVENTORY::DeleteNext()
{
	if(next)
	{
		if(next->GetNext() != 0)
			next->GetNext()->SetPreviousPtr(this);

		next = next->GetNext();

		delete [] next;
	}
}

INVENTORY *INVENTORY::GetNext() 
{
	return next;
}

INVENTORY *INVENTORY::GetPrevious() 
{
	return prev;
}

INVENTORY *INVENTORY::FindItem(char *name) 
{
	INVENTORY *curItem;

	curItem = Begining();

	do
	{
		if(!strcmpi(curItem->GetItem()->szname,name))
		{
			return curItem;
		}

		curItem = curItem->GetNext();
	}
	while(curItem != 0);

	return NULL;
}

INVENTORY *INVENTORY::FindID(ITEMID id) 
{
	INVENTORY *curItem;

	curItem = Begining();

	do
	{
		if(curItem->GetItem()->uiID == (unsigned int)id)
		{
			return curItem;
		}

		curItem = curItem->GetNext();
	}
	while(curItem != 0);

	return NULL;
}

INVENTORY *INVENTORY::Begining()
{
	INVENTORY *curInv = this;

	while(curInv->GetPrevious() != 0)
	{
		curInv = curInv->GetPrevious();
	}

	return curInv;
}

INVENTORY *INVENTORY::End()
{
	INVENTORY *curInv = this;

	while(curInv->GetNext() != 0)
	{
		curInv = curInv->GetNext();
	}

	return curInv;
}

void INVENTORY::SetNext(ITEM *i)
{
	INVENTORY *ptr = new INVENTORY;
	ptr->SetItem(*i);

	if(next != 0)
	{
		ptr->SetNextPtr(next);
		next->SetPreviousPtr(ptr);
	}

	next = ptr;
	ptr->SetPreviousPtr(this);
}

void INVENTORY::SetPrevious(ITEM *i)
{
	INVENTORY *ptr = new INVENTORY;
	ptr->SetItem(*i);

	if(prev != 0)
	{
		ptr->SetPreviousPtr(prev);
		prev->SetNextPtr(ptr);
	}

	prev = ptr;
	ptr->SetNextPtr(this);
}

void INVENTORY::SetPreviousPtr(INVENTORY *ptr)
{
	prev = ptr;
}

void INVENTORY::SetNextPtr(INVENTORY *ptr)
{
	next = ptr;
}

void INVENTORY::SetItem(ITEM i)
{
	sdata = i;
}

void PLAYER::SetInventory(INVENTORY *ptr)
{
	sinventory.SetPreviousPtr(0);
	sinventory.SetNextPtr(ptr->GetNext());
	sinventory.SetItem(*ptr->GetItem());
}

void PLAYER::AddExperience(long val)
{
	long overflow = 0;
	lexperience += val;

	while(lexperience > ((long)ceil(100+(pow(10,ilevel)/pow(2,ilevel+1)))))
	{
		overflow = lexperience - ((long)ceil(100+(pow(10,ilevel)/pow(2,ilevel+1))));
		AddLevel(1);
		lexperience = overflow;
	}
}

void PLAYER::AddHP(long val)
{
	lhp += val;

	if(lhp > lhpmax)
		lhp = lhpmax;
}

void PLAYER::AddHPMax(long val)
{
  lhpmax += val;
}

void PLAYER::AddProficiency(long val)
{
  lweaponproficiency += val;

  if(lweaponproficiency >= 100)
  {
	  weapon.ilevel++;

	  if(weapon.levelup)
		weapon.levelup(&weapon);

	  lweaponproficiency = 0;
  }
}

void PLAYER::AddVitality(int val)
{
  ivitality += val;
  srand((unsigned int)time(NULL));
  lhpmax += (rand()%(int)ceil(sqrt(istrength*ivitality)) + (ivitality*2));
}

void PLAYER::AddIntelligence(int val)
{
  iintelligence += val;
}

void PLAYER::AddStrength(int val)
{
  istrength += val;
}

void PLAYER::AddDexterity(int val)
{
  idexterity += val;
}

void PLAYER::AddLevel(int val)
{
	srand((unsigned int)time(NULL));
	ilevel += val;
	idexterity += rand()%4+2;
	AddVitality(rand()%4+2);
	istrength += rand()%4+2;
	iintelligence += rand()%4+2;
	std::cout << "You've gained a level!" << std::endl;
}

void PLAYER::SetExperience(long exp)
{
	lexperience = exp;
}

void PLAYER::SetName(char *name)
{
	strcpy(szName,name);
}

void PLAYER::SetFile(char *name)
{
	strcpy(szFile,name);
}

void PLAYER::SetDexterity(int dex)
{
	idexterity = dex;
}

void PLAYER::SetHP(long hp)
{
	lhp = hp;
}

void PLAYER::SetHPMax(long hp)
{
	lhpmax = hp;
}

void PLAYER::SetIntelligence(int intelligence)
{
	iintelligence = intelligence;
}

void PLAYER::SetLevel(int lvl)
{
	ilevel = lvl;
}

void PLAYER::SetProficiency(int prof)
{
	lweaponproficiency = prof;
}

void PLAYER::SetStrength(int str)
{
	istrength = str;
}

void PLAYER::SetVitality(int vit)
{
	ivitality = vit;
}

void PLAYER::SetCurrentArmor(ITEM item)
{
	armor = item;
}

void PLAYER::SetCurrentWeapon(ITEM item)
{
	weapon = item;
}