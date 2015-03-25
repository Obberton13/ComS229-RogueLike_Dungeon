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

typedef enum pmode {
	pmode_look,
	pmode_control,
	pmode_quit
} pmode_t;

dungeon_t dungeon;

int open_file(FILE **f, char *mode);
int calculate_file_size(void);
void printMap(int scrX, int scrY);
void save_dungeon(FILE *f);
int load_dungeon(FILE *f);
int move_player(int x, int y);
int is_open_space(int x, int y);

int main(int argc, char *argv[])
{
	initscr();
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
	dungeon.monsters_generated = 0;
	dungeon.game_turn = 0;
	generateDungeon();
	if(!open_file(&f, "r")&&!load_dungeon(f))
	{
		perror("Successfully loaded the dungeon");
		fclose(f);
	}
	find_paths();
	char result = 0;
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
						 			endwin();
									open_file(&f, "w");
									save_dungeon(f);
									fclose(f);
									exit(0);
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
						open_file(&f, "w");
						save_dungeon(f);
						fclose(f);
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
	}while(result==0);
	if(result==2)printf("Oh, no, the player died!\n");

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
		endwin();
		exit(1);
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
	fwrite("RLG229", 1, 6, f);//0-5
	unsigned int version = 1;
	fwrite(&version, 4, 1, f);//6-9
	unsigned int filesize = calculate_file_size();
	fwrite(&filesize, 4, 1, f);//10-13
	fwrite(&filesize, 4, 1, f);//14-17
	int x, y;
	for(y=0;y<DUNGEON_Y;y++)//
	{
		for(x=0;x<DUNGEON_X;x++)
		{
			unsigned char values[5];
			switch(dungeon.map[x][y].tile)
			{
				case ter_room:
					values[0] = 1;
					values[1] = 1;
					values[2] = 0;
					values[3] = 0;
					values[4] = 0;
					break;
				case ter_corridor:
					values[0] = 1;
					values[1] = 0;
					values[2] = 1;
					values[3] = 0;
					values[4] = 0;
					break;
				case ter_rock:
					values[0] = 0;
					values[1] = 0;
					values[2] = 0;
					values[3] = dungeon.map[x][y].hardness;
					values[4] = 0;
					break;
				case ter_immutable:
					values[0] = 1;
					values[1] = 0;
					values[2] = 0;
					values[3] = 255;
					values[4] = 0;
					break;
				case ter_stair_up:
					values[0] = 1;
					values[1] = 0;
					values[2] = 0;
					values[3] = 0;
					values[4] = 2;
					break;
				case ter_stair_down:
					values[0] = 1;
					values[1] = 0;
					values[2] = 0;
					values[3] = 0;
					values[4] = 1;
					break;
				case ter_debug:
					values[0] = 1;
					values[1] = 0;
					values[2] = 0;
					values[3] = 0;
					values[4] = 0;
					break;
				default:
					break;
			}
			fwrite(values, 1, 5, f);
		}
	}
	fwrite(&dungeon.rooms.count, 2, 1, f);
	for(x=0;x<dungeon.rooms.count;x++)
	{
		unsigned char values[4];
		values[0] = dungeon.rooms.list[x].x;
		values[1] = dungeon.rooms.list[x].y;
		values[2] = dungeon.rooms.list[x].w;
		values[3] = dungeon.rooms.list[x].h;
		fwrite(values, 1, 4, f);
	}
	unsigned char pcloc[2] = {dungeon.monsters.list[0].x, dungeon.monsters.list[0].y};
	fwrite(pcloc, 1, 2, f);
	unsigned int gtmg[2] = {dungeon.game_turn, dungeon.monsters_generated};
	fwrite(gtmg, 4, 2, f);
	unsigned short count = dungeon.monsters.count-1;
	fwrite(&count, 2, 1, f);
	//TODO list of NPCs
	for(x = 1; x < dungeon.monsters.count; x++)
	{
		char values[8];
		values[0] = dungeon.monsters.list[x].displayChar;
		values[1] = dungeon.monsters.list[x].x;
		values[2] = dungeon.monsters.list[x].y;
		values[3] = dungeon.monsters.list[x].speed;
		values[4] = (dungeon.monsters.list[x].flags&MONSTER_SMART)>0;
		values[5] = (dungeon.monsters.list[x].flags&MONSTER_TELEP)>0;
		values[6] = dungeon.monsters.list[x].px;
		values[7] = dungeon.monsters.list[x].py;
		fwrite(values, 1, 8, f);
		unsigned int intvalues[2];
		intvalues[0] = dungeon.monsters.list[x].sequence_num;
		intvalues[1] = dungeon.monsters.list[x].initiative + dungeon.game_turn;
		fwrite(intvalues, 4, 2, f);
		int i[20];
		int j;
		for(j = 0; j < 20; j++)
		{
			i[j] = 0;
		}
		fwrite(i, 1, 20, f);
	}
}

