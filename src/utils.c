//==============================//
//                              //
//       UTILITY FUNCTIONS      //
//                              //
//==============================//============================================80
// Utility functions for SDL, OpenGL, GLEW

// INCLUDES //================================================================80

// sdl
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
// c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// custom
#include "module/utils.h"

// SDL Utils //-----------------------------------------------------------------

// create an instance of an sdl window
SDL_Window* create_window(char* window_title, int w, int h, int winflags) {
  SDL_Window *window;
  window = SDL_CreateWindow(
    "GEC",                   // window title
    SDL_WINDOWPOS_UNDEFINED, // initial x position
    SDL_WINDOWPOS_UNDEFINED, // initial y position
    w,                       // width
    h,                       // height
    winflags                 // bit mask flags
  );
  // Check that the window was successfully created
  if (window == NULL) {
    fprintf(stderr, "Error creating window: %s\nAborting...\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  return window;
}

// OpenGL Utils //--------------------------------------------------------------
// just loads all the text from a file into a single char*
char* load_shader_code(char* path) {

  FILE *f;
  long size;
  char *buf;

  // try to open it
  if ((f = fopen(path, "r")) == NULL) {
    fprintf(stderr, "Error: Couldn't open shader file. \n");
    exit(EXIT_FAILURE);
  }

  // check file size
  fseek(f , 0L , SEEK_END);
  size = ftell(f);
  rewind(f);

  // allocate memory
  buf = calloc(1, size + 1);
  if (!buf) {
    fclose(f);
    fprintf(stderr, "ERROR: Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  // copy to buf
  if(1 != fread(buf , size, 1 , f)) {
    fclose(f);
    free(buf);
    fprintf(stderr, "ERROR: Shader file read failed.\n");
    exit(EXIT_FAILURE);
  }

  // close stream
  fclose(f);
  return buf;
}
