#include <stdio.h>
#include <stdio.h>
#include <math.h>
// make sure pi is defined in this compiler
#ifndef M_PI 
#define M_PI 3.1415926535 
#endif

// Typedefs
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

// Prototypes

// from original
Tri makeTri(Vec3D p1, Vec3D p2, Vec3D p3);
Vec3D makeVec(double x, double y, double z, double w);

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

// Main
int main()
{
    // make a couple test matrices and vector
    Vec3D testp1 = makeVec(0.0, 0.0, 0.0, 1.0);
    Vec3D testp2 = makeVec(0.0, 1.0, 0.0, 1.0);
    Vec3D testp3 = makeVec(1.0, 1.0, 0.0, 1.0);
    Vec3D testp4 = makeVec(1.0, 0.0, 0.0, 1.0);
    // create triangles with uniform winding
    Tri testTri1 = makeTri(testp1, testp2, testp3);
    Tri testTri2 = makeTri(testp1, testp3, testp4);

    double projMatrix[4][4];
    getProjMatrix(projMatrix);
    Vec3D newVec = multMatVec(projMatrix, testp1);

    printf("Compile succesful.\n");
    return 0;
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
    geIdMatrix(m);

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