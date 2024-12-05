#include "tetromino.h"
#include <string.h>

const char *t[] = {"    ###  #      ", " #   ##  #      ", " #  ###         ", " #  ##   #      "};
const char *z[] = {"    ##   ##     ", "  #  ##  #      ", "    ##   ##     ", "  #  ##  #      "};
const char *i[] = {"  #   #   #   # ", "    ####        ", "  #   #   #   # ", "    ####        "};
const char *l[] = {" #   #   ##     ", "       # ###    ", " ##   #   #     ", "    ### #       "};
const char *j[] = {"  #   #  ##     ", "    ###   #     ", " ##  #   #      ", "     #   ###    "};
const char *o[] = {"     ##  ##     ", "     ##  ##     ", "     ##  ##     ", "     ##  ##     "};
const char *s[] = {"     ## ##      ", " #   ##   #     ", "     ## ##      ", " #   ##   #     "};

void tetromino_rotate(struct scene_object *tetromino)
{
    if (tetromino->rotation == 3)
    {
        tetromino->rotation = 0;
    }
    else
    {
        tetromino->rotation++;
    }

    switch (tetromino->tetromino_type)
    {
    case 's':
        memcpy(tetromino->texture, s[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    case 'z':
        memcpy(tetromino->texture, z[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    case 'i':
        memcpy(tetromino->texture, i[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    case 'l':
        memcpy(tetromino->texture, l[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    case 'j':
        memcpy(tetromino->texture, j[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    case 'o':
        memcpy(tetromino->texture, o[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    case 't':
        memcpy(tetromino->texture, t[tetromino->rotation], tetromino->width * tetromino->height);
        break;

    default:
        break;
    }
}

struct scene_object tetromino_create(int id, char name)
{
    struct scene_object tetromino = scene_create_object(id, 4, 4);
    tetromino.rotation = 0;
    tetromino.tetromino_type = name;

    switch (name)
    {
    case 's':
        tetromino.color = 1;
        memcpy(tetromino.texture, s[0], tetromino.width * tetromino.height);
        break;

    case 'z':
        tetromino.color = 2;
        memcpy(tetromino.texture, z[0], tetromino.width * tetromino.height);
        break;

    case 'i':
        tetromino.color = 3;
        memcpy(tetromino.texture, i[0], tetromino.width * tetromino.height);
        break;

    case 'j':
        tetromino.color = 4;
        memcpy(tetromino.texture, j[0], tetromino.width * tetromino.height);
        break;

    case 'l':
        tetromino.color = 5;
        memcpy(tetromino.texture, l[0], tetromino.width * tetromino.height);
        break;

    case 'o':
        tetromino.color = 6;
        memcpy(tetromino.texture, o[0], tetromino.width * tetromino.height);
        break;

    case 't':
        tetromino.color = 7;
        memcpy(tetromino.texture, t[0], tetromino.width * tetromino.height);
        break;

    default:
        break;
    }

    return tetromino;
}