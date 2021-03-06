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

#ifndef __SIEGE_INPUT_JOYSTICK_H__
#define __SIEGE_INPUT_JOYSTICK_H__

#include "../common.h"
#include "../modules/input.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _SGJoystick
{
    SGuint id;
    void* handle;

    size_t numbuttons;
    SGbool* bprev;
    SGbool* bcurr;

    size_t numaxis;
    float* aprev;
    float* acurr;
    float* adelt;
} _SGJoystick;

#ifdef SG_BUILD_LIBRARY
size_t _sg_joyNum;
_SGJoystick** _sg_joyJoys;
SGCoreJoystickCallbacks _sg_joyCallbacks;
#endif // SG_BUILD_LIBRARY

void SG_EXPORT _sg_cbJoystickButton(void* joystick, SGuint button, SGbool down);
void SG_EXPORT _sg_cbJoystickMove(void* joystick, float* axis);

void SG_EXPORT _sgJoystickUpdate(void);

SGbool SG_EXPORT _sgJoystickInit(void);
SGbool SG_EXPORT _sgJoystickDeinit(void);

_SGJoystick* SG_EXPORT _sgJoystickCreate(SGuint id);
void SG_EXPORT _sgJoystickDestroy(_SGJoystick* joy);

size_t SG_EXPORT sgJoystickGetNumJoysticks(void);
size_t SG_EXPORT sgJoystickGetNumButtons(SGuint joy);
size_t SG_EXPORT sgJoystickGetNumAxis(SGuint joy);

SGbool SG_EXPORT sgJoystickGetButtonPrev(SGuint joy, SGuint button);
SGbool SG_EXPORT sgJoystickGetButton(SGuint joy, SGuint button);
SGbool SG_EXPORT sgJoystickGetButtonPress(SGuint joy, SGuint button);
SGbool SG_EXPORT sgJoystickGetButtonRelease(SGuint joy, SGuint button);

float* SG_EXPORT sgJoystickGetAxisPrev(SGuint joy);
float SG_EXPORT sgJoystickGetAxisIndexPrev(SGuint joy, SGuint axis);
float* SG_EXPORT sgJoystickGetAxis(SGuint joy);
float SG_EXPORT sgJoystickGetAxisIndex(SGuint joy, SGuint axis);
float* SG_EXPORT sgJoystickGetAxisDelta(SGuint joy);
float SG_EXPORT sgJoystickGetAxisIndexDelta(SGuint joy, SGuint axis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_INPUT_JOYSTICK_H__
