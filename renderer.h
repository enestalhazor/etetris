#ifndef RENDERER
#define RENDERER
#include "scene.h"
#include "linked_list.h"
#include <pthread.h>

struct renderer_parameters
{
    int resolution_x;
    int resolution_y;
    struct scene *scene;

    void (* work)(struct scene *scene, int res_x, int res_y);
};

void *renderer_start(void *parameters);

#endif