#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "audio.h"
#include "render.h"

#define MAX_AUDIOSOURCE_NAME 32
#define MAX_CHARACTERS_NAME 32
#define MAX_COMPONENTS 6
#define MAX_GAMEOBJECTS 1000
#define MAX_CHILDRENS 100
#define MAX_SCENES 10

typedef struct GameObject GameObject;
typedef struct Component Component;
typedef struct Scene Scene;

typedef enum{
  COMPONENT_TRANSFORM,
  COMPONENT_RENDERER,
  COMPONENT_RIGIDBODY,
  COMPONENT_COLLIDER,
  COMPONENT_AUDIO_SOURCE,
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
      SDL_Color color;
      bool visible;
      int layer;
    }renderer;

    struct{
      float X_velocity, Y_velocity;
      float mass;
      bool useGravity;
    }rigidbody;

    struct{
      
      //TODO: add collision options from physics.c
    }collider;

    struct{
      char clipName[MAX_AUDIOSOURCE_NAME];
      AudioSource *audioSource;
      bool loop;
      bool playOnAwake;
    }audio_source;

    struct{
      void *scriptData;
      void(*CostumeUpdate)(void *scriptData, float deltaTime);
    }script;

  }data;
}Component;

typedef struct GameObject{
  char name[MAX_CHARACTERS_NAME];
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

typedef struct{
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
#endif // !GAME_H
