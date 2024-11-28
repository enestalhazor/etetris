#include <stdio.h>
#include "scene.h"

int main(void)
{
    struct scene_object object1 = scene_create_object(1, 5, 3);
    struct scene_object object2 = scene_create_object(2, 3, 7);
    struct scene_object object3 = scene_create_object(3, 1, 5);
    struct scene_object object4 = scene_create_object(4, 4, 1);
    struct scene_object object5 = scene_create_object(5, 2, 8);

    struct scene scene1 = scene_create();
    scene_add_object(&scene1, object1);
    scene_add_object(&scene1, object2);
    scene_add_object(&scene1, object3);
    scene_add_object(&scene1, object4);
    scene_add_object(&scene1, object5);
    scene_print_info(scene1);

    scene_remove_object(&scene1, 2);
    scene_remove_object(&scene1, 5);
    scene_print_info(scene1);

    return 0;
}