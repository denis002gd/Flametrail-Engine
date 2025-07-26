#ifndef RENDER_H
#define RENDER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

typedef struct Game{
  SDL_Window *window;
  SDL_Renderer *renderer;
  
  Uint64 earlyFrame;
  Uint64 lateFrame;
  Uint64 elapsedTime;
  double deltaTime;
  int running;
}GameContext;

int InitGameContext(GameContext *Game_Context);
void FreeGameContext(GameContext *Game_Context);

#endif // !RENDER_H
