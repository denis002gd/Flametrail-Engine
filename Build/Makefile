# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g `sdl2-config --cflags` -ISweatEngine/HeaderFiles

# SDL2 Libraries
LDFLAGS = `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf

# Source files (note the ../ to go up one directory)
SRCS = ../Src/main.c ../Src/render.c ../Src/timer.c ../Src/game.c ../Src/input.c ../Src/audio.c ../Src/maths.c ../Src/physics.c ../Src/UI.c ../Src/collision.c ../Src/engine.c ../Src/-lm

# Object files (these stay in current directory since Makefile is in Build/)
SRCNAMES = $(notdir $(SRCS))                    # strips folders -> main.c, render.c, ...
OBJNAMES = $(SRCNAMES:.c=.o)                    # replace .c -> .o
OBJS = $(OBJNAMES)                              # no Build/ prefix needed since we're already in Build/

# Executable name
TARGET = program

# Default rule
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Pattern rule to compile .c files to .o files
%.o: ../Src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
