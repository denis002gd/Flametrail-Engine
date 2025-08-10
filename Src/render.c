#include "../HeaderFiles/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include "../HeaderFiles/game.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
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

void RenderScene(SDL_Renderer *renderer, SceneManager *sceneManager){
    if(!renderer || !sceneManager) return;
    
    SDL_RenderClear(renderer);
    
    Scene *currentScene = sceneManager->activeScene;
    if(!currentScene || !currentScene->isActive) {
        SDL_RenderPresent(renderer);
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
            
            Renderer_UpdateTexture(obj);
            renderableObjects[renderableCount++] = obj;
        }
    }
    
    qsort(renderableObjects, renderableCount, sizeof(GameObject*), CompareRenderLayers);
    
    for(int i = 0; i < renderableCount; i++){
        Component *renderComp = GameObj_GetComponent(renderableObjects[i], COMPONENT_RENDERER);
        if(renderComp) {
            SDL_RenderCopyEx(
                renderer, 
                renderComp->data.renderer.texture.texture, 
                NULL, 
                renderComp->data.renderer.texture.textureRect,
                renderComp->data.renderer.texture.angle,  
                renderComp->data.renderer.texture.center, 
                renderComp->data.renderer.texture.flip
            );
        }
    }
    
    SDL_RenderPresent(renderer);
}
