#include<stdio.h>

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
	return;
}