int load_dungeon(FILE *f)
{
	if(f==NULL) 
	{
		perror("load dungeon got passed a null pointer");
		return 1;
	}
	char *FileType = malloc(7);
	fread(FileType, 1, 6, f);//0-5
	FileType[6] = 0;
	//if(!strcmp(FileType, "RLG229"))
	//{
	//	perror("Loading dungeon recieved incorrect file type");
	//	return 1;
	//}
	free(FileType);
	unsigned int version;
	fread(&version, 4, 1, f);//6-9
	//if(version != 1)
	//{
	//	perror("Dungeon file is out of date");
	//	return 1;
	//}
	unsigned int filesize;
	int x, y;
	fread(&filesize, 4, 1, f);//10-13
	fread(&filesize, 4, 1, f);//14-17
	dungeon_init();
	int i = 0;
	for(y=0;y<DUNGEON_Y;y++)
	{
		for(x=0;x<DUNGEON_X;x++)
		{
			unsigned char values[5];
			fread(values, 1, 5, f);
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
				switch(values[4])
				{
					case 0:
						break;
					case 1:
						dungeon.map[x][y].tile = ter_stair_down;
						break;
					case 2:
						dungeon.map[x][y].tile = ter_stair_up;
						break;
					default:
						perror("invalid stair value");
						exit(1);
				}
			}
			i++;
			filesize-=5;
		}
	}
	fread(&dungeon.rooms.count, 2, 1, f);
	filesize-=2;
	for(x=0;x<dungeon.rooms.count;x++)
	{
		if(filesize<0)
		{
			fprintf(stderr, "ran out of filesize without running out of dungeons in the list\n");
			return 1;
		}
		unsigned char values[4];
		fread(values, 1, 4, f);
		dungeon.rooms.list[x].x = values[0];
		dungeon.rooms.list[x].y = values[1];
		dungeon.rooms.list[x].w = values[2];
		dungeon.rooms.list[x].h = values[3];
		filesize-=4;
	}
	unsigned char px, py;
	fread(&px, 1, 1, f);
	filesize-=1;
	fread(&py, 1, 1, f);
	filesize-=1;
	fread(&dungeon.game_turn, 4, 1, f);
	filesize-=4;
	fread(&dungeon.monsters_generated, 4, 1, f);
	filesize-=4;
	fread(&dungeon.monsters.count, 2, 1, f);
	filesize-=2;
	dungeon.monsters.list = realloc(dungeon.monsters.list, sizeof(monster_t) * ++dungeon.monsters.count);
	dungeon.monsters.max = dungeon.monsters.count;
	if(!dungeon.monsters.list)
	{
		perror("Malloc failed to allocate storage for monsters on load");
		endwin();
		exit(1);
	}
	for(x=1;x<dungeon.monsters.count; x++)
	{
		if(filesize<0)
		{
			perror("ran out of filesize without running out of monsters in the list\n");
			return 1;
		}
		dungeon.monsters.list[x].flags = 0;
		unsigned char values[8];
		fread(values, 1, 8, f);
		filesize-=8;
		dungeon.monsters.list[x].displayChar = values[0];
		dungeon.monsters.list[x].x = values[1];
		dungeon.monsters.list[x].y = values[2];
		dungeon.map[dungeon.monsters.list[x].x][dungeon.monsters.list[x].y].monsterIndex=x;
		dungeon.monsters.list[x].speed = values[3];
		if(values[4]) dungeon.monsters.list[x].flags+=MONSTER_SMART;
		if(values[5]) dungeon.monsters.list[x].flags+=MONSTER_TELEP;
		dungeon.monsters.list[x].px = (values[6]==255)? dungeon.monsters.list[x].x : values[6];
		dungeon.monsters.list[x].py = (values[7]==255)? dungeon.monsters.list[x].y : values[7];
		unsigned int intvalues[2];
		fread(intvalues, 4, 2, f);
		filesize-=8;
		dungeon.monsters.list[x].sequence_num = intvalues[0];
		dungeon.monsters.list[x].initiative = intvalues[1]-dungeon.game_turn;
		
		unsigned char derp[20];
		fread(derp, sizeof(derp), 1, f);
		filesize-=20;
	}
	if(filesize!=0)
	{
		fprintf(stderr, "%d bytes left over\n", filesize);
		return 1;
	}
	return 0;
}

int calculate_file_size()
{
	int size = 160*96*5;//all of the dungeon tiles
	size += dungeon.rooms.count*4;//values for all of the rooms
	size += 2;//short containing number of rooms
	size += 2;//x and y for the PC
	size += 8;//game turn and monster sequence number
	size += 2;//number of monsters
	size += 36 * dungeon.monsters.count-1;//36 bytes for each monster
	//if there was a user block, it would go here.
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
