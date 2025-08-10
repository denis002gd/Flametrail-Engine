#include "../HeaderFiles/engine.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>

SceneManager sceneMan = {0};
SDL_Texture *circleTexture = NULL;
SDL_Texture *squareTexture = NULL;
int objectCounter = 0;

int main() {
    EngineConfig engineConfig = {
        .windowName = "Test",
        .width = 900,
        .height = 700,
        .bgColor = (Color){30, 30, 50, 255},
        .masterVolume = 0.5f,
        .windowFlags = 0,
        .vsync = false,
    };
    
    EngineCore engineCore = {0};
    
    if (Engine_Initialize(&engineConfig, &engineCore) == INIT_FAILED) {
        return 1;
    }
    
    engineCore.sceneManager = &sceneMan;
    
    circleTexture = IMG_LoadTexture(engineCore.gameContext.renderer, "../NonEssentials/Textures/circle.png");
    squareTexture = IMG_LoadTexture(engineCore.gameContext.renderer, "../NonEssentials/Textures/square.png");
    
    if (!squareTexture) {
        Engine_Shutdown(&engineCore);
        return 1;
    }
    
    if (!circleTexture) {
        circleTexture = squareTexture;
    }
    
    Scene_CreateScene(&sceneMan, "main");
    SceneManager_LoadScene(&sceneMan, "main");
    
    engineCore.isRunning = true;
    
    while (engineCore.isRunning) {
        Time_Update(&engineCore.timeInfo);
        UpdateInput(&engineCore.inputManager);
        
        while (SDL_PollEvent(&engineCore.inputEvent)) {
            Input_ProcessEvent(&engineCore.inputEvent, &engineCore.inputManager);
        }
        
        if (Input_GetMouseButton(0, &engineCore.inputManager)) {
            int x, y;
            Input_GetMousePosition(&engineCore.inputManager, &x, &y);
            
            char name[32];
            snprintf(name, sizeof(name), "circle_%d", objectCounter++);
            
            GameObject *obj = GameObj_Create(name);
            GameObj_AddComponent(obj, COMPONENT_RENDERER);
            
            Component *transform = GameObj_GetComponent(obj, COMPONENT_TRANSFORM);
            transform->data.transform.x = (float)x;
            transform->data.transform.y = (float)y;
            transform->data.transform.scaleX = 64.0f;
            transform->data.transform.scaleY = 64.0f;
            
            Renderer_SetTexture(obj, circleTexture, 1, 1, 0.0f, SDL_FLIP_NONE, engineCore.gameContext.renderer);
            Scene_AddGameObject(&sceneMan, obj);
            GameObj_Start(obj);
        }
        
        if (Input_GetMouseButton(2, &engineCore.inputManager)) {
            int x, y;
            Input_GetMousePosition(&engineCore.inputManager, &x, &y);
            
            char name[32];
            snprintf(name, sizeof(name), "square_%d", objectCounter++);
            
            GameObject *obj = GameObj_Create(name);
            GameObj_AddComponent(obj, COMPONENT_RENDERER);
            
            Component *transform = GameObj_GetComponent(obj, COMPONENT_TRANSFORM);
            transform->data.transform.x = (float)x;
            transform->data.transform.y = (float)y;
            transform->data.transform.scaleX = 64.0f;
            transform->data.transform.scaleY = 64.0f;
            
            Renderer_SetTexture(obj, squareTexture, 1, 1, 45.0f, SDL_FLIP_NONE, engineCore.gameContext.renderer);
            Scene_AddGameObject(&sceneMan, obj);
            GameObj_Start(obj);
        }
        
        if (sceneMan.activeScene) {
            SceneManager_Update(&sceneMan, engineCore.timeInfo.deltaTime);
        }
        
        SDL_SetRenderDrawColor(engineCore.gameContext.renderer, 
                              engineCore.engineConfig.bgColor.r,
                              engineCore.engineConfig.bgColor.g, 
                              engineCore.engineConfig.bgColor.b,
                              engineCore.engineConfig.bgColor.a);
        
        RenderScene(engineCore.gameContext.renderer, &sceneMan);
        
        if (Input_ShouldQuit(&engineCore.inputManager)) {
            engineCore.isRunning = false;
        }
        
        SDL_Delay(16);
    }
    
    Engine_Shutdown(&engineCore);
    return 0;
}
