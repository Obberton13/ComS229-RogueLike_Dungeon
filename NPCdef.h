#pragma once
#include "Dice.h"
#include <vector>

#define NPC_SMART 		0x00000001
#define NPC_TELEPATH	0x00000002
#define NPC_TUNNEL		0x00000004
#define NPC_PASS		0x00000008
#define NPC_BIT5		0x00000010
#define NPC_BIT6		0x00000020
#define NPC_BIT7		0x00000040
#define NPC_BIT8		0x00000080
#define NPC_BIT9		0x00000100
#define NPC_BIT10		0x00000200
#define NPC_BIT11		0x00000400
#define NPC_BIT12		0x00000800
#define NPC_BIT13		0x00001000
#define NPC_BIT14		0x00002000
#define NPC_BIT15		0x00004000
#define NPC_BIT16		0x00008000
#define NPC_BIT17 		0x00010000
#define NPC_BIT18		0x00020000
#define NPC_BIT19		0x00040000
#define NPC_BIT20		0x00080000
#define NPC_BIT21		0x00100000
#define NPC_BIT22		0x00200000
#define NPC_BIT23		0x00400000
#define NPC_BIT24		0x00800000
#define NPC_BIT25		0x01000000
#define NPC_BIT26		0x02000000
#define NPC_BIT27		0x04000000
#define NPC_BIT28		0x08000000
#define NPC_BIT29		0x10000000
#define NPC_BIT30		0x20000000
#define NPC_BIT31		0x40000000
#define NPC_BIT32		0x80000000

//TODO temporary color definitions for Windows b/c no ncurses
#define COLOR_BLACK 1
#define COLOR_WHITE 2
#define COLOR_BLUE 3
#define COLOR_CYAN 4
#define COLOR_YELLOW 5
#define COLOR_MAGENTA 6
#define COLOR_RED 7
#define COLOR_GREEN 8

class NPCdef
{
private:
	static std::vector<NPCdef*> definitions;
	char symbol;
	bool symbolSet;
	std::string name;
	bool nameSet;
	std::string description;
	bool descriptionSet;
	std::string color;
	bool colorSet;
	unsigned int abilities;
	bool abilitiesSet;
	Dice speed;
	Dice damage;
	Dice HP;
	
public:
	char getSymbol();
	std::string getName();
	std::string getDescription();
	std::string getColor();
	unsigned int getAbilities();
	Dice getSpeed();
	Dice getDamage();
	Dice getHP();

	void setSymbol(const char c);
	void setName(const std::string &str);
	void setDescription(const std::string &str);
	void setColor(const std::string &str);
	void setAbilities(const std::string &str);
	void setSpeed(const Dice &d);
	void setDamage(const Dice &d);
	void setHP(const Dice &d);

	static int parseDefs();
	const static std::vector<NPCdef*> &getDefs();
	static NPCdef *getRandom();

	inline bool isFinished()
	{
		return symbolSet && nameSet && descriptionSet && colorSet && abilitiesSet 
			&& speed.isAssigned() && damage.isAssigned() && HP.isAssigned();
	}

	NPCdef();
	~NPCdef();

	static void deleteDefs();

	friend std::ostream &operator<<(std::ostream &o, const NPCdef &npc);
};
