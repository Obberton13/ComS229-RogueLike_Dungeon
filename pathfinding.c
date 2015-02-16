#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include"dungeon.h"
#include"bheap.h"
#include"pathfinding.h"

typedef struct path_cell {
	struct path_cell *previous;
	int distance, x, y;
} path_cell_t;

static int compare(void *v1, void *v2);
static void trace_path(path_cell_t *distance[DUNGEON_X][DUNGEON_Y]);

void find_path(int monsterIndex)
{
	path_cell_t* distance[DUNGEON_X][DUNGEON_Y];
	
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
			distance[x][y]->previous = NULL;
		}
	}
	x = dungeon.monsters.list[monsterIndex].x;
	y = dungeon.monsters.list[monsterIndex].y;
	distance[x][y]->distance = 0;
	bheap_t heap;
	bheap_init(&heap, compare);
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
				int notify = (INT_MAX-distance[newX][newY]->distance);
				distance[newX][newY]->distance = dist+1;
				distance[newX][newY]->previous = current;
				if(notify)
				{
					bheap_item_changed(&heap, distance[newX][newY]);
				}
				else
				{
					bheap_add(&heap, distance[newX][newY]);
				}
			}
		}
	}
	trace_path(distance);
	bheap_destroy(&heap);
	for(x=0;x<DUNGEON_X;x++)
	{
		for(y=0;y<DUNGEON_Y;y++)
		{
			free(distance[x][y]);
		}
	}
}

static int compare(void *v1, void *v2)
{
	path_cell_t *c1 = v1;
	path_cell_t *c2 = v2;
	return c2->distance - c1->distance;
}

static void trace_path(path_cell_t *distance[DUNGEON_X][DUNGEON_Y])
{
	int x = dungeon.monsters.list[0].x;
	int y = dungeon.monsters.list[0].y;
	path_cell_t *previous = distance[x][y]->previous;
	while(previous)
	{
		dungeon.map[previous->x][previous->y].tile = ter_debug;
		previous = previous->previous;
	}
}
