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

void initializeDungeon(DungeonTile **map);
int saveRoom(Room room, DungeonTile **map);
void generateAllRooms(DungeonTile **map);
int generateRoom(DungeonTile **map);
int canPlaceRoom(Room room, DungeonTile **map);

void generateDungeon(DungeonTile **map)
{
	initializeDungeon(map);
	generateAllRooms(map);
	return;
}

void initializeDungeon(DungeonTile **map)
{
	int x, y;
	for(x=0;x<160;x++)
	{
		for(y=0;y<96;y++)
		{
			map[x][y] = ROCK;
		}
	}
	for(x=0;x<160;x++)
	{
		map[x][0] = EDGE;
		map[x][95] = EDGE;
	}
	for(y=1;y<95;y++)
	{
		map[0][y] = EDGE;
		map[159][y] = EDGE;
	}
}

void generateAllRooms(DungeonTile **map)
{
	int roomsGenerated = 0,failures = 0;
	while(roomsGenerated<12||failures<2000)
	{
		if(generateRoom(map))
		{
			roomsGenerated++;
		}
		else
		{
			failures++;
		}
	}
}

int generateRoom(DungeonTile **map)
{
	//TODO I should not have printf's here
	Room room; 
	room.x = random()%160;
	room.y = random()%96;
	int roomSize = random()%40;
	room.w = roomSize + random()%10 + 8;//8-48
	room.h = roomSize + random()%8 + 5;//5-41
	printf("Generating room at %d, %d with size %d, %d: ", room.x, room.y, room.w, room.h);
	int saved = saveRoom(room, map);
	if(saved)
	{
		printf("Failed\n");
	}
	else
	{
		printf("Succeeded\n");
	}
	return saved;
}

int saveRoom(Room room, DungeonTile **map)
{
	if(!canPlaceRoom(room, map))
	{
		return 1;
	}
	int x, y;
	for(x=room.x;x<room.x+room.w;x++)
	{
		for(y=room.y;y<room.y+room.h;y++)
		{
			map[x][y] = FLOOR;
		}
	}
	return 0;
}

int canPlaceRoom(Room room, DungeonTile **map)
{
	if(room.x<3||room.y<3||room.x+room.w+3>160||room.y+room.h+3>96)
	{
		return 0;
	}
	int x,y;
	for(x=room.x-3;x<room.x+room.w+3;x++)
	{
		for(y=room.y-3;y<room.y+room.h+3;y++)
		{
			if(map[x][y]!=ROCK)
			{
				return 0;
			}
		}
	}
	return 1;
}
