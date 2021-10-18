#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_
#define uint unsigned int
typedef struct drawable {
  void* geo;
  uint geo_type;
  uint color;
} drawable;
#endif
