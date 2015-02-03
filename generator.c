#include<stdlib.h>
#include<limits.h>
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
			dungeon.map[x][y] = ter_rock;
		}
	}
	for(x=0;x<160;x++)
	{
		dungeon.map[x][0] = ter_immutable;
		dungeon.map[x][95] = ter_immutable;
	}
	for(y=1;y<95;y++)
	{
		dungeon.map[0][y] = ter_immutable;
		dungeon.map[159][y] = ter_immutable;
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

void saveRoom(room_t room) 
{
	int x, y;
	for(x=room.x;x<room.x+room.w;x++)
	{
		for(y=room.y;y<room.y+room.h;y++)
		{
			dungeon.map[x][y] = ter_floor;
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
			if(dungeon.map[x][y]!=ter_rock)
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
	int connected[MAX_ROOMS];
	for(i=0;i<MAX_ROOMS;i++)
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

int findClosestRoom(int i, int connected[30], int isConnected)
{
	int x;
	int closest = -1;
	int minDist = INT_MAX; //maximum value for uint. I should just include limits.h
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
		int dist = ((dx*dx)+(dy*dy)); //technically sqr root, but that doesn't really matter
		if(dist<minDist)
		{
			closest = x;
			minDist = dist;
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
				dungeon.map[++x][y] = ter_floor;
			}
		}
		else if(x>room2.x+(room2.w/2))
		{
			for(i=0;i<d;i++)
			{
				dungeon.map[--x][y] = ter_floor;
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
						dungeon.map[++x][y] = ter_floor;
					}
					break;
				case 2:
					for(i=0;i<d;i++)
					{
						dungeon.map[--x][y] = ter_floor;
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
				dungeon.map[x][++y] = ter_floor;
			}
		}
		else if(y>room2.y+(room2.h/2))
		{
			for(i=0; i<d;i++)
			{
				dungeon.map[x][--y] = ter_floor;
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
						dungeon.map[x][++y] = ter_floor;
					}
					break;
				case 2:
					for(i=0;i<d;i++)
					{
						dungeon.map[x][--y] = ter_floor;
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
