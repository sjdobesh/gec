#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <SDL2/SDL.h>
#include "sprite.h"
#include "win.h"

// prototypes
void sprite_controller(unsigned int* keys, SDL_Event event, int* loop);
void sprite_update(sprite* s, unsigned int keys);
void control(win_parameters* p, unsigned int* keys, int* loop);

#endif
