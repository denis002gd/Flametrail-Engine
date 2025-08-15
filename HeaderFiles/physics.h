#ifndef PHYSICS_H
#define PHYSICS_H

#define GRAVITY 1000.7
//fs
#include <stdbool.h>
#include "maths.h"
#include "timer.h"

typedef struct GameObject GameObject;

typedef struct{
   float mass;
   Vector2 velocity;
   Vector2 acceleration;
   bool useGravity;
  
   float drag;       
   bool isKinematic;   
   bool freezeX;      
   bool freezeY;     
   bool useCollisions;
   bool ts;
} RigidBody;

void Physics_ApplyGravity(RigidBody *rigidBody, float deltaTime);
void Physics_IntegrateVelocity(RigidBody *rigidBody, Vector2 *position, float deltaTime);
void Physics_AddForce(RigidBody *rigidBody, Vector2 force);
void Physics_SetVelocity(RigidBody *rigidBody, Vector2 velocity);
void Physics_ApplyDrag(RigidBody *rigidBody, float dragCoefficient, float deltaTime);

#endif // !PHYSICS_H
