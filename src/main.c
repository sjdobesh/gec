//============================================================================80

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <stdio.h>
#include "module/win.h"
#include "module/main.h"
#include "module/controller.h"

// MAIN //====================================================================80
int main(int argc, char* argv[]) {

  unsigned int keys = 0;

  // INIT //--------------------------------------------------------------------
  printf("Initializing...\n");
  // Window parameters
  win_parameters* p;
  p = init_win_parameters(VERT_PATH,FRAG_PATH,TEX_PATH,WIN_WIDTH,WIN_HEIGHT);
  if (init_win(p)) { exit(EXIT_FAILURE); }


  // RUN //---------------------------------------------------------------------
  printf("Running...\n");
  int loop = 1;
  while (loop == 1) {

    // // SDL event handling
    control(p, &keys, &loop);

    // render changes
    win_render(p);
  }

  // EXIT //--------------------------------------------------------------------
  printf("Exiting...\n");
  win_clean(p);

  exit(EXIT_SUCCESS);

}
