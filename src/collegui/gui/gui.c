#include <stdlib.h>
#include "ColleGUI.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

#ifdef _WIN32

typedef struct {
    void (*callback)(void *);
    void *user_data;
} CallbackData;

CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title) {
    wchar_t szClassName[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, szClassName, 255);
    szClassName[255] = L'\0';

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = DefWindowProcW;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = szClassName;
    wc.hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_ARROW));
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        return NULL;
    }

    HWND hwnd = CreateWindowW(
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

CGUIState *CShowGUI(CGUIState *state) {
    if (!state || !state->hwnd) return NULL;
    ShowWindow(state->hwnd, SW_SHOW);
    return state;
}

CGUIState *CUpdateGUI(CGUIState *state) {
    if (!state || !state->hwnd) return NULL;
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UpdateWindow(state->hwnd);
    return state;
}

static LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND) {
        CallbackData *cb = (CallbackData *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (cb && cb->callback) cb->callback(cb->user_data);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CShowButton(CGUIState *state, int x, int y, int width, int height, const char *label, void (*callback)(void *), void *user_data) {
    wchar_t wlabel[256];
    MultiByteToWideChar(CP_UTF8, 0, label, -1, wlabel, 255);
    wlabel[255] = L'\0';
    HWND btn = CreateWindowW(L"BUTTON", wlabel, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height, state->hwnd, NULL, GetModuleHandleW(NULL), NULL);
    if (!btn) return false;
    if (callback) {
        CallbackData *cb = malloc(sizeof(CallbackData));
        cb->callback = callback;
        cb->user_data = user_data;
        SetWindowLongPtr(btn, GWLP_USERDATA, (LONG_PTR)cb);
        SetWindowLongPtr(btn, GWLP_WNDPROC, (LONG_PTR)ButtonWndProc);
    }
    return true;
}

bool CShowTextBox(CGUIState *state, int x, int y, int width, int height, const char *initial_text, void (*callback)(const char *, void *), void *user_data) {
    wchar_t wtext[256];
    MultiByteToWideChar(CP_UTF8, 0, initial_text, -1, wtext, 255);
    wtext[255] = L'\0';
    HWND edit = CreateWindowW(L"EDIT", wtext, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        x, y, width, height, state->hwnd, NULL, GetModuleHandleW(NULL), NULL);
    if (!edit) return false;
    /* TODO: CallBack */
    return true;
}

bool CShowLabel(CGUIState *state, int x, int y, int width, int height, const char *text) {
    wchar_t wtext[256];
    MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, 255);
    wtext[255] = L'\0';
    HWND label = CreateWindowW(L"STATIC", wtext, WS_CHILD | WS_VISIBLE,
        x, y, width, height, state->hwnd, NULL, GetModuleHandleW(NULL), NULL);
    return label != NULL;
}

bool CShowCheckBox(CGUIState *state, int x, int y, const char *label, bool checked, void (*callback)(bool, void *), void *user_data) {
    wchar_t wlabel[256];
    MultiByteToWideChar(CP_UTF8, 0, label, -1, wlabel, 255);
    wlabel[255] = L'\0';
    HWND chk = CreateWindowW(L"BUTTON", wlabel, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        x, y, 100, 24, state->hwnd, NULL, GetModuleHandleW(NULL), NULL);
    if (!chk) return false;
    SendMessageW(chk, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
    /* Todo: CallBack */
    return true;
}

bool CShowListBox(CGUIState *state, int x, int y, int width, int height, const char **items, int item_count, void (*callback)(int, void *), void *user_data) {
    HWND list = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY,
        x, y, width, height, state->hwnd, NULL, GetModuleHandleW(NULL), NULL);
    if (!list) return false;
    for (int i = 0; i < item_count; ++i) {
        wchar_t witem[256];
        MultiByteToWideChar(CP_UTF8, 0, items[i], -1, witem, 255);
        witem[255] = L'\0';
        SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)witem);
    }
    /* Todo: Callback */
    return true;
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


