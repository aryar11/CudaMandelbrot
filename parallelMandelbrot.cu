#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>

// CUDA kernel: compute one pixel's supersampled Mandelbrot average
__global__ void mandelbrot_kernel(double *result, int width, int height, int max_iter, double x_min, double x_max, double y_min, double y_max, int SS){

    int px = blockIdx.x * blockDim.x + threadIdx.x;
    int py = blockIdx.y * blockDim.y + threadIdx.y;
    if (px >= width || py >= height) return;
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;
    double invSS2 = 1.0 / (SS * SS);
    double sum = 0.0;

    // unroll supersample loop
    #pragma unroll
    for (int sy = 0; sy < SS; ++sy) {
        #pragma unroll
        for (int sx = 0; sx < SS; ++sx) {
            double cx = x_min + (px + (sx + 0.5) * invSS2) * dx;
            double cy = y_min + (py + (sy + 0.5) * invSS2) * dy;

            double zx = 0.0;
            double zy = 0.0;
            int iter = 0;
            double zx2 = 0.0;
            double zy2 = 0.0;

            while (zx2 + zy2 <= 4.0 && iter < max_iter) {
                zy = 2.0 * zx * zy + cy;
                zx = zx2 - zy2 + cx;
                zx2 = zx * zx;
                zy2 = zy * zy;
                ++iter;
            }
            sum += iter;
        }
    }
    result[py * width + px] = sum * invSS2;
}

//host 
extern "C" double* compute_mandelbrot_cuda( int width, int height, int max_iter, double x_min, double x_max, double y_min, double y_max, int SS){
    if (width <= 0 || height <= 0 || max_iter <= 0 || SS <= 0) {
        fprintf(stderr, "Invalid parameters for compute_mandelbrot_cuda\n");
        return NULL;
    }

    size_t total = (size_t)width * height;
    size_t bytes = total * sizeof(double);
    double *h_result = (double*)malloc(bytes);
    if (!h_result) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    double *d_result;
    cudaMalloc(&d_result, bytes);

    dim3 block(16, 16);
    dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);

    mandelbrot_kernel<<<grid, block>>>( d_result, width, height, max_iter, x_min, x_max, y_min, y_max, SS);
    cudaDeviceSynchronize();
    cudaMemcpy(h_result, d_result, bytes, cudaMemcpyDeviceToHost);
    cudaFree(d_result);
    return h_result;
}
