/*
 * Copyright (c) 2007 SIEGE Development Team
 * All rights reserved.
 *
 * This file is part of libSIEGE.
 *
 * This software is copyrighted work licensed under the terms of the
 * 2-clause BSD license. Please consult the file "license.txt" for
 * details.
 *
 * If you did not recieve the file with this program, please email
 * Tim Chas <darkuranium@gmail.com>.
 */

#ifndef __SIEGE_MODULES_GRAPHICS_H__
#define __SIEGE_MODULES_GRAPHICS_H__
#include "../common.h"
#include "../util/link.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct SGModuleGraphics
{
	SGuint SG_EXPORT (*sgmGraphicsContextCreate)(void** context, SGuint width, SGuint height, SGuint bpp);
	SGuint SG_EXPORT (*sgmGraphicsContextDestroy)(void* context);
	SGuint SG_EXPORT (*sgmGraphicsContextResize)(void* context, SGuint width, SGuint height);
	SGuint SG_EXPORT (*sgmGraphicsContextClear)(void* context, float* color);

	SGuint SG_EXPORT (*sgmGraphicsViewportCreate)(void** viewport, void* context);
	//SGuint SG_EXPORT (*sgmGraphicsViewportCreateView)(void** viewport, void* context, SGuint wx, SGuint wy, SGuint wwidth, SGuint wheight, float x, float y, float width, float height);
	SGuint SG_EXPORT (*sgmGraphicsViewportDestroy)(void* viewport);
	SGuint SG_EXPORT (*sgmGraphicsViewportSetView)(void* viewport, SGuint wx, SGuint wy, SGuint wwidth, SGuint wheight, float x, float y, float width, float height);
	//SGuint SG_EXPORT (*sgmGraphicsViewportGetView)(void* viewport, SGuint* wx, SGuint* wy, SGuint* wwidth, SGuint* wheight, float* x, float* y, float* width, float* height);

	SGuint SG_EXPORT (*sgmGraphicsSurfaceCreate)(void** surface, void* context);
	//SGuint SG_EXPORT (*sgmGraphicsSurfaceCreateTexture)(void** surface, void* context, void* texture);
	//SGuint SG_EXPORT (*sgmGraphicsSurfaceCreateData)(void** surface, void* context, SGuint width, SGuint height, SGuint bpp, void* data);
	SGuint SG_EXPORT (*sgmGraphicsSurfaceDestroy)(void* surface);
	SGuint SG_EXPORT (*sgmGraphicsSurfaceSetTexture)(void* surface, void* texture);
	SGuint SG_EXPORT (*sgmGraphicsSurfaceGetTexture)(void* surface, void** texture);
	SGuint SG_EXPORT (*sgmGraphicsSurfaceSetData)(void* surface, SGuint width, SGuint height, SGuint bpp, void* data);
	SGuint SG_EXPORT (*sgmGraphicsSurfaceGetData)(void* surface, SGuint* width, SGuint* height, SGuint* bpp, void** data);
	SGuint SG_EXPORT (*sgmGraphicsSurfaceFreeData)(void* surface, void* data);

	SGuint SG_EXPORT (*sgmGraphicsSurfaceGetSize)(void* surface, SGuint* width, SGuint* height);

	SGuint SG_EXPORT (*sgmGraphicsSurfaceDraw)(void* surface, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle);

	SGuint SG_EXPORT (*sgmGraphicsSurfaceSetTarget)(void* surface);
	//SGuint SG_EXPORT (*sgmGraphicsSurfaceGetTarget)(void** surface);

	SGuint SG_EXPORT (*sgmGraphicsSurfaceResetTarget)(void* surface);


	SGuint SG_EXPORT (*sgmGraphicsTextureCreate)(void** texture, void* context);
	//SGuint SG_EXPORT (*sgmGraphicsTextureCreateData)(void** texture, void* context, SGuint width, SGuint height, SGuint bpp, void* data);

	SGuint SG_EXPORT (*sgmGraphicsTextureDestroy)(void* texture);
	SGuint SG_EXPORT (*sgmGraphicsTextureSetData)(void* texture, SGuint width, SGuint height, SGuint bpp, void* data);
	SGuint SG_EXPORT (*sgmGraphicsTextureGetData)(void* texture, SGuint* width, SGuint* height, SGuint* bpp, void** data);
	SGuint SG_EXPORT (*sgmGraphicsTextureFreeData)(void* data);
	SGuint SG_EXPORT (*sgmGraphicsTextureGetSize)(void* texture, SGuint* width, SGuint* height);
	SGuint SG_EXPORT (*sgmGraphicsTextureDraw)(void* texture, float x, float y, float z, float xscale, float yscale, float xoffset, float yoffset, float angle);

	SGuint SG_EXPORT (*sgmGraphicsDrawPrimitive)(void* context, void* texture, SGuint type, SGuint numverts, float* points, float* texcoords, float* colors);
	SGuint SG_EXPORT (*sgmGraphicsDrawSetColor)(void* context, float* color);
	SGuint SG_EXPORT (*sgmGraphicsDrawPointSetSize)(void* context, float size);
	//SGuint SG_EXPORT (*sgmGraphicsDrawPointGetSize)(void* context, float* size);
	SGuint SG_EXPORT (*sgmGraphicsDrawLineSetWidth)(void* context, float size);
	//SGuint SG_EXPORT (*sgmGraphicsDrawLineGetWidth)(void* context, float* size);
	//SGuint SG_EXPORT (*sgmGraphicsDrawLineSetStipple)(void* context, SGuint factor, SGuint pattern);
	//SGuint SG_EXPORT (*sgmGraphicsDrawLineGetStipple)(void* context, SGuint* factor, SGuint* pattern);
	//SGuint SG_EXPORT (*sgmGraphicsDrawPolygonSetStipple)(void* context, SGubyte* pattern);
	//SGuint SG_EXPORT (*sgmGraphicsDrawPolygonGetStipple)(void* context*, SGubyte** pattern);

    SGuint SG_EXPORT (*sgmGraphicsDrawSetBlendFunc)(void* context, SGenum src, SGenum dst);
    SGuint SG_EXPORT (*sgmGraphicsDrawSetBlendEquation)(void* context, SGenum equation);
    
	/**
	 * \name Graphics Load
	 */
	/// @{
	SGuint SG_EXPORT (*sgmGraphicsLoadFile)(const char* fname, SGuint* width, SGuint* height, SGuint* bpp, void** data);
	//SGuint SG_EXPORT (*sgmGraphicsLoadStream)(void* stream, SGuint* width, SGuint* height, SGuint* bpp, void** data);
	SGuint SG_EXPORT (*sgmGraphicsLoadFreeData)(void* data);
	/// @}
} SGModuleGraphics;

#ifdef SG_BUILD_LIBRARY
SGModuleGraphics _sg_modGraphics;
#endif // SG_BUILD_LIBRARY

void SG_EXPORT _sgModuleLoadGraphics(SGLibrary* lib);

SGModuleGraphics* SG_EXPORT sgModuleGetGraphics(void);

SGubyte* SG_EXPORT _sgGraphicsToCanvasSize(SGuint width, SGuint height, SGuint bpp, SGubyte* data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_MODULES_GRAPHICS_H__
