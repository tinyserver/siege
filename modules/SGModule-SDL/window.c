#include "main.h"
#include "window.h"
//#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

void cbWindowOpen()
{
    if(main_window->cbWindow->open != NULL)
        main_window->cbWindow->open(main_window);
}
int cbWindowClose()
{
    if(main_window->cbWindow->close != NULL)
        main_window->cbWindow->close(main_window);

    return 1;
}
void cbWindowResize(int x, int y)
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
    Window* cwindow = (Window*)window;

    Uint32 sdlflags = 0;
    if(useGL)
        sdlflags |= SDL_OPENGL;
    else
        sdlflags |= SDL_HWSURFACE;
    if(flags & SG_WINDOW_FULLSCREEN)
        sdlflags |= SDL_FULLSCREEN;
    if(flags & SG_WINDOW_RESIZABLE)
        sdlflags |= SDL_RESIZABLE;

    cwindow->surface = SDL_SetVideoMode(width, height, bpp, sdlflags);

    cbWindowOpen();
    cbWindowResize(width, height);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowIsOpened(void* window, SGbool* opened)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    Window* cwindow = (Window*)window;

    *opened = cwindow->opened;
    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowClose(void* window)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    Window* cwindow = (Window*)window;

    cwindow->opened = SG_FALSE;
    cbWindowClose();

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowIsClosed(void* window, SGbool* closed);
SGuint SG_EXPORT sgCoreWindowSetTitle(void* window, char* title) /// TODO: icon title + icon
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE

    SDL_WM_SetCaption(title, title);

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowGetTitle(void* window, char** title);
//SGuint SG_EXPORT sgCoreWindowFreeTitle(char* title);
SGuint SG_EXPORT sgCoreWindowSetSize(void* window, SGuint width, SGuint height)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    Window* cwindow = (Window*)window;

    cwindow->surface = SDL_SetVideoMode(width, height, cwindow->surface->format->BitsPerPixel, cwindow->surface->flags);
    cbWindowResize(width, height);

    return SG_OK;
}
SGuint SG_EXPORT sgCoreWindowGetSize(void* window, SGuint* width, SGuint* height)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    Window* cwindow = (Window*)window;

    *width = cwindow->surface->w;
    *height = cwindow->surface->h;

    return SG_OK;
}
//SGuint SG_EXPORT sgCoreWindowPollEvents(void* window);
SGuint SG_EXPORT sgCoreWindowSwapBuffers(void* window)
{
    if(window == NULL)
        return SG_OK; // SG_INVALID_VALUE
    Window* cwindow = (Window*)window;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_ACTIVEEVENT:
                SDL_PollEvent(&event); // next one is SDL_KEYUP
                break;
            case SDL_VIDEORESIZE:
                sgCoreWindowSetSize(window, event.resize.w, event.resize.h);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                keyStates = SDL_GetKeyState(&keyNumStates);
                cbKeyboardKey(event.key.keysym.sym, event.key.state == SDL_PRESSED);
                if(event.key.keysym.unicode != 0)
                    cbKeyboardChar(event.key.keysym.unicode, event.key.state == SDL_PRESSED);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                mouseState = SDL_GetMouseState(&mouseX, &mouseY);
                if(event.button.button == SDL_BUTTON_WHEELUP)
                {
                    if(event.button.state == SDL_PRESSED)
                        mouseW++;
                }
                else if(event.button.button == SDL_BUTTON_WHEELDOWN)
                {
                    if(event.button.state == SDL_PRESSED)
                        mouseW--;
                }
                else
                    cbMouseButton(event.button.button, event.button.state == SDL_PRESSED);
                break;

            case SDL_MOUSEMOTION:
                mouseState = SDL_GetMouseState(&mouseX, &mouseY);
                cbMouseMove(event.motion.x, event.motion.y);
                break;

            case SDL_QUIT:
                sgCoreWindowClose(window);
                break;

            default:
                break;
        }
    }

    if(useGL)
        SDL_GL_SwapBuffers();
    else
        SDL_Flip(cwindow->surface);

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
