#ifndef _INTERFACE_GUI_H
#define _INTERFACE_GUI_H

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <wingdi.h>

struct CGUIState{
    unsigned int y;
    unsigned int x;
    unsigned int width;
    unsigned int height;
    char title[256];
}typedef CGUIState;

#else
#include <X11/Xlib.h>

struct CGUIState{
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
CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title);

#endif /* _INTERFACE_GUI_H */
