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

// custom module
#include "module/win.h"
#include "module/resources.h"
#include "module/phys.h"
#include "module/sprite.h"
#include "module/controller.h"

//---------------------------------------
// initialize a window parameters struct
//---------------------------------------
// I: vert_path - char*
//    char path - char*
//    w, h      - int
// O: parameter - win_parameters*
//------------------------------------------------------------------------------
win_parameters* init_win_parameters(
  char* vert_path, char* frag_path, char* tex_path, // paths
  int w, int h                                      // window size
) {
  win_parameters* p = malloc(sizeof(win_parameters));
  // size
  p->w = w;
  p->h = h;
  // shader paths
  p->vert_path = vert_path;
  p->frag_path = frag_path;
  // texture parameters
  p->t       = malloc(sizeof(tex_parameters));
  p->t->path = tex_path;
  // sprite
  p->s = create_sprite((w/2), (h/2), 100, 100);
  // add rigid body
  add_rb(p->s, 0.0, 20.0, 0.95, 1.0);
  return p;
}

//---------------------------------------------
// initialize a window with a parameter struct
//---------------------------------------------
// I: p         - win_parameters
// O: exit code - int
//------------------------------------------------------------------------------
int init_win(win_parameters* p) {
  p->window  = init_sdl(p);     // window
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
  // bind vao
  glGenVertexArrays(1, &(p->vao));
  glBindVertexArray(p->vao);
  p->shader_prog = compile_shader(p->vert_path, p->frag_path);
  glUseProgram(p->shader_prog);
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
  rect proj = square2norm(p->s->box, p->w, p->h);
  GLfloat verts[4][4] = {
    { proj.pos.x, proj.pos.y, 0.0, 0.0 }, // TL
    { proj.dim.x, proj.pos.y, 1.0, 0.0 }, // TR
    { proj.dim.x, proj.dim.y, 1.0, 1.0 }, // BR
    { proj.pos.x, proj.dim.y, 0.0, 1.0 }, // BL
  };
  // indicies for any simple quad
  GLint indicies[] = {
    0, 1, 2, 0, 2, 3
  };
  // BIND BUFFERS //
  // vertex buffer
  glGenBuffers(1, &(p->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  // element buffer
  glGenBuffers(1, &(p->ebo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
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

//----------------------------
// Reset OpenGL geometry data
//----------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
int update_win_geometry(win_parameters* p) {
  // Screen Quad //-------------------------
  rect proj = square2norm(p->s->box, p->w, p->h);
  GLfloat verts[4][4] = {
    { proj.pos.x, proj.pos.y, 0.0, 0.0 }, // TL
    { proj.dim.x, proj.pos.y, 1.0, 0.0 }, // TR
    { proj.dim.x, proj.dim.y, 1.0, 1.0 }, // BR
    { proj.pos.x, proj.dim.y, 0.0, 1.0 }, // BL
  };
  // vertex buffer
  glGenBuffers(1, &(p->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
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
  // load_image(p->t);
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

//------------------------
// update OpenGL textures
//------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
int update_win_textures(win_parameters* p) {
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
  free(p->s->rb);
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


// //-------------------------
// // load an image with stbi
// //-------------------------
// // I: texture data - tex_parameters*
// // O: exit code    - int
// //------------------------------------------------------------------------------
// int load_image(tex_parameters* t) {
//   t->pixel_buf = (char*)stbi_load(
//     t->path,
//     &(t->w),
//     &(t->h),
//     &(t->c),
//     STBI_rgb_alpha
//   );
//   return 0;
// }

//---------------------------------------------------------
// convert from point from pixel to normalized coordinates
//---------------------------------------------------------
// I: point        - vec2
//    (win_w)idth  - float
//    (hin_h)eight - float
// O: exit code    - int
//------------------------------------------------------------------------------
vec2 point2norm(vec2 point, float win_w, float win_h) {
  vec2 projected = {
    ((point.x / win_w) * 2) - 1,
    -1 * (((point.y / win_h) * 2) - 1)
  };
  return projected;
}
//------------------------------------------------------
// project square into normalized coordinates
//------------------------------------------------------
// I: (r)ectangle  - rect*
//    (win_w)idth  - float
//    (hin_h)eigth - float
// O: rectangle    - rect
//------------------------------------------------------------------------------
rect square2norm(rect r, float win_w, float win_h) {
  rect projected = {
    point2norm(r.pos, win_w, win_h),
    point2norm(vadd(r.pos, r.dim), win_w, win_h)
  };
  return projected;
}

