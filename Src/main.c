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
    
 FontData *gameFont = TextRenderer_LoadFont(&engineCore.textRenderer, "../Extras/Fonts/Roboto-Bold.ttf", 24); 
 FontData *titleFont = TextRenderer_LoadFont(&engineCore.textRenderer, "../Extras/Fonts/PlaywriteAUQLD-Regular.ttf", 34); 
    Scene_CreateScene(&sceneMan, "main");
    SceneManager_LoadScene(&sceneMan, "main");

    int score = 0;
    int lives = 3;
    float gameTime = 0.0f;
    int objectCount = 0;
    char playerName[] = "Denis";
    engineCore.isRunning = true;
    float rot = 0.0f;



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
        transform->data.transform.scaleX = 20.0f;
        transform->data.transform.scaleY = 20.0f;
        
        Renderer_SetTexture(obj, circleTexture, 1, 1, 0.0f, SDL_FLIP_NONE, engineCore.gameContext.renderer);
        Scene_AddGameObject(&sceneMan, obj);
        GameObj_Start(obj);
        
        score += 10;
        objectCount++; 
    }
    
    rot += 0.1f;
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
        transform->data.transform.scaleX = 20.0f;
        transform->data.transform.scaleY = 20.0f;
        transform->data.transform.rotation = rot;
        Renderer_SetTexture(obj, squareTexture, 1, 1, rot, SDL_FLIP_NONE, engineCore.gameContext.renderer);
        Scene_AddGameObject(&sceneMan, obj);
        GameObj_Start(obj);
        
        score += 15;
        objectCount++; 
    }
    
    if (sceneMan.activeScene) {
        SceneManager_Update(&sceneMan, engineCore.timeInfo.deltaTime);
    }
    
    gameTime += engineCore.timeInfo.deltaTime; 
    
    SDL_SetRenderDrawColor(engineCore.gameContext.renderer, 
                          engineConfig.bgColor.r, 
                          engineConfig.bgColor.g, 
                          engineConfig.bgColor.b,
                          engineConfig.bgColor.a);
    
    SDL_RenderClear(engineCore.gameContext.renderer); 
    
    RenderScene(engineCore.gameContext.renderer, &sceneMan);
    
    if(gameFont) {
        SDL_Rect textRect;
        
        if(titleFont) {
            textRect = (SDL_Rect){300, 50, 0, 0};
            SDL_Color titleColor = {255, 255, 0, 255}; 
            TextRender_RenderTextColored(&engineCore.textRenderer, titleFont, &textRect, 
                                         titleColor, "Game Engine Demo");
        }
        
        textRect = (SDL_Rect){20, 20, 0, 0};
        SDL_Color scoreColor = {0, 255, 0, 255}; 
        TextRender_RenderTextColored(&engineCore.textRenderer, gameFont, &textRect, 
                                     scoreColor, "Score: %d", score);
        
        textRect.y += 30;
        SDL_Color livesColor = {255, 100, 100, 255};
        TextRender_RenderTextColored(&engineCore.textRenderer, gameFont, &textRect, 
                                     livesColor, "Lives: %d", lives);
        
        textRect.y += 30;
        TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, 
                               "Player: %s", playerName);
        
        textRect.y += 30;
        TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, 
                               "Time: %.1f seconds", gameTime);
        
        textRect.y += 30;
        TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, 
                               "Objects Created: %d", objectCount);
        
        textRect = (SDL_Rect){750, 650, 0, 0};
        SDL_Color fpsColor = {200, 200, 200, 255};
        TextRender_RenderTextColored(&engineCore.textRenderer, gameFont, &textRect, 
                                     fpsColor, "FPS: %.1f", engineCore.timeInfo.fps);
        
        textRect = (SDL_Rect){250, 650, 0, 0};
        TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, 
                               "Click: +Score | Space: Reset | ESC: Quit");
        
        if(score > 100) {
            textRect = (SDL_Rect){400, 200, 0, 0};
            SDL_Color bonusColor = {255, 215, 0, 255}; 
            TextRender_RenderTextColored(&engineCore.textRenderer, gameFont, &textRect, 
                                         bonusColor, "HIGH SCORE!");
        }
        
        float health = 100.0f - (gameTime * 2.0f); 
        if(health < 0) health = 0;
        
        textRect = (SDL_Rect){20, 200, 0, 0};
        SDL_Color healthColor;
        if(health > 60) {
            healthColor = (SDL_Color){0, 255, 0, 255};
        } else if(health > 30) {
            healthColor = (SDL_Color){255, 255, 0, 255}; 
        } else {
            healthColor = (SDL_Color){255, 0, 0, 255}; 
        }
        
        TextRender_RenderTextColored(&engineCore.textRenderer, gameFont, &textRect, 
                                     healthColor, "Health: %.0f%%", health);
    }
    
    SDL_RenderPresent(engineCore.gameContext.renderer);
    
    if (Input_ShouldQuit(&engineCore.inputManager)) {
        engineCore.isRunning = false;
    }
    
    SDL_Delay(16);
}
    
    Engine_Shutdown(&engineCore);
    return 0;
}
