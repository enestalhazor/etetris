#include <stdio.h>
#include "renderer.h"
#include <unistd.h>
#include "enes_util.h"
#include <string.h>
#include <stdlib.h>
#include "physics_engine.h"
#include "linked_list.h"
#include "tetromino.h"

#define ESC "\x1b"

// changes pixel(x, y) value to c in matrix
void set_pixel(char *matrix, int x, int y, char c, int resolution_x, int resolution_y)
{
    if (x >= resolution_x || x < 0 || y >= resolution_y || y < 0)
    {
        return;
    }

    matrix[y * resolution_x + x] = c;
}

char get_pixel(const char *matrix, int x, int y, int resolution_x, int resolution_y)
{
    return matrix[y * resolution_x + x];
}

int frame_count = 0;

void print_matrix(const char *matrix, const char *color_matrix, int resolution_x, int resolution_y)
{
    for (int i = 0; i < resolution_y; i++)
    {
        for (int j = 0; j < resolution_x; j++)
        {
            char c = get_pixel(matrix, j, i, resolution_x, resolution_y);
            if (c != ' ')
            {
                switch (get_pixel(color_matrix, j, i, resolution_x, resolution_y))
                {
                case 0:
                    printf(ESC "[48;2;255;255;255m");
                    printf(ESC "[38;2;255;255;255m");
                    break;

                case 1:
                    printf(ESC "[48;2;252;123;3m");
                    printf(ESC "[38;2;252;123;3m");

                    break;
                case 2:
                    printf(ESC "[48;2;252;244;3m");
                    printf(ESC "[38;2;252;244;3m");

                    break;

                case 3:
                    printf(ESC "[48;2;3;252;44m");
                    printf(ESC "[38;2;3;252;44m");

                    break;

                case 4:
                    printf(ESC "[48;2;3;132;252m");
                    printf(ESC "[38;2;3;132;252m");

                    break;

                case 5:
                    printf(ESC "[48;2;132;3;252m");
                    printf(ESC "[38;2;132;3;252m");

                    break;

                case 6:
                    printf(ESC "[48;2;252;3;3m");
                    printf(ESC "[38;2;252;3;3m");

                    break;

                case 7:
                    printf(ESC "[48;2;3;252;235m");
                    printf(ESC "[38;2;3;252;235m");

                    break;

                default:
                    break;
                }
            }
            printf("%c", c);
            printf(RESET_TEXT);
        }
        printf("\n");
    }
}

void cursor_top_left()
{
    printf(ESC "[H");
}

void draw_object_on_matrix(char *matrix, char *color_matrix, struct scene_object object, int resolution_x, int resolution_y)
{
    for (int h = 0; h < object.height; h++)
    {
        for (int w = 0; w < object.width; w++)
        {
            if (get_pixel(matrix, object.x + w, object.y + h, resolution_x, resolution_y) == ' ')
            {
                set_pixel(matrix, object.x + w, object.y + h, get_pixel(object.texture, w, h, object.width, object.height), resolution_x, resolution_y);
                set_pixel(color_matrix, object.x + w, object.y + h, object.color, resolution_x, resolution_y);
            }
        }
    }
}

void clear_matrix(char *matrix, int resolution_x, int resolution_y)
{
    memset(matrix, ' ', resolution_y * resolution_x);
}

void *renderer_start(void *parameters)
{
    struct renderer_parameters *p = (struct renderer_parameters *)parameters;
    int resolution_x = p->resolution_x;
    int resolution_y = p->resolution_y;
    struct scene *scene = p->scene;

    // create matrix
    char *matrix = (char *)malloc(resolution_y * resolution_x);
    p->matrix = matrix;
    char *color_matrix = (char *)malloc(resolution_y * resolution_x);

    while (1)
    {
        // clear the matrix before filling the matrix with object.
        clear_matrix(matrix, resolution_x, resolution_y);
        // for each object in the scene draw matrix
        for (int i = 0; i < scene->object_count; i++)
        {
            struct scene_object object = scene->objects[i];
            draw_object_on_matrix(matrix, color_matrix, object, resolution_x, resolution_y);
        }

        // Put the curser in home position (0, 0)
        cursor_top_left();

        // print the matrix on the screen
        print_matrix(matrix, color_matrix, resolution_x, resolution_y);

        frame_count++;

        printf("%d", frame_count);

        if (frame_count % 3 == 0)
        {

            for (int i = 0; i < scene->object_count; i++)
            {
                if (scene->objects[i].id < 1000 && scene->objects[i].is_landed == 0)
                {
                    if (physics_is_valid(&scene->objects[i], 'd', matrix, resolution_x, resolution_y))
                    {
                        scene->objects[i].y++;
                    }
                    else
                    {
                        scene->objects[i].is_landed = 1;
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
                }
            }
        }

        pthread_mutex_lock(&p->m);

        int c = shift(&p->queue);

        pthread_mutex_unlock(&p->m);
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
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(&scene->objects[flying_tetromino], 'l', matrix, resolution_x, resolution_y))
            {
                scene->objects[flying_tetromino].x--;
            }

            break;

        case 'd':
            if (scene->objects[flying_tetromino].is_landed == 0 && physics_is_valid(&scene->objects[flying_tetromino], 'r', matrix, resolution_x, resolution_y))
            {
                scene->objects[flying_tetromino].x++;
            }

            break;
        }

        // we wait before rendering the next frame
        usleep(1000 * (1000 / 30));
    }
}