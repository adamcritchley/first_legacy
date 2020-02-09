#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "item.h"

class INVENTORY{
public:
	INVENTORY():next(0),prev(0),sdata(){}
	INVENTORY *GetNext();
	INVENTORY *GetPrevious();
	void SetNextPtr(INVENTORY*);
	void SetPreviousPtr(INVENTORY*);
	void SetNext(ITEM *);
	void SetPrevious(ITEM *);
	void SetItem(ITEM);
	ITEM *GetItem() {return &sdata;}
	INVENTORY *FindItem(char *);
	INVENTORY *FindID(ITEMID);
	INVENTORY *Begining();
	INVENTORY *End();
	unsigned int GetSize();
	void DeleteNext();
	void DeletePrev();

private:
	INVENTORY	*next;
	INVENTORY	*prev;
	ITEM		sdata;
};

class PLAYER{
public:
	char *		GetFile() { return szFile; }
	char *		GetName() { return szName;}
	INVENTORY	*GetInventory() {return &sinventory;}
	long		GetHP() {return lhp;}
	long		GetMaxHP() {return lhpmax;}
	int			GetDexterity() {return idexterity;}
	int			GetVitality() {return ivitality;}
	int			GetStrength() {return istrength;}
	int			GetLevel() {return ilevel;}
	int			GetIntelligence() {return iintelligence;}
	long		GetProficiency() {return lweaponproficiency;}
	long		GetExperience() {return lexperience;}
	ITEM		GetCurrentArmor() {return armor;}
	ITEM		GetCurrentWeapon() {return weapon;}

	void		SetFile(char *);
	void		SetName(char *);
	void		SetCurrentArmor(ITEM);
	void		SetCurrentWeapon(ITEM);
	void		SetVitality(int);
	void		SetIntelligence(int);
	void		SetStrength(int);
	void		SetDexterity(int);
	void		SetLevel(int);
	void		SetHP(long);
	void		SetHPMax(long);
	void		SetProficiency(int);
	void		SetExperience(long);
	void		SetInventory(INVENTORY *);

	void		AddVitality(int);
	void		AddIntelligence(int);
	void		AddStrength(int);
	void		AddDexterity(int);
	void		AddLevel(int);
	void		AddHP(long);
	void		AddHPMax(long);
	void		AddProficiency(long);
	void		AddExperience(long);

private:
	char		szName[20];
	char		szFile[50];
	INVENTORY	sinventory;
	long lhp;
	long lhpmax;
	int  iintelligence;
	int  idexterity;
	int  ivitality;
	int  istrength;
	int  ilevel;
	long lexperience;
	long lweaponproficiency;
	ITEM armor;
	ITEM weapon;
};

void SafeMT(INVENTORY*);

#endif