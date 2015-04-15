#pragma once
//#include <cstdlib>
#include <iostream>
#include "NPC.h"
#include "Item.h"
typedef enum terrain_type
{
	ter_corridor,
	ter_room,
	ter_stair_up,
	ter_stair_down,
	ter_wall,
	ter_immutable,
	ter_debug
} terrain_type_t;

class DungeonTile
{
private:
	unsigned char hardness, x, y;
	unsigned short dist_to_player;
	NPC *monster;
	Item *item;
	terrain_type_t type;
public:
	DungeonTile();
	~DungeonTile();
	int compare(DungeonTile other);
	void set_type(terrain_type_t type);
	terrain_type_t get_type(void);
	unsigned char get_hardness();
	void set_hardness(unsigned char hardness);
	unsigned short get_dist_to_player();
	void set_dist_to_player(unsigned short dist);
	NPC* get_monster();
	void set_monster(NPC *npc);

	inline void setX(unsigned char x){ this->x = x; }
	inline void setY(unsigned char y){ this->y = y; }
	inline unsigned char getX() { return x; }
	inline unsigned char getY() { return y; }
	inline void setItem(Item *i) { this->item = i; }
	inline Item* getItem() { return item; }

	friend std::ostream &operator<<(std::ostream &o, const DungeonTile d);

	inline int operator<(const DungeonTile &other)
	{
		return this->dist_to_player < other.dist_to_player;
	}
	inline int operator>(const DungeonTile &other)
	{
		return this->dist_to_player > other.dist_to_player;
	}

	inline int operator<=(const DungeonTile &other)
	{
		return this->dist_to_player <= other.dist_to_player;
	}
	inline int operator>=(const DungeonTile &other)
	{
		return this->dist_to_player >= other.dist_to_player;
	}
	inline int operator==(const DungeonTile &other)
	{
		return this->dist_to_player == other.dist_to_player;
	}
	inline int operator!=(const DungeonTile &other)
	{
		return this->dist_to_player != other.dist_to_player;
	}
};

std::ostream &operator<<(std::ostream &o, const DungeonTile d);

