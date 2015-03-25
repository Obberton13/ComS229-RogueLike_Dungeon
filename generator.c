#include<stdlib.h>
#include<limits.h>
#include<stdio.h>//TODO TEMPORARY

//used for the enum, which will be used globally
#include "dungeon.h"

static void saveRoom(room_t room);
static void generateAllRooms(void);
static int generateRoom(void);
static int canPlaceRoom(room_t room);
static void connectAllRooms(void);
static void generateStairs(void);
static int findClosestRoom(int i, int connected[30], int isConnected);
static void connectRooms(room_t room1, room_t room2);
static int inRoom(int x, int y, room_t room);
static void spawnAllMonsters();

//this is the main function for this .c file.
void generateDungeon()
{
	generateAllRooms();
	connectAllRooms();
	generateStairs();
	spawnAllMonsters();
	return;
}

void dungeon_init()
{
	int x, y;
	//initialize the rooms part of the dungeon
	dungeon.rooms.count = 0;
	for(x=0;x<MAX_ROOMS;x++)
	{
		dungeon.rooms.list[x].x = 0;
		dungeon.rooms.list[x].y = 0;
		dungeon.rooms.list[x].w = 0;
		dungeon.rooms.list[x].h = 0;
		dungeon.rooms.list[x].size = 0;
	}
	//TODO initialize the monsters part of the dungeon
	dungeon.monsters.count = 0;
	dungeon.monsters.list = malloc(sizeof(monster_t)*dungeon.monsters.max);
	for(x=0;x<dungeon.monsters.max;x++)
	{
		dungeon.monsters.list[x].x = 0;
		dungeon.monsters.list[x].y = 0;
		dungeon.monsters.list[x].displayChar = 0;
		dungeon.monsters.list[x].flags = 0;
	}
	//initialize the map part of the dungeon
	dungeon.map = (terrain_cell_t**) malloc(sizeof(terrain_cell_t*)*DUNGEON_X);
	for(x=0;x<DUNGEON_X;x++)
	{
		dungeon.map[x] = (terrain_cell_t*) malloc(sizeof(terrain_cell_t)*DUNGEON_Y);
	}
	for(x=0;x<DUNGEON_X;x++)
	{
		for (y=0;y<DUNGEON_Y;y++)
		{
			dungeon.map[x][y].tile = ter_rock;
			dungeon.map[x][y].hardness = random() % 254+1;
			dungeon.map[x][y].monsterIndex = dungeon.monsters.max;
			dungeon.map[x][y].distToPlayer = INT_MAX;
		}
	}
	for(x=0;x<DUNGEON_X;x++)
	{
		dungeon.map[x][0].tile = ter_immutable;
		dungeon.map[x][0].hardness = 255;
		dungeon.map[x][95].tile = ter_immutable;
		dungeon.map[x][95].hardness = 255;
	}
	for(y=1;y<DUNGEON_Y;y++)
	{
		dungeon.map[0][y].tile = ter_immutable;
		dungeon.map[0][y].hardness = 255;
		dungeon.map[159][y].tile = ter_immutable;
		dungeon.map[159][y].hardness = 255;
	}
}

void dungeon_free()
{
	int x;
	for(x=0;x<DUNGEON_X;x++)
	{
		free(dungeon.map[x]);
	}
	free(dungeon.map);
	free(dungeon.monsters.list);
}

static void generateAllRooms()
{
	int failures = 0;
	while((dungeon.rooms.count<MIN_ROOMS
		||failures<MAX_PLACEMENT_ATTEMPTS)
		&&dungeon.rooms.count<MAX_ROOMS)
	{
		if(!generateRoom())
		{
			failures = 0;
		}
		else
		{
			failures++;
		}
	}
}

static int generateRoom()
{
	room_t room; 
	room.x = random()%DUNGEON_X;
	room.y = random()%DUNGEON_Y;
	room.size = random()%25;
	room.w = room.size + random()%10 + ROOM_MIN_W;
	room.h = room.size + random()%8 + ROOM_MIN_H;
	if(!canPlaceRoom(room))
	{
		saveRoom(room);
		return 0;
	}
	return 1;
}

