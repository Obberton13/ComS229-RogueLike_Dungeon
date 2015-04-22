#pragma once
#include "NPC.h"
#include "Item.h"
#include <vector>

#define PC_INVENTORY_SLOTS 10
#define PC_ARMOR_SLOTS 12
#define PC_TOTAL_SLOTS (PC_INVENTORY_SLOTS + PC_ARMOR_SLOTS)

#define SLOT_WEAPON 0
#define SLOT_OFFHAND 1
#define SLOT_RANGED 2
#define SLOT_ARMOR 3
#define SLOT_HELMET 4
#define SLOT_CLOAK 5
#define SLOT_GLOVES 6
#define SLOT_BOOTS 7
#define SLOT_AMULET 8
#define SLOT_LIGHT 9
#define SLOT_RING1 10
#define SLOT_RING2 11

class PC : public NPC
{
private:
	Item** slots;
public:
	PC(NPCdef* def);
	~PC();
	int pick_up_item(Item* item);
	Item* get_item(char slot);
	int put_item(char slot, Item* item);
	void attack(NPC* defender);
};
