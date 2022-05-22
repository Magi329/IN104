// Copy from https://github.com/Steboss/music_retrieval/tree/master/stft

/*
Dans audio_read.c, on  définit la fonction de audioread,
qui permet de transformer un son .wav en un vecteur 1D

*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <math.h>

#include <unistd.h>
#include <string.h>
#include "audio_read.h"



#define TRUE 1 
#define FALSE 0

//sudo -apt install libfftw3-dev
//sudo apt install fftw3
//gcc -Wall stft.c -lfftw3 -lm
#define PI 3.14159265

/**
 * Convert seconds into hh:mm:ss format
 * Params:
 *	seconds - seconds value
 * Returns: hms - formatted string
 **/
char* seconds_to_time(float raw_seconds) {
  char *hms;
  int hours, hours_residue, minutes, seconds, milliseconds;
  hms = (char*) malloc(100);

  sprintf(hms, "%f", raw_seconds);

  hours = (int) raw_seconds/3600;
  hours_residue = (int) raw_seconds % 3600;
  minutes = hours_residue/60;
  seconds = hours_residue % 60;
  milliseconds = 0;

  // get the decimal part of raw_seconds to get milliseconds
  char *pos;
  pos = strchr(hms, '.');
  int ipos = (int) (pos - hms);
  char decimalpart[15];
  memset(decimalpart, ' ', sizeof(decimalpart));
  strncpy(decimalpart, &hms[ipos+1], 3);
  milliseconds = atoi(decimalpart);	

  
  sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
  return hms;
}


double hamming(int windowLength, double *buffer){
  for(int i=0; i<windowLength; i++){
    buffer[i] = 0.54 - 0.46*cos((2*PI*i)/(windowLength-1.0));
  }
  return *buffer;

}

double* stft(double *wav_data, int samples, int windowSize, int hop_size,\
             double *magnitude, int sample_freq, int length)
{
  printf("Initialization of parameters...\n");
  int i,counter ;
  counter = 0 ;
  double hamming_result[windowSize];
  double summa;
  fftw_complex *stft_data, *fft_result, *storage;
  stft_data = (fftw_complex*)(fftw_malloc(sizeof(fftw_complex)*(windowSize)));
  fft_result= (fftw_complex*)(fftw_malloc(sizeof(fftw_complex)*(windowSize)));
  storage = (fftw_complex*)(fftw_malloc(sizeof(fftw_complex)*(samples)));
  printf("Total length of storage %d\n", (samples));
  fftw_plan plan_forward;
  plan_forward = fftw_plan_dft_1d(windowSize,stft_data,fft_result, FFTW_FORWARD,FFTW_ESTIMATE);
  printf("Creation of a hamming window...");
  hamming(windowSize, hamming_result);
  for (i=0; i<windowSize; i++)
  {
    summa+=hamming_result[i]*hamming_result[i];
  }
 
  int chunkPosition = 0; 
  int readIndex ; 
  int n_elem_read = 0 ; 

  while (counter < samples- windowSize ){
    
    for(i=0; i<windowSize; i++){

      readIndex = chunkPosition + i;
      stft_data[i] = wav_data[readIndex]*hamming_result[i];
      n_elem_read+=1;
    }
    fftw_execute(plan_forward);
    for (i=0; i<windowSize/2 +1 ;i++)
    {
      storage[counter] = fft_result[i]; 
      counter+=1;

    }

    chunkPosition += hop_size/2;
  }
  printf("%d\n", counter);

  for (i=0; i<counter; i++)
  {
    storage[i] /= (windowSize/2);
  }
  printf("Magnitude\n");

  for (i=0; i< counter; i++)
  {
    magnitude[i] = cabs(storage[i]);
  }

  fftw_destroy_plan(plan_forward);
  fftw_free(stft_data);
  fftw_free(fft_result);
  fftw_free(storage);

  //free(hamming_result);
  return magnitude;
}







unsigned char buffer4[4];
unsigned char buffer2[2];

char* seconds_to_time(float seconds);


 FILE *ptr;
 char *filename;
 
 struct HEADER header;
 

/* obj: coller deux vecteurs en un
   entrée 2 vecteur;
   sortie 1 vecteur
  */
double *coller_2_vect(double *p1, double *p2){
  double *p = p1;
  if(p == NULL){
    printf(" p fail.\n");
  }
  printf("begin coller.\n");
  
  for(int i=0;i<257;i++){
    p1++;
  }
  
  for(int i=0;i<257;i++){
    *p1 = *p2;
    p1++;
    p2++;
  }
  printf("colle done\n");
  return p;
}




