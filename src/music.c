#include "music.h"
#include "../build/skytheme.h" //not sure why it has trouble with the music folder.
#include "soundbank_bin.h"
#include <tonc_memmap.h>
#include <tonc_memdef.h>

#define samplerate 18157
#define buffersize 304
#define looppoint 374842
//timer0count=65536-round(2^24/16000)=0xFBE8

typedef struct buffer{
    s8 buffer0[304];
    s8 buffer1[304];
} buffer __attribute__((packed));

typedef struct songdata {
    s8 *data;//this is the adress of skytheme
    u32 Position;
    
} songdata;

typedef struct buffer_attributes{
    u8 currentbuffer;
    s8 *bufferadress
    
} buffer_attributes;

songdata SongData;
buffer_attributes BufferAttributes;
buffer Buffer;

void initmusicregisters(){
 
    
//REG_SNDDSCNT= 0x0f0f; //enables pwm, turning on left adnright speaker for dirrect a, and ressting fifo
    REG_SOUNDCNT_H = 0x0b0f;//i think this might be same as last line
    REG_SOUNDCNT_X =0x80;//enables sound
    
    REG_TM0D=0xffff-(16777216/(samplerate));
    REG_TM0CNT =TM_ENABLE;
    
    
    REG_DMA1SAD = (u32) skytheme;//0c044cc0 has a lot of data there if we need to test if its in the right spot
    REG_DMA1DAD = (u32) &(REG_FIFOA);
    REG_DMA1CNT_H = 0xB640;

}

void switchoutdma(){
    if (BufferAttributes.currentbuffer==1){
        REG_DMA1CNT_H = 0;
        REG_DMA1SAD = (u32)&Buffer.buffer0[0];
        REG_DMA1CNT_H = 0xB640;
        BufferAttributes.currentbuffer=0;
    }
    else {
        BufferAttributes.currentbuffer=1;
    }
}

void fillbuffer(){
    if (BufferAttributes.currentbuffer==1){
        for (u32 i=0; i<buffersize; ++i){
            Buffer.buffer0[i]=skytheme[++SongData.Position];
            if (SongData.Position>=sizeof(skytheme)){
                SongData.Position=looppoint;
            }
        }
    }
    else {
        for (u32 i=0; i<buffersize; ++i){
            Buffer.buffer1[i]=skytheme[++SongData.Position];
            if (SongData.Position>=sizeof(skytheme)){
                SongData.Position=looppoint;
            }
        }
    }
}

void initilizebuffervariables(){
    SongData.Position=0;
    BufferAttributes.currentbuffer=1;
}

