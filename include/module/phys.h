//==============//
//              //
//    PHYS.H    //
//              //
//=================================//
// Newtonian physics module header //
//============================================================================80
#ifndef _PHYS_H_
#define _PHYS_H_

// STRUCTS //-------------------------------------------------------------------

// 2d vector
typedef struct vec2 {
  float x, y;
} vec2;

// 3d vector
typedef struct vec3 {
  float x, y, z;
} vec3;


// rigid body
typedef struct rigid_body {
  vec2 *pos, *acc, *vel;
  float g, drag, max_speed, elasticity;
} rigid_body;

// PROTOTYPES //----------------------------------------------------------------

// VECTOR //
vec2* vadd(vec2* a, vec2* b);
vec2* vmul(vec2* a, float b);

// RIGIDBODY //
rigid_body* create_rb      ();
int         free_rb        (rigid_body*);
int         apply_force    (rigid_body* rb, vec2* force);
int         update_physics (rigid_body* rb);

#endif