void audio_read(char *fname, double *moyenne, double *e_type)
{
  
  int length = 30*22050; // chaque son dure 30s , sample freq = 22050

  int windowSize = 512;
  
  int hop_size = 512;
  
  int col = length/(windowSize/2);
  
  int row = (windowSize/2)+1;
  
  int samples = row * col ; // taille des echantillons
  
  printf("samples:%d.\n",samples);
  
  double* wav_data = malloc(sizeof(double)*(length));
  
  double *magnitude = malloc(sizeof(double)*(col*row)); //amplitude
  
  int sample_freq = 22050;
  
  
//READ THE FILE OF SON ECHANTILLON
 
 // pour avoir wav_data
filename = (char*) malloc(sizeof(char) * 1024);
 if (filename == NULL) {
   printf("Error in malloc\n");
   exit(1);
 }


 // get file path
 char cwd[1024];
 if (getcwd(cwd, sizeof(cwd)) != NULL) {
   
    strcpy(filename, cwd);

    
    
    strcat(filename, "/");
    strcat(filename, fname);
    printf("%s\n", filename);
 }

 // open file
 printf("Opening  file..\n");
 ptr = fopen(fname, "rb");
 printf("chemin : %s\n", filename);
 if (ptr == NULL) {
    printf("Error opening file\n");
    exit(1);
 }
 
 /*create written file*/
 FILE* fw = fopen("son echantillon.txt","w");
	if (fw== NULL) {
  			  printf ("Error written fichier.\n") ;
  			  exit(1) ;
 		 }
 
 int read = 0;
 
 // read header parts

 read = fread(header.riff, sizeof(header.riff), 1, ptr);
 fprintf(fw,"(1-4): %s \n", header.riff); 

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fw,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
 
 // convert little endian to big endian 4 byte int
 header.overall_size  = buffer4[0] | 
                        (buffer4[1]<<8) | 
                        (buffer4[2]<<16) | 
                        (buffer4[3]<<24);

 fprintf(fw,"(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

 read = fread(header.wave, sizeof(header.wave), 1, ptr);
 fprintf(fw,"(9-12) Wave marker: %s\n", header.wave);

 read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
 fprintf(fw,"(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fw,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 // convert little endian to big endian 4 byte integer
 header.length_of_fmt = buffer4[0] |
                            (buffer4[1] << 8) |
                            (buffer4[2] << 16) |
                            (buffer4[3] << 24);
 fprintf(fw,"(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

 read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u n", buffer2[0], buffer2[1]);
 
 header.format_type = buffer2[0] | (buffer2[1] << 8);
 char format_name[10] = "";
 if (header.format_type == 1)
   strcpy(format_name,"PCM"); 
 else if (header.format_type == 6)
  strcpy(format_name, "A-law");
 else if (header.format_type == 7)
  strcpy(format_name, "Mu-law");

 fprintf(fw,"(21-22) Format type: %u %s \n", header.format_type, format_name);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 fprintf(fw,"%u %u \n", buffer2[0], buffer2[1]);

 header.channels = buffer2[0] | (buffer2[1] << 8);
 fprintf(fw,"(23-24) Channels: %u \n", header.channels);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fw,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.sample_rate = buffer4[0] |
                        (buffer4[1] << 8) |
                        (buffer4[2] << 16) |
                        (buffer4[3] << 24);

 fprintf(fw,"(25-28) Sample rate: %u\n", header.sample_rate);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fw,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.byterate  = buffer4[0] |
                        (buffer4[1] << 8) |
                        (buffer4[2] << 16) |
                        (buffer4[3] << 24);
 fprintf(fw,"(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 fprintf(fw,"%u %u \n", buffer2[0], buffer2[1]);

 header.block_align = buffer2[0] |
                    (buffer2[1] << 8);
 fprintf(fw,"(33-34) Block Alignment: %u \n", header.block_align);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 fprintf(fw,"%u %u \n", buffer2[0], buffer2[1]);

 header.bits_per_sample = buffer2[0] |
                    (buffer2[1] << 8);
 fprintf(fw,"(35-36) Bits per sample: %u \n", header.bits_per_sample);

 read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
 fprintf(fw,"(37-40) Data Marker: %s \n", header.data_chunk_header);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fw,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.data_size = buffer4[0] |
                (buffer4[1] << 8) |
                (buffer4[2] << 16) | 
                (buffer4[3] << 24 );
 fprintf(fw,"(41-44) Size of data chunk: %u \n", header.data_size);


 // calculate no.of samples
 long unsigned num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 fprintf(fw,"Number of samples:%lu \n", num_samples);

 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 fprintf(fw,"Size of each sample:%ld bytes\n", size_of_each_sample);

 // calculate duration of file
 float duration_in_seconds = (float) header.overall_size / header.byterate;
 fprintf(fw,"Approx.Duration in seconds=%f\n", duration_in_seconds);
 fprintf(fw,"Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));




 // read each sample from data chunk if PCM
 if (header.format_type == 1) { // PCM
    //printf("Dump sample data? Y/N?");
    char c = 'Y';
    //scanf("%c", &c);
    if (c == 'Y' || c == 'y') { 
        long i =0;
        char data_buffer[size_of_each_sample];
        int  size_is_correct = TRUE;

        // make sure that the bytes-per-sample is completely divisible by num.of channels
        long bytes_in_each_channel = (size_of_each_sample / header.channels);
        if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
            printf("Error: %ld x %ud <> %ldn", bytes_in_each_channel, header.channels, size_of_each_sample);
            size_is_correct = FALSE;
        }
 
        if (size_is_correct) { 
                    // the valid amplitude range for values based on the bits per sample
            long low_limit = 0l;
            long high_limit = 0l;

            switch (header.bits_per_sample) {
                case 8:
                    low_limit = -128;
                    high_limit = 127;
                    break;
                case 16:
                    low_limit = -32768;
                    high_limit = 32767;
                    break;
                case 32:
                    low_limit = -2147483648;
                    high_limit = 2147483647;
                    break;
            }					



            printf("nn.Valid range for data values : %ld to %ld n", low_limit, high_limit);
            for (i =1; i <= length; i++) {
                fprintf(fw,"==========Sample %ld / %ld=============n", i, num_samples);
                read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
                if (read == 1) {
                
                    // dump the data read
                    unsigned int  xchannels = 0;
                    int data_in_channel = 0;
                    int offset = 0; // move the offset for every iteration in the loop below
                    for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
                    	
                        fprintf(fw, "Channel#%d : ", (xchannels+1));
                        
                        // convert data from little endian to big endian based on bytes in each channel sample
                        if (bytes_in_each_channel == 4) {
                            data_in_channel = (data_buffer[offset] & 0x00ff) | 
                                                ((data_buffer[offset + 1] & 0x00ff) <<8) | 
                                                ((data_buffer[offset + 2] & 0x00ff) <<16) | 
                                                (data_buffer[offset + 3]<<24);
                        }
                        else if (bytes_in_each_channel == 2) {
                            data_in_channel = (data_buffer[offset] & 0x00ff) |
                                                (data_buffer[offset + 1] << 8);
                        }
                        else if (bytes_in_each_channel == 1) {
                            data_in_channel = data_buffer[offset] & 0x00ff;
                            data_in_channel -= 128; //in wave, 8-bit are unsigned, so shifting to signed
                        }

                        offset += bytes_in_each_channel;		
                        fprintf(fw,"%d ", data_in_channel);
                        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        
                        
                        //remplir de tableau wav_data avec les elements data_in_channel;
                        
     					wav_data[i] = data_in_channel;
     					//printf("wav_data[%ld]=%f\n",i,wav_data[i]);
     					
     					
                         
                        // check if value was in range
                        if (data_in_channel < low_limit || data_in_channel > high_limit)
                            fprintf(fw,"**value out of rangen");

                        fprintf(fw," | ");
                    }

                    fprintf(fw,"n");
                }
                else {
                    fprintf(fw,"Error reading file. %d bytesn", read);
                    break;
                }

            } // 	for (i =1; i <= num_samples; i++) {

        } // 	if (size_is_correct) { 

     } // if (c == 'Y' || c == 'y') { 
 } //  if (header.format_type == 1) { 

 fprintf(fw,"Closing file..n\n");
 fclose(ptr);

  // cleanup before quitting
 free(filename);
 

  stft(&wav_data[0], samples, windowSize, hop_size,&magnitude[0],sample_freq, length);
  
  // for (int i = 0; i < 10; i++)
  // {
  //   printf("%f\n", magnitude[i]);
  // }
  // printf("col = %d, row = %d\n",col,row);




// calculer la moyenne et écart_type de magnetude
  // double* moyenne = malloc(sizeof(double)*row);
  // double* e_type = malloc(sizeof(double)*row);
 
  

  for(int i=0;i<row;i++){
    double moy_tmp = 0;
    double ecart_tmp = 0;

    for(int j=0;j<col;j++){
      moy_tmp += magnitude[i+j*row];
    }
    moyenne[i] = moy_tmp/col;

    for(int j=0;j<col;j++){
      ecart_tmp += (magnitude[i+j*row] - moyenne[i])*(magnitude[i+j*row] - moyenne[i]);
    }
    e_type[i] = sqrt(ecart_tmp/col);

    
  }

// verifier si on a de bons resultats
// for(int i=0;i<3;i++){
// printf("%d ligne : moyenne = %f, écart_type = %f\n",i,moyenne[i],e_type[i]);
// }







}

