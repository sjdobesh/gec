// sdl event parser

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "module/sprite.h"
#include "module/win.h"
#include "module/controller.h"

void sprite_controller(unsigned int* keys, SDL_Event event, int* loop) {
   switch(event.type) {

     // window events
     case SDL_WINDOWEVENT:
       if (event.window.event == SDL_WINDOWEVENT_CLOSE) { *loop = 0; }
       break;

     // mouse clicks
     case SDL_MOUSEBUTTONDOWN:
       break;

     // key press
     case SDL_KEYDOWN:
       switch(event.key.keysym.sym) {
         case SDLK_w: // up
           *keys |= UP;
           break;
         case SDLK_s: // down
           *keys |= DOWN;
           break;
         case SDLK_a: // left
           *keys |= LEFT;
           break;
         case SDLK_d: // right
           *keys |= RIGHT;
           break;
       }
       break;

     // key release
     case SDL_KEYUP:
       switch(event.key.keysym.sym) {
         case SDLK_w: // up
           *keys &= ~UP;
           break;
         case SDLK_s: // down
           *keys &= ~DOWN;
           break;
         case SDLK_a: // left
           *keys &= ~LEFT;
           break;
         case SDLK_d: // right
           *keys &= ~RIGHT;
           break;
       }
       break;
   }
}

void sprite_update(sprite* s, unsigned int keys) {
  #define sqrt2over2 0.70710678 // for 45 deg angles
  float speed = 0.05;
  switch(keys) {
    case UP:
      s->y += speed;
      break;
    case DOWN:
      s->y -= speed;
      break;
    case RIGHT:
      s->x += speed;
      break;
    case LEFT:
      s->x -= speed;
      break;
    case UR:
      s->y += (speed * sqrt2over2);
      s->x += (speed * sqrt2over2);
      break;
    case UL:
      s->y += (speed * sqrt2over2);
      s->x -= (speed * sqrt2over2);
      break;
    case DR:
      s->y -= (speed * sqrt2over2);
      s->x += (speed * sqrt2over2);
      break;
    case DL:
      s->y -= (speed * sqrt2over2);
      s->x -= (speed * sqrt2over2);
      break;
  }
}

void control(win_parameters* p, unsigned int* keys, int* loop) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    sprite_controller(keys, event, loop);
  }

  // make changes with sdl event data
  sprite_update(p->s, *keys);

  update_win_geometry(p);
}
