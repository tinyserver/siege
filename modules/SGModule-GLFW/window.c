#include "main.h"
#include "window.h"
#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

void GLFWCALL windowOpen()
{
    if(main_window->cbWindow->open != NULL)
        main_window->cbWindow->open(main_window);
}
int GLFWCALL windowClose()
{
    if(main_window->cbWindow->close != NULL)
        main_window->cbWindow->close(main_window);

    return 1;
}
void GLFWCALL windowResize(int x, int y)
{
    if(main_window->cbWindow->resize != NULL)
        main_window->cbWindow->resize(main_window, x, y);
}

SGuint SG_EXPORT sgCoreWindowCreate(void** window)
{
    *window = calloc(1, sizeof(Window));
    Window** cwindow = (Window**)window;
    (*cwindow)->cbWindow = calloc(1, sizeof(SGCoreWindowCallbacks));
    (*cwindow)->cbJoystick = calloc(1, sizeof(SGCoreJoystickCallbacks));
    (*cwindow)->cbKeyboard = calloc(1, sizeof(SGCoreKeyboardCallbacks));
    (*cwindow)->cbMouse = calloc(1, sizeof(SGCoreMouseCallbacks));

    main_window = *window;

    return SG_OK;
}

SGuint SG_EXPORT sgCoreWindowDestroy(void* window)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    Window* cwindow = (Window*)window;

    sgCoreWindowClose(window);
    free(cwindow->cbWindow);
    free(cwindow->cbJoystick);
    free(cwindow->cbKeyboard);
    free(cwindow->cbMouse);
    free(cwindow);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowOpen(void* window, SGuint width, SGuint height, SGubyte bpp, SGenum flags)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    int red, green, blue, alpha, depth, stencil;
    switch(bpp)
    {
        case 0:
            red = 0;
            green = 0;
            blue = 0;
            alpha = 0;

            depth = 8;
            stencil = 0;
            break;
        case 8:
            red = 3;
            green = 3;
            blue = 2;
            alpha = 0;

            depth = 8;
            stencil = 0;
            break;
        case 15:
            red = 5;
            green = 5;
            blue = 5;
            alpha = 0;

            depth = 8;
            stencil = 0;
            break;
        case 16:
            red = 5;
            green = 6;
            blue = 5;
            alpha = 0;

            depth = 8;
            stencil = 0;
            break;
        case 24:
            red = 8;
            green = 8;
            blue = 8;
            alpha = 0;

            depth = 16;
            stencil = 0;
            break;
        case 32:
            red = 8;
            green = 8;
            blue = 8;
            alpha = 8;

            depth = 32;
            stencil = 0;
            break;
        default:
            break;
    }
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);

    if(!glfwOpenWindow(width, height, red, green, blue, alpha, depth, stencil, (flags & SG_WINDOW_FULLSCREEN) ? GLFW_FULLSCREEN : GLFW_WINDOW))
        return SG_UNKNOWN_ERROR;

    windowOpen();

    Window tmpWindow;
    tmpWindow.cbWindow   = malloc(sizeof(SGCoreWindowCallbacks));
    tmpWindow.cbJoystick = malloc(sizeof(SGCoreJoystickCallbacks));
    tmpWindow.cbKeyboard = malloc(sizeof(SGCoreKeyboardCallbacks));
    tmpWindow.cbMouse    = malloc(sizeof(SGCoreMouseCallbacks));

    memcpy(tmpWindow.cbWindow  , main_window->cbWindow  , sizeof(SGCoreWindowCallbacks));
    memcpy(tmpWindow.cbJoystick, main_window->cbJoystick, sizeof(SGCoreJoystickCallbacks));
    memcpy(tmpWindow.cbKeyboard, main_window->cbKeyboard, sizeof(SGCoreKeyboardCallbacks));
    memcpy(tmpWindow.cbMouse   , main_window->cbMouse   , sizeof(SGCoreMouseCallbacks));

    memset(main_window->cbWindow  , 0, sizeof(SGCoreWindowCallbacks));
    memset(main_window->cbJoystick, 0, sizeof(SGCoreJoystickCallbacks));
    memset(main_window->cbKeyboard, 0, sizeof(SGCoreKeyboardCallbacks));
    memset(main_window->cbMouse   , 0, sizeof(SGCoreMouseCallbacks));

    glfwSetWindowCloseCallback(windowClose);
    glfwSetWindowSizeCallback(windowResize);
    glfwSetMouseButtonCallback(mouseButton);
    glfwSetMousePosCallback(mouseMove);
    glfwSetMouseWheelCallback(mouseWheel);
    glfwSetKeyCallback(keyboardKey);
    glfwSetCharCallback(keyboardChar);
    // joystick

    memcpy(main_window->cbWindow  , tmpWindow.cbWindow  , sizeof(SGCoreWindowCallbacks));
    memcpy(main_window->cbJoystick, tmpWindow.cbJoystick, sizeof(SGCoreJoystickCallbacks));
    memcpy(main_window->cbKeyboard, tmpWindow.cbKeyboard, sizeof(SGCoreKeyboardCallbacks));
    memcpy(main_window->cbMouse   , tmpWindow.cbMouse   , sizeof(SGCoreMouseCallbacks));

    free(tmpWindow.cbWindow);
    free(tmpWindow.cbJoystick);
    free(tmpWindow.cbKeyboard);
    free(tmpWindow.cbMouse);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowIsOpened(void* window, SGbool* opened)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    *opened = (SGbool)glfwGetWindowParam(GLFW_OPENED);
    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowClose(void* window)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    glfwCloseWindow();
    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowIsClosed(void* window, SGbool* closed);
SGuint SG_EXPORT sgCoreWindowSetTitle(void* window, char* title)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    glfwSetWindowTitle(title);
    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowGetTitle(void* window, char** title);
//SGuint SG_EXPORT sgCoreWindowFreeTitle(char* title);
SGuint SG_EXPORT sgCoreWindowSetSize(void* window, SGuint width, SGuint height)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    glfwSetWindowSize(width, height);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowGetSize(void* window, SGuint* width, SGuint* height)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    glfwGetWindowSize(width, height);

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowPollEvents(void* window);
SGuint SG_EXPORT sgCoreWindowSwapBuffers(void* window)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    glfwSwapBuffers();

    return SG_OK;
}

SGuint SG_EXPORT sgCoreWindowSetCallbacks(void* window, SGCoreWindowCallbacks* callbacks)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    Window* cwindow = (Window*)window;
    if(callbacks == NULL)
        cwindow->cbWindow = NULL; // TODO: check if memcpy handles NULL gracefully (if it does, remove this if/else)
    else
        cwindow->cbWindow = memcpy(cwindow->cbWindow, callbacks, sizeof(SGCoreWindowCallbacks));

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowGetCallbacks(void* window, SGCoreWindowCallbacks** callbacks);
