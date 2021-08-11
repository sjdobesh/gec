// Main Header
// constants and paths defined here (will eventuall be arg parsed)
//============================================================================80
#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "module/win.h"

// window
#define WIN_WIDTH  512
#define WIN_HEIGHT 512

// paths
#define VERT_PATH "../shaders/test.vert"
#define FRAG_PATH "../shaders/test.frag"
#define TEX_PATH  "../textures/texture.jpg"

// prototypes
int init_win(win_parameters* p);

#endif

