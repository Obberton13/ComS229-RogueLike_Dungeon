#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "bheap.h"

#define PTRSIZE sizeof(void*)

static void percolate_up(bheap_t *heap, int pos);
static void percolate_down(bheap_t *heap, int pos);
static void swap(bheap_t *heap, int pos1, int pos2);

void bheap_init(bheap_t *heap)
{
	heap->items = malloc(PTRSIZE*10);
	heap->size = 0;
	heap->alloc_size = 10;
}

void bheap_add(bheap_t *heap, void *value)
{
	if(heap->size+1>=heap->alloc_size)
	{
		heap->items = realloc(heap->items, PTRSIZE*heap->alloc_size*2);
		heap->alloc_size*=2;
		if(!heap->items)
		{
			printf("Unable to reallocate storage for you binary heap");
			exit(1);
		}
	}
	heap->items[heap->size] = value;
	percolate_up(heap, heap->size);
	heap->size++;
}

void* bheap_remove(bheap_t *heap)
{
	void*  toReturn = heap->items[0];
	heap->items[0] = heap->items[--heap->size];
	heap->items[heap->size] = NULL;
	percolate_down(heap, 0);
	return toReturn;
}

void bheap_item_changed(bheap_t *heap, void *item)
{
	//printf("ITEM CHANGED: %p\n", item);
	int x;
	for(x=0;x<heap->size;x++)
	{
		if(heap->items[x]==item)
		{
			break;
		}
	}
	swap(heap, x, heap->size-1);
	percolate_up(heap, heap->size-1);
	//I have to check both because it is possible that the last value was larger than the item removed.
	percolate_down(heap, x);
}

void bheap_destroy(bheap_t *heap)
{
	free(heap->items);
}

static void percolate_up(bheap_t *heap, int pos)
{
	if(!pos) return;
	int parent = (pos-1)/2; //odd numbers truncate down, so no worries there.
	if(heap->compare(heap->items[pos], heap->items[parent])>0)
	{
		swap(heap, pos, parent);
		percolate_up(heap, parent);
	}
}

static void percolate_down(bheap_t *heap, int pos)
{
	int right = (pos+1)*2;
	int left = right-1;
	if(right<heap->size)
	{
		if(heap->compare(heap->items[left], heap->items[right])>=0)//if left is greater than/equal to right
		{
			if(heap->compare(heap->items[left], heap->items[pos])>0)//and left is greater than pos
			{
				swap(heap, pos, left);//then swap the two
				percolate_down(heap, left);//and continue in the left subtree
			}
		}
		else//otherwise (if right > left)
		{
			if(heap->compare(heap->items[right], heap->items[pos])>0)//if right is greater than pos
			{
				swap(heap, pos, right);//swap the two
				percolate_down(heap, right);//and continue in the right subtree
			}
		}
	}
	else if(left<heap->size)
	{
		if(heap->compare(heap->items[left], heap->items[pos])>0)
		{
			swap(heap, left, pos);
		}
	}
}

static void swap(bheap_t *heap, int pos1, int pos2)
{
	void *t;
	t = heap->items[pos1];
	heap->items[pos1]=heap->items[pos2];
	heap->items[pos2]=t;
}
