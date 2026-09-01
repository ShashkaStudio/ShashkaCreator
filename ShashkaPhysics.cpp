#include "ShashkaPhysics.h"

void UpdateUniformMovement(kinematicSquare& object) {
    if (!object.active) return;

    object.x += object.vx * deltaTime;
    object.y += object.vy * deltaTime;
}

void DrawKinematicSquare(window& update, kinematicSquare& object) {
    if (!object.active) return;

    square visualBody;

    visualBody.x = object.x;
    visualBody.y = object.y;
    visualBody.width = object.width;
    visualBody.height = object.height;
    visualBody.angle = object.angle;
    visualBody.color = object.color;
    visualBody.active = object.active;
    visualBody.tag = object.tag;

    DrawSquare(update, visualBody);
}

void UpdateAcceleratedMovement(kinematicSquare& object) {
    if (!object.active) return;

    object.vx += object.ax * deltaTime;
    object.vy += object.ay * deltaTime;

    object.x += object.vx * deltaTime;
    object.y += object.vy * deltaTime;
}

void UpdateFreeFall(kinematicSquare& object) {
    if (!object.active) return;

    object.vy += g * deltaTime;
    object.y += object.vy * deltaTime;
}

void LaunchBallisticMovement(kinematicSquare& object, float initialVelocity, float angle) {
    float radAngle = angle * rad;

    object.vx = initialVelocity * cosf(radAngle);
    object.vy = -initialVelocity * sinf(radAngle);
}

void UpdateBallisticMovement(kinematicSquare& object) {
    if (!object.active) return;

    object.x += object.vx * deltaTime;

    object.vy += g * deltaTime;
    object.y += object.vy * deltaTime;
}

void UpdateCircularMovement(kinematicSquare& object, float centerX, float centerY, float radius, float angularVelocity) {
    if (!object.active) return;

    object.orbitAngle += angularVelocity * deltaTime;

    if (object.orbitAngle >= 360.0f) object.orbitAngle -= 360.0f;
    if (object.orbitAngle < 0.0f) object.orbitAngle += 360.0f;

    float radAngle = object.orbitAngle * rad;

    object.x = centerX + radius * cosf(radAngle);
    object.y = centerY + radius * sinf(radAngle);

    object.angle = object.orbitAngle;
}

void ApplyGravity(dynamicSquare& object) {
    if (!object.active) return;

    float fg = object.mass * g;
    object.fy += fg;
}

void UpdateDynamic(dynamicSquare& object) {
    if (!object.active) return;

    object.ax = object.fx / object.mass;
    object.ay = object.fy / object.mass;

    object.vx += object.ax * deltaTime;
    object.vy += object.ay * deltaTime;

    if (fabsf(object.vx) < 1.0f) object.vx = 0.0f;

    object.x += object.vx * deltaTime;
    object.y += object.vy * deltaTime;

    object.fx = 0.0f;
    object.fy = 0.0f;
    object.currentN = 0.0f;
}

void DrawDynamicSquare(window& update, dynamicSquare& object) {
    if (!object.active) return;

    square visualBody;

    visualBody.x = object.x;
    visualBody.y = object.y;
    visualBody.width = object.width;
    visualBody.height = object.height;
    visualBody.angle = object.angle;
    visualBody.color = object.color;
    visualBody.active = object.active;
    visualBody.tag = object.tag;

    DrawSquare(update, visualBody);
}

bool ApplyNormalReaction(dynamicSquare& dynamicObject, const dynamicSquare& staticObject) {
    if (!dynamicObject.active || !staticObject.active) return false;

    float p_halfW = dynamicObject.width / 2.0f;
    float p_halfH = dynamicObject.height / 2.0f;
    float f_halfW = staticObject.width / 2.0f;
    float f_halfH = staticObject.height / 2.0f;

    float p_left   = dynamicObject.x - p_halfW;
    float p_right  = dynamicObject.x + p_halfW;
    float p_top    = dynamicObject.y - p_halfH;
    float p_bottom = dynamicObject.y + p_halfH;

    float f_left   = staticObject.x - f_halfW;
    float f_right  = staticObject.x + f_halfW;
    float f_top    = staticObject.y - f_halfH;
    float f_bottom = staticObject.y + f_halfH;

    if (p_right > f_left && p_left < f_right && p_bottom > f_top && p_top < f_bottom) {
        float overlapX = (p_halfW + f_halfW) - fabsf(dynamicObject.x - staticObject.x);
        float overlapY = (p_halfH + f_halfH) - fabsf(dynamicObject.y - staticObject.y);

        if (overlapY < overlapX) {
            if (dynamicObject.y < staticObject.y) {
                dynamicObject.y -= overlapY;
                if (dynamicObject.vy > 0.0f) dynamicObject.vy = 0.0f;

                float normalX = 0.0f;
                float normalY = -1.0f;

                float gravityY = dynamicObject.mass * g;

                dynamicObject.currentN = fabsf(0.0f * normalX + gravityY * normalY);

                return true;
            }

            else if (dynamicObject.y > staticObject.y) {
                dynamicObject.y += overlapY;
                if (dynamicObject.vy < 0.0f) dynamicObject.vy = 0.0f;
            }
        }

        else {
            if (dynamicObject.x < staticObject.x) dynamicObject.x -= overlapX;
            else dynamicObject.x += overlapX;
            dynamicObject.vx = 0.0f;
            dynamicObject.fx = 0.0f;
        }
    }

    return false;
}

