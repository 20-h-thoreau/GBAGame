
#include "gameoverscreen.h"
#include <tonc_oam.h>
#include "defs.h"
#include "Graphics.h"
#include "music.h"
#include "score.h"

void deathanimation(bool playerhit, bool enemyhit,u16 playerx, u16 playery, u8 playerangle, u16 enemyx, u16 enemyy, u8 enemyangle){

    for (u8 timer=0,animationcounter=0; animationcounter<4;++timer){
        if (timer==0x0f){
            timer=0;
            ++animationcounter;
        }
        vsync();
        fillbuffer();
        switchoutdma();
        OAM_CLEAR();
        if (playerhit==false){
            createship(playerx,playery,0,playerangle,true);
        }
        else{
            createship(playerx,playery,0,(8+animationcounter),true);
        }
        
        
        if(enemyhit==false){
            createship(enemyx,enemyy,8,enemyangle,false);
        }
        else{
            createship(enemyx,enemyy,8,(8+animationcounter),true);//this can be changed to false once we fix the problem with the pallete generation
        }
        displayscore();
    }
    
}

void gameoverscreen(void){
    

    bool startpressed=false;
    vu16 button=0xff;
    u8 timer=0;
    bool pressstart=false;

    u8 GameOver[]="GAME OVER";
    char message[11]="PRESS START";

    
    while (startpressed==false){
        vsync();
        fillbuffer();
        switchoutdma();
        OAM_CLEAR();
        displaytext(GameOver, 9, 0, 80,40,01);
        
        if ((timer&0x1f)==0){
            pressstart=(!pressstart); //this flickers the press start text
        }
        if (pressstart==true){
            displaytext(message, 10, 10, 130,130,01);
        }
        qran_range(0,0xffff);
        button=*Button_Reg & 0x03ff;
        if (button==((((0xffff)^(1<<start))) &0x03ff )){
            startpressed=true;
            return;
        }
        ++timer;
        
        
    }
    
}
