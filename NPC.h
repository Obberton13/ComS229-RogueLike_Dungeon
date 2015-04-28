#pragma once
#include <iostream>
#include <string>
#include "NPCdef.h"

class NPC
{
protected:
	unsigned char x, y;
	char symbol;
	int hitpoints, speed, initiative;
	unsigned int abilities;
	std::string name, description;
	short color;
	Dice damage;
	bool alive;
public:
	NPC();
	NPC(unsigned char x, unsigned char y);
	NPC(unsigned char x, unsigned char y, NPCdef *def);
	virtual ~NPC();

	void setPos(unsigned char x, unsigned char y);
	virtual void attack(NPC* defender);
	virtual void hit(int amount);
	void reset_initiative();
	void use_initiative();
	bool is_next_turn();

	inline unsigned char getX(){ return x; }
	inline unsigned char getY(){ return y; }
	inline unsigned int getAbilities(){ return abilities; }
	inline int getInitiative() { return initiative; }
	inline char getSymbol(){return symbol;}
	inline short getColor() {return color;}
	inline int getHitpoints() {return hitpoints;}
	inline bool isAlive() {return alive;}

	inline int isNextTurn()
	{
		return initiative >= 10/speed;
	}

	friend std::ostream &operator<<(std::ostream &o, const NPC &npc);
};

