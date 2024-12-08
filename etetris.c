#include <stdio.h>
#include "renderer.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "tetromino.h"
#include <string.h>
#include "physics_engine.h"
#include "linked_list.h"
#include "gameplay.h"
#include "setup_stdio.h"

int main(void)
{
    struct scene scene = scene_create();

    printf("Welcome to Etetris Game\n");
    printf("enter nickname: ");
    scanf("%s", scene.nickname);

    setup_stdin();
    struct renderer_parameters parameters;
    parameters.resolution_x = 40;
    parameters.resolution_y = 30;
    parameters.scene = &scene;
    parameters.work = &gameplay_rule;
    scene.res_x = parameters.resolution_x;
    scene.res_y = parameters.resolution_y;
    scene.chamber_width = 20;

    struct scene_object ground = scene_create_object(1000, scene.chamber_width, 1);
    struct scene_object left_wall = scene_create_object(1001, 1, parameters.resolution_y - 1);
    struct scene_object right_wall = scene_create_object(1002, 1, parameters.resolution_y - 1);
    ground.y = parameters.resolution_y - 1;
    right_wall.x = scene.chamber_width - 1;

    memset(ground.texture, '#', ground.width * ground.height);
    memset(left_wall.texture, '#', left_wall.width * left_wall.height);
    memset(right_wall.texture, '#', right_wall.width * right_wall.height);

    scene_add_object(&scene, ground);
    scene_add_object(&scene, left_wall);
    scene_add_object(&scene, right_wall);

    struct scene_object portrait = scene_create_object(1006, 8, 8);
    portrait.x = right_wall.x + 3;
    portrait.y = 1;
    memcpy(portrait.texture, "#########      ##      ##      ##      ##      ##      #########", portrait.width * portrait.height);
    scene_add_object(&scene, portrait);

    struct scene_object pile = scene_create_object(1003, scene.chamber_width - right_wall.width - left_wall.width, parameters.resolution_y - 2);
    pile.x = 1;
    pile.y = 1;
    pile.color = 3;
    scene_add_object(&scene, pile);

    struct scene_object score = scene_create_object(1004, 20, 1);
    score.x = right_wall.x + 3;
    score.y = portrait.y + portrait.height + 1;
    score.is_text = 1;
    scene_add_object(&scene, score);

    struct scene_object nickname = scene_create_object(1008, 20, 1);
    nickname.x = right_wall.x + 3;
    nickname.y = score.y + score.height + 1;
    nickname.is_text = 1;
    scene_add_object(&scene, nickname);

    struct scene_object pause = scene_create_object(1009, 20, 1);
    pause.x = right_wall.x + 3;
    pause.y = score.y + score.height + 3;
    pause.is_text = 1;
    scene_add_object(&scene, pause);

    gameplay_spawn_tetromino(&scene);

    pthread_t t1;
    pthread_create(&t1, NULL, &renderer_start, &parameters);

    pthread_t t2;
    pthread_create(&t2, NULL, &gameplay_input, NULL);

    pthread_join(t1, NULL);

    return 0;
}