#include <ColleGUI.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ColleGUI/gui.h"

#ifdef _WIN32
void on_button(void *user_data) {
  printf("Button pressed!\n");
  fflush(stdout);
}

void on_checkbox(bool checked, void *user_data) {
  printf("Checkbox: %s\n", checked ? "Checked" : "Unchecked");
  fflush(stdout);
}

void on_textbox(const char *text, void *user_data) {
  printf("TextBox Changed: %s\n", text);
  fflush(stdout);
}

void on_listbox(int index, void *user_data) {
  printf("ListBox selected index: %d\n", index);
  fflush(stdout);
}

void on_toolbar(int idx, void *user_data) {
  printf("Toolbar button %d pressed\n", idx);
  fflush(stdout);
}
void on_statusbar(const char *text, void *user_data) {
  printf("StatusBar: %s\n", text);
  fflush(stdout);
}
void on_progress(int value) {
  printf("ProgressBar value: %d\n", value);
  fflush(stdout);
}
void on_trackbar(int value, void *user_data) {
  printf("TrackBar value: %d\n", value);
  fflush(stdout);
}
void on_updown(int value, void *user_data) {
  printf("UpDown value: %d\n", value);
  fflush(stdout);
}
void on_listview(int idx, void *user_data) {
  printf("ListView selected: %d\n", idx);
  fflush(stdout);
}
void on_treeview(int idx, void *user_data) {
  printf("TreeView selected: %d\n", idx);
  fflush(stdout);
}
void on_tab(int idx, void *user_data) {
  printf("Tab selected: %d\n", idx);
  fflush(stdout);
}
void on_calendar(const char *date, void *user_data) {
  printf("Calendar date: %s\n", date);
  fflush(stdout);
}
void on_datetime(const char *dt, void *user_data) {
  printf("DateTimePicker: %s\n", dt);
  fflush(stdout);
}

int main() {
  CGUIState *g = CCreateGUI(0, 0, 400, 300, "ColleGUI Test");
  CShowGUI(g);
  UpdateWindow(g->hwnd);

  CShowLabel(g, 10, 10, 100, 24, "Label");
  CShowButton(g, 10, 40, 100, 30, "Button", on_button, NULL);
  CShowTextBox(g, 10, 80, 150, 24, "Initial", on_textbox, NULL);
  CShowCheckBox(g, 10, 120, "Check me", false, on_checkbox, NULL);

  const char *items[] = {"Item 1", "Item 2", "Item 3"};
  CShowListBox(g, 10, 160, 120, 60, items, 3, on_listbox, NULL);

  while (CUpdateGUI(g)) {
  }
  free(g);
}

#else /* X11 */

void on_button(void *user_data) { printf("Button clicked!\n"); }

void on_quit(void *user_data) { printf("See you!\n"); }

int main() {
  CGUIState *g = CCreateGUI(0, 0, 300, 200, "ColleGUI Test");

  CShowGUI(g);

  CShowButton(g, 10, 40, 100, 30, "Button", on_button, NULL);

  CSetQuitCallback(g, on_quit, NULL);

  while (CUpdateGUI(g)) {
  }
  CFreeGUI(g);
}

#endif
