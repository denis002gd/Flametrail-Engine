#include "../HeaderFiles/collision.h"


bool Collision_CirclesAreOverlapping(Vector2 C1, float r1, Vector2 C2, float r2){
  float dx = C2.x - C1.x;
  float dy = C2.y - C1.y;
  float dist2 = dx * dx + dy * dy;

  float rad2 = (r1 + r2) * (r1 + r2);

  return (dist2 <= rad2);
}
bool Collision_IsPointInsideCircle(Vector2 circle, float radius, Vector2 point){
  float dx = point.x - circle.x;
  float dy = point.y - circle.y;
  float dist = dx * dx + dy * dy;
  return (dist <= (radius * radius));
}

bool Collision_AABB(SDL_Rect A, SDL_Rect B){
  return A.x + A.w > B.x && A.x < B.x + B.w
        && A.y + A.h > B.y && A.y < B.y + B.h;
}
