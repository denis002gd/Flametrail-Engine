#include "../HeaderFiles/engine.h"
#include <stdio.h>


SceneManager sceneMan = {0};

int main(){
  EngineConfig engineConfig = {
    .windowName = "TestGame",
    .width = 900,
    .height = 700,
    .bgColor = (Color){0,0,0,255},
    .masterVolume = 1,
    .windowFlags = 0,
    .vsync = false,
  };
  EngineCore engineCore = {0};
  //this already shuts down the engine
  if(Engine_Initialize(&engineConfig, &engineCore) == INIT_FAILED){
    printf("Falied to initialize\n");
    exit(1);
  }

  Audio_LoadClip(&engineCore.audioManager,"../NonEssentials/testRes/meow.mp3", "meow", false);
  Audio_LoadClip(&engineCore.audioManager, "../NonEssentials/testRes/meow2.mp3", "meow2", false);
  Audio_LoadClip(&engineCore.audioManager, "../NonEssentials/testRes/SampleMusic.mp3", "song", true);

  //TODO: add an abstraction layer for calculating time
  engineCore.timeInfo.earlyFrame = SDL_GetTicks();
  engineCore.timeInfo.lateFrame = 0;
  engineCore.isRunning = 1;
  Audio_PlayMusic(&engineCore.audioManager, "song", 1.0f, true); 

  while(engineCore.isRunning){
    //delta time calculation
    engineCore.timeInfo.lateFrame = engineCore.timeInfo.earlyFrame;
    engineCore.timeInfo.earlyFrame = SDL_GetTicks();
    Time_CalculateDeltaTime(&engineCore.timeInfo);
    Time_CalculateGlobalTime(&engineCore.timeInfo);
    UpdateInput(&engineCore.inputManager);
    while(SDL_PollEvent(&engineCore.inputEvent)){
      Input_ProcessEvent(&engineCore.inputEvent, &engineCore.inputManager); 
    }
//////////////////////////Add code here/////////////////////////////////

    if(Input_GetKeyDown(SDL_SCANCODE_1, &engineCore.inputManager)){
  
    }
   
////////////////////////////////////////////////////////////////////////

    if(Input_ShouldQuit(&engineCore.inputManager)){
     engineCore.isRunning = 0; 
    }
    SDL_Delay(16);
  }
  Engine_Shutdown(&engineCore);
  return 0;
}
