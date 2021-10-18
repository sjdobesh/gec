#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#define uint unsigned int

enum component_type {
  SPRITE,
  RIGIDBODY,
  DRAWABLE,
};

typedef struct component {
  void* ptr;
  uint type;
  uint id;
} component;

typedef struct component_collection {
  component* c;
  uint n;
} component_collection;

#endif
