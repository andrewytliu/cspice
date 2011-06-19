#include <math.h>
#include <stdio.h>
#include <cuda.h>

#include "integral.h"

__constant__ double PI;

__device__ void complexDiv(double *num_r, double *num_i,
                           double *dem_r, double *dem_i,
                           double *res_r, double *res_i) {
   *res_r = ((*num_r)*(*dem_r)+(*num_i)*(*dem_i)) / ((*dem_r)*(*dem_r)+(*dem_i)*(*dem_i));
   *res_i = ((*num_i)*(*dem_r)-(*num_r)*(*dem_i)) / ((*dem_r)*(*dem_r)+(*dem_i)*(*dem_i));
}


__device__ void gpuEval(double *coefficients, int size, double freq,
                        double *real, double *image) {
   double omega = 2.0 * PI * freq ;
   *image = 0.0;
   *real = 0.0 ;

   int maxOrder = size;
   double s = 1 ;

   for(int order = 0 ; order < maxOrder ; ++ order) {
      // i^0 = 1, i^1 = i, i^2 = -1, i^3 = -i
      switch(order & 3) {
         case 0:
            *real  += s * coefficients[order] ; break ;
         case 1:
            *image += s * coefficients[order] ; break ;
         case 2:
            *real  -= s * coefficients[order] ; break ;
         case 3:
            *image -= s * coefficients[order] ; break ;
      }
      s *= omega ;
   }
}

__global__ void freqKernel(double *freq, double *real, double *image,
                           double *tf_num, int tf_num_size,
                           double *tf_den, int tf_den_size,
                           double start, double ratio) {
   int index = blockIdx.x;
   //printf("GPU: %d\n", index);
   double cur_freq = start * powf(ratio, index);
   freq[index] = cur_freq;
   double num_r, num_i, den_r, den_i;
   gpuEval(tf_num, tf_num_size, cur_freq, &num_r, &num_i);
   gpuEval(tf_den, tf_den_size, cur_freq, &den_r, &den_i);
   complexDiv(&num_r, &num_i, &den_r, &den_i, &real[index], &image[index]);
}

void freqGpuSimulate(double *freq, double *real, double *image, double *tf_num, int tf_num_s, double *tf_den, int tf_den_s, double start, double ratio, int kernels) {
  double host_PI = acos(-1.0);

  double *freq_c, *real_c, *image_c, *tf_num_c, *tf_den_c;
  cudaMemcpyToSymbol("PI", &host_PI, sizeof(host_PI));
  cudaMalloc((void**) &freq_c,  kernels * sizeof(double));
  cudaMalloc((void**) &real_c,  kernels * sizeof(double));
  cudaMalloc((void**) &image_c, kernels * sizeof(double));
  cudaMalloc((void**) &tf_num_c, tf_num_s * sizeof(double));
  cudaMalloc((void**) &tf_den_c, tf_den_s * sizeof(double));

  cudaMemcpy(tf_num_c, tf_num, tf_num_s * sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(tf_den_c, tf_den, tf_den_s * sizeof(double), cudaMemcpyHostToDevice);
  freqKernel <<< kernels, 1 >>> (freq_c, real_c, image_c, tf_num_c, tf_num_s, tf_den_c, tf_den_s, start, ratio);
  cudaMemcpy(freq, freq_c, kernels * sizeof(double), cudaMemcpyDeviceToHost);
  cudaMemcpy(real, real_c, kernels * sizeof(double), cudaMemcpyDeviceToHost);
  cudaMemcpy(image, image_c, kernels * sizeof(double), cudaMemcpyDeviceToHost);

  cudaFree(freq_c);
  cudaFree(real_c);
  cudaFree(image_c);
  cudaFree(tf_num_c);
  cudaFree(tf_den_c);
}

