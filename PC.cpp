#include "PC.h"
#include <limits.h>

#ifdef SCREEN_W
#define SCREEN_W 24
#define SCREEN_H 80
#endif

PC::PC()
{
	NPC::x = 0;
	NPC::y = 0;
	NPC::symbol = '@';
	NPC::hitpoints = Dice(15, 2, 8).Roll();
	NPC::speed = 10;
	NPC::initiative = 10;
	NPC::abilities = NPC_TUNNEL|NPC_TELEPATH;
	NPC::name = "Intarmin Hanurnatural";
	NPC::description = "The player character";
	NPC::color = COLOR_WHITE;
	NPC::damage = Dice(0, 2, 6);
	NPC::alive = true;
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

void PC::print_equipment(std::string message)
{
	unsigned char i, row = 2;
	mvprintw(row, 4, message.c_str());
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

void PC::print_inventory(std::string message)
{
	unsigned char i, row = 2;
	mvprintw(row, 4, message.c_str());
	for(i = PC_ARMOR_SLOTS; i < PC_TOTAL_SLOTS;i++)
	{
		if(slots[i])
		{
			std::string toPrint;
			toPrint.push_back(i + '0' - PC_ARMOR_SLOTS);
			toPrint+= ") ";
			toPrint += slots[i]->getName();
			mvprintw(++row, 2, toPrint.c_str());
		}
	}
}

int PC::equip(char slot)
{
	int index = get_carry_index(slot);
	if(index==INT_MAX)
	{
		return 1;
	}
	Item* item = slots[index];
	if(!item) return 1;
	unsigned int type = item->getType();
	switch(type)
	{
		case ITEM_WEAPON:
			swap_items(index, SLOT_WEAPON);
			break;
		case ITEM_OFFHAND:
			swap_items(index, SLOT_OFFHAND);
			break;
		case ITEM_RANGED:
			swap_items(index, SLOT_RANGED);
			break;
		case ITEM_ARMOR:
			swap_items(index, SLOT_ARMOR);
			break;
		case ITEM_HELMET:
			swap_items(index, SLOT_HELMET);
			break;
		case ITEM_CLOAK:
			swap_items(index, SLOT_CLOAK);
			break;
		case ITEM_GLOVES:
			swap_items(index, SLOT_GLOVES);
			break;
		case ITEM_BOOTS:
			swap_items(index, SLOT_BOOTS);
			break;
		case ITEM_RING://TODO special case
			if(slots[SLOT_RING1])
			{
				Item* tmp = slots[SLOT_RING1];
				slots[SLOT_RING1] = slots[index];
				slots[index] = slots[SLOT_RING2];
				slots[SLOT_RING2] = tmp;
			}
			swap_items(index, SLOT_RING1);
			break;
		case ITEM_AMULET:
			swap_items(index, SLOT_AMULET);
			break;
		case ITEM_LIGHT:
			swap_items(index, SLOT_LIGHT);
			break;
		case ITEM_SCROLL:
		case ITEM_BOOK:
		case ITEM_FLASK:
		case ITEM_GOLD:
		case ITEM_AMMO:
		case ITEM_FOOD:
		case ITEM_WAND:
		case ITEM_CONTAINER:
		default:
			return 1;
	}
	return 0;
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
	delete slots[index];
	slots[index] = NULL;
	return 0;
}

void PC::swap_items(unsigned int ind1, unsigned int ind2)
{
	Item* tmp = slots[ind1];
	slots[ind1] = slots[ind2];
	slots[ind2] = tmp;
}

PC::~PC()
{
	for(int i = 0; i < PC_TOTAL_SLOTS; i++)
	{
		delete slots[i];
	}
	delete[] slots;
}
