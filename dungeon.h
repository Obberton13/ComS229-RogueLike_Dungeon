#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_X 160
#define DUNGEON_Y 96
#define MIN_ROOMS 12
#define MAX_ROOMS 30
#define ROOM_MIN_W 8
#define ROOM_MIN_H 5
#define ROOM_SEPARATION 3
#define MAX_PLACEMENT_ATTEMPTS 2000
#define USAGE "Usage: DungeonGame [-s <seed>][--save][--load][--nummon <monsters>]\n"

#define MONSTER_SMART 0x00000001
#define MONSTER_TELEP 0x00000002
#define MONSTER_BIT03 0x00000004
#define MONSTER_BIT04 0x00000008
#define MONSTER_BIT05 0x00000010
#define MONSTER_BIT06 0x00000020
#define MONSTER_BIT07 0x00000040
#define MONSTER_BIT08 0x00000080
#define MONSTER_BIT09 0x00000100
#define MONSTER_BIT10 0x00000200
#define MONSTER_BIT11 0x00000400
#define MONSTER_BIT12 0x00000800
#define MONSTER_BIT13 0x00001000
#define MONSTER_BIT14 0x00002000
#define MONSTER_BIT15 0x00004000
#define MONSTER_BIT16 0x00008000
#define MONSTER_BIT17 0x00010000
#define MONSTER_BIT18 0x00020000
#define MONSTER_BIT19 0x00040000
#define MONSTER_BIT20 0x00080000
#define MONSTER_BIT21 0x00100000
#define MONSTER_BIT22 0x00200000
#define MONSTER_BIT23 0x00400000
#define MONSTER_BIT24 0x00800000
#define MONSTER_BIT25 0x01000000
#define MONSTER_BIT26 0x02000000
#define MONSTER_BIT27 0x04000000
#define MONSTER_BIT28 0x08000000
#define MONSTER_BIT29 0x10000000
#define MONSTER_BIT30 0x20000000
#define MONSTER_BIT31 0x40000000
#define MONSTER_BIT32 0x80000000

typedef enum terrain_tile {
	ter_rock,
	ter_immutable,
	ter_room,
	ter_corridor,
	ter_stair_up,
	ter_stair_down,
	ter_debug
} terrain_tile_t;

typedef struct terrain_cell {
	terrain_tile_t tile;
	int distToPlayer;
	unsigned char hardness;
	int monsterIndex;
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

typedef struct monster {
	int x, y, px, py;
	char displayChar;
	unsigned int flags;
	char speed, initiative;
} monster_t;

typedef struct monster_list {
	monster_t *list;
	int count;
	int max;
} monster_list_t;

typedef struct dungeon {
	terrain_cell_t **map;
	room_list_t rooms;
	monster_list_t monsters;
} dungeon_t;

extern dungeon_t dungeon;

void generateDungeon(void);
void dungeon_init(void);
void dungeon_free(void);

#endif
