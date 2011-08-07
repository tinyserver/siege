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

#ifndef __SIEGE_BACKEND_PHYSICS_SHAPE_H__
#define __SIEGE_BACKEND_PHYSICS_SHAPE_H__

#include "../../common.h"

#ifndef SG_FPTR
#define SG_FPTR(name)	name
#endif // SG_FPTR

#ifdef __cplusplus
extern "C"
{
#endif
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeCreate)(void** shape, void* body, float xoffset, float yoffset, SGenum type, size_t numverts, float* verts);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeDestroy)(void* shape);
    
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeSetGroup)(void* shape, SGuint group);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeGetGroup)(void* shape, SGuint* group);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeSetFriction)(void* shape, float friction);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeGetFriction)(void* shape, float* friction);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeSetRestitution)(void* shape, float restitution);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeGetRestitution)(void* shape, float* restitution);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeSetData)(void* shape, void* data);
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeGetData)(void* shape, void** data);
    
    SGenum SG_EXPORT SG_FPTR(sgmPhysicsShapeGetBBox)(void* shape, float* t, float* l, float* b, float* r);
#ifdef __cplusplus
}
#endif

#endif // __SIEGE_BACKEND_PHYSICS_SHAPE_H__
