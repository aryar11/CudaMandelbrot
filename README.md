# CudaMandelbrot

A Mandelbrot set visualizer and generator written in C and CUDA. Implemented CUDA optimization to parallelize the Mandelbrot set generation via NVIDIA GPU's.

## Features

- **GPU Acceleration**: Uses CUDA to compute the Mandelbrot set much faster than CPU-only implemenation
- **Customizable Output**: Settings for image size, region of interest, optional CUDA usage. 

## Requirements

- NVIDIA GPU with CUDA support (For CUDA usage)
- CUDA Toolkit
- GCC
- Make

## Build Instructions

Clone the repository and build using the makefile:

```bash
git clone https://github.com/aryar11/CudaMandelbrot.git
cd CudaMandelbrot
make
```

This will generate the executable

## Usage

Command-Line Options

| Option                | Description                                | Default |
| --------------------- | ------------------------------------------ | ------- |
| `-w`, `--width`       | Set the image width                        | 800     |
| `-h`, `--height`      | Set the image height                       | 600     |
| `-i`, `--max-iter`    | Maximum number of iterations per point     | 100     |
| `-s`, `--supersample` | Supersampling factor                       | 2       |
| `--xmin`              | Minimum x-coordinate of the viewing window | -2.0    |
| `--xmax`              | Maximum x-coordinate of the viewing window | 1.0     |
| `--ymin`              | Minimum y-coordinate of the viewing window | -1.0    |
| `--ymax`              | Maximum y-coordinate of the viewing window | 1.0     |
| `-c`, `--cuda`        | Enable CUDA acceleration                   | Off     |
| `--help`              | Display usage message                      |         |


Example:

```bash
./mandelbrot -w 1920 -h 1080 -i 500 --xmin -2.0 --xmax 1.0 --ymin -1.0 --ymax 1.0 -s 4 -c
```

This command renders a Mandelbrot set with 500 iterations per point using CUDA with supersampling

## Output

The program generates a SDL texture popup of a Mandelbrot set. The user is able to zoom in one time (via mouse scroll) and the set will regenerate with the new zoomed in field. Currently, there is a bug after 2 zoom in's the mandelbrot set does not properly display. 

## File Structure

- `mandelbrot.c` : Main program logic 
- `parallelMandelbrot.cu`: CUDA kernel for Mandelbrot computation
- `generateImage.h`: Call's either the parallel or serial mandelbrot function and generates the SDL window.
- `Makefile`: Build instructions

## Contributing

Feel free to reach out to me with any idea on how to improve or add functionality to this tool. This was a fun personal project I worked on to learn some CUDA programming basics and I am always looking for ways to parallelize problems via CUDA!
