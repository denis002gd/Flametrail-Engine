#ifndef RENDER_H
#define RENDER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct SceneManager SceneManager;
typedef struct GameObject GameObject;

typedef struct{
  int active;
  int layer;
  SDL_Texture *texture;
  SDL_Rect *textureRect;
}Texture;

void RenderScene(SDL_Renderer *renderer, SceneManager *sceneManager);

bool Init_Img();
int Renderer_SetTexture(GameObject *gameObject, const char* path, int layer, int active, SDL_Renderer *renderer);
void Renderer_UpdateTexture(GameObject *gameObject);
void Renderer_DestroyTextures(GameObject *gameObject);
#endif // !RENDER_H
