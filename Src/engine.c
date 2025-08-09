#include "../HeaderFiles/engine.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

bool InitGameContext(GameContext *Game_Context){

  if(!Game_Context->window){
    fprintf(stderr, "You need to create a window before using InitGameContext(), Error: %s\n", SDL_GetError());
    return 1;
  }

  if(!Game_Context->renderer){
    fprintf(stderr, "Failed at initializing renderer, Error: %s\n", SDL_GetError());
    return 1;
  }
  return 0;
}

void FreeGameContext(GameContext *Game_Context){
  SDL_DestroyRenderer(Game_Context->renderer);
  SDL_DestroyWindow(Game_Context->window);
}
void Engine_Shutdown(EngineCore *engineCore){
   FreeGameContext(&engineCore->gameContext);
   if(engineCore->engineState.img_initialized){
    IMG_Quit();
   }
   if(engineCore->engineState.mix_initialized){
    Mix_Quit();
   }
   if(engineCore->engineState.sdl_initialized){
    SDL_Quit();
   }
   
}
bool Engine_VerifyEngineCore(EngineCore *engineCore){
  //these functions return false on fail
  if(engineCore->engineState.img_initialized && engineCore->engineState.mix_initialized && engineCore->engineState.sdl_initialized){
    return true;
  }
  return false;
}
InitResult Engine_Initialize(EngineConfig *engineConfig, EngineCore *engineCore){
  if(!engineConfig || !engineCore){
    printf("Invalid erguments in function Engine_Initialize()\n");
    Engine_Shutdown(engineCore);
    return INIT_FAILED;
  }
   engineCore->gameContext.window = SDL_CreateWindow(engineConfig->windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
                                                    ,engineConfig->width, engineConfig->height, engineConfig->windowFlags);
  if(!engineCore->gameContext.window){
    printf("Failed to create a window\n");
    Engine_Shutdown(engineCore);
    return INIT_FAILED;
  }
  engineCore->gameContext.renderer = SDL_CreateRenderer(engineCore->gameContext.window, 0, engineConfig->rendererFlags);
  if(!engineCore->gameContext.renderer){
    printf("Failed to create a renderer\n");
    Engine_Shutdown(engineCore);
    return INIT_FAILED;
  }

  engineCore->engineState.sdl_initialized = (InitGameContext(&engineCore->gameContext) == 0);
  engineCore->engineState.img_initialized = Init_Img(); 
  engineCore->engineState.mix_initialized = Audio_Init(&engineCore->audioManager);

  if(!Engine_VerifyEngineCore(engineCore)){
    printf("Failed to initialize tools\n");
    Engine_Shutdown(engineCore);
    return INIT_FAILED;
  }
  engineCore->shouldQuit = false;
  engineCore->isInitialized = true;
  engineCore->isRunning = true; 
  return INIT_SUCCESS;
}
