#include "Graphics.h"
#include "posprintf.h"
#include "graphics.h"
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



void savescore(){
    
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
    leaderboard.name[0][0]='z';
    leaderboard.name[0][1]='a';
    leaderboard.name[0][2]='k';
    leaderboard.score[0]=3;
}
#define scorestartingx 32
#define scorestartingy 64
#define scorestartingsprite 22
void displayscoreboard(){
    char name[3];
    for(u8 y=0; y<5; ++y){
        if (leaderboard.name[y][0]!=0){
            name[0]=leaderboard.name[y][0]-32;
            name[1]=leaderboard.name[y][1]-32;
            name[2]=leaderboard.name[y][2]-32;
            displaytext(name, 3, (scorestartingsprite+(y*8)), scorestartingx ,scorestartingy+(y*16),1 );
            displaynumber(leaderboard.score[y], scorestartingx+32, scorestartingy+y*16, scorestartingsprite+3+y*8);
        }
    }
    
}
//(u16 number, u8 x, u8 y, u8 sprite)

//displaytext(message, length, sprite, x,y,pallette);
