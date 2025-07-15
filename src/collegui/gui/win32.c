#ifdef _WIN32
#include <stdlib.h>
#include "ColleGUI.h"
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "design.h"

#ifdef DEBUG
#define LOG(fmt, ...) do { \
    char _log_buf[256]; \
    snprintf(_log_buf, sizeof(_log_buf), fmt, __VA_ARGS__); \
    OutputDebugStringA(_log_buf); \
} while(0)
#else
#define LOG(fmt, ...) do {} while(0)
#endif

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND: {
            HWND ctrl = (HWND)lParam;
            if (ctrl) {
                CallbackData *cb = (CallbackData *)GetWindowLongPtr(ctrl, GWLP_USERDATA);
                if (cb && cb->callback) {
                    wchar_t className[32];
                    GetClassNameW(ctrl, className, 31);
                    if (_wcsicmp(className, L"BUTTON") == 0) {
                        LONG_PTR style = GetWindowLongPtr(ctrl, GWL_STYLE);
                        if (style & BS_AUTOCHECKBOX) {
                            BOOL checked = SendMessageW(ctrl, BM_GETCHECK, 0, 0) == BST_CHECKED;
                            ((void (*)(bool, void *))cb->callback)((checked ? true : false), cb->user_data);
                        } else {
                            ((void (*)(void *))cb->callback)(cb->user_data);
                        }
                    } else if (_wcsicmp(className, L"EDIT") == 0) {
                        wchar_t wbuf[256];
                        GetWindowTextW(ctrl, wbuf, 255);
                        char buf[256];
                        WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, 255, NULL, NULL);
                        ((void (*)(const char *, void *))cb->callback)(buf, cb->user_data);
                    } else if (_wcsicmp(className, L"LISTBOX") == 0) {
                        int sel = (int)SendMessageW(ctrl, LB_GETCURSEL, 0, 0);
                        ((void (*)(int, void *))cb->callback)(sel, cb->user_data);
                    }
                }
            }
            break;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

CGUIState *CCreateGUI(int x, int y, unsigned int width, unsigned int height, const char *title) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = title;
    wc.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(IDC_ARROW));
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!GetClassInfoA(wc.hInstance, title, &wc)) {
        if (!RegisterClassA(&wc)) {
            LOG("RegisterClassA failed: %lu\n", GetLastError());
            return NULL;
        }
    }

    HWND hwnd = CreateWindowA(
	title,
	title,
        WS_OVERLAPPEDWINDOW,
        x, y,
        width, height,
        NULL, NULL,
        wc.hInstance,
        NULL
    );

    if (!hwnd) {
        LOG("CreateWindowW failed: %lu\n", GetLastError());
        return NULL;
    }

    CGUIState *state = malloc(sizeof(CGUIState));
    if (!state) {
        LOG("malloc for CGUIState failed\n");
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

bool CShowButton(CGUIState *state, int x, int y, int width, int height, const char *label, void (*callback)(void *), void *user_data) {
    wchar_t wlabel[256];
    MultiByteToWideChar(CP_UTF8, 0, label, -1, wlabel, sizeof(wlabel)/sizeof(wlabel[0]));
    HWND btn = CreateWindowW(
        L"BUTTON", wlabel,
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        x, y, width, height,
        state->hwnd, NULL, GetModuleHandleW(NULL), NULL
    );
    if (!btn) return false;
    if (callback) {
        CallbackData *cb = malloc(sizeof(CallbackData));
        cb->callback = (void (*)(void *))callback;
        cb->user_data = user_data;
        SetWindowLongPtr(btn, GWLP_USERDATA, (LONG_PTR)cb);
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
        cb->callback = (void *)callback;
        cb->user_data = user_data;
        SetWindowLongPtr(chk, GWLP_USERDATA, (LONG_PTR)cb);
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
        cb->callback = (void *)callback;
        cb->user_data = user_data;
        SetWindowLongPtr(list, GWLP_USERDATA, (LONG_PTR)cb);
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
        cb->callback = (void *)callback;
        cb->user_data = user_data;
        SetWindowLongPtr(edit, GWLP_USERDATA, (LONG_PTR)cb);
    }
    return true;
}

bool CSetFont(HWND hwnd, const char *font_name, int font_size, bool bold) {
    wchar_t wfont[128];
    MultiByteToWideChar(CP_UTF8, 0, font_name, -1, wfont, sizeof(wfont)/sizeof(wfont[0]));
    HFONT hFont = CreateFontW(
        -MulDiv(font_size, GetDeviceCaps(GetDC(hwnd), LOGPIXELSY), 72), // 高さ
        0, 0, 0,
        bold ? FW_BOLD : FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        wfont
    );
    if (!hFont) return false;
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
    return true;
}

#endif