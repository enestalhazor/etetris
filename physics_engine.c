#include <stdio.h>
#include "physics_engine.h"
#include <string.h>

static char get_pixel(const char *matrix, int x, int y, int resolution_x, int resolution_y)
{
    return matrix[y * resolution_x + x];
}

int physics_is_valid(struct scene_object *object, char direction, const char *matrix, int resolution_x, int resolution_y)
{

    switch (direction)
    {
    case 'd':
        for (int i = 0; i < object->width; i++)
        {
            if (get_pixel(object->texture, i, object->height - 1, object->width, object->height) == '#')
            {
                if (get_pixel(matrix, i + object->x, object->y + object->height, resolution_x, resolution_y) != ' ')
                {
                    return 0;
                }
            }
        }
        
        break;

    default:
        break;
    }

    return 1;
}
