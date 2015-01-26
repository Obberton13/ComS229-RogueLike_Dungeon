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
void generateAllRooms(int **map, RoomsList *list);
int generateRoom(int **map, RoomsList *list);
int canPlaceRoom(Room room, int **map);
void connectAllRooms(int **map, RoomsList *list);
int findClosestRoom(int i, RoomsList *list, int connected[30], int isConnected);
void connectRooms(Room room1, Room room2, int **map);
int inRoom(int x, int y, Room room);
int isConnected(int i, int connected[30]);

//TODO temporary functions
void printRooms(RoomsList list);

//this is the main function for this .c file.
void generateDungeon(int **map)
{
	initializeDungeon(map);
	RoomsList list;
	list.count = 0;
	generateAllRooms(map, &list);
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

void generateAllRooms(int **map, RoomsList *list)
{
	int failures = 0;
	while((list->count<12||failures<2000)&&list->count<30)
	{
		if(!generateRoom(map, list))
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
	room.w = roomSize + random()%10 + 8;
	room.h = roomSize + random()%8 + 5;
	if(!canPlaceRoom(room, map))
	{
		saveRoom(room, map, list);
		return 0;
	}
	return 1;
}

void saveRoom(Room room, int **map, RoomsList *list) 
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
	return;
}

int canPlaceRoom(Room room, int **map) 
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
	int connected[30];
	for(i=0;i<30;i++)
	{
		connected[i]=0;
	}
	connected[0] = 1;
	for(i=0;i<list->count-1;i++)
	{
		
		//find the closest connected room to the closest unconnected room to room 0
		//closest unconnected room to room 0:
		int closest = findClosestRoom(i, list, connected, 1);//keep in mind 0 is connected
		if(closest == -1)
		{
			continue;
		}
		printf("Found closest unconnected room %d\n", closest);
		//closest connected room:
		int closestConnected = findClosestRoom(closest, list, connected, 0);
		if(closestConnected == -1)//findClosestRoom should always be able to find a connected room.
		{
			continue;
		}
		printf("Found closest connected room %d\n", closestConnected);
		connectRooms(list->list[closest], list->list[closestConnected], map);
		connected[closest]=1;
	}
}

//TODO this function has an error in it somewhere, but I don't know where.
int findClosestRoom(int i, RoomsList *list, int connected[30], int isConnected)
{
	int x;
	int closest = -1;//If I don't initialize this I get a seg fault. 
	//Default value is like 80 or something with seed 123456789
	int minDistSqr = 999999;
	int cX = list->list[i].x + (list->list[i].w/2);
	int cY = list->list[i].y + (list->list[i].h/2);
	for(x=list->count-1;x>=0;x--)
	{
		printf("Entered for loop\n");
		if(connected[x]==isConnected)
		{
			continue;
		}
		printf("Didn't use a continue on %d\n", x);
		int centerX = list->list[x].x+(list->list[x].w/2);
		int centerY = list->list[x].y+(list->list[x].h/2);
		int dx = centerX-cX;
		int dy = centerY-cY;
		int distSqr=((dx*dx)+(dy*dy));
		if(distSqr<minDistSqr)
		{
			printf("New smallest: %d\n", x);
			closest = x;
			minDistSqr = distSqr;
		}
	}
	return closest;
}

void connectRooms(Room room1, Room room2, int **map)
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
		if(x<room2.x+(room2.w/2))
		{
			x++;
		}
		else if(x>room2.x+(room2.w/2))
		{
			x--;
		}
		map[x][y]=2;
		if(y<room2.y+(room2.h/2))
		{
			y++;
		}
		else if(y>room2.y+(room2.h/2))
		{
			y--;
		}
		map[x][y]=2;
	}
}

int inRoom(int x, int y, Room room)
{
	if(x<=room.x||x>=room.x+room.w-1||y<=room.y||y>=room.y+room.h-1)
	{
		return 1;
	}
	return 0;
}
