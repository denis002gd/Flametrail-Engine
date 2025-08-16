#include "../HeaderFiles/render.h"
#include "../HeaderFiles/camera.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include "../HeaderFiles/game.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

bool Init_Img(){
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(IMG_Init(flags) != flags){
        printf("Failed to initialize IMG: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

int Renderer_SetTexture(GameObject *gameObject, SDL_Texture *texture, int layer, int active,
                double angle, SDL_RendererFlip flip, SDL_Renderer *renderer){
    if(!gameObject || !texture || !renderer){
        printf("Invalid parameters in Renderer_SetTexture()\n");
        return 1;
    }
    
    Component *rendererComp = GameObj_GetComponent(gameObject, COMPONENT_RENDERER);
    if(!rendererComp){
        printf("GameObject \"%s\" does not have a renderer component\n", gameObject->name);
        return 1;
    } 
    
    rendererComp->data.renderer.texture.texture = texture;
    rendererComp->data.renderer.texture.flip = flip;
    rendererComp->data.renderer.texture.angle = angle;
    rendererComp->data.renderer.texture.active = active;
    rendererComp->data.renderer.texture.layer = layer;
    
    // update texture position based on current transform
    Renderer_UpdateTexture(gameObject);
    
    return 0;
}

void Renderer_UpdateTexture(GameObject *gameObject){
    if(!gameObject) return;
    
    Component *rendererComp = GameObj_GetComponent(gameObject, COMPONENT_RENDERER);
    Component *transformComp = GameObj_GetComponent(gameObject, COMPONENT_TRANSFORM);
    
    if(!rendererComp || !transformComp) return;
    
    // update texture rectangle based on transform
    if(rendererComp->data.renderer.texture.textureRect) {
        rendererComp->data.renderer.texture.textureRect->x = 
            (int)(transformComp->data.transform.x - (transformComp->data.transform.scaleX / 2));
        rendererComp->data.renderer.texture.textureRect->y = 
            (int)(transformComp->data.transform.y - (transformComp->data.transform.scaleY / 2));
        rendererComp->data.renderer.texture.textureRect->w = 
            (int)transformComp->data.transform.scaleX;
        rendererComp->data.renderer.texture.textureRect->h = 
            (int)transformComp->data.transform.scaleY;
    }
    
    // update rotation center
    if(rendererComp->data.renderer.texture.center) {
        rendererComp->data.renderer.texture.center->x = 
            (int)(transformComp->data.transform.scaleX / 2);
        rendererComp->data.renderer.texture.center->y = 
            (int)(transformComp->data.transform.scaleY / 2);
    }
    
    rendererComp->data.renderer.texture.angle = transformComp->data.transform.rotation;
}

void Renderer_DestroyTextures(GameObject *gameObject){
    if(!gameObject) return;
    
    Component *rendererComp = GameObj_GetComponent(gameObject, COMPONENT_RENDERER);
    if(!rendererComp) return;
      if(rendererComp->data.renderer.texture.texture) {
        SDL_DestroyTexture(rendererComp->data.renderer.texture.texture);
        rendererComp->data.renderer.texture.texture = NULL;
    }
}
//function for comparing layers
static int CompareRenderLayers(const void *a, const void *b) {
    GameObject *objA = *(GameObject**)a;
    GameObject *objB = *(GameObject**)b;
    
    if(!objA || !objB) return 0;
    
    Component *renderA = GameObj_GetComponent(objA, COMPONENT_RENDERER);
    Component *renderB = GameObj_GetComponent(objB, COMPONENT_RENDERER);
    
    if(!renderA || !renderB) return 0;
    
    return renderA->data.renderer.texture.layer - renderB->data.renderer.texture.layer;
}

void RenderScene(SDL_Renderer *renderer, SceneManager *sceneManager, Camera *camera){
    if(!renderer || !sceneManager || !camera) return;
    
    Scene *currentScene = sceneManager->activeScene;
    if(!currentScene || !currentScene->isActive) {
        return; 
    }
    
    GameObject *renderableObjects[MAX_GAMEOBJECTS];
    int renderableCount = 0;
    
    for(int i = 0; i < currentScene->gameObjectsCount; i++){
        GameObject *obj = currentScene->objects[i];
        if(!obj || !obj->isActive) continue;
        
        Component *renderComp = GameObj_GetComponent(obj, COMPONENT_RENDERER);
        if(renderComp && renderComp->isEnabled && 
           renderComp->data.renderer.texture.active &&
           renderComp->data.renderer.texture.texture) {
            
            renderableObjects[renderableCount++] = obj;
        }
    }
    
    // sort by render layers
    qsort(renderableObjects, renderableCount, sizeof(GameObject*), CompareRenderLayers);
    
    // render each object with camera transformations
    for(int i = 0; i < renderableCount; i++){
        GameObject *obj = renderableObjects[i];
        Component *renderComp = GameObj_GetComponent(obj, COMPONENT_RENDERER);
        Component *transformComp = GameObj_GetComponent(obj, COMPONENT_TRANSFORM);
        
        if(!renderComp || !transformComp) continue;
        
        Vector2 worldPos = {
            transformComp->data.transform.x,
            transformComp->data.transform.y
        };
        
        SDL_FRect worldRect = {
            worldPos.x - (transformComp->data.transform.scaleX / 2),
            worldPos.y - (transformComp->data.transform.scaleY / 2),
            transformComp->data.transform.scaleX,
            transformComp->data.transform.scaleY
        };
        
        if (!Camera_IsPointVisible(camera, worldPos)) {
            Vector2 corners[4] = {
                {worldRect.x, worldRect.y},                           // top-left
                {worldRect.x + worldRect.w, worldRect.y},             // top-right
                {worldRect.x, worldRect.y + worldRect.h},             // bottom-left
                {worldRect.x + worldRect.w, worldRect.y + worldRect.h} // bottom-right
            };
            
            bool anyVisible = false;
            for(int c = 0; c < 4; c++) {
                if(Camera_IsPointVisible(camera, corners[c])) {
                    anyVisible = true;
                    break;
                }
            }
            
            if(!anyVisible) continue;
        }
        
        SDL_Rect screenRect = Camera_WorldRectToScreen(camera, worldRect);
        
        SDL_Point rotationCenter = {
            (int)(screenRect.w / 2),
            (int)(screenRect.h / 2)
        };
        
        SDL_RenderCopyEx(
            renderer, 
            renderComp->data.renderer.texture.texture, 
            NULL,  
            &screenRect,
            transformComp->data.transform.rotation,
            &rotationCenter,
            renderComp->data.renderer.texture.flip
        );
    }
}
bool TextRender_Initialize(TextRender *textRenderer, SDL_Renderer *renderer){
    if(!textRenderer || !renderer) return false;

    if(TTF_Init() == -1){
        printf("Failed to initialize TTF\n");
        return false;
    }
    memset(textRenderer, 0, sizeof(TextRender));
    textRenderer->renderer = renderer;
    textRenderer->isInitialized = true;
    textRenderer->fontCount = 0;
    return true;
}
FontData *TextRenderer_LoadFont(TextRender *textRenderer, const char *fontPath, int fontSize){
    if(!textRenderer || !fontPath || fontSize <= 0){
        return NULL;
    }
    if(textRenderer->fontCount >= 10){
        printf("Max number of fonts reached\n");
        return NULL;
    }
    FontData *fontData = malloc(sizeof(FontData));
    if(!fontData) return NULL;
    fontData->font = TTF_OpenFont(fontPath, fontSize);
    if(!fontData->font){
        printf("Failed to load font, %s\n", TTF_GetError());
        return NULL;
    }
    fontData->fontSize = fontSize;
    fontData->color = (SDL_Color){255,255,255,255};
    strncpy(fontData->path, fontPath, sizeof(fontData->path) - 1);
    fontData->path[sizeof(fontData->path) - 1] = '\0';

    textRenderer->fonts[textRenderer->fontCount] = fontData;
    textRenderer->fontCount++;
    return fontData;
}
SDL_Texture *TextRender_CreateTextTexture(TextRender *textRender, FontData *fontData, const char *format, ...){
    if(!textRender || !fontData) return NULL;
     char buffer[MAX_TEXT_CHARACTERS];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    SDL_Surface *textSurface = TTF_RenderText_Solid(fontData->font, buffer, fontData->color);
    if(!textSurface){
        printf("Failed to make the text surface\n");
        return NULL;
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(textRender->renderer, textSurface);
    if(!textTexture){
        printf("Failed to make the text texture\n");
        return NULL;
    }
    SDL_FreeSurface(textSurface);
    return textTexture;
}
void TextRender_RenderText(TextRender *textRenderer, FontData *font, SDL_Rect *destRect ,const char *format, ...){
     if (!textRenderer || !font || !destRect || !format) return;
    
    char buffer[MAX_TEXT_CHARACTERS];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    SDL_Texture *textTexture = TextRender_CreateTextTexture(textRenderer, font, "%s", buffer);
    if (!textTexture) return;
    
    if (destRect->w == 0 || destRect->h == 0) {
        SDL_QueryTexture(textTexture, NULL, NULL, &destRect->w, &destRect->h);
    }
    
    SDL_RenderCopy(textRenderer->renderer, textTexture, NULL, destRect);
    
    SDL_DestroyTexture(textTexture);
}
void TextRender_RenderTextColored(TextRender *textRenderer, FontData *font, SDL_Rect *destRect, SDL_Color color, const char *format, ...){
    if (!textRenderer || !font || !destRect || !format) return;
    
    char buffer[MAX_TEXT_CHARACTERS];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    SDL_Color originalColor = font->color;
    font->color = color;
    
    TextRender_RenderText(textRenderer, font, destRect, "%s", buffer);
    
    font->color = originalColor;
}
void TextRenderer_GetTextSize(FontData *font, int *width, int *height, const char *format, ...) {
    if (!font || !format) return;
    
    char buffer[MAX_TEXT_CHARACTERS];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    TTF_SizeText(font->font, buffer, width, height);
}

void TextRenderer_DestroyFont(FontData *font) {
    if (!font) return;
    
    if (font->font) {
        TTF_CloseFont(font->font);
    }
    free(font);
}

void TextRenderer_Cleanup(TextRender *textRenderer) {
    if (!textRenderer) return;
    
    for (int i = 0; i < textRenderer->fontCount; i++) {
        TextRenderer_DestroyFont(textRenderer->fonts[i]);
    }
    
    if (textRenderer->isInitialized) {
        TTF_Quit();
    }
    
    memset(textRenderer, 0, sizeof(TextRender));
}
void LineRender_RenderArrow(SDL_Renderer *renderer, Vector2 origin, Vector2 target, SDL_Color arrowColor, Camera *camera){
    if(!renderer || !camera) return;
    
    // Convert world coordinates to screen coordinates
    Vector2 screenOrigin = Camera_WorldToScreen(camera, origin);
    Vector2 screenTarget = Camera_WorldToScreen(camera, target);
    
    SDL_SetRenderDrawColor(renderer, arrowColor.r, arrowColor.g, arrowColor.b, arrowColor.a);
    SDL_RenderDrawLineF(renderer, screenOrigin.x, screenOrigin.y, screenTarget.x, screenTarget.y);
    
    // Calculate arrow head
    Vector2 direction = Vec2_Sub(target, origin);
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    
    if(length > 0) {
        // Normalize direction
        direction.x /= length;
        direction.y /= length;
        
        // Arrow head size scaled by camera zoom
        float arrowHeadSize = 10.0f * camera->zoom;
        
        // Arrow head points
        Vector2 arrowHead1 = {
            target.x - direction.x * arrowHeadSize + direction.y * arrowHeadSize * 0.5f,
            target.y - direction.y * arrowHeadSize - direction.x * arrowHeadSize * 0.5f
        };
        
        Vector2 arrowHead2 = {
            target.x - direction.x * arrowHeadSize - direction.y * arrowHeadSize * 0.5f,
            target.y - direction.y * arrowHeadSize + direction.x * arrowHeadSize * 0.5f
        };
        
        // Convert arrow head points to screen coordinates
        Vector2 screenArrowHead1 = Camera_WorldToScreen(camera, arrowHead1);
        Vector2 screenArrowHead2 = Camera_WorldToScreen(camera, arrowHead2);
        
        // Draw arrow head lines
        SDL_RenderDrawLineF(renderer, screenTarget.x, screenTarget.y, screenArrowHead1.x, screenArrowHead1.y);
        SDL_RenderDrawLineF(renderer, screenTarget.x, screenTarget.y, screenArrowHead2.x, screenArrowHead2.y);
    }
}
