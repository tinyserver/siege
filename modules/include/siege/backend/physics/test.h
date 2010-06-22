#ifndef __SIEGE_PHYSICS_TEST_H__
#define __SIEGE_PHYSICS_TEST_H__

#include "../../common.h"

#ifdef SG_LEGACY_API
#    define sgPhysicsBodyWorldToLocal_TEST sgmPhysicsBodyWorldToLocal_TEST
#    define sgPhysicsBodyLocalToWorld_TEST sgmPhysicsBodyLocalToWorld_TEST
#    define sgPhysicsShapeGetBB_TEST sgmPhysicsShapeGetBB_TEST
#    define sgPhysicsShapeGetPoints_TEST sgmPhysicsShapeGetPoints_TEST
#    define sgPhysicsShapeFreePoints_TEST sgmPhysicsShapeFreePoints_TEST
#endif // SG_LEGACY_API

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef SIEGE_TEST
    SGuint SG_EXPORT sgmPhysicsBodyWorldToLocal_TEST(void* body, float* x, float* y);
    SGuint SG_EXPORT sgmPhysicsBodyLocalToWorld_TEST(void* body, float* x, float* y);
    SGuint SG_EXPORT sgmPhysicsShapeGetBB_TEST(void* shape, float* t, float* l, float* b, float* r);
    SGuint SG_EXPORT sgmPhysicsShapeGetPoints_TEST(void* shape, SGuint* pnum, float** points);
    SGuint SG_EXPORT sgmPhysicsShapeFreePoints_TEST(float* points);
#endif // SIEGE_TEST

#ifdef __cplusplus
}
#endif

#endif // __SIEGE_PHYSICS_TEST_H__
