#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#include "dungeon.h"
#include "pathfinding.h"


typedef enum dmode {
	mode_save,
	mode_load,
	mode_normal,
} dmode_t;

dungeon_t dungeon;

int open_file(FILE **f, char *mode);
long int calculate_file_size(void);
void printMap(void);
void save_dungeon(FILE *f);
int load_dungeon(FILE *f);

int main(int argc, char *argv[])
{
	dmode_t m = mode_normal;
	unsigned int seed = time(NULL);
	FILE *f = NULL;
	dungeon.monsters.max = 0;
	if(argc>1)
	{
		if(strcmp(argv[1], "--help")==0)
		{
			fprintf(stderr, USAGE);
			return 0;
		}
		else if(strcmp(argv[1], "-s")==0)
		{
			if (argc!=3) 
			{
				fprintf(stderr, USAGE);
				return 1;
			}
			int length = strlen(argv[2]), i;
			seed = 0;
			for(i=0;i<length;i++)
			{
				seed = (seed*10)+(argv[2][i] - '0');
			}
			printf("Seed: %d\n", seed);
		}
		else if(strcmp(argv[1], "--save")==0)
		{
			m = mode_save;
		}
		else if(strcmp(argv[1], "--load")==0)
		{
			m = mode_load;
		}
		else if(strcmp(argv[1], "--nummon")==0)
		{
			if(argc!=3)
			{
				fprintf(stderr, USAGE);
				return 1;
			}
			int length = strlen(argv[2]), i;
			for(i=0;i<length;i++)
			{
				dungeon.monsters.max = (10*dungeon.monsters.max) + (argv[2][i] - '0');
			}
			printf("Selected NumMon as an agrument");
		}
	}
	if(dungeon.monsters.max == 0)
	{
		dungeon.monsters.max = 30;
	}
	dungeon.monsters.max++;//I need 1 spot for the player.
	printf("%d\n", dungeon.monsters.max);
	int size = dungeon.monsters.max * sizeof(monster_t);
	dungeon.monsters.list = malloc(size);
	srand(seed);
	dungeon_init();
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
	int x = 0;
	find_paths();
	do
	{
		int i;
		for(i=0;i<dungeon.monsters.count&&!x;i++)
		{
			dungeon.monsters.list[i].initiative--;//decreas the initiative
			if(!dungeon.monsters.list[i].initiative)//if initiative counter has reached 0
			{
				move_monster(i, &x);//move the monster
				dungeon.monsters.list[i].initiative = dungeon.monsters.list[i].speed;//reset the initiative
				if(i==0)//if the current monster is the player
				{
					find_paths();//recalculate the distances to the player
					printMap();//refresh the screen
					sleep(1);//and wait to do it again next time.
				}
			}
		}
	}while(!x);
	if(x==1)
	{
		printf("Oh, no, the player died!\n");
	}

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
	free(dungeon.monsters.list);
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
			/*int dist = dungeon.map[x][y].distToPlayer;
			if(dist>224)
			{
				printf("ff");
			}
			else
			{
				printf("%2x", dist);
			}//*/
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
				case ter_debug:
					toPrint = '*';
					break;
				case ter_debug2:
					toPrint = 'd';
					break;
				case ter_debug3:
					toPrint = '+';
					break;
				default:
					printf("\n\nInvalid dungeon tile ID: %d", dungeon.map[x][y].tile);
					return;
			}
			if(dungeon.map[x][y].monsterIndex!=dungeon.monsters.max)
			{
				toPrint = dungeon.monsters.list[dungeon.map[x][y].monsterIndex].displayChar;
			}
			printf("%c", toPrint);//*/
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
	fwrite("RLG229", 1, 6, f);
	long int version = 0;
	fwrite(&version, sizeof(version), 1, f);
	long int filesize = calculate_file_size();
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
	fwrite(&dungeon.rooms.count, sizeof(dungeon.rooms.count), 1, f);
	for(x=0;x<dungeon.rooms.count;x++)
	{
		unsigned char values[4];
		values[0] = dungeon.rooms.list[x].x;
		values[1] = dungeon.rooms.list[x].y;
		values[2] = dungeon.rooms.list[x].w;
		values[3] = dungeon.rooms.list[x].h;
		fwrite(values, sizeof(values), 1, f);
	}
}

int load_dungeon(FILE *f)
{
	char *FileType = malloc(7);
	fread(FileType, 1, 6, f);
	free(FileType);
	long int version;
	fread(&version, sizeof(version), 1, f);
	long int filesize;
	fread(&filesize, sizeof(filesize), 1, f);
	int x, y;
	for(y=0;y<DUNGEON_Y;y++)
	{
		for(x=0;x<DUNGEON_X;x++)
		{
			unsigned char values[4];
			fread(values, sizeof(values), 1, f);
			if(dungeon.map[x][y].tile!=ter_immutable)
			{
				if(values[1])
				{
					dungeon.map[x][y].tile = ter_room;
				}
				else if(values[2])
				{
					dungeon.map[x][y].tile = ter_corridor;
				}
				dungeon.map[x][y].hardness = values[3];
			}
		}
	}
	fread(&dungeon.rooms.count, sizeof(dungeon.rooms.count), 1, f);
	filesize-=2;
	for(x=0;x<dungeon.rooms.count;x++)
	{
		if(filesize<0)
		{
			fprintf(stderr, "ran out of filesize without running out of dungeons in the list\n");
			return 1;
		}
		unsigned char values[4];
		fread(values, sizeof(values), 1, f);
		dungeon.rooms.list[x].x = values[0];
		dungeon.rooms.list[x].y = values[1];
		dungeon.rooms.list[x].w = values[2];
		dungeon.rooms.list[x].h = values[3];
		filesize-=4;
	}
	return 0;
}

long int calculate_file_size()
{
	long int size = 160*96*4;//all of the dungeon tiles
	size += dungeon.rooms.count*4;//values for all of the rooms
	size += 2;//short containing number of rooms
	return size;
}
