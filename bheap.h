#ifndef BHEAP_H
#define BHEAP_H

//return negative if v2 is less than v2, 0 if equal, positive otherwise
typedef int (*Comparator)(void *v1, void *v2);

typedef struct bheap {
	void **items;
	int size;
	Comparator compare;
	int alloc_size;
} bheap_t;

void bheap_init(bheap_t *heap);
void bheap_add(bheap_t *heap, void *value);
void* bheap_remove(bheap_t *heap);
void bheap_item_changed(bheap_t *heap, void* item);
void bheap_destroy(bheap_t *heap);

#endif