static void saveRoom(room_t room) 
{
	int x, y;
	for(x=room.x;x<room.x+room.w;x++)
	{
		for(y=room.y;y<room.y+room.h;y++)
		{
			dungeon.map[x][y].tile = ter_room;
		}
	}
	dungeon.rooms.list[dungeon.rooms.count]=room;
	dungeon.rooms.count+=1;
	return;
}

static int canPlaceRoom(room_t room) 
{
	if(room.x<ROOM_SEPARATION
		||room.y<ROOM_SEPARATION
		||room.x+room.w+ROOM_SEPARATION>=DUNGEON_X
		||room.y+room.h+ROOM_SEPARATION>=DUNGEON_Y)
	{
		return 1;
	}
	int x,y;
	for(x=room.x-ROOM_SEPARATION;x<room.x+room.w+ROOM_SEPARATION;x++)
	{
		for(y=room.y-ROOM_SEPARATION;y<room.y+room.h+ROOM_SEPARATION;y++)
		{
			if(dungeon.map[x][y].tile!=ter_rock)
			{
				return 1;
			}
		}
	}
	return 0;
}

static void connectAllRooms()
{
	int i;
	int connected[MAX_ROOMS];
	for(i=0;i<MAX_ROOMS;i++)
	{
		connected[i]=0;
	}
	connected[0] = 1;
	for(i=0;i<dungeon.rooms.count;i++)
	{
		int closest = findClosestRoom(i, connected, 1);
		if(closest == -1)
		{
			continue;
		}
		int closestConnected = findClosestRoom(closest, connected, 0);
		if(closestConnected == -1)
		{
			continue;
		}
		connectRooms(dungeon.rooms.list[closest], dungeon.rooms.list[closestConnected]);
		connected[closest]=1;
	}
}

static void generateStairs()
{
	int roomId = random() % dungeon.rooms.count;
	room_t rm = dungeon.rooms.list[roomId];
	int x = rm.x + random() % rm.w;
	int y = rm.y + random() % rm.h;
	dungeon.map[x][y].tile = ter_stair_down;
	roomId = random() % dungeon.rooms.count;
	rm = dungeon.rooms.list[roomId];
	x = rm.x + random() % rm.w;
	y = rm.y + random() % rm.h;
	dungeon.map[x][y].tile = ter_stair_up;
}

static int findClosestRoom(int i, int connected[MAX_ROOMS], int isConnected)
{
	int x;
	int closest = -1;
	int minDist = INT_MAX; 
	int cX = dungeon.rooms.list[i].x + (dungeon.rooms.list[i].w/2);
	int cY = dungeon.rooms.list[i].y + (dungeon.rooms.list[i].h/2);
	for(x=dungeon.rooms.count-1;x>=0;x--)
	{
		if(connected[x]==isConnected)
		{
			continue;
		}
		int centerX = dungeon.rooms.list[x].x+(dungeon.rooms.list[x].w/2);
		int centerY = dungeon.rooms.list[x].y+(dungeon.rooms.list[x].h/2);
		int dx = centerX-cX;
		int dy = centerY-cY;
		int dist = ((dx*dx)+(dy*dy)); //technically sqr root, but that doesn't really matter
		if(dist<minDist)
		{
			closest = x;
			minDist = dist;
		}
	}
	return closest;
}

