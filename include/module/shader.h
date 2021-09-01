//================//
//                //
//    SHADER.H    //
//                //
//=========================================================//
// Shader header, just prototypes of compilation functions //
//============================================================================80
#ifndef _SHADER_H_
#define _SHADER_H_

typedef struct shaders {
  unsigned int* ptrs;
  char** names;
  int n;
} shaders;

// prototypes //
char*        load_shader_code (char* path);
unsigned int compile_shader   (char* vert_path, char* frag_path);
shaders*     init_shaders     (char** names, int n);
void         free_shaders     (shaders* s);
int          find_shader      (shaders s, char* name, unsigned int* shader);
int          add_shader       (shaders* s, char* vert, char* frag, char* name);
int          compile_shaders  (shaders* s, char** verts, char** frags, int n);

#endif
