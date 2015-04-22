#pragma once
#include "NPC.h"
#include "Item.h"
#include <vector>

#define PC_INVENTORY_SLOTS 10
#define PC_ARMOR_SLOTS 12
#define PC_TOTAL_SLOTS (PC_INVENTORY_SLOTS + PC_ARMOR_SLOTS)

class PC : public NPC
{
private:
	Item** slots;
public:
	PC();
	~PC();
	int pick_up_item(Item* item);
	Item* get_item(char slot);
	int put_item(char slot, Item* item);
};
