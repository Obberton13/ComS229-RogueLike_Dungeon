#include "Dungeon.h"
#include "NPC.h"
#include "Item.h"
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <climits>

static int gameLoop();

int main(int argc, char *argv[])
{
	if (NPCdef::parseDefs())
	{
		return 1;
	}
	if (ItemDef::parseObjects())
	{
		return 1;
	}
	srand(time(0));
	while (!gameLoop()){}
	NPCdef::deleteDefs();
	ItemDef::deleteDefs();
	return 0;
}

int gameLoop()
{
	Dungeon *d = new Dungeon;
	while (d->game_loop()){}
	std::cout << *d;
	delete d;
	return 1;
}

Dungeon::Dungeon()
{
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
	spawn_monsters();
	spawn_items();
	find_monster_distances();
}

void Dungeon::initialize()
{
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
	int rm = std::rand() % rooms.size();
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
		int rm = std::rand() % rooms.size();
		x = (rand() % rooms[rm].w) + rooms[rm].x;
		y = (rand() % rooms[rm].h) + rooms[rm].y;
		NPC* tmp = new NPC(x, y);
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
				&& map[newX][newY].get_hardness() == 0)
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
	for (i = 0; i < npcs.size(); i++)
	{
	}
	return 0;
}

int Dungeon::move_monster(unsigned int index, unsigned char x, unsigned char y)
{
	DungeonTile t = map[x][y];
	NPC *monster = npcs[index];
	unsigned int abilities = monster->getAbilities();
	if ((abilities&NPC_TUNNEL)&&(map[x][y].get_hardness()!=0))
	{
		map[x][y].set_type(ter_room);
		map[x][y].set_hardness(0);
	}
	else if (abilities&NPC_PASS) { }
	else
	{
		return 1;
	}
	map[monster->getX()][monster->getY()].set_monster(NULL);
	map[x][y].set_monster(monster);
	monster->setPos(x, y);
	return 0;
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

