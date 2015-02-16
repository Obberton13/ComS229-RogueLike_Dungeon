#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "bheap.h"

#define PTRSIZE sizeof(void*)

static void percolate_up(bheap_t *heap, int pos);
static void percolate_down(bheap_t *heap, int pos);
static void swap(bheap_t *heap, int pos1, int pos2);

void bheap_init(bheap_t *heap, Comparator comp)
{
	heap->items = malloc(PTRSIZE*10);
	heap->size = 0;
	heap->compare = comp;
	heap->alloc_size = 10;
}

void bheap_add(bheap_t *heap, void *value)
{
	if(heap->size+1>=heap->alloc_size)
	{//TODO This probably has something wrong with it. Need to initialize all of the things to NULL
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
	heap->size--;
	heap->items[0] = heap->items[heap->size];
	heap->items[heap->size] = NULL;
	percolate_down(heap, 0);
	return toReturn;
}

void bheap_item_changed(bheap_t *heap, void *item)
{
	int x;
	for(x=0;x<heap->size;x++)
	{
		if(heap->items[x]==item)
		{
			break;
		}
	}
	swap(heap, x, heap->size);
	percolate_up(heap, heap->size);
	//I have to check both because it is possible that the last value was larger than the item removed.
	percolate_down(heap, x);
	percolate_up(heap, x);
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
		if(heap->compare(heap->items[left], heap->items[right])>0)
		{
			swap(heap, pos, left);
			percolate_down(heap, left);
		}
		else
		{
			swap(heap, pos, right);
			percolate_down(heap, right);
		}
	}
	else if(left<heap->size)
	{
		swap(heap, pos, left);
	}
}

static void swap(bheap_t *heap, int pos1, int pos2)
{
	void *t;
	t = heap->items[pos1];
	heap->items[pos1]=heap->items[pos2];
	heap->items[pos2]=t;
}
