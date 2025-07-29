#ifndef MATHS_H
#define MATHS_H

#include <math.h>

typedef struct{
  float x, y; 
}Vector2;

float Math_Pow2(float x);
float Math_Clamp(float val, float min, float max);
float Vec2_GetDistance(Vector2 A, Vector2 B);
float Vec2_GetMagnitude(Vector2 A);
Vector2 Vec2_Sub(Vector2 A, Vector2 B);
Vector2 Vec2_Add(Vector2 A, Vector2 B);
Vector2 Vec2_Normalize(Vector2 A);
Vector2 Vec2_Scale(Vector2 A, float scale);
Vector2 Vec2_Clamp(Vector2 target, Vector2 min, Vector2 max);
float Vec2_Dot(Vector2 A, Vector2 B);

float EaseInOutBack(float x);
#endif // !M:w
// ATHS_H
