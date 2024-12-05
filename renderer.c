#include <stdio.h>
#include "renderer.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "enes_util.h"

#define ESC "\x1b"

// changes pixel(x, y) value to c in matrix
static void set_pixel(char *matrix, int x, int y, char c, int resolution_x, int resolution_y)
{
    if (x >= resolution_x || x < 0 || y >= resolution_y || y < 0)
    {
        return;
    }

    matrix[y * resolution_x + x] = c;
}

static char get_pixel(const char *matrix, int x, int y, int resolution_x, int resolution_y)
{
    return matrix[y * resolution_x + x];
}

int frame_count = 0;

static void print_matrix(const char *matrix, const char *color_matrix, int resolution_x, int resolution_y)
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

static void cursor_top_left()
{
    printf(ESC "[H");
}

static void draw_object_on_matrix(char *matrix, char *color_matrix, const struct scene_object object, int resolution_x, int resolution_y)
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

static void clear_matrix(char *matrix, int resolution_x, int resolution_y)
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
    char *color_matrix = (char *)malloc(resolution_y * resolution_x);

    while (1)
    {
        // clear the matrix before filling the matrix with object.
        clear_matrix(matrix, resolution_x, resolution_y);
        // for each object in the scene draw matrix
        pthread_mutex_lock(&scene->mutex);
        if (scene->game_over)
        {

            printf("\n\nGame over: %d\n", scene->score);
            return NULL;
        }
        for (int i = 0; i < scene->object_count; i++)
        {
            const struct scene_object object = scene->objects[i];
            draw_object_on_matrix(matrix, color_matrix, object, resolution_x, resolution_y);
        }
        pthread_mutex_unlock(&scene->mutex);

        // Put the curser in home position (0, 0)
        cursor_top_left();

        // print the matrix on the screen
        print_matrix(matrix, color_matrix, resolution_x, resolution_y);

        frame_count++;

        printf("score= %d", scene->score);

        p->work(scene, resolution_x, resolution_y);

        // we wait before rendering the next frame
        usleep(1000 * (1000 / 30));
    }
}