//================//
//                //
//    SPRITE.H    //
//                //
//====================//
// Sprite data struct //
//============================================================================80

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "module/phys.h"

// STRUCTS //-------------------------------------------------------------------

// sprite data
typedef struct sprite {
  rect box;       // bounding box
  int rb_flag;    // boolean for if this sprite has physics
  rigid_body* rb; // rigid_body struct
} sprite;

sprite* create_sprite(float x, float y, float w, float h);
int add_rb(sprite* s, float g, float max_speed, float drag, float elasticity);

#endif
