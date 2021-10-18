   1   │ //============//
   2   │ //            //
   3   │ //    GE.C    //
   4   │ //            //
   5   │ //==============================//
   6   │ // Main game engine interface   //
   7   │ // aggregating other components //
   8   │ //============================================================================80
   9   │
  10   │ #include <stdint.h>
  11   │
  12   │ #include "module/vec.h"
  13   │ #include "module/resources.h"
  14   │
  15   │ // ENUMS //
  16   │
  17   │ enum component_type {
  18   │   SPRITE     = (1u << 0),
  19   │   RIGID_BODY = (1u << 1),
  20   │   DATA_BUF   = (1u << 2),
  21   │ };
  22   │
  23   │ enum game_state{
  24   │   MENU = (1u << 0),
  25   │   LVL1 = (1u << 1),
  26   │ };
  27   │
  28   │ // STRUCTS //
  29   │
  30   │ typedef struct component {
  31   │   void*    ptr;
  32   │   uint16_t type;
  33   │ } component;
  34   │
  35   │ typedef struct component_collection {
  36   │   component* c;
  37   │   int        n;
  38   │ } component_collection;
  39   │
  40   │ typedef struct game_obj {
  41   │   int  id;
  42   │   vec2 pos;
  43   │   component_collection components;
  44   │ } game_obj;
  45   │
  46   │ typedef struct go_collection {
  47   │   game_obj* obj;
  48   │   int       n;
  49   │ } go_collection;
  50   │
  51   │ typedef struct game{
  52   │   uint16_t      state;
  53   │   go_collection objs;
  54   │   resource_collection resources;
  55   │ } game;
  56   │
  57   │ //--------------------------------
  58   │ // single update for a game state
  59   │ //--------------------------------
  60   │ game update(game g, int* loop) {
  61   │   // loop through game objects
  62   │   // loop through components
  63   │   // for (int i = 0; i < g.objs.n; i++) {
  64   │   //   for (int j = 0; j < g.objs.obj[i].components.n; j++) {
  65   │   //     switch(g.objs.obj[i].components.c[j].type) {
  66   │   //       case SPRITE:
  67   │   //         update_sprite();
  68   │   //         break;
  69   │   //       case RIGID_BODY:
  70   │   //         update_rb();
  71   │   //         break;
  72   │   //     }
  73   │   //   }
  74   │   // }
  75   │   return g;
  76   │ }
  77   │
  78   │ //---------------------------------------
  79   │ // continually update a given game state
  80   │ //---------------------------------------
  81   │ int run(game g) {
  82   │   int loop = 1;
  83   │   while(loop) {
  84   │     g = update(g, &loop);
  85   │   }
  86   │   return 0;
  87   │ }
