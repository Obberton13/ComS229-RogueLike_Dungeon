#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "generator.h"

void printMap(DungeonTile **map);

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
	DungeonTile** map = (DungeonTile**) malloc(sizeof(DungeonTile*)*160);
	int x;
	for(x=0;x<160;x++)
	{
		map[x] = (DungeonTile*) malloc(sizeof(DungeonTile)*96);
	}
	generateDungeon(map);
	printMap(map);
	for(x=0;x<160;x++)
	{
		free(map[x]);
	}
	free(map);
	return 0;
}

void printMap(DungeonTile **map)
{
	int x, y;
	for(y=0;y<96;y++)
	{
		for(x=0;x<160;x++)
		{
			char toPrint;
			switch(map[x][y])
			{
				case ROCK: //Rock
					toPrint = '#';
					break;
				case EDGE: //Immutable wall
					toPrint = '|';
					break;
				case FLOOR: //floor
					toPrint = '.';
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
