#include "NPC.h"

NPC::NPC() :
	x(0),
	y(0),
	alive(true)
{
	NPCdef *def = NPCdef::getRandom();
	this->hitpoints = def->getHP().Roll();
	this->speed = def->getSpeed().Roll();
	this->initiative = 100/this->speed;
	this->symbol = def->getSymbol();
	this->name = def->getName();
	this->description = def->getDescription();
	this->abilities = def->getAbilities();
	this->color = def->getColor();
	this->damage = def->getDamage();

}

NPC::NPC(unsigned char x, unsigned char y):
	x(x),
	y(y),
	alive(true)
{
	NPCdef* def = NPCdef::getRandom();
	this->hitpoints = def->getHP().Roll();
	this->speed = def->getSpeed().Roll();
	this->initiative = 100/this->speed;
	this->symbol = def->getSymbol();
	this->name = def->getName();
	this->description = def->getDescription();
	this->abilities = def->getAbilities();
	this->color = def->getColor();
	this->damage = def->getDamage();
}

NPC::NPC(unsigned char x, unsigned char y, NPCdef *def) : 
	x(x),
	y(y),
	alive(true)
{
	this->hitpoints = def->getHP().Roll();
	this->speed = def->getSpeed().Roll();
	this->initiative = 100/this->speed;
	this->initiative = 0;
	this->symbol = def->getSymbol();
	this->name = def->getName();
	this->description = def->getDescription();
	this->abilities = def->getAbilities();
	this->color = def->getColor();
	this->damage = def->getDamage();

}

void NPC::setPos(unsigned char x, unsigned char y)
{
	this->x = x;
	this->y = y;
}

void NPC::attack(NPC* defender)
{
	defender->hit(damage.Roll());
}

void NPC::hit(int amount)
{
	this->hitpoints-=amount;
	std::cerr << this->name << " got hit for " << amount << "points of damage." << std::endl;
	if(this->hitpoints<=0)
	{
		alive = false;
	}
}

void NPC::reset_initiative()
{
	this->initiative = 0;
}

void NPC::use_initiative()
{
	this->initiative++;
}

bool NPC::is_next_turn()
{
	return this->initiative >= 100/speed;
}

NPC::~NPC()
{
}

std::ostream &operator<<(std::ostream &o, const NPC &npc)
{
	o << npc.symbol;
	return o;
}
