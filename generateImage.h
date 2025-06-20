#ifndef GENERATE_IMAGE_H
#define GENERATE_IMAGE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
// build the window & renderer, draw the buffer, wait for quit
void generate_image(int width, int height, int max_iter, double x_min, double x_max, double y_min, double y_max, int SS, bool cuda);

// render into an existing renderer
static void hsv_to_rgb(double h, double s, double v, Uint8 *out_r, Uint8 *out_g, Uint8 *out_b);

#endif // GENERATE_IMAGE_H
