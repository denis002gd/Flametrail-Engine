#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include "maths.h"
#include <SDL2/SDL.h>

      typedef struct{

      }Box_Collider;

      typedef struct{

      }Circle_Collider;

       typedef struct{

      }Mesh_Collider;
bool Collision_CirclesAreOverlapping(Vector2 C1, float r1, Vector2 C2, float r2);
bool Collision_IsPointInsideCircle(Vector2 circle, float radius, Vector2 point);

bool Collision_AABB(SDL_Rect A, SDL_Rect B);
#endif // !COLLISION_H


