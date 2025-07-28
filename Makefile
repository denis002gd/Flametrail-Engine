# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g `sdl2-config --cflags`

# SDL2 Libraries
LDFLAGS = `sdl2-config --libs` -lSDL2_mixer

# Source files
SRCS = main.c render.c timer.c game.c input.c audio.c maths.c -lm

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = program

# Default rule
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

