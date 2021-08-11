//============================================================================80

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <stdio.h>
#include "module/win.h"
#include "module/main.h"

// MAIN //====================================================================80
int main(int argc, char* argv[]) {

  // INIT //--------------------------------------------------------------------
  printf("Initializing...\n");
  // Window parameters
  win_parameters* p = init_win_parameters(
    VERT_PATH, FRAG_PATH, TEX_PATH,
    WIN_WIDTH, WIN_HEIGHT
  );
  if (init_win(p)) { exit(EXIT_FAILURE); }


  // RUN //---------------------------------------------------------------------
  printf("Running...\n");
  int loop = 1;
  while (loop == 1) {
   SDL_Event event;
   while (SDL_PollEvent(&event)) {
     if (event.type         == SDL_WINDOWEVENT &&
         event.window.event == SDL_WINDOWEVENT_CLOSE) {
       loop = 0;
       break;
     }
   }
   win_render(p);
  }

  // EXIT //--------------------------------------------------------------------
  printf("Exiting...\n");
  win_clean(p);

  return 0;

}
