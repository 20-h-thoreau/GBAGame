#include "bg.h"
#include <tonc_memmap.h>
#include <tonc_core.h>
#include "clouds_bin.h"
#include "cloudmap_bin.h"
#include "backgroundpallete_bin.h"
#include "defs.h"

void subtitlecloud1();
const int cloud1[4][5]={{1,2,3,4,5},{6,7,7,8,9},{10,11,12,13,14},{0,0,0,0,0}};

void initbackground(){
    //REG_BGxCNT
    //REG_BGxHOFS
    //REG_BGxVOFS
    REG_BG0CNT=0b1000000000000000; //maybe more clouds?
    REG_BG1CNT=0b1000000100000101; //islands
    REG_BG2CNT=0b1000001000001010; //clouds
    REG_BG3CNT=0b0000001100001111; //title screen goes here
    
    REG_BG0VOFS=0;
    REG_BG1VOFS=0;
    REG_BG2VOFS=0;
    REG_BG3VOFS=0;
    REG_BG0HOFS=0;
    REG_BG1HOFS=0;
    REG_BG2HOFS=0;
    REG_BG3HOFS=0;

}

void loadbackground(){
    tonccpy(BGPaletteMem,backgroundpallete_bin,backgroundpallete_bin_size);
    
    tonccpy(&Mem_Tile[2][0],clouds_bin,clouds_bin_size);
    u16 *ptr=0x06001000;
    
    for (int y=0; y<4; ++y){
        for (int x=0; x<5; ++x){
            *ptr=cloud1[y][x];
            ++ptr;
        }
        ptr+=(32-5);
    }
    //void subtilecloud1(ptr);
    
}
u32 scroll=0;
void scrollbackground(){
    ++scroll;
    REG_BG0VOFS=(scroll>>1)&0x1ff;
    REG_BG1VOFS=scroll;
    REG_BG2VOFS=(scroll<<1)&0x1ff;
    
}

void subtitlecloud1(){
    /*for (int y=0; y<4; ++y){
        for (int x=0; x<5; ++x){
            *ptr=1;
            ptr+=2;
        }
        ptr+=(64-10);
    }*/
    int *mapptr=0x06000802;
    mapptr=2;
    
}
