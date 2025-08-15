#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "audio.h"
#include "collision.h"
#include "SDL2/SDL_image.h"
#include "physics.h"
#include "render.h"

#define MAX_AUDIOSOURCE_NAME 32
#define MAX_CHARACTERS_NAME 32
#define MAX_COMPONENTS 15
#define MAX_GAMEOBJECTS 3000
#define MAX_CHILDRENS 100
#define MAX_SCENES 10

typedef struct GameObject GameObject;
typedef struct Component Component;
typedef struct Scene Scene;

typedef enum{
  COMPONENT_TRANSFORM,
  COMPONENT_CAMERA,
  COMPONENT_RENDERER,
  COMPONENT_RIGIDBODY,
  COMPONENT_COLLIDER,
  COMPONENT_AUDIO_SOURCE,
  COMPONENT_UI_ELEMENT,
  COMPONENT_SCRIPT,
}ComponentType;

typedef struct Component{
  ComponentType type;
  GameObject* owner;
  bool isEnabled;

  void(*Start)(Component *self);
  void(*Update)(Component *self, double deltaTime);
  void(*OnDestroy)(Component *self);
  void(*OnCollision)(Component* self, struct GameObject *other);

  union{
    struct{
      float x, y;
      float rotation;
      float scaleX, scaleY;
    }transform;
    
    struct{
      bool isEnabled;
      float fieldOfView;
       
    }camera;

    struct{
     Texture texture; 
    }renderer;

    struct{
      RigidBody rigidBody; 
    }rigidbody;

    struct{
      Box_Collider box_collider;
      Circle_Collider circle_collider;
      Mesh_Collider mesh_collider;
    }collider;

    struct{
     //Add ui elements
    }ui_element;

    struct{
      char clipName[MAX_AUDIOSOURCE_NAME];
      AudioSource *audioSource;
      bool loop;
      bool playOnAwake;
    }audio_source;

    struct{
      void *scriptData;
      void(*CostumeStart)(void *scriptData);
      void(*CostumeUpdate)(void *scriptData, float deltaTime);
    }script;

  }data;
}Component;

typedef struct GameObject{
  char name[MAX_CHARACTERS_NAME];
  char tag[MAX_CHARACTERS_NAME];
  int layer;
  bool isActive;
  Component* components[MAX_COMPONENTS];
  int numComponents;

  //an object has a transform by default
  Component *transform;

  GameObject *parent;
  GameObject *children[MAX_CHILDRENS];
  int childCount;

  Scene *scene;
}GameObject;

typedef struct Scene{
  char name[MAX_CHARACTERS_NAME];
  GameObject *objects[MAX_GAMEOBJECTS];
  int gameObjectsCount;
  bool isActive;

  void(*OnSceneLoad)(Scene *self);
  void(*OnSceneUnload)(Scene *self);
}Scene;

typedef struct SceneManager{
  Scene *scenes[MAX_SCENES];
  int numberOfScenes;
  Scene *activeScene;
  Scene *sceneToLoad;
}SceneManager;

Component *Component_Create(ComponentType type);
void Component_Destroy(Component *component);
GameObject *GameObj_Create(const char *name);
Component *GameObj_AddComponent(GameObject *gameObj, ComponentType type);
Component *GameObj_GetComponent(GameObject *gameObj, ComponentType type);
bool GameObj_RemoveComponent(GameObject *gameObj, ComponentType type);
void GameObj_SetParent(GameObject *parentObj, GameObject *childObj);
void GameObj_Update(GameObject *gameObj, double deltaTime);
void GameObj_Start(GameObject *gameObj);
void GameObj_Destroy(GameObject *gameObj);


void SceneManager_UnloadAll(SceneManager *sceneMan);
void ScriptComponent_Start(Component *comp);
void ScriptComponent_Update(Component *comp, double deltaTime);
Scene *Scene_CreateScene(SceneManager *sceneMan,const char* name);
bool SceneManager_LoadScene(SceneManager *sceneMan, const char *name);
bool Scene_AddGameObject(SceneManager *sceneMan, GameObject *gameObj);
void SceneManager_Update(SceneManager *sceneMan, float deltaTime);
GameObject *GameObj_Find(SceneManager *sceneMan, const char *name);

//physics functions integration

void RigidbodyComponent_Update(Component *comp, double deltaTime);

void GameObj_AddForce(GameObject *gameObj, Vector2 force);
void GameObj_AddForceXY(GameObject *gameObj, float forceX, float forceY);
void GameObj_SetVelocity(GameObject *gameObj, Vector2 velocity);
void GameObj_SetVelocityXY(GameObject *gameObj, float velocityX, float velocityY);
Vector2 GameObj_GetVelocity(GameObject *gameObj);
Vector2 GameObj_GetPosition(GameObject *gameObj);
void GameObj_SetPosition(GameObject *gameObj, Vector2 position);
void GameObj_AddImpulse(GameObject *gameObj, Vector2 impulse);
#endif // !GAME_H
