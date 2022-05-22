#include "encoding.h"
#include "audio_read.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
Dans encoding.c, on définit la fonction savetocsv
qui lit tous les fichiers de wav, et écrire leurs moyenne & ecart-type dans le fichier features.csv
*/

const char *itoa(int i, int i1);

int savetocsv() {
    char * genres[10] = {"blues", "classical", "country", "disco", "hiphop", "jazz", "metal", "pop", "reggae", "rock"};
    int nb_genre = 10;

    // open our csv file
    FILE *csv = fopen("features.csv", "w");
    if (csv == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    printf("Succeed opening file!\n");

    // initialize our caracteristics
    double *moyenne = malloc(sizeof(double) * 257);
    double *e_type = malloc(sizeof(double) * 257);


    for(int g = 0 ; g < nb_genre-8 ; g++) {
        printf("boucle %d begins.\n",g);
        for (int i=0 ; i < 100; i++){
            char path[1000];
            if (i < 10){
                sprintf(path, "./genres/%s/%s.0000%d.wav", genres[g],genres[g], i);
                audio_read(path, moyenne, e_type);
                // save the data
                fprintf(csv,"%d, ", g);
                for (int j = 0; j < 256; j++) {
                    fprintf(csv, "%f,%f,", moyenne[j],e_type[j]);
                }
                fprintf(csv, "%f,%f\n",moyenne[256],e_type[256]);
                
            }
            else {
                sprintf(path, "./genres/%s/%s.000%d.wav", genres[g], genres[g], i);
                audio_read(path, moyenne, e_type);
                fprintf(csv,"%d, ", g);
                for (int j = 0; j < 256; j++) {
                    fprintf(csv, "%f,%f,", moyenne[j],e_type[j]);
                }
                fprintf(csv, "%f,%f\n",moyenne[256],e_type[256]);
            }
            
        }
        //printf("boucle %d ends.\n",g);
        
    }

    

    fclose(csv);
    return 0;
}

const char *itoa(int i, int i1) {
    return NULL;
}

int main(int argc, char **argv)
{


    /* lignes de commande au terminal

    cd ~/Desktop/in104/IN104_2/IN104/src/include

    gcc audio_read.c encoding.c main.c -Wall -lfftw3 -lm -o audioread

    
    */



    // char *fname = "../../../../IN104/genres/blues/blues.00000.wav";
    // audio_read(fname);

    savetocsv();




    return 0;
}