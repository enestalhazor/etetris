#include "tetromino.h"
#include <string.h>

struct scene_object tetromino_create(int id, char name)
{
    struct scene_object tetromino = scene_create_object(id, 4, 4);

    switch (name)
    {
    case 's':
        tetromino.color = 0;
        tetromino.width = 3;
        tetromino.height = 2;
        memcpy(tetromino.texture, " #### ", tetromino.width * tetromino.height);
        break;

    case 'z':
        tetromino.color = 1;
        tetromino.width = 3;
        tetromino.height = 2;
        memcpy(tetromino.texture, "##  ##", tetromino.width * tetromino.height);
        break;

    case 'i':
        tetromino.color = 2;
        tetromino.width = 1;
        tetromino.height = 4;
        memcpy(tetromino.texture, "####", tetromino.width * tetromino.height);
        break;

    case 'j':
        tetromino.color = 3;
        tetromino.width = 2;
        tetromino.height = 3;
        memcpy(tetromino.texture, " # ###", tetromino.width * tetromino.height);
        break;

    case 'l':
        tetromino.color = 4;
        tetromino.width = 2;
        tetromino.height = 3;
        memcpy(tetromino.texture, "# # ##", tetromino.width * tetromino.height);
        break;

    case 'o':
        tetromino.color = 5;
        tetromino.width = 2;
        tetromino.height = 2;
        memcpy(tetromino.texture, "####", tetromino.width * tetromino.height);
        break;

    case 't':
        tetromino.color = 6;
        tetromino.width = 3;
        tetromino.height = 2;
        memcpy(tetromino.texture, "### # ", tetromino.width * tetromino.height);
        break;

    default: 
        break;
    }

    return tetromino;
}