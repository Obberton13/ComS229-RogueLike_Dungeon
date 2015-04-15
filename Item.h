#pragma once
#include "ItemDef.h"
class Item
{
private:
	unsigned char x, y;
	ItemDef* definition;
public:
	Item();
	Item(unsigned char x, unsigned char y);
	Item(unsigned char x, unsigned char y, ItemDef *def);
	~Item();

	inline char getSymbol() {return definition->getSymbol();}
	inline short getColor() {return definition->getColor();}

	friend std::ostream &operator<<(std::ostream &o, Item &i);
};

inline std::ostream &operator<<(std::ostream &o, Item &i)
{
	o << i.definition->getSymbol();
	return o;
}

