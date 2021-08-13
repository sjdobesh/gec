//==============//
//              //
//    PHYS.C    //
//              //
//==========================//
// Newtonian physics module //
//============================================================================80

// std
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// custom module
#include "module/phys.h"

// VECTOR FUNCTIONS //----------------------------------------------------------

// vector add
vec2* vadd(vec2* a, vec2* b) {

  a->x += b->x;
  a->y += b->y;

  return a;

}

// vector multiply
vec2* vmul(vec2* v, float a) {

  v->x = v->x * a;
  v->y = v->y * a;

  return v;

}

// vector limit
vec2* vlimit(vec2* v, float lim) {

  v->x = v->x > lim ? lim : (v->x < -lim ? -lim : v->x);
  v->y = v->y > lim ? lim : (v->y < -lim ? -lim : v->y);

  return v;

}

vec2* create_vec2(float x, float y) {

  vec2* v = malloc(sizeof(vec2));
  v->x = x;
  v->y = y;

  return v;

}

//---------------------------------------
// create a rigid body data struct
//---------------------------------------
// I: void
// O: (r)igid (b)ody - rigid_body*
//------------------------------------------------------------------------------
rigid_body* create_rb() {

  rigid_body* rb = malloc(sizeof(rigid_body));
  rb->pos = create_vec2(0, 0);
  rb->dim = create_vec2(0, 0);
  rb->acc = create_vec2(0, 0);
  rb->vel = create_vec2(0, 0);
  rb->pos->x = rb->pos->y = 0.0;
  rb->acc->x = rb->acc->y = 0.0;
  rb->vel->x = rb->vel->y = 0.0;
  // some default constants
  rb->g          = 0.005;
  rb->drag       = 0.91;
  rb->elasticity = 1.0;
  rb->max_speed  = 10.0;
  rb->max_speed  = 10.0;
  rb->overlap    = 0;
  rb->colliding  = 0;

  return rb;

}

//-----------------------------------------------
// free allocated memory for a rigid_body struct
//-----------------------------------------------
// I: (r)igid (b)ody - rigid_body*
// O: exit code      - int
//------------------------------------------------------------------------------
int free_rb(rigid_body* rb) {

  free(rb->pos);
  free(rb->dim);
  free(rb->acc);
  free(rb->vel);
  free(rb);

  return 0;

}

//------------------------------------
// apply force to a rigid body struct
//------------------------------------
// I: (r)igid (b)ody - rigid_body*
// O: exit code      - int
//------------------------------------------------------------------------------
int apply_force(rigid_body* rb, vec2* force) {

  rb->acc = vadd(rb->acc, force);

  return 0;

}

//------------------------------------
// update a rigidbodies physics
//------------------------------------
// I: (r)igid (b)ody - rigid_body*
// O: exit code      - int
//------------------------------------------------------------------------------
int update_physics(rigid_body* rb) {

  vec2 grav = { 0, -1 * (rb->g) };          // apply gravity
  apply_force(rb, &grav);
  rb->vel = vadd(rb->vel, rb->acc);         // update velocity
  rb->acc = vmul(rb->acc, 0);               // zero acceleration
  rb->vel = vmul(rb->vel, rb->drag);        // drag coeffecient
  rb->vel = vlimit(rb->vel, rb->max_speed); // limit to max speed
  rb->pos = vadd(rb->pos, rb->vel);         // move rigid_body

  return 0;

}

//----------------------------
// get a rigid body's corners
//----------------------------
// I: (r)igid (b)ody  - rigid_body*
// O: list of corners - rigid_body* // note, always of size 4
//------------------------------------------------------------------------------
vec2* get_corners(rigid_body* rb) {

  vec2* corner_list = malloc(sizeof(vec2) * 4);
  // tl
  corner_list[0].x = rb->pos->x - rb->dim->x;
  corner_list[0].y = rb->pos->y + rb->dim->y;
  // tr
  corner_list[1].x = rb->pos->x + rb->dim->x;
  corner_list[1].y = rb->pos->y + rb->dim->y;
  // bl
  corner_list[2].x = rb->pos->x - rb->dim->x;
  corner_list[2].y = rb->pos->y - rb->dim->y;
  // br
  corner_list[3].x = rb->pos->x + rb->dim->x;
  corner_list[3].y = rb->pos->y - rb->dim->y;

  return corner_list;

}

//------------------------------------------
// given a two rigid bodies, detect overlap
//------------------------------------------
// I: rigid body a - rigid_body*
//    rigid body b - rigid_body*
// O: overlap    - int (bool)
//------------------------------------------------------------------------------
int overlap(rigid_body* a, rigid_body* b) {

  // check if each corner of a is in or outside b
  vec2* a_corners = get_corners(a);
  vec2* b_corners = get_corners(b);
  for (int i = 0; i < 4; i++) {
    if (a_corners[i].x > b_corners[0].x && a_corners[i].x < b_corners[1].x &&
        a_corners[i].y > b_corners[2].y && a_corners[i].y < b_corners[0].y) {
      return 1;
    }
  }

  return 0;

}
//------------------------------------------------------------
// given a set of rigid bodies, detect collisions
//------------------------------------------------------------
// I: (r)igid (b)ody (l)ist    - rigid_body*
//    (n)umber of rigid bodies - int
// O: exit code                - int
//------------------------------------------------------------------------------
int collide_collection(rigid_body** rbl, int n) {

  // check each body against one another once
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) { // this shaves some time but still O(n^2)
      printf("i: %d, j: %d\n", i, j);
      // collision
      if (overlap(rbl[i], rbl[j])) {
        if (rbl[i]->overlap == 1) {
          rbl[i]->colliding = 0;
        } else {
          rbl[i]->colliding = 1;
          rbl[i]->overlap   = 1;
        }
      // no collision
      } else {
        if (rbl[i]->overlap == 0) {
          rbl[i]->colliding = 0;
        } else {
          rbl[i]->colliding = 1;
          rbl[i]->overlap   = 0;
        }
      }
    }
  }

  return 0;

}
