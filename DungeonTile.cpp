#include "DungeonTile.h"
#include <climits>
#include <cstdlib>
#include <iostream>

DungeonTile::DungeonTile()
{
	hardness = UCHAR_MAX;
	dist_to_player = USHRT_MAX;
	monster = NULL;
	item = NULL;
	type = ter_immutable;
}

DungeonTile::~DungeonTile() {}

int DungeonTile::compare(DungeonTile other)
{
	return this->hardness < other.hardness;
}

void DungeonTile::set_type(terrain_type_t type)
{
	terrain_type_t t = type;
	this->type = t;
}

terrain_type_t DungeonTile::get_type()
{
	return this->type;
}

unsigned char DungeonTile::get_hardness()
{
	return hardness;
}

void DungeonTile::set_hardness(unsigned char hardness)
{
	this->hardness = hardness;
}

unsigned short DungeonTile::get_dist_to_player()
{
	return this->dist_to_player;
}

void DungeonTile::set_dist_to_player(unsigned short dist)
{
	this->dist_to_player = dist;
}

NPC* DungeonTile::get_monster()
{
	return monster;
}

void DungeonTile::set_monster(NPC *npc)
{
	monster = npc;
}

std::ostream &operator<<(std::ostream &o, const DungeonTile d)
{
	if (d.monster)
	{
		o << *d.monster;
		return o;
	}
	if (d.item)
	{
		o << *d.item;
		return o;
	}
	switch (d.type)
	{
	case ter_corridor:
		o << ',';
		break;
	case ter_room:
		o << '.';
		break;
	case ter_immutable:
		o << '|';
		break;
	case ter_wall:
		o << '#';
		break;
	default:
		std::cerr << "Invalid dungeon tile" << std::endl;
	}
	return o;
}