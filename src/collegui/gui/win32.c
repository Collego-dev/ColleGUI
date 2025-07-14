#ifdef _WIN32
#include <stdlib.h>
#include "ColleGUI.h"


#include <tchar.h>

CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const TCHAR *title) {
    static TCHAR szClassName[256];
    size_t len = _tcslen(title);
    if (len >= 256) len = 255;
    _tcsncpy(szClassName, title, len);
    szClassName[len] = _T('\0');
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

#endif
