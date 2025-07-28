#include "audio.h"
#include "input.h"
#include "render.h"
#include "timer.h"
#include "game.h"
#include "maths.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>


int main(){
  InputState inputState = {0};
  AudioManager audioMan = {0};
  InitInput(&inputState);
  Audio_Init(&audioMan);
  Audio_LoadClip(&audioMan,"testRes/meow.mp3", "meow", false);
  Audio_LoadClip(&audioMan, "testRes/meow2.mp3", "meow2", false);
  Audio_LoadClip(&audioMan, "testRes/SampleMusic.mp3", "song", true);
  GameContext game_cont = {0};
  TimeInfo time = {0};
  game_cont.window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
  if(InitGameContext(&game_cont)){
    exit(1);
  }
  Timer newTimer = {0};
  Timer_Set(&newTimer, 5.0f);
  time.earlyFrame = SDL_GetTicks();
  time.lateFrame = 0;
  game_cont.running = 1;
  SDL_Event event;
  Audio_PlayMusic(&audioMan, "song", 1.0f, true);
  while(game_cont.running){
    //delta time calculation
    time.lateFrame = time.earlyFrame;
    time.earlyFrame = SDL_GetTicks();
    Time_CalculateDeltaTime(&time);
    Time_CalculateGlobalTime(&time);

    UpdateInput(&inputState);
    while(SDL_PollEvent(&event)){
      Input_ProcessEvent(&event, &inputState); 
    }

 
    if(Input_GetKeyDown(SDL_SCANCODE_P, &inputState)){
      printf("Rasp: %.3f\n",Vec2_Dot((Vector2){-20.0f, -30.0f}, (Vector2){-5.0f, -2.0f}));
    }


    if(Input_ShouldQuit(&inputState)){
     game_cont.running = 0; 
    }
    SDL_Delay(16);
  }
  Audio_Quit(&audioMan);
  FreeGameContext(&game_cont);
  return 0;
}
