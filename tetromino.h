#ifndef TETROMINO
#define TETROMINO
#include "scene.h"

struct scene_object tetromino_create(int id, char name);

void tetromino_rotate(struct scene_object *tetromino);

#endif