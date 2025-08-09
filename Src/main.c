#include "../HeaderFiles/engine.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>


SceneManager sceneMan = {0};

int main(){
  EngineConfig engineConfig = {
    .windowName = "TestGame",
    .width = 900,
    .height = 700,
    .bgColor = (Color){20,60,0,255},
    .masterVolume = 1,
    .windowFlags = 0,
    .vsync = false,
  };
  
  EngineCore engineCore = {0};
  
  if(Engine_Initialize(&engineConfig, &engineCore) == INIT_FAILED){
    printf("Failed to initialize\n");
    exit(1);
  }
  
  engineCore.sceneManager = &sceneMan;
  
  Audio_LoadClip(&engineCore.audioManager,"../NonEssentials/testRes/meow.mp3", "meow", false);
  Audio_LoadClip(&engineCore.audioManager, "../NonEssentials/testRes/meow2.mp3", "meow2", false);
  Audio_LoadClip(&engineCore.audioManager, "../NonEssentials/testRes/SampleMusic.mp3", "song", true);

  Scene *mainScene = Scene_CreateScene(&sceneMan, "main scene");
  SceneManager_LoadScene(&sceneMan, "main scene");

  engineCore.timeInfo.earlyFrame = SDL_GetTicks();
  engineCore.timeInfo.lateFrame = 0;
  engineCore.isRunning = 1;
  
  Audio_PlayMusic(&engineCore.audioManager, "song", 1.0f, true); 

  while(engineCore.isRunning){
    engineCore.timeInfo.lateFrame = engineCore.timeInfo.earlyFrame;
    engineCore.timeInfo.earlyFrame = SDL_GetTicks();
    Time_CalculateDeltaTime(&engineCore.timeInfo);
    Time_CalculateGlobalTime(&engineCore.timeInfo);
    
    UpdateInput(&engineCore.inputManager);
    
    while(SDL_PollEvent(&engineCore.inputEvent)){
      Input_ProcessEvent(&engineCore.inputEvent, &engineCore.inputManager); 
    }

    if(Input_GetMouseButtonDown(0, &engineCore.inputManager)){
      
      GameObject *point = GameObj_Create("PointObj");

      Component *rendererComp = GameObj_AddComponent(point, COMPONENT_RENDERER);
      
      Component *transform = GameObj_GetComponent(point, COMPONENT_TRANSFORM);
   
      int x = 0, y = 0;
      Input_GetMousePosition(&engineCore.inputManager, &x, &y);
      
 
      transform->data.transform.x = (float)x;
      transform->data.transform.y = (float)y;
      transform->data.transform.rotation = 0.0f;
      transform->data.transform.scaleX = 50.0f; 
      transform->data.transform.scaleY = 50.0f;
      printf("transform set to position (%d, %d)\n", x, y);
      
      int textureResult = Renderer_SetTexture(point, "../NonEssentials/Textures/circle.png", 
                                            1, 1, 0.0f, SDL_FLIP_NONE, 
                                            engineCore.gameContext.renderer);
      bool addResult = Scene_AddGameObject(&sceneMan, point);
      Audio_PlayOneShot(&engineCore.audioManager, "meow", 0.3f); 
      GameObj_Start(point);
    }
   
    SDL_SetRenderDrawColor(engineCore.gameContext.renderer, 
                          engineCore.engineConfig.bgColor.r,
                          engineCore.engineConfig.bgColor.g, 
                          engineCore.engineConfig.bgColor.b,
                          engineCore.engineConfig.bgColor.a);
    
    RenderScene(engineCore.gameContext.renderer, &sceneMan);
    
    if(Input_ShouldQuit(&engineCore.inputManager)){
      engineCore.isRunning = 0; 
    }
    
    SDL_Delay(16);
  }
  
  Engine_Shutdown(&engineCore);
  return 0;
}
