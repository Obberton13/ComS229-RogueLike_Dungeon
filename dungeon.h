#ifndef DUNGEON_H
#define DUNGEON_H

#define WALL_MAX_HARDNESS 255
#define DUNGEON_X 160
#define DUNGEON_Y 96
#define MIN_ROOMS 12
#define MAX_ROOMS 30
#define ROOM_MIN_W 8
#define ROOM_MIN_H 5
#define ROOM_SEPARATION 3
#define MAX_PLACEMENT_ATTEMPTS 2000

#define MAX_PATHFINDING_DISTANCE 30

#define NUM_MONSTER_TYPES 10
#define MAX_MONSTERS 30
#define MAX_MONSTER_PLACEMENT_ATTEMPTS 100

typedef enum terrain_tile {
	ter_rock,
	ter_immutable,
	ter_room,
	ter_corridor,
} terrain_tile_t;

typedef struct monster {
	int type; 
	char toDisplay;
} monster_t;

typedef struct monster_list {
	monster_t list[MAX_MONSTERS];
	int count;
} monster_list;

typedef struct terrain_cell {
	terrain_tile_t tile;
	int monsterIndex;
	unsigned char hardness;
} terrain_cell_t;

typedef struct room {
	unsigned char x;
	unsigned char y;
	unsigned char w;
	unsigned char h;
	unsigned char size;
} room_t;

typedef struct room_list {
	unsigned short count;
	room_t list[MAX_ROOMS];
} room_list_t;

typedef struct dungeon {
	terrain_cell_t** map;
	room_list_t list;
} dungeon_t;

typedef struct path_cell {
	int x, y, dist;
} path_cell_t;

extern dungeon_t dungeon;
extern monster_list monsters;

void generateDungeon(void);
void initializeDungeon();

#endif
