#ifndef _WIN32
#include <X11/X.h>
#include "ColleGUI/gui.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ColleGUI.h"


void DefaultQuitCallback(void* user_data) {}

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
    XSelectInput(display, window, ExposureMask | ButtonPressMask);

    XEvent event;

    GC gc = XCreateGC(display, window, 0, NULL);

    CGUIState *state = malloc(sizeof(struct CGUIState));
    state->display = display;
    state->event = event;
    state->window = window;
    state->wm_delete_window = wm_delete_window;
    state->gc = gc;
    state->x = x;
    state->y = y;
    state->width = width;
    state->height = height;
    state->quitcallback = DefaultQuitCallback;
    state->user_data = NULL;

    return state;
}

CGUIState *CShowGUI(CGUIState *state) {
    XMapWindow(state->display, state->window);
    XFlush(state->display);
    return state;
}

CGUIState *CUpdateGUI(CGUIState *state) {
    XNextEvent(state->display, &state->event);

    XEvent *ev = &state->event;

    switch (ev->type) {
        case Expose:
            for (int i = 0; i < state->button_count; i++) {
                Button *btn = &state->buttons[i];
                XDrawRectangle(state->display, state->window, state->gc, btn->x, btn->y, btn->width, btn->height);
                XDrawString(state->display, state->window, state->gc, btn->x + 5, btn->y + 15, btn->label, strlen(btn->label));
            }
            break;

        case ButtonPress:
            switch (ev->xbutton.button) {
                case Button1: 
                    int x = ev->xbutton.x;
                    int y = ev->xbutton.y;

                    for (int i=0; i < state->button_count; i++) {
                        Button *b = &state->buttons[i];
                        if (x >= b->x && x <= b->x + b->width &&
                                y >= b->y && y <= b->y + b->height) {
                            if (b->callback)
                                b->callback(b->user_data);
                        }
                   }
                   break;
                case Button2:
                   break;
                case Button3:
                   break;
                case Button4:
                   break;
                case Button5:
                   break;
                default:
                   fprintf(stderr, "ColleGUI(X11): Unknown button: %d\n", ev->xbutton.button);
                   break;
            }
           break;

        case ClientMessage:
           if (ev->xclient.data.l[0] == state->wm_delete_window) {
               state->quitcallback(state->user_data);
           }
           return NULL;
    }
   return state;
}


bool CShowButton(CGUIState *state, int x, int y, int width, int height, const char *label, void (*callback)(void *), void *user_data){
    if (state->button_count >= state->button_capacity) {
        int new_capa = state->button_capacity == 0 ? 8 : state->button_capacity * 2;
        Button *new_buttons = realloc(state->buttons, new_capa * sizeof(Button));

        if (!new_buttons){
            perror("ColleGUI(X11): realloc: ");
            return false;
        }
        state->buttons = new_buttons;
        state->button_capacity = new_capa;
    }

    state->buttons[state->button_count++] = (Button) { .x=x, .y = y, .width = width, .height = height, .label = label, .callback = callback, .user_data = user_data };

    return false;
}

bool CSetQuitCallback(CGUIState *state, void (*quitcallback)(void *), void *user_data) {
    state->quitcallback = quitcallback;
    state->user_data = user_data;

    return false;
}

void CFreeGUI(CGUIState *state){
    free(state->buttons);
    free(state);
}


#endif
