#include "main.h"
#include "space.h"

#include <stdio.h>
#include <stdlib.h>

SGuint SG_EXPORT sgmPhysicsSpaceCreate(void** space)
{
    *space = cpSpaceNew();
    cpSpace** cspace = (cpSpace**)space;
    (*cspace)->iterations = 10; // TEST
    (*cspace)->elasticIterations = 10;
    (*cspace)->damping = 0.75;
    return SG_OK;
}

SGuint SG_EXPORT sgmPhysicsSpaceDestroy(void* space)
{
    if(space == NULL)
        return SG_OK; // SG_INVALID_VALUE

    //cpSpaceFreeChildren(space);
    cpSpaceFree(space);
    return SG_OK;
}

SGuint SG_EXPORT sgmPhysicsSpaceStep(void* space, float time)
{
    if(space == NULL)
        return SG_OK; // SG_INVALID_VALUE

    cpSpaceStep(space, time);
    return SG_OK;
}

SGuint SG_EXPORT sgmPhysicsSpaceSetGravity(void* space, float x, float y)
{
    if(space == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpSpace* cspace = (cpSpace*)space;
    cspace->gravity.x = x;
    cspace->gravity.y = y;
    return SG_OK;
}
//SGuint SG_EXPORT sgmPhysicsSpaceGetGravity(void* space, float* x, float* y);

SGuint SG_EXPORT sgmPhysicsSpaceAddShape(void* space, void* shape)
{
    if(space == NULL || shape == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpShape* cshape = (cpShape*)shape;
    cpBody* cbody = cshape->body;
    if(((DataExt*)cbody->data)->type != SG_PHYSICS_BODY_STATIC)
        cpSpaceAddShape(space, shape);
    else
        cpSpaceAddStaticShape(space, shape);
    return SG_OK;
}
SGuint SG_EXPORT sgmPhysicsSpaceRemoveShape(void* space, void* shape)
{
    if(space == NULL || shape == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpShape* cshape = (cpShape*)shape;
    cpBody* cbody = cshape->body;
    if(((DataExt*)cbody->data)->type != SG_PHYSICS_BODY_STATIC)
        cpSpaceRemoveShape(space, shape);
    else
        cpSpaceRemoveStaticShape(space, shape);
    return SG_OK;
}
//SGuint SG_EXPORT sgmPhysicsSpaceAddStaticShape(void* space, void* shape);
//SGuint SG_EXPORT sgmPhysicsSpaceRemoveStaticShape(void* space, void* shape);
SGuint SG_EXPORT sgmPhysicsSpaceAddBody(void* space, void* body)
{
    if(space == NULL || body == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpBody* cbody = (cpBody*)body;
    if((((DataExt*)cbody->data)->type != SG_PHYSICS_BODY_STATIC) && (((DataExt*)cbody->data)->type != SG_PHYSICS_BODY_SEMISTATIC)) // we shouldn't put the body in, if it's static
        cpSpaceAddBody(space, body);
    return SG_OK;
}
SGuint SG_EXPORT sgmPhysicsSpaceRemoveBody(void* space, void* body)
{
    if(space == NULL || body == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpBody* cbody = (cpBody*)body;
    if((((DataExt*)cbody->data)->type != SG_PHYSICS_BODY_STATIC) && (((DataExt*)cbody->data)->type != SG_PHYSICS_BODY_SEMISTATIC)) // we shouldn't put the body in, if it's static
        cpSpaceRemoveBody(space, body);
    return SG_OK;
}
SGuint SG_EXPORT sgmPhysicsSpaceAddConstraint(void* space, void* constraint)
{
    if(space == NULL || constraint == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpSpaceAddConstraint(space, constraint);
    return SG_OK;
}
SGuint SG_EXPORT sgmPhysicsSpaceRemoveConstraint(void* space, void* constraint)
{
    if(space == NULL || constraint == NULL)
        return SG_OK; // SG_INVALID_VALUE
    cpSpaceRemoveConstraint(space, constraint);
    return SG_OK;
}
