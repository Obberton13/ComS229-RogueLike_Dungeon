#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <ncurses.h>

#include "dungeon.h"
#include "pathfinding.h"

#define SCREEN_W 80
#define SCREEN_H 24

typedef enum dmode {
	mode_save,
	mode_load,
	mode_normal,
} dmode_t;

typedef enum pmode {
	pmode_look,
	pmode_control,
	pmode_quit
} pmode_t;

dungeon_t dungeon;

int open_file(FILE **f, char *mode);
long int calculate_file_size(void);
void printMap(int scrX, int scrY);
void save_dungeon(FILE *f);
int load_dungeon(FILE *f);
int move_player(int x, int y);
int is_open_space(int x, int y);


int main(int argc, char *argv[])
{
	initscr();
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
	//Here is the main game loop
	find_paths();
	char result;
	int i;
	pmode_t mode = pmode_control;
	int scrX = 0, scrY = 0;
	scrX = dungeon.monsters.list[0].x - (SCREEN_W/2);
	scrY = dungeon.monsters.list[0].y - (SCREEN_H/2);
	if(scrX < 0) scrX = 0;
	if(scrY < 0) scrY = 0;
	if(scrX > (DUNGEON_X - SCREEN_W)) scrX = DUNGEON_X - SCREEN_W;
	if(scrY > (DUNGEON_Y - SCREEN_H)) scrY = DUNGEON_Y - SCREEN_H;
	printMap(scrX, scrY);
	char in = 0;
	do
	{
		int numDead = 0;
		switch (mode)
		{
			case pmode_control:
				for(i=0;i<dungeon.monsters.count;i++)
				{
					if(dungeon.monsters.list[i].initiative<0) 
					{
						if(!i) result = 2;
						numDead++;
						continue;
					}
					
					dungeon.monsters.list[i].initiative--;//decrease the initiative
					if(!dungeon.monsters.list[i].initiative)//if initiative counter has reached 0
					{
						dungeon.monsters.list[i].initiative = dungeon.monsters.list[i].speed;//reset the initiative
						if(i==0)//if the current monster is the player
						{//move the player and the screen
							find_paths();//recalculate the distances to the player
							in = getch();
							switch(in)
							{
								case '7':
								case 'y'://up and left
									if(move_player(-1, -1)) break;
									scrX--;
									scrY--;
									break;
								case '8':
								case 'k'://up
									if(move_player(0, -1)) break;
									scrY--;

									break;
								case '9':
								case 'u'://up and right
									if(move_player(1, -1))break;
									scrY--;
									scrX++;
									break;
								case '6':
								case 'l'://right 
									if(move_player(1, 0))break;
									scrX++;
									break;
								case '3':
								case 'n'://down and right
									if(move_player(1, 1))break;
									scrX++;
									scrY++;
									break;
								case '2':
								case 'j'://down
									if(move_player(0, 1))break;
									scrY++;
									break;
								case '1':
								case 'b'://down and left
									if(move_player(-1, 1))break;
									scrX--;
									scrY++;
									break;
								case '4':
								case 'h'://left
									if(move_player(-1, 0))break;
									scrX--;
									break;
								case '<'://down stairs
									if(dungeon.map[dungeon.monsters.list[0].x][dungeon.monsters.list[0].y].tile == ter_stair_up)
									{
										int x;
										for(x=0;x<DUNGEON_X;x++)
										{
											free(dungeon.map[x]);
										}
										free(dungeon.map);
										free(dungeon.monsters.list);
										dungeon_init();
										generateDungeon();
									}
									break;
								case '>'://up stairs
									if(dungeon.map[dungeon.monsters.list[0].x][dungeon.monsters.list[0].y].tile == ter_stair_down)
									{
										int x;
										for(x=0;x<DUNGEON_X;x++)
										{
											free(dungeon.map[x]);
										}
										free(dungeon.map);
										free(dungeon.monsters.list);
										dungeon_init();
										generateDungeon();
									}
									break;
								case 'L'://Look mode
									mode = pmode_look;
									scrX = dungeon.monsters.list[0].x - (SCREEN_W/2);
									scrY = dungeon.monsters.list[0].y - (SCREEN_H/2);
									if(scrX < 0) scrX = 0;
									if(scrY < 0) scrY = 0;
									if(scrX > (DUNGEON_X - SCREEN_W)) scrX = DUNGEON_X - SCREEN_W;
									if(scrY > (DUNGEON_Y - SCREEN_H)) scrY = DUNGEON_Y - SCREEN_H;

									break;
								case 'S'://quit
									mode = pmode_quit;
								case ' '://rest for 1 turn.
									break;
								case 27://exit look mode
									//27 is the 'escape' key
									mvprintw(2, 2, "Escape was pressed!");
								default:
									mvprintw(1, 1, "Unbound key: %#o", in);
									break;
							}
							scrX = dungeon.monsters.list[0].x - (SCREEN_W/2);
							scrY = dungeon.monsters.list[0].y - (SCREEN_H/2);
							if(scrX < 0) scrX = 0;
							if(scrY < 0) scrY = 0;
							if(scrX > (DUNGEON_X - SCREEN_W)) scrX = DUNGEON_X - SCREEN_W;
							if(scrY > (DUNGEON_Y - SCREEN_H)) scrY = DUNGEON_Y - SCREEN_H;
							printMap(scrX, scrY);//refresh the screen
						}
						else move_monster(i);//otherwise move the monster
					}
					if(dungeon.monsters.list[0].initiative<0)result = 2;
				}
				break;
			case pmode_look:
				in = getch();
				switch(in)
				{
					case '7':
					case 'y'://up and left
						scrX--;
						scrY--;
						break;
					case '8':
					case 'k'://up
						scrY--;
						break;
					case '9':
					case 'u'://up and right
						scrY--;
						scrX++;
						break;
					case '6':
					case 'l'://right 
						scrX++;
						break;
					case '3':
					case 'n'://down and right
						scrX++;
						scrY++;
						break;
					case '2':
					case 'j'://down
						scrY++;
						break;
					case '1':
					case 'b'://down and left
						scrX--;
						scrY++;
						break;
					case '4':
					case 'h'://left
						scrX--;
						break;
					case 'S'://quit
						endwin();
						exit(1);
					case ' '://rest for 1 turn.
						break;
					case 27://exit look mode
						mode = pmode_control;
					default:
						mvprintw(1, 1, "Unbound key: %#o", in);
				}

				if(scrX < 0) scrX = 0;
				if(scrY < 0) scrY = 0;
				if(scrX > (DUNGEON_X - SCREEN_W)) scrX = DUNGEON_X - SCREEN_W;
				if(scrY > (DUNGEON_Y - SCREEN_H)) scrY = DUNGEON_Y - SCREEN_H;

				printMap(scrX, scrY);
				break;
			default: 
				endwin();
				exit(1);
		}
		if(numDead==dungeon.monsters.count-1)
		{
			result = 1;
		}
	}while(result==0||mode == 1);
	if(result==2)printf("Oh, no, the player died!\n");
	else if(result==1)printf("GG, you killed all of the things");

	if(m==mode_save)
	{
		if(!open_file(&f, "w"))
		{
			save_dungeon(f);
		}
		else
		{
			fprintf(stderr, "Error loading the file\n");
			return 1;
		}
	}
	int x;
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
	endwin();
	return 0;
}

