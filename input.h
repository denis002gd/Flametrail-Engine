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
}InputState;

void InitInput(InputState *input);
void UpdateInput(InputState *input);
void Input_ProcessEvent(SDL_Event* event, InputState *input);
bool Input_GetKey(SDL_Scancode key, InputState *input);
bool Input_GetKeyDown(SDL_Scancode key, InputState *input);
bool Input_GetKeyUp(SDL_Scancode key, InputState *input);
void Input_GetMousePosition(InputState *input, int *x, int *y);
bool Input_GetMouseButton(int button, InputState *input);
bool Input_GetMouseButtonDown(int button, InputState *input);
bool Input_GetMouseButtonUp(int button, InputState *input);
bool Input_ShouldQuit(InputState *input);



#endif // !INPUT_H
