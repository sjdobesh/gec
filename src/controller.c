//====================//
//                    //
//    CONTROLLER.C    //
//                    //
//=====================================================//
// Handle SDL Events and use them to control a sprite. //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// custom module
#include "module/sprite.h"
#include "module/win.h"
#include "module/controller.h"

//-----------------------------------------
// control a sprite struct with sdl events
//-----------------------------------------
// I: parameters - win_parameters*
//    keys       - unsigned int*
//    loop       - int*
// O: exit code  - int
//----------------------------------------------------------------------------80
// parse events, update sprite data, update context geometry
// main exterior function call
int control_sprite(win_parameters* p, unsigned int* keys, int* loop) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) { event_parser(keys, event, loop); }
  update_sprite(p->s, *keys);
  update_win_geometry(p);
  return 0;
}

//-----------------------------------
// parse an sdl event into key flags
//-----------------------------------
// I: key flags  - unsigned int*
//    event      - SDL_Event
//    loop flag  - int*
// O: exit code  - int
//----------------------------------------------------------------------------80
int event_parser(unsigned int* keys, SDL_Event event, int* loop) {
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
   return 0;
}

//----------------------------------
// apply key flags to sprite struct
//----------------------------------
// I: (s)prite   - sprite*
//    key flags  - unsigned int
// O: exit code  - int
//----------------------------------------------------------------------------80
int update_sprite(sprite* s, unsigned int keys) {
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
  return 0;
}

