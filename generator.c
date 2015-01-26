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
void saveRoom(Room room, int **map, RoomsList *list);
void generateAllRooms(int **map, RoomsList list);
int generateRoom(int **map, RoomsList *list);
int canPlaceRoom(Room room, int **map);
void connectAllRooms(int **map, RoomsList *list);
Room findClosestConnectedRoom(int i, RoomsList *list);
void connectRooms(Room room1, Room room2);

//TODO temporary functions
void printRooms(RoomsList list);

//this is the main function for this .c file.
void generateDungeon(int **map)
{
	initializeDungeon(map);
	RoomsList list;
	list.count = 0;
	generateAllRooms(map, list);
	connectAllRooms(map, &list);
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
	int failures = 0;
	while((list.count<12||failures<2000)&&list.count<30)
	{
		if(!generateRoom(map, &list))
		{
			failures = 0;
		}
		else
		{
			failures++;
		}
	}
}

int generateRoom(int **map, RoomsList *list)
{
	Room room; 
	room.x = random()%160;
	room.y = random()%96;
	int roomSize = random()%40;
	room.w = roomSize + random()%10 + 8;//8-48
	room.h = roomSize + random()%8 + 5;//5-41
	if(!canPlaceRoom(room, map))
	{
		printf("Generated room at (%d, %d) with size (%d, %d)\n",
			room.x, room.y, room.w, room.h);
		printf("Room number %d\n", list->count);
		saveRoom(room, map, list);
		return 0;
	}
	return 1;
}

void saveRoom(Room room, int **map, RoomsList *list) //1 on fail, 0 on success
{
	int x, y;
	for(x=room.x;x<room.x+room.w;x++)
	{
		for(y=room.y;y<room.y+room.h;y++)
		{
			map[x][y] = 2;
		}
	}
	list->list[list->count]=room;
	list->count+=1;
	printf("%d\n", list->count);
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

void connectAllRooms(int **map, RoomsList *list)
{
	int i;
	for(i=1;i<list->count;i++)
	{
		Room closest = findClosestConnectedRoom(i, list);
		connectRooms(closest, list->list[i]);
	}
}

Room findClosestConnectedRoom(int i, RoomsList *list)
{
	int x;
	Room closest;
	int minDistSqr = 999999;
	int cX = list->list[i].x + (list->list[i].w/2);
	int cY = list->list[i].y + (list->list[i].h/2);
	for(x=0;x<i;x++)
	{
		int centerX = list->list[x].x+(list->list[x].w/2);
		int centerY = list->list[x].y+(list->list[x].h/2);
		int dx = centerX-cX;
		int dy = centerY-cY;
		int distSqr=((dx*dx)+(dy*dy));
		if(distSqr<minDistSqr)
		{
			closest = list->list[x];
			minDistSqr = distSqr;
		}
	}
	return closest;
}
