#include<stdio.h>
#include<stdlib.h>

#include"bheap.h"

#define NUMBER 150

int compare(void *v1, void *v2)
{
	int *i1 = v1;
	int *i2 = v2;
	return *i2 - *i1;
}

void printheap(bheap_t *heap)
{
	int i;
	for(i=0;i<heap->size;i++)
	{
		int *val = heap->items[i];
		printf("%d ", *val);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	srand(0);
	bheap_t heap;
	heap.compare = compare;
	bheap_init(&heap);
	int arr[NUMBER];
	int i;
	for(i=0;i<NUMBER;i++)
	{
		arr[i] = rand()%100;
	}
	for(i=0;i<NUMBER;i++)
	{
		bheap_add(&heap, (void*)&arr[i]);
		printf("%d->", arr[i]);
		printheap(&heap);
	}
	for(i=0;i<5;i++)
	{
		int z = rand()%NUMBER;
		arr[z] = rand()%100;
		bheap_item_changed(&heap, (void*)&arr[z]);
		printheap(&heap);
	}//*/
	int ret[NUMBER];
	for(i=0;i<NUMBER;i++)
	{
		ret[i]=*(int*)bheap_remove(&heap);
		printheap(&heap);
	}
	for(i=0;i<NUMBER;i++)
	{
		printf("%d ", ret[i]);
	}
	printf("\n");
	bheap_destroy(&heap);
	return 0;
}
