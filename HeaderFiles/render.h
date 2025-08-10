#ifndef RENDER_H
#define RENDER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
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
 double angle;
 SDL_Point *center;
 SDL_RendererFlip flip;
}Texture;

void RenderScene(SDL_Renderer *renderer, SceneManager *sceneManager);

bool Init_Img();
int Renderer_SetTexture(GameObject *gameObject, SDL_Texture *texture, int layer, int active,
                double angle, SDL_RendererFlip flip,  SDL_Renderer *renderer);
void Renderer_UpdateTexture(GameObject *gameObject);
void Renderer_DestroyTextures(GameObject *gameObject);
#endif // !RENDER_H
