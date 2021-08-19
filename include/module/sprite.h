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

#endif
