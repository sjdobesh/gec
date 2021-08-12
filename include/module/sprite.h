#ifndef _SPRITE_H_
#define _SPRITE_H_

// sprite data
typedef struct sprite {
    float x, y; // origin pos
    float h, w; // height & width
} sprite;

// key flags
enum key_switches {
  UP    = (1u << 0),
  DOWN  = (1u << 1),
  LEFT  = (1u << 2),
  RIGHT = (1u << 3),
  UL    = UP | LEFT,
  UR    = UP | RIGHT,
  DL    = DOWN | LEFT,
  DR    = DOWN | RIGHT,
};

#endif
