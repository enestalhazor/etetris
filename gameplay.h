#ifndef GAMEPLAY
#define GAMEPLAY
#include "linked_list.h"
#include "scene.h"

void *gameplay_input(void *p);

void gameplay_rule(struct scene *scene, int res_x, int res_y);

#endif