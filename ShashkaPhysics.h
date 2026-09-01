#ifndef SHASHKAPHYSICS_H
#define SHASHKAPHYSICS_H

#include "ShashkaCreator.h"

struct kinematicSquare {
    float x, y;
    float width, height;
    float angle;

    RGB color;

    std::string tag;
    bool active;

    float vx = 0.0f;
    float vy = 0.0f;

    float ax = 0.0f;
    float ay = 0.0f;

    float orbitAngle = 0.0f;
};

struct dynamicSquare {
    float x, y;
    float width, height;
    float angle;

    RGB color;

    std::string tag;
    bool active;

    float mass = 1.0f;
    float friction = 0.3f;

    float vx = 0.0f;
    float vy = 0.0f;

    float ax = 0.0f;
    float ay = 0.0f;

    float fx = 0.0f;
    float fy = 0.0f;

    float currentN = 0.0f;
};

struct liquid {
    float x, y;
    float width, height;

    float density;

    RGB color;

    std::string tag;
    bool active;
};

enum jointType {
    spring,
    rope,
    rod
};

struct joint {
    jointType type;

    float anchorX;
    float anchorY;

    float length;
    float stiffness;
    float damping;

    bool active = true;
};

const float g = 981.0f;

void UpdateUniformMovement(kinematicSquare& object);

void DrawKinematicSquare(window& update,kinematicSquare& object);

void UpdateAcceleratedMovement(kinematicSquare& object);

void UpdateFreeFall(kinematicSquare& object);

void LaunchBallisticMovement(kinematicSquare& object, float initialVelocity, float angle);

void UpdateBallisticMovement(kinematicSquare& object);

void UpdateCircularMovement(kinematicSquare& object, float centerX, float centerY, float radius, float angularVelocity);

void ApplyGravity(dynamicSquare& object);

void UpdateDynamic(dynamicSquare& object);

void DrawDynamicSquare(window& update, dynamicSquare& object);

bool ApplyNormalReaction(dynamicSquare& dynamicObject, const dynamicSquare& staticObject);

void ApplyForce(dynamicSquare& object, float fx, float fy);

void ApplyFriction(dynamicSquare& object);

void UpdateDynamicCollision(dynamicSquare& objectA, dynamicSquare& objectB);

float KineticEnergy(dynamicSquare& object);

float PotentialEnergy(dynamicSquare& object, float ground);

float PowerByForce(float force, float velocity);

float PowerByWork(float work, float time);

void ApplyArchimedesForce(dynamicSquare& object, const liquid& fluid);

void DrawLiquid(window& update, liquid& object);

bool IsCollide(const dynamicSquare& objectA, const square& objectB);

bool IsCollide(const dynamicSquare& objectA, const dynamicSquare& objectB);

bool IsCollide(const liquid& objectA, const liquid& objectB);

bool IsCollide(const kinematicSquare& objectA, const liquid& objectB);

bool IsCollide(const dynamicSquare& objectA, const liquid& objectB);

bool IsCollide(const dynamicSquare& objectA, const kinematicSquare& objectB);

bool IsCollide(const kinematicSquare& objectA, const kinematicSquare& objectB);

bool IsCollide(const kinematicSquare& objectA, const square& objectB);

bool IsCursorTouched(window& update, liquid& object);

bool IsCursorTouched(window& update, kinematicSquare& object);

bool IsCursorTouched(window& update, dynamicSquare& object);

void ApplyJoint(dynamicSquare& object, joint& anchor);

#endif // SHASHKAPHYSICS_H
