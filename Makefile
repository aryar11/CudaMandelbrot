# Compiler toolchain
CXX := clang++
NVCC := nvcc

# Pull in only the compile flags here:
CFLAGS  := `sdl2-config --cflags` -ggdb3 -O0 --std=c99 -Wall
NVCCFLAGS := -O0 -g -std=c++11

# Link flags go here:
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lm -L/usr/local/cuda/lib64 -lcudart

# Sources
SRC_C   := mandelbrot.c generateImage.c
SRC_CU  := parallelMandelbrot.cu

# Objects
OBJ_C   := $(SRC_C:.c=.o)
OBJ_CU  := $(SRC_CU:.cu=.o)
OBJ     := $(OBJ_C) $(OBJ_CU)

# Default target
all: out

# C compiler rule
%.o: %.c generateImage.h mandelbrot.h parallelMandelbrot.h
	$(CC) $(CFLAGS) -c $< -o $@

# CUDA compiler rule
%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

out: $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o out

# Clean up
clean:
	rm -f $(OBJ) out