void printMap(int scrX, int scrY)
{
	int x, y;

	for(y=scrY;y<scrY+SCREEN_H;y++)
	{
		for(x=scrX;x<scrX+SCREEN_W;x++)
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
				case ter_debug:
					toPrint = '*';
					break;
				case ter_stair_down:
					toPrint = '>';
					break;
				default:
					mvprintw(0, 0, "Invalid dungeon tile");
					return;
			}
			if(dungeon.map[x][y].monsterIndex!=dungeon.monsters.max)
			{
				if(dungeon.monsters.list[dungeon.map[x][y].monsterIndex].initiative>0)
				toPrint = dungeon.monsters.list[dungeon.map[x][y].monsterIndex].displayChar;
			}
			mvaddch(y-scrY, x-scrX, toPrint);
		}
	}
	refresh();
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

int move_player(int x, int y)
{
	int px = dungeon.monsters.list[0].x;
	int py = dungeon.monsters.list[0].y;
	x+=px;
	y+=py;
	if(!is_open_space(x, y))
	{
		dungeon.monsters.list[0].x = x;
		dungeon.monsters.list[0].y = y;
		dungeon.map[px][py].monsterIndex = dungeon.monsters.max;
		dungeon.map[x][y].monsterIndex = 0;
		return 0;
	}
	return 1;
}

int is_open_space(int x, int y)
{
	if(dungeon.map[x][y].tile==ter_room
		||dungeon.map[x][y].tile==ter_corridor
		||dungeon.map[x][y].tile==ter_stair_up
		||dungeon.map[x][y].tile==ter_stair_down)
	{
		return 0;
	}
	return 1;
}
