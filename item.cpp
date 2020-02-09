#include "item.h"
#include <iostream>
#include <string.h>

void Axe(ITEM *item)
{
	switch(item->ilevel)
	{
	case 5:
		std::cout << std::endl << "Your Axe was transformed into a Halbard!" << std::endl;
		strcpy(item->szname,"Halbard");
		item->istrength = 5;
		break;
	}
}

void Blade(ITEM *item)
{
	switch(item->ilevel)
	{
	case 5:
		std::cout << std::endl << "Your Blade transformed into a Bastard Sword!" << std::endl;
		strcpy(item->szname,"Bastard Sword");
		item->istrength = 4;
		break;

	case 8:
		std::cout << std::endl << "Your Bastard Sword transformed into a Longsword!" << std::endl;
		strcpy(item->szname,"Long Sword");
		item->istrength = 8;
		break;
	}
}

void Broadsword(ITEM *item)
{
	switch(item->ilevel)
	{
	case 6:
		std::cout << std::endl << "Your Broadsword transformed into a Heavy Edge!" << std::endl;
		strcpy(item->szname,"Heavy Edge");
		item->istrength = 5;
		break;

	case 10:
		std::cout << std::endl << "Your Heavy Edge transformed into Zweihander!" << std::endl;
		strcpy(item->szname,"Zweihander");
		item->istrength = 14;
		break;
	}
}

void Club(ITEM *item)
{
	switch(item->ilevel)
	{
	case 3:
		std::cout << std::endl << "Your Club transformed into a Morning Star!" << std::endl;
		strcpy(item->szname,"Morning Star");
		item->istrength = 5;
		break;
	case 6:
		std::cout << std::endl << "Your Morning Star transformed into a Spiked Hammer!" << std::endl;
		strcpy(item->szname,"Spiked Mace");
		item->istrength = 7;
		break;
	case 10:
		std::cout << std::endl << "Your Spiked Mace transformed into a Twin Flail!" << std::endl;
		strcpy(item->szname,"Twin Flail");
		item->istrength = 9;
		break;
	}
}

void Knife(ITEM *item)
{
	switch(item->ilevel)
	{
	case 6:
		std::cout << std::endl << "Your Knife transformed into a Dagger!" << std::endl;
		strcpy(item->szname,"Dagger");
		item->istrength = 4;
		break;
	}
}

void Foil(ITEM *item)
{
	switch(item->ilevel)
	{
	case 4:
		std::cout << std::endl << "Your Foil transformed into a Epee!" << std::endl;
		strcpy(item->szname,"Epee");
		item->istrength = 3;
		break;
	case 6:
		std::cout << std::endl << "Your Epee transformed into a Rapier!" << std::endl;
		strcpy(item->szname,"Rapier");
		item->istrength = 5;
		break;
	}
}

void Scimitar(ITEM *item)
{
	switch(item->ilevel)
	{
	case 6:
		std::cout << std::endl << "Your Scimitar transformed into a Bladed Gutter!" << std::endl;
		strcpy(item->szname,"Bladed Gutter");
		item->istrength = 6;
		break;
	}
}

void Hammer(ITEM *item)
{
	switch(item->ilevel)
	{
	case 5:
		std::cout << std::endl << "Your Hammer transformed into a Maul!" << std::endl;
		strcpy(item->szname,"Maul!");
		item->istrength = 5;
		break;
	case 10:
		std::cout << std::endl << "Your Cudgel transformed into a War Hammer!" << std::endl;
		strcpy(item->szname,"War Hammer");
		item->istrength = 9;
		break;
	}
}

void Breastplate(ITEM *item)
{
	switch(item->ilevel)
	{
	case 6:
		std::cout << std::endl << "Your Breastplate transformed into a Full Plate!" << std::endl;
		strcpy(item->szname,"Full Plate");
		item->istrength = 6;
		break;
	case 10:
		std::cout << std::endl << "Your Full Plate transformed into Legacy Armor!" << std::endl;
		strcpy(item->szname,"Legacy Armor");
		item->istrength = 10;
		break;
	}
}

void Baldric(ITEM *item)
{
	switch(item->ilevel)
	{
	case 5:
		std::cout << std::endl << "Your Baldric transformed into a Leather Tunic!" << std::endl;
		strcpy(item->szname,"Leather Tunic");
		item->istrength = 4;
		break;
	case 7:
		std::cout << std::endl << "Your Full Plate transformed into Studded Leather!" << std::endl;
		strcpy(item->szname,"Studded Leather");
		item->istrength = 8;
		break;
	}
}

void Chainmail(ITEM *item)
{
	switch(item->ilevel)
	{
	case 5:
		std::cout << std::endl << "Your Chainmail transformed into Ringed Mail!" << std::endl;
		strcpy(item->szname,"Ringed Mail");
		item->istrength = 6;
		break;
	case 8:
		std::cout << std::endl << "Your Ringed Mail transformed into Cervian Mail!" << std::endl;
		strcpy(item->szname,"Cervian Mail");
		item->istrength = 9;
		break;
	}
}

void Cape(ITEM *item)
{
	switch(item->ilevel)
	{
	case 4:
		std::cout << std::endl << "Your Cape transformed into a Robe!" << std::endl;
		strcpy(item->szname,"Robe");
		item->istrength = 5;
		break;
	}
}