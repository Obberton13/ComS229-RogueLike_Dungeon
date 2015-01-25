#include<stdlib.h>
#include<stdio.h>//TODO TEMPORARY

//used for the enum, which will be used globally
#include "generator.h"

typedef struct Room {
	int x;
	int y;
	int w;
	int h;
} Room;

typedef struct RoomsList {
	int count;
	Room list[30];
} RoomsList;

void initializeDungeon(int **map);
void saveRoom(Room room, int **map, RoomsList list);
void generateAllRooms(int **map, RoomsList list);
int generateRoom(int **map, RoomsList list);
int canPlaceRoom(Room room, int **map);

void generateDungeon(int **map)
{
	initializeDungeon(map);
	RoomsList list;
	generateAllRooms(map, list);
	
	return;
}

void initializeDungeon(int **map)
{
	int x, y;
	for(x=0;x<160;x++)
	{
		for(y=0;y<96;y++)
		{
			map[x][y] = 0;
		}
	}
	for(x=0;x<160;x++)
	{
		map[x][0] = 1;
		map[x][95] = 1;
	}
	for(y=1;y<95;y++)
	{
		map[0][y] = 1;
		map[159][y] = 1;
	}
}

void generateAllRooms(int **map, RoomsList list)
{
	int roomsGenerated = 0,failures = 0;
	while((roomsGenerated<12||failures<2000)&&list.count<30)
	{
		if(!generateRoom(map, list))
		{
			printf("Successfully generated room %d\n", ++roomsGenerated);
			//roomsGenerated++;
			failures = 0;
		}
		else
		{
			failures++;
		}
	}
}

int generateRoom(int **map, RoomsList list)
{
	Room room; 
	room.x = random()%160;
	room.y = random()%96;
	int roomSize = random()%40;
	room.w = roomSize + random()%10 + 8;//8-48
	room.h = roomSize + random()%8 + 5;//5-41
	if(!canPlaceRoom(room, map))
	{
		saveRoom(room, map, list);
		return 0;
	}
	return 1;
}

void saveRoom(Room room, int **map, RoomsList list) //1 on fail, 0 on success
{
	int x, y;
	for(x=room.x;x<room.x+room.w;x++)
	{
		for(y=room.y;y<room.y+room.h;y++)
		{
			map[x][y] = 2;
		}
	}
	list.list[list.count++]=room;
	return;
}

int canPlaceRoom(Room room, int **map) //0 on can't, 1 on can.
{
	if(room.x<3||room.y<3||room.x+room.w+3>=160||room.y+room.h+3>=96)
	{
		return 1;
	}
	int x,y;
	for(x=room.x-3;x<room.x+room.w+3;x++)
	{
		for(y=room.y-3;y<room.y+room.h+3;y++)
		{
			if(map[x][y]!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}
