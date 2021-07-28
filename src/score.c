#include "Graphics.h"
#include "posprintf.h"

extern u16 score;

typedef struct Leaderboard{
    volatile char name[10][3];
    vu16 score[10];
} Leaderboard;

struct Leaderboard leaderboard;

void displayscore(){ //displays the score at the top right side of screen;
    char message[5];
    posprintf(message, "%5d", score);
    displaytext(message, 5, 16, 192,4,1);
    
}



void savescore(){
    
}

void initleaderboard(){
   
    for(u8 x=0,y=0; y<=10;++x){
        leaderboard.name[y][x]=0;
        if (x==2){
            x=0xff;
            leaderboard.score[y]=0;
            ++y;
        }
        
    }

}
void leaderboardpreset(){
    leaderboard.name[1][0]='z';
    leaderboard.name[1][1]='a';
    leaderboard.name[1][2]='k';
    leaderboard.score[1]=3;
}
