//===================//
//                   //
//    RESOURCES.H    //
//                   //
//=========================================//
// resource managing component to handle   //
// loading and organizing shader programs. //
//============================================================================80

#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include "module/shader.h"
#include "module/texture.h"

// DEFINITIONS //---------------------------------------------------------------

// Generic macros for resource collections

#define add_resource(collection, name, ref) _Generic((collection), \
  shaders:  add_shader, \
  textures: add_texture)(collection, name, ref)

#define get_resource(collection, name, ref) _Generic((collection), \
  shaders:  find_shader, \
  textures: find_texture)(collection, name, ref)

// STRUCTS //-------------------------------------------------------------------

typedef struct resources {
  shaders s;
  textures t;
} resources;

// PROTOTYPES //----------------------------------------------------------------

resources* init_resources  (shaders s, textures t);
shaders*   load_shaders    (char** vert_paths, char** frag_paths, char** names, int n);
int        set_shader      (shaders s, char* name);
int        set_texture     (resources r, char* shader_name, char* tex_name);

#endif
