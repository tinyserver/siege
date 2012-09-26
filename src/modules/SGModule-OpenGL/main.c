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

#include <stdlib.h>

#ifndef GL_FRAMEBUFFER_EXT
#define GL_FRAMEBUFFER_EXT          0
#define GL_COLOR_ATTACHMENT0_EXT    0
#define GL_FRAMEBUFFER_COMPLETE_EXT 0
#endif

PROC_HANDLE;

void checkFBO(FBOFunctions* fbo)
{
#define CHECK(FBO, NAME)                    \
    FBO->NAME = GET_PROC_ADDRESS(#NAME);    \
    if(FBO->NAME == NULL)                   \
        FBO->hasFBO = 0;

    //GLboolean (*glIsRenderbufferEXT) (GLuint);
    CHECK(fbo, glBindRenderbufferEXT);
    CHECK(fbo, glDeleteRenderbuffersEXT);
    CHECK(fbo, glGenRenderbuffersEXT);
    CHECK(fbo, glRenderbufferStorageEXT);
    //void (*glGetRenderbufferParameterivEXT) (GLenum, GLenum, GLint *);
    //GLboolean (*glIsFramebufferEXT) (GLuint);
    CHECK(fbo, glIsFramebufferEXT);
    CHECK(fbo, glBindFramebufferEXT);
    CHECK(fbo, glDeleteFramebuffersEXT);
    CHECK(fbo, glGenFramebuffersEXT);
    CHECK(fbo, glCheckFramebufferStatusEXT);
    //void (*glFramebufferTexture1DEXT) (GLenum, GLenum, GLenum, GLuint, GLint);
    CHECK(fbo, glFramebufferTexture2DEXT);
    //void (*glFramebufferTexture3DEXT) (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
    CHECK(fbo, glFramebufferRenderbufferEXT);
    //void (*glGetFramebufferAttachmentParameterivEXT) (GLenum, GLenum, GLenum, GLint *);
    //void (*glGenerateMipmapEXT) (GLenum);
#undef CHECK
}

SGuint higherPower(SGuint num)
{
    SGuint ret = 2;
    while(ret < num)
        ret <<= 1;
    return ret;
}

SGenum SG_EXPORT sgmModuleInit(SGModuleInfo** minfo)
{
    *minfo = (SGModuleInfo*)calloc(1, sizeof(SGModuleInfo));
    (*minfo)->vmajor = SG_VERSION_MAJOR;
    (*minfo)->vminor = SG_VERSION_MINOR;
    (*minfo)->vpatch = SG_VERSION_PATCH;

    // unknown version
    (*minfo)->mmajor = 0;
    (*minfo)->mminor = 0;
    (*minfo)->mpatch = 0;
    (*minfo)->type = SG_MODULE_GRAPHICS;
    (*minfo)->name = "OpenGL";

    INIT_HANDLE();

    return SG_OK;
}

SGenum SG_EXPORT sgmModuleExit(SGModuleInfo* minfo)
{
    if(minfo == NULL)
        return SG_OK; // SG_INVALID_VALUE

    DEINIT_HANDLE();

    free(minfo);
    return SG_OK;
}

SGenum SG_EXPORT sgmModuleMatch(SGModuleInfo** minfos, SGuint numinfos, SGbool* ok)
{
    *ok = SG_TRUE; // we'll let the core module decide for us...
    return SG_OK;
}

