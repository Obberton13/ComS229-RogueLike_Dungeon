#include<stdio.h>

void generateDungeon(int **map)
{
	int x, y;
	for(y=1;y<96;y++)
	{
		for(x=1;x<159;x++)
		{
			map[x][y]=1;
			printf("1");
		}
		printf("\n");
	}
	return;
}
