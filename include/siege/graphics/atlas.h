/*
 * Copyright (c) 2007 SIEGE Development Team
 * All rights reserved.
 *
 * This file is part of libSIEGE.
 *
 * This software is copyrighted work licensed under the terms of the
 * 2-clause BSD license. Please consult the file "COPYING.txt" for
 * details.
 *
 * If you did not recieve the file with this program, please email
 * Tim Chas <darkuranium@gmail.com>.
 */

#ifndef __SIEGE_GRAPHICS_ATLAS_H__
#define __SIEGE_GRAPHICS_ATLAS_H__

#include "../common.h"
#include "texture.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct SGAtlasArea
{
    SGushort x, y, w, h; // TODO: use (not yet existing) SGRect for this?
    size_t index;
} SGAtlasArea;

typedef struct SGAtlasNode
{
    struct SGAtlasNode* child[2];
    SGAtlasArea area;
    SGbool reserved;
} SGAtlasNode;

typedef struct SGAtlasTexture
{
    SGTexture* texture;
    SGAtlasNode* root;
    SGbool owner;
} SGAtlasTexture;

typedef struct SGAtlas
{
    size_t width;
    size_t height;
    SGenum bpp;
    size_t numtextures;
    SGAtlasTexture* textures;
} SGAtlas;

SGAtlas* SG_EXPORT sgAtlasCreate(size_t width, size_t height, SGenum bpp);
SGAtlas* SG_EXPORT sgAtlasCreateData(size_t width, size_t height, SGenum bpp, void* data);
SGAtlas* SG_EXPORT sgAtlasCreateTexture(SGTexture* texture, SGbool owner);
SGAtlas* SG_EXPORT sgAtlasCreateFile(const char* fname);
void SG_EXPORT sgAtlasDestroy(SGAtlas* atlas);

SGAtlasArea* SG_EXPORT sgAtlasAreaReserve(SGAtlas* atlas, size_t width, size_t height, SGbool overflow);
void SG_EXPORT sgAtlasAreaSetData(SGAtlas* atlas, SGAtlasArea* area, size_t width, size_t height, SGenum bpp, void* data);

void SG_EXPORT sgAtlasAreaDrawRads3f2f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle);
void SG_EXPORT sgAtlasAreaDrawDegs3f2f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle);
void SG_EXPORT sgAtlasAreaDrawRads2f2f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float xscale, float yscale, float xoffset, float yoffset, float angle);
void SG_EXPORT sgAtlasAreaDrawDegs2f2f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float xscale, float yscale, float xoffset, float yoffset, float angle);
void SG_EXPORT sgAtlasAreaDrawRads3f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float xscale, float yscale, float angle);
void SG_EXPORT sgAtlasAreaDrawDegs3f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float xscale, float yscale, float angle);
void SG_EXPORT sgAtlasAreaDrawRads2f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float xscale, float yscale, float angle);
void SG_EXPORT sgAtlasAreaDrawDegs2f2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float xscale, float yscale, float angle);
void SG_EXPORT sgAtlasAreaDrawRads3f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float angle);
void SG_EXPORT sgAtlasAreaDrawDegs3f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float angle);
void SG_EXPORT sgAtlasAreaDrawRads2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float angle);
void SG_EXPORT sgAtlasAreaDrawDegs2f1f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float angle);
void SG_EXPORT sgAtlasAreaDraw3f2f2f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset);
void SG_EXPORT sgAtlasAreaDraw2f2f2f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float xscale, float yscale, float xoffset, float yoffset);
void SG_EXPORT sgAtlasAreaDraw3f2f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z, float xscale, float yscale);
void SG_EXPORT sgAtlasAreaDraw2f2f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float xscale, float yscale);
void SG_EXPORT sgAtlasAreaDraw3f(SGAtlas* atlas, SGAtlasArea* area, float x, float y, float z);
void SG_EXPORT sgAtlasAreaDraw2f(SGAtlas* atlas, SGAtlasArea* area, float x, float y);
void SG_EXPORT sgAtlasAreaDraw(SGAtlas* atlas, SGAtlasArea* area);

void SG_EXPORT sgAtlasGetTexCoords4i(SGAtlas* atlas, SGint x, SGint y, SGint w, SGint h, float* s0, float* t0, float* s1, float* t1);
void SG_EXPORT sgAtlasGetTexCoordsA(SGAtlas* atlas, SGAtlasArea* area, float* s0, float* t0, float* s1, float* t1);

size_t SG_EXPORT sgAtlasGetNumTextures(SGAtlas* atlas);
SGTexture* SG_EXPORT sgAtlasGetTexture(SGAtlas* atlas, size_t index);
SGTexture* SG_EXPORT sgAtlasGetTextureA(SGAtlas* atlas, SGAtlasArea* area);

void SG_EXPORT sgAtlasGetSize(SGAtlas* atlas, size_t* width, size_t* height);
size_t SG_EXPORT sgAtlasGetWidth(SGAtlas* atlas);
size_t SG_EXPORT sgAtlasGetHeight(SGAtlas* atlas);

void SG_EXPORT sgAtlasDrawDBG(SGAtlas* atlas, float x, float y, size_t index, SGbool wires);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_GRAPHICS_ATLAS_H__
