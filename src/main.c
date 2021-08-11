//============================================================================80

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <stdio.h>
#include "module/win.h"
#include "module/main.h"

SDL_Window*   window;
SDL_GLContext context;
GLuint        vao, vbo, ebo, tex;
GLuint        vert_shader;
GLuint        frag_shader;
GLuint        shader_prog;

// INIT //----------------------------------------------------------------------

int init_win() {

  // create window & context
  window = init_sdl(WIN_WIDTH, WIN_HEIGHT);

  context = init_context(window);;

  // init_shaders();
  init_win_shaders(
    &vao,
    &vert_shader,
    &frag_shader,
    &shader_prog,
    VERT_PATH,
    FRAG_PATH
  );

  // init_geometry();
  init_win_geometry(
    &vbo,
    &ebo,
    &shader_prog
  );

  // init_textures();
  init_win_textures(&tex, &shader_prog);

  return 0;

}


// MAIN //====================================================================80
int main(int argc, char* argv[]) {

  printf("Initializing...\n");
  if (init_win()) { return 1; }

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
   win_render(window);
  }

  printf("Exiting...\n");

  win_clean(
    &shader_prog, &vert_shader, &frag_shader,
    &vao, &vbo, &ebo, &tex,
    &context, &window
  );
  // clean();

  return 0;

}
