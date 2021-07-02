//============================================================================80

// INCLUDES //------------------------------------------------------------------

// sdl & opengl
#include <SDL.h>
#include <GL/gl.h>
// c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// photon
#include "module/sdl_utils.h"

// DEFINES //-------------------------------------------------------------------

// window size
#define HEIGHT 1000
#define WIDTH  1000
// window features
#define WINFLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

// MAIN //====================================================================80

int main(int argc, char* argv[]) {

  // INIT SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
     fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
     exit(EXIT_FAILURE);
  }
  // WINDOW & RENDERER
  SDL_Window*   window   = create_window("Photon", WIDTH, HEIGHT, WINFLAGS);
  SDL_GLContext context  = SDL_GL_CreateContext(window);
  SDL_Renderer* renderer = create_renderer(window);


  // MAIN LOOP
  int loop = 1;
  while (loop) {

    // EVENTS
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch(event.type) {
        // mouse click
        case SDL_MOUSEBUTTONDOWN:
          break;
        // key press
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_w:
              break;
            case SDLK_s:
              break;
            case SDLK_a:
              break;
            case SDLK_d:
              break;
            case SDLK_SPACE:
              break;
            case SDLK_z:
              break;
            case SDLK_ESCAPE:
              loop = 0;
              break;
          }
        break;
      }
    }

    // RENDER
    glViewport(0, 0,WIDTH, HEIGHT);
    glClearColor(1.0, 0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

  }

  // CLEAN UP
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);

}
