#include <stdio.h>
#include "gameplay.h"
#include "enes_util.h"
#include "tetromino.h"
#include "physics_engine.h"
#include <string.h>
#include "linked_list.h"

struct node *queue;
pthread_mutex_t queue_mutex;
int gameplay_time = 0;

static char get_pixel(const char *matrix, int x, int y, int resolution_x, int resolution_y)
{
    return matrix[y * resolution_x + x];
}

static void set_pixel(char *matrix, int x, int y, char c, int resolution_x, int resolution_y)
{
    if (x >= resolution_x || x < 0 || y >= resolution_y || y < 0)
    {
        return;
    }

    matrix[y * resolution_x + x] = c;
}

static int check_score(struct scene *scene, int res_x, int res_y)
{
    struct scene_object *pile = scene_get_object(scene, 1003);
    int total_score = 0;

    for (int h = 0; h < pile->height; h++)
    {
        int full = 1;
        for (int w = 0; w < pile->width; w++)
        {
            if (get_pixel(pile->texture, w, h, pile->width, pile->height) != '#')
            {
                full = 0;
                break;
            }
        }
        if (full)
        {
            total_score = total_score + 10;
            char copy[10000] = {0};
            memcpy(copy, pile->texture, pile->width * pile->height);

            memset(pile->texture, ' ', pile->width);
            memcpy(pile->texture + pile->width, copy, h * pile->width);
        }
    }
    return total_score;
}

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

void gameplay_spawn_tetromino(struct scene *scene)
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
    o.x = scene->res_x / 2 - 2;
    o.y = 0;
    scene_add_object(scene, o);
}

void gameplay_rule(struct scene *scene, int res_x, int res_y)
{
    gameplay_time++;
    pthread_mutex_lock(&scene->mutex);
    if (gameplay_time % 10 == 0)
    {
        for (int i = 0; i < scene->object_count; i++)
        {
            struct scene_object *obj = &scene->objects[i];
            if (obj->id < 1000 && obj->is_landed == 0)
            {
                if (physics_is_valid(obj->id, 'd', scene, res_x, res_y))
                {
                    obj->y++;
                }
                else
                {
                    obj->is_landed = 1;
                    struct scene_object *pile = scene_get_object(scene, 1003);

                    for (int h = 0; h < obj->height; h++)
                    {
                        for (int w = 0; w < obj->width; w++)
                        {
                            if (get_pixel(obj->texture, w, h, obj->width, obj->height) != ' ')
                            {
                                set_pixel(pile->texture, obj->x - 2 + w, obj->y - 2 + h, get_pixel(obj->texture, w, h, obj->width, obj->height), pile->width, pile->height);
                                if ((obj->y - 2 + h) == 0)
                                {
                                    scene->game_over = 1;
                                    pthread_mutex_unlock(&scene->mutex);
                                    return;
                                }
                            }
                        }
                    }

                    scene->score = scene->score + check_score(scene, res_x, res_y);
                    scene_remove_object(scene, obj->id);
                    gameplay_spawn_tetromino(scene);
                }
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
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(scene->objects[flying_tetromino].id, 'l', scene, res_x, res_y))
            {
                scene->objects[flying_tetromino].x--;
            }
            break;

        case 'd':
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(scene->objects[flying_tetromino].id, 'r', scene, res_x, res_y))
            {
                scene->objects[flying_tetromino].x++;
            }
            break;

        case 'w':
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(scene->objects[flying_tetromino].id, 'o', scene, res_x, res_y))
            {
                tetromino_rotate(&scene->objects[flying_tetromino]);
            }

            break;

        case 's':
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(scene->objects[flying_tetromino].id, 'd', scene, res_x, res_y))
            {
                scene->objects[flying_tetromino].y++;
            }
            break;
        }
    }

    pthread_mutex_unlock(&scene->mutex);
}
