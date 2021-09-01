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

// STRUCTS //-------------------------------------------------------------------

// struct that carries any controller data (keyboard + mouse)
typedef struct controller {
  uint32_t keys;             // key flags
  int      mouse_x, mouse_y; // mouse data
  uint32_t mouse_b;          // mouse button flag
} controller;

// PROTOTYPES //----------------------------------------------------------------

uint32_t get_mouse        (int* mouse_x, int* mouse_y);
int      event_parser     (uint32_t* keys, SDL_Event event, int* loop);
int      update_sprite    (sprite* s, uint32_t keys);
int      update_sprite_rb (sprite* s, uint32_t keys);
int      resolve_physics  (win_parameters* p);
int      control_sprite   (win_parameters* p, controller* ctrl, int* loop);

#endif
