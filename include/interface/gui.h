#ifndef _INTERFACE_GUI_H
#define _INTERFACE_GUI_H

#ifdef _WIN32

#else
#include <X11/Xlib.h>


struct CGUIState{
    unsigned int y;
    unsigned int x;
    unsigned int width;
    unsigned int height;
}typedef CGUIState;

#endif

/**
 *
 * Crate a Simple GUI.
 * WARN: Please free after use*/
CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height);


#endif /* _INTERFACE_GUI_H */
