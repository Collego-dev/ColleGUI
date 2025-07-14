#ifndef _INTERFACE_GUI_H
#define _INTERFACE_GUI_H

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <wingdi.h>

struct CGUIState{
    HWND hwnd;
    unsigned int y, x;
    unsigned int width, height;
    char title[256];
}typedef CGUIState;

typedef struct {
    void (*callback)(void *);
    void *user_data;
} CallbackData;

#else
#include <X11/Xlib.h>
#include <stdbool.h>

struct Button {
    int x, y, width, height;
    const char *label;

    /* CallBack */
    void (*callback)(void *);
    void *user_data;
} typedef Button ;

struct CGUIState{
    Display *display;
    XEvent event;
    Window window;
    Atom wm_delete_window;
    GC gc;
    Button *buttons;

    void (*quitcallback)(void *);
    void *user_data;

    unsigned int button_count;
    unsigned int button_capacity;
    unsigned int y, x;
    unsigned int width, height;
    char title[256];
} typedef CGUIState;

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

/**
* Show a button
* @param state CGUIState pointer
* @param x x position
* @param y y position
* @param width button width
* @param height button height
* @param label button label
* @param callback function called when button is pressed
* @param user_data user data passed to callback
* @return true on success, false on failure
*/
bool CShowButton(CGUIState *state, int x, int y, int width, int height, const char *label, void (*callback)(void *), void *user_data);

/**
* Show a text box
* @param state CGUIState pointer
* @param x x position
* @param y y position
* @param width textbox width
* @param height textbox height
* @param initial_text initial text
* @param callback function called when text changes
* @param user_data user data passed to callback
* @return true on success, false on failure
*/
bool CShowTextBox(CGUIState *state, int x, int y, int width, int height, const char *initial_text, void (*callback)(const char *, void *), void *user_data);

/**
* Show a label
* @param state CGUIState pointer
* @param x x position
* @param y y position
* @param width label width
* @param height label height
* @param text label text
* @return true on success, false on failure
*/
bool CShowLabel(CGUIState *state, int x, int y, int width, int height, const char *text);

/**
* Show a checkbox
* @param state CGUIState pointer
* @param x x position
* @param y y position
* @param label checkbox label
* @param checked initial checked state
* @param callback function called when state changes
* @param user_data user data passed to callback
* @return true on success, false on failure
*/
bool CShowCheckBox(CGUIState *state, int x, int y, const char *label, bool checked, void (*callback)(bool, void *), void *user_data);

/**
* Show a list box
* @param state CGUIState pointer
* @param x x position
* @param y y position
* @param width listbox width
* @param height listbox height
* @param items array of strings
* @param item_count number of items
* @param callback function called when selection changes
* @param user_data user data passed to callback
* @return true on success, false on failure
*/
bool CShowListBox(CGUIState *state, int x, int y, int width, int height, const char **items, int item_count, void (*callback)(int, void *), void *user_data);

bool CSetQuitCallback(CGUIState *state, void (*quitcallback)(void *), void *user_data);

void CFreeGUI(CGUIState* state);


#endif /* _INTERFACE_GUI_H */
