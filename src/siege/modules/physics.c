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

#define SG_BUILD_LIBRARY
#include <siege/util/link.h>
#include <siege/modules/modules.h>
#include <siege/modules/physics.h>

void SG_EXPORT _sgModuleLoadPhysics(SGLibrary* lib)
{
    void* fptr;

    /// Physics
    _SG_BIND(sgmPhysicsBodyCreate);
    _SG_BIND(sgmPhysicsBodyDestroy);

    _SG_BIND(sgmPhysicsBodySetMass);
    _SG_BIND(sgmPhysicsBodyGetMass);
    _SG_BIND(sgmPhysicsBodySetMoment);
    _SG_BIND(sgmPhysicsBodyGetMoment);
    _SG_BIND(sgmPhysicsBodySetPosition);
    _SG_BIND(sgmPhysicsBodyGetPosition);
    _SG_BIND(sgmPhysicsBodySetVelocity);
    _SG_BIND(sgmPhysicsBodyGetVelocity);
    _SG_BIND(sgmPhysicsBodySetForce);
    _SG_BIND(sgmPhysicsBodyGetForce);
    _SG_BIND(sgmPhysicsBodySetAngle);
    _SG_BIND(sgmPhysicsBodyGetAngle);
    _SG_BIND(sgmPhysicsBodySetAngularVelocity);
    _SG_BIND(sgmPhysicsBodyGetAngularVelocity);
    _SG_BIND(sgmPhysicsBodySetTorque);
    _SG_BIND(sgmPhysicsBodyGetTorque);
    _SG_BIND(sgmPhysicsBodySetData);
    _SG_BIND(sgmPhysicsBodyGetData);

    // constraint

    _SG_BIND(sgmPhysicsShapeCreate);
    _SG_BIND(sgmPhysicsShapeDestroy);

    _SG_BIND(sgmPhysicsSpaceCreate);
    _SG_BIND(sgmPhysicsSpaceDestroy);
    _SG_BIND(sgmPhysicsSpaceStep);
    _SG_BIND(sgmPhysicsSpaceSetGravity);

    _SG_BIND(sgmPhysicsSpaceAddShape);
    _SG_BIND(sgmPhysicsSpaceRemoveShape);
    //_SG_BIND(sgmPhysicsSpaceAddStaticShape);
    //_SG_BIND(sgmPhysicsSpaceRemoveStaticShape);
    _SG_BIND(sgmPhysicsSpaceAddBody);
    _SG_BIND(sgmPhysicsSpaceRemoveBody);
    _SG_BIND(sgmPhysicsSpaceAddConstraint);
    _SG_BIND(sgmPhysicsSpaceRemoveConstraint);

    _SG_BIND(sgmPhysicsBodyWorldToLocal_TEST);
    _SG_BIND(sgmPhysicsBodyLocalToWorld_TEST);
    _SG_BIND(sgmPhysicsShapeGetBB_TEST);
    _SG_BIND(sgmPhysicsShapeGetPoints_TEST);
    _SG_BIND(sgmPhysicsShapeFreePoints_TEST);
}
