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
#include <siege/physics/space.h>
#include <siege/modules/physics.h>

#include <stdlib.h>

SGbool SG_EXPORT _sgPhysicsSpaceInit(void)
{
    _sg_physSpaceMain = sgPhysicsSpaceCreate();
    sgPhysicsSpaceSetGravity(_sg_physSpaceMain, 0, 10);
    return SG_TRUE;
}
SGbool SG_EXPORT _sgPhysicsSpaceDeinit(void)
{
    sgPhysicsSpaceDestroy(_sg_physSpaceMain);
    return SG_TRUE;
}

SGPhysicsSpace* SG_EXPORT sgPhysicsSpaceCreate(void)
{
    SGPhysicsSpace* space = malloc(sizeof(SGPhysicsSpace));
    if(sgmPhysicsSpaceCreate != NULL)
        sgmPhysicsSpaceCreate(&space->handle);

    sgPhysicsSpaceSetGravity(space, 0.0, 0.0);

    return space;
}
void SG_EXPORT sgPhysicsSpaceDestroy(SGPhysicsSpace* space)
{
    if(space == NULL)
        return;

    if(sgmPhysicsSpaceDestroy != NULL)
        sgmPhysicsSpaceDestroy(space->handle);

    free(space);
}

void SG_EXPORT sgPhysicsSpaceStep(SGPhysicsSpace* space, float time)
{
    if(space == NULL)
        return;

    if(sgmPhysicsSpaceStep != NULL)
        sgmPhysicsSpaceStep(space->handle, time);
}
void SG_EXPORT sgPhysicsSpaceSetGravity(SGPhysicsSpace* space, float x, float y)
{
    if(space == NULL)
        return;

    if(sgmPhysicsSpaceSetGravity != NULL)
        sgmPhysicsSpaceSetGravity(space->handle, x, y);
}
//void SG_EXPORT sgPhysicsSpaceGetGravity(SGPhysicsSpace* space, float* x, float* y);
