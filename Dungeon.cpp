#include "Dungeon.h"
#include "NPC.h"
#include "Item.h"
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <climits>
#include <unistd.h>

static int gameLoop();

int main(int argc, char *argv[])
{
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
	if (NPCdef::parseDefs())
	{
		return 1;
	}
	if (ItemDef::parseObjects())
	{
		return 1;
	}
	srand(time(0));
	while (!gameLoop());//do nothing but the inner game loop.
	NPCdef::deleteDefs();
	ItemDef::deleteDefs();
	endwin();
	return 0;
}

int gameLoop()
{
	Dungeon *d = new Dungeon;
	while (!d->game_loop())
		;//do nothing except the game loop
	delete d;
	return 1;
}

Dungeon::Dungeon()
{
	scrX = 0;
	scrY = 0;
	map = new DungeonTile*[DUNGEON_X]();
	for (int i = 0; i < DUNGEON_X; i++)
	{
		map[i] = new DungeonTile[DUNGEON_Y]();
	}//*/
	generate();
}

Dungeon::~Dungeon()
{
	for (int i = 0; i < DUNGEON_X; i++)
	{
		delete[] map[i];
	}
	delete[] map;
	unsigned int i = 0;
	for(i = 0; i< npcs.size(); i++)
	{
		delete npcs[i];
	}
	for(i = 0; i<items.size(); i++)
	{
		delete items[i];
	}
}

void Dungeon::generate()
{
	initialize();
	generate_rooms();
	connect_all_rooms();
	spawn_player();
	find_monster_distances();
	spawn_monsters();
	spawn_items();
}

void Dungeon::initialize()
{
	current_state = GAME_CONTROL;
	int x, y;
	for (x = 1; x < DUNGEON_X-1; x++)
	{
		for (y = 1; y < DUNGEON_Y-1; y++)
		{
			map[x][y].set_type(ter_wall);
			map[x][y].set_hardness((rand() % (UCHAR_MAX-2))+1);
			map[x][y].setX(x);
			map[x][y].setY(y);
		}
	}
	for (x = 0; x < DUNGEON_X; x++)
	{
		map[x][0].set_type(ter_immutable);
		map[x][0].set_hardness(255);
		map[x][DUNGEON_Y - 1].set_type(ter_immutable);
		map[x][DUNGEON_Y - 1].set_hardness(255);
		map[x][0].setX(x);
		map[x][0].setY(0);
		map[x][DUNGEON_Y-1].setX(x);
		map[x][DUNGEON_Y-1].setY(DUNGEON_Y-1);
	}
	for (y = 1; y < DUNGEON_Y-1; y++)
	{
		map[0][y].set_type(ter_immutable);
		map[0][y].set_hardness(255);
		map[DUNGEON_X-1][y].set_type(ter_immutable);
		map[DUNGEON_X-1][y].set_hardness(255);
		map[0][y].setX(0);
		map[0][y].setY(y);
		map[DUNGEON_X-1][y].setX(DUNGEON_X-1);
		map[DUNGEON_X-1][y].setY(y);
	}
}

void Dungeon::generate_rooms()
{
	int attempts  = 0;
	while ((attempts < MAX_ROOM_ATTEMPTS
		|| rooms.size() < MIN_ROOMS)
		&& rooms.size() < MAX_ROOMS)
	{
		if (!generate_room())
		{
			attempts++;
		}
	}
}

int Dungeon::generate_room()
{
	unsigned char x, y, w, h;
	h = rand() % 8 + ROOM_MIN_H;
	w = rand() % 10 + ROOM_MIN_W;
	x = (rand() % (DUNGEON_X - w - (ROOM_SEPARATION * 2) - 2)) + ROOM_SEPARATION + 1;
	y = (rand() % (DUNGEON_Y - h - (ROOM_SEPARATION * 2) - 2)) + ROOM_SEPARATION + 1;
	unsigned char i, j;
	for (i = x - ROOM_SEPARATION; i < x + ROOM_SEPARATION + w; i++)
	{
		for (j = y - ROOM_SEPARATION; j < y + ROOM_SEPARATION + h; j++)
		{
			if (map[i][j].get_type() != ter_wall)
			{
				return 1;
			}
		}
	}
	struct room room;
	room.h = h;
	room.w = w;
	room.x = x;
	room.y = y;
	rooms.push_back(room);
	for (i = x; i < x + w; i++)
	{
		for (j = y; j < y + h; j++)
		{
			map[i][j].set_type(ter_room);
			map[i][j].set_hardness(0);
		}
	}
	return 0;
}

