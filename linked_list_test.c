#include <stdio.h>
#include "linked_list.h"
#include "enes_util.h"

int main()
{
    struct linked_list list;
    list_init(&list);
    for (int i = 0; i < 100; i++)
    {
        list_push(&list, get_random_number(0, 200));
    }
    list_sort(&list);
    list_print(&list);
    list_shift(&list);
    list_print(&list);


}