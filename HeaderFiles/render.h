#ifndef RENDER_H
#define RENDER_H
#include "maths.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define MAX_TEXT_CHARACTERS 1024
#define MAX_FONTPATH_LENGHT 255

typedef struct SceneManager SceneManager;
typedef struct GameObject GameObject;
typedef struct Camera Camera;
typedef struct{
  int active;
  int layer;
  SDL_Texture *texture;
  SDL_Rect *textureRect;
  double angle;
  SDL_Point *center;
  SDL_RendererFlip flip;
}Texture;

typedef struct{
  TTF_Font *font;
  SDL_Color color;
  int fontSize;
 char path[MAX_FONTPATH_LENGHT];
}FontData;

typedef struct{
  FontData *fonts[10];
  int fontCount;
  SDL_Renderer *renderer;
  bool isInitialized;
}TextRender;

void RenderScene(SDL_Renderer *renderer, SceneManager *sceneManager, Camera *camera);

bool Init_Img();
int Renderer_SetTexture(GameObject *gameObject, SDL_Texture *texture, int layer, int active,
                double angle, SDL_RendererFlip flip,  SDL_Renderer *renderer);
void Renderer_UpdateTexture(GameObject *gameObject);
void Renderer_DestroyTextures(GameObject *gameObject);

//+=+=+=+=+=+=+=FontsRenderFunctions+=+=+=+=+=+=+=+
bool TextRender_Initialize(TextRender *textRenderer, SDL_Renderer *renderer);
FontData *TextRenderer_LoadFont(TextRender *textRenderer, const char *fontPath, int fontSize);
SDL_Texture *TextRender_CreateTextTexture(TextRender *textRender, FontData *fontData, const char *format, ...);
void TextRender_RenderText(TextRender *textRenderer, FontData *font, SDL_Rect *destRect ,const char *format, ...);
void TextRender_RenderTextColored(TextRender *textRenderer, FontData *font, SDL_Rect *destRect, SDL_Color color, const char *format, ...);
void TextRender_GetTextSize(FontData *font, int *width, int *height, const char *format, ...);
void TextRender_DestroyFont(FontData *font);
void TextRender_Cleanup(TextRender *textRender);
void LineRender_RenderArrow(SDL_Renderer *renderer, Vector2 origin, Vector2 target, SDL_Color arrowColor, Camera *camera);
#endif // !RENDER_H
