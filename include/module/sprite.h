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
  float x, y;     // origin pos
  float h, w;     // height & width
  int rb_flag;    // boolean for if this sprite has physics
  rigid_body* rb; // rigid_body struct
} sprite;

#endif
