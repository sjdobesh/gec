//====================//
//                    //
//    CONTROLLER.H    //
//                    //
//==========================================//
// Header for SDL window and openGL context //
//============================================================================80
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

// SDL
#include <SDL2/SDL.h>

// custom module
#include "sprite.h"
#include "win.h"

// DEFINITIONS //---------------------------------------------------------------

#define sqrt2over2 0.70710678 // for 45 deg angles

// ENUMS //---------------------------------------------------------------------

// key flag enums
enum key_switches {
  UP    = (1u << 0),
  DOWN  = (1u << 1),
  LEFT  = (1u << 2),
  RIGHT = (1u << 3),
  UL    = UP | LEFT,
  UR    = UP | RIGHT,
  DL    = DOWN | LEFT,
  DR    = DOWN | RIGHT,
};

// PROTOTYPES //----------------------------------------------------------------

uint32_t get_mouse(int* mouse_x, int* mouse_y);
int event_parser     (unsigned int* keys, SDL_Event event, int* loop);
int update_sprite    (sprite* s, unsigned int keys);
int update_sprite_rb (sprite* s, unsigned int keys);
int control_sprite   (win_parameters* p, unsigned int* keys, int* loop);

#endif
