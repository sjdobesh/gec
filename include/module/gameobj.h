#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_
#define uint unsigned int
#include "component.h"
typedef struct gameobj {
  uint id;
  component_collection components;
  int active;
} gameobj;

typedef struct gameobj_collection {
  gameobj* objs;
  uint n;
} gameobj_collection;
#endif
