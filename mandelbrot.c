#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "generateImage.h"

/**
 * compute mandelbrot iteration counts on an width X height grid
 *
 * @param width     self-explanatory
 * @param height    self-explanatory
 * @param max_iter  max number of iterations per point
 * @param x_min     min real coordinate
 * @param x_max     max real coord
 * @param y_min     min imaginary coord
 * @param y_max     max imaginary coord
 * @param SS        supersampling factor (1 = no supersampling)
 * @return          Pointer double array of length n*m,
 *                  where result[i*m + j] is the iteration count for row i, col j.
 */
double *compute_mandelbrot(int width, int height, int max_iter, double x_min, double x_max, double y_min, double y_max, int SS){
    if (width <= 0 || height <= 0 || max_iter <= 0 || SS <= 0) {
        fprintf(stderr, "Invalid parameters for compute_mandelbrot\n");
        return NULL;
    }
    
    int total = width * height;
    double *result = malloc((size_t)total * sizeof(double));
    if (!result) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // step sizes per pixel
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;
    double invSS2 = 1.0 / (SS * SS);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double sum = 0.0;

            // supersample
            for (int sy = 0; sy < SS; ++sy) {
                for (int sx = 0; sx < SS; ++sx) {
                    // sample point in complex plane
                    double cx = x_min + (x + (sx + 0.5) * invSS2) * dx;
                    double cy = y_min + (y + (sy + 0.5) * invSS2) * dy;

                    // Mandelbrot iteration: z = zx + i*zy
                    double zx = 0.0, zy = 0.0;
                    int iter = 0;
                    while (zx*zx + zy*zy <= 4.0 && iter < max_iter) {
                        double xt = zx*zx - zy*zy + cx;
                        zy = 2.0*zx*zy + cy;
                        zx = xt;
                        ++iter;
                    }

                    sum += iter;
                }
            }
            //store the average iteration count for this pixel
            result[y * width + x] = sum * invSS2;
        }
    }

    return result;
}


static void usage(const char *prog){
    fprintf(stderr,
        "Usage: %s [options]\n"
        "Options:\n"
        " -w, --width WIDTH  image width (default 800)\n"
        " -h, --height HEIGHT image height (default 600)\n"
        " -i, --max-iter ITERS max iterations (default 100)\n"
        "-s, --supersample SS supersampling factor (default 2)\n"
        "--xmin XMIN minimum x (default -2.0)\n"
        " --xmax XMAX maximum x (default  1.0)\n"
        "--ymin YMIN minimum y (default -1.0)\n"
        "--ymax YMAX maximum y (default  1.0)\n"
        "-c, --cuda use CUDA (default: off)\n"
        "--help this message\n",
        prog);
    exit(1);
}

int main(int argc, char *argv[]){
    int width = 800, height = 600, max_iter = 100, SS = 2;
    double x_min = -2.0, x_max = 1.0, y_min = -1.0, y_max = 1.0;
    bool cuda = false;
    static struct option opts[] = {
        { "width", required_argument, NULL, 'w' },
        { "height",required_argument, NULL,'h' },
        { "max-iter",required_argument, NULL, 'i' },
        { "supersample",required_argument, NULL,'s' },
        { "xmin", required_argument, NULL, 0 },
        { "xmax", required_argument, NULL, 1},
        { "ymin", required_argument, NULL, 2},
        { "ymax", required_argument, NULL, 3},
        { "cuda", no_argument, NULL, 'c' },
        { "help", no_argument, NULL,  4  },
        { NULL,0,NULL,0 }
    };

    int opt, idx;
    while ((opt = getopt_long(argc, argv, "w:h:i:s:c", opts, &idx)) != -1) {
        switch (opt) {
            case 'w': width= atoi(optarg); break;
            case 'h': height = atoi(optarg); break;
            case 'i': max_iter = atoi(optarg); break;
            case 's': SS = atoi(optarg); break;
            case 'c': cuda = true; break;
            case  0: x_min = atof(optarg); break;  // --xmin
            case  1: x_max = atof(optarg); break;  // --xmax
            case  2: y_min = atof(optarg); break;  // --ymin
            case  3: y_max = atof(optarg); break;  // --ymax
            case  4:                
            default:  usage(argv[0]);
        }
    }

    if (width <= 0 || height <= 0 || max_iter <= 0 || SS <= 0)
        usage(argv[0]);
    if (x_min >= x_max || y_min >= y_max)
        usage(argv[0]);

    generate_image(width, height, max_iter,  x_min, x_max, y_min, y_max, SS, cuda);
    return 0;
}
