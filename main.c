#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "generator.h"

int **map;

void printMap(void);

int main(int argc, char *argv[])
{
	map = (int**) malloc(sizeof(int*)*160);
	int x;
	for(x=0;x<160;x++)
	{
		map[x] = (int*) malloc(sizeof(int)*96);
	}
	generateDungeon(map);
	printMap();
	free(map);
	return 0;
}

void printMap()
{
	int x, y;
	for(y=0;y<96;y++)
	{
		for(x=0;x<160;x++)
		{
			printf("%1d", map[x][y]);
		}
		printf("\n");
	}
}
