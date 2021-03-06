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
    Vec3D n;
}Tri;

typedef struct Mesh
{
    Tri* triPtr;
}Mesh;

/////////////////////////
// Prototype Functions //
///////////////////////////////////////////////////////////////////////////////
Tri makeTri(Vec3D p1, Vec3D p2, Vec3D p3);
Vec3D makeVec(double x, double y, double z, double w);
void drawTri(Tri tri, SDL_Renderer* renderer);
Tri* objParse(int argc, char* argv[], Tri *triListPtr);

// Matrix Generators
void getProjMatrix(double projMatrix[4][4]);
void getEmptyMatrix(double m[4][4]);
void getIdMatrix(double m[4][4]);
void getTranslationMatrix(double m[4][4], double x, double y, double z);
void getInvertXY(double m[4][4]);

// Rot Matrices
void getXRotMatrix(double m[4][4], double rotTheta);
void getYRotMatrix(double m[4][4], double rotTheta);
void getZRotMatrix(double m[4][4], double rotTheta);

// Matrix Functions
Vec3D multMatVec(double matrix[4][4], Vec3D vecIn);
void multMats(double m1[4][4], double m2[4][4], double mOut[4][4]);
double degToRad(double degrees);

// Vector Functions
Vec3D vAdd(Vec3D v1, Vec3D v2);
Vec3D vSub(Vec3D v1, Vec3D v2);
Vec3D vMult(Vec3D v1, Vec3D v2);
Vec3D vDiv(Vec3D v1, Vec3D v2);
double vDotP(Vec3D v1, Vec3D v2);
double vLength(Vec3D v);
Vec3D vNormalize(Vec3D v);
Vec3D vCrossProd(Vec3D v1, Vec3D v2);
Vec3D translateZ(Vec3D p, double add);
Tri transformTri(double rotTheta, Tri tri);
Vec3D scaleForView(Vec3D v);

//////////
// MAIN //
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    Mesh mesh;
    Tri* triListPtr;
    triListPtr = objParse(argc, argv, triListPtr);
    mesh.triPtr = triListPtr;

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


// draw Tri with three lines
void drawTri(Tri tri, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, tri.p1.x, tri.p1.y, tri.p2.x, tri.p2.y);
    SDL_RenderDrawLine(renderer, tri.p2.x, tri.p2.y, tri.p3.x, tri.p3.y);
    SDL_RenderDrawLine(renderer, tri.p3.x, tri.p3.y, tri.p1.x, tri.p1.y);
    SDL_RenderPresent(renderer);
}

// objParser
Tri* objParse(int argc, char* argv[], Tri *triListPtr){

    // Open file and create line buffer to read in contents
    if (argc == 2)
    {
        printf("File Command : %s\n", argv[1]);
    }
    else
    {
        printf("No File Request.\n");
    }
    


    FILE *fptr;
    int bufferLength = 64;
    char buffer[bufferLength];
    if ((fptr = fopen("./cube.obj","r")) == NULL)
    {
        printf("Error opening obj file");
        exit(1);
    }

    // count lines
    int lines = 0;
    int verts = 0;
    int faces = 0;
    while (fgets(buffer, bufferLength, fptr))
    {
        lines += 1;
        //printf("%s", buffer);
        if (buffer[0] == 'v' && buffer[1] == ' ')
        {
            verts += 1;
        }
        else if (buffer[0] == 'f')
        {
            faces += 1;
        }
    }
    fclose(fptr);
    printf("Lines : %d\n", lines);
    printf("Verts : %d\n", verts);
    printf("Faces : %d\n", faces);

    // Make appropriately sized arrays
    double vertList[verts * 3];
    int faceList[faces * 3];

    // Before reopening file, clear buffer
    memset(buffer, 0, sizeof(buffer));

    // Re open file
    if (argc == 2)
    {
        if ((fptr = fopen(argv[1],"r")) == NULL)
        {
            printf("Error opening obj file");
            exit(1);
        }
    }
    else
    {
        if ((fptr = fopen("cube.obj","r")) == NULL)
        {
            printf("Error opening obj file");
            exit(1);
        }
    }
    int vertIndex = 0;
    int faceIndex = 0;
    const char delim[2] = " ";
    while (fgets(buffer, bufferLength, fptr))
    {
        // split our buffer by space delimiter into tokens
        if (buffer[0] ==  'v' && buffer[1] == ' ')
        {
            char* token = strtok(buffer, delim);
            printf("Dumping first token : %s\n", token);
            // dump first token item
            token = strtok(NULL, delim);
            // while the line isn't empty
            while (token != NULL)
            {
                //printf("%s\n", token);
                char* temp;
                temp = strdup(token);
                printf("%s\n", temp);
                // append items to array

                char* end;
                vertList[vertIndex] = strtod(temp, &end);
                vertIndex += 1;


                // Get next token
                token = strtok(NULL, delim);
            }
        }
        // otherwise add to face list
        if (buffer[0] ==  'f')
        {
            char* token = strtok(buffer, delim);
            // dump first token item
            printf("Dumping first token : %s\n", token);
            token = strtok(NULL, delim);
            // while the line isn't empty
            while (token != NULL)
            {
                printf("%s\n", token);
                
                // Only take the first one and
                // append items to array
                faceList[faceIndex] = atoi(token);
                faceIndex += 1;


                // Progress to next token
                token = strtok(NULL, delim);
            }
        }
    }

    fclose(fptr);

    // FILE PARSED, PRINT CONTENTS

    printf("\nPrinting vertex coordinates\n");
    for (int i = 0; i < vertIndex; i++)
    {
        if (i%3 == 0)
        {
            printf("\n");
        }
        printf("%f\n", vertList[i]);
    }
    printf("\nPrinting face vertices\n");
    for (int i = 0; i < faceIndex; i++)
    {
        if (i%3 == 0)
        {
            printf("\n");
        }
        printf("%i ", faceList[i]);
    }

    printf("\n");

    // NOW PASS LISTS INTO TRI LIST

    // Make a point list
    Vec3D pointList[vertIndex / 3];
    int pointInd = 0;
    for (int i = 0; i < vertIndex; i= i + 3)
    {
        Vec3D p1;
        p1.x = vertList[i];
        p1.y = vertList[i + 1];
        p1.z = vertList[i + 2];

        // w stays 1 for now
        p1.w = 1.0;

        pointList[pointInd] = p1;
        pointInd += 1;
    }
    printf("Generated %ld points.\n", sizeof(pointList));

    // Assemble triangles

    // Make a point list
    printf("%ld\n", sizeof(pointList));
    int triListLength = (int)sizeof(pointList) / 3;
    printf("%d\n", triListLength);
    Tri triList[triListLength];
    int triInd = 0;

    for (int i = 0; i < faceIndex; i = i + 3)
    {
        Tri newTri;
        newTri.p1 = pointList[faceList[i]];
        newTri.p2 = pointList[faceList[i + 1]];
        newTri.p3 = pointList[faceList[i + 2]];
        triList[triInd] = newTri;
        triInd += 1;
    }
    printf("Generated %ld triangles.\n", sizeof(triList)/sizeof(triList[0]));
    triListPtr = triList;

    // return a pointer to our list
    return triListPtr;
}


// Easier declaration and field population
Vec3D makeVec(double x, double y, double z, double w)
{
    Vec3D returnVec;
    returnVec.x = x;
    returnVec.y = y;
    returnVec.z = z;
    returnVec.w = w;

    return returnVec;
}

// Declare and populate in one line
Tri makeTri(Vec3D p1, Vec3D p2, Vec3D p3)
{
    Tri returnTri;
    returnTri.p1 = p1;
    returnTri.p2 = p2;
    returnTri.p3 = p3;

    return returnTri;
}

/////////////////////////////////////
// MATRIX TRANSFORMATION FUNCTIONS //
///////////////////////////////////////////////////////////////////////////////

// multiply a vector by a matrix
// used to project points of tris 
Vec3D multMatVec(double matrix[4][4], Vec3D vecIn)
{
    // Unpack vector
    double x = vecIn.x;
    double y = vecIn.y;
    double z = vecIn.z;
    double w = vecIn.w;

    double outX = ((x * matrix[0][0]) +
                   (y * matrix[1][0]) +
                   (z * matrix[2][0]) +
                   (w * matrix[3][0]));

    double outY = ((x * matrix[0][0]) +
                   (y * matrix[1][0]) +
                   (z * matrix[2][0]) +
                   (w * matrix[3][0]));

    double outZ = ((x * matrix[0][0]) +
                   (y * matrix[1][0]) +
                   (z * matrix[2][0]) +
                   (w * matrix[3][0]));

    double outW = ((x * matrix[0][0]) +
                   (y * matrix[1][0]) +
                   (z * matrix[2][0]) +
                   (w * matrix[3][0]));

    // Make an output vector 
    Vec3D vecOut = makeVec(outX, outY, outZ, outW);
    return vecOut;
}

// multiply two square matrices
void multMats(double m1[4][4], double m2[4][4], double mOut[4][4])
{
    // fill out matrix with 0s
    getEmptyMatrix(mOut);

    // multiply 
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            mOut[row][col] = ((m1[row][0] * m2[0][col]) +
                              (m1[row][1] * m2[1][col]) +
                              (m1[row][2] * m2[2][col]) +
                              (m1[row][3] * m2[3][col]));
        }
    }

    // return void
    return;
}

/////////////////////////////////
// MATRIX GENERATION FUNCTIONS //
///////////////////////////////////////////////////////////////////////////////

// Generate the basic projection matrix
void getProjMatrix(double projMatrix[4][4])
{
    // hard code constants for now
    
    // Calc aspect ratio
    double h = 300.0;
    double w = 300.0;
    double a = h / w;

    // fov calc
    double fovDeg = 90.0;
    double fovRad = degToRad(fovDeg/2);

    // z domain
    double zNear = 0.01;
    double zFar = 1000.0;

    // fov factor
    double f = 1.0 / tan(fovRad/2);

    // calc q
    double q = zFar / (zFar - zNear);

    // make matrix
    // first its empty
    getEmptyMatrix(projMatrix);

    // then edit appropriate matrix indices
    projMatrix[0][0] = (a * f);
    projMatrix[1][1] = f;
    projMatrix[2][2] = q;
    projMatrix[2][3] = 1.0;
    projMatrix[3][2] = -zNear * q;

    // return void
    return;
}

// fill any matrix passed to it with 0s
void getEmptyMatrix(double m[4][4])
{
    // fill with 0s
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = 0.0;
        }
    }

    //return void
    return;
}

// make the identity matrix
void getIdMatrix(double m[4][4])
{
    // empty
    getEmptyMatrix(m);
    
    // add one in the identity row
    m[0][0] = 1.0;
    m[1][1] = 1.0;
    m[2][2] = 1.0;
    m[3][3] = 1.0;

    // finished
    return;
}

// make translation matrix which accepts x, y, and z offsets
void getTranslationMatrix(double m[4][4], double x, double y, double z)
{
    // start with identity matrix
    getIdMatrix(m);

    // add offset
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;

    // finished
    return;
}

void getInvertXY(double m[4][4])
{
    // get identity
    getIdMatrix(m);

    // make x and y into -1
    m[0][0] = -1.0;
    m[1][1] = -1.0;
}

// ROT MATRICES

// Xrot
void getXRotMatrix(double m[4][4], double rotTheta)
{
    // start with identity
    getIdMatrix(m);

    //apply rotation coeffecients
    m[1][1] = cos(rotTheta);
    m[1][2] = sin(rotTheta);
    m[2][1] = -sin(rotTheta);
    m[2][2] = cos(rotTheta);

    return;
}

