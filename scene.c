#include "scene.h"
#include <string.h>
#include <stdio.h>

struct scene scene_create(void)
{
    struct scene scene;
    scene.object_count = 0;
    memset(scene.objects, 0, sizeof(struct scene_object) * 100);
    return scene;
}

void scene_add_object(struct scene *scene, struct scene_object object)
{
    scene->objects[scene->object_count++] = object;
}

void scene_remove_object(struct scene *scene, int id)
{
    struct scene_object temp[100] = {0};
    int p = 0;

    for (int i = 0; i < scene->object_count; i++)
    {
        struct scene_object o = scene->objects[i];

        if (o.id != id)
        {
            temp[p++] = o;
        }
    }

    memcpy(scene->objects, temp, 100 * sizeof(struct scene_object));
    scene->object_count--;
}

struct scene_object scene_create_object(int id, int width, int height)
{
    struct scene_object object;
    object.id = id;
    object.width = width;
    object.height = height;
    object.color = 0;
    object.x = 0;
    object.y = 0;

    return object;
}

void scene_print_info(struct scene scene)
{
     printf("\nscene (%d objects)\n", scene.object_count);
    
    for (int i = 0; i < scene.object_count; i++)
    {
        scene_object_print_info(scene.objects[i]);
    }
    
}

void scene_object_print_info(struct scene_object object)
{
    printf("scene object #%d\n", object.id);
    printf("dimensions = %d x %d\n", object.width, object.height);
    printf("position = %d, %d\n", object.x, object.y);
    printf("color = %d\n", object.color);
}

