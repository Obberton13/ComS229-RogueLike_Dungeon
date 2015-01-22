#include<stdlib.h>
#include<stdio.h>//TODO TEMPORARY

int generateRoom(int **map);
int saveRoom(int roomX, int roomY, int roomW, int roomH, int **map);
void generateDungeon(int **map)
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
	generateRoom(map);
	return;
}

int generateRoom(int **map)
{
	//TODO I should not have printf's here
	int roomX = random()%150+1;
	int roomY = random()%85+1;
	int roomW = random()%15 + 8;//8-22
	int roomH = random()%12 + 5;//5-16
	printf("Generating room at %d, %d with size %d, %d\n", roomX, roomY, roomW, roomH);
	return saveRoom(roomX, roomY, roomW, roomH, map);
}

int saveRoom(int roomX, int roomY, int roomW, int roomH, int **map)
{
	if(((roomX+roomW)>159)||((roomY+roomH)>95))
	{
		return 1;
	}
	int x, y;
	for(x=roomX;x<roomX+roomW;x++)
	{
		for(y=roomY;y<roomY+roomH;y++)
		{
			map[x][y] = 2;
		}
	}
	return 0;
}
