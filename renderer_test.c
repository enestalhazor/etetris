#include <stdio.h>
#include "renderer.h"
#include <stdlib.h>
#include <pthread.h>

int main(void)
{
  pthread_t t1;

  struct scene scene = scene_create();

  struct scene_object o0 = scene_create_object(0, 20, 10);
  o0.x = 5;
  o0.y = 2;
  scene_add_object(&scene, o0);

  struct scene_object o1 = scene_create_object(1, 30, 10);
  o1.x = 20;
  o1.y = 20;
  scene_add_object(&scene, o1);

  struct scene_object o2 = scene_create_object(4, 20, 19);
  o2.x = 40;
  o2.y = 30;
  scene_add_object(&scene, o2);

  struct renderer_parameters parameters;
  parameters.resolution_x = 150;
  parameters.resolution_y = 40;
  parameters.scene = scene;

  pthread_create(&t1, NULL, &renderer_start, &parameters);

  while (1)
  {
    char c = fgetc(stdin);

    if (c == 'd')
    {
      struct scene_object *o = &parameters.scene.objects[0];
      o->x = o->x + 2;
      o->y = o->y + 2;

      parameters.scene.objects[1].x++;
      parameters.scene.objects[1].y += 2;

      parameters.scene.objects[2].x += 3;
      parameters.scene.objects[2].y += 1;
    }
    else if (c == 'u')
    {
      parameters.scene.objects[0].x = -3;
      parameters.scene.objects[0].y--;

      parameters.scene.objects[1].x--;
      parameters.scene.objects[1].y -= 2;

      parameters.scene.objects[2].x -= 3;
      parameters.scene.objects[2].y -= 1;
    }
  }

  pthread_join(t1, NULL);

  return 0;
}