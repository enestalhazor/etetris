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
    char *matrix;
    struct node *queue;
    pthread_mutex_t m;
};

void *renderer_start(void *parameters);





#endif