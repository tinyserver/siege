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

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <siege/backend.h>
#include "texture.h"
#include "surface.h"

typedef struct ContextData
{
    SGuint bpp;
    struct SurfaceData* target;

    struct FBOFunctions fbo;
} ContextData;

#endif // __CONTEXT_H__
