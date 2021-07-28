#include "Graphics.h"
#include "posprintf.h"

extern u16 score;

void displayscore(){ //displays the score at the top right side of screen;
    char message[5];
    posprintf(message, "%5d", score);
    displaytext(message, 5, 16, 192,4,1);
    
}



void savescore(){
    
}
