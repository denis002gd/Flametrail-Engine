#include "../HeaderFiles/audio.h"
#include "../HeaderFiles/input.h"
#include "../HeaderFiles/render.h"
#include "../HeaderFiles/timer.h"
#include "../HeaderFiles/game.h"
#include "../HeaderFiles/collision.h"
#include "../HeaderFiles/maths.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>


SceneManager sceneMan = {0};

void PlayScriptTest(void *scriptData, float deltaTime){
  printf("Player script has been called, everthing works fine :)\n");
}
void ScriptInitTest(void *scriptData){
  printf("Costume script on this object was called successfully in start\n");
}
void SetupGameScene(Scene *scene){
  printf("Setting up game scene\n");
  GameObject* player = GameObj_Create("Player");
  GameObj_AddComponent(player, COMPONENT_RENDERER);
  GameObj_AddComponent(player, COMPONENT_RIGIDBODY);

  GameObject* enemy1 = GameObj_Create("Enemy 1");
  GameObj_AddComponent(enemy1, COMPONENT_RENDERER);
  GameObj_AddComponent(enemy1, COMPONENT_RIGIDBODY);
  Scene_AddGameObject(&sceneMan, enemy1);

  GameObject* boss = GameObj_Create("Boss");
  GameObj_AddComponent(boss, COMPONENT_RENDERER);
  GameObj_AddComponent(boss, COMPONENT_RIGIDBODY);
  Scene_AddGameObject(&sceneMan, boss);

  //adding a costume script Component
  Component *script = GameObj_AddComponent(player, COMPONENT_SCRIPT);
  script->data.script.CostumeStart = ScriptInitTest;
  player->transform->data.transform.x = 100.0f;
  player->transform->data.transform.y = 100.0f;
  Scene_AddGameObject(&sceneMan, player);

}

int main(){
  InputState inputState = {0};
  AudioManager audioMan = {0};
  InitInput(&inputState);
  Audio_Init(&audioMan);
  Audio_LoadClip(&audioMan,"../NonEssentials/testRes/meow.mp3", "meow", false);
  Audio_LoadClip(&audioMan, "../NonEssentials/testRes/meow2.mp3", "meow2", false);
  Audio_LoadClip(&audioMan, "../NonEssentials/testRes/SampleMusic.mp3", "song", true);
  GameContext game_cont = {0};
  TimeInfo time = {0};
  game_cont.window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
  if(InitGameContext(&game_cont)){
    exit(1);
  }
  Timer newTimer = {0};
  Timer_Set(&newTimer, 5.0f);
  time.earlyFrame = SDL_GetTicks();
  time.lateFrame = 0;
  game_cont.running = 1;
  float volumeEase = 0.0f;
  bool ret;
  SDL_Event event;
  Audio_PlayMusic(&audioMan, "song", 1.0f, true);
 
  Scene *menuScene = Scene_CreateScene(&sceneMan, "MenuScene");
  Scene *Level1 = Scene_CreateScene(&sceneMan, "GameLevel1");
  Level1->OnSceneLoad = SetupGameScene;

  SceneManager_LoadScene(&sceneMan, "GameLevel1");
  while(game_cont.running){
    //delta time calculation
    time.lateFrame = time.earlyFrame;
    time.earlyFrame = SDL_GetTicks();
    Time_CalculateDeltaTime(&time);
    Time_CalculateGlobalTime(&time);
    UpdateInput(&inputState);
    while(SDL_PollEvent(&event)){
      Input_ProcessEvent(&event, &inputState); 
    }
//////////////////////////Add code here/////////////////////////////////

    if(Input_GetKeyDown(SDL_SCANCODE_1, &inputState)){
      SceneManager_LoadScene(&sceneMan, "MenuScene");
      float matrix[3][3] = {
        {4,-2, 6},
        {2, 5, 0},
        {-2,1,-4},
      };
      printf("Rasp: %.1f\n", Det_3D(matrix));
    }
    if(Input_GetKeyDown(SDL_SCANCODE_2, &inputState)){
      SceneManager_LoadScene(&sceneMan, "GameLevel1");
      Audio_PlayOneShot(&audioMan, "meow", 1.0f);
    }
    if(Input_GetKeyDown(SDL_SCANCODE_3, &inputState)){

    }
    SceneManager_Update(&sceneMan, time.deltaTime);

   if(Input_GetKeyDown(SDL_SCANCODE_SPACE, &inputState)){
    GameObject *player = GameObj_Find(&sceneMan, "Player");
      if(player){
        Component *rigidBody = GameObj_GetComponent(player, COMPONENT_RIGIDBODY);
        rigidBody->data.rigidbody.mass += 10.0f;
        printf("Player rigidbody mass: %.2f\n", rigidBody->data.rigidbody.mass);
      }
    }
  

////////////////////////////////////////////////////////////////////////

    if(Input_ShouldQuit(&inputState)){
     game_cont.running = 0; 
    }
    SDL_Delay(16);
  }
  Audio_Quit(&audioMan);
  FreeGameContext(&game_cont);
  return 0;
}
