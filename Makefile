# Compiler toolchain
CXX := clang++
NVCC := nvcc

CFLAGS  := `sdl2-config --cflags` -ggdb3 -O0 --std=c99 -Wall
NVCCFLAGS := -O0 -g -std=c++11

# Link flags
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lm -L/usr/local/cuda/lib64 -lcudart

SRC_C   := mandelbrot.c generateImage.c
SRC_CU  := parallelMandelbrot.cu
OBJ_C   := $(SRC_C:.c=.o)
OBJ_CU  := $(SRC_CU:.cu=.o)
OBJ     := $(OBJ_C) $(OBJ_CU)

# target
all: mandelbrot

%.o: %.c generateImage.h mandelbrot.h parallelMandelbrot.h
	$(CC) $(CFLAGS) -c $< -o $@

# CUDA compiler
%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

mandelbrot: $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o mandelbrot

clean:
	rm -f $(OBJ) mandelbrot
