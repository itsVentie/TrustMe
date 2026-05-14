#include "../../include/modules/payload.h"
#include <windows.h>
#include <ctime>

void RunPayload() {
    srand(static_cast<unsigned int>(time(NULL)));

    while (true) {
        if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
            break;
        }

        POINT p;
        if (GetCursorPos(&p)) {
            int x = p.x + (rand() % 3) - 1;
            int y = p.y + (rand() % 3) - 1;
            SetCursorPos(x, y);
        }

        Sleep(15);
    }
}