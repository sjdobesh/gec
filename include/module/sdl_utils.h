// INCLUDES //================================================================80

// sdl
#include <SDL.h>
// c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// SDL Utils //-----------------------------------------------------------------

// create an instance of an sdl window
SDL_Window* create_window(char*, int, int, int);

// create sdl renderer for a window
SDL_Renderer* create_renderer(SDL_Window*);

// screen clearing utility
void clear_screen(SDL_Renderer*);

// create an SDL color with hex code
// 0xRRGGBB
SDL_Color hex2sdl_color(int);
