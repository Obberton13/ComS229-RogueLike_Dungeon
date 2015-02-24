#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include"dungeon.h"
#include"bheap.h"
#include"pathfinding.h"

typedef struct path_cell {
	int distance, x, y;
} path_cell_t;

static int compare(void *v1, void *v2);

void find_paths()
{
	path_cell_t* distance[DUNGEON_X][DUNGEON_Y];//array of array of pointers to path cell t
	
	int x, y;
	//initialize all the things
	for(x=0;x<DUNGEON_X;x++)
	{
		for(y=0;y<DUNGEON_Y;y++)
		{
			distance[x][y] = malloc(sizeof(path_cell_t));
			distance[x][y]->distance = INT_MAX;
			distance[x][y]->x = x;
			distance[x][y]->y = y;
		}
	}
	x = dungeon.monsters.list[0].x;
	y = dungeon.monsters.list[0].y;
	distance[x][y]->distance = 0;
	dungeon.map[x][y].distToPlayer = 0;
	bheap_t heap;
	heap.compare = compare;
	bheap_init(&heap);
	bheap_add(&heap, distance[x][y]);
	char offsetX[8] = {0,0,1,1,1,-1,-1,-1};
	char offsetY[8] = {1,-1,1,0,-1,1,0,-1};
	while(heap.size)
	{
		path_cell_t *current = bheap_remove(&heap);
		x = current->x;
		y = current->y;
		int dist = current->distance;
		int i;
		for(i=0;i<8;i++)
		{
			int newX = x+offsetX[i];
			int newY = y+offsetY[i];
			if((distance[newX][newY]->distance>dist+1)
			&&((dungeon.map[newX][newY].tile==ter_room)
			||(dungeon.map[newX][newY].tile==ter_corridor)))
			{
				dungeon.map[newX][newY].distToPlayer = dist+1;
				int notify = (INT_MAX-distance[newX][newY]->distance);
				distance[newX][newY]->distance = dist+1;
				if(notify)
				{
					bheap_item_changed(&heap, &distance[newX][newY]);
				}
				else
				{
					bheap_add(&heap, distance[newX][newY]);
				}
			}
		}
	}
	bheap_destroy(&heap);
	for(x=0;x<DUNGEON_X;x++)
	{
		for(y=0;y<DUNGEON_Y;y++) 
		{
			free(distance[x][y]);
		}
	}
	x = dungeon.monsters.list[0].x;
	y = dungeon.monsters.list[0].y;
	dungeon.map[x][y].distToPlayer = 0;
}

static int compare(void *v1, void *v2)
{
	path_cell_t *c1 = v1;
	path_cell_t *c2 = v2;
	return c2->distance - c1->distance;
}

void move_monster(int monsterIndex)
{
	int x = dungeon.monsters.list[monsterIndex].x;
	int y = dungeon.monsters.list[monsterIndex].y;
	int offsetX[8] = {0,0,1,1,1,-1,-1,-1};
	int offsetY[8] = {1,-1,1,0,-1,1,0,-1};
	int dist = INT_MAX;
	int i, j=0, newX, newY;
	for(i=0;i<8;i++)
	{
		newX = x+offsetX[i];
		newY = y+offsetY[i];
		if(dungeon.map[newX][newY].distToPlayer<dist)
		{
			dist = dungeon.map[newX][newY].distToPlayer;
			j=i;
		}
	}
	newX = x + offsetX[j];
	newY = y + offsetY[j];
	if(dungeon.map[newX][newY].monsterIndex!=dungeon.monsters.max)//If it is a monster
	{
		dungeon.monsters.list[dungeon.map[newX][newY].monsterIndex].initiative = -1;//set its initiative to -1 (kills it)
	}
	dungeon.monsters.list[monsterIndex].x = newX;//move the monster
	dungeon.monsters.list[monsterIndex].y = newY;
	dungeon.map[x][y].monsterIndex = dungeon.monsters.max;
	dungeon.map[newX][newY].monsterIndex = monsterIndex;
}
