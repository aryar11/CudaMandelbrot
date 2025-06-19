#ifdef __cplusplus
extern "C" {
#endif

double* compute_mandelbrot_cuda(
    int width, int height, int max_iter,
    double x_min, double x_max,
    double y_min, double y_max,
    int SS);

#ifdef __cplusplus
}
#endif
