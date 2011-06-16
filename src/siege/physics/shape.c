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
#include <siege/physics/shape.h>
#include <siege/modules/physics.h>

#include <siege/util/vector.h>

//
#include <siege/graphics/draw.h>
//

#include <stdlib.h>
#include <string.h>
#include <math.h>

SGPhysicsShape* SG_EXPORT sgPhysicsShapeCreate(SGPhysicsBody* body, SGenum type)
{
    SGPhysicsShape* shape = malloc(sizeof(SGPhysicsShape));
    if(shape == NULL)
        return NULL;

    shape->body = body;
    shape->type = type;

    shape->x = SG_NAN;
    shape->y = SG_NAN;
    shape->numverts = 0;
    shape->verts = NULL;

    return shape;
}
SGPhysicsShape* SG_EXPORT sgPhysicsShapeCreateSegment(SGPhysicsBody* body, float x1, float y1, float x2, float y2, float width)
{
    SGPhysicsShape* shape = sgPhysicsShapeCreate(body, SG_PHYSICS_SHAPE_SEGMENT);
    if(shape == NULL)
        return NULL;

    shape->x = 0;
    shape->y = 0;

    shape->numverts = 2;
    shape->verts = malloc(5 * sizeof(float));
    shape->verts[0] = x1;
    shape->verts[1] = y1;
    shape->verts[2] = x2;
    shape->verts[3] = y2;
    shape->verts[4] = width;

    if(sgmPhysicsShapeCreate != NULL)
        sgmPhysicsShapeCreate(&shape->handle, body->handle, 0, 0, shape->type, shape->numverts, shape->verts);
    if(sgmPhysicsSpaceAddShape != NULL)
        sgmPhysicsSpaceAddShape(body->space->handle, shape->handle);

    return shape;
}
SGPhysicsShape* SG_EXPORT sgPhysicsShapeCreatePoly(SGPhysicsBody* body, float x, float y, float* verts, size_t numverts)
{
    SGPhysicsShape* shape = sgPhysicsShapeCreate(body, SG_PHYSICS_SHAPE_POLYGON);
    if(shape == NULL)
        return NULL;

    shape->x = x;
    shape->y = y;

    shape->numverts = numverts;
    shape->verts = malloc(2 * numverts * sizeof(float));
    memcpy(shape->verts, verts, 2 * numverts * sizeof(float));

    if(sgmPhysicsShapeCreate != NULL)
        sgmPhysicsShapeCreate(&shape->handle, body->handle, x, y, shape->type, shape->numverts, shape->verts);
    if(sgmPhysicsSpaceAddShape != NULL)
        sgmPhysicsSpaceAddShape(body->space->handle, shape->handle);

    return shape;
}
SGPhysicsShape* SG_EXPORT sgPhysicsShapeCreateCircle(SGPhysicsBody* body, float x, float y, float r1, float r2)
{
    SGPhysicsShape* shape = sgPhysicsShapeCreate(body, SG_PHYSICS_SHAPE_CIRCLE);
    if(shape == NULL)
        return NULL;

    if(r2 < r1)
    {
        float tmp = r1;
        r1 = r2;
        r2 = tmp;
    }

    shape->x = x;
    shape->y = y;

    shape->numverts = 1;
    shape->verts = malloc(2 * sizeof(float));
    shape->verts[0] = r1;
    shape->verts[1] = r2;

    if(sgmPhysicsShapeCreate != NULL)
        sgmPhysicsShapeCreate(&shape->handle, body->handle, x, y, shape->type, shape->numverts, &shape->verts[1]);
    if(sgmPhysicsSpaceAddShape != NULL)
        sgmPhysicsSpaceAddShape(body->space->handle, shape->handle);

    return shape;
}
void SG_EXPORT sgPhysicsShapeDestroy(SGPhysicsShape* shape)
{
    if(shape == NULL)
        return;

    if(sgmPhysicsSpaceRemoveShape != NULL)
        sgmPhysicsSpaceRemoveShape(shape->body->space->handle, shape->handle);
    if(sgmPhysicsShapeDestroy != NULL)
        sgmPhysicsShapeDestroy(shape->handle);

    free(shape->verts);

    free(shape);
}

