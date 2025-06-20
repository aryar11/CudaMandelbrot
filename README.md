# CudaMandelbrot

A Mandelbrot set visualizer and generator written in C and CUDA. Implemented CUDA optimization to parallelize the Mandelbrot set generation via NVIDIA GPU's.

## Features

- **GPU Acceleration**: Leverages CUDA to compute the Mandelbrot set much faster than CPU-only solutions.
- **Customizable Output**: Settings for image size, region of interest, optional CUDA usage. 

## Requirements

- NVIDIA GPU with CUDA support (For CUDA useage)
- CUDA Toolkit
- GCC or compatible C compiler
- Make

## Build Instructions

Clone the repository and build using the provided Makefile:

```bash
git clone https://github.com/aryar11/CudaMandelbrot.git
cd CudaMandelbrot
make
```

This will generate the executable(s) in the build directory.

## Usage

Basic usage example:

```bash
./mandelbrot [options]
```

Common options (customize this list based on your actual CLI options):

- `-w <width>`: Set the image width (default: 800)
- `-h <height>`: Set the image height (default: 600)
- `-i <iterations>`: Set the maximum number of iterations (default: 100)
- `-x <center_x>`: Set the x-coordinate of the center point (default: -0.5)
- `-y <center_y>`: Set the y-coordinate of the center point (default: 0)
- `-c <cuda>`: Optional Cuda Usage (default: 1)

Example:

```bash
./CudaMandelbrot -w 1920 -h 1080 -i 2000 -o output.ppm -x -0.75 -y 0.1 -z 2
```

## Output

The program generates Mandelbrot set images in PPM format by default. You can view them with most image viewers or convert them to other formats using tools like ImageMagick:

```bash
convert output.ppm output.png
```

## File Structure

- `main.c` / `main.cu`: Main program logic (CPU and CUDA versions)
- `mandelbrot.cu`: CUDA kernel for Mandelbrot computation
- `Makefile`: Build instructions
- `README.md`: This file

## Contributing

Pull requests, bug reports, and feature suggestions are welcome! Please open an issue or submit a PR.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

Let me know if you’d like to add usage examples, images, or further details!
