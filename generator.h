#define DUNGEON_X 160
#define DUNGEON_Y 96
#define MIN_ROOMS 12
#define MAX_ROOMS 30
#define ROOM_MIN_W 8
#define ROOM_MIN_H 5
#define ROOM_SEPARATION 3
#define MAX_PLACEMENT_ATTEMPTS 2000

typedef enum terrain_tile {
	ter_rock_hard,
	ter_rock_soft,
	ter_rock_immutable,
	ter_floor	
} terrain_tile_t;

typedef struct room {
	int x;
	int y;
	int w;
	int h;
} room_t;

typedef struct room_list {
	int count;
	room_t list[MAX_ROOMS];
} room_list_t;

typedef struct dungeon {
	char** map;
	room_list_t list;
} dungeon_t;

extern dungeon_t dungeon;

void generateDungeon(void);
