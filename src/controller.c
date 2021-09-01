//====================//
//                    //
//    CONTROLLER.C    //
//                    //
//=====================================================//
// Handle SDL Events and use them to control a sprite. //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>


// std
#include <stdint.h>

// custom module
#include "module/phys.h"
#include "module/img.h"
#include "module/sprite.h"
#include "module/win.h"
#include "module/controller.h"

//-----------------------------------------
// control a sprite struct with sdl events
//-----------------------------------------
// I: parameters - win_parameters*
//    keys       - uint32_t*
//    loop       - int*
// O: exit code  - int
//----------------------------------------------------------------------------80
// Details: parse events, update sprite data, update context geometry
// main exterior function call
int control_sprite(win_parameters* p, controller* ctrl, int* loop) {
  // poll sdl events
  SDL_Event event;
  while (SDL_PollEvent(&event)) { event_parser(&ctrl->keys, event, loop); }
  // update sprite with key data
  if (p->s->rb_flag) {
    update_sprite_rb(p->s, ctrl->keys);
    resolve_physics(p);
  } else {
    update_sprite(p->s, ctrl->keys);
  }
  // technically done controlling sprite here
  update_win_geometry(p);
  return 0;
}

int resolve_physics(win_parameters* p) {
  // collection test
  // floor rigid_body
  rigid_body* floor = create_rb();
  floor->box.pos.x = 0;
  floor->box.pos.y = p->h;
  floor->box.dim.x = p->w;
  floor->box.dim.y = 5;
  rigid_body* rbl = malloc(sizeof(rigid_body) * 2);
  // copy rigid bodies to list
  rbl[0] = *(p->s->rb);
  rbl[1] = *floor;

  // update filter with collision
  collide_player(&rbl, 2);
  if (rbl[0].colliding && rbl[0].overlap) {
    p->t->pixel_buf = filter(p->t->pixel_buf, p->t->w * p->t->h, 4);

    // resolve collision
    // resolve_collision(p->s->rb, floor); // TODO <-
    p->s->rb->vel = vmul(p->s->rb->vel, -1);

    // clamp small values if almost still
    if (vmag(p->s->rb->vel) < 0.5) {
      vec2* v = create_vec2(0.0, p->s->rb->g);
      apply_force(p->s->rb, v);
      free(v);
    }
    update_win_textures(p);
  } else if (rbl[0].colliding && !rbl[0].overlap) {
    p->t->pixel_buf = unfilter(p->t->pixel_buf, p->t->w * p->t->h, 4);
    update_win_textures(p);
  } else if (!rbl[0].colliding && rbl[0].overlap) {
    p->s->rb->vel.y = p->s->rb->vel.y * 0.0;
    vec2* v = create_vec2(0.0, p->s->rb->g);
    apply_force(p->s->rb, v);
    free(v);
  }
  // update exterior rigid body
  p->s->rb->colliding = rbl[0].colliding;
  p->s->rb->overlap   = rbl[0].overlap;
  free(rbl);
  return 0;
}

//---------------------------------------
// get mouse coordinates and button data
//---------------------------------------
// I: mouse x coord     - int*
//    mouse y coord     - int*
// O: mouse button mask - uint32_t
//----------------------------------------------------------------------------80
uint32_t get_mouse(int* mouse_x, int* mouse_y) {
  SDL_PumpEvents();
  return SDL_GetMouseState(mouse_x, mouse_y);
}

//-----------------------------------
// parse an sdl event into key flags
//-----------------------------------
// I: key flags  - uint32_t*
//    event      - SDL_Event
//    loop flag  - int*
// O: exit code  - int
//----------------------------------------------------------------------------80
int event_parser(uint32_t* keys, SDL_Event event, int* loop) {
  switch(event.type) {
    // window events
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) { *loop = 0; }
      break;
    // mouse clicks
    case SDL_MOUSEBUTTONDOWN:
      break;
    // key press
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
        case SDLK_ESCAPE: // escape
          *loop = 0;
        case SDLK_w: // up
          *keys |= UP;
          break;
        case SDLK_s: // down
          *keys |= DOWN;
          break;
        case SDLK_a: // left
          *keys |= LEFT;
          break;
        case SDLK_d: // right
          *keys |= RIGHT;
          break;
      }
      break;
    // key release
    case SDL_KEYUP:
      switch(event.key.keysym.sym) {
        case SDLK_w: // up
          *keys &= ~UP;
          break;
        case SDLK_s: // down
          *keys &= ~DOWN;
          break;
        case SDLK_a: // left
          *keys &= ~LEFT;
          break;
        case SDLK_d: // right
          *keys &= ~RIGHT;
          break;
      }
      break;
  }
  return 0;
}

//----------------------------------
// apply key flags to sprite struct
//----------------------------------
// I: (s)prite   - sprite*
//    key flags  - unsigned int
// O: exit code  - int
//----------------------------------------------------------------------------80
int update_sprite_rb(sprite* s, uint32_t keys) {
  float speed = 2.0;
  vec2 v = {0.0, 0.0};
  // apply vector based on keys. Remember, 0,0 is tl, h,w is br
  /* TODO (fix) this doesn't allow for more than 2 keys
   * pressed at a time since we are directly matching
   * enums for the case. */
  switch(keys) {
    case UP:
      v.y = -speed;
      break;
    case DOWN:
      v.y = +speed;
      break;
    case RIGHT:
      v.x = +speed;
      break;
    case LEFT:
      v.x = -speed;
      break;
    case UR:
      v.x = +(speed * sqrt2over2);
      v.y = -(speed * sqrt2over2);
      break;
    case UL:
      v.x = -(speed * sqrt2over2);
      v.y = -(speed * sqrt2over2);
      break;
    case DR:
      v.x = +(speed * sqrt2over2);
      v.y = +(speed * sqrt2over2);
      break;
    case DL:
      v.x = -(speed * sqrt2over2);
      v.y = +(speed * sqrt2over2);
      break;
  }
  s->rb->acc = vadd(s->rb->acc, v);
  update_physics(s->rb);
  // update sprite pos to rigid bodies
  s->box.pos = s->rb->box.pos;
  return 0;
}

//----------------------------------
// apply key flags to sprite struct
//----------------------------------
// I: (s)prite   - sprite*
//    key flags  - unsigned int
// O: exit code  - int
//----------------------------------------------------------------------------80
int update_sprite(sprite* s, uint32_t keys) {
  float speed = 5;
  switch(keys) {
    case UP:
      s->box.pos.y -= speed;
      break;
    case DOWN:
      s->box.pos.y += speed;
      break;
    case RIGHT:
      s->box.pos.x += speed;
      break;
    case LEFT:
      s->box.pos.x -= speed;
      break;
    case UR:
      s->box.pos.y -= (speed * sqrt2over2);
      s->box.pos.x += (speed * sqrt2over2);
      break;
    case UL:
      s->box.pos.y -= (speed * sqrt2over2);
      s->box.pos.x -= (speed * sqrt2over2);
      break;
    case DR:
      s->box.pos.y += (speed * sqrt2over2);
      s->box.pos.x += (speed * sqrt2over2);
      break;
    case DL:
      s->box.pos.y += (speed * sqrt2over2);
      s->box.pos.x -= (speed * sqrt2over2);
      break;
  }
  return 0;
}
