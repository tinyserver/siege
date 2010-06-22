#include "main.h"
#include "mouse.h"
#include "window.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

void mouseButton(int button, int action)
{
    if(button == 2)
        button = 3;
    else if(button == 3)
        button = 2;

    if(main_window->cbMouse->button != NULL)
        main_window->cbMouse->button((void*)1, button, action);
}

void mouseMove(int x, int y)
{
    if(main_window->cbMouse->move != NULL)
        main_window->cbMouse->move((void*)1, x, y);
}

void mouseWheel(int w)
{
    if(main_window->cbMouse->wheel != NULL)
        main_window->cbMouse->wheel((void*)1, w);
}

SGuint SG_EXPORT sgmCoreMouseCreate(void** mouse, void* window)
{
    *mouse = (void*)1;
    return SG_OK;
}
SGuint SG_EXPORT sgmCoreMouseDestroy(void* mouse)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    return SG_OK;
}
// should these two [sgmCoreMouseShow,sgmCoreMouseHide] be put into a single function?
SGuint SG_EXPORT sgmCoreMouseShow(void* mouse)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_ShowCursor(1);

    return SG_OK;
}
SGuint SG_EXPORT sgmCoreMouseHide(void* mouse)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_ShowCursor(0);

    return SG_OK;
}
//SGuint SG_EXPORT sgmCoreMouseIsShown(void* mouse, SGbool* shown);
//SGuint SG_EXPORT sgmCoreMouseIsHidden(void* mouse, SGbool* hidden);
SGuint SG_EXPORT sgmCoreMouseGetNumButtons(void* mouse, SGuint* numbuttons)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *numbuttons = 7;

    return SG_OK;
}
//SGuint SG_EXPORT sgmCoreMouseButtonSetState(void* mouse, SGbool* state);
SGuint SG_EXPORT sgmCoreMouseButtonGetState(void* mouse, SGbool* state)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SGuint numbuttons;
    sgmCoreMouseGetNumButtons(mouse, &numbuttons);
    SGuint i;
    for(i = 0; i < numbuttons; i++)
        state[i] = mouseState & SDL_BUTTON(i);

    return SG_OK;
}
SGuint SG_EXPORT sgmCoreMouseSetPosition(void* mouse, SGint x, SGint y)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_WarpMouse(x, y);
    mouseX = x;
    mouseY = y;
    //cbMouseMove(x, y);

    return SG_OK;
}
SGuint SG_EXPORT sgmCoreMouseGetPosition(void* mouse, SGint* x, SGint* y)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *x = mouseX;
    *y = mouseY;

    return SG_OK;
}
//SGuint SG_EXPORT sgmCoreMouseSetWheel(void* mouse, SGint w);
SGuint SG_EXPORT sgmCoreMouseGetWheel(void* mouse, SGint* w)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *w = mouseW;

    return SG_OK;
}

SGuint SG_EXPORT sgmCoreMouseSetCallbacks(void* mouse, SGCoreMouseCallbacks* callbacks)
{
    if(mouse == NULL)
        return SG_OK; // SG_INVALID_VALUE

    if(callbacks == NULL)
        main_window->cbMouse = NULL; // TODO: check if memcpy handles NULL gracefully (if it does, remove this if/else)
    else
        main_window->cbMouse = memcpy(main_window->cbMouse, callbacks, sizeof(SGCoreMouseCallbacks));

    return SG_OK;
}
//SGuint SG_EXPORT sgmCoreMouseGetCallbacks(void* mouse, SGCoreMouseCallbacks** callbacks);
