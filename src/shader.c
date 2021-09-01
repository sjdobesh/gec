//================//
//                //
//    SHADER.C    //
//                //
//=================================================================//
// Shader component, compile shaders from paths into a program ptr //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// custom
#include "module/shader.h"


// INIT & FREE //---------------------------------------------------------------

//--------------------------------
// allocate shaders and set names
//------------------------------------------------------------------------------
shaders* init_shaders(char** names, int n) {
  shaders* s = malloc(sizeof(shaders));
  s->ptrs    = malloc(sizeof(unsigned int) * n);
  s->names   = names;
  s->n       = n;
  return s;
}

//-----------------------
// free shader selection
//------------------------------------------------------------------------------
void free_shaders(shaders* s) {
  free(s->ptrs);
  free(s);
}
//--------------------------------
// loads shader code into a char*
//--------------------------------
// I: path        - char*
// O: code buffer - char*
//------------------------------------------------------------------------------

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

//-------------------------------------------------------
// compile to shader program and return reference to it.
// note that this just creates the program on the gpu,
// this will still need to be use with glUseProgram()
//-------------------------------------------------------
// I: vert_path          - char*
//    frag_path          - char*
// O: shader program ptr - uint
//------------------------------------------------------------------------------
unsigned int compile_shader(char* vert_path, char* frag_path) {
  GLint status;
  unsigned int vert, frag, shader;
  char err_buf[1024]; // I hope this is big enough... :)
  // vertex
  const char* vert_shader_src = load_shader_code(vert_path);
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_shader_src, NULL);
  glCompileShader(vert);
  glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(vert, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(stderr, "ERROR: Vertex shader compilation failed: %s\n", err_buf);
    exit(EXIT_FAILURE);
  }
  // fragment
  const char* frag_shader_src = load_shader_code(frag_path);
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_shader_src, NULL);
  glCompileShader(frag);
  glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(frag, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(stderr, "ERROR: Fragment shader compilation failed: %s\n", err_buf);
    exit(EXIT_FAILURE);
  }
  // link
  shader = glCreateProgram();
  glAttachShader(shader, vert);
  glAttachShader(shader, frag);
  glBindFragDataLocation(shader, 0, "out_Color");
  glLinkProgram(shader);
  return shader;
}

//-------------------------------------
// find a shader ptr out of shader set
//------------------------------------------------------------------------------
int find_shader(shaders s, char* name, unsigned int* shader) {
  for (int i = 0; i < s.n; i++) {
    if (!strcmp(s.names[i], name)) {
      *shader = s.ptrs[i];
      return 0;
    }
  }
  return 1;
}

//---------------------------------------
// compile and add a shader to resources
//------------------------------------------------------------------------------
int add_shader(shaders* s, char* vert, char* frag, char* name) {
  // expand
  s->n++;
  s->names = realloc(s->names, sizeof(char*) * s->n);
  s->ptrs  = realloc(s->ptrs, sizeof(char*) * s->n);
  // compile and append
  s->ptrs[s->n-1] = compile_shader(vert, frag);
  return 0;
}

//---------------------------
// compile a list of shaders
//------------------------------------------------------------------------------
int compile_shaders(shaders* s, char** verts, char** frags, int n) {
  for (int i = 0; i < n; i++) {
    s->ptrs[i]  = compile_shader(verts[i], frags[i]);
  }
  return 0;
}
