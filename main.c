#include "audio.h"
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
  AudioManager audioMan = {0};
  InitInput(&inputState);
  double sec = 0;
  Audio_Init(&audioMan);
  Audio_LoadClip(&audioMan,"testRes/meow.mp3", "meow", false);
  Audio_LoadClip(&audioMan, "testRes/meow2.mp3", "meow2", false);
  Audio_LoadClip(&audioMan, "testRes/SampleMusic.mp3", "song", true);
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
  float masterAudio = 1.0f;
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

        if (Input_GetKeyDown(SDL_SCANCODE_1, &inputState)) {
            printf("Cat one is playing!\n");
            Audio_PlayOneShot(&audioMan, "meow", 1.0f);
        }
        if (Input_GetKeyDown(SDL_SCANCODE_2, &inputState)) {
            printf("Cat two is playing!\n");
            Audio_PlayOneShot(&audioMan, "meow2", 1.0f);
        }
        if(Input_GetKeyDown(SDL_SCANCODE_R, &inputState)){
          Audio_StopMusic();
          printf("Music Stopped\n");
        }
    if(Input_GetKeyDown(SDL_SCANCODE_P, &inputState)){
          Audio_PlayMusic(&audioMan, "song", 0.8f, true);
          printf("Is Playing Music\n");
        }
  
        if (Input_GetKeyDown(SDL_SCANCODE_A, &inputState)) {
            masterAudio -= 0.1f;
            printf("Volume: %.1f\n", masterAudio);
            Audio_SetMasterVolume(&audioMan, masterAudio);
        }
if (Input_GetKeyDown(SDL_SCANCODE_D, &inputState)) {
            masterAudio += 0.1f;
            printf("Volume: %.1f\n", masterAudio);
            Audio_SetMasterVolume(&audioMan, masterAudio);
        }
        
              game_cont.running = !Input_ShouldQuit(&inputState);
    SDL_Delay(16);
  }
  Audio_Quit(&audioMan);
  FreeGameContext(&game_cont);
  return 0;
}
