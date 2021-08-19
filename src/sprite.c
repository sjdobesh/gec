//================//
//                //
//    SPRITE.C    //
//                //
//===================//
// Sprite functions. //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// custom module
#include "module/sprite.h"
#include "module/phys.h"

sprite* create_sprite(float x, float y, float w, float h) {
  rect bounding = {{x, y},{w, h}};
  sprite* s = malloc(sizeof(sprite));
  s->box = bounding;
  return s;
}

// add a rigid_body to a sprite
int add_rb(sprite* s, float g, float max_speed, float drag, float elasticity) {
  if (s->rb == NULL) {
    rigid_body rb = {
      s->box,
      {0, 0}, {0, 0},
      g, drag, max_speed, elasticity,
      0, 0,
      {0, 0}
    };
    s->rb_flag = 1;
    s->rb = malloc(sizeof(rigid_body));
    *(s->rb) = rb;
    return 0;
  } else {
    return 1;
  }
}
