//==============//
//              //
//    PHYS.H    //
//              //
//=================================//
// Newtonian physics module header //
//============================================================================80
#ifndef _PHYS_H_
#define _PHYS_H_

// DEFINITIONS //---------------------------------------------------------------

#define PI 3.14159265

// STRUCTS //-------------------------------------------------------------------

// 2d vector
typedef struct vec2 {
  float x, y;
} vec2;

// 3d vector
typedef struct vec3 {
  float x, y, z;
} vec3;

// line struct
typedef struct line {
  vec2 a, b;
} line;

// ray struct
typedef struct ray {
  vec2 pos, dir;
} ray;

// rectangle struct
typedef struct rect {
  vec2 pos, dim; // tl & br
} rect;

// rigid body
typedef struct rigid_body {
  rect box;
  vec2 acc, vel;
  float g, drag, max_speed, elasticity;
  int overlap, colliding;
  vec2 contact_normal;
} rigid_body;

// PROTOTYPES //----------------------------------------------------------------

// math functions
float rad2deg(float rad);
float deg2rad(float deg);
float dist(vec2 a, vec2 b);

// VECTOR //
vec2 vadd(vec2 a, vec2 b);
vec2 vsub(vec2 a, vec2 b);
vec2 vmul(vec2 v, float s);
vec2 vdiv(vec2 a, vec2 b);
vec2 vlim(vec2 v, float lim);
float vmag(vec2 v);
vec2* create_vec2(float x, float y);

// lines and rays
line* create_line       (vec2 a, vec2 b);
ray*  create_ray        (vec2 pos, vec2 vec);
line  ray_to_line       (ray r);
int   line_intersect    (line a, line b, vec2** x);
int   nearest_intersect (ray r, line* sides);
int   pointxrect        (vec2 p, rect r);
int   rectxrect         (rect r1, rect r2);
int   rayxrect          (ray r, rect target, vec2* contact_point,
                         vec2* contact_normal, float* hit_near);

// RIGIDBODY //
rigid_body* create_rb          ();
int         apply_force        (rigid_body* rb, vec2* force);
int         update_physics     (rigid_body* rb);
int         collide_collection (rigid_body** rbl, int n);
int         collide_player     (rigid_body** rbl, int n);
int         resolve_collision  (rigid_body* a, rigid_body* b);
int         overlap            (rigid_body* a, rigid_body* b);
int         collide            (rigid_body* a, rigid_body* b);
vec2*       get_corners        (rigid_body* rb);
line*       get_sides          (vec2* corners);

#endif
