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

int open_file(FILE **f, char *mode);
int calculate_file_size(void);
void printMap(void);
void save_dungeon(FILE *f);
int load_dungeon(FILE *f);

int main(int argc, char *argv[])
{
	dmode_t m = mode_normal;
	unsigned int seed = time(NULL);
	FILE *f = NULL;
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
	dungeon.map = (terrain_cell_t**) malloc(sizeof(terrain_cell_t*)*DUNGEON_X);
	int x;
	for(x=0;x<DUNGEON_X;x++)
	{
		dungeon.map[x] = (terrain_cell_t*) malloc(sizeof(terrain_cell_t)*DUNGEON_Y);
	}
	if(m!=mode_load)
	{
		generateDungeon();
	}
	else
	{
		if(!open_file(&f, "r"))
		{
			load_dungeon(f);
		}
		else
		{
			fprintf(stderr, "Error loading the file\n");
			return 1;
		}
	}
	printMap();	
	if(m==mode_save)
	{
		//TODO Save the dungeon to ~/.rlg229/dungeon
		if(!open_file(&f, "w"))
		{
			//TODO save the dungeon to this file
			save_dungeon(f);
		}
		else
		{
			fprintf(stderr, "Error loading the file\n");
			return 1;
		}
	}
	for(x=0;x<DUNGEON_X;x++)
	{
		free(dungeon.map[x]);
	}
	free(dungeon.map);
	if(f)
	{
		fclose(f);
	}
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
			switch(dungeon.map[x][y].tile)
			{
				case ter_rock:
					toPrint = '#';
					break;
				case ter_immutable:
					toPrint = '|';
					break;
				case ter_room:
					toPrint = '.';
					break;
				case ter_corridor:
					toPrint = ',';
					break;
				default:
					printf("\n\nInvalid dungeon tile ID: %d", dungeon.map[x][y].tile);
					return;
			}
			printf("%c", toPrint);
		}
		printf("\n");
	}
}

int open_file(FILE **f, char *mode)
{
	char *path;
	char *home;
	char *file = ".rlg229/dungeon";
	home = getenv("HOME");
	path = malloc(strlen(home) + strlen(file) + 2);
	if(!path)
	{
		fprintf(stderr, "malloc failed!\n");
		return 1;
	}
	sprintf(path, "%s/%s", home, file);
	*f = fopen(path, mode);
	if(!*f)
	{
		perror(path);
		free(path);
		return 1;
	}
	free(path);
	return 0;
}

void save_dungeon(FILE *f)
{
	//fwrite(toPrint, size, number, file)
	fwrite("RLG229", sizeof("RLG229"), 1, f);
	int version = 0;
	fwrite(&version, sizeof(version), 1, f);
	int filesize = calculate_file_size();
	fwrite(&filesize, sizeof(version), 1, f);
	int x, y;
	for(y=0;y<DUNGEON_Y;y++)
	{
		for(x=0;x<DUNGEON_X;x++)
		{
			unsigned char values[4];
			values[0] = (dungeon.map[x][y].tile==ter_room||dungeon.map[x][y].tile==ter_corridor);
			values[1] = (dungeon.map[x][y].tile==ter_room);
			values[2] = (dungeon.map[x][y].tile==ter_corridor);
			values[3] = dungeon.map[x][y].hardness;
			fwrite(values, sizeof(values), 1, f);
		}
	}
	fwrite(&dungeon.list.count, sizeof(dungeon.list.count), 1, f);
	for(x=0;x<dungeon.list.count;x++)
	{
		unsigned char values[4];
		values[0] = dungeon.list.list[x].x;
		values[1] = dungeon.list.list[x].y;
		values[2] = dungeon.list.list[x].w;
		values[3] = dungeon.list.list[x].h;
		fwrite(values, sizeof(values), 1, f);
	}
}

int load_dungeon(FILE *f)
{
	char *FileType = malloc(sizeof("RLG229"));
	fread(FileType, sizeof("RLG229"), 1, f);
	printf("%s\n" FileType);
	free(FileType);
	int version;
	fread(&version, sizeof(version), 1, f);
	int filesize;
	fread(&filesize, sizeof(filesize), 1, f);
	int x, y;
	for(y=0;y<DUNGEON_Y;y++)
	{
		for(x=0;x<DUNGEON_X;x++)
		{
			unsigned char values[4];
			fread(values, sizeof(values), 1, f);
			if(values[1])
			{
				dungeon.map[x][y].tile = ter_room;
			}
			if(values[2])
			{
				dungeon.map[x][y].tile = ter_corridor;
			}
			dungeon.map[x][y].hardness = values[3];
		}
	}
	fread(&dungeon.list.count, sizeof(dungeon.list.count), 1, f);
	for(x=0;x<dungeon.list.count;x++)
	{
		unsigned char values[4];
		fread(values, sizeof(values), 1, f);
		dungeon.list.list[x].x = values[0];
		dungeon.list.list[x].y = values[1];
		dungeon.list.list[x].w = values[2];
		dungeon.list.list[x].h = values[3];
	}
	return 0;
}

int calculate_file_size()
{
	int size = 160*96*4;//all of the dungeon tiles
	size += dungeon.list.count*4;//values for all of the rooms
	size += 2;//short containing number of rooms
	return size;
}
