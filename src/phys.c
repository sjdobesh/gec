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
#include <float.h>

// custom module
#include "module/phys.h"

// UTILITY FUNCTIONS //---------------------------------------------------------

void fswap(float* a, float* b) {
  float temp = *a;
  *a = *b;
  *b = temp;
}

// MATH FUNCTIONS //------------------------------------------------------------

//--------------------
// radians to degrees
//--------------------
// I: (rad)ians - float
// O: degrees   - float
//------------------------------------------------------------------------------
float rad2deg(float rad) {
  return rad * (180.0 / PI);
}

//--------------------
// degrees to radians
//--------------------
// I: (deg)ress - float
// O: radians   - float
//------------------------------------------------------------------------------
float deg2rad(float deg) {
  return deg * (PI / 180.0);
}

//---------------------------------
// get distance between two points
//---------------------------------
// I: vector (a) - vec2
//    vector (b) - vec2
// O: distance   - float
//------------------------------------------------------------------------------
float dist(vec2 a, vec2 b) {
  return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

// VECTOR FUNCTIONS //----------------------------------------------------------

//-----------------
// vector addition
//-----------------
// I: vector (a)   - vec2
//    vector (b)   - vec2
// O: vector (a+b) - float
//------------------------------------------------------------------------------
vec2 vadd(vec2 a, vec2 b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

//--------------------
// vector subtraction
//--------------------
// I: vector (a)   - vec2
//    vector (b)   - vec2
// O: vector (a-b) - float
//------------------------------------------------------------------------------
vec2 vsub(vec2 a, vec2 b) {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

//-----------------
// vector multiply
//-----------------
// I: (v)ector            - vec2
//    (s)calar            - float
// O: scaled vector (v*s) - float
//------------------------------------------------------------------------------
vec2 vmul(vec2 v, float s) {
  v.x = v.x * s;
  v.y = v.y * s;
  return v;
}

//------------------------------
// element wise vector division
//------------------------------
// I: vector a     - vec2
//    vector b     - float
// O: vector (a/b) - float
//------------------------------------------------------------------------------
vec2 vdiv(vec2 a, vec2 b) {
  a.x = a.x / b.x;
  a.y = a.y / b.y;
  return a;
}

//---------------
// get magnitude
//---------------
// I: (v)ector - vec2
// O: magnitude  - float
//------------------------------------------------------------------------------
float vmag(vec2 v) {
  return sqrt(pow(v.x, 2) + pow(v.y, 2)); // pythagorean
}

//--------------
// vector limit
//--------------
// I: (v)ector                      - vec2
//    (lim)it                       - float
// O: vector with magnitude = limit - vec2
//------------------------------------------------------------------------------
vec2 vlim(vec2 v, float lim) {
  v.x = v.x > lim ? lim : (v.x < -lim ? -lim : v.x);
  v.y = v.y > lim ? lim : (v.y < -lim ? -lim : v.y);
  return v;
}

// ALLOCATION & FREEING UTILITIES //--------------------------------------------

//-------------------------------
// allocate and set a vector ptr
//-------------------------------
// I: x       - float
//    y       - float
// O: vec ptr - vec2*
//------------------------------------------------------------------------------
vec2* create_vec2(float x, float y) {
  vec2* v = malloc(sizeof(vec2));
  v->x = x;
  v->y = y;
  return v;
}

//-----------------------------
// allocate and set a line ptr
//-----------------------------
// I: a    - vec2
//    b    - vec2
// O: line - line*
//------------------------------------------------------------------------------
line* create_line(vec2 a, vec2 b) {
  line* l = malloc(sizeof(line));
  l->a = a;
  l->b = a;
  return l;
}

//----------------------------
// allocate and set a ray ptr
//----------------------------
// I: (pos)ition  - vec2
//    (dir)ection - vec2
// O: ray         - ray*
//------------------------------------------------------------------------------
ray* create_ray(vec2 pos, vec2 dir) {
  ray* r = malloc(sizeof(ray));
  r->pos = pos;
  r->dir = dir;
  return r;
}

//---------------------------
// allocate a rigid body ptr
//---------------------------
// I: void
// O: rb ptr - rigid_body*
//------------------------------------------------------------------------------
// TODO parametize default arguments
rigid_body* create_rb() {
  rigid_body* rb = malloc(sizeof(rigid_body));
  // some default constants
  rb->g          = 1; // gravity
  rb->drag       = 0.91;
  rb->elasticity = 1.0;
  rb->max_speed  = 100.0;
  return rb;
}
// GEOMETRY FUNCTIONS //--------------------------------------------------------

//---------------------------
// point intersect rectangle
//---------------------------
// I: (p)oint     - vec2
//    (r)ectangle - rect
// O: intersect   - bool
//------------------------------------------------------------------------------
int pointxrect(vec2 p, rect r) {
  return (p.x >= r.pos.x && p.y >= r.pos.y) &&
         (p.x <= (r.pos.x + r.dim.x) && p.y <= (r.pos.y+r.dim.y));
}

//-------------------------------
// rectangle intersect rectangle
//-------------------------------
// I: (r)ectangle 1 - rect
//    (r)ectangle 2 - rect
// O: intersect     - bool
//------------------------------------------------------------------------------
int rectxrect(rect r1, rect r2) {
  return (r1.pos.x < r2.pos.x + r2.dim.x) && // left
         (r1.pos.x + r1.dim.x > r2.pos.x) && // right
         (r1.pos.y < r2.pos.y + r2.dim.y) && // top
         (r1.pos.y + r1.dim.y > r2.pos.y);   // bottom
}

//-------------------------
// ray intersect rectangle
//-------------------------
// I: (r)ay              - rect
//    (target) rectangle - rect
// O: intersect          - bool
//------------------------------------------------------------------------------
int rayxrect(ray r, rect target,
             vec2* contact_point, vec2* contact_normal, float* hit_near) {
  // near and far intersects
  vec2 t_near = vdiv(vsub(target.pos, r.pos), r.dir);
  vec2 t_far  = vdiv(vsub(vadd(target.pos, target.dim), r.pos), r.dir);
  // swap if out of order
  if (t_near.x < t_far.x) fswap(&t_near.x, &t_far.x);
  if (t_near.y < t_far.y) fswap(&t_near.y, &t_far.y);
  // check for miss
  if (t_near.x > t_far.y || t_near.y > t_far.y) return 0;
  // find far and near values
  *hit_near     = fmax(t_near.x, t_near.y); // send this back outside
  float hit_far = fmin(t_far.x,  t_far.y);
  // check for miss
  if (hit_far < 0) return 0;
  // hit! find contact normal
  *contact_point = vadd(r.pos, vmul(r.dir, *hit_near)); // TODO be carefulllllll
  if (t_near.x > t_near.y) {
    if (r.dir.x < 0) {
      contact_normal = create_vec2(1, 0);
    } else {
      contact_normal = create_vec2(-1, 0);
    }
  } else if (t_near.x < t_near.y) {
    if (r.dir.y < 0) {
      contact_normal = create_vec2(0, 1);
    } else {
      contact_normal = create_vec2(0, -1);
    }
  }
  return 1;
}

//------------------------------------------------------------
// convert a ray into a line representing it's origin and tip
//------------------------------------------------------------
// I: (r)ay - ray
// O: line  - line
//------------------------------------------------------------------------------
line ray_to_line(ray r) {
  vec2* tip = create_vec2(r.pos.x + r.dir.x, r.pos.y + r.dir.y);
  line* l = create_line(r.pos, *tip);
  free(tip);
  return *l;
}

//------------------------------------------------------------------
// given two points defining a line, find the function for the line
//------------------------------------------------------------------
// I: (l)ine - line
//    a      - float*
//    b      - float*
//    c      - float*
// O: exit code - int
//    a, b, & c returned via ptr
//------------------------------------------------------------------------------
int get_line_fn(line l, float* a, float* b, float* c) {
  *a = l.b.y - l.a.y;
  *b = l.a.x - l.b.x;
  *c = (*a * l.a.x) + (*b * l.a.y);
  return 0;
}

//----------------------------------
// determine if two lines intersect
//----------------------------------
// I: line (a)       - line
//    line (b)       - line
//    intersect (x)  - vec2**
// O: intersect bool - int
//    returns point by ptr
//------------------------------------------------------------------------------
int line_intersect(line a, line b, vec2** x) {
  float a_a, a_b, a_c, b_a, b_b, b_c;
  get_line_fn(a, &a_a, &a_b, &a_c);
  get_line_fn(b, &b_a, &b_b, &b_c);
  float det = (a_a * b_b) - (a_b * b_a);
  if (det == 0) {
    return 0; // lines are parallel
  } else {
    // intersect lines (not segments)
    vec2* intersect = create_vec2(
      ((b_b * a_c)-(a_b * b_c)) / det,
      ((a_a * b_c)-(b_a * a_c)) / det
    );
    // check if point is within both line segments range
    if (fminf(a.a.x, a.b.x) <= intersect->x && // line 1
        fmaxf(a.a.x, a.b.x) >= intersect->x &&
        fminf(a.a.y, a.b.y) <= intersect->y &&
        fmaxf(a.a.y, a.b.y) >= intersect->y &&
        fminf(b.a.x, b.b.x) <= intersect->x && // line 2
        fmaxf(b.a.x, b.b.x) >= intersect->x &&
        fminf(b.a.y, b.b.y) <= intersect->y &&
        fmaxf(b.a.y, b.b.y) >= intersect->y
    ) {
      *x = intersect;
      return 1;
    } else {
      return 0;
    }
  }
}

// RIGIDBODY PHYSICS FUNCTIONS //-----------------------------------------------

//---------------------------------------------
// apply a force vector to a rigid body struct
//---------------------------------------------
// I: (r)igid (b)ody - rigid_body*
// O: exit code      - int
//------------------------------------------------------------------------------
int apply_force(rigid_body* rb, vec2* force) {
  rb->acc = vadd(rb->acc, *force);
  return 0;
}

//------------------------------------
// update a rigidbodies physics
//------------------------------------
// I: (r)igid (b)ody - rigid_body*
// O: exit code      - int
//------------------------------------------------------------------------------
int update_physics(rigid_body* rb) {
  vec2 grav = { 0, rb->g };                 // apply gravity
  apply_force(rb, &grav);
  rb->vel = vadd(rb->vel, rb->acc);         // update velocity
  rb->acc = vmul(rb->acc, 0);               // zero acceleration
  rb->vel = vmul(rb->vel, rb->drag);        // drag coeffecient
  rb->vel = vlim(rb->vel, rb->max_speed);   // limit to max speed
  rb->box.pos = vadd(rb->box.pos, rb->vel); // move rigid_body
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
  corner_list[0].x = rb->box.pos.x - rb->box.dim.x; // tl
  corner_list[0].y = rb->box.pos.y + rb->box.dim.y;
  corner_list[1].x = rb->box.pos.x + rb->box.dim.x; // tr
  corner_list[1].y = rb->box.pos.y + rb->box.dim.y;
  corner_list[2].x = rb->box.pos.x - rb->box.dim.x; // bl
  corner_list[2].y = rb->box.pos.y - rb->box.dim.y;
  corner_list[3].x = rb->box.pos.x + rb->box.dim.x; // br
  corner_list[3].y = rb->box.pos.y - rb->box.dim.y;
  return corner_list;
}

//--------------------------
// get a rigid body's sides
//--------------------------
// I: corners       - vec2* // always of size 4
// O: list of sides - line* // note, always of size 4
//------------------------------------------------------------------------------
line* get_sides(vec2* corners) {
  line* line_list = malloc(sizeof(line) * 4);
  // tl tr bl br                                   // side order
  line_list = create_line(corners[0], corners[1]); // t, r, b, l
  line* l = line_list + sizeof(line);              //     1
  l = create_line(corners[1], corners[3]);         //   4 X 2
  l += sizeof(line);                               //     3
  l = create_line(corners[3], corners[2]);
  l += sizeof(line);
  l = create_line(corners[2], corners[0]);
  return line_list;
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
  free(a_corners);
  free(b_corners);
  return 0;
}

//-------------------------------------------------
// collide a against b and set a's collision flags
//-------------------------------------------------
// I: rigid body (a) - rigid_body*
//    rigid body (b) - rigid_body*
// O: exit code             - int (bool)
//------------------------------------------------------------------------------
int collide(rigid_body* a, rigid_body* b) {
  // collision
  if (rectxrect(a->box, b->box)) {
    if (a->overlap == 1) {
      a->colliding = 0;
    } else {
      a->colliding = 1;
      a->overlap   = 1;
    }
  // no collision
  } else {
    if (a->overlap == 0) {
      a->colliding = 0;
    } else {
      a->colliding = 1;
      a->overlap   = 0;
    }
  }
  return 0;
}

//----------------------------------------------------------------------------
// given a rigid body and its neighbors, detect overlap for the first element
//----------------------------------------------------------------------------
// I: (r)igid (b)ody (l)ist - rigid_body*
//    (n)umber              - int
// O: exit code             - int (bool)
//------------------------------------------------------------------------------
int collide_player(rigid_body** rbl, int n) {
  for (int i = 1; i < n; i++) {
    // collision
    collide(rbl[0], rbl[i]);
  }
  return 0;
}

//--------------------------------------------------------------------
// given a set of rigid bodies, detect collisions between all of them
//--------------------------------------------------------------------
// I: (r)igid (b)ody (l)ist    - rigid_body*
//    (n)umber of rigid bodies - int
// O: exit code                - int
//------------------------------------------------------------------------------
int collide_collection(rigid_body** rbl, int n) {
  // check each body against one another once
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) { // this shaves some time but still O(n^2)
      // collision
      collide(rbl[i], rbl[j]);
    }
  }
  return 0;
}

//------------------------------------------------------------------
// given a list of lines and a ray, locate the nearest intersection
//------------------------------------------------------------------
// I: (r)ay    - ray
//    sides    - line*
// O: side idx - int
//------------------------------------------------------------------------------
int nearest_intersect(ray r, line* sides) {
  vec2* intersection;
  line* side = sides;
  float nearest_intersect_dist = FLT_MAX;
  int nearest_side             = -1;
  for (int i = 0; i < 4; i++) {
    if (line_intersect(ray_to_line(r), *side, &intersection)) {
      float d = dist(*intersection, r.pos);
      if (d < nearest_intersect_dist) {
        nearest_intersect_dist = d;
        nearest_side           = i;
      }
    }
    // inc ptr
    side += sizeof(line);
  }
  return nearest_side;
}

//-------------------------------------------------
// resolve a collision between rigid body a and b
// TODO resolve this after addressing window needs
//-------------------------------------------------
// I: rigid body (a) - rigid_body*
//    rigid body (b) - rigid_body*
// O: exit code      - int
//------------------------------------------------------------------------------
int resolve_collision(rigid_body* a, rigid_body* b) {
  // get ray of possible collision intersection (vel + width/2 vector + pos)
  vec2* v = create_vec2(a->vel.x * (1 + a->box.dim.x), a->vel.y * (1 + a->box.dim.y));
  ray* r  = create_ray(a->box.pos, *v);
  // intersect v with all sides of square b keeping closest intersection
  vec2* corners    = get_corners(b);
  line* sides      = get_sides(corners);
  int nearest_side = nearest_intersect(*r, sides);
  // correct based on wall normal
  if (nearest_side % 2) { // right and left
    a->vel.x *= -1;
  } else {                // up and down
    a->vel.y *= -1;
  }
  // cleanup
  free(corners);
  free(r);
  free(v);
  return 0;
}
