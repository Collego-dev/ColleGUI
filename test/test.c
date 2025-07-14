#include "ColleGUI/gui.h"
#include <ColleGUI.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool quited = false;

#ifdef _WIN32

void on_button(void *user_data) {
    MessageBoxA(NULL, "Button pressed!", "Callback", MB_OK);
}

void on_checkbox(bool checked, void *user_data) {
    MessageBoxA(NULL, checked ? "Checked" : "Unchecked", "Checkbox", MB_OK);
}

void on_textbox(const char *text, void *user_data) {
    MessageBoxA(NULL, text, "TextBox Changed", MB_OK);
}

void on_listbox(int index, void *user_data) {
    char buf[64];
    sprintf(buf, "Selected index: %d", index);
    MessageBoxA(NULL, buf, "ListBox", MB_OK);
}

int main() {
    CGUIState *g = CCreateGUI(0, 0, 400, 300, "ColleGUI Test");
    CShowGUI(g);

    CShowLabel(g, 10, 10, 100, 24, "Label");
    CShowButton(g, 10, 40, 100, 30, "Button", on_button, NULL);
    CShowTextBox(g, 10, 80, 150, 24, "Initial", on_textbox, NULL);
    CShowCheckBox(g, 10, 120, "Check me", false, on_checkbox, NULL);

    const char *items[] = { "Item 1", "Item 2", "Item 3" };
    CShowListBox(g, 10, 160, 120, 60, items, 3, on_listbox, NULL);

    while (CUpdateGUI(g)) {
        
    }
    free(g);
}

#else /* X11 */

void on_button(void *user_data) {
    printf("Button clicked!\n");
}

int main() {
    CGUIState *g = CCreateGUI(0, 0, 300, 200, "ColleGUI Test");

    CShowGUI(g);

    CShowButton(g, 10, 40, 100, 30, "Button", on_button, NULL);

    while (CUpdateGUI(g)) {

    }
    CFreeGUI(g);
}

#endif

