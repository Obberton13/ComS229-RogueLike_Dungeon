#include <fstream>
#include <sstream>
#include <boost/regex.hpp>
#include "ItemDef.h"

std::vector<ItemDef*> ItemDef::definitions;

int ItemDef::parseObjects()
{
	std::string path;
	std::string home;
	std::string file= ".rlg229/object_desc.txt";
	home = getenv("HOME");
	path = home + "/" + file;
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
		std::cerr << line << std::endl;
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
			//std::cout << "New obj made" << std::endl;
		}
		else if (word == "NAME")
		{
			boost::regex regex("NAME (.*)");
			boost::cmatch matches;
			boost::regex_match(line.c_str(), matches, regex);
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
			//std::cout << "Set obj color to: " << color << std::endl;
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
			boost::regex regex("TYPE (.*)");
			boost::cmatch matches;
			boost::regex_match(line.c_str(), matches, regex);
			std::string type(matches[1]);
			obj->setType(type);
			//std::cerr << obj->getType() << std::endl;
			//std::cerr << "Set obj type" << std::endl;
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
				std::cerr << "obj not finished" << std::endl;
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
	//std::cout << definitions.size() << std::endl;
	return 0;
}


ItemDef::ItemDef():
	nameSet(false),
	descSet(false),
	typeSet(false),
	symbSet(false),
	colorSet(false),
	hitSet(false),
	damSet(false),
	dodgeSet(false),
	defSet(false),
	weightSet(false),
	spdSet(false),
	attrSet(false),
	valueSet(false){}

ItemDef::~ItemDef()
{
}

unsigned int ItemDef::getType()
{
	return type;
}

void ItemDef::setType(std::string t)
{
	type = 0;
	if (t == "WEAPON")
	{
		type |= ITEM_WEAPON;
		setSymbol('|');
	}
	else if (t == "OFFHAND")
	{
		type |= ITEM_OFFHAND;
		setSymbol(')');
	}
	else if (t == "RANGED")
	{
		type |= ITEM_RANGED;
		setSymbol('}');
	}
	else if (t == "ARMOR")
	{
		type |= ITEM_WEAPON;
		setSymbol('[');
	}
	else if (t == "HELMET")
	{
		type |= ITEM_HELMET;
		setSymbol(']');
	}
	else if (t == "CLOAK")
	{
		type |= ITEM_CLOAK;
		setSymbol('(');
	}
	else if (t == "GLOVES")
	{
		type |= ITEM_GLOVES;
		setSymbol('{');
	}
	else if (t == "BOOTS")
	{
		type |= ITEM_BOOTS;
		setSymbol('\\');
	}
	else if(t == "RING")
	{
		type |= ITEM_RING;
		setSymbol('=');
	}
	else if (t == "AMULET")
	{
		type |= ITEM_AMULET;
		setSymbol('\"');
	}
	else if (t == "LIGHT")
	{
		type |= ITEM_LIGHT;
		setSymbol('_');
	}
	else if (t == "SCROLL")
	{
		type |= ITEM_SCROLL;
		setSymbol('~');
	}
	else if (t == "BOOK")
	{
		type |= ITEM_BOOK;
		setSymbol('?');
	}
	else if (t == "FLASK")
	{
		type |= ITEM_FLASK;
		setSymbol('!');
	}
	else if (t == "GOLD")
	{
		type |= ITEM_GOLD;
		setSymbol('$');
	}
	else if (t == "AMMUNITION")
	{
		type |= ITEM_AMMO;
		setSymbol('/');
	}
	else if (t == "FOOD")
	{
		type |= ITEM_FOOD;
		setSymbol(',');
	}
	else if (t == "WAND")
	{
		type |= ITEM_WAND;
		setSymbol('-');
	}
	else if (t == "CONTAINER")
	{
		type |= ITEM_CONTAINER;
		setSymbol('%');
	}
	else
	{
		std::cerr << "Invalid type: " << t << std::endl;
		return;
	}
	typeSet = true;
}

short ItemDef::getColor()
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
	colorSet = true;
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
	unsigned int i;
	for(i = 0; i<definitions.size(); i++)
	{
		delete definitions[i];
	}
}
