#include "../HeaderFiles/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include "../HeaderFiles/game.h"
#include <stdio.h>
#include <string.h>

bool Init_Img(){
    int flags = IMG_INIT_PNG | IMG_INIT_PNG;
    if(IMG_Init(flags & flags) != flags){
        printf("Failed to initialize IMG: %s\n", IMG_GetError());
        return false;
    }
    return true;
}
int Renderer_SetTexture(GameObject *gameObject, const char* path, int layer, int active, SDL_Renderer *renderer){
    if(!gameObject || !path || !renderer){
        printf("Not all necessary fields are included in SetTexture()\n");
        return 1;
    }
    
    Component *texture = GameObj_GetComponent(gameObject, COMPONENT_RENDERER);
    if(!texture){
        printf("The gameObject \"%s\" does not include a renderer\n", gameObject->name);
        return 1;
    } 
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path);
    if(!newTexture){
        printf("Failed loading texture from path: %s\n", path);
        return 1;
    }
    //if the renderer already contains a texture, then replace it
    if(texture->data.renderer.texture.texture){
        SDL_DestroyTexture(texture->data.renderer.texture.texture);
    }
    
    texture->data.renderer.texture.texture = newTexture;
    
    if(!texture->data.renderer.texture.textureRect){
        texture->data.renderer.texture.textureRect = malloc(sizeof(SDL_Rect));
    }
   //center the texture position to match the coordonates 
    texture->data.renderer.texture.textureRect->x = gameObject->transform->data.transform.x - (gameObject->transform->data.transform.scaleX / 2);
    texture->data.renderer.texture.textureRect->y = gameObject->transform->data.transform.y - (gameObject->transform->data.transform.scaleY / 2);
    texture->data.renderer.texture.textureRect->w = gameObject->transform->data.transform.scaleX;
    texture->data.renderer.texture.textureRect->h = gameObject->transform->data.transform.scaleY;
    
    texture->data.renderer.texture.active = active;
    texture->data.renderer.texture.layer = layer;
    
    return 0;
}
void Renderer_UpdateTexture(GameObject *gameObject);
void Renderer_DestroyTextures(GameObject *gameObject);
