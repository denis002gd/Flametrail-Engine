#include "../HeaderFiles/physics.h"

void Physics_ApplyGravity(RigidBody *rigidBody, float deltaTime) {
    if (!rigidBody || !rigidBody->useGravity) return;
    
    rigidBody->acceleration.y += GRAVITY * deltaTime;
}

void Physics_IntegrateVelocity(RigidBody *rigidBody, Vector2 *position, float deltaTime) {
    if (!rigidBody || !position) return;
    
    if (!rigidBody->freezeX) {
        position->x += rigidBody->velocity.x * deltaTime;
    }
    if (!rigidBody->freezeY) {
        position->y += rigidBody->velocity.y * deltaTime;
    }
    
    rigidBody->velocity = Vec2_Add(rigidBody->velocity, 
                                   Vec2_Scale(rigidBody->acceleration, deltaTime));
    
    rigidBody->acceleration.x = 0.0f;
    rigidBody->acceleration.y = 0.0f;
}

void Physics_AddForce(RigidBody *rigidBody, Vector2 force) {
    if (!rigidBody) return;
    
    Vector2 acceleration = Vec2_Scale(force, 1.0f / rigidBody->mass);
    rigidBody->acceleration = Vec2_Add(rigidBody->acceleration, acceleration);
}

void Physics_SetVelocity(RigidBody *rigidBody, Vector2 velocity) {
    if (!rigidBody) return;
    rigidBody->velocity = velocity;
}

void Physics_ApplyDrag(RigidBody *rigidBody, float dragCoefficient, float deltaTime) {
    if (!rigidBody || dragCoefficient <= 0.0f) return;
    
    Vector2 dragForce = Vec2_Scale(rigidBody->velocity, -dragCoefficient);
    Physics_AddForce(rigidBody, dragForce);
}
