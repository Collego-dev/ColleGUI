#include "ColleGUI/gui.h"
#include <ColleGUI.h>
#include <stdbool.h>
#include <stdlib.h>

bool quited = false;

#ifdef _WIN32

int main() {
    CGUIState *g = CCreateGUI(0, 0, 300, 200, "ColleGUI Test");
    CShowGUI(g);
    while (CUpdateGUI(g)) {
        
    }
    free(g);
}

#else /* X11 */

int main() {
    CGUIState *g = CCreateGUI(0, 0, 300, 200, "ColleGUI Test");

    CShowGUI(g);
    while (!quited) {

    }
    free(g);
}

#endif

