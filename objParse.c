////////////////////////
// Object File Parser //
// Must return a list of verticies and faces //
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>


int main(int argc, char* argv[]){

    // Open file and create line buffer to read in contents
    FILE *fptr;
    int bufferLength = 255;
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
    double vertList[verts];
    int faceList[faces];

    // Re open file
    if ((fptr = fopen("./cube.obj","r")) == NULL)
    {
        printf("Error opening obj file");
        exit(1);
    }

    int vertIndex = 0;
    int faceIndex = 0;
    while (fgets(buffer, bufferLength, fptr))
    {
        //split our buffer by space delimiter into tokens

        
        // Means split items will be added to vert list
        if (buffer[0] ==  'v' && buffer[1] == ' ')
        {
            char * token = strtok(buffer, " ");
            // dump first token item
            token = strtok(NULL, " ");
            // while the line isn't empty
            while (token != NULL)
            {
                // append items to array
                char * end;
                vertList[vertIndex] = strtod(token, &end);
                vertIndex += 1;

                printf("%s\n", token);

                // Get next token
                token = strtok(NULL, " ");
            }
        }
        // otherwise add to face list
        else if (buffer[0] ==  'f')
        {
            char * token = strtok(buffer, " ");
            // dump first token item
            token = strtok(NULL, " ");
            // while the line isn't empty
            while (token != NULL)
            {
                // still need to split v/vt/vn
                char * subToken = strtok(token, "/");
                // Only take the first one and
                // append items to array
                char * end;
                printf("%s\n", subToken);
                faceList[vertIndex] = atoi(subToken);
                faceIndex += 1;
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(fptr);

    // FILE PARSED, PRINT CONTENTS

    printf("\nPrinting vertex coordinates\n");
    for (int i = 0; i < vertIndex+1; i++)
    {
        printf("%f\n", vertList[i]);
    }
    printf("\nPrinting face vertices\n");
    for (int i = 0; i < faceIndex+1; i++)
    {
        printf("%i\n", faceList[i]);
    }


    return 0;

}