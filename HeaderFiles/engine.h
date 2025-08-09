#ifndef ENGINE_H
#define ENGINE_H

#include "game.h"
#include "input.h"
#include "UI.h"
#include "audio.h"
#include "entity.h"
#include "maths.h"
#include "physics.h"
#include "render.h"
#include "timer.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

typedef struct GameObject GameObject;

typedef struct{
  bool sdl_initialized;
  bool sceneManager_initialized;
  bool img_initialized;
  bool mix_initialized;
  bool ttf_initialized;
  bool renderer_initialized;
}EngineState;

typedef enum{
  INIT_SUCCESS,
  INIT_FAILED,
  INIT_SDL_FAILED,
  INIT_IMG_FAILED,
  INIT_MIX_FAILED,
  INIT_TTF_FAILED,
  INIT_RENDERER_FAILED,
}InitResult;

typedef struct{
 
 SDL_Window *window;
 SDL_Renderer *renderer;
}GameContext;

typedef struct{
 Uint8 r, g, b, a;
}Color;

typedef struct{
 const char* windowName;
 int width;
 int height;
 Uint32 windowFlags;
 SDL_RendererFlags rendererFlags;
 bool vsync;
 Color bgColor;
 float masterVolume;
}EngineConfig;

typedef struct{
  bool isRunning;

  EngineConfig engineConfig;
   
  SDL_Event inputEvent;

  GameContext gameContext;
  SceneManager *sceneManager;
  AudioManager audioManager;
  InputState inputManager;
  TimeInfo timeInfo;

  bool isInitialized;
  bool shouldQuit;

  EngineState engineState;
  
}EngineCore;
//initialization of all SDL utilities, engine confing determines the window layout, engine core can be an empty struct
InitResult Engine_Initialize(EngineConfig *engineConfig, EngineCore *engineCore);

void Engine_Shutdown(EngineCore *engineCore);

bool InitGameContext(GameContext *Game_Context);
void FreeGameContext(GameContext *Game_Context);
#endif // !ENGINE_H
