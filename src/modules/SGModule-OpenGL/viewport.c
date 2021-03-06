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

#include "common.h"

SGenum SG_EXPORT sgmGraphicsViewportCreate(void** viewport, void* context)
{
    // OpenGL doesn't need this
    return SG_OK;
}
//SGenum SG_EXPORT sgmGraphicsViewportCreateView(void** viewport, void* context, SGuint wx, SGuint wy, SGuint wwidth, SGuint wheight, float x, float y, float width, float height)
SGenum SG_EXPORT sgmGraphicsViewportDestroy(void* viewport)
{
    // neither does it need this
    return SG_OK;
}
SGenum SG_EXPORT sgmGraphicsViewportSetView(void* viewport, SGuint wx, SGuint wy, SGuint wwidth, SGuint wheight, float x, float y, float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(wx, wy, wwidth, wheight);
    glOrtho(x, x + width, y + height, y, 127, -128);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return SG_OK;
}
//SGenum SG_EXPORT sgmGraphicsViewportGetView(void* viewport, SGuint* wx, SGuint* wy, SGuint* wwidth, SGuint* wheight, float* x, float* y, float* width, float* height)
