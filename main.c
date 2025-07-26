#include "input.h"
#include "render.h"
#include "timer.h"
#include "game.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>


int main(){
  InputState inputState = {0};
  InitInput(&inputState);
  double sec = 0;
  GameContext game_cont = {0};
  game_cont.window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
  if(InitGameContext(&game_cont)){
    exit(1);
  }
  game_cont.earlyFrame = SDL_GetTicks();
  game_cont.lateFrame = 0;
  game_cont.running = 1;
  SDL_Event event;
  sec = 0;
  while(game_cont.running){
    //delta time calculation
    game_cont.lateFrame = game_cont.earlyFrame;
    game_cont.earlyFrame = SDL_GetTicks();
    CalculateDeltaTime(&game_cont);
    sec += game_cont.deltaTime;


    UpdateInput(&inputState);
    while(SDL_PollEvent(&event)){
      Input_ProcessEvent(&event, &inputState); 
    }

       // Now use Unity-style input checking
        if (Input_GetKeyDown(SDL_SCANCODE_SPACE, &inputState)) {
            printf("Space key pressed!\n");
        }
        
        if (Input_GetKey(SDL_SCANCODE_W, &inputState)) {
            printf("W key held down\n");
        }
        
        if (Input_GetMouseButtonDown(0, &inputState)) { // Left mouse button
            int x, y;
            Input_GetMousePosition(&inputState, &x, &y);
            printf("Mouse clicked at (%d, %d)\n", x, y);
        }
    SDL_Delay(16);
  }
  FreeGameContext(&game_cont);
  return 0;
}
