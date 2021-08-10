#ifndef _UTILS_
#define _UTILS

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// SDL Utils //-----------------------------------------------------------------

// create an instance of an sdl window
SDL_Window* create_window(char*, int, int, int);

// OpenGL Utils //--------------------------------------------------------------

// load shader code into a buffer for compilation
char* load_shader_code(char*);

#endif
