#ifndef SCENE
#define SCENE
#include <pthread.h>
#include "renderer.h"

struct scene_object
{
    int id;
    int color;
    int width;
    int height;
    int x;
    int y;
    char texture[1000];
    int rotation;
    char tetromino_type;
    int is_text;
};

struct scene
{
    struct scene_object objects[1000];
    int object_count;
    int score;
    pthread_mutex_t mutex;
    int res_x;
    int res_y;
    int chamber_width;
    char nickname[50];
    int is_paused;
};

struct scene scene_create(void);

void scene_add_object(struct scene *scene, struct scene_object object);

void scene_remove_object(struct scene *scene, int id);

struct scene_object *scene_get_object(struct scene *scene, int id);

struct scene_object scene_create_object(int id, int width, int height);

void scene_print_info(struct scene scene);

void scene_object_print_info(struct scene_object object);

#endif