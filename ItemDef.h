#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Dice.h"
#include <curses.h>

#define ITEM_WEAPON		0x00000001
#define ITEM_OFFHAND	0x00000002
#define ITEM_RANGED		0x00000004
#define ITEM_ARMOR		0x00000008
#define ITEM_HELMET		0x00000010
#define ITEM_CLOAK		0x00000020
#define ITEM_GLOVES		0x00000040
#define ITEM_BOOTS		0x00000080
#define ITEM_RING		0x00000100
#define ITEM_AMULET		0x00000200
#define ITEM_LIGHT		0x00000400
#define ITEM_SCROLL		0x00000800
#define ITEM_BOOK		0x00001000
#define ITEM_FLASK		0x00002000
#define ITEM_GOLD		0x00004000
#define ITEM_AMMO		0x00008000
#define ITEM_FOOD		0x00010000
#define ITEM_WAND		0x00020000
#define ITEM_CONTAINER	0x00040000
#define ITEM_BIT19		0x00080000
#define ITEM_BIT20		0x00100000
#define ITEM_BIT21		0x00200000
#define ITEM_BIT22		0x00400000
#define ITEM_BIT23		0x00800000
#define ITEM_BIT24		0x01000000
#define ITEM_BIT25		0x02000000
#define ITEM_BIT26		0x04000000
#define ITEM_BIT27		0x08000000
#define ITEM_BIT28		0x10000000
#define ITEM_BIT29		0x20000000
#define ITEM_BIT30		0x40000000
#define ITEM_BIT31		0x80000000

class ItemDef
{
private:
	bool nameSet;
	std::string name;
	bool descSet;
	std::string description;
	bool typeSet;
	unsigned int type;
	bool symbSet;
	char symbol;
	bool colorSet;
	short color;
	bool hitSet;
	Dice hit;
	bool damSet;
	Dice damage;
	bool dodgeSet;
	Dice dodge;
	bool defSet;
	Dice defence;
	bool weightSet;
	Dice weight;
	bool spdSet;
	Dice speed;
	bool attrSet;
	Dice attribute;
	bool valueSet;
	Dice val;

	static std::vector<ItemDef*> definitions;

public:

	inline std::string getName(){ return name; }
	inline void setName(std::string n){ this->name = n; nameSet = true; }

	inline std::string getDescription(){ return description; }
	inline void setDescription(std::string d){ description = d; descSet = true; }
	
	std::string getType();//{ return type; }
	void setType(std::string t);//{ type = t; typeSet = true; }

	inline char getSymbol(){ return symbol; }
	inline void setSymbol(char s){ symbol = s; symbSet = true; }

	short getColor();// { return color; }
	void setColor(std::string c);//{ color = c; colorSet = true; }

	inline Dice getHit() { return hit; }
	inline void setHit(Dice h) { hit = h; hitSet = true; }

	inline Dice getDamage(){ return damage; }
	inline void setDamage(Dice d){ damage = d; damSet = true; }

	inline Dice getDodge(){ return dodge; }
	inline void setDodge(Dice d){ dodge = d; dodgeSet = true; }

	inline Dice getDefence(){ return defence; }
	inline void setDefence(Dice d){ defence = d; defSet = true; }

	inline Dice getWeight(){ return weight; }
	inline void setWeight(Dice w){ weight = w; weightSet = true; }

	inline Dice getSpeed(){ return speed; }
	inline void setSpeed(Dice s){ speed = s; spdSet = true; }

	inline Dice getAttribute(){ return attribute; }
	inline void setAttribute(Dice a){ attribute = a; attrSet = true; }

	inline Dice getValue(){ return val; }
	inline void setValue(Dice v){ val = v; valueSet = true; }

	inline bool isFinished()
	{
		return nameSet && descSet && typeSet && symbSet &&	colorSet &&	damSet && dodgeSet && 
			defSet && weightSet && spdSet && attrSet && valueSet && hitSet;
	}
	static int parseObjects();
	static ItemDef* getRandom();
	ItemDef();
	~ItemDef();
	static void deleteDefs();
	friend std::ostream &operator<<(std::ostream &o, const ItemDef &i);
};
