#ifndef CAMERA_H
#define CAMERA_H

#include "maths.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct GameObject GameObject;

typedef struct Camera{
  Vector2 position;
  Vector2 target;
  Vector2 offset;

  float zoom;
  float rotation;
  float fieldOfView;

  int viewportWidth;
  int viewportHeight;

  bool isFollowing;
  GameObject *followTarget;
  float followDamping;
  Vector2 followOffset;

  bool hasBounds;
  SDL_Rect worldRect;
  
  bool isShaking;
  float shakeIntensity;
  float shakeDuration;
  float shakeFrequency;
  float shakePhase;
  float shakeAngle;
  float shakeDecay;
  float shakeTimer;
  Vector2 shakeOffset;

  float moveSpeed;
  Vector2 velocity;
}Camera;

Camera *Camera_Create(int viewportWidth, int viewportHeight);
void Camera_Initialize(Camera *camera, int viewportWidth, int viewportHeight);
void Camera_Destroy(Camera *camera);
void Camera_Update(Camera *camera, float deltaTime);

Vector2 Camera_WorldToScreen(Camera *camera, Vector2 worldPos);
Vector2 Camera_ScreenToWorld(Camera *camera, Vector2 screenPos);
SDL_Rect Camera_WorldRectToScreen(Camera *camera, SDL_FRect worldRect);
Vector2 Camera_GetMouseWorldPosition(Camera *camera, int mouseX, int mouseY);
bool Camera_IsPointVisible(Camera *camera, Vector2 worldPoint);

void Camera_StartShake(Camera *camera, float shakeIntensity, float shakeDuration, float shakeFrequency, float shakeDecay);
void Camera_StopShake(Camera *camera);

void Camera_SetPosition(Camera *camera, Vector2 position);
void Camera_SetTarget(Camera *camera, Vector2 target);
void Camera_SetFollowTarget(Camera *camera, GameObject *target, Vector2 offset);
void Camera_StopFollowing(Camera *camera);
//value between 0.1 (zoom out) - 10.0 (zoom in) | 1.0(normal)
void Camera_Zoom(Camera *camera, float zoom);
void Camera_SetBounds(Camera *camera, SDL_Rect worldBounds);

#endif // !CAMERA_H
