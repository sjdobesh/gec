//=============//
//             //
//    WIN.H    //
//             //
//==========================================//
// Header for SDL window and openGL context //
//============================================================================80
#ifndef _WIN_H_
#define _WIN_H_

// SDL & OpenGL
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// custom module
#include "module/sprite.h"

// STRUCTS //-------------------------------------------------------------------

// texture parameters to load in an image
typedef struct tex_parameters {
  char* path;      // path to this texture
  int w, h, c;     // width, height, channels
  char* pixel_buf; // buffer for pixel bytes
} tex_parameters;

// window parameters for opengl and sdl
typedef struct win_parameters {
  SDL_Window*     window;
  SDL_GLContext   context;
  GLuint          vao, vbo, ebo, tex;
  GLuint          vert_shader;
  GLuint          frag_shader;
  GLuint          shader_prog;
  char*           vert_path;
  char*           frag_path;
  tex_parameters* t;
  sprite*         s;
  int             w, h;
} win_parameters;


// PROTOTYPES //----------------------------------------------------------------

int             init_win            (win_parameters*);
win_parameters* init_win_parameters (char*, char*, char*, int, int);
SDL_Window*     init_sdl            (win_parameters*);
SDL_GLContext   init_context        (win_parameters*);
int             win_render          (win_parameters*);
int             win_clean           (win_parameters*);
int             init_win_shaders    (win_parameters*);
int             init_win_geometry   (win_parameters*);
int             init_win_textures   (win_parameters*);
int             update_win_geometry (win_parameters*);
int             update_win_textures (win_parameters*);
char*           load_shader_code    (char*);
int             load_image          (tex_parameters*);

#endif
