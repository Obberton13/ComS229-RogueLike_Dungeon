#pragma once
#include <iostream>
#include <vector>

#include "DungeonTile.h"
#include "PC.h"

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
#define SCREEN_W 80
#define SCREEN_H 24

class Dungeon
{
public:
	Dungeon(PC* pc); 
	~Dungeon();
	void generate();
	int game_loop();

	friend std::ostream &operator<<(std::ostream &o, const Dungeon &d);

private:
	PC* pc;
	char scrX, scrY;
	typedef enum game_state {
		GAME_CONTROL = 0,
		GAME_LOOK = 1,
		GAME_INVENTORY = 2,
		GAME_DEATH = 3,
		GAME_QUIT = 4
	} game_state_t;
	struct room
	{
		unsigned char x;
		unsigned char y;
		unsigned char h;
		unsigned char w;
		unsigned char size;
	};
	game_state_t current_state;
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
	void move_monster(NPC* npc, unsigned char x, unsigned char y);
	void print();
	void putchar(unsigned char x, unsigned char y, unsigned char posX, unsigned char posY);
	int player_input();
	int push_monster(NPC* npc, bool isFirst);
	int look_action();
	int control_action();
	int move_player(char x, char y);
	int death_action();
	int quit_action();
	int equip_action();
	int unequip_action();
	int drop_action();
	int expunge_action();
	void quit();
};

std::ostream &operator<<(std::ostream &o, const Dungeon &d);
