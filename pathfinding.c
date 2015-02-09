#include<stdio.h>
#include<stdlib.h>

#include "dungeon.h"

typedef struct path_queue_node {
	path_cell value;
	struct path_queue_node *next;
} path_queue_node;

typedef struct path_queue {
	path_queue_node *head, *tail;
	int size;
} path_queue;

void path_queue_add(path_queue *q, path_cell cell);
int path_queue_remove(path_queue *q, path_cell *cell);
void path_queue_init(path_queue *q);

void setPathingDistances()
{
	//TODO fill the entire map with distances from the player.
	monster_t player = monsters.list[0];
	path_cell start;
	start.x = player.x;
	start.y = player.y;
	start.dist = 0;
	dungeon.map[start.x][start.y].distFromPlayer=0;
	path_queue q;
	path_queue_init(&q);
	q.size = 0;
	while(q.size)
	{
		path_cell current;
		if(path_queue_remove(&q, &current))
		{
			printf("Oops. You did something wrong. You accessed an empty queue\n");
			return;
		};
		if(dungeon.map[current.x+1][current.y].tile==ter_room||
			dungeon.map[current.x+1][current.y].tile==ter_corridor)
		{
			path_cell toAdd;
			toAdd.dist = current.dist+1;
			toAdd.x = current.x+1;
			toAdd.y = current.y;
			path_queue_add(&q, toAdd);
		}
		if(dungeon.map[current.x][current.y+1].tile==ter_room||
			dungeon.map[current.x][current.y+1].tile==ter_corridor)
		{
			path_cell toAdd;
			toAdd.dist = current.dist+1;
			toAdd.x = current.x;
			toAdd.y = current.y+1;
			path_queue_add(&q, toAdd);
		}
		if(dungeon.map[current.x-1][current.y].tile==ter_room||
			dungeon.map[current.x-1][current.y].tile==ter_corridor)
		{
			path_cell toAdd;
			toAdd.dist = current.dist+1;
			toAdd.x = current.x-1;
			toAdd.y = current.y;
			path_queue_add(&q, toAdd);
		}
		if(dungeon.map[current.x][current.y-1].tile==ter_room||
			dungeon.map[current.x][current.y-1].tile==ter_corridor)
		{
			path_cell toAdd;
			toAdd.dist = current.dist+1;
			toAdd.x = current.x;
			toAdd.y = current.y-1;
			path_queue_add(&q, toAdd);
		}
	}
}

void path_queue_init(path_queue *q)
{
	q->size = 0;
	q->head = q->tail = NULL;
}

void path_queue_add(path_queue *q, path_cell cell)
{
	path_queue_node *qn;
	if(!(qn=malloc(sizeof(*qn))))
	{
		perror("malloc");
		exit(1);
	}
	qn->value = cell;
	qn->next = NULL;
	if(q->size==0)
	{
		q->head = qn;
	}
	else
	{
		q->head->next = qn;
	}
	q->tail = qn;
	q->size++;
}

int path_queue_remove(path_queue *q, path_cell *value)
{
	if(q->size == 0) 
	{
		return -1;
	}
	path_queue_node *tmp;
	q->size--;
	*value = q->head->value;
	tmp = q->head;
	q->head = q->head->next;
	free(tmp);
	if(!q->size)
	{
		q->tail = NULL;
	}

	return 0;
}
