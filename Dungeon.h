#pragma once
#include <iostream>
#include <vector>

#include "DungeonTile.h"

#define DUNGEON_X 160
#define DUNGEON_Y 96
#define MIN_ROOMS 12
#define MAX_ROOMS 30
#define ROOM_MIN_W 8
#define ROOM_MIN_H 5
#define ROOM_SEPARATION 3
#define MAX_ROOM_ATTEMPTS 2000
#define MAX_MONSTERS 30
#define MAX_ITEMS 30

class Dungeon
{
public:
	Dungeon();
	~Dungeon();
	void generate();
	int game_loop();
//	int save();

	friend std::ostream &operator<<(std::ostream &o, const Dungeon &d);

private:
	struct room
	{
		unsigned char x;
		unsigned char y;
		unsigned char h;
		unsigned char w;
		unsigned char size;
	};
	DungeonTile **map;
	std::vector<struct room> rooms;
	std::vector<NPC *> npcs;
	std::vector<Item *> items;
	void initialize();
	void generate_rooms();
	int generate_room();
	void connect_all_rooms();
	unsigned char get_closest_room(unsigned char roomIndex, bool connArray[]);
	void connect_rooms(unsigned char rm1, unsigned char rm2);
	void spawn_player();
	void spawn_monsters();
	void spawn_items();
	void find_monster_distances();
	int move_monster(unsigned int index, unsigned char x, unsigned char y);
};

std::ostream &operator<<(std::ostream &o, const Dungeon &d);