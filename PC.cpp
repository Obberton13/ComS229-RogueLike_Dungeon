#include "PC.h"


PC::PC()
{
	unsigned char i;
	slots = new Item*[PC_TOTAL_SLOTS];
	for(i = 0; i < PC_TOTAL_SLOTS; i++)
	{
		slots[i] = NULL;
	}
}

int PC::pick_up_item(Item* item)
{
	unsigned char i;
	for(i = PC_ARMOR_SLOTS; i < PC_TOTAL_SLOTS; i++)
	{
		if(slots[i]!=NULL)
		{
			slots[i] = item;
			return 0;
		}
	}
	return 1;
}

Item* PC::get_item(char slot)
{
	if(slot > 'l') return NULL;
	else if(slot >= 'a') slot -= 'a' + PC_ARMOR_SLOTS;
	else if(slot >= '0') slot =- '0';//makes '0' actually 0.
	return slots[(int)slot];
}

PC::~PC(){}
