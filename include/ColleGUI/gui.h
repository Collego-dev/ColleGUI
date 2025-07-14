#ifndef _INTERFACE_GUI_H
#define _INTERFACE_GUI_H

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <wingdi.h>

struct CGUIState{
    HWND hwnd;
    unsigned int y;
    unsigned int x;
    unsigned int width;
    unsigned int height;
    char title[256];
}typedef CGUIState;

#else
#include <X11/Xlib.h>

struct CGUIState{
    Display *display;
    XEvent event;
    Window window;
    unsigned int y;
    unsigned int x;
    unsigned int width;
    unsigned int height;
    char title[256];
}typedef CGUIState;

#endif

/**
* create a GUI window
* @param x x position of the window
* @param y y position of the window
* @param width width of the window
* @param height height of the window
* @param title title of the window
* @return CGUIState pointer on success, NULL on failure
*/
#ifdef _WIN32
CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const TCHAR *title);
#else
CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title);
#endif

/**
* Show the GUI window
* @param state CGUIState pointer
* @return CGUIState pointer on success, NULL on failure
*/
CGUIState *CShowGUI(CGUIState *state);

/**
* Update the GUI window
* @param state CGUIState pointer
* @return CGUIState pointer on success, NULL on failure
*/
CGUIState *CUpdateGUI(CGUIState *state);


#endif /* _INTERFACE_GUI_H */
