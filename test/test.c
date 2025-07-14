#include <interface/gui.h>
#include <stdbool.h>

bool quited = false;

#ifdef _WIN32

int main() {
    CGUIState *g = CCreateGUI(0, 0, 300, 200, "ColleGUI Test");
    CShowGUI(g);
    CUpdateGUI(g);
}

#else /* X11 */

int main() {
    CGUIState *g = CCreateGUI(0, 0, 300, 200, "ColleGUI Test");

    while (!quited) {

    }
}

#endif