void ApplyForce(dynamicSquare& object, float fx, float fy) {
    if (!object.active) return;

    object.fx += fx;
    object.fy += fy;
}

void ApplyFriction(dynamicSquare& object) {
    if (!object.active) return;

    if (object.currentN > 0.001f && fabsf(object.vx) > 0.01f) {
        float frictionMagnitude = object.friction * object.currentN;

        if (object.vx > 0.0f) {
            object.fx -= frictionMagnitude;
        }

        else if (object.vx < 0.0f) {
            object.fx += frictionMagnitude;
        }
    }
}

void UpdateDynamicCollision(dynamicSquare& objectA, dynamicSquare& objectB) {
    if (!objectA.active || !objectB.active) return;

    float a_halfW = objectA.width / 2.0f;
    float a_halfH = objectA.height / 2.0f;
    float b_halfW = objectB.width / 2.0f;
    float b_halfH = objectB.height / 2.0f;

    float a_left   = objectA.x - a_halfW;
    float a_right  = objectA.x + a_halfW;
    float a_top    = objectA.y - a_halfH;
    float a_bottom = objectA.y + a_halfH;

    float b_left   = objectB.x - b_halfW;
    float b_right  = objectB.x + b_halfW;
    float b_top    = objectB.y - b_halfH;
    float b_bottom = objectB.y + b_halfH;

    if (a_right > b_left && a_left < b_right && a_bottom > b_top && a_top < b_bottom) {
        float overlapX = (a_halfW + b_halfW) - fabsf(objectA.x - objectB.x);
        float overlapY = (a_halfH + b_halfH) - fabsf(objectA.y - objectB.y);

        float normalX = 0.0f;
        float normalY = 0.0f;

        if (overlapY < overlapX) {
            normalY = (objectB.y > objectA.y) ? 1.0f : -1.0f;
            objectA.y -= normalY * (overlapY / 2.0f);
            objectB.y += normalY * (overlapY / 2.0f);
        }

        else {
            normalX = (objectB.x > objectA.x) ? 1.0f : -1.0f;

            objectA.x -= normalX * (overlapX / 2.0f);
            objectB.x += normalX * (overlapX / 2.0f);
        }

        float relVelX = objectB.vx - objectA.vx;
        float relVelY = objectB.vy - objectA.vy;

        float velAlongNormal = relVelX * normalX + relVelY * normalY;

        if (velAlongNormal > 0) return;

        float e = 0.8f;

        float impulseMagnitude = -(1.0f + e) * velAlongNormal;
        impulseMagnitude /= (1.0f / objectA.mass + 1.0f / objectB.mass);

        float impulseX = impulseMagnitude * normalX;
        float impulseY = impulseMagnitude * normalY;

        objectA.vx -= (1.0f / objectA.mass) * impulseX;
        objectA.vy -= (1.0f / objectA.mass) * impulseY;

        objectB.vx += (1.0f / objectB.mass) * impulseX;
        objectB.vy += (1.0f / objectB.mass) * impulseY;
    }
}


float KineticEnergy(dynamicSquare& object) {
    float totalVelocitySq = (object.vx * object.vx) + (object.vy * object.vy);
    return (object.mass * totalVelocitySq) / 2.0f;
}

float PotentialEnergy(dynamicSquare& object, float ground) {
    float h = ground - object.y;
    if (h < 0.0f) h = 0.0f;
    return object.mass * g * h;
}

float Work(float force, float distance) {
    return force * distance;
}

float PowerByForce(float force, float velocity) {
    return force * velocity;
}

float PowerByWork(float work, float time) {
    return work / time;
}

void ApplyArchimedesForce(dynamicSquare& object, const liquid& fluid) {
    if (!object.active || !fluid.active) return;

    float o_hw = object.width / 2.0f;
    float o_hh = object.height / 2.0f;

    float w_hw = fluid.width / 2.0f;
    float w_hh = fluid.height / 2.0f;

    float o_left = object.x - o_hw; float o_right = object.x + o_hw;
    float o_top  = object.y - o_hh; float o_bottom = object.y + o_hh;

    float w_left = fluid.x - w_hw; float w_right = fluid.x + w_hw;
    float w_top  = fluid.y - w_hh; float w_bottom = fluid.y + w_hh;

    if (o_right > w_left && o_left < w_right && o_bottom > w_top && o_top < w_bottom) {

        float overlapLeft   = (o_left > w_left) ? o_left : w_left;
        float overlapRight  = (o_right < w_right) ? o_right : w_right;
        float overlapTop    = (o_top > w_top) ? o_top : w_top;
        float overlapBottom = (o_bottom < w_bottom) ? o_bottom : w_bottom;

        float overlapW = overlapRight - overlapLeft;
        float overlapH = overlapBottom - overlapTop;

        float v_submerged = overlapW * overlapH;

        float fa = fluid.density * (g / 100.0f) * v_submerged;

        object.fy -= fa;

        object.fx -= object.vx * 2.0f * fluid.density * overlapH;
        object.fy -= object.vy * 2.0f * fluid.density * overlapW;
    }
}

