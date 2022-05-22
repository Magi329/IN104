 #include <stdio.h>
 #include <stdlib.h>

 
 #include "utils.h"
 #include "audio_read.h"


 void prediction(char *filew, char *fileb, char *son){
    double W[10][514];
    double B[10][1];
    

    read_array(filew, 10 , 514, W);
    //double arr[10][514];
    read_array(filew, 10 , 1, B);
    //double arr[10][1];

    double *pred = malloc(sizeof(double)*10);

    double *moyenne = malloc(sizeof(double)*257); 
    double *e_type = malloc(sizeof(double)*257);

    audio_read(son, moyenne, e_type);

    double x[514][1];

    for(int i=0;i<257;i++){
       x[i][0] = moyenne[i];
       x[i+257][0] = e_type[i];

    }
     

     // Matrix multiplication
     double y[10][1];
     for(int i=0;i<10;i++){
        y[i][0] =0;
         for(int k=0;k<514;k++){
            y[i][0] += W[i][k]*x[k][0] + B[i][0];
         }  
     }

     // find maximum
    int index_max=0;     //init
    for(int i=1;i<10;++i)
    {
        if(y[i][0]>y[index_max][0])
        {
            index_max=i;     // pour trouver la position
        }
    }

   //printf("max=%f,index_max=%d",y[index_max][0],index_max);

   // printf
   if(index_max ==0){
      printf("The music genre is blues.\n");
   }
   if(index_max ==1){
      printf("The music genre is classical.\n");
   }
   if(index_max ==2){
      printf("The music genre is country.\n");
   }
   if(index_max ==3){
      printf("The music genre is disco.\n");
   }
   if(index_max ==4){
      printf("The music genre is hiphop.\n");
   }
   if(index_max ==5){
      printf("The music genre is jazz.\n");
   }
   if(index_max ==6){
      printf("The music genre is metal.\n");
   }
   if(index_max ==7){
      printf("The music genre is pop.\n");
   }
   if(index_max ==8){
      printf("The music genre is reggae.\n");
   }
   if(index_max ==9){
      printf("The music genre is rock.\n");
   }
 }


int main()
{

   prediction("path to w","path to b","./genres/hiphop/hiphop.00030.wav");

   return 0;
}