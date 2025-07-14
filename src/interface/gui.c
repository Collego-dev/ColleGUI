#include <stdlib.h>
#ifdef _WIN32


#else /* X11 */
#include "interface/gui.h"
#include <X11/Xlib.h>
#include <stdio.h>


CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height)
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

    XEvent event;

    CGUIState *state = malloc(sizeof(struct CGUIState));

    state->display = display;
    state->event = event;
    state->root = window;

    return state;
}

#endif


