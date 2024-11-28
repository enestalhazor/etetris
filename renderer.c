#include <stdio.h>
#include "renderer.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "physics_engine.h"

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

char get_pixel(char *matrix, int x, int y, int resolution_x, int resolution_y)
{
    return matrix[y * resolution_x + x];
}

int frame_count = 0;

void print_matrix(char *matrix, int resolution_x, int resolution_y)
{
    for (int i = 0; i < resolution_y; i++)
    {
        for (int j = 0; j < resolution_x; j++)
        {
            printf("%c", get_pixel(matrix, j, i, resolution_x, resolution_y));
        }
        printf("\n");
    }
}

void cursor_top_left()
{
    printf(ESC "[H");
}

void draw_object_on_matrix(char *matrix, struct scene_object object, int resolution_x, int resolution_y)
{
    for (int h = 0; h < object.height; h++)
    {
        for (int w = 0; w < object.width; w++)
        {
            set_pixel(matrix, object.x + w, object.y + h, get_pixel(object.texture, w, h, object.width, object.height), resolution_x, resolution_y);
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

    while (1)
    {
        // clear the matrix before filling the matrix with object.
        clear_matrix(matrix, resolution_x, resolution_y);
        // for each object in the scene draw matrix
        for (int i = 0; i < scene->object_count; i++)
        {
            struct scene_object object = scene->objects[i];
            draw_object_on_matrix(matrix, object, resolution_x, resolution_y);
        }

        // Put the curser in home position (0, 0)
        printf(ESC "c");
        cursor_top_left();

        // print the matrix on the screen
        print_matrix(matrix, resolution_x, resolution_y);

        frame_count++;

        printf("%d", frame_count);

         for (int i = 0; i < scene->object_count; i++)
         {
             if (physics_is_valid(&scene->objects[i], 'd', matrix, resolution_x, resolution_y))
             {
                 scene->objects[i].y++;
             }
         }

        // we wait before rendering the next frame
        usleep(1000 * (1000 / 30));
    }
}