#include "NPCdef.h"
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

std::vector<NPCdef*> NPCdef::definitions;

NPCdef::NPCdef()
{
}

NPCdef::~NPCdef()
{
}
char NPCdef::getSymbol()
{
	return this->symbol;
}
std::string NPCdef::getName()
{
	return this->name;
}
std::string NPCdef::getDescription()
{
	return this->description;
}
std::string NPCdef::getColor()
{
	return this->color;
}
unsigned int NPCdef::getAbilities()
{
	return this->abilities;
}
Dice NPCdef::getSpeed()
{
	return this->speed;
}
Dice NPCdef::getDamage()
{
	return this->damage;
}
Dice NPCdef::getHP()
{
	return this->HP;
}

void NPCdef::setSymbol(const char c)
{
	this->symbol = c;
	this->symbolSet = true;
}
void NPCdef::setName(const std::string &str)
{
	this->name = str;
	this->nameSet = true;
}
void NPCdef::setDescription(const std::string &str)
{
	this->description = str;
	this->descriptionSet = true;
}
void NPCdef::setColor(const std::string &col)
{
	if (col == "BLACK") color = COLOR_BLACK;
	else if (col == "WHITE") color = COLOR_WHITE;
	else if (col == "GREEN") color = COLOR_GREEN;
	else if (col == "RED") color = COLOR_RED;
	else if (col == "BLUE") color = COLOR_BLUE;
	else if (col == "MAGENTA") color = COLOR_MAGENTA;
	else if (col == "YELLOW") color = COLOR_YELLOW;
	else if (col == "CYAN") color = COLOR_CYAN;
	else throw "Invalid color selected: " + col;
	this->colorSet = true;
}
void NPCdef::setAbilities(const std::string &str)
{
	abilities = 0;
	if (str == "SMART") abilities |= NPC_SMART;
	if (str == "TELEPATH") abilities |= NPC_TELEPATH;
	if (str == "TUNNEL") abilities |= NPC_TUNNEL;
	if (str == "PASS") abilities |= NPC_PASS;
	this->abilitiesSet = true;
}
void NPCdef::setSpeed(const Dice &d)
{
	this->speed = d;
}
void NPCdef::setDamage(const Dice &d)
{
	this->damage = d;
}
void NPCdef::setHP(const Dice &d)
{
	this->HP = d;
}

std::ostream &operator<<(std::ostream &o, const NPCdef &npc)
{
	return o << npc.symbol;
}

const std::vector<NPCdef*> &NPCdef::getDefs()
{
	return definitions;
}

int NPCdef::parseDefs()
{
	std::string path("monsterdef.txt");
	std::ifstream f(path);
	if (!f)
	{
		std::cout << "File does not exist" << std::endl;
		return 1;
	}
	std::string line;
	std::getline(f, line);
	if (line != "RLG229 MONSTER DESCRIPTION 1")
	{
		std::cout << "File has incorrect metadata" << std::endl;
		return 1;
	}
	NPCdef *monster;
	while (std::getline(f, line))
	{
		//std::cout << line << std::endl;
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		//std::cout << word << " ";
		if (word == "")
			continue;
		if (word == "BEGIN")
		{
			iss >> word;
			if (word != "MONSTER")
			{
				std::cout << "Invalid BEGIN tag" << std::endl;
				continue;
			}
			monster = new NPCdef();
			//std::cout << "New monster made" << std::endl;
		}
		else if (word == "NAME")
		{
			std::tr1::regex regex("NAME (.*)");
			std::tr1::cmatch matches;
			std::tr1::regex_match(line.c_str(), matches, regex);
			std::string str(matches[1]);
			monster->setName(str);
			//std::cout << "Set monster's name" << std::endl;
		}
		else if (word == "DESC")
		{
			std::string description;
			std::getline(f, line);
			while (line != ".")
			{
				description += line;
				description += "\n";
				std::getline(f, line);
			}
			monster->setDescription(description);
			//std::cout << "Set monster description" << std::endl;
		}
		else if (word == "SYMB")
		{
			char c;
			iss >> c;
			monster->setSymbol(c);
			//std::cout << "Set monster symbol" << std::endl;
		}
		else if (word == "COLOR")
		{
			std::string color;
			iss >> color;
			monster->setColor(color);
			//std::cout << "Set monster color" << std::endl;
		}
		else if (word == "SPEED")
		{
			iss >> word;
			Dice d(word);
			monster->setSpeed(d);
			//std::cout << "Set monster speed" << std::endl;
		}
		else if (word == "ABIL")
		{
			std::tr1::regex regex("ABIL (.*)");
			std::tr1::cmatch matches;
			std::tr1::regex_match(line.c_str(), matches, regex);
			std::string str(matches[1]);
			monster->setAbilities(str);
			//std::cout << "Set monster Abilities" << std::endl;
		}
		else if (word == "HP")
		{
			iss >> word;
			Dice d(word);
			monster->setHP(d);
			//std::cout << "Set monster HP" << std::endl;
		}
		else if (word == "DAM")
		{
			iss >> word;
			Dice d(word);
			monster->setDamage(d);
			//std::cout << "Set monster Damage" << std::endl;
		}
		else if (word == "END")
		{
			if (!monster->isFinished())
			{
				std::cerr << "Monster not finished" << std::endl;
				continue;
			}
			definitions.push_back(monster);
		//	std::cout << "monster finished" << std::endl;
		}
		else
		{
			std::cerr << "Found an unexpeded identifier in "<< path << ": " << word << std::endl;
			return 1;
		}
	}
	return 0;
}

NPCdef *NPCdef::getRandom()
{
	unsigned int index = std::rand() % definitions.size();
	NPCdef * def = definitions[index];
	return definitions[index];
}

void NPCdef::deleteDefs()
{
	while (definitions.empty())
	{
		delete definitions[definitions.size() - 1];
		definitions.pop_back();
	}
}
