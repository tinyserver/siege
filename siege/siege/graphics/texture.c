#define SG_BUILD_LIBRARY
#include <siege/graphics/texture.h>
#include <siege/core/window.h>
#include <siege/modules/graphics.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

SGTexture* SG_EXPORT sgTextureCreateFile(char* fname)
{
    SGuint width;
    SGuint height;
    SGuint bpp;
    void* data;

    SGuint ret;
    if(_sg_modGraphics.sgmGraphicsLoadFile != NULL)
        ret = _sg_modGraphics.sgmGraphicsLoadFile(fname, &width, &height, &bpp, &data);
    if((_sg_modGraphics.sgmGraphicsLoadFile == NULL) || (ret != SG_OK))
        fprintf(stderr, "Could not load image %s\n", fname);

    SGTexture* texture = sgTextureCreateData(width, height, bpp, data);
    if(_sg_modGraphics.sgmGraphicsLoadFreeData != NULL)
        _sg_modGraphics.sgmGraphicsLoadFreeData(data);
    return texture;
}
SGTexture* SG_EXPORT sgTextureCreateData(SGuint width, SGuint height, SGenum bpp, void* data)
{
    SGTexture* texture = malloc(sizeof(SGTexture));
    if(texture == NULL)
        return NULL;

    if(data != NULL)
        data = _sgGraphicsToCanvasSize(width, height, bpp, data);

    if(_sg_modGraphics.sgmGraphicsTextureCreate != NULL)
        _sg_modGraphics.sgmGraphicsTextureCreate(&texture->handle, _sg_gfxHandle);
    if(_sg_modGraphics.sgmGraphicsTextureSetData != NULL)
        _sg_modGraphics.sgmGraphicsTextureSetData(texture->handle, width, height, bpp, data);

    // as allocated by _sgGraphicsToCanvasSize (or else it's null, and we don't care)
    free(data);
    return texture;
}
SGTexture* SG_EXPORT sgTextureCreate(SGuint width, SGuint height, SGenum bpp)
{
    return sgTextureCreateData(width, height, bpp, NULL);
}
void SG_EXPORT sgTextureDestroy(SGTexture* texture)
{
    if(_sg_modGraphics.sgmGraphicsTextureDestroy != NULL)
        _sg_modGraphics.sgmGraphicsTextureDestroy(texture->handle);
    free(texture);
}

void SG_EXPORT sgTextureDrawRads3f2f2f1f(SGTexture* texture, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle)
{
    if(_sg_modGraphics.sgmGraphicsTextureDraw != NULL)
        _sg_modGraphics.sgmGraphicsTextureDraw(texture->handle, x, y, z, xscale, yscale, xoffset, yoffset, angle);
}
void SG_EXPORT sgTextureDrawDegs3f2f2f1f(SGTexture* texture, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle)
{
    sgTextureDrawRads3f2f2f1f(texture, x, y, z, xscale, yscale, xoffset, yoffset, angle * M_PI / 180.0);
}
void SG_EXPORT sgTextureDrawRads3f2f1f(SGTexture* texture, float x, float y, float z, float xscale, float yscale, float angle)
{
    sgTextureDrawRads3f2f2f1f(texture, x, y, z, xscale, yscale, 0.0, 0.0, angle);
}
void SG_EXPORT sgTextureDrawDegs3f2f1f(SGTexture* texture, float x, float y, float z, float xscale, float yscale, float angle)
{
    sgTextureDrawDegs3f2f2f1f(texture, x, y, z, xscale, yscale, 0.0, 0.0, angle);
}
/*void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y, float xscale, float yscale, float angle);
void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y, float z, float xscale, float yscale);
void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y, float xscale, float yscale);
void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y, float z, float angle);
void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y, float angle);
void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y, float z);
void SG_EXPORT sgTextureDraw(SGTexture* texture, float x, float y);*/
void SG_EXPORT sgTextureDraw(SGTexture* texture)
{
    sgTextureDrawRads3f2f2f1f(texture, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void SG_EXPORT sgTextureGetSize(SGTexture* texture, SGuint* width, SGuint* height)
{
    if((width == NULL) && (height == NULL))
        return;
    SGuint buf;

    // make sure we don't pass any nulls
    if(width == NULL)
        width = &buf;
    if(height == NULL)
        height = &buf;

    if(_sg_modGraphics.sgmGraphicsTextureGetSize != NULL)
        _sg_modGraphics.sgmGraphicsTextureGetSize(texture->handle, width, height);
}
SGuint SG_EXPORT sgTextureGetWidth(SGTexture* texture)
{
    SGuint width;
    SGuint height;
    if(_sg_modGraphics.sgmGraphicsTextureGetSize != NULL)
        _sg_modGraphics.sgmGraphicsTextureGetSize(texture->handle, &width, &height);
    return width;
}
SGuint SG_EXPORT sgTextureGetHeight(SGTexture* texture)
{
    SGuint width;
    SGuint height;
    if(_sg_modGraphics.sgmGraphicsTextureGetSize != NULL)
        _sg_modGraphics.sgmGraphicsTextureGetSize(texture->handle, &width, &height);
    return height;
}
