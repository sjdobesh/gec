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

//---------------------------------------
// create a rigid body data struct
//---------------------------------------
// I: void
// O: (r)igid (b)ody - rigid_body*
//------------------------------------------------------------------------------
rigid_body* create_rb() {
  rigid_body* rb = malloc(sizeof(rigid_body));
  rb->pos = malloc(sizeof(vec2));
  rb->acc = malloc(sizeof(vec2));
  rb->vel = malloc(sizeof(vec2));
  rb->pos->x = rb->pos->y = 0.0;
  rb->acc->x = rb->acc->y = 0.0;
  rb->vel->x = rb->vel->y = 0.0;
  // some default constants
  rb->g          = 0.005;
  rb->drag       = 0.91;
  rb->elasticity = 1.0;
  rb->max_speed  = 10.0;
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
