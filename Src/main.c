#include "../HeaderFiles/engine.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

SceneManager sceneMan = {0};
SDL_Texture *circleTexture = NULL;
SDL_Texture *squareTexture = NULL;
int objectCounter = 0;
void UpdatePos(GameObject *gameObj, float t){
    Component *rb = GameObj_GetComponent(gameObj, COMPONENT_RIGIDBODY);
    Physics_AddForce(&rb->data.rigidbody.rigidBody, (Vector2){10.0f, 2000.0f});
    Physics_ApplyGravity(&rb->data.rigidbody.rigidBody, t);
}

int main() {
    EngineConfig engineConfig = {
        .windowName = "Test",
        .width = 900,
        .height = 700,
        .bgColor = (Color){30, 30, 50, 255},
        .masterVolume = 0.5f,
        .windowFlags = SDL_WINDOW_BORDERLESS,
        .vsync = false,
    };
    
   srand(time(NULL)); 
    EngineCore engineCore = {0};
    
    if (Engine_Initialize(&engineConfig, &engineCore) == INIT_FAILED) {
        return 1;
    }
    
    engineCore.camera = Camera_Create(engineConfig.width, engineConfig.height);
    if (!engineCore.camera) {
        Engine_Shutdown(&engineCore);
        return 1;
    }
    
    Camera_SetPosition(engineCore.camera, (Vector2){450, 350});
    Camera_Zoom(engineCore.camera, 1.0f);
    
    Camera_SetBounds(engineCore.camera, (SDL_Rect){0, 0, 1800, 1400});
    
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
    
 FontData *gameFont = TextRenderer_LoadFont(&engineCore.textRenderer, "../Extras/Fonts/Roboto-Bold.ttf", 40); 
 FontData *titleFont = TextRenderer_LoadFont(&engineCore.textRenderer, "../Extras/Fonts/PlaywriteAUQLD-Regular.ttf", 34); 
    Scene_CreateScene(&sceneMan, "Physics Test");
    SceneManager_LoadScene(&sceneMan, "Physics Test");

    Scene_CreateScene(&sceneMan, "Collision Test");
    int score = 0;
    int lives = 3;
    float gameTime = 0.0f;
    float speed = 15.0f;
    int objectCount = 0;
    char playerName[] = "Denis";
    engineCore.isRunning = true;
    float rot = 0.0f; 
        char name[32];
        snprintf(name, sizeof(name), "player_%d", objectCounter++);
        
        GameObject *obj = GameObj_Create(name);
        GameObj_AddComponent(obj, COMPONENT_RENDERER);
        GameObj_AddComponent(obj, COMPONENT_RIGIDBODY);
    Component *rb = GameObj_GetComponent(obj, COMPONENT_RIGIDBODY);
    rb->data.rigidbody.rigidBody.drag = 2;
        Component *transform = GameObj_GetComponent(obj, COMPONENT_TRANSFORM);
        transform->data.transform.x = 500;
        transform->data.transform.y = 500;
        transform->data.transform.scaleX = 100.0f;
        transform->data.transform.scaleY = 100.0f;
        transform->data.transform.rotation = 45.0f;
        Renderer_SetTexture(obj, squareTexture, 1, 1, rot, SDL_FLIP_NONE, engineCore.gameContext.renderer);
        Scene_AddGameObject(&sceneMan, obj);
        GameObj_Start(obj);
       



       GameObject *newObj = GameObj_Create("steve");
        GameObj_AddComponent(newObj, COMPONENT_RENDERER);
        Component *newTransform = GameObj_GetComponent(newObj, COMPONENT_TRANSFORM);
        newTransform->data.transform.x = 500;
        newTransform->data.transform.y = 500;
        newTransform->data.transform.scaleX = 100.0f;
        newTransform->data.transform.scaleY = 100.0f;
        newTransform->data.transform.rotation = rot;
        Renderer_SetTexture(newObj, squareTexture, 1, 1, rot, SDL_FLIP_NONE, engineCore.gameContext.renderer);
        Scene_AddGameObject(&sceneMan, newObj);
        GameObj_Start(newObj);


        score += 15;
        objectCount++; 
        Vector2 forceVector = {0, 0};
SDL_Rect textRect = {10, 10, 170, 30};
while (engineCore.isRunning) {
    Time_Update(&engineCore.timeInfo);
    UpdateInput(&engineCore.inputManager);
    
    while (SDL_PollEvent(&engineCore.inputEvent)) {
        Input_ProcessEvent(&engineCore.inputEvent, &engineCore.inputManager);
    }
        Camera_Update(engineCore.camera, engineCore.timeInfo.deltaTime);
    
    if (Input_GetKey(SDL_SCANCODE_UP, &engineCore.inputManager)) {
        Vector2 pos = engineCore.camera->position;
        pos.y -= 200.0f * engineCore.timeInfo.deltaTime;
        Camera_SetPosition(engineCore.camera, pos);
    }
    if (Input_GetKey(SDL_SCANCODE_DOWN, &engineCore.inputManager)) {
        Vector2 pos = engineCore.camera->position;
        pos.y += 200.0f * engineCore.timeInfo.deltaTime;
        Camera_SetPosition(engineCore.camera, pos);
    }
    if (Input_GetKey(SDL_SCANCODE_LEFT, &engineCore.inputManager)) {
        Vector2 pos = engineCore.camera->position;
        pos.x -= 200.0f * engineCore.timeInfo.deltaTime;
        Camera_SetPosition(engineCore.camera, pos);
    }
    if (Input_GetKey(SDL_SCANCODE_RIGHT, &engineCore.inputManager)) {
        Vector2 pos = engineCore.camera->position;
        pos.x += 200.0f * engineCore.timeInfo.deltaTime;
        Camera_SetPosition(engineCore.camera, pos);
    }
    
    // Zoom controls
    if (Input_GetKey(SDL_SCANCODE_Q, &engineCore.inputManager)) {
        Camera_Zoom(engineCore.camera, engineCore.camera->zoom + 1.0f * engineCore.timeInfo.deltaTime);
    }
    if (Input_GetKey(SDL_SCANCODE_E, &engineCore.inputManager)) {
        Camera_Zoom(engineCore.camera, engineCore.camera->zoom - 1.0f * engineCore.timeInfo.deltaTime);
    }
    
    if (Input_GetKeyDown(SDL_SCANCODE_F, &engineCore.inputManager)) {
        GameObject *player = GameObj_Find(&sceneMan, "player_0");
        if (player) {
            Camera_SetFollowTarget(engineCore.camera, player, (Vector2){0, 0});
        }
    }
    
    if (Input_GetKeyDown(SDL_SCANCODE_SPACE, &engineCore.inputManager)) {
        Camera_StartShake(engineCore.camera, 10.0f, 0.5f, 15.0f, 5.0f);
    }

    if(Input_GetKeyDown(SDL_SCANCODE_1, &engineCore.inputManager)){
         engineCore.camera->rotation += 5.0f;
        }
    if(Input_GetKeyDown(SDL_SCANCODE_2, &engineCore.inputManager)){
        SceneManager_LoadScene(&sceneMan, "Collision Test");
        }

   if (Input_GetMouseButtonDown(0, &engineCore.inputManager)) {
    int x, y;
    Input_GetMousePosition(&engineCore.inputManager, &x, &y);
    
    Vector2 worldPos = Camera_GetMouseWorldPosition(engineCore.camera, x, y);
    
    char name[32];
    snprintf(name, sizeof(name), "circle_%d", objectCounter++);
    
    GameObject *obj = GameObj_Create(name);
    GameObj_AddComponent(obj, COMPONENT_RENDERER);
    GameObj_AddComponent(obj, COMPONENT_RIGIDBODY); 
    Component *transform = GameObj_GetComponent(obj, COMPONENT_TRANSFORM);
    
    transform->data.transform.x = worldPos.x;
    transform->data.transform.y = worldPos.y;
    
    float scale = (rand() % 50) + 20; 
    transform->data.transform.scaleX = scale;
    transform->data.transform.scaleY = scale;
    
    Component *rb = GameObj_GetComponent(obj, COMPONENT_RIGIDBODY);
    Renderer_SetTexture(obj, circleTexture, 1, 1, 0.0f, SDL_FLIP_NONE, engineCore.gameContext.renderer);
    Scene_AddGameObject(&sceneMan, obj);
    GameObj_Start(obj);
    
    score += 10;
    objectCount++; 
}


if (Input_GetKey(SDL_SCANCODE_D, &engineCore.inputManager)) {
    Physics_AddForce(&rb->data.rigidbody.rigidBody, (Vector2){speed * 100, 0});
}
if (Input_GetKey(SDL_SCANCODE_A, &engineCore.inputManager)) {
    Physics_AddForce(&rb->data.rigidbody.rigidBody, (Vector2){-speed * 100, 0});
}
if (Input_GetKey(SDL_SCANCODE_W, &engineCore.inputManager)) {
    Physics_AddForce(&rb->data.rigidbody.rigidBody, (Vector2){0, -speed * 100});
}
if (Input_GetKey(SDL_SCANCODE_S, &engineCore.inputManager)) {
    Physics_AddForce(&rb->data.rigidbody.rigidBody, (Vector2){0, speed * 100});
}

if(rb->data.rigidbody.rigidBody.velocity.x > 0){
    rb->data.rigidbody.rigidBody.velocity.x -= rb->data.rigidbody.rigidBody.drag * engineCore.timeInfo.deltaTime;
} else if(rb->data.rigidbody.rigidBody.velocity.x < 0){
    rb->data.rigidbody.rigidBody.velocity.x += rb->data.rigidbody.rigidBody.drag * engineCore.timeInfo.deltaTime;
}

if(rb->data.rigidbody.rigidBody.velocity.y > 0){
    rb->data.rigidbody.rigidBody.velocity.y -= rb->data.rigidbody.rigidBody.drag * engineCore.timeInfo.deltaTime;
} else if(rb->data.rigidbody.rigidBody.velocity.y < 0){
    rb->data.rigidbody.rigidBody.velocity.y += rb->data.rigidbody.rigidBody.drag * engineCore.timeInfo.deltaTime;
}

forceVector.x = rb->data.rigidbody.rigidBody.velocity.x;
forceVector.y = rb->data.rigidbody.rigidBody.velocity.y;


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
    
    RenderScene(engineCore.gameContext.renderer, &sceneMan, engineCore.camera);
        textRect.y = 10;
float arrowScale = 0.13f;
Vector2 centerPos = {transform->data.transform.x, 
                    transform->data.transform.y};
if (fabs(forceVector.x) > 1.0f) {
    Vector2 xStart = centerPos;
    Vector2 xEnd = {centerPos.x + (forceVector.x * arrowScale), centerPos.y};
    LineRender_RenderArrow(engineCore.gameContext.renderer, xStart, xEnd, (SDL_Color){255, 0, 0, 255}, engineCore.camera);
}

if (fabs(forceVector.y) > 1.0f) {
    Vector2 yStart = centerPos;
    Vector2 yEnd = {centerPos.x, centerPos.y + (forceVector.y * arrowScale)};
    LineRender_RenderArrow(engineCore.gameContext.renderer, yStart, yEnd, (SDL_Color){0, 255, 0, 255}, engineCore.camera);
}
    TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, "Scene: %s", sceneMan.activeScene->name);
        textRect.y = 50;
    TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, "fps: %.3f", engineCore.timeInfo.fps);
        textRect.y = 80;
    TextRender_RenderText(&engineCore.textRenderer, gameFont, &textRect, "Objects: %d", sceneMan.activeScene->gameObjectsCount);
    SDL_RenderPresent(engineCore.gameContext.renderer);
   
    SDL_Rect obj1 = {
            (int)transform->data.transform.x, 
            (int)transform->data.transform.y,
            (int)transform->data.transform.scaleX, 
            (int)transform->data.transform.scaleY
        };

        SDL_Rect obj2 = {
            (int)newTransform->data.transform.x, 
            (int)newTransform->data.transform.y,
            (int)newTransform->data.transform.scaleX, 
            (int)newTransform->data.transform.scaleY
        };
   

    if (Input_ShouldQuit(&engineCore.inputManager)) {
        engineCore.isRunning = false;
    }
    
    SDL_Delay(16);
}
    
    Engine_Shutdown(&engineCore);
    return 0;
}
