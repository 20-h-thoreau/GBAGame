#include "vblank.h"

void vsync() {
    while (Reg_VCount >= 160);
    while (Reg_VCount < 160);
    
    fillbuffer();
    switchoutdma();
    OAM_CLEAR();
    scrollbackground();
}
