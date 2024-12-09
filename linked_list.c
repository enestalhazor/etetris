#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/*
    linked list is a data structure.
*/

struct node
{
    int value;
    struct node *next;
};

static struct node *create_node(int value)
{
    struct node *p = (struct node *)malloc(sizeof(struct node));

    p->value = value;
    p->next = NULL;

    return p;
}

void list_init(struct linked_list *list)
{
    list->head = NULL;
    list->size = 0;
}

void list_print(const struct linked_list *list)
{
    const struct node *current = list->head;

    printf("(%d)[", list->size);

    while (current != NULL)
    {
        printf("%d, ", current->value);

        fflush(stdout);
        current = current->next;
    }

    printf("]\n\n");
}

void list_push(struct linked_list *list, int value)
{
    list->size++;
    struct node *new_node = create_node(value);

    if (list->head == NULL)
    {
        list->head = new_node;
        return;
    }

    struct node *current = list->head;

    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
}

int list_pop(struct linked_list *list)
{
    struct node *current = list->head;

    while (current->next->next != NULL)
    {
        current = current->next;

        if (current->next->next == NULL)
        {
            int temp = current->next->value;
            free(current->next);
            current->next = NULL;
            list->size--;
            return temp;
        }
    }
}

int list_shift(struct linked_list *list)
{
    if (list->head == NULL)
    {
        return 0;
    }
    struct node *temp = list->head->next;
    int p = list->head->value;
    free(list->head);
    list->head = temp;
    list->size--;

    return p;
}

void list_unshift(struct linked_list *list, int value)
{
    list->size++;
    struct node *new_node = create_node(value);
    new_node->next = list->head;
    list->head = new_node;
}

int list_get(const struct linked_list *list, int i)
{
    struct node *current = list->head;
    for (int j = 0; j < i; j++)
    {
        current = current->next;
    }

    return current->value;
}

void list_set(struct linked_list *list, int i, int value)
{
    struct node *current = list->head;

    for (int j = 0; j < i; j++)
    {
        current = current->next;
    }
    current->value = value;
}

void list_insert(struct linked_list *list, int i, int value)
{
    if (i == 0)
    {
        list_unshift(list, value);
        return;
    }

    list->size++;

    struct node *current = list->head;
    struct node *new_node = create_node(value);

    for (int j = 1; j < i; j++)
    {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

int list_delete_at(struct linked_list *list, int i)
{
    if (i == 0)
    {
        return list_shift(list);
    }

    list->size--;
    struct node *current = list->head;

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

void list_delete_all(struct linked_list *list, int value)
{
    struct node *current = list->head;
    int i = 0;

    while (current != NULL)
    {
        if (current->value == value)
        {
            list_delete_at(list, i);
            i--;
        }
        current = current->next;
        i++;
    }
}

void list_sort(struct linked_list *list)
{
    if (list->head == NULL)
    {
        return;
    }

    int i = 0;
    struct node *current = list->head;

    while (current->next != NULL)
    {
        struct linked_list right_list;
        right_list.head = current->next;
        int max_i = list_max(&right_list);

        if (current->value < list_get(&right_list, max_i))
        {
            int temp = current->value;
            current->value = list_get(&right_list, max_i);
            list_set(&right_list, max_i, temp);
        }

        current = current->next;
        i++;
    }
}

int list_max(const struct linked_list *list)
{
    if (list->head == NULL)
    {
        return -1;
    }

    const struct node *current = list->head;
    int max = current->value;
    int i = 0;
    int max_i = 0;

    while (current != NULL)
    {
        if (max < current->value)
        {
            max = current->value;
            max_i = i;
        }

        current = current->next;
        i++;
    }

    return max_i;
}
