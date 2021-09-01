//==============//
//              //
//    MAIN.C    //
//              //
//=====================//
// Photon Game Engine. //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// std
#include <stdio.h>
#include <stdint.h>

// custom module
#include "module/win.h"
#include "module/phys.h"
#include "module/controller.h"
#include "module/main.h"

// MAIN //====================================================================80
int main(int argc, char* argv[]) {

  // controller... maybe this should go in win_parameters? can't due to recursive dependencies
  controller ctrl;

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
    control_sprite(p, &ctrl, &loop); // SDL event handling
    win_render(p);                   // render changes
  }

  // EXIT //--------------------------------------------------------------------
  printf("Exiting...\n");
  win_clean(p);
  exit(EXIT_SUCCESS);

}
