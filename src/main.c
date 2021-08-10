//============================================================================80

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <stdio.h>
#include "module/win.h"
#include "module/main.h"
#include "module/utils.h"

SDL_Window    *window;
SDL_GLContext  context;
GLuint         vao, vbo, ebo, tex;
GLuint         vert_shader;
GLuint         frag_shader;
GLuint         shader_prog;

// INIT //----------------------------------------------------------------------

int init() {
  // init SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL video\n");
    return 1;
  }

  // create window
  window = init_win(WIN_WIDTH, WIN_HEIGHT);

  context = init_context(window);;

  init_shaders(
    vao,
    vert_shader,
    frag_shader,
    shader_prog,
    VERT_PATH,
    FRAG_PATH
  );

  init_geometry(
    vbo,
    ebo,
    shader_prog,
    verts,
    indicies
  );

  init_textures(tex, shader_prog);

  return 0;
}

// int init_shaders() {
//   GLint status;
//   char err_buf[512];
//
//   glGenVertexArrays(1, &vao);
//   glBindVertexArray(vao);
//
//   // Compile vertex shader
//   const char* vert_shader_src = load_shader_code(VERT_PATH);
//   vert_shader = glCreateShader(GL_VERTEX_SHADER);
//   glShaderSource(vert_shader, 1, &vert_shader_src, NULL);
//   glCompileShader(vert_shader);
//   glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
//   if (status != GL_TRUE) {
//     glGetShaderInfoLog(vert_shader, sizeof(err_buf), NULL, err_buf);
//     err_buf[sizeof(err_buf)-1] = '\0';
//     fprintf(stderr, "ERROR: Vertex shader compilation failed: %s\n", err_buf);
//     return 1;
//   }
//
//   // Compile fragment shader
//   const char* frag_shader_src = load_shader_code(FRAG_PATH);
//   frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
//   glShaderSource(frag_shader, 1, &frag_shader_src, NULL);
//   glCompileShader(frag_shader);
//   glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
//   if (status != GL_TRUE) {
//     glGetShaderInfoLog(frag_shader, sizeof(err_buf), NULL, err_buf);
//     err_buf[sizeof(err_buf)-1] = '\0';
//     fprintf(stderr, "ERROR: Fragment shader compilation failed: %s\n", err_buf);
//     return 1;
//   }
//
//   // Link vertex and fragment shaders
//   shader_prog = glCreateProgram();
//   glAttachShader(shader_prog, vert_shader);
//   glAttachShader(shader_prog, frag_shader);
//   glBindFragDataLocation(shader_prog, 0, "out_Color");
//   glLinkProgram(shader_prog);
//   glUseProgram(shader_prog);
//
//   return 0;
//
// }
//
// int init_geometry() {
//
//   // vertex buffer
//   glGenBuffers(1, &vbo);
//   glBindBuffer(GL_ARRAY_BUFFER, vbo);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
//
//   // element buffer
//   glGenBuffers(1, &ebo);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
//
//   // bind vertex position attribute
//   GLint pos_attr_loc = glGetAttribLocation(shader_prog, "in_Position");
//   glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);
//   glEnableVertexAttribArray(pos_attr_loc);
//
//   // bind vertex texture coordinate attribute
//   GLint tex_attr_loc = glGetAttribLocation(shader_prog, "in_Texcoord");
//   glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
//   glEnableVertexAttribArray(tex_attr_loc);
//
//   return 0;
//
// }
//
// int init_textures() {
//
//   glGenTextures(1, &tex);
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, tex);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//   glUniform1i(glGetUniformLocation(shader_prog, "tex"), 0);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, logo_rgba);
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//   return 0;
//
// }
//
// int clean() {
//
//   glUseProgram(0);
//   glDisableVertexAttribArray(0);
//   glDetachShader(shader_prog, vert_shader);
//   glDetachShader(shader_prog, frag_shader);
//   glDeleteProgram(shader_prog);
//   glDeleteShader(vert_shader);
//   glDeleteShader(frag_shader);
//   glDeleteTextures(1, &tex);
//   glDeleteBuffers(1, &ebo);
//   glDeleteBuffers(1, &vbo);
//   glDeleteVertexArrays(1, &vao);
//   SDL_GL_DeleteContext(context);
//   SDL_DestroyWindow(window);
//   SDL_Quit();
//
//   return 0;
//
// }
//
// int render() {
//
//   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
//   glClear(GL_COLOR_BUFFER_BIT);
//   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
//   SDL_GL_SwapWindow(window);
//
//   return 0;
//
// }

// MAIN //====================================================================80
int main(int argc, char* argv[]) {

  printf("Initializing...\n");
  if (init()) { return 1; }

  printf("Running...\n");
  int loop = 1;
  while (loop == 1) {
   SDL_Event event;
   while (SDL_PollEvent(&event)) {
     if (event.type         == SDL_WINDOWEVENT &&
         event.window.event == SDL_WINDOWEVENT_CLOSE) {
       loop = 0;
       break;
     }
   }
   render();
  }

  printf("Exiting...\n");
  clean();

  return 0;

}
