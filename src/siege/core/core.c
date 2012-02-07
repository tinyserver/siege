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

#define SG_BUILD_LIBRARY
#include <siege/util/thread.h>

#include <siege/audio/source.h>
#include <siege/core/console.h>
#include <siege/core/core.h>
#include <siege/core/event.h>
#include <siege/core/entity.h>
#include <siege/core/window.h>
#include <siege/graphics/light.h>
#include <siege/graphics/surface.h>
#include <siege/graphics/draw.h>
#include <siege/graphics/turtle.h>
#include <siege/graphics/viewport.h>
#include <siege/input/keyboard.h>
#include <siege/input/mouse.h>
#include <siege/input/joystick.h>
#include <siege/modules/modules.h>
#include <siege/physics/space.h>
#include <siege/physics/collision.h>

#include <stdlib.h>
#include <stdio.h>

static SGbool _sg_firstLoop;
static SGbool _sg_exitNow;
static SGint _sg_exitVal;
static SGbool _sg_hasInited = SG_FALSE;
static SGulong _sg_curTick = 0;

static SGThread* _sg_renderThread;
static SGint SG_EXPORT _sgRenderThread(void* data)
{
    while(!_sg_exitNow)
    {
        sgDrawClear();
        sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_DRAW);
        sgWindowSwapBuffers();
    }
    return 0;
}

SGuint SG_EXPORT sgLoadModulesv(size_t n, va_list args)
{
	SGuint loaded = 0;
	size_t i;
	for(i = 0; i < n; i++)
		loaded += sgLoadModule(va_arg(args, char*));
	return loaded;
}
SGuint SG_EXPORT sgLoadModules(size_t n, ...)
{
	va_list args;
	va_start(args, n);
	SGuint ret = sgLoadModulesv(n, args);
	va_end(args);
	return ret;
}
SGbool SG_EXPORT sgLoadModule(const char* name)
{
	SGModule* module = sgModuleLoad(name);
	if(module == NULL)
		return SG_FALSE;
	return SG_TRUE;
}

SGbool SG_EXPORT sgInit(SGenum flags)
{
    _sg_firstLoop = SG_TRUE;
    _sg_exitNow = SG_FALSE;
    _sg_exitVal = 0;

    if(flags & SG_INIT_RENDERTHREAD)
        _sg_renderThread = sgThreadCreate(0, _sgRenderThread, NULL);
    else
        _sg_renderThread = NULL;

	_sgEventInit();

    SGList* modList = sgModuleGetList();

	size_t i;
	size_t nmodules = modList ? sgListLength(modList) : 0;
	SGListNode* node;
	SGModuleInfo** infos = malloc(nmodules * sizeof(SGModuleInfo*));
	SGModule* module;
	for(i = 0, node = modList ? modList->first : NULL; node != NULL; node = node->next, i++)
	{
		module = node->item;
		infos[i] = module->minfo;
	}
	SGbool ok = SG_TRUE;
	SGbool mok;
	for(node = modList ? modList->first : NULL; node != NULL; node = node->next)
	{
		module = node->item;
		mok = SG_TRUE;
		if(module->sgmModuleMatch != NULL)
			module->sgmModuleMatch(infos, nmodules, &mok);
		if(!mok)
		{
			fprintf(stderr, "Could not load module %s: Incompatible with other modules\n", module->minfo->name);
			ok = SG_FALSE;
		}
	}
	free(infos);
	if(!ok)
		return SG_FALSE;

	sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_INIT);
	_sg_hasInited = SG_TRUE;

	_sgWindowInit();
	_sgViewportInit();
	_sgTurtleInit();
	_sgSurfaceInit();

	_sgAudioSourceInit();

	_sgKeyboardInit();
	_sgMouseInit();
	_sgJoystickInit();

	_sgConsoleInit();

	_sgDrawInit();

	_sgPhysicsSpaceInit();
    _sgPhysicsCollisionInit();
	
	_sgEntityInit();

	//sgWindowOpen(width, height, bpp, flags);

    _sgLightInit();

	return SG_TRUE;
}

SGbool SG_EXPORT sgDeinit(void)
{
    sgStop(0);
    sgThreadDestroy(_sg_renderThread);

	_sg_hasInited = SG_FALSE;
	sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_DEINIT);

    _sgEntityDeinit();
    
    _sgLightDeinit();

    _sgPhysicsCollisionDeinit();
	_sgPhysicsSpaceDeinit();

	_sgDrawDeinit();

	_sgConsoleDeinit();

	_sgJoystickDeinit();
	_sgMouseDeinit();
	_sgKeyboardDeinit();

	_sgAudioSourceDeinit();

	_sgSurfaceDeinit();
	_sgTurtleDeinit();
	_sgViewportDeinit();
	_sgWindowDeinit();

	_sgEventDeinit();

    SGList* modList;
    while((modList = sgModuleGetList()))
        sgModuleUnload(modList->first->item);

	return SG_TRUE;
}

SGint SG_EXPORT sgRun(void)
{
	_sg_firstLoop = SG_TRUE;
	if(_sg_exitNow)
		return _sg_exitVal;
	while(sgLoop(&_sg_exitVal))
	{
        if(!_sg_renderThread)
        {
            sgWindowSwapBuffers();
            sgDrawClear();
        }
	}
	return _sg_exitVal;
}

SGbool SG_EXPORT sgLoop(SGint* code)
{
	_sg_curTick++;

	if(_sg_firstLoop)
	{
		sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_START);
		if(_sg_renderThread)
            sgThreadStart(_sg_renderThread);
		_sg_firstLoop = SG_FALSE;
	}

	sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_TICKB);

	sgPhysicsSpaceStep(_sg_physSpaceMain, 0.125);
	
    SGList* modList = sgModuleGetList();
	SGListNode* node;
	SGModule* module;
	for(node = modList ? modList->first : NULL; node != NULL; node = node->next)
	{
		module = node->item;
		if(module->sgmModuleTick)
			module->sgmModuleTick(_sg_curTick);
	}
	
    sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_TICK);
    sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_TICKE);

    if(!_sg_renderThread)
        sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_DRAW);

	if(code != NULL)
		*code = _sg_exitVal;
	if(_sg_exitNow)
	{
		sgEventCall(SG_EV_INTERNAL, (SGuint)1, (SGenum)SG_EVF_EXIT);
		return SG_FALSE;
	}
	return SG_TRUE;
}

void SG_EXPORT sgStop(SGint ret)
{
    _sg_exitNow = SG_TRUE;
    _sg_exitVal = ret;
    if(_sg_renderThread)
        sgThreadJoin(_sg_renderThread);
}

SGulong SG_EXPORT sgGetTick(void)
{
	return _sg_curTick;
}
