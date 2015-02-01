#include<stdlib.h>
#include<stdio.h>//TODO TEMPORARY

//used for the enum, which will be used globally
#include "generator.h"

void initializeDungeon(void);
void saveRoom(room_t room);
void generateAllRooms(void);
int generateRoom(void);
int canPlaceRoom(room_t room);
void connectAllRooms(void);
int findClosestRoom(int i, int connected[30], int isConnected);
void connectRooms(room_t room1, room_t room2);
int inRoom(int x, int y, room_t room);
int isConnected(int i, int connected[30]);

//TODO temporary functions
void printRooms(room_list_t list);

//this is the main function for this .c file.
void generateDungeon()
{
	initializeDungeon();
	dungeon.list.count = 0;
	generateAllRooms();
	connectAllRooms();
	return;
}

void initializeDungeon()
{
	int x, y;
	for(x=0;x<160;x++)
	{
		for(y=0;y<96;y++)
		{
			dungeon.map[x][y] = 0;
		}
	}
	for(x=0;x<160;x++)
	{
		dungeon.map[x][0] = 1;
		dungeon.map[x][95] = 1;
	}
	for(y=1;y<95;y++)
	{
		dungeon.map[0][y] = 1;
		dungeon.map[159][y] = 1;
	}
}

void generateAllRooms()
{
	int failures = 0;
	while((dungeon.list.count<MIN_ROOMS
		||failures<MAX_PLACEMENT_ATTEMPTS)
		&&dungeon.list.count<MAX_ROOMS)
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

int generateRoom()
{
	room_t room; 
	room.x = random()%DUNGEON_X;
	room.y = random()%DUNGEON_Y;
	int roomSize = random()%40;//TODO I could maybe adjust the base size to be a bit smaller.
	//A 40x40 room is pretty big IMO.
	room.w = roomSize + random()%10 + 8;
	room.h = roomSize + random()%8 + 5;
	if(!canPlaceRoom(room))
	{
		saveRoom(room);
		return 0;
	}
	return 1;
}

void saveRoom(room_t room) 
{
	int x, y;
	for(x=room.x;x<room.x+room.w;x++)
	{
		for(y=room.y;y<room.y+room.h;y++)
		{
			dungeon.map[x][y] = 2;
		}
	}
	dungeon.list.list[dungeon.list.count]=room;
	dungeon.list.count+=1;
	return;
}

int canPlaceRoom(room_t room) 
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
			if(dungeon.map[x][y]!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}

void connectAllRooms()
{
	int i;
	int connected[30];
	for(i=0;i<30;i++)
	{
		connected[i]=0;
	}
	connected[0] = 1;
	for(i=0;i<dungeon.list.count;i++)
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
		connectRooms(dungeon.list.list[closest], dungeon.list.list[closestConnected]);
		connected[closest]=1;
	}
}

//TODO this function has an error in it somewhere, but I don't know where.
int findClosestRoom(int i, int connected[30], int isConnected)
{
	int x;
	int closest = -1;//If I don't initialize this I get a seg fault. 
	//Default value is like 80 or something with seed 123456789
	int minDistSqr = 999999;
	int cX = dungeon.list.list[i].x + (dungeon.list.list[i].w/2);
	int cY = dungeon.list.list[i].y + (dungeon.list.list[i].h/2);
	for(x=dungeon.list.count-1;x>=0;x--)
	{
		if(connected[x]==isConnected)
		{
			continue;
		}
		int centerX = dungeon.list.list[x].x+(dungeon.list.list[x].w/2);
		int centerY = dungeon.list.list[x].y+(dungeon.list.list[x].h/2);
		int dx = centerX-cX;
		int dy = centerY-cY;
		int distSqr=((dx*dx)+(dy*dy));
		if(distSqr<minDistSqr)
		{
			closest = x;
			minDistSqr = distSqr;
		}
	}
	return closest;
}

void connectRooms(room_t room1, room_t room2)
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
				dungeon.map[++x][y] = 2;
			}
		}
		else if(x>room2.x+(room2.w/2))
		{
			for(i=0;i<d;i++)
			{
				dungeon.map[--x][y] = 2;
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
						dungeon.map[++x][y] = 2;
					}
					break;
				case 2:
					for(i=0;i<d;i++)
					{
						dungeon.map[--x][y] = 2;
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
				dungeon.map[x][++y] = 2;
			}
		}
		else if(y>room2.y+(room2.h/2))
		{
			for(i=0; i<d;i++)
			{
				dungeon.map[x][--y] = 2;
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
						dungeon.map[x][++y] = 2;
					}
					break;
				case 2:
					for(i=0;i<d;i++)
					{
						dungeon.map[x][--y] = 2;
					}
					break;
				default:
					break;
			}
		}
	}
}

int inRoom(int x, int y, room_t room)
{
	if(x<=room.x||x>=room.x+room.w-1||y<=room.y||y>=room.y+room.h-1)
	{
		return 1;
	}
	return 0;
}
