#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/*
    linked list is a data structure.
*/

struct node *create_node(int value)
{
    struct node *p = (struct node *)malloc(sizeof(struct node));

    p->value = value;
    p->next = NULL;

    return p;
}

void print_linked_list(const struct node *root)
{
    const struct node *current = root;
    printf("[");

    while (current != NULL)
    {
        printf("%d, ", current->value);

        fflush(stdout);
        current = current->next;
    }

    printf("]\n");
}

void push(struct node **root, int value)
{
    struct node *new_node = create_node(value);

    if (*root == NULL)
    {
        *root = new_node;
        return;
    }

    struct node *current = *root;

    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
}

int pop(struct node *root)
{
    struct node *current = root;

    while (current->next->next != NULL)
    {
        current = current->next;

        if (current->next->next == NULL)
        {
            int temp = current->next->value;
            free(current->next);
            current->next = NULL;
            return temp;
        }
    }
}

int shift(struct node **root)
{
    if (*root == NULL)
    {
        return 0;
    }
    struct node *temp = (*root)->next;
    int p = (*root)->value;
    free(*root);
    *root = temp;

    return p;
}

void unshift(struct node **root, int value)
{
    struct node *new_node = create_node(value);
    new_node->next = *root;
    *root = new_node;
}

int len(struct node *root)
{
    struct node *current = root;
    int i = 0;

    while (current != NULL)
    {
        current = current->next;
        i++;
    }

    return i;
}

int get(struct node *root, int i)
{
    struct node *current = root;
    for (int j = 0; j < i; j++)
    {
        current = current->next;
    }

    return current->value;
}

void set(struct node *root, int i, int value)
{
    struct node *current = root;

    for (int j = 0; j < i; j++)
    {
        current = current->next;
    }
    current->value = value;
}

void insert(struct node **root, int i, int value)
{
    if (i == 0)
    {
        unshift(root, value);
        return;
    }

    struct node *current = *root;
    struct node *new_node = create_node(value);

    for (int j = 1; j < i; j++)
    {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

int delete_at(struct node **root, int i)
{
    if (i == 0)
    {
        return shift(root);
    }

    struct node *current = *root;

    for (int j = 1; j < i; j++)
    {
        current = current->next;
    }

    struct node *deleted = current->next;
    current->next = current->next->next;
    int deleted_num = deleted->value;
    free(deleted);

    return deleted_num;
}

void delete_all(struct node **root, int value)
{
    struct node *current = *root;
    int i = 0;

    while (current != NULL)
    {
        if (current->value == value)
        {
            delete_at(root, i);
            i--;
        }
        current = current->next;
        i++;
    }
}
