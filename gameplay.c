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

static struct scene_object new_tetromino()
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
    struct scene_object o = tetromino_create(get_random_number(0, 999), c);

    return o;
}

void gameplay_spawn_tetromino(struct scene *scene)
{
    struct scene_object *pending_tetromino = scene_get_object(scene, 1005);
    if (pending_tetromino == NULL)
    {
        struct scene_object t1 = new_tetromino();
        t1.x = scene->chamber_width / 2 - 2;
        scene_add_object(scene, t1);
    }
    else
    {
        pending_tetromino->id = get_random_number(0, 999);
        pending_tetromino->x = scene->chamber_width / 2 - 2;
        pending_tetromino->y = 0;
    }

    struct scene_object *right_wall = scene_get_object(scene, 1002);
    struct scene_object tetromino = new_tetromino();
    tetromino.id = 1005;
    tetromino.x = right_wall->x + 5;
    tetromino.y = 3;
    scene_add_object(scene, tetromino);
}

void gameplay_rule(struct scene *scene)
{
    int res_x = scene->res_x;
    int res_y = scene->res_y;

    gameplay_time++;
    pthread_mutex_lock(&scene->mutex);

    struct scene_object *score = scene_get_object(scene, 1004);
    memset(score->texture, ' ', score->width * score->height);
    sprintf(score->texture, "score: %d", scene->score);

    int speed = 10 - scene->score / 40;

    if (gameplay_time % speed == 0)
    {
        for (int i = 0; i < scene->object_count; i++)
        {
            struct scene_object *obj = &scene->objects[i];
            if (obj->id < 1000 && obj->is_landed == 0)
            {
                if (physics_is_valid(obj->id, 'd', scene))
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
                                set_pixel(pile->texture, obj->x - 1 + w, obj->y - 1 + h, get_pixel(obj->texture, w, h, obj->width, obj->height), pile->width, pile->height);
                                if ((obj->y - 1 + h) == 0)
                                {
                                    scene->game_over = 1;
                                }
                            }
                        }
                    }

                    scene->score = scene->score + check_score(scene, res_x, res_y);
                    scene_remove_object(scene, obj->id);
                    gameplay_spawn_tetromino(scene);

                    if (scene->game_over)
                    {
                        scene->score = 0;
                        struct scene_object *pile = scene_get_object(scene, 1003);
                        memset(pile->texture, ' ', pile->height * pile->width);
                        scene->game_over = 0;
                    }
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
            if (scene->objects[i].id < 1000)
            {
                flying_tetromino = i;
            }
        }

        switch (c)
        {
        case 'a':
        case 'A':
            if (physics_is_valid(scene->objects[flying_tetromino].id, 'l', scene))
            {
                scene->objects[flying_tetromino].x--;
            }
            break;

        case 'd':
        case 'D':
            if (physics_is_valid(scene->objects[flying_tetromino].id, 'r', scene))
            {
                scene->objects[flying_tetromino].x++;
            }
            break;

        case 'w':
        case 'W':
            if (physics_is_valid(scene->objects[flying_tetromino].id, 'o', scene))
            {
                tetromino_rotate(&scene->objects[flying_tetromino]);
            }

            break;

        case 's':
        case 'S':
            if (physics_is_valid(scene->objects[flying_tetromino].id, 'd', scene))
            {
                scene->objects[flying_tetromino].y++;
            }
            break;

        case 'r':
        case 'R':
            scene->score = 0;
            struct scene_object *pile = scene_get_object(scene, 1003);
            memset(pile->texture, ' ', pile->height * pile->width);
            scene->game_over = 0;

            break;
        }
    }

    pthread_mutex_unlock(&scene->mutex);
}