void DrawLiquid(window& update, liquid& object) {
    if (!object.active) return;

    square visualBody;

    visualBody.x = object.x;
    visualBody.y = object.y;
    visualBody.width = object.width;
    visualBody.height = object.height;
    visualBody.color = object.color;
    visualBody.active = object.active;
    visualBody.tag = object.tag;

    DrawSquare(update, visualBody);
}

bool IsCollide(const dynamicSquare& objectA, const square& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const dynamicSquare& objectA, const dynamicSquare& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const kinematicSquare& objectA, const square& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const kinematicSquare& objectA, const kinematicSquare& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const dynamicSquare& objectA, const kinematicSquare& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const dynamicSquare& objectA, const liquid& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const kinematicSquare& objectA, const liquid& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCollide(const liquid& objectA, const liquid& objectB) {
    if (!objectA.active || !objectB.active) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

bool IsCursorTouched(window& update, dynamicSquare& object) {
    if (object.active == false) return false;

    POINT m;
    GetCursorPos(&m);
    ScreenToClient(update.hwnd, &m);

    float left   = object.x - (object.width / 2.0f);
    float right  = object.x + (object.width / 2.0f);
    float top    = object.y - (object.height / 2.0f);
    float bottom = object.y + (object.height / 2.0f);

    if (m.x >= left && m.x <= right && m.y >= top && m.y <= bottom) {
        return true;
    }

    return false;
}

bool IsCursorTouched(window& update, kinematicSquare& object) {
    if (object.active == false) return false;

    POINT m;
    GetCursorPos(&m);
    ScreenToClient(update.hwnd, &m);

    float left   = object.x - (object.width / 2.0f);
    float right  = object.x + (object.width / 2.0f);
    float top    = object.y - (object.height / 2.0f);
    float bottom = object.y + (object.height / 2.0f);

    if (m.x >= left && m.x <= right && m.y >= top && m.y <= bottom) {
        return true;
    }

    return false;
}

bool IsCursorTouched(window& update, liquid& object) {
    if (object.active == false) return false;

    POINT m;
    GetCursorPos(&m);
    ScreenToClient(update.hwnd, &m);

    float left   = object.x - (object.width / 2.0f);
    float right  = object.x + (object.width / 2.0f);
    float top    = object.y - (object.height / 2.0f);
    float bottom = object.y + (object.height / 2.0f);

    if (m.x >= left && m.x <= right && m.y >= top && m.y <= bottom) {
        return true;
    }

    return false;
}

void ApplyJoint(dynamicSquare& object, joint& anchor) {
    if (!object.active || !anchor.active) return;

    float dx = anchor.anchorX - object.x;
    float dy = anchor.anchorY - object.y;

    float currentLength = sqrtf(dx * dx + dy * dy);
    if (currentLength < 0.001f) return;

    float dirX = dx / currentLength;
    float dirY = dy / currentLength;

    if (anchor.type == spring) {
        float deltaX = currentLength - anchor.length;

        float criticalDamping = 2.0f * sqrtf(anchor.stiffness * object.mass);

        float actualDamping = criticalDamping * anchor.damping;

        float springForce = anchor.stiffness * deltaX;

        float velProj = object.vx * dirX + object.vy * dirY;
        float dampingForce = actualDamping * velProj;

        float totalForce = springForce + dampingForce;

        float maxSafeForce = 3000.0f;
        if (totalForce > maxSafeForce) totalForce = maxSafeForce;
        if (totalForce < -maxSafeForce) totalForce = -maxSafeForce;

        object.fx += totalForce * dirX;
        object.fy += totalForce * dirY;

        object.vx -= velProj * dirX * anchor.damping * deltaTime * 10.0f;
        object.vy -= velProj * dirY * anchor.damping * deltaTime * 10.0f;
    }

    else if (anchor.type == rope) {
        if (currentLength > anchor.length) {
            float overlap = currentLength - anchor.length;
            object.x += dirX * overlap;
            object.y += dirY * overlap;

            float velProj = object.vx * dirX + object.vy * dirY;
            if (velProj < 0.0f) {
                object.vx -= velProj * dirX;
                object.vy -= velProj * dirY;
            }
        }
    }

    else if (anchor.type == rod) {
        float overlap = currentLength - anchor.length;
        object.x += dirX * overlap;
        object.y += dirY * overlap;

        float velProj = object.vx * dirX + object.vy * dirY;
        object.vx -= velProj * dirX;
        object.vy -= velProj * dirY;

        object.vx *= (1.0f - anchor.damping * deltaTime);
        object.vy *= (1.0f - anchor.damping * deltaTime);
    }
}
