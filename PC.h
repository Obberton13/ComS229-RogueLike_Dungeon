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
	int get_carry_index(char slot);
	int get_armor_index(char slot);
	void swap_items(unsigned int ind1, unsigned int ind2);
public:
	PC();
	~PC();
	int pick_up_item(Item* item);
	void attack(NPC* defender);
	void print_equipment(std::string message);
	void print_inventory(std::string message);
	int equip(char slot);
	int unequip(char slot);
	Item* drop(char slot);
	int expunge(char slot);
};
