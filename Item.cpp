#include "Item.h"


Item::Item()
{
}

Item::Item(unsigned char x, unsigned char y) :
	x(x),
	y(y),
	definition(ItemDef::getRandom())
{
}


Item::~Item()
{
}
