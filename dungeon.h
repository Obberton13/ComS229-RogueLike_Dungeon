#define DUNGEON_X 160
#define DUNGEON_Y 96
#define MIN_ROOMS 12
#define MAX_ROOMS 30
#define ROOM_MIN_W 8
#define ROOM_MIN_H 5
#define ROOM_SEPARATION 3
#define MAX_PLACEMENT_ATTEMPTS 2000
#define MAX_MONSTERS 20
#define USAGE "Usage: DungeonGame [-s <seed>][--save][--load][--nummon <monsters>]\n"

typedef enum terrain_tile {
	ter_rock,
	ter_immutable,
	ter_room,
	ter_corridor,
	ter_debug,
	ter_debug2,
	ter_debug3
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
	int x, y;
	char displayChar;
	unsigned int flags;
	char speed, initiative;
} monster_t;

typedef struct monster_list {
	monster_t list[MAX_MONSTERS];
	int count;
} monster_list_t;

typedef struct dungeon {
	terrain_cell_t **map;
	room_list_t rooms;
	monster_list_t monsters;
} dungeon_t;

extern dungeon_t dungeon;

void generateDungeon(void);
void dungeon_init(void);
