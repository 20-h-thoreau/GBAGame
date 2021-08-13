#include "bg.h"
#include <tonc_memmap.h>
#include <tonc_core.h>
#include "clouds_bin.h"
#include "cloudmap_bin.h"
#include "backgroundpallete_bin.h"
#include "defs.h"

void initbackground(){
    //REG_BGxCNT
    //REG_BGxHOFS
    //REG_BGxVOFS
    REG_BG0CNT=0b1000000000000000;
    REG_BG1CNT=0b1000000100000101; //islands
    REG_BG2CNT=0b1000001000001010; //clouds
    REG_BG3CNT=0b0000001100001111; //title screen goes here
    
    REG_BG0HOFS=0;
    REG_BG1HOFS=0;
    REG_BG2HOFS=0;
    REG_BG3HOFS=0;
}

void loadbackground(){
    tonccpy(BGPaletteMem,backgroundpallete_bin,backgroundpallete_bin_size);
    
    tonccpy(&Mem_Tile[1][0],clouds_bin,clouds_bin_size);
    
    //tonccpy(&se_mem[30][0],cloudmap_bin,cloudmap_bin_size);
}
u32 scroll=0;
void scrollbackground(){
    ++scroll;
    REG_BG1HOFS=scroll&0xffff;
    REG_BG2HOFS=(scroll>>1)&0xffff;
    
}
