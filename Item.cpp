#include "Item.h"


Item::Item()
{
}

Item::Item(unsigned char x, unsigned char y) :
	y(y),
	x(x),
	definition(ItemDef::getRandom())
{
}


Item::~Item()
{
}
