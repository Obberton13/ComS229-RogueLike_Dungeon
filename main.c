#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "generator.h"

int **map;

void printMap(void);

int main(int argc, char *argv[])
{
	unsigned int seed = time(NULL);
	if(argc>1)
	{
		if(strcmp(argv[1], "--help")==0)
		{
			printf("Usage: DungeonGame [-s seed]\n");
			return 0;
		}
		if(strcmp(argv[1], "-s")==0)
		{
			if (argc!=3) 
			{
				printf("Usage: DungeonGame [-s seed]\n");
				return 0;
			}
			int length = strlen(argv[2]), i;
			seed = 0;
			for(i=0;i<length;i++)
			{
				seed = (seed*10)+(argv[2][i] - '0');
			}
			printf("Seed: %d\n", seed);
		}
	}
	srand(seed);
	map = (int**) malloc(sizeof(int*)*160);
	int x;
	for(x=0;x<160;x++)
	{
		map[x] = (int*) malloc(sizeof(int)*96);
	}
	generateDungeon(map);
	printMap();
	for(x=0;x<160;x++)
	{
		free(map[x]);
	}
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
			char toPrint;
			switch(map[x][y])
			{
				case 0: //Rock
					toPrint = '#';
					break;
				case 1: //Immutable wall
					toPrint = '|';
					break;
				case 2: //floor
					toPrint = '.';
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				default:
					printf("\n\nInvalid dungeon tile ID: %d", map[x][y]);
					return;
			}
			printf("%c", toPrint);
		}
		printf("\n");
	}
}
