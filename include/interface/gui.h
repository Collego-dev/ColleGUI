#ifndef _INTERFACE_GUI_H
#define _INTERFACE_GUI_H

struct CGUIState{
    unsigned int y;
    unsigned int x;
}typedef CGUIState;

CGUIState CCreateGUI(unsigned int x, unsigned int y);


#endif /* _INTERFACE_GUI_H */