float SG_EXPORT sgPhysicsShapeGetAreaS(SGPhysicsShape* shape)
{
    if(shape == NULL)
        return SG_NAN;

    SGVec2 curr;
    SGVec2 next;
    float area;
    size_t i;

    switch(shape->type)
    {
        case SG_PHYSICS_SHAPE_SEGMENT:
            return hypot(shape->verts[0] - shape->verts[2], shape->verts[1] - shape->verts[3]) * shape->verts[4];

        case SG_PHYSICS_SHAPE_POLYGON:
            area = 0;
            for(i = 0; i < shape->numverts; i++)
            {
                curr.x = shape->verts[2*i];
                curr.y = shape->verts[2*i+1];
                next.x = shape->verts[2*((i+1) % shape->numverts)];
                next.y = shape->verts[2*((i+1) % shape->numverts)+1];

                area += sgVec2Cross(curr, next);
            }
            return area / 2.0;

        case SG_PHYSICS_SHAPE_CIRCLE:
            return SG_PI * (shape->verts[1] * shape->verts[1] - shape->verts[0] * shape->verts[0]);
    }
    return SG_NAN;
}
float SG_EXPORT sgPhysicsShapeGetAreaU(SGPhysicsShape* shape)
{
    return fabs(sgPhysicsShapeGetAreaS(shape));
}
float SG_EXPORT sgPhysicsShapeGetMass(SGPhysicsShape* shape, float density)
{
    return sgPhysicsShapeGetAreaU(shape) * density;
}
float SG_EXPORT sgPhysicsShapeGetMomentMass(SGPhysicsShape* shape, float mass)
{
    if(shape == NULL)
        return SG_NAN;

    SGVec2 curr;
    SGVec2 next;
    float nom;
    float den;
    size_t i;

    switch(shape->type)
    {
        case SG_PHYSICS_SHAPE_SEGMENT:
            return mass * (((shape->verts[0] - shape->verts[2]) * (shape->verts[0] - shape->verts[2]) -
                            (shape->verts[1] - shape->verts[3]) * (shape->verts[1] - shape->verts[3])) / 12.0 +
                           ((shape->verts[0] - shape->verts[2]) * (shape->verts[0] - shape->verts[2]) +
                            (shape->verts[1] - shape->verts[3]) * (shape->verts[1] - shape->verts[3])) / 2.0);

        case SG_PHYSICS_SHAPE_POLYGON:
            nom = 0;
            den = 0;
            for(i = 0; i < shape->numverts; i++)
            {
                curr.x = shape->verts[2*i] - shape->x;
                curr.y = shape->verts[2*i+1] - shape->y;
                next.x = shape->verts[2*((i+1) % shape->numverts)] - shape->x;
                next.y = shape->verts[2*((i+1) % shape->numverts)+1] - shape->y;

                nom += fabs(sgVec2Cross(curr, next)) * (sgVec2GetLength2(next) + sgVec2Dot(next, curr) + sgVec2GetLength2(curr));
                den += fabs(sgVec2Cross(curr, next));
            }
            return nom / den * mass / 6.0;

        case SG_PHYSICS_SHAPE_CIRCLE:
            return mass * (shape->verts[0] * shape->verts[0] + shape->verts[1] * shape->verts[1]) / 2.0;
    }
    return SG_NAN;
}
float SG_EXPORT sgPhysicsShapeGetMomentDensity(SGPhysicsShape* shape, float density)
{
    if(shape == NULL)
        return SG_NAN;

    SGVec2 curr;
    SGVec2 next;
    float moment;
    size_t i;

    switch(shape->type)
    {
        case SG_PHYSICS_SHAPE_SEGMENT:
            return sgPhysicsShapeGetMomentMass(shape, sgPhysicsShapeGetMass(shape, density));

        case SG_PHYSICS_SHAPE_POLYGON:
            moment = 0;
            for(i = 0; i < shape->numverts; i++)
            {
                curr.x = shape->verts[2*i] - shape->x;
                curr.y = shape->verts[2*i+1] - shape->y;
                next.x = shape->verts[2*((i+1) % shape->numverts)] - shape->x;
                next.y = shape->verts[2*((i+1) % shape->numverts)+1] - shape->y;

                moment += density / 12.0 * fabs(sgVec2Cross(curr, next)) * (sgVec2GetLength2(next) + sgVec2Dot(next, curr) + sgVec2GetLength2(curr));
            }
            return moment;

        case SG_PHYSICS_SHAPE_CIRCLE:
            return sgPhysicsShapeGetMomentMass(shape, sgPhysicsShapeGetMass(shape, density));
    }
    return SG_NAN;
}

