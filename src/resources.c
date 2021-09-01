//===================//
//                   //
//    RESOURCES.C    //
//                   //
//=========================================//
// resource managing component to handle   //
// loading and organizing shader programs. //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// std
#include <stdlib.h>

// custom
#include "module/texture.h"
#include "module/shader.h"
#include "module/resources.h"

// SHADER MANAGEMENT //---------------------------------------------------------

//----------------------
// use a shader by name
//------------------------------------------------------------------------------
int set_shader(shaders s, char* name) {
  unsigned int shader;
  if (!get_resource(s, name, &shader)) {
    glUseProgram(shader);
    return 0;
  } else {
    return 1; // ERROR
  }
}

//-----------------------
// load shader resources
//------------------------------------------------------------------------------
shaders* load_shaders(char** v_paths, char** f_paths, char** names, int n) {
  shaders* s = init_shaders(names, n);
  compile_shaders(s, v_paths, f_paths, n);
  return s;
}

// TEXTURE MANAGEMENT //--------------------------------------------------------

//-----------------------
// use a texture by name
//------------------------------------------------------------------------------
int set_texture(resources r, char* shader_name, char* tex_name) {
  unsigned int shader;
  texture t;
  if (!get_resource(r.s, shader_name, &shader)) {
    if (!get_resource(r.t, tex_name, &t)) {
      bind_texture(t, shader);
      return 0;
    }
  }
  return 1;
}

//------------------------
// load texture resources
//------------------------------------------------------------------------------
textures* load_textures(
  char** tex_paths, char** names,
  int* widths, int* heights,
  int n
) {
  textures* t = init_textures(names, n);
  t->t = create_textures(tex_paths, names, widths, heights, n);
  return t;
}

// RESOURCE MANAGEMENT //-------------------------------------------------------

//--------------------------------------------------
// allocate resources and populate with collections
//------------------------------------------------------------------------------
resources* init_resources(shaders s, textures t) {
  resources* r = malloc(sizeof(resources));
  r->s = s;
  r->t = t;
  return r;
}


//-------------------------------------------------------------
// MAIN PUBLIC INTERFACE- load and populate a resources struct
//------------------------------------------------------------------------------
resources* load_resources (
  char** vert_paths, char** frag_paths, char** tex_paths,
  char** s_names, char** t_names,
  int* widths, int* heights,
  int num_shader, int num_tex
) {
  shaders* s   = load_shaders(vert_paths, frag_paths, s_names, num_shader);
  textures* t  = load_textures(tex_paths, t_names, widths, heights, num_tex);
  resources* r = init_resources(*s, *t);
  return r;
}

// set resource struct
resources* set_resources() {
  // shader
  char** vert_paths = malloc(sizeof(char*));
  char** frag_paths = malloc(sizeof(char*));
  vert_paths[0]     = "../shaders/test.vert";
  frag_paths[0]     = "../shaders/test.frag";
  char** s_names    = malloc(sizeof(char*));
  s_names[0]        = "default";
  // texture
  char** tex_paths  = malloc(sizeof(char*));
  tex_paths[0]      = "../textures/texture.jpg";
  int* widths       = malloc(sizeof(int));
  int* heights      = malloc(sizeof(int));
  char** t_names    = malloc(sizeof(char*));
  t_names[0]        = "default";
  widths[0] = heights[0] = 100;
  // count
  int num_shader, num_tex;
  num_shader = num_tex = 1;
  // resources
  resources* r = load_resources(vert_paths, frag_paths, tex_paths, s_names, t_names, widths, heights, num_shader, num_tex);
  return r;
}
