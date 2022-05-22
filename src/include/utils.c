#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read_array(char filename[],  int m, int n, double arr[][n])
{
    FILE* fp = fopen(filename, "r");
    //double arr[10][514];
    if (!fp)
        printf("Can't open file\n");
 
    else {
        char buffer[4096];
 
        int row = 0;
        int column = 0;
 
        while (fgets(buffer,
                     4096, fp)) {
            column = 0;
            char* value = strtok(buffer, ", ");
            while (value) {
                arr[row][column] = atof(value);
                value = strtok(NULL, ", ");
                column++;
            }
            row++;
        }
        fclose(fp);
    }
}
