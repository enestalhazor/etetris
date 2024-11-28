#include <stdio.h>
#include "renderer.h"
#include <stdlib.h>
#include <pthread.h>
#include "tetromino.h"
#include <string.h>

int main(void)
{
    struct scene scene = scene_create();
    struct renderer_parameters parameters;
    parameters.resolution_x = 80;
    parameters.resolution_y = 40;
    parameters.scene = &scene;

    pthread_t t1;

    struct scene_object ground = scene_create_object(100, parameters.resolution_x, 2);
    struct scene_object left_wall = scene_create_object(100, 2, parameters.resolution_y - 2);
    struct scene_object right_wall = scene_create_object(100, 2, parameters.resolution_y - 2);
    ground.y = parameters.resolution_y - 2;
    right_wall.x = parameters.resolution_x - 2;
    memset(ground.texture, '#', ground.width * ground.height);
    memset(left_wall.texture, '#', left_wall.width * left_wall.height);
    memset(right_wall.texture, '#', right_wall.width * right_wall.height);

    scene_add_object(&scene, ground);
    scene_add_object(&scene, left_wall);
    scene_add_object(&scene, right_wall);

    struct scene_object o0 = tetromino_create(0, 't');
    o0.x = 30;
    o0.y = 2;
    scene_add_object(&scene, o0);

    struct scene_object o1 = tetromino_create(1, 'l');
    o1.x = 40;
    o1.y = 6;
    scene_add_object(&scene, o1);

    struct scene_object o2 = tetromino_create(2, 'j');
    o2.x = 49;
    o2.y = 10;

    struct scene_object o3 = tetromino_create(3, 's');
    o3.x = 50;
    o3.y = 15;
    scene_add_object(&scene, o3);

    scene_add_object(&scene, o2);
    
    struct scene_object o4 = tetromino_create(1, 'i');
    o4.x = 49;
    o4.y = 0;
    scene_add_object(&scene, o4);

    pthread_create(&t1, NULL, &renderer_start, &parameters);

    pthread_join(t1, NULL);

    return 0;
}