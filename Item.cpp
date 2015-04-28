#include "Item.h"

Item::Item(unsigned char x, unsigned char y)
{
	this->x = new unsigned char (x);
	this->y = new unsigned char (y);
	ItemDef* def = ItemDef::getRandom();
	this->symbol = new unsigned char (def->getSymbol());
	this->name = new std::string(def->getName());
	this->description = new std::string(def->getDescription().c_str());
	this->color = new short (def->getColor());
	this->damage = new Dice(def->getDamage());
	this->hit = new Dice(def->getHit());
	this->dodge = new Dice(def->getDodge());
	this->attribute = new Dice(def->getAttribute());
	this->type = new unsigned int (def->getType());
	this->weight = new unsigned int (def->getWeight().Roll());
	this->speed = new unsigned int (def->getSpeed().Roll());
	this->value = new unsigned int (def->getValue().Roll());
}
Item::~Item()
{
	delete x;
	delete y;
	delete symbol;
	delete name;
	delete description;
	delete color;
	delete damage;
	delete hit;
	delete dodge;
	delete attribute;
	delete type;
	delete weight;
	delete speed;
	delete value;
}
