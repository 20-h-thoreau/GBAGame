#include "Graphics.h"
#include "posprintf.h"
#include "graphics.h"
#include "music.h"
#include "score.h"
#include <tonc_oam.h>
#include <tonc_core.h>

extern u16 score;

typedef struct Leaderboard{
    volatile char name[5][3];
    vu16 score[5];
} Leaderboard;

struct Leaderboard leaderboard;

void displayscore(){ //displays the score at the top right side of screen;
    char message[5];
    posprintf(message, "%5d", score);
    displaytext(message, 5, 16, 192,4,1);
    
}
void displaynumber(u16 number, u8 x, u8 y, u8 sprite){
    char message[5];
    posprintf(message, "%5d", number);
    displaytext(message, 5, sprite, x, y, 1);
    
}



void initleaderboard(){
   
    for(u8 x=0,y=0; y<5;++x){
        leaderboard.name[y][x]=0;
        if (x==2){
            x=0xff;
            leaderboard.score[y]=0;
            ++y;
        }
        
    }

}
void leaderboardpreset(){
    leaderboard.name[0][0]='Z';
    leaderboard.name[0][1]='A';
    leaderboard.name[0][2]='K';
    leaderboard.score[0]=3;
    
    leaderboard.name[1][0]='K';
    leaderboard.name[1][1]='T';
    leaderboard.name[1][2]='Y';
    leaderboard.score[1]=2;
    
}
#define scorestartingx 80
#define scorestartingy 56
#define scorestartingsprite 22

void displayscoreboard(){
    char name[3];
    for(u8 y=0; y<5; ++y){
        if (leaderboard.name[y][0]!=0){
            name[0]=leaderboard.name[y][0];
            name[1]=leaderboard.name[y][1];
            name[2]=leaderboard.name[y][2];
            displaytext(name, 3, (scorestartingsprite+(y*8)), scorestartingx ,scorestartingy+(y*12),1 );
            displaynumber(leaderboard.score[y], scorestartingx+32, scorestartingy+y*12, scorestartingsprite+3+y*8);
        }
    }
    
}
//(u16 number, u8 x, u8 y, u8 sprite)

//displaytext(message, length, sprite, x,y,pallette);
u8 boardpos=0xff;

void addtoleaderboard(char l1, char l2,char l3){ //might want to check that boardpos has been initiliazed and itsnt ff
       
        leaderboard.name[boardpos][0]=l1;
        leaderboard.name[boardpos][1]=l2;
        leaderboard.name[boardpos][2]=l3;
        leaderboard.score[boardpos]=score;
}

void addunderline(u8 leterpointer){
    Obj_Attributes[(scorestartingsprite-1)*4]=0b0000000000000000+ (scorestartingy+(12*boardpos)+2);
    Obj_Attributes[(scorestartingsprite-1)*4+1]=0b0000000000000000+(scorestartingx+(8*leterpointer));
    Obj_Attributes[(scorestartingsprite-1)*4+2]=0b0000000000000000+(62+(1<<0xc));
   
}
//21 is sprite
//(char string[], u8 length, u16 StartingSprite, u8 StartingX, u8 StartingY, u8 Pallete){

void openupboardposition(){
    boardpos=leaderboardpositionfinder();
    
    for (u8 i=3; i>=boardpos; --i){
        leaderboard.name[i+1][0]=leaderboard.name[i][0];
        leaderboard.name[i+1][1]=leaderboard.name[i][1];
        leaderboard.name[i+1][2]=leaderboard.name[i][2];
        leaderboard.score[i+1]=leaderboard.score[i];
    }
}


u8 leaderboardpositionfinder(){
    for(u8 i=3; i>=0; --i){
        if (leaderboard.score[i]>score){
            return (i+1);
        }
    }
    return 0;
}

bool checkforhighscore(){
    if(score>=leaderboard.score[4]){
        return true;
    }
    return false;
}

void savescore(){
    char save[4]={'S','A','V','E'};
    sram_memcpy((vu8 *)0x0e000000, &save, 4);//this tells us this is save data and not junk.
    sram_memcpy((vu8 *)0x0e000004, &leaderboard, 25);
}


void loadscore(){
    sram_memcpy(&leaderboard, (vu8 *)0x0e000004, 25);
}

bool verifysave(){
    char test[4]=("SAVE");
    return sram_memcmp ((vu8 *)0x0e000000,&test,4);
    /*sram_memcpy(&test, (vu8 *)0x0e000000,4 );
    switch (test[0]){
        case 'S':
            break;
        default:
            return false;
    }
    switch (test[1]){
        case 'A':
            break;
        default:
            return false;
    }
    switch (test[2]){
        case 'V':
            break;
        default:
            return false;
    }
    switch (test[3]){
        case 'E':
            return true;
        default:
            return false;
    }*/
   
}


//thank you to pinobatch for these two functions
void sram_memcpy(volatile unsigned char *dst, const volatile unsigned char *src, size_t size) {
  for (;size > 0;--size) *dst++ = *src++;
}

//IWRAM_CODE, I have modified this to just slightly tell if equall, and not care about diference
bool sram_memcmp(const volatile unsigned char *dst, const volatile unsigned char *src, size_t size) {
  for (;size > 0;--size) {

    unsigned int a = *dst++;
    unsigned int b = *src++;
    if (a != b) return false;
  }
  return true;
}
