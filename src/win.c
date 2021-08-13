//=============//
//             //
//    WIN.C    //
//             //
//======================================//
// Handle SDL window and openGL context //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// stb image library
#define STB_IMAGE_IMPLEMENTATION
#include <module/stb_image.h>

// custom module
#include "module/win.h"
#include "module/sprite.h"
#include "module/controller.h"
#include "module/phys.h"

//---------------------------------------
// initialize a window parameters struct
//---------------------------------------
// I: vert_path - char*
//    char path - char*
//    w, h      - int
// O: parameter - win_parameters*
//------------------------------------------------------------------------------
win_parameters* init_win_parameters(
  char* vert_path, char* frag_path, char* tex_path,
  int w, int h
) {

  win_parameters* p = malloc(sizeof(win_parameters));
  p->vert_path = vert_path;
  p->frag_path = frag_path;
  p->t         = malloc(sizeof(tex_parameters));
  p->t->path   = tex_path;
  p->s         = malloc(sizeof(sprite));
  p->s->x = p->s->y = 0;
  p->s->h = p->s->w = 0.5; // still in normalized coordinates
  // create rigid body
  p->s->rb     = create_rb();
  p->w         = w;
  p->h         = h;

  return p;

}

//---------------------------------------------
// initialize a window with a parameter struct
//---------------------------------------------
// I: p         - win_parameters
// O: exit code - int
//------------------------------------------------------------------------------
int init_win(win_parameters* p) {

  p->window = init_sdl(p);      // window
  p->context = init_context(p); // context
  init_win_shaders(p);          // shaders
  init_win_geometry(p);         // geometry buffers
  init_win_textures(p);         // textures

  return 0;

}

//--------------------------
// initialize an SDL window
//--------------------------
// I: width & height - int
// O: sdl window ptr - SDL_Window*
//------------------------------------------------------------------------------
SDL_Window* init_sdl(win_parameters* p) {

  // init SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "ERROR: Failed to initialize SDL video.\n");
    exit(EXIT_FAILURE);
  }

  // create window
  SDL_Window* window = SDL_CreateWindow(
    "Photon",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    p->w, p->h,
    SDL_WINDOW_OPENGL
  );
  if (window == NULL) {
    fprintf(stderr, "ERROR: Failed to create main window.\n");
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

   return window;

}

