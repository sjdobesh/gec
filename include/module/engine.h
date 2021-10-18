#ifndef _ENGINE_H_
#define _ENGINE_H_

#define uint unsigned int

#include "win.h"
#include "gameobj.h"
#include "databuf.h"

typedef struct engine_data {
  win_parameters win_p;
  uint game_state;
  gameobj_collection entities;
  gameobj debug;
  databuf dbuf;
} engine_data;

#endif
