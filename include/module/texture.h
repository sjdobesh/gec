//=================//
//                 //
//    TEXTURE.H    //
//                 //
//======================================//
// texture managing component to handle //
// loading and organizing gl textures.  //
//============================================================================80

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

// STRUCTS //-------------------------------------------------------------------

// texture parameters to load in an image
typedef struct texture {
  int w, h, c;      // width, height, channels
  char* pixel_buf;  // buffer for pixel bytes
  char* name;
  unsigned int gl_ptr;
} texture;

// collection of textures. basically just a wrapper with a count
typedef struct textures {
  texture* t;
  int n;
} textures;


// PROTOTYPES //----------------------------------------------------------------

char*     create_image    (char* path, int w, int h, int c);
texture   create_texture  (char* path, char* name, int width, int height);
texture*  create_textures (char** paths, char** names,
                           int* widths, int* heights,
                           int n);
int       bind_texture    (texture t, unsigned int shader);
textures* init_textures   (char** names, int n);
int       find_texture    (textures t, char* name, texture* tex);
int       add_texture     (textures* t, char* path, char* name, int w, int h);

#endif
