#include "bg.h"
#include <tonc_memmap.h>
#include <tonc_core.h>
#include "clouds_bin.h"
#include "cloudmap_bin.h"
#include "backgroundpallete_bin.h"
#include "defs.h"
#define metatilexlen 0x4
#define metatileylen 0x4
#define mapxlen (0x20/metatilexlen)
#define mapylen (0x40/metatileylen)

void subtitlecloud1();
const u16 blank[metatileylen][metatilexlen]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
const u16 cloud0[metatileylen][metatilexlen]={{1,2,3,4},{6,7,7,8},{10,11,12,13},{0,0,0,0}};
const u16 *tiles[2]={&blank,&cloud0};
const u16 bg2map[mapylen][mapxlen]={{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0},{1,0,1,0,1,0,1,0}};
//4x4, meta tiles wide. 10 tall, set these correct after while

void loadtile(u16 *mapptr, u16 metatilex, u16 metatiley, u16 tile);
u32 scroll;
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
    
    u32 scroll=0;

}

#define bg2 0x06001000
void loadbackground(){
    tonccpy(BGPaletteMem,backgroundpallete_bin,backgroundpallete_bin_size);
    
    tonccpy(&Mem_Tile[2][0],clouds_bin,clouds_bin_size);
    
    for (u8 y=0; y<mapylen; ++y){
        for(u8 x=0;x<mapxlen;++x){
            loadtile(bg2,x,y,bg2map[y][x]);
    
        }
    }
    

    //void subtilecloud1(ptr);
    
}

void loadtile(u16 *mapptr, u16 metatilex, u16 metatiley, u16 tile){
    metatilex*=mapxlen/2;
    metatiley*=mapylen*mapxlen; //this should not be larger than 6 wide and 10 tall for the inputs
    mapptr+=metatilex+metatiley;
    u16 (*tileptr)[metatileylen][metatilexlen]=tiles[tile];
    for (u8 y=0; y<metatileylen; ++y){
        for (u8 x=0; x<metatilexlen; ++x){
            *mapptr=(*tileptr)[y][x];
            ++mapptr;
            
        }
        mapptr+=(32-(metatilexlen));
    }
}
//0 3e=x
//0x40==y

void scrollbackground(){
    ++scroll;
    REG_BG0VOFS=(scroll>>1)&0x1ff;
    REG_BG1VOFS=scroll;
    REG_BG2VOFS=(scroll>>2)&0x1ff;
    
}


