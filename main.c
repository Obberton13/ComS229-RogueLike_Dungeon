#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"

<<<<<<< HEAD
#define DUNGEON_X = 160
#define DUNGEON_Y = 96
#define MIN_ROOMS = 12
#define MAX_ROOMS = 30
#define ROOM_MIN_W = 8
#define ROOM_MIN_H = 5
#define ROOM_SEPARATION = 3
#define MAX_PLACEMENT_ATTEMPTS = 2000

typedef enum mode {
	MODE_SAVE,
	MODE_LOAD,
	MODE_NORMAL,
} mode_t;



dungeon_t dungeon;

void printMap();

int main(int argc, char *argv[])
{
	mode_t m = MODE_NORMAL;
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
		if(strcmp(argv[1], "--save")==0)
		{
			m = SAVE;
		}
		if(strcmp(argv[1], "--load")==0)
		{
			m = LOAD;
		}
	}
	srand(seed);
	dungeon.map = (char**) malloc(sizeof(char*)*160);
	int x;
	for(x=0;x<160;x++)
	{
		dungeon.map[x] = (char*) malloc(sizeof(char)*96);
	}
	if(m!=LOAD)
	{
		generateDungeon();
	}
	else
	{
		//TODO load the dungeon from ~/.rlg229/dungeon
		printf("Load Mode!\n");
	}
	printMap();
	if(m==SAVE)
	{
		//TODO Save the dungeon
		printf("Save Mode!\n");
	}
	for(x=0;x<160;x++)
	{
		free(dungeon.map[x]);
	}
	free(dungeon.map);
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
			switch(dungeon.map[x][y])
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
				default:
					printf("\n\nInvalid dungeon tile ID: %d", dungeon.map[x][y]);
					return;
			}
			printf("%c", toPrint);
		}
		printf("\n");
	}
}