void Dungeon::connect_all_rooms()
{
	//TODO fix this function to actually connect the rooms.
	bool connected[MAX_ROOMS];
	unsigned char i;
	for (i = 0; i < rooms.size(); i++)
	{
		connected[i] = false;
	}
	connected[0] = 1;
	for (i = 1; i < rooms.size(); i++)
	{
		unsigned char rm1 = get_closest_room(0, connected);
		unsigned char rm2 = get_closest_room(rm1, connected);
		connect_rooms(rm1, rm2);
		connected[rm1] = true;
	}
}

unsigned char Dungeon::get_closest_room(unsigned char roomIndex, bool connArray[])
{
	unsigned char i;
	unsigned int minDist = UINT_MAX;
	unsigned char minRoom = 0;
	for (i = 1; i < rooms.size(); i++)
	{
		if (connArray[i] == connArray[roomIndex])
		{
			continue;
		}
		unsigned char rm1X = rooms[i].x + (rooms[i].w / 2);
		unsigned char rm1Y = rooms[i].y + (rooms[i].h / 2);
		unsigned char rm2X = rooms[roomIndex].x + (rooms[roomIndex].w / 2);
		unsigned char rm2Y = rooms[roomIndex].y + (rooms[roomIndex].h / 2);
		unsigned dist = (rm1X - rm2X)*(rm1X - rm2X) + (rm1Y-rm2Y)*(rm1Y-rm2Y);
		if (dist < minDist)
		{
			minDist = dist;
			minRoom = i;
		}
	}
	return minRoom;
}

void Dungeon::connect_rooms(unsigned char rm1, unsigned char rm2)
{
	unsigned char rm1X = rooms[rm1].x + rooms[rm1].w / 2;
	unsigned char rm1Y = rooms[rm1].y + rooms[rm1].h / 2;
	unsigned char rm2X = rooms[rm2].x + rooms[rm2].w / 2;
	unsigned char rm2Y = rooms[rm2].y + rooms[rm2].h / 2;
	unsigned char x, y;
	y = rm1Y;
	for (x = rm1X; x != rm2X;)
	{
		map[x][y].set_type(ter_corridor);
		map[x][y].set_hardness(0);
		x += (x < rm2X) ? 1 : -1;
	}
	for (y = rm1Y; y != rm2Y;)
	{
		map[x][y].set_type(ter_corridor);
		map[x][y].set_hardness(0);
		y += (y < rm2Y) ? 1 : -1;
	}
}

void Dungeon::spawn_player()
{
	NPCdef* pc = new NPCdef();
	pc->setAbilities("TUNNEL TELEPATH");
	pc->setColor("WHITE");
	pc->setDamage(Dice(0, 2, 6));
	pc->setDescription("The player character. It's YOU!");
	pc->setName("Intarmin Hanurnatural");
	pc->setHP(Dice(15, 2, 8));
	pc->setSpeed(Dice(10, 0, 0));
	pc->setSymbol('@');
	int rm = 0;
	unsigned char x = (rand() % rooms[rm].w) + rooms[rm].x;
	unsigned char y = (rand() % rooms[rm].h) + rooms[rm].y;
	NPC* tmp = new NPC(x, y, pc);
	map[x][y].set_monster(tmp);
	npcs.push_back(tmp);
	delete pc;
}

void Dungeon::spawn_monsters()
{
	unsigned char x, y, i;
	for (i = 0; i < MAX_MONSTERS; i++)
	{
		int rm = (std::rand() % (rooms.size() - 1)) + 1;
		x = (rand() % rooms[rm].w) + rooms[rm].x;
		y = (rand() % rooms[rm].h) + rooms[rm].y;
		NPC* tmp = new NPC(x, y);
		NPC* existing = map[x][y].get_monster();
		if(existing)
		{
			push_monster(existing, true);
		}
		map[x][y].set_monster(tmp);
		npcs.push_back(tmp);
	}
}

