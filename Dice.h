#pragma once
#include <string>
#include <iostream>

class Dice
{
private:
	unsigned int base;
	unsigned int number;
	unsigned int sides;
	bool assigned;
public:
	Dice();
	Dice(int base, int number, int sides);
	Dice(const std::string &str);
	~Dice();

	Dice &operator=(const Dice &other);
	unsigned int Roll();
	inline bool isAssigned()
	{
		return this->assigned;
	}

	friend std::ostream &operator<<(std::ostream &o, const Dice &d);
};

std::ostream &operator<<(std::ostream &o, const Dice &d);
