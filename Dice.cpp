#include "Dice.h"
#include <sstream>
#include <boost/regex.hpp>


Dice::Dice()
{
	this->base = 0; 
	this->number = 0;
	this->sides = 0;
	this->assigned = false;
}

Dice::Dice(const std::string &str)
{
	boost::regex regex("^([0-9]+)\\+([0-9]+)d([0-9]+)$");
	boost::cmatch matches;
	boost::regex_match(str.c_str(), matches, regex);
	std::istringstream(matches[1]) >> this->base;
	std::istringstream(matches[2]) >> this->number;
	std::istringstream(matches[3]) >> this->sides;
	this->assigned = true;
}

Dice::Dice(int base, int number, int sides)
{
	this->base = base;
	this->number = number;
	this->sides = sides;
}


Dice::~Dice()
{
}

Dice &Dice::operator=(const Dice &other)
{
	this->base = other.base;
	this->number = other.number;
	this->sides = other.sides;
	this->assigned = other.assigned;
	return *this;
}

unsigned int Dice::Roll()
{
	unsigned int result = base, i;
	for(i = 0; i < number; i++)
	{
		result += (random() % sides) + 1;//somehow, this works even when I don't include <random>
	}
	return result;
}

std::ostream &operator<<(std::ostream &o, const Dice &d)
{
	return o << d.base << "+" << d.number << "d" << d.sides;
}
