#include <stdio.h>
#include "linked_list.h"
#include "enes_util.h"

int main()
{
    struct linked_list list;
    list_init(&list);
    list_push(&list, 5);
    list_push(&list, 10);
    list_push(&list, 20);
    list_push(&list, 55);
    list_push(&list, 11);

    
    list_print(&list);
    int index = list_find(&list, 5);

    if(index != -1)
    {
        printf("found: %d. index\n", index);
    }
    else
    {
        printf("not found\n");
    }
}