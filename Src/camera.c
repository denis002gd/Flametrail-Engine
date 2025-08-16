#include "../HeaderFiles/camera.h"
#include "../HeaderFiles/game.h"
#include <SDL2/SDL_rect.h>

static unsigned int fastRandSeed = 1;

static void fastRand_Seed(unsigned int seed1, unsigned int seed2) {
    fastRandSeed = (seed1 * 1103515245U + seed2) ^ (seed2 << 16);
    if (fastRandSeed == 0) fastRandSeed = 1; 
}

static unsigned int fastRand(void) {
    fastRandSeed = fastRandSeed * 1103515245U + 12345U;
    return (fastRandSeed >> 16) & 0x7FFF; 
}

Camera *Camera_Create(int viewportWidth, int viewportHeight){
  Camera *camera = calloc(1,sizeof(Camera));
  if(!camera) return NULL;
  Camera_Initialize(camera, viewportWidth, viewportHeight);
  return camera;
}
void Camera_Initialize(Camera *camera, int viewportWidth, int viewportHeight){
  camera->position = (Vector2){0.0f, 0.0f};
  camera->target = (Vector2){0.0f, 0.0f};
  camera->offset = (Vector2){viewportWidth / 2.0f, viewportHeight / 2.0f};

  camera->zoom = 1.0f;
  camera->rotation = 0.0f;
  camera->fieldOfView = 45.0f;

  camera->viewportHeight = viewportHeight;
  camera->viewportWidth = viewportWidth;

  camera->isFollowing = false;
  camera->followTarget = NULL;
  camera->followDamping = 0.1f;
  camera->followOffset = (Vector2){0.0f, 0.0f};

  camera->hasBounds = false;
  camera->worldRect = (SDL_Rect){0,0,0,0};

  camera->isShaking = false;
  camera->shakeDuration = 0.0f;
  camera->shakeIntensity = 0.0f;
  camera->shakeTimer = 0.0f;
  camera->shakeDecay = 3.5f;
  camera->shakeOffset = (Vector2){0.0f, 0.0f};

  camera->moveSpeed = 1.0f;
  camera->velocity = (Vector2){0.0f, 0.0f};
}
void Camera_Destroy(Camera *camera){
  if(camera){
    free(camera);
  }
}

void Camera_Update(Camera *camera, float deltaTime){
  if(!camera) return;

  if(camera->isFollowing && camera->followTarget){
    Vector2 targetPos = GameObj_GetPosition(camera->followTarget);
    targetPos = Vec2_Add(targetPos, camera->followOffset);
    
    Vector2 diff = Vec2_Sub(targetPos, camera->position);
    diff = Vec2_Scale(diff, camera->followDamping);
    camera->position = Vec2_Add(camera->position, diff);
  }

  if(camera->isShaking){
    camera->shakeTimer -= deltaTime;
    if(camera->shakeTimer <= 0){
      Camera_StopShake(camera);
    }else{
      float t = (camera->shakeDuration - camera->shakeTimer);
      float intensity = camera->shakeIntensity * expf(-camera->shakeDecay * t);

      camera->shakePhase += (2.0f * M_PI) * camera->shakeFrequency * deltaTime;
      camera->shakeOffset.x = cosf(camera->shakeAngle) * sinf(camera->shakePhase) * intensity;
      camera->shakeOffset.y = sinf(camera->shakeAngle) * sinf(camera->shakePhase) * intensity;
    }
  }
  
  if(camera->hasBounds){
    //this is for finding how far the camera sees (/2 for world centering)
    float halfViewWidth = (camera->viewportWidth / camera->zoom) / 2.0f;
    float halfViewHeight = (camera->viewportHeight / camera->zoom) / 2.0f;
    //clamping the camera position within world bounds
    camera->position.x = Math_Clamp(camera->position.x, camera->worldRect.x + halfViewWidth
                                    ,camera->worldRect.x + camera->worldRect.w - halfViewWidth);

    camera->position.y = Math_Clamp(camera->position.y, camera->worldRect.y + halfViewHeight
                                    ,camera->worldRect.y + camera->worldRect.h - halfViewHeight);
  }
}
void Camera_StartShake(Camera *camera, float shakeIntensity, float shakeDuration, float shakeFrequency, float shakeDecay){
  if(!camera) return;
  fastRand_Seed(37, 227);
  camera->shakeDuration = shakeDuration;
  camera->shakeFrequency = shakeFrequency;
  camera->shakeIntensity = shakeIntensity;
  camera->shakeDecay = shakeDecay;
  camera->shakeAngle = (fastRand() % 360) * (M_PI / 180);
  camera->shakePhase = 0.0f;
  camera->shakeTimer = shakeDuration;
  camera->isShaking = true;
}
void Camera_StopShake(Camera *camera){
  if(!camera) return;
  camera->isShaking = false;
  camera->shakeDuration = 0.0f;
  camera->shakeOffset = (Vector2){0.0f,0.0f};
}
Vector2 Camera_WorldToScreen(Camera *camera, Vector2 worldPos){
  if(!camera) return (Vector2){0.0f, 0.0f};

  Vector2 relative = Vec2_Sub(worldPos, camera->position);
  //apply camera transformations
  relative = Vec2_Add(relative, camera->shakeOffset);
  relative = Vec2_Scale(relative, camera->zoom);
  if(camera->rotation != 0.0f){
     float rotMatrix[2][2];
        Matrix_Rotate(rotMatrix, camera->rotation * M_PI / 180.0f);
         relative = Vec2_MatrixRotation(rotMatrix, relative);
  }
  relative = Vec2_Add(relative, camera->offset);
  return relative;
}
Vector2 Camera_ScreenToWorld(Camera *camera, Vector2 screenPos){
  if(!camera)return (Vector2){0.0f, 0.0f};
  //reversed world to screen steps
  Vector2 relative = Vec2_Sub(screenPos, camera->offset);
  relative = Vec2_Scale(relative, 1.0f / camera->zoom);
  relative = Vec2_Sub(relative, camera->shakeOffset);
  Vector2 worldPos = Vec2_Add(relative, camera->position);

  return worldPos;
}
SDL_Rect Camera_WorldRectToScreen(Camera *camera, SDL_FRect worldRect){
  Vector2 topLeft = {worldRect.x, worldRect.y};
  Vector2 bottomRight = {worldRect.x + worldRect.w, worldRect.y + worldRect.h};

  Vector2 screenTopLeft = Camera_WorldToScreen(camera, topLeft);
  Vector2 screenBottmRight = Camera_WorldToScreen(camera, bottomRight);

  SDL_Rect screenRect = {
    (int)screenTopLeft.x,
    (int)screenTopLeft.y,
    (int)(screenBottmRight.x - screenTopLeft.x),
    (int)(screenBottmRight.y - screenTopLeft.y)
  };
  return screenRect;
}
Vector2 Camera_GetMouseWorldPosition(Camera *camera, int mouseX, int mouseY){ 
  Vector2 screenPos = (Vector2){(float)mouseX, (float)mouseY}; 
  return Camera_ScreenToWorld(camera, screenPos);
}
bool Camera_IsPointVisible(Camera *camera, Vector2 worldPoint){
  if(!camera) return false;
  Vector2 screenPoint = Camera_WorldToScreen(camera, worldPoint);

  return (screenPoint.x >= 0 && screenPoint.x <= camera->viewportWidth &&
          screenPoint.y >= 0 && screenPoint.y <= camera->viewportHeight);
}
void Camera_SetPosition(Camera *camera, Vector2 position){
  if(!camera) return;
  camera->position = position;
}
void Camera_SetTarget(Camera *camera, Vector2 target){
  if(!camera) return;
  camera->target = target;
  camera->position = Vec2_Sub(target, camera->offset);
}
void Camera_SetFollowTarget(Camera *camera, GameObject *target, Vector2 offset){
  if(!camera || !target) return;
  camera->followTarget = target;
  camera->followOffset = offset;
  camera->isFollowing = true;
}
void Camera_StopFollowing(Camera *camera){
  if(!camera) return;
  camera->isFollowing = false;
  camera->followTarget = NULL;
}
void Camera_Zoom(Camera *camera, float zoom){
  if(!camera) return;
  camera->zoom = Math_Clamp(zoom, 0.1f, 10.0f);
}
void Camera_SetBounds(Camera *camera, SDL_Rect worldBounds){
  if(!camera) return;
  camera->hasBounds = true;
  camera->worldRect = worldBounds;
}
