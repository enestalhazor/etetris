#include <stdio.h>
#include "physics_engine.h"
#include <string.h>
#include "tetromino.h"


static char get_pixel(const char *matrix, int x, int y, int resolution_x, int resolution_y)
{
    return matrix[y * resolution_x + x];
}

// changes pixel(x, y) value to c in matrix
static void set_pixel(char *matrix, int x, int y, char c, int resolution_x, int resolution_y)
{
    if (x >= resolution_x || x < 0 || y >= resolution_y || y < 0)
    {
        return;
    }

    matrix[y * resolution_x + x] = c;
}

int physics_is_valid(int id, char direction, const struct scene *scene)
{
    int res_x = scene->res_x;
    int res_y = scene->res_y;


    struct scene clone = *scene;
    struct scene_object *object = NULL;

    for (int i = 0; i < clone.object_count; i++)
    {
        if (clone.objects[i].id == id)
        {
            object = &clone.objects[i];
            break;
        }
    }

    switch (direction)
    {
    case 'd':
        object->y++;
        break;
    case 'l':
        object->x--;
        break;
    case 'r':
        object->x++;
        break;

    case 'o':
    tetromino_rotate(object);
    break;

    default:
        break;
    }

    char matrix[100000];
    memset(matrix, ' ', 100000);

    for (int i = 0; i < clone.object_count; i++)
    {
        struct scene_object o = clone.objects[i];
        for (int h = 0; h < o.height; h++)
        {
            for (int w = 0; w < o.width; w++)
            {
                if(get_pixel(o.texture, w, h, o.width, o.height) == ' ')
                {
                    continue;
                }


                if (get_pixel(matrix, o.x + w, o.y + h, res_x, res_y) == ' ')
                {
                    set_pixel(matrix, o.x + w, o.y + h, get_pixel(o.texture, w, h, o.width, o.height), res_x, res_y);
                }
                else
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}
