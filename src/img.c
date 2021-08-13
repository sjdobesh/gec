//=============//
//             //
//    IMG.C    //
//             //
//=============//===========================//
// Image functions to use on loaded sprites //
//============================================================================80

#include "module/img.h"

//-----------------------
// basic filter function
//-----------------------
// I: pixels     - char*
//    (n)umber   - int
//    (c)hannels - int
// O: pixels     - char*
//----------------------------------------------------------------------------80
// parse events, update sprite data, update context geometry
// main exterior function call
char* filter(char* pixels, int n, int c) {

  for (int i = 0; i < n * c; i++) {
    pixels[i] -= 50;
  }

  return pixels;

}

char* unfilter(char* pixels, int n, int c) {

  for (int i = 0; i < n * c; i++) {
    pixels[i] += 50;
  }

  return pixels;

}
