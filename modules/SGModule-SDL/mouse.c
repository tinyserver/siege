#include "main.h"
#include "mouse.h"
#include "window.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

void cbMouseButton(int button, int action)
{
    if(main_window->cbMouse->button != NULL)
        main_window->cbMouse->button((void*)1, button, action);
}

void cbMouseMove(int x, int y)
{
    if(main_window->cbMouse->move != NULL)
        main_window->cbMouse->move((void*)1, x, y);
}

void cbMouseWheel(int w)
{
    if(main_window->cbMouse->wheel != NULL)
        main_window->cbMouse->wheel((void*)1, w);
}

SGuint SG_EXPORT sgCoreMouseCreate(void** mouse, void* window)
{
    *mouse = (void*)1;
    return SG_OK;
}
SGuint SG_EXPORT sgCoreMouseDestroy(void* mouse)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    return SG_OK;
}
// should these two [sgCoreMouseShow,sgCoreMouseHide] be put into a single function?
SGuint SG_EXPORT sgCoreMouseShow(void* mouse)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_ShowCursor(1);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreMouseHide(void* mouse)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_ShowCursor(0);

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreMouseIsShown(void* mouse, SGbool* shown);
//SGuint SG_EXPORT sgCoreMouseIsHidden(void* mouse, SGbool* hidden);
SGuint SG_EXPORT sgCoreMouseGetNumButtons(void* mouse, SGuint* numbuttons)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *numbuttons = 7;

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreMouseButtonSetState(void* mouse, SGbool* state);
SGuint SG_EXPORT sgCoreMouseButtonGetState(void* mouse, SGbool* state)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SGuint numbuttons;
    sgCoreMouseGetNumButtons(mouse, &numbuttons);
    SGuint i;
    for(i = 0; i < numbuttons; i++)
        state[i] = mouseState & SDL_BUTTON(i);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreMouseSetPosition(void* mouse, SGint x, SGint y)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_WarpMouse(x, y);
    mouseX = x;
    mouseY = y;
    //cbMouseMove(x, y);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreMouseGetPosition(void* mouse, SGint* x, SGint* y)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *x = mouseX;
    *y = mouseY;

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreMouseSetWheel(void* mouse, SGint w);
SGuint SG_EXPORT sgCoreMouseGetWheel(void* mouse, SGint* w)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *w = mouseW;

    return SG_OK;
}

SGuint SG_EXPORT sgCoreMouseSetCallbacks(void* mouse, SGCoreMouseCallbacks* callbacks)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    if(callbacks == NULL)
        main_window->cbMouse = NULL; // TODO: check if memcpy handles NULL gracefully (if it does, remove this if/else)
    else
        main_window->cbMouse = memcpy(main_window->cbMouse, callbacks, sizeof(SGCoreMouseCallbacks));

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreMouseGetCallbacks(void* mouse, SGCoreMouseCallbacks** callbacks);
