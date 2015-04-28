#pragma once
#include "ItemDef.h"
class Item
{
private:
	unsigned char* x;
	unsigned char* y;
	unsigned char* symbol;
	std::string* name;
	std::string* description;
	short* color;
	Dice* damage;
	Dice* hit;
	Dice* dodge;
	Dice* attribute;
	unsigned int* type;
	unsigned int* weight;
	unsigned int* speed;
	unsigned int* value;
public:
	~Item();
	Item(unsigned char x, unsigned char y);
	Item(unsigned char x, unsigned char y, ItemDef *def);

	inline char getSymbol() {return *symbol;}
	inline short getColor() {return *color;}
	inline unsigned int rollDamage() {return damage->Roll();}
	inline std::string getName() {return *name;}
	inline unsigned int getType() {
		return *type;
	}

	friend std::ostream &operator<<(std::ostream &o, Item &i);
};

inline std::ostream &operator<<(std::ostream &o, Item &i)
{
	o << i.symbol;
	return o;
}

