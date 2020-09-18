//////////////////////////////////////////////
// gec v0.1                                 //
// Author: Samuel Dobesh                    //
// Date: Sept 17th 2020                     //
//////////////////////////////////////////////
// Desc: A basic graphics engine built in C //
// with minimal 3rd party libraries.        //
// Utilizes SDL for window interfacing      //
///////////////////////////////////////////////////////////////////////////////

/////////////
// Imports //
///////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////
// Define Datatypes //
///////////////////////////////////////////////////////////////////////////////

typedef struct Vec3D
{
    double x;
    double y;
    double z;
    double w;
}Vec3D;    

typedef struct Tri
{
    Vec3D p1;
    Vec3D p2;
    Vec3D p3;
}Tri;

typedef struct Mesh
{
    Tri *triPtr;
}Mesh;

/////////////////////////
// Prototype Functions //
///////////////////////////////////////////////////////////////////////////////
Tri makeTri(Vec3D p1, Vec3D p2, Vec3D p3);
Vec3D makeVec(double x, double y, double z, double w);
void drawTri(Tri tri, SDL_Renderer* renderer);
Tri* parseObj(char fileName[]);

//////////
// MAIN //
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                Vec3D p1, p2, p3;
                
                p1.x = 320;
                p1.y = 200;
                
                p2.x = 300;
                p2.y = 240;

                p3.x = 340;
                p3.y = 240;
                
                Tri tri = makeTri(p1, p2, p3);

                drawTri(tri, renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}

///////////////
// Functions //
///////////////////////////////////////////////////////////////////////////////

Vec3D makeVector(double x, double y, double z, double w)
{
    Vec3D returnVec;
    returnVec.x = x;
    returnVec.y = y;
    returnVec.z = z;
    returnVec.w = w;

    return returnVec;
}

Tri makeTri(Vec3D p1, Vec3D p2, Vec3D p3)
{
    Tri returnTri;
    returnTri.p1 = p1;
    returnTri.p2 = p2;
    returnTri.p3 = p3;

    return returnTri;
}

void drawTri(Tri tri, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, tri.p1.x, tri.p1.y, tri.p2.x, tri.p2.y);
    SDL_RenderDrawLine(renderer, tri.p2.x, tri.p2.y, tri.p3.x, tri.p3.y);
    SDL_RenderDrawLine(renderer, tri.p3.x, tri.p3.y, tri.p1.x, tri.p1.y);
    SDL_RenderPresent(renderer);
}

Tri* parseObj(char fileName[])
{
    Tri array[10];
    Tri *ptr;
    ptr = array;
    return ptr;
}
