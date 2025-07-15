#ifdef _WIN32
#include <stdlib.h>
#include "ColleGUI.h"
#include <windows.h>
#include <tchar.h>

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title) {
    wchar_t szClassName[256];
    wchar_t szWindowTitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, szClassName, 255);
    szClassName[255] = L'\0';
    MultiByteToWideChar(CP_UTF8, 0, title, -1, szWindowTitle, 255);
    szWindowTitle[255] = L'\0';

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = szClassName;
    wc.hCursor = LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_ARROW));
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        return NULL;
    }

    HWND hwnd = CreateWindowW(
        szClassName,
        szWindowTitle,
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

static LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND && HIWORD(wParam) == EN_CHANGE) {
        CallbackData *cb = (CallbackData *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (cb && cb->callback) {
            wchar_t wbuf[256];
            GetWindowTextW(hwnd, wbuf, 255);
            char buf[256];
            WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, 255, NULL, NULL);
            ((void (*)(const char *, void *))cb->callback)(buf, cb->user_data);
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static LRESULT CALLBACK CheckBoxWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED) {
        CallbackData *cb = (CallbackData *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (cb && cb->callback) {
            BOOL checked = SendMessageW(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
            ((void (*)(bool, void *))cb->callback)(checked, cb->user_data);
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static LRESULT CALLBACK ListBoxWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND && HIWORD(wParam) == LBN_SELCHANGE) {
        CallbackData *cb = (CallbackData *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (cb && cb->callback) {
            int sel = (int)SendMessageW(hwnd, LB_GETCURSEL, 0, 0);
            ((void (*)(int, void *))cb->callback)(sel, cb->user_data);
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CShowButton(CGUIState *state, int x, int y, int width, int height, const char *label, void (*callback)(void *), void *user_data) {
    wchar_t wlabel[256];
    MultiByteToWideChar(CP_UTF8, 0, label, -1, wlabel, 255);
    wlabel[255] = L'\0';
    HWND btn = CreateWindowW(
        L"BUTTON", wlabel,
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        x, y, width, height,
        state->hwnd, NULL, GetModuleHandleW(NULL), NULL
    );
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
    HWND edit = CreateWindowW(
        L"EDIT", wtext,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_TABSTOP | ES_AUTOHSCROLL,
        x, y, width, height,
        state->hwnd, NULL, GetModuleHandleW(NULL), NULL
    );
    if (!edit) return false;
    if (callback) {
        CallbackData *cb = malloc(sizeof(CallbackData));
        // キャストして格納
        cb->callback = (void (*)(void *))callback;
        cb->user_data = user_data;
        SetWindowLongPtr(edit, GWLP_USERDATA, (LONG_PTR)cb);
        SetWindowLongPtr(edit, GWLP_WNDPROC, (LONG_PTR)EditWndProc);
    }
    return true;
}

bool CShowLabel(CGUIState *state, int x, int y, int width, int height, const char *text) {
    wchar_t wtext[256];
    MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, 255);
    wtext[255] = L'\0';
    HWND label = CreateWindowW(
        L"STATIC", wtext,
        WS_CHILD | WS_VISIBLE | SS_CENTER | WS_TABSTOP,
        x, y, width, height,
        state->hwnd, NULL, GetModuleHandleW(NULL), NULL
    );
    return label != NULL;
}

bool CShowCheckBox(CGUIState *state, int x, int y, const char *label, bool checked, void (*callback)(bool, void *), void *user_data) {
    wchar_t wlabel[256];
    MultiByteToWideChar(CP_UTF8, 0, label, -1, wlabel, 255);
    wlabel[255] = L'\0';
    HWND chk = CreateWindowW(
        L"BUTTON", wlabel,
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP,
        x, y, 100, 24,
        state->hwnd, NULL, GetModuleHandleW(NULL), NULL
    );
    if (!chk) return false;
    SendMessageW(chk, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
    if (callback) {
        CallbackData *cb = malloc(sizeof(CallbackData));
        cb->callback = (void (*)(void *))callback;
        cb->user_data = user_data;
        SetWindowLongPtr(chk, GWLP_USERDATA, (LONG_PTR)cb);
        SetWindowLongPtr(chk, GWLP_WNDPROC, (LONG_PTR)CheckBoxWndProc);
    }
    return true;
}

bool CShowListBox(CGUIState *state, int x, int y, int width, int height, const char **items, int item_count, void (*callback)(int, void *), void *user_data) {
    HWND list = CreateWindowW(
        L"LISTBOX", NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | LBS_STANDARD,
        x, y, width, height,
        state->hwnd, NULL, GetModuleHandleW(NULL), NULL
    );
    if (!list) return false;
    for (int i = 0; i < item_count; ++i) {
        wchar_t witem[256];
        MultiByteToWideChar(CP_UTF8, 0, items[i], -1, witem, 255);
        witem[255] = L'\0';
        SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)witem);
    }
    if (callback) {
        CallbackData *cb = malloc(sizeof(CallbackData));
        cb->callback = (void (*)(void *))callback;
        cb->user_data = user_data;
        SetWindowLongPtr(list, GWLP_USERDATA, (LONG_PTR)cb);
        SetWindowLongPtr(list, GWLP_WNDPROC, (LONG_PTR)ListBoxWndProc);
    }
    return true;
}

#endif
