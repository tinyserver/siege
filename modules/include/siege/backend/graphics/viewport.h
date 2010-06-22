#ifndef __SIEGE_GRAPHICS_VIEWPORT_H__
#define __SIEGE_GRAPHICS_VIEWPORT_H__

#include "../../common.h"

#ifdef SG_LEGACY_API
#    define sgGraphicsViewportCreate sgmGraphicsViewportCreate
//#    define sgGraphicsViewportCreateView sgmGraphicsViewportCreateView
#    define sgGraphicsViewportDestroy sgmGraphicsViewportDestroy
#    define sgGraphicsViewportSetView sgmGraphicsViewportSetView
//#    define sgGraphicsViewportGetView sgmGraphicsViewportGetView
#endif // SG_LEGACY_API

#ifdef __cplusplus
extern "C"
{
#endif
    SGuint SG_EXPORT sgmGraphicsViewportCreate(void** viewport, void* context);
    //SGuint SG_EXPORT sgmGraphicsViewportCreateView(void** viewport, void* context, SGuint wx, SGuint wy, SGuint wwidth, SGuint wheight, float x, float y, float width, float height);
    SGuint SG_EXPORT sgmGraphicsViewportDestroy(void* viewport);
    SGuint SG_EXPORT sgmGraphicsViewportSetView(void* viewport, SGuint wx, SGuint wy, SGuint wwidth, SGuint wheight, float x, float y, float width, float height);
    //SGuint SG_EXPORT sgmGraphicsViewportGetView(void* viewport, SGuint* wx, SGuint* wy, SGuint* wwidth, SGuint* wheight, float* x, float* y, float* width, float* height);
#ifdef __cplusplus
}
#endif

#endif // __SIEGE_GRAPHICS_VIEWPORT_H__
