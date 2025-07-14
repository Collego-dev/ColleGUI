#include <stdlib.h>
#include "interface/gui.h"

#ifdef _WIN32

CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title) {
    static TCHAR szClassName[256];
    _sntprintf(szClassName, sizeof(szClassName)/sizeof(TCHAR), _T("%s"), title);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = szClassName;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!RegisterClass(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        return NULL;
    }

    HWND hwnd = CreateWindow(
        szClassName,
        szClassName,
        WS_OVERLAPPEDWINDOW,
        x, y,
        width, height,
        NULL, NULL,
        wc.hInstance,
        NULL
    );

    if (!hwnd) {
        return NULL;
    }

    CGUIState *state = malloc(sizeof(CGUIState));
    if (!state) {
        DestroyWindow(hwnd);
        return NULL;
    }

    state->hwnd = hwnd;

    return state;
}

#else /* X11 */

#include <X11/Xlib.h>
#include <stdio.h>


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

    XEvent event;

    CGUIState *state = malloc(sizeof(struct CGUIState));

    state->display = display;
    state->event = event;
    state->window = window;

    return state;
}

#endif