//---------------------------------------
// initialize an OpenGL context (& GLEW)
//---------------------------------------
// I: sdl window ptr - SDL_Window*
// O: sdl glcontext  - SDL_GLContext
//------------------------------------------------------------------------------
SDL_GLContext init_context(win_parameters* p) {

  // set gl attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // init rendering context
  SDL_GLContext context = SDL_GL_CreateContext(p->window);
  if (context == NULL) {
    fprintf(stderr, "ERROR: Context creation failed.\n");
    SDL_DestroyWindow(p->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_GL_SetSwapInterval(1); // vsync

  // GLEW
  glewExperimental = GL_TRUE; // OpenGL 3.+
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "ERROR: GLEW Initialization failed.\n");
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(p->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  return context;

}

//-------------------------------------
// initialize an OpenGL shader program
//-------------------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
int init_win_shaders(win_parameters* p) {

  GLint status;
  char err_buf[512];

  glGenVertexArrays(1, &(p->vao));
  glBindVertexArray(p->vao);

  // Compile vertex shader
  const char* vert_shader_src = load_shader_code(p->vert_path);
  p->vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(p->vert_shader, 1, &vert_shader_src, NULL);
  glCompileShader(p->vert_shader);
  glGetShaderiv(p->vert_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(p->vert_shader, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(stderr, "ERROR: Vertex shader compilation failed: %s\n", err_buf);
    return 1;
  }

  // Compile fragment shader
  const char* frag_shader_src = load_shader_code(p->frag_path);
  p->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(p->frag_shader, 1, &frag_shader_src, NULL);
  glCompileShader(p->frag_shader);
  glGetShaderiv(p->frag_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(p->frag_shader, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(stderr, "ERROR: Fragment shader compilation failed: %s\n", err_buf);
    return 1;
  }

  // Link vertex and fragment shaders
  p->shader_prog = glCreateProgram();
  glAttachShader(p->shader_prog, p->vert_shader);
  glAttachShader(p->shader_prog, p->frag_shader);
  glBindFragDataLocation(p->shader_prog, 0, "out_Color");
  glLinkProgram(p->shader_prog);
  glUseProgram (p->shader_prog);

  return 0;

}

//------------------------------------
// initialize an OpenGL geometry data
//------------------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
int init_win_geometry(win_parameters* p) {

  // Screen Quad //-------------------------
  GLfloat const_verts[4][4] = {

    // //  x      y      s      t
    // { -1.0, -1.0,  0.0,  1.0 }, // BL
    // { -1.0,  1.0,  0.0,  0.0 }, // TL
    // {  1.0,  1.0,  1.0,  0.0 }, // TR
    // {  1.0, -1.0,  1.0,  1.0 }, // BR
    //    location    texture

    { p->s->x - p->s->w, p->s->y - p->s->h,   0.0,   1.0 }, // BL
    { p->s->x - p->s->w, p->s->y + p->s->h,   0.0,   0.0 }, // TL
    { p->s->x + p->s->w, p->s->y + p->s->h,   1.0,   0.0 }, // TR
    { p->s->x + p->s->w, p->s->y - p->s->h,   1.0,   1.0 }, // BR
  };
  // indicies for any simple quad
  GLint const_indicies[] = {
    0, 1, 2, 0, 2, 3
  };

  // vertex buffer
  glGenBuffers(1, &(p->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(const_verts), const_verts, GL_STATIC_DRAW);

  // element buffer
  glGenBuffers(1, &(p->ebo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(const_indicies), const_indicies, GL_STATIC_DRAW);

  // bind vertex position attribute
  GLint pos_attr_loc = glGetAttribLocation(p->shader_prog, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(pos_attr_loc);

  // bind vertex texture coordinate attribute
  GLint tex_attr_loc = glGetAttribLocation(p->shader_prog, "in_Texcoord");
  glVertexAttribPointer(
    tex_attr_loc,
    2,
    GL_FLOAT,
    GL_FALSE,
    4 * sizeof(GLfloat),
    (void*)(2 * sizeof(GLfloat))
  );
  glEnableVertexAttribArray(tex_attr_loc);

  return 0;

}

int update_win_geometry(win_parameters* p) {

  // Screen Quad //-------------------------
  GLfloat const_verts[4][4] = {
    { p->s->x - p->s->w, p->s->y - p->s->h,   0.0,   1.0 }, // BL
    { p->s->x - p->s->w, p->s->y + p->s->h,   0.0,   0.0 }, // TL
    { p->s->x + p->s->w, p->s->y + p->s->h,   1.0,   0.0 }, // TR
    { p->s->x + p->s->w, p->s->y - p->s->h,   1.0,   1.0 }, // BR
  };

  // vertex buffer
  glGenBuffers(1, &(p->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(const_verts), const_verts, GL_STATIC_DRAW);

  // bind vertex position attribute
  GLint pos_attr_loc = glGetAttribLocation(p->shader_prog, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(pos_attr_loc);

  return 0;

}

//----------------------------
// initialize OpenGL textures
//----------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
int init_win_textures(win_parameters* p) {

  // load a texture with texture parameters (t)
  load_image(p->t);

  // make a texture
  glGenTextures(1, &(p->tex));
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, p->tex);
  glTexImage2D(
    GL_TEXTURE_2D,    // target
    0,                // level
    GL_RGBA,          // internal format
    p->t->w, p->t->h, // width, height
    0,                // border
    GL_RGB,           // format
    GL_UNSIGNED_BYTE, // type
    NULL              // data
  );
  // bind it to the uniform
  glUniform1i(glGetUniformLocation(p->shader_prog, "tex"), 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexSubImage2D(
    GL_TEXTURE_2D,               // target
    0, 0, 0,                     // level, x&y offset
    p->t->w, p->t->h,            // width, height
    GL_RGBA,                     // format
    GL_UNSIGNED_INT_8_8_8_8_REV, // type
    p->t->pixel_buf              // pixels
  );
  // blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return 0;

}

//------------------
// free OpenGL data
//------------------
// I: window parameters - win_parameters*
// O: exit code         - int
//------------------------------------------------------------------------------
int win_clean(win_parameters* p) {

  // clean out gl program data
  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDetachShader(p->shader_prog, p->vert_shader);
  glDetachShader(p->shader_prog, p->frag_shader);
  glDeleteProgram(p->shader_prog);
  glDeleteShader(p->vert_shader);
  glDeleteShader(p->frag_shader);
  glDeleteTextures(1, &(p->tex));
  glDeleteBuffers(1, &(p->ebo));
  glDeleteBuffers(1, &(p->vbo));
  glDeleteVertexArrays(1, &(p->vao));
  // sdl items
  SDL_GL_DeleteContext(p->context);
  SDL_DestroyWindow(p->window);
  SDL_Quit();
  // free nested structs
  free(p->t);
  free_rb(p->s->rb);
  free(p->s);
  free(p);

  return 0;

}

//-----------------------------
// Render openGL in SDL_Window
//-----------------------------
// I: window      - SDL_Window*
// O: exit code   - int
//------------------------------------------------------------------------------
int win_render(win_parameters* p) {

  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  SDL_GL_SwapWindow(p->window);

  return 0;

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

//-------------------------
// load an image with stbi
//-------------------------
// I: texture data - tex_parameters*
// O: exit code    - int
//------------------------------------------------------------------------------
int load_image(tex_parameters* p) {

  p->pixel_buf = (char*)stbi_load(
    p->path,
    &(p->w),
    &(p->h),
    &(p->c),
    STBI_rgb_alpha
  );

  return 0;

}
