#include "maths.h"

Vector2 Vec2_Sub(Vector2 A, Vector2 B){
  
  return (Vector2){A.x - B.x, A.y - B.y};
}

Vector2 Vec2_Add(Vector2 A, Vector2 B){
  return (Vector2){A.x + B.x, A.y + B.y};
}

float Math_Pow2(float x){
  return x * x;
}

float Vec2_GetDistance(Vector2 A, Vector2 B){
  return sqrt(Math_Pow2(A.x - B.x) + Math_Pow2(A.y - B.y));
}

float Vec2_GetMagnitude(Vector2 A){
  return sqrt((Math_Pow2(A.x)) + (Math_Pow2(A.y)));
}

Vector2 Vec2_Normalize(Vector2 A){
  float mag = Vec2_GetMagnitude(A);
  return (Vector2){A.x / mag, A.y / mag};
}

Vector2 Vec2_Scale(Vector2 A, float scale){ 
  return (Vector2){A.x * scale, A.y * scale};
}

float Math_Clamp(float val, float min, float max){
  if(val < min) return min;
  if(val > max) return max;
  return val;
}
Vector2 Vec2_Clamp(Vector2 target, Vector2 min, Vector2 max){

  Vector2 result = (Vector2){Math_Clamp(target.x, min.x, max.x),
  Math_Clamp(target.y, min.y, max.y)};

  return result;
}

float Vec2_Dot(Vector2 A, Vector2 B){
  float dot = (A.x * B.x) + (A.y * B.y);
  return dot / (Vec2_GetMagnitude(A) * Vec2_GetMagnitude(B));
}

float EaseInOutBack(float x){
  const float c1 = 1.70158;
  const float c2 = c1 * 1.525;

  return x < 0.5 
  ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 
  : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;

}
