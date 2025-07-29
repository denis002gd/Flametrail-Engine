#include "../HeaderFiles/render.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

int InitGameContext(GameContext *Game_Context){

  if(!Game_Context->window){
    fprintf(stderr, "You need to create a window before using InitGameContext(), Error: %s\n", SDL_GetError());
    return 1;
  }

  Game_Context->renderer = SDL_CreateRenderer(Game_Context->window, 0, 0);

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
