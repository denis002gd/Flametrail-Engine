#include "render.h"
#include "timer.h"
#include "game.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>


int main(){
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
    game_cont.lateFrame = game_cont.earlyFrame;
    game_cont.earlyFrame = SDL_GetTicks();
    CalculateDeltaTime(&game_cont);
    sec += game_cont.deltaTime;
    while(SDL_PollEvent(&event)){
      switch (event.type) {
        case SDL_QUIT:
        game_cont.running = 0;
      
      }
    }
    printf("Secunde de la inceput: %lf\n", sec * 1000);
    SDL_Delay(16);
  }
  FreeGameContext(&game_cont);
  return 0;
}
