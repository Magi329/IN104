// Copy from https://github.com/Steboss/music_retrieval/tree/master/stft

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <math.h>

#define PI 3.14159265

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

int main()
{
  double wav_data[32];
  int samples = 16;
  int windowSize = 8;
  int hop_size = 2;
  double magnitude[16];
  int sample_freq = 2;
  int length = 32;

  for (int i = 0; i < 32; i++)
  {
     wav_data[i] = i * 2.5;
  }

  stft(&wav_data[0], samples, windowSize, hop_size,&magnitude[0],sample_freq, length);
  
  for (int i = 0; i < 16; i++)
  {
    printf("%f\n", magnitude[i]);
  }

  return 0; 
}