void Dungeon::spawn_items()
{
	unsigned char x, y, i;
	for (i = 0; i < MAX_ITEMS; i++)
	{
		int rm = std::rand() % rooms.size();
		x = (rand() % rooms[rm].w) + rooms[rm].x;
		y = (rand() % rooms[rm].h) + rooms[rm].y;
		Item* tmp = new Item(x, y);
		map[x][y].setItem(tmp);
		items.push_back(tmp);
	}
}

void Dungeon::find_monster_distances()
{
	unsigned char x, y;
	for (x = 0; x < DUNGEON_X; x++)
	{
		for (y = 0; y < DUNGEON_Y; y++)
		{
			map[x][y].set_dist_to_player(USHRT_MAX);
		}
	}
	std::vector<DungeonTile*> *q = new std::vector<DungeonTile*>();
	x = npcs[0]->getX();
	y = npcs[0]->getY();
	map[x][y].set_dist_to_player(0);
	q->push_back(&map[x][y]);
	char offsetX[8] = {0, 0, -1, -1, -1, 1, 1, 1};
	char offsetY[8] = {-1, 1, 0, 1, -1, 0, 1, -1};
	while (!q->empty())
	{
		DungeonTile* t = q->back();
		q->pop_back();
		unsigned char i;
		unsigned short dist = t->get_dist_to_player();
		x = t->getX();
		y = t->getY();
		for (i = 0; i < 8; i++)
		{
			unsigned char newX = x + offsetX[i], newY = y + offsetY[i];
			if (map[newX][newY].get_dist_to_player() == USHRT_MAX 
				&& map[newX][newY].get_hardness()==0)
			{
				q->push_back(&map[newX][newY]);
			}
			if (dist + 1 < map[newX][newY].get_dist_to_player())
			{
				map[newX][newY].set_dist_to_player(dist + 1);
			}
		}
		std::make_heap(q->begin(), q->end());
	}
	delete q;
}

int Dungeon::game_loop()
{
	unsigned int i;
	print();//TODO debugging switch statement.
	for (i = 0; i < npcs.size(); i++)
	{
		if(!npcs[i]->isAlive())//If the monster is DEAD, don't worry about it.
		{
			if(i==0)//If it is actually the player that is dead, end the game.
			{
				current_state = GAME_DEATH;
				return 2;
			}
			continue;
		}
		if(!npcs[i]->is_next_turn())
		{
			npcs[i]->use_initiative();
			continue;
		}
		if(i==0)//If it is the player's turn...
		{
			while(player_input())//Do everything the player wants.
					//In a while loop because some things (like Look Mode) should
					//not use a game turn.
				;
		}
		else
		{
			//Move the monster, or have the monster attack.
			if(push_monster(npcs[i], true)==2)//2 indicates player death. Anything else we don't really care.
			{
				current_state = GAME_DEATH;
				return 2;
			}
		}
	}
	if(current_state == GAME_DEATH) return 1;
	return 0;
}

void Dungeon::move_monster(NPC* npc, unsigned char x, unsigned char y)
{
	DungeonTile t = map[x][y];
	map[npc->getX()][npc->getY()].set_monster(NULL);
	map[x][y].set_monster(npc);
	npc->setPos(x, y);
}

void Dungeon::print()
{
	if(scrX < 0) scrX = 0;
	if(scrY < 0) scrX = 0;
	if(scrX+SCREEN_W >= DUNGEON_X) scrX = DUNGEON_X - SCREEN_W;
	if(scrY+SCREEN_H >= DUNGEON_Y) scrY = DUNGEON_Y - SCREEN_H;
	unsigned char x, y;
	for(x = 0; x < SCREEN_W; x++)
	{
		for(y = 0; y < SCREEN_H; y++)
		{
			putchar(x+scrX, y+scrY, x, y);
		}
	}//*/
	//don't mind this, just debugging stuff.
	/*unsigned char x, y;
	for(x = 0; x < DUNGEON_X; x++)
	{
		for(y = 0; y < DUNGEON_Y; y++)
		{
			putchar(x, y, x, y);
		}
	}//*/
	refresh();//make sure to actually update the game screen.
}

void Dungeon::putchar(unsigned char x, unsigned char y, unsigned char posX, unsigned char posY)
{
	NPC* npc = map[x][y].get_monster();
	if(npc)
	{
		attron(COLOR_PAIR(npc->getColor()));
		mvaddch(posY, posX, npc->getSymbol());
		attroff(COLOR_PAIR(npc->getColor()));
		return;
	}
	Item* item = map[x][y].getItem();
	if(item)
	{
		attron(COLOR_PAIR(item->getColor()));
		mvaddch(posY, posX, item->getSymbol());
		attroff(COLOR_PAIR(item->getColor()));
		return;
	}
	switch(map[x][y].get_type())
	{
	case ter_corridor:
		mvaddch(posY, posX, ',');
		return;
	case ter_room:
		mvaddch(posY, posX, '.');
		return;
	case ter_immutable:
		mvaddch(posY, posX, '|');
		return;
	case ter_wall:
		mvaddch(posY, posX, '#');
		return;
	default: 
		std::cerr << "Invalid dungeon Tile" << std::endl;
		attron(COLOR_PAIR(COLOR_RED));
		mvaddch(posY, posX, '*');
		attroff(COLOR_PAIR(COLOR_RED));
	}
	return;
}

//returns true if the monster was moved or there was no monster to move.
int Dungeon::push_monster(NPC* npc, bool isFirst)
{
	if(npc==NULL)
	{
		return 0;
	}
	if(isFirst) npc->reset_initiative();
	signed char offsetX[8] = {0,0,1,1,1,-1,-1,-1};
	signed char offsetY[8] = {1,-1,1,0,-1,1,0,-1};
	unsigned char x = npc->getX();
	unsigned char y = npc->getY();
	unsigned char newX=0, newY=0, i, testX, testY;
	unsigned short minDist = USHRT_MAX, dist;
	unsigned int abilities = npc->getAbilities();
	if(abilities & (NPC_PASS|NPC_TUNNEL))
	{
		unsigned char px = npcs[0]->getX();
		unsigned char py = npcs[0]->getY();
		newX = (px==x) ? x : (px > x) ? x + 1 : x - 1;
		newY = (py==y) ? y : (py > y) ? y + 1 : y - 1;
		//still need to check for existing monsters.
		NPC* existing = map[newX][newY].get_monster();
		if(existing == npcs[0])//if the thing that it is trying to move to is the PC
		{
			//attack the PC
			npc->attack(npcs[0]);
			return 1;
		}
		move_monster(npc, newX, newY);
		if(push_monster(existing, false))
		{
			move_monster(npc, x, y);
			move_monster(existing, newX, newY);
			return 1;
		}
		if(abilities & NPC_TUNNEL)
		{
			map[x][y].set_hardness(0);
			map[x][y].set_type(ter_room);
		}
		return 0;
	}
	for(i=0; i<8; i++)
	{
		testX = x + offsetX[i];
		testY = y + offsetY[i];
		{
			if(map[testX][testY].get_hardness()) continue;
		}
		dist = map[testX][testY].get_dist_to_player();
		if(dist<minDist)
		{
			minDist = dist;
			newX = testX;
			newY = testY;
		}
	}
	if(minDist == 0)
	{
		if(!isFirst)
		{
			//TODO attack the player.
			npc->attack(npcs[0]);
			//TODO check if the player is dead, and end the game if they are.
			if(npcs[0]->isAlive())
			{
				return 2;
			}
			return 1;
		}
		else
		{
			//this shouldn't really happen. If a monster is pushed onto the player, the loop above didn't work as it was supposed to. We'll just keep the monster here.
			return 1;
		}
	}
	if(!newX || !newY)//If there was NO OPEN SPOTS FOUND AT ALL, keep the monster here.
	{
		return 1;
	}
	if ((abilities&NPC_TUNNEL)&&(map[newX][newY].get_hardness()!=0))//if the monster can tunnel
	{
		//set the place the monster went to to traversable terrain.
		map[x][y].set_type(ter_room);
		map[x][y].set_hardness(0);
	}
	NPC* existing = map[newX][newY].get_monster();//I do this to prevent accidental infinite recursion.
	move_monster(npc, newX, newY);//If we move the monster, there is no way the monster that was here before
	if(push_monster(existing, false))//move the monster that we just moved on top of.
	{
		//if the monster that we just tried to move over can't move, move the monster back.
		move_monster(npc, x, y);
		//make sure the map doesn't think the new spot doesn't have a monster in it for some reason.
		move_monster(existing, newX, newY);
		return 1;
	}
	return 0;
}

int Dungeon::player_input()
{
	switch(current_state)
	{
		case GAME_LOOK:
			return look_action();
		case GAME_CONTROL:
			return control_action();
		case GAME_INVENTORY:
			return inventory_action();
		case GAME_DEATH:
			return death_action();
		case GAME_QUIT:
			return quit_action();
		default:
			std::string str = "Invalid game state. Exiting.";
			unsigned char x = SCREEN_W/2 - str.length()/2;
			unsigned char y = SCREEN_H/2;
			mvprintw(y, x, str.c_str());
			exit(1);
	}
}

int Dungeon::look_action()
{
	char c = getch();
	switch(c)
	{
		case 27://27 is escape.
			return 0;
	}
	print();
	return 1;
}

int Dungeon::control_action()
{
	char c = getch();
	switch(c)
	{
		case '7':
		case 'y'://up and left
			if(move_player(-1, -1)) return 1;
			return 0;
		case '8':
		case 'k'://up
			if(move_player(0, -1)) return 1;
			return 0;
		case '9':
		case 'u'://up and right
			if(move_player(1, -1))return 1;
			return 0;
		case '6':
		case 'l'://right 
			if(move_player(1, 0))return 1;
			return 0;
		case '3':
		case 'n'://down and right
			if(move_player(1, 1))return 1;
			return 0;
		case '2':
		case 'j'://down
			if(move_player(0, 1))return 1;
			return 0;
		case '1':
		case 'b'://down and left
			if(move_player(-1, 1))return 1;
			return 0;
		case '4':
		case 'h'://left
			if(move_player(-1, 0))return 1;
			return 0;
		case 'L'://Look mode
			current_state = GAME_LOOK;
			return 2;
		case 'S'://quit
			current_state = GAME_QUIT;
			quit();
		case ' '://rest for 1 turn.
			return 0;
		case 27://exit look mode
			current_state = GAME_CONTROL;
			return 1;
		default:
			return 1;
	}
}

int Dungeon::move_player(char x, char y)
{
	unsigned char newX = npcs[0]->getX()+x;
	unsigned char newY = npcs[0]->getY()+y;
	if(map[newX][newY].get_type()==ter_immutable)
	{
		return 1;
	}
	NPC* other = map[newX][newY].get_monster();
	if(other)
	{
		//TODO everything involving npcs[0] should be turned into the PC.
		npcs[0]->attack(other);
		return 0;
	}
	Item* item;
	if(item)
	{
		//TODO make the PC pick up the item.
	}
	unsigned char oldX = npcs[0]->getX();
	unsigned char oldY = npcs[0]->getY();
	npcs[0]->setPos(newX, newY);
	map[oldX][oldY].set_monster(NULL);
	map[newX][newY].set_monster(npcs[0]);
	scrX = newX-SCREEN_W/2;
	scrY = newY-SCREEN_H/2;
	map[newX][newY].set_hardness(0);
	map[newX][newY].set_type(ter_room);
	find_monster_distances();
	return 0;
}

int Dungeon::inventory_action()
{
	char c = getch();
	switch(c)
	{
		case 'S':
			quit();
		default:
			return 1;
	}
}

int Dungeon::death_action()
{
	std::string s = "YOU DIED! Would you like to play again? (y/n)";
	unsigned char x = SCREEN_W/2 - s.length()/2;
	unsigned char y = SCREEN_H/2;
	mvprintw(y, x, s.c_str());
	char c = getch();
	switch(c)
	{
		case 'n':
			quit();
			return 1;
		default:
			return 1;
	}
}

int Dungeon::quit_action()
{
	std::string s = "Are you sure you want to quit? (y/n)";
	unsigned char x = SCREEN_W/2 - s.length()/2;
	unsigned char y = SCREEN_H/2;
	mvprintw(y, x, s.c_str());
	char c = getch();
	switch(c)
	{
		case 'y':
			quit();
		case 'n':
			current_state = GAME_CONTROL;
			return 0;
		default:
			return 1;
	}
}

void Dungeon::quit()
{
	delete this;
	NPCdef::deleteDefs();
	ItemDef::deleteDefs();
	endwin();
	exit(0);
}

std::ostream &operator<<(std::ostream &o, const Dungeon &d)
{
	int x, y;
	for (y = 0; y < DUNGEON_Y; y++)
	{
		for (x = 0; x < DUNGEON_X; x++)
		{
			o << d.map[x][y];
		}
		o << std::endl;
	}
	return o;
}

