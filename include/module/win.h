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

// window parameters for opengl and sdl
typedef struct win_parameters {
  SDL_Window*   window;
  SDL_GLContext context;
  GLuint        vao, vbo, ebo, tex;
  GLuint        vert_shader;
  GLuint        frag_shader;
  GLuint        shader_prog;
  char*         vert_path;
  char*         frag_path;
  int           w, h;
} win_parameters;

// function prototypes
win_parameters* init_win_parameters (char*, char*, int, int);
SDL_Window*     init_sdl            (win_parameters*);
SDL_GLContext   init_context        (win_parameters*);
int             win_render          (win_parameters*);
int             win_clean           (win_parameters*);
int             init_win_shaders    (win_parameters*);
int             init_win_geometry   (win_parameters*);
int             init_win_textures   (win_parameters*);
char*           load_shader_code    (char* path);

#endif
