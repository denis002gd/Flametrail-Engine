#include "input.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <string.h>


//initialize the input struct
void InitInput(InputState *input){
  memset(input, 0, sizeof(InputState));
}
//call this once per frame before processing events to register current input
void UpdateInput(InputState *input){
  memcpy(input->prev_keys, input->keys, sizeof(input->keys));
  memcpy(input->prevMouseButtons, input->mouseButtons, sizeof(input->mouseButtons));

  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  for(int i = 0; i < SDL_NUM_SCANCODES; i++){
    input->keys[i] = keystate[i];
  }
  Uint32 mouseState = SDL_GetMouseState(&input->mouseX, &input->mouseY);
  input->mouseButtons[0] = (mouseState & SDL_BUTTON_LMASK) != 0;
  input->mouseButtons[1] = (mouseState & SDL_BUTTON_MMASK) != 0;
  input->mouseButtons[2] = (mouseState & SDL_BUTTON_RMASK) != 0;
  input->mouseButtons[3] = (mouseState & SDL_BUTTON_X1MASK) != 0;
  input->mouseButtons[4] = (mouseState & SDL_BUTTON_X2MASK) != 0;
}
//fuction for processing SDL events - call this once per frame
void Input_ProcessEvent(SDL_Event *event, InputState *input){
  switch (event->type) {
    case SDL_QUIT:
    input->closeGame = true;
    break;
  }
}
//register if key is pressed down
bool Input_GetKey(SDL_Scancode key, InputState *input){
  return input->keys[key];
}
//returns true at the button press down
bool Input_GetKeyDown(SDL_Scancode key, InputState *input){
  return input->keys[key] && !input->prev_keys[key];
}
//returns true at button release
bool Input_GetKeyUp(SDL_Scancode key, InputState *input){
  return !input->keys[key] && input->prev_keys[key];
}
//asigns the mouse position to the x and y integer pointers
void Input_GetMousePosition(InputState *input, int *x, int *y){
  *x = input->mouseX;
  *y = input->mouseY;
}
bool Input_GetMouseButton(int button, InputState *input){
  if(button < 0 || button >= 5) return false; //outisde of button range
  return input->mouseButtons[button];
}
bool Input_GetMouseButtonDown(int button, InputState *input){
  if(button < 0 || button >= 5) return false;
  return input->mouseButtons[button] && !input->prevMouseButtons[button];
}
bool Input_GetMouseButtonUp(int button, InputState *input){
  if(button < 0 || button >= 5) return false;
  return !input->mouseButtons[button] && input->prevMouseButtons[button];
}

bool Input_ShouldQuit(InputState *input){
  return input->closeGame;
}
