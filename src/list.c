#include "../include/list.h"
#include "../include/utils.h"

#include <string.h>
#include <stdlib.h>

int push_to(list_t *list, list_item_t *item) {
  if (list == NULL || item == NULL) {
    perror("list or item cannot be null!\n");		
		return false;
	}

  if (list->size == 0) {
    list->items = (list_item_t**)malloc(sizeof(list_item_t*) * list->capacity);
  }

  if (list->size == list->capacity) {
    list->capacity += CAPACITY_INCREASE;

    list_item_t **new_items = (list_item_t**) realloc(list->items, sizeof(list_item_t*) * list->capacity);
    if (new_items == NULL) {
      perror("Memory Allocation For new_items!\n");
      return false;
    }

    list->items = new_items;
  }

  item->index = list->size;
  list->items[list->size] = (list_item_t*)item;
  list->size++;

  return true;
}

int pop(list_t *list) {
  if (list == NULL) {
    perror("list cannot be null!\n");
    return false;
  }

  if (list->size == 0) {
    perror("list is already empty!\n");
    return false;
  }

  return remove_at(list, list->size - 1);
}

int remove_at(list_t *list, const int index) {
  if (list == NULL) {
    perror("list cannot be null!\n");
    return false;
  }

  if (list->size == 0) {
    perror("list is already empty!\n");
    return true;
  }

  for (int i = 0; i < list->size; i++) {
    list->items[i] = list->items[i + 1];
    if (list->items[i] != NULL && i <= index) {
      list->items[i]->index = i;
    }
  }

  list->items[list->size - 1] = NULL;
  list->size--;

  if (list->capacity - list->size >= CAPACITY_INCREASE) {
    list->capacity -= CAPACITY_INCREASE;
    list_item_t **temp_items = (list_item_t**) realloc(list->items, sizeof(list_item_t*) * list->capacity);

    if (temp_items == NULL) {
      perror("Memory Allocation For temp_items!\n");
      return false;
    }

    list->items = temp_items;
  }

  return true;
}

int free_list(list_t *list) {
  printf("List cleaning...\n");
  if (list == NULL) return true;

  for (int i = 0; i < list->size; i++) {
    if (list->items[i] != NULL) {
      free(list->items[i]);
    }
  }

  printf("List items cleaned...\n");
  free(list->items);
  free(list);
  return true;
}

