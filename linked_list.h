#ifndef LINKED_LIST
#define LINKED_LIST
#include <stdio.h>
#include <stdlib.h>


/*
    linked list is a data structure.
*/

struct node
{
    int value;
    struct node *next;
};

struct node *create_node(int value);

void print_linked_list(const struct node *root);

void push(struct node **root, int value);

int pop(struct node *root);

int shift(struct node **root);

void unshift(struct node **root, int value);

int len(struct node *root);

int get(struct node *root, int i);

void set(struct node *root, int i, int value);

void insert(struct node **root, int i, int value);

int delete_at(struct node **root, int i);

void delete_all(struct node **root, int value);
 
#endif