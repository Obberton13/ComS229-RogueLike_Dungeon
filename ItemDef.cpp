#include <fstream>
#include <sstream>
#include <regex>
#include "ItemDef.h"

std::vector<ItemDef*> ItemDef::definitions;

int ItemDef::parseObjects()
{
	std::string path("object_desc.txt");
	std::ifstream f(path);
	if (!f)
	{
		std::cerr << "File does not exist" << std::endl;
		return 1;
	}
	std::string line;
	std::getline(f, line);
	if (line != "RLG229 OBJECT DESCRIPTION 1")
	{
		std::cerr << "File has incorrect metadata" << std::endl;
		return 1;
	}
	ItemDef *obj;
	while (std::getline(f, line))
	{
		if (line.length() == 0) continue;
		//std::cout << line << std::endl;
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		//std::cout << word << " ";
		if (word == "BEGIN")
		{
			iss >> word;
			if (word != "OBJECT")
			{
				std::cerr << "invalid object starter line: " << line << std::endl;
				continue;
			}
			obj = new ItemDef();
			//obj->setSymbol('/');
			//std::cout << "New obj made" << std::endl;
		}
		else if (word == "NAME")
		{
			std::tr1::regex regex("NAME (.*)");
			std::tr1::cmatch matches;
			std::tr1::regex_match(line.c_str(), matches, regex);
			std::string str(matches[1]);
			obj->setName(str);
			//std::cout << "Set obj's name" << std::endl;
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
			obj->setDescription(description);
			//std::cout << "Set obj description" << std::endl;
		}
		else if (word == "SYMB")
		{
			char c;
			iss >> c;
			obj->setSymbol(c);
			//std::cout << "Set obj symbol" << std::endl;
		}
		else if (word == "COLOR")
		{
			std::string color;
			iss >> color;
			obj->setColor(color);
			std::cout << "Set obj color to: " << color << std::endl;
		}
		else if (word == "SPEED")
		{
			iss >> word;
			Dice d(word);
			obj->setSpeed(d);
			//std::cout << "Set obj speed" << std::endl;
		}
		else if (word == "DAM")
		{
			iss >> word;
			Dice d(word);
			obj->setDamage(d);
			//std::cout << "Set obj Damage" << std::endl;
		}
		else if (word == "TYPE")
		{
			std::tr1::regex regex("TYPE (.*)");
			std::tr1::cmatch matches;
			std::tr1::regex_match(line.c_str(), matches, regex);
			std::string type(matches[1]);
			obj->setType(type);
		}
		else if (word == "WEIGHT")
		{
			iss >> word;
			Dice d(word);
			obj->setWeight(d);
			//std::cout << "Set obj weight" << std::endl;
		}
		else if (word == "HIT")
		{
			iss >> word;
			Dice d(word);
			obj->setHit(d);
			//std::cout << "Set obj hit bonus" << std::endl;
		}
		else if (word == "ATTR")
		{
			iss >> word;
			Dice d(word);
			obj->setAttribute(d);
			//std::cout << "Set obj attribute" << std::endl;
		}
		else if (word == "VAL")
		{
			iss >> word;
			Dice d(word);
			obj->setValue(d);
			//std::cout << "Set obj value" << std::endl;
		}
		else if (word == "DODGE")
		{
			iss >> word;
			Dice d(word);
			obj->setDodge(d);
			//std::cout << "Set obj dodge bonus" << std::endl;
		}
		else if (word == "DEF")
		{
			iss >> word;
			Dice d(word);
			obj->setDefence(d);
			//std::cout << "Set obj defence bonus" << std::endl;
		}
		else if (word == "END")
		{
			if (!obj->isFinished())
			{
				//std::cout << "obj not finished" << std::endl;
				continue;
			}
			definitions.push_back(obj);
			obj = NULL;
			//std::cout << *obj << std::endl << std::endl;
			//std::cout << "obj finished" << std::endl;
		}
		else
		{
			std::cerr << "Found an unexpeded identifier in " << path << ": " << word << std::endl;
			break;
		}
	}
	return 0;
}


ItemDef::ItemDef()
{
}


ItemDef::~ItemDef()
{
}

std::string ItemDef::getType()
{
	return std::string();
}

void ItemDef::setType(std::string t)
{
	if (t == "WEAPON")
	{
		type |= ITEM_WEAPON;
		setSymbol('|');
	}
	if (t == "OFFHAND")
	{
		type |= ITEM_OFFHAND;
		setSymbol(')');
	}
	if (t == "RANGED")
	{
		type |= ITEM_RANGED;
		setSymbol('}');
	}
	if (t == "ARMOR")
	{
		type |= ITEM_WEAPON;
		setSymbol('[');
	}
	if (t == "HELMET")
	{
		type |= ITEM_HELMET;
		setSymbol(']');
	}
	if (t == "CLOAK")
	{
		type |= ITEM_CLOAK;
		setSymbol('(');
	}
	if (t == "GLOVES")
	{
		type |= ITEM_GLOVES;
		setSymbol('{');
	}
	if (t == "BOOTS")
	{
		type |= ITEM_BOOTS;
		setSymbol('\\');
	}
	if (t == "RING")
	{
		type |= ITEM_RING;
		setSymbol('=');
	}
	if (t == "AMULET")
	{
		type |= ITEM_AMULET;
		setSymbol('\"');
	}
	if (t == "LIGHT")
	{
		type |= ITEM_LIGHT;
		setSymbol('_');
	}
	if (t == "SCROLL")
	{
		type |= ITEM_SCROLL;
		setSymbol('~');
	}
	if (t == "BOOK")
	{
		type |= ITEM_BOOK;
		setSymbol('?');
	}
	if (t == "FLASK")
	{
		type |= ITEM_FLASK;
		setSymbol('!');
	}
	if (t == "GOLD")
	{
		type |= ITEM_GOLD;
		setSymbol('$');
	}
	if (t == "AMMUNITION")
	{
		type |= ITEM_AMMO;
		setSymbol('/');
	}
	if (t == "FOOD")
	{
		type |= ITEM_FOOD;
		setSymbol(',');
	}
	if (t == "WAND")
	{
		type |= ITEM_WAND;
		setSymbol('-');
	}
	if (t == "CONTAINER")
	{
		type |= ITEM_CONTAINER;
		setSymbol('%');
	}
	std::cout << getSymbol() << std::endl;
}

unsigned char ItemDef::getColor()
{
	return color;
}

void ItemDef::setColor(std::string col)
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
}

ItemDef* ItemDef::getRandom()
{
	unsigned int index = std::rand() % definitions.size();
	return definitions[index];
}

std::ostream &operator<<(std::ostream &o, const ItemDef &i)
{
	return o << i.name << std::endl
		<< i.description
		<< i.type << std::endl
		<< i.symbol << std::endl
		<< i.color << std::endl
		<< i.hit << std::endl
		<< i.damage << std::endl
		<< i.dodge << std::endl
		<< i.defence << std::endl
		<< i.weight << std::endl
		<< i.speed << std::endl
		<< i.attribute << std::endl
		<< i.val;
}

void ItemDef::deleteDefs()
{
	while (definitions.empty())
	{
		delete definitions[definitions.size() - 1];
		definitions.pop_back();
	}
}