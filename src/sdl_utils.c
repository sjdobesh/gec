//==============================//============================================80

// INCLUDES //================================================================80

// sdl
#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
// c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// gec
#include "module/sdl_utils.h"
// SDL Utils //-----------------------------------------------------------------

// create an instance of an sdl window
SDL_Window* create_window(char* window_title, int w, int h, int winflags) {
  SDL_Window *window;
  window = SDL_CreateWindow(
    "GEC",                   // window title
    SDL_WINDOWPOS_UNDEFINED, // initial x position
    SDL_WINDOWPOS_UNDEFINED, // initial y position
    w,                       // width
    h,                       // height
    winflags                    // bit mask flags
  );
  // Check that the window was successfully created
  if (window == NULL) {
    fprintf(stderr, "Error creating window: %s\nAborting...\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  return window;
}

// create sdl renderer for a window
SDL_Renderer* create_renderer(SDL_Window* window) {
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  return renderer;
}

// screen clearing utility
void clear_screen(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
}

// create an SDL color with hex code
// 0xRRGGBB
SDL_Color hex2sdl_color(int hex){
  SDL_Color c = {(hex & 0xff0000) >> 16,
                 (hex & 0x00ff00) >> 8,
                 (hex & 0x0000ff),
                 0xff};
  return c;
}
