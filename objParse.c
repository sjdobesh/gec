////////////////////////
// Object File Parser //
// Must return a list of verticies and faces //
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Type def our structs
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


int objParse(int argc, char* argv[]);

int main(int argc, char* argv[]){
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    objParse(argc,argv);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Obj processed in %f seconds.\n", cpu_time_used);
    return 0;
}

int objParse(int argc, char* argv[]){

    // Open file and create line buffer to read in contents
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

    // Before reopening file, clear buffer just in case
    memset(buffer, 0, sizeof(buffer));

    // Re open file
    if ((fptr = fopen("./cube.obj","r")) == NULL)
    {
        printf("Error opening obj file");
        exit(1);
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
            printf("Dumping first token : %s\n", token);
            // dump first token item
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
    Tri triList[sizeof(pointList) / 3];
    int triInd = 0;
    for (int i = 0; i < faceIndex; i= i + 3)
    {
        Tri newTri;
        newTri.p1 = pointList[faceList[i]];
        newTri.p2 = pointList[faceList[i + 1]];
        newTri.p3 = pointList[faceList[i + 2]];
        triList[triInd] = newTri;
        triInd += 1;
    }
    printf("Generated %ld triangles.\n", sizeof(triList));
    return 0;

}

