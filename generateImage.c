#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <time.h>
#include "generateImage.h"
#include "mandelbrot.h"
#include "parallelMandelbrot.h"

/*
* this function converts HSV color values to RGB.
* h is in degrees [0,360), s and v are in [0,1.0].
* out_r, out_g, out_b are the output RGB values in [0,255]     
*/
static void hsv_to_rgb(double h, double s, double v, Uint8 *out_r, Uint8 *out_g, Uint8 *out_b){
    double c = v * s;
    double x = c * (1 - fabs(fmod(h/60.0, 2.0) - 1));
    double m = v - c;
    double rp=0, gp=0, bp=0;
    if (h < 60) { rp = c; gp = x; bp = 0; }
    else if (h <120) { rp = x; gp = c; bp = 0;}
    else if (h <180)  { rp = 0; gp = c; bp = x; }
    else if (h <240) { rp = 0; gp = x; bp = c; }
    else if (h <300) { rp = x; gp = 0; bp = c;}
    else { rp = c; gp = 0; bp = x; }
    *out_r = (Uint8)((rp + m) * 255);
    *out_g = (Uint8)((gp + m) * 255);
    *out_b = (Uint8)((bp + m) * 255);
}



void generate_image(int width, int height, int max_iter, int x_min, int x_max, int y_min, int y_max, int SS, bool cuda){
    //init SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);

    double *mandel = NULL;
    bool dirty = true;

    SDL_Event e;
    bool quit = false;
    const double zoomFactor = 0.8;    // 80% zoom in, 125% zoom out

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                // mouse position as fractal center
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                double cx = x_min + mx /(double)width * (x_max - x_min);
                double cy =y_min + my /(double)height* (y_max - y_min);

                //  factor
                double zoom = (e.wheel.y > 0 ? zoomFactor : 1.0/zoomFactor);

                double halfW =(x_max - x_min) * 0.5 * zoom;
                double halfH =(y_max - y_min) * 0.5 * zoom;

                // recenter around (cx,cy)
                x_min = cx - halfW;
                x_max = cx + halfW;
                y_min = cy - halfH;
                y_max = cy + halfH;
                dirty = true;
            }
        }

        if (dirty) {
            free(mandel);
            if(cuda) { 
                clock_t start = clock();
                mandel = compute_mandelbrot_cuda(width, height, max_iter, x_min, x_max, y_min, y_max, SS); 
                clock_t end = clock();
                double total_time = (double)(end - start) / CLOCKS_PER_SEC;
                printf("CUDA computation took %.6f seconds\n", total_time);
            }
            else{ 
                clock_t start = clock();
                mandel = compute_mandelbrot(width, height, max_iter,x_min, x_max, y_min, y_max, SS); 
                clock_t end = clock();
                double total_time = (double)(end - start) / CLOCKS_PER_SEC;
                printf("Serial computation took %.6f seconds\n", total_time);
            }
            uint32_t *pixels = malloc(width * height * sizeof(uint32_t));
            for (int i = 0; i < width*height; i++) {
                double val = mandel[i];
                Uint8 r, g, b;
                if (val >= max_iter) {
                    r = g = b = 0;
                } else {
                    double hue = 360.0 * (val / (double)max_iter);
                    hsv_to_rgb(hue, 1.0, 1.0, &r, &g, &b);
                }
                pixels[i] = (r<<16) | (g<<8) | b;
            }
            SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(uint32_t));
            free(pixels);

            dirty = false;
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);

        SDL_Delay(10);
    }
    free(mandel);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return;
}


