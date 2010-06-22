#ifndef __SIEGE_BACKEND_GRAPHICS_TEXTURE_H__
#define __SIEGE_BACKEND_GRAPHICS_TEXTURE_H__

#include "../../common.h"

#ifdef __cplusplus
extern "C"
{
#endif
    SGuint SG_EXPORT sgmGraphicsTextureCreate(void** texture, void* context);
    //SGuint SG_EXPORT sgmGraphicsTextureCreateData(void** texture, void* context, SGuint width, SGuint height, SGuint bpp, void* data);
    SGuint SG_EXPORT sgmGraphicsTextureDestroy(void* texture);
    SGuint SG_EXPORT sgmGraphicsTextureSetData(void* texture, SGuint width, SGuint height, SGuint bpp, void* data);
    //SGuint SG_EXPORT sgmGraphicsTextureGetData(void* texture, SGuint* width, SGuint* height, SGuint* bpp, void** data);
    //SGuint SG_EXPORT sgmGraphicsTextureFreeData(void* data);
    SGuint SG_EXPORT sgmGraphicsTextureGetSize(void* texture, SGuint* width, SGuint* height);
    SGuint SG_EXPORT sgmGraphicsTextureDraw(void* texture, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle);
#ifdef __cplusplus
}
#endif

#endif // __SIEGE_BACKEND_GRAPHICS_TEXTURE_H__
