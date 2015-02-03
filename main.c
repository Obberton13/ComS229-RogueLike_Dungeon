#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"


typedef enum dmode {
	mode_save,
	mode_load,
	mode_normal,
} dmode_t;

dungeon_t dungeon;

void printMap();

int main(int argc, char *argv[])
{
	dmode_t m = mode_normal;
	unsigned int seed = time(NULL);
	//FILE *f;
	if(argc>1)
	{
		if(strcmp(argv[1], "--help")==0)
		{
			fprintf(stderr, "Usage: DungeonGame [-s seed]\n");
			return 0;
		}
		if(strcmp(argv[1], "-s")==0)
		{
			if (argc!=3) 
			{
				fprintf(stderr, "Usage: DungeonGame [-s seed]\n");
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
		if(strcmp(argv[1], "--save")==0)
		{
			m = mode_save;
		}
		if(strcmp(argv[1], "--load")==0)
		{
			m = mode_load;
		}
	}
	srand(seed);
	dungeon.map = (terrain_tile_t**) malloc(sizeof(terrain_tile_t*)*DUNGEON_X);
	int x;
	for(x=0;x<DUNGEON_X;x++)
	{
		dungeon.map[x] = (terrain_tile_t*) malloc(sizeof(terrain_tile_t)*DUNGEON_Y);
	}
	if(m!=mode_load)
	{
		generateDungeon();
	}
	else
	{
		//TODO load the dungeon from ~/.rlg229/dungeon
		printf("Load Mode!\n");
	}
	printMap();
	if(m==mode_save)
	{
		//TODO Save the dungeon to ~/.rlg229/dungeon
		printf("Save Mode!\n");
	}
	for(x=0;x<DUNGEON_X;x++)
	{
		free(dungeon.map[x]);
	}
	free(dungeon.map);
	return 0;
}

void printMap()
{
	int x, y;
	for(y=0;y<DUNGEON_Y;y++)
	{
		for(x=0;x<DUNGEON_X;x++)
		{
			char toPrint;
			switch(dungeon.map[x][y])
			{
				case ter_rock:
					toPrint = '#';
					break;
				case ter_immutable:
					toPrint = '|';
					break;
				case ter_floor:
					toPrint = '.';
					break;
				default:
					printf("\n\nInvalid dungeon tile ID: %d", dungeon.map[x][y]);
					return;
			}
			printf("%c", toPrint);
		}
		printf("\n");
	}
}
