# Specify the compiler
CC = gcc

# Specify the compiler flags
CFLAGS = -Wall -Wextra

# Specify the Raylib include path
RAYLIB_INCLUDE_PATH = /path/to/raylib/include

# Specify the Raylib library path
RAYLIB_LIB_PATH = /path/to/raylib/lib

# Specify the Raylib library name
RAYLIB_LIB = -lraylib

# Specify the output file
OUTPUT = main

# Rule to build the program
all: main.c
	$(CC) $(CFLAGS) -I$(RAYLIB_INCLUDE_PATH) -L$(RAYLIB_LIB_PATH) $(RAYLIB_LIB) main.c -o $(OUTPUT)

# Rule to clean the build
clean:
	rm -f $(OUTPUT)
