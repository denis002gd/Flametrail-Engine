#include "game.h"
#include "audio.h"
#include <stdio.h>
#include <string.h>
/*A component is a logic block that can be assigned to a game object (everything in the world is a game object),
this mimics unity's behavioural way of programming
(available components start with COMPONENT_)
*/
Component *Component_Create(ComponentType type){
 Component *comp = malloc(sizeof(Component));
 if(!comp) return NULL;
 memset(comp, 0, sizeof(Component));
  comp->type = type;
  comp->isEnabled = true;

  switch(type){
    case COMPONENT_TRANSFORM:
     comp->data.transform.scaleX = 1.0f;
     comp->data.transform.scaleY = 1.0f;
    break;
    case COMPONENT_RENDERER:
      comp->data.renderer.color = (SDL_Color){255,255,255,255};
      comp->data.renderer.visible = true;
      comp->data.renderer.layer = 0;
    break;
    case COMPONENT_RIGIDBODY:
      comp->data.rigidbody.mass = 1.0f;
      comp->data.rigidbody.useGravity = true;
    break;
    case COMPONENT_AUDIO_SOURCE:
      //DEFAULT VALUES | volume = 1.0f, loop = false, playOnAwake = false
      comp->data.audio_source.audioSource = Audio_CreateAudioSource(1.0f, false, false);
      comp->data.audio_source.loop = false;
      comp->data.audio_source.playOnAwake = true;
    break;
  }
  return comp;
}
//Destroys a component
void Component_Destroy(Component *component){
  if(!component) return;

  if(component->OnDestroy){
    component->OnDestroy(component);
  }
  //TODO: If the component is a script data, i need to free that too
  free(component);
}
//Creates a game object with a given name (other values are not assigned except transform and number of components)
GameObject *GameObj_Create(const char *name){
  GameObject *gameObj = malloc(sizeof(GameObject));
  if(!gameObj) return NULL;
  
  memset(gameObj, 0, sizeof(GameObject));
  strncpy(gameObj->name, name, MAX_CHARACTERS_NAME - 1);
  gameObj->name[MAX_CHARACTERS_NAME - 1] = '\0';
  
  gameObj->transform = Component_Create(COMPONENT_TRANSFORM);
  gameObj->transform->owner = gameObj;
  gameObj->components[0] = gameObj->transform;
  gameObj->numComponents = 1;
  return gameObj;
}
//Returns a component from a game object
Component *GameObj_GetComponent(GameObject *gameObj, ComponentType type){
   if(!gameObj || !type) return NULL;

  for(int i = 0; i < gameObj->numComponents; i++){
     if(gameObj->components[i]->type == type){
      return gameObj->components[i];
    }
  }
  return NULL; 
}
Component *GameObj_AddComponent(GameObject *gameObj, ComponentType type){
  if(!gameObj || !type || gameObj->numComponents >= MAX_COMPONENTS) return NULL;

  for(int i = 0; i < gameObj->numComponents; i++){
    if(gameObj->components[i]->type == type){
      printf("Object \"%s\" already has this component\n", gameObj->name);
      //TODO: Allow more of the same components to the same game object
      //this is allowed for now but might add the ability to add more of the same component
    }
  }
  Component *comp = Component_Create(type);
  if(!comp) return NULL;

  comp->owner = gameObj;
  gameObj->components[gameObj->numComponents] = comp;
  gameObj->numComponents++;

  return comp;
}
//Removes a component (by type) from a game object (warning: if an object has multiple components of the same type it will destroy the first one assigned)
bool GameObj_RemoveComponent(GameObject *gameObj, ComponentType type){
  if(!gameObj || !type) return false;

  for(int i = 0; i < gameObj->numComponents; i++){
     if(gameObj->components[i]->type == type){
      Component_Destroy(gameObj->components[i]);
      //shift remaining components
      for(int j = i; j < gameObj->numComponents - 1; j++){
        gameObj->components[j] = gameObj->components[j + 1];
      }
      gameObj->numComponents--;
      return true;
    } 
  }
  return false;
}
//Sets a parent for the passed child (the original parent of the child will be removed)
void GameObj_SetParent(GameObject *parentObj, GameObject *childObj){
  if(!parentObj || !childObj) return;
  //if child already has a parent remove it
  if(childObj->parent){
    GameObject *oldParent = childObj->parent;
    for(int i = 0; i < oldParent->childCount; i++){
      if(oldParent->children[i] == childObj){

        for(int j = i; j < oldParent->childCount - 1; j++){
          oldParent->children[j] = oldParent->children[j + 1];
        }
        oldParent->childCount--;
        break;
      }
    }
  }
  childObj->parent = parentObj;
  if(parentObj && parentObj->childCount < MAX_CHILDRENS){
    parentObj->children[parentObj->childCount] = childObj;
    parentObj->childCount++;
  }
}
//updates the game objext every frame unless not active
void GameObj_Update(GameObject *gameObj, double deltaTime){
  if(!gameObj) return; 
  for(int i = 0; i < gameObj->numComponents; i++){
     Component *comp = gameObj->components[i];
     if(comp->Update && comp->isEnabled){
      comp->Update(comp, deltaTime);
    }
  }
  //updates the components in children too
  for(int i = 0; i < gameObj->childCount; i++){
    GameObj_Update(gameObj->children[i], deltaTime);
  }
}
//updates the object as soon as active
void GameObj_Start(GameObject *gameObj){
  if(!gameObj) return; 
  for(int i = 0; i < gameObj->numComponents; i++){
     Component *comp = gameObj->components[i];
     if(comp->Start && comp->isEnabled){
      comp->Start(comp);
    }
  }
  for(int i = 0; i < gameObj->childCount; i++){
    GameObj_Start(gameObj->children[i]);
  }
}
//Destroys the object and all his child objects
void GameObj_Destroy(GameObject *gameObj){
  if(!gameObj) return;

  for(int i = 0; i < gameObj->numComponents; i++){
    Component_Destroy(gameObj->components[i]);
  }
  for(int i = 0; i < gameObj->childCount; i++){
    GameObj_Destroy(gameObj->children[i]);
  }
  free(gameObj);
  gameObj = NULL;
}