static void connectRooms(room_t room1, room_t room2)
{
	int x = room1.x+(room1.w/2);
	int y = room1.y+(room1.h/2);
	while(!inRoom(x, y, room1))
	{
		if(x<room2.x+(room2.w/2))
		{
			x++;
		}
		else if(x>room2.x+(room2.w/2))
		{
			x--;
		}
		if(y<room2.y+(room2.h/2))
		{
			y++;
		}
		else if(y>room2.y+(room2.h/2))
		{
			y--;
		}
	}
	while(inRoom(x,y, room2))
	{
		int i;
		int d = random() % 4 + 2;
		if(x<room2.x+(room2.w/2))
		{
			for(i=0;i<d;i++)
			{
				x++;
				if(dungeon.map[x][y].tile!=ter_room)
				{
					dungeon.map[x][y].tile = ter_corridor;
				}
			}
		}
		else if(x>room2.x+(room2.w/2))
		{
			for(i=0;i<d;i++)
			{
				--x;
				if(dungeon.map[x][y].tile!=ter_room)
				{
					dungeon.map[x][y].tile = ter_corridor;
				}
			}
		}
		else
		{
			int r = random() % 3;
			d/=2;
			switch(r)
			{
				case 1:
					for(i=0;i<d;i++)
					{
						++x;
						if(dungeon.map[x][y].tile!=ter_room)
						{
							dungeon.map[x][y].tile = ter_corridor;
						}
					}
					break;
				case 2:
					for(i=0;i<d;i++)
					{
						--x;
						if(dungeon.map[x][y].tile!=ter_room)
						{
							dungeon.map[x][y].tile = ter_corridor;
						}
					}
					break;
				default:
					break;
			}
		}
		d = random() % 4 + 2;
		if(y<room2.y+(room2.h/2))
		{
			for(i=0;i<d;i++)
			{
				++y;
				if(dungeon.map[x][y].tile!=ter_room)
				{
					dungeon.map[x][y].tile = ter_corridor;
				}
			}
		}
		else if(y>room2.y+(room2.h/2))
		{
			for(i=0; i<d;i++)
			{
				--y;
				if(dungeon.map[x][y].tile!=ter_room)
				{
					dungeon.map[x][y].tile = ter_corridor;
				}
			}
		}
		else
		{
			int r = random() % 3;
			d /= 2;
			switch(r)
			{
				case 1:
					for(i=0;i<d;i++)
					{
						++y;
						if(dungeon.map[x][y].tile!=ter_room)
						{
							dungeon.map[x][y].tile = ter_corridor;
						}
					}
					break;
				case 2:
					for(i=0;i<d;i++)
					{
						--y;
						if(dungeon.map[x][y].tile!=ter_room)
						{
							dungeon.map[x][y].tile = ter_corridor;
						}
					}
					break;
				default:
					break;
			}
		}
	}
}

static int inRoom(int x, int y, room_t room)
{
	if(x<=room.x||x>=room.x+room.w-1||y<=room.y||y>=room.y+room.h-1)
	{
		return 1;
	}
	return 0;
}

static void spawnAllMonsters()
{
	int i;
	for(i=0;i<dungeon.monsters.max;i++)
	{
		int room = rand()%dungeon.rooms.count;
		int x = (rand()%dungeon.rooms.list[room].w)+dungeon.rooms.list[room].x;
		int y = (rand()%dungeon.rooms.list[room].h)+dungeon.rooms.list[room].y;
		dungeon.map[x][y].monsterIndex = dungeon.monsters.count;
		dungeon.monsters.list[dungeon.monsters.count].x = x;
		dungeon.monsters.list[dungeon.monsters.count].y = y;
		dungeon.monsters.list[dungeon.monsters.count].px = x;
		dungeon.monsters.list[dungeon.monsters.count].py = y;
		if(dungeon.monsters.count)
		{
			char dispChar = rand()%('z'-'a')+'a';
			char speed = 100/(rand()%11+5);
			dungeon.monsters.list[dungeon.monsters.count].displayChar = dispChar;
			dungeon.monsters.list[dungeon.monsters.count].flags = rand();
			dungeon.monsters.list[dungeon.monsters.count].speed = speed;
			dungeon.monsters.list[dungeon.monsters.count].initiative = speed;
		}
		else
		{
			dungeon.monsters.list[dungeon.monsters.count].flags = rand();
			dungeon.monsters.list[dungeon.monsters.count].displayChar = '@';
			dungeon.monsters.list[dungeon.monsters.count].speed = 10;
			dungeon.monsters.list[dungeon.monsters.count].initiative = 10;
		}
		dungeon.monsters.list[dungeon.monsters.count].sequence_num = dungeon.monsters_generated;
		dungeon.monsters.count++;
		dungeon.monsters_generated++;
	}
}