// Yrot
void getYRotMatrix(double m[4][4], double rotTheta)
{
    // start with identity
    getIdMatrix(m);

    //apply rotation coeffecients
    m[0][0] = cos(rotTheta);
    m[0][2] = sin(rotTheta);
    m[2][0] = -sin(rotTheta);
    m[2][2] = cos(rotTheta);

    return;
}

// Zrot
void getZRotMatrix(double m[4][4], double rotTheta)
{
    // start with identity
    getIdMatrix(m);

    //apply rotation coeffecients
    m[0][0] = cos(rotTheta);
    m[0][1] = sin(rotTheta);
    m[1][0] = -sin(rotTheta);
    m[1][1] = cos(rotTheta);

    return;
}

//void qInvert(double mIn[4][4], double mOut[4][4])

//void matrixPointAt(Vec3D pos, Vec3D target, Vec3D up, double mOut[4][4])

// degrees to radians
double degToRad(double degrees) 
{
    return degrees * (M_PI / 180.0);
}

//////////////////////
// VECTOR FUNCTIONS //
///////////////////////////////////////////////////////////////////////////////

Vec3D vAdd(Vec3D v1, Vec3D v2)
{
    // make new vec and return it
    return makeVec(v1.x + v2.x,
                   v1.y + v2.y,
                   v1.z + v2.z,
                   1.0);
}

Vec3D vSub(Vec3D v1, Vec3D v2)
{
    return makeVec(v1.x - v2.x,
                   v1.y - v2.y,
                   v1.z - v2.z,
                   1.0);
}

Vec3D vMult(Vec3D v1, Vec3D v2)
{
    return makeVec(v1.x * v2.x,
                   v1.y * v2.y,
                   v1.z * v2.z,
                   1.0);
}

Vec3D vDiv(Vec3D v1, Vec3D v2)
{
    return makeVec(v1.x / v2.x,
                   v1.y / v2.y,
                   v1.z / v2.z,
                   1.0);

}

double vDotP(Vec3D v1, Vec3D v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}


double vLength(Vec3D v)
{
    return sqrt(vDotP(v, v));
}

Vec3D vNormalize(Vec3D v)
{
    double length = vLength(v);
    return makeVec(v.x / length,
                   v.y / length,
                   v.z / length,
                   v.w);
}

Vec3D vCrossProd(Vec3D v1, Vec3D v2){
    return makeVec((v1.y * v2.z) - (v1.z * v2.y),
                   (v1.z * v2.x) - (v1.x * v2.z),
                   (v1.x * v2.y) - (v1.y * v2.x),
                   1.0);
}

Vec3D translateZ(Vec3D p, double add)
{
    return makeVec(p.x, p.y, p.z + add, p.w);
}

Tri transformTri(double rotTheta, Tri tri)
{
    //Do rotations
    double zRotMatrix[4][4];
    getZRotMatrix(zRotMatrix, rotTheta);
    double xRotMatrix[4][4];
    getXRotMatrix(xRotMatrix, rotTheta);
    double combinedMatrix[4][4];
    multMats(xRotMatrix, zRotMatrix, combinedMatrix);

    //apply to points
    tri.p1 = multMatVec(combinedMatrix, tri.p1);
    tri.p2 = multMatVec(combinedMatrix, tri.p2);
    tri.p3 = multMatVec(combinedMatrix, tri.p3);

    // z transform
    tri.p1 = translateZ(tri.p1, 3.0);
    tri.p2 = translateZ(tri.p2, 3.0);
    tri.p3 = translateZ(tri.p3, 3.0);

    // normal calculation
    Vec3D line1 = vSub(tri.p2, tri.p1);
    Vec3D line2 = vSub(tri.p3, tri.p1);
    Vec3D normal = vCrossProd(line1, line2);

    // normalize
    normal = vNormalize(normal);

    // add normal to tri and return
    tri.n = normal;

    return tri;
}

Vec3D scaleForView(Vec3D v)
{
    double h = 300.0;
    double w = 300.0;
    return makeVec((v.x + 1.0) * (0.5 * w), (v.y + 1.0) * (0.5 * h), v.z, v.w);
}
