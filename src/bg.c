#include "bg.h"
#include <tonc_memmap.h>

void initbackground(){
    //REG_BGxCNT
    //REG_BGxHOFS
    //REG_BGxVOFS
    REG_BG0CNT=0b1000000000000000;
    REG_BG1CNT=0b1000000000000001; //islands
    REG_BG2CNT=0b1000000000000010; //clouds
    REG_BG3CNT=0b0000000000000011; //title screen goes here
    
    REG_BG0HOFS=0;
    REG_BG1HOFS=0;
    REG_BG2HOFS=0;
    REG_BG3HOFS=0;
}

u32 scroll=0;
void scrollbackground(){
    ++scroll;
    REG_BG1HOFS=scroll&0xffff;
    REG_BG2HOFS=(scroll>>1)&0xffff;
    
}
