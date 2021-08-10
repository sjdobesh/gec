//============================================================================80
#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// definitions

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

#define VERT_PATH "../shaders/test.vert"
#define FRAG_PATH "../shaders/test.frag"

// prototypes

int init();
int render();
int clean();
int init_shaders();
int init_geometry();
int init_textures();

// Test quad //-------------------------
const GLfloat verts[6][4] = {
  //  x      y      s      t
  { -1.0f, -1.0f,  0.0f,  1.0f }, // BL
  { -1.0f,  1.0f,  0.0f,  0.0f }, // TL
  {  1.0f,  1.0f,  1.0f,  0.0f }, // TR
  {  1.0f, -1.0f,  1.0f,  1.0f }, // BR
};
const GLint indicies[] = {
    0, 1, 2, 0, 2, 3
};

#endif

