//=================//
//                 //
//    TEXTURE.C    //
//                 //
//======================================//
// texture managing component to handle //
// loading and organizing gl textures.  //
//============================================================================80

// SDL & OpenGL
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// stb image library
#define STB_IMAGE_IMPLEMENTATION
#include <module/stb_image.h>

#include "module/texture.h"

//---------------------------------
// allocate textures and set names
//------------------------------------------------------------------------------
textures* init_textures(char** names, int n) {
  textures* t = malloc(sizeof(textures));
  t->n        = n;
  return t;
}

//------------------------
// free texture selection
//------------------------------------------------------------------------------
void free_textures(textures* t) {
  free(t->t);
  free(t);
}

//-------------------------
// load an image with stbi
//------------------------------------------------------------------------------
char* create_image(char* path, int w, int h, int c) {
  return (char*)stbi_load(path, &w, &h, &c, STBI_rgb_alpha);
}

//----------------------------
// initialize OpenGL textures
//------------------------------------------------------------------------------
texture* create_textures(char** paths, char** names, int* widths, int* heights, int n) {
  texture* textures      = malloc(sizeof(texture) * n);
  unsigned int* tex_ptrs = malloc(sizeof(unsigned int) * n);
  glGenTextures(n, tex_ptrs);
  for (int i = 0; i < n; i++) {
    textures[i].pixel_buf = create_image(paths[i], widths[i], heights[i], 3);
    textures[i].w         = widths[i];
    textures[i].h         = heights[i];
    textures[i].c         = 3; // we're assuming rgb for now
    textures[i].name      = names[i];
    textures[i].gl_ptr    = *(tex_ptrs + (i * sizeof(unsigned int)));
  }
  free(tex_ptrs);
  return textures;
}

//-----------------------
// load just one texture
//------------------------------------------------------------------------------
texture create_texture(char* path, char* name, int width, int height) {
  texture t;
  glGenTextures(1, &(t.gl_ptr));
  t.pixel_buf = create_image(path, width, height, 3);
  t.w         = width;
  t.h         = height;
  t.c         = 3; // we're assuming rgb for now
  t.name      = name;
  return t;
}

//----------------------
// bind a given texture
//------------------------------------------------------------------------------
int bind_texture(texture t, unsigned int shader) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t.gl_ptr);
  glTexImage2D(
    GL_TEXTURE_2D,    // target
    0,                // level
    GL_RGBA,          // internal format
    t.w, t.h,         // width, height
    0,                // border
    GL_RGB,           // format
    GL_UNSIGNED_BYTE, // type
    NULL              // data
  );
  // bind it to the uniform
  glUniform1i(glGetUniformLocation(shader, "tex"), 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexSubImage2D(
    GL_TEXTURE_2D,               // target
    0, 0, 0,                     // level, x&y offset
    t.w, t.h,                    // width, height
    GL_RGBA,                     // format
    GL_UNSIGNED_INT_8_8_8_8_REV, // type
    t.pixel_buf                  // pixels
  );
  // blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return 0;
}

//---------------------------------------
// find a texture ptr out of texture set
//------------------------------------------------------------------------------
int find_texture(textures t, char* name, texture* tex) {
  for (int i = 0; i < t.n; i++) {
    if (!strcmp(t.t[i].name, name)) {
      *tex = t.t[i];
      return 0;
    }
  }
  return 1;
}

//----------------------------------------------
// compile and add a texture to loaded textures
//------------------------------------------------------------------------------
int add_texture(textures* t, char* path, char* name, int w, int h) {
  // expand
  t->n++;
  t->t = realloc(t->t, sizeof(texture) * t->n);
  // load and append
  t->t[t->n-1] = create_texture(path, name, w, h);
  return 0;
}
