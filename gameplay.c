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
    int total_score = 0;
    char matrix[100000];
    memset(matrix, ' ', 100000);

    for (int i = 0; i < scene->object_count; i++)
    {
        struct scene_object o = scene->objects[i];
        for (int h = 0; h < o.height; h++)
        {
            for (int w = 0; w < o.width; w++)
            {
                if (get_pixel(o.texture, w, h, o.width, o.height) != ' ')
                {
                    set_pixel(matrix, o.x + w, o.y + h, get_pixel(o.texture, w, h, o.width, o.height), res_x, res_y);
                }
            }
        }
    }

    for (int j = 0; j < res_y - 2; j++)
    {
        int score = 1;
        for (int i = 0; i < res_x; i++)
        {
            if (get_pixel(matrix, i, j, res_x, res_y) != '#')
            {
                score = 0;
                break;
            }
        }
        if (score)
        {
            total_score = total_score + 10;
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
    o.x = 12;
    o.y = 0;
    scene_add_object(scene, o);
}

void gameplay_rule(struct scene *scene, int res_x, int res_y)
{
    gameplay_time++;
    pthread_mutex_lock(&scene->mutex);
    if (gameplay_time % 6 == 0)
    {
        for (int i = 0; i < scene->object_count; i++)
        {
            if (scene->objects[i].id < 1000 && scene->objects[i].is_landed == 0)
            {
                if (physics_is_valid(scene->objects[i].id, 'd', scene, res_x, res_y))
                {
                    scene->objects[i].y++;
                }
                else
                {
                    scene->objects[i].is_landed = 1;
                    scene->score = scene->score + check_score(scene, res_x, res_y);
                    add_new_tetromino(scene);
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
