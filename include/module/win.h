#ifndef _WIN_H_
#define _WIN_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

int render();
int clean();
int init_shaders();
int init_geometry();
int init_textures();

SDL_Window* init_win(int w, int h);
SDL_GLContext init_context(SDL_Window* window);

#endif
