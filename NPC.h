#pragma once
#include <iostream>
#include <string>
#include "NPCdef.h"

class NPC
{
private:
	unsigned char x, y;
	char symbol;
	int hitpoints, speed, initiative;
	unsigned int abilities;
	std::string name, description;
	short color;
	Dice damage;
	bool isAlive;
public:
	NPC();
	NPC(unsigned char x, unsigned char y);
	NPC(unsigned char x, unsigned char y, NPCdef *def);
	~NPC();

	void setPos(unsigned char x, unsigned char y);

	inline unsigned char getX(){ return x; }
	inline unsigned char getY(){ return y; }
	inline unsigned int getAbilities(){ return abilities; }
	inline int getInitiative() { return initiative; }
	inline char getSymbol(){return symbol;}
	inline short getColor() {return color;}

	inline int isNextTurn()
	{
		return initiative == 0;
	}

	friend std::ostream &operator<<(std::ostream &o, const NPC &npc);
};

