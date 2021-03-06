/*
    Copyright (c) 2007 SIEGE Development Team
    All rights reserved.

    This file is part of libSIEGE.

    This software is copyrighted work licensed under the terms of the
    2-clause BSD license. Please consult the file "license.txt" for
    details.

    If you did not recieve the file with this program, please email
    Tim Chas <darkuranium@gmail.com>.
*/

#ifndef __SIEGE_PHYSICS_BODY_H__
#define __SIEGE_PHYSICS_BODY_H__

#include "../common.h"
#include "space.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct SGEntity;

typedef struct SGPhysicsBody
{
    void* handle;
    SGPhysicsSpace* space;
    void* data;
    
    SGenum type;
    
    struct SGEntity* entity;
} SGPhysicsBody;

SGPhysicsBody* SG_EXPORT sgPhysicsBodyCreate(SGPhysicsSpace* space, SGenum type);
void SG_EXPORT sgPhysicsBodyDestroy(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetData(SGPhysicsBody* body, void* data);
void* SG_EXPORT sgPhysicsBodyGetData(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetSleeping(SGPhysicsBody* body, SGbool sleeping);
SGbool SG_EXPORT sgPhysicsBodyGetSleeping(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetPos(SGPhysicsBody* body, float x, float y);
void SG_EXPORT sgPhysicsBodyGetPos(SGPhysicsBody* body, float* x, float* y);

void SG_EXPORT sgPhysicsBodySetPosX(SGPhysicsBody* body, float x);
float SG_EXPORT sgPhysicsBodyGetPosX(SGPhysicsBody* body);
void SG_EXPORT sgPhysicsBodySetPosY(SGPhysicsBody* body, float y);
float SG_EXPORT sgPhysicsBodyGetPosY(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetAngleRads(SGPhysicsBody* body, float rads);
float SG_EXPORT sgPhysicsBodyGetAngleRads(SGPhysicsBody* body);
void SG_EXPORT sgPhysicsBodySetAngleDegs(SGPhysicsBody* body, float degs);
float SG_EXPORT sgPhysicsBodyGetAngleDegs(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetVel(SGPhysicsBody* body, float x, float y);
void SG_EXPORT sgPhysicsBodyGetVel(SGPhysicsBody* body, float* x, float* y);

void SG_EXPORT sgPhysicsBodySetVelX(SGPhysicsBody* body, float x);
float SG_EXPORT sgPhysicsBodyGetVelX(SGPhysicsBody* body);
void SG_EXPORT sgPhysicsBodySetVelY(SGPhysicsBody* body, float y);
float SG_EXPORT sgPhysicsBodyGetVelY(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetAngVelRads(SGPhysicsBody* body, float rads);
float SG_EXPORT sgPhysicsBodyGetAngVelRads(SGPhysicsBody* body);
void SG_EXPORT sgPhysicsBodySetAngVelDegs(SGPhysicsBody* body, float degs);
float SG_EXPORT sgPhysicsBodyGetAngVelDegs(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetMass(SGPhysicsBody* body, float mass);
float SG_EXPORT sgPhysicsBodyGetMass(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodySetMoment(SGPhysicsBody* body, float moment);
float SG_EXPORT sgPhysicsBodyGetMoment(SGPhysicsBody* body);

void SG_EXPORT sgPhysicsBodyApplyImpulse(SGPhysicsBody* body, float jx, float jy, float rx, float ry);
void SG_EXPORT sgPhysicsBodyApplyForce(SGPhysicsBody* body, float jx, float jy, float rx, float ry);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_PHYSICS_BODY_H__
