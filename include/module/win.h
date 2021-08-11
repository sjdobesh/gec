//=================//
//                 //
//      WIN.H      //
//                 //
//==========================================//
// Header for SDL window and openGL context //
//============================================================================80
#ifndef _WIN_H_
#define _WIN_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// structs
typedef struct win_parameters {
  SDL_Window*   window;
  SDL_GLContext context;
  GLuint        vao, vbo, ebo, tex;
  GLuint        vert_shader;
  GLuint        frag_shader;
  GLuint        shader_prog;
} win_parameters;

// prototypes
SDL_Window* init_sdl(int w, int h);
SDL_GLContext init_context(SDL_Window* window);
int win_render();
int win_clean();
int init_win_shaders(
  GLuint* vao,
  GLuint* vert_shader,
  GLuint* frag_shader,
  GLuint* shader_prog,
  char*  vert_path,
  char*  frag_path
);
int init_win_geometry(
  GLuint*  vbo,
  GLuint*  ebo,
  GLuint*  shader_prog
);
int init_win_textures();
char* load_shader_code(char* path);

#endif
