#include "PC.h"
#include <limits.h>

#ifdef SCREEN_W
#define SCREEN_W 24
#define SCREEN_H 80
#endif

PC::PC(NPCdef* def):NPC(0, 0, def)
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
		if(!slots[i])
		{
			slots[i] = item;
			return 0;
		}
	}
	return 1;
}

int PC::get_armor_index(char slot)
{
	if(slot > 'l') return INT_MAX;
	if(slot < 'a') return INT_MAX;
	return (int)(slot - 'a');
}

int PC::get_carry_index(char slot)
{
	if(slot > '9') return INT_MAX;
	if(slot < '0') return INT_MAX;
	return (int)(slot - '0' + PC_ARMOR_SLOTS);
}

int put_item(char slot, Item* item)
{
	//TODO unimplemented
	return 0;
}

void PC::attack(NPC* defender)
{
	int i;
	int dmg = 0;
	if(!slots[SLOT_WEAPON]) dmg += NPC::damage.Roll();
	for(i = 0; i < PC_TOTAL_SLOTS; i++)
	{
		if(slots[i])
		{
			dmg += slots[i]->rollDamage();
		}
	}
	defender->hit(dmg);
}

void PC::print_equipment()
{
	unsigned char i, row;
	for(i = 0; i < PC_ARMOR_SLOTS; i++)
	{
		if(slots[i])
		{
			std::string toPrint;
			toPrint.push_back(i + 'a');
			toPrint += ") ";
			toPrint += slots[i]->getName();
			mvprintw(++row, 2, toPrint.c_str());
		}
	}
}

void PC::print_inventory()
{
	unsigned char i, row;
	for(i = PC_ARMOR_SLOTS; i < PC_TOTAL_SLOTS;i++)
	{
		std::string toPrint;
		toPrint.push_back(i + '0');
		toPrint+= ") ";
		toPrint += slots[i]->getName();
		mvprintw(++row, 2, toPrint.c_str());
	}
}

int PC::equip(char slot)
{
	int index = get_carry_index(slot);
	if(index==INT_MAX) return 1;
	Item* item = slots[index];
	std::string attributes = item->getType();
	return 0;
	//TODO this will involve a giant switch statement.
}

int PC::unequip(char slot)
{
	int index = get_armor_index(slot);
	if(index == INT_MAX) return 1;
	Item* item = slots[index];
	unsigned char i;
	for(i = PC_ARMOR_SLOTS; i < PC_TOTAL_SLOTS; i++)
	{
		if(!slots[i])
		{
			slots[i] = item;
			slots[index] = NULL;
			return 0;
		}
	}
	return 1;
}

Item* PC::drop(char slot)
{
	int index = get_carry_index(slot);
	if(index == INT_MAX)
	{
		return NULL;
	}
	Item* item = slots[index];
	slots[index] = NULL;
	return item;
}

int PC::expunge(char slot)
{
	int index = get_carry_index(slot);
	if(index == INT_MAX) return 1;
	slots[index] = NULL;
	return 0;
}

PC::~PC()
{
	delete[] slots;
}
