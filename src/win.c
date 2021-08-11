//=================//
//                 //
//      WIN.C      //
//                 //
//======================================//
// Handle SDL window and openGL context //
//============================================================================80

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "module/logo.h" // SDL logo byte data
#include "module/win.h"  // win header

//--------------------------
// initialize an SDL window
//--------------------------
// I: width & height - int
// O: sdl window ptr - SDL_Window*
//----------------------------------------------------------------------------80
SDL_Window* init_sdl(int w, int h) {

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
    w, h,
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
//----------------------------------------------------------------------------80
SDL_GLContext init_context(SDL_Window* window) {

  // set gl attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // init rendering context
  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    fprintf(stderr, "ERROR: Context creation failed.\n");
    SDL_DestroyWindow(window);
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
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  return context;

}

//-------------------------------------
// initialize an OpenGL shader program
//-------------------------------------
// I: vao         - Gluint *
//    vert_shader - GLuint *
//    frag_shader - GLuint *
//    shader_prog - GLuint *
// O: exit code   - int
//----------------------------------------------------------------------------80
int init_win_shaders(
  GLuint* vao,
  GLuint* vert_shader,
  GLuint* frag_shader,
  GLuint* shader_prog,
  char*  vert_path,
  char*  frag_path
) {

  GLint status;
  char err_buf[512];

  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);

  // Compile vertex shader
  const char* vert_shader_src = load_shader_code(vert_path);
  *vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(*vert_shader, 1, &vert_shader_src, NULL);
  glCompileShader(*vert_shader);
  glGetShaderiv(*vert_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(*vert_shader, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(stderr, "ERROR: Vertex shader compilation failed: %s\n", err_buf);
    return 1;
  }

  // Compile fragment shader
  const char* frag_shader_src = load_shader_code(frag_path);
  *frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(*frag_shader, 1, &frag_shader_src, NULL);
  glCompileShader(*frag_shader);
  glGetShaderiv(*frag_shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(*frag_shader, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(stderr, "ERROR: Fragment shader compilation failed: %s\n", err_buf);
    return 1;
  }

  // Link vertex and fragment shaders
  *shader_prog = glCreateProgram();
  glAttachShader(*shader_prog, *vert_shader);
  glAttachShader(*shader_prog, *frag_shader);
  glBindFragDataLocation(*shader_prog, 0, "out_Color");
  glLinkProgram(*shader_prog);
  glUseProgram(*shader_prog);

  return 0;

}

//------------------------------------
// initialize an OpenGL geometry data
//------------------------------------
// I: vbo         - Gluint*
//    ebo         - GLuint*
//    shader_prog - GLuint*
//    verts       - GLfloat**
//    indices     - GLint*
// O: exit code   - int
//----------------------------------------------------------------------------80
int init_win_geometry(
  GLuint*  vbo,
  GLuint*  ebo,
  GLuint*  shader_prog
) {

  // Screen Quad //-------------------------
  const GLfloat const_verts[4][4] = {
    //  x      y      s      t
    { -1.0, -1.0,  0.0,  1.0 }, // BL
    { -1.0,  1.0,  0.0,  0.0 }, // TL
    {  1.0,  1.0,  1.0,  0.0 }, // TR
    {  1.0, -1.0,  1.0,  1.0 }, // BR
  };
  const GLint const_indicies[] = {
    0, 1, 2, 0, 2, 3
  };

  // vertex buffer
  printf("Vertex Buffer...\n");
  glGenBuffers(1, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(const_verts), const_verts, GL_STATIC_DRAW);

  // element buffer
  printf("Element Buffer...\n");
  glGenBuffers(1, ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(const_indicies), const_indicies, GL_STATIC_DRAW);

  // bind vertex position attribute
  printf("Position Attribute...\n");
  GLint pos_attr_loc = glGetAttribLocation(*shader_prog, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(pos_attr_loc);

  // bind vertex texture coordinate attribute
  printf("Texture Coordinate...\n");
  GLint tex_attr_loc = glGetAttribLocation(*shader_prog, "in_Texcoord");
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
// initialize OpenGL textures
//----------------------------
// I: tex         - Gluint*
//    shader_prog - GLuint*
// O: exit code   - int
//----------------------------------------------------------------------------80
int init_win_textures(GLuint* tex, GLuint* shader_prog) {

  glGenTextures(1, tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glUniform1i(glGetUniformLocation(*shader_prog, "tex"), 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, logo_rgba);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return 0;

}

//------------------
// free OpenGL data
//------------------
// I: shader_prog - Gluint*
//    vert_shader - GLuint*
//    frag_shader - GLuint*
//    vao         - GLuint*
//    vbo         - GLuint*
//    ebo         - GLuint*
//    tex         - GLuint*
//    context     - SDL_GLContext*
//    window      - SDL_Window**
// O: exit code   - int
//----------------------------------------------------------------------------80
int win_clean(
  GLuint* shader_prog, GLuint* vert_shader, GLuint* frag_shader,
  GLuint* vao, GLuint* vbo, GLuint* ebo, GLuint* tex,
  SDL_GLContext* context, SDL_Window** window
) {

  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDetachShader(*shader_prog, *vert_shader);
  glDetachShader(*shader_prog, *frag_shader);
  glDeleteProgram(*shader_prog);
  glDeleteShader(*vert_shader);
  glDeleteShader(*frag_shader);
  glDeleteTextures(1, tex);
  glDeleteBuffers(1, ebo);
  glDeleteBuffers(1, vbo);
  glDeleteVertexArrays(1, vao);
  SDL_GL_DeleteContext(*context);
  SDL_DestroyWindow(*window);
  SDL_Quit();

  return 0;

}

//-----------------------------
// Render openGL in SDL_Window
//-----------------------------
// I: window      - SDL_Window*
// O: exit code   - int
//----------------------------------------------------------------------------80
int win_render(SDL_Window* window) {

  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  SDL_GL_SwapWindow(window);

  return 0;

}


// Shader Util //-------------------------------------------------------------80
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
