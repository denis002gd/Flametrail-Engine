#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <stdbool.h>

typedef struct{
  bool keys[SDL_NUM_SCANCODES];
  bool prev_keys[SDL_NUM_SCANCODES];
  bool mouseButtons[5];
  bool prevMouseButtons[5];
  int mouseX, mouseY;
  bool closeGame;
}InputStatel;
void GetInput(SDL_Event event);

#endif // !INPUT_H
