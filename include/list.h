#ifndef GAME_LIBRARY_LIST_H
#define GAME_LIBRARY_LIST_H

#include <stdio.h>

#define DEFAULT_CAPACITY  12
#define CAPACITY_INCREASE 2

typedef struct list_item {
	void *value;
	int index;
} list_item_t;

typedef struct list {
	list_item_t **items;
	int size;
	int capacity;
} list_t;

int push_to(list_t *list, list_item_t *item);
int pop(list_t *list);
int remove_at(list_t *list, const int index);
int free_list(list_t *list);

#endif //  GAME_LIBRARY_LIST_H
