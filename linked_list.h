#ifndef LINKED_LIST
#define LINKED_LIST
#include <stdio.h>
#include <stdlib.h>

/*
    linked list is a data structure.
*/

struct linked_list
{
    struct node *head;
    int size;
};

void list_init(struct linked_list *list);
void list_print(const struct linked_list *list);
void list_push(struct linked_list *list, int value);
int list_pop(struct linked_list *list);
int list_shift(struct linked_list *list);
void list_unshift(struct linked_list *list, int value);
int list_get(const struct linked_list *list, int i);
void list_set(struct linked_list *list, int i, int value);
void list_insert(struct linked_list *list, int i, int value);
int list_delete_at(struct linked_list *list, int i);
void list_delete_all(struct linked_list *list, int value);
void list_sort(struct linked_list *list);
int list_max(const struct linked_list *list);
int list_find(const struct linked_list *list, int value);

#endif