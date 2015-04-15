#include "NPC.h"

NPC::NPC() :
	x(0),
	y(0),
	isAlive(true)
{
	NPCdef *def = NPCdef::getRandom();
	this->hitpoints = def->getHP().Roll();
	this->speed = def->getSpeed().Roll();
	this->initiative = this->speed;
	this->symbol = def->getSymbol();
	this->name = def->getName();
	this->description = def->getDescription();
}

NPC::NPC(unsigned char x, unsigned char y) :
	x(x),
	y(y),
	isAlive(true)
{
	NPCdef* def = NPCdef::getRandom();
	this->hitpoints = def->getHP().Roll();
	this->speed = def->getSpeed().Roll();
	this->initiative = this->speed;
	this->symbol = def->getSymbol();
	this->name = def->getName();
	this->description = def->getDescription();
}

NPC::NPC(unsigned char x, unsigned char y, NPCdef *def) : 
	x(x),
	y(y),
	isAlive(true)
{
	this->hitpoints = def->getHP().Roll();
	this->speed = def->getSpeed().Roll();
	this->initiative = this->speed;
	this->symbol = def->getSymbol();
	this->name = def->getName();
	this->description = def->getDescription();
	this->abilities = def->getAbilities();
}

void NPC::setPos(unsigned char x, unsigned char y)
{
	this->x = x;
	this->y = y;
}

NPC::~NPC()
{
}

std::ostream &operator<<(std::ostream &o, const NPC &npc)
{
	o << npc.symbol;
	return o;
}
