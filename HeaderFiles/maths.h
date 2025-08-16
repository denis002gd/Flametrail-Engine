#ifndef MATHS_H
#define MATHS_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

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
float Vec2_AngleCos(Vector2 A, Vector2 B);
Vector2 Vec2_Porjection(Vector2 A, Vector2 B);
float Scalar_Projection(Vector2 A, Vector2 B);
float Math_Orient2D(Vector2 A, Vector2 B, Vector2 C);
float Vec2_Dot(Vector2 A, Vector2 B);
float Det_3D(float m[3][3]);
float Det_2D(float m[2][2]);
void Matrix_Rotate(float m[2][2], float angle);
Vector2 Vec2_MatrixRotation(float rotationMatrix[2][2], Vector2 vector);
float EaseInOutBack(float x);
float Math_InCircle2D(Vector2 A, Vector2 B, Vector2 C, Vector2 D);
bool Math_InCircle2DCheck(float resultedValue);
#endif // !M:w
