#include <stdio.h>
#include "gameplay.h"
#include "enes_util.h"
#include "tetromino.h"
#include "physics_engine.h"
#include "linked_list.h"

struct node *queue;
pthread_mutex_t queue_mutex;

void *gameplay_input(void *p)
{
    while (1)
    {
        char c = fgetc(stdin);
        pthread_mutex_lock(&queue_mutex);
        push(&queue, c);
        pthread_mutex_unlock(&queue_mutex);
    }
}

static void add_new_tetromino(struct scene *scene)
{
    char c = 0;
    int r = get_random_number(0, 6);

    switch (r)
    {
    case 0:
        c = 't';
        break;
    case 1:
        c = 's';
        break;
    case 2:
        c = 'z';
        break;
    case 3:
        c = 'i';
        break;
    case 4:
        c = 'l';
        break;
    case 5:
        c = 'o';
        break;
    case 6:
        c = 'j';
        break;
    default:
        break;
    }
    struct scene_object o = tetromino_create(get_random_number(0, 1000), c);
    o.x = 20;
    o.y = 0;
    scene_add_object(scene, o);
}

void gameplay_rule(struct scene *scene)
{
    pthread_mutex_lock(&scene->mutex);
    for (int i = 0; i < scene->object_count; i++)
    {
        if (scene->objects[i].id < 1000 && scene->objects[i].is_landed == 0)
        {
            if (physics_is_valid(&scene->objects[i], 'd', scene))
            {
                scene->objects[i].y++;
            }
            else
            {
                scene->objects[i].is_landed = 1;
                add_new_tetromino(scene);
            }
        }
    }

    while (len(queue) != 0)
    {
        pthread_mutex_lock(&queue_mutex);
        int c = shift(&queue);
        pthread_mutex_unlock(&queue_mutex);
        int flying_tetromino = -1;

        for (int i = 0; i < scene->object_count; i++)
        {
            if (scene->objects[i].is_landed == 0)
            {
                flying_tetromino = i;
            }
        }

        switch (c)
        {
        case 'a':
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(&scene->objects[flying_tetromino], 'l', scene))
            {
                scene->objects[flying_tetromino].x--;
            }

            break;

        case 'd':
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(&scene->objects[flying_tetromino], 'r', scene))
            {
                scene->objects[flying_tetromino].x++;
            }

            break;
        }
    }

    pthread_mutex_unlock(&scene->mutex);
}