void SG_EXPORT sgPhysicsShapeDrawDBG(SGPhysicsShape* shape)
{
    if(shape == NULL)
        return;

    // draw BB
    float t, l, b, r;
    if(sgmPhysicsShapeGetBB_TEST != NULL)
        sgmPhysicsShapeGetBB_TEST(shape->handle, &t, &l, &b, &r);

    sgDrawColor4f(0.5, 0.5, 0.5, 0.5);
    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_LINE_LOOP);
        sgDrawVertex2f(l, t);
        sgDrawVertex2f(r, t);
        sgDrawVertex2f(r, b);
        sgDrawVertex2f(l, b);
    sgDrawEnd();

    sgDrawColor4f(0.0, 0.5, 0.75, 0.75);
    {
        float angle = sgPhysicsBodyGetAngleRads(shape->body);
        float x, y;
        sgPhysicsBodyGetPos(shape->body, &x, &y);
        float ta;
        float tl;
        float tx[2];
        float ty[2];
        if(shape->type == SG_PHYSICS_SHAPE_SEGMENT)
        {
            ta = atan2(shape->y + shape->verts[1], shape->x + shape->verts[0]);
            tl = hypot(shape->y + shape->verts[1], shape->x + shape->verts[0]);
            tx[0] = cos(ta + angle) * tl;
            ty[0] = sin(ta + angle) * tl;
            ta = atan2(shape->y + shape->verts[3], shape->x + shape->verts[2]);
            tl = hypot(shape->y + shape->verts[3], shape->x + shape->verts[2]);
            tx[1] = cos(ta + angle) * tl;
            ty[1] = sin(ta + angle) * tl;

            sgDrawLineSetWidth(shape->verts[4]);
            sgDrawLine(x + tx[0], y + ty[0], x + tx[1], y + ty[1]);
            sgDrawLineSetWidth(1.0);
            return;
        }
        else if(shape->type == SG_PHYSICS_SHAPE_CIRCLE)
        {
            //sgDrawLineSetWidth(fabs(shape->verts[1] - shape->verts[0])*2.0);
            //sgDrawCircle(x, y, (shape->verts[0] + shape->verts[1]) / 2.0, SG_FALSE);
            //sgDrawLineSetWidth(1.0);
            
            ta = atan2(shape->y, shape->x);
            tl = hypot(shape->y, shape->x);
            tx[0] = cos(ta + angle) * tl;
            ty[0] = sin(ta + angle) * tl;
            sgDrawCircle(x + tx[0], y + ty[0], shape->verts[0], SG_FALSE);
            sgDrawCircle(x + tx[0], y + ty[0], shape->verts[1], SG_FALSE);
            
            x += tx[0];
            y += ty[0];
            
            //ta = atan2(ty[0], tx[0]);
            //tl = hypot(ty[0], tx[0]);
            tx[0] = cos(angle) * shape->verts[0];
            ty[0] = sin(angle) * shape->verts[0];
            tx[1] = cos(angle) * shape->verts[1];
            ty[1] = sin(angle) * shape->verts[1];
            sgDrawLine(x + tx[0], y + ty[0], x + tx[1], y + ty[1]);
            sgDrawLine(x - tx[0], y - ty[0], x, y);
            return;
        }
    }

    // draw shape
    SGuint pnum;
    float* points;
    if(sgmPhysicsBodyLocalToWorld_TEST == NULL)
        return;
    if(sgmPhysicsShapeGetPoints_TEST != NULL)
        sgmPhysicsShapeGetPoints_TEST(shape->handle, &pnum, &points);

    SGuint i;
    for(i = 0; i < pnum; i++)
        sgmPhysicsBodyLocalToWorld_TEST(shape->body->handle, &points[2*i], &points[2*i+1]);

    sgDrawBegin(SG_GRAPHICS_PRIMITIVE_LINE_LOOP);
        for(i = 0; i < pnum; i++)
            sgDrawVertex2f(points[2*i], points[2*i+1]);
    sgDrawEnd();
    sgDrawColor4f(1.0, 1.0, 1.0, 1.0);

    if(sgmPhysicsShapeFreePoints_TEST != NULL)
        sgmPhysicsShapeFreePoints_TEST(points);
}
