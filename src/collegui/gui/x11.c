#ifndef _WIN32
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "ColleGUI.h"


CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title)
{
    Display* display = XOpenDisplay(NULL);

    if (NULL == display) {
        fprintf(stderr, "ColleGUI(X11): Failed to open display\n");
        return NULL;
    }

    Window root = DefaultRootWindow(display);
    if (None == root) {
        fprintf(stderr, "ColleGUI(X11): No root window found\n");
        return NULL;
    }

    Window window = XCreateSimpleWindow(display, root, x, y, width, height, 0, 0, 0xffffffff);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, & wm_delete_window, 1);

    XStoreName(display, window, title);

    XEvent event;

    CGUIState *state = malloc(sizeof(struct CGUIState));
    state->display = display;
    state->event = event;
    state->window = window;
    state->x = x;
    state->y = y;
    state->width = width;
    state->height = height;

    return state;
}

CGUIState *CShowGUI(CGUIState *state) {
    XMapWindow(state->display, state->window);
    XFlush(state->display);
    return state;
}

CGUIState *CUpdateGUI(CGUIState *state) {
    XExposeEvent expose;
    expose.type = Expose;
    expose.display = state->display;
    expose.window = state->window;
    expose.x = state->x;
    expose.y = state->y;
    expose.width = state->width;
    expose.height = state->height;
    expose.count = 0;

    XSendEvent(state->display, state->window, False, ExposureMask, (XEvent *)&expose);
    XFlush(state->display);
    return state;
}

#endif
