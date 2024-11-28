#include <stdio.h>
#include "renderer.h"
#include <stdlib.h>
#include <pthread.h>
#include "tetromino.h"
#include <string.h>
#include "enes_util.h"

int main(void)
{
    struct scene scene = scene_create();
    struct renderer_parameters parameters;
    parameters.resolution_x = 200;
    parameters.resolution_y = 60;
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


    for (int i = 0; i < 1000; i++)
    {
        struct scene_object t;
        int k = get_random_number(0, 7);

        switch (k)
        {
        case 0:
            t = tetromino_create(i, 'i');
            break;
        
        case 1:
            t = tetromino_create(i, 'j');
            break;
            
        case 2:
            t = tetromino_create(i, 'l');
            break;
        
        case 3:
            t = tetromino_create(i, 'o');
            break;
        
        case 4:
            t = tetromino_create(i, 's');
            break;
        
        case 5:
            t = tetromino_create(i, 't');
            break;
        
        case 6:
            t = tetromino_create(i, 'z');
            break;
        
        default:
            break;
        }
        t.x = get_random_number(2, parameters.resolution_x - 5);
        t.y = get_random_number(0, 10);
        scene_add_object(&scene, t);
    }

    pthread_create(&t1, NULL, &renderer_start, &parameters);

    pthread_join(t1, NULL);

    return 0;
}