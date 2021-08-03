
#include "gameoverscreen.h"
#include <tonc_oam.h>
#include "defs.h"
#include "Graphics.h"
#include "music.h"
#include "score.h"

extern u16 score;

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
    
    bool newhighscore=checkforhighscore();
    bool save=newhighscore;
    if (newhighscore==true){
        openupboardposition();
    }
    bool startpressed=false;
    vu16 button=0xffff;
    u8 timer=0;
    bool pressstart=false;

    char GameOver[]="GAME OVER";
    char message[11]="PRESS START";

    
    u8 leters[3];
    leters[0]='A';
    leters[1]='A';
    leters[2]='A';
    
    button=0xffff;
    u8 leterpointer=0;
   
        
    while (startpressed==false){
        
        vsync();
        fillbuffer();
        switchoutdma();
        OAM_CLEAR();

        displayscoreboard();
        displaytext(GameOver, 9, 0, 80,32,01);
        
        if ((timer&0x1f)==0){
            pressstart=(!pressstart); //this flickers the press start text
        }
        if (pressstart==true){
            displaytext(message, 10, 10, 130,130,01);
        }
        qran_range(0,0xffff);
        
        button=*Button_Reg;
        
        if (newhighscore==true){
            addunderline(leterpointer);
            
            if ((timer&0x7)==0){
                
                if (  (button==(0x03ff^(1<<b))) || (button==(0x03ff^(1<<a))) || (button==(0x03ff^(1<<start))) ){
                    ++leterpointer;
                    if (leterpointer>=3){
                        newhighscore=false;
                    }
                }
                else{
                    if (button==(0x03ff^(1<<down))){
                        ++leters[leterpointer];
                        if (leters[leterpointer]>0x5a){
                            leters[leterpointer]=0x41;
                        }
                        
                    }
                    else{
                        if (button==(0x03ff^(1<<up))){
                            --leters[leterpointer];
                         
                            if(leters[leterpointer]<0x41){
                                leters[leterpointer]=0x5a;
                            }
                        }
                    }

            
                }
            
         
                addtoleaderboard(leters[0],leters[1],leters[2]);
                
            }
        }
        else{
            if (button==((((0x03ff)^(1<<start))) )){
                startpressed=true;
            }
        }
        
            
        
    

        ++timer;

        
        
    }
    if(save==true){
        savescore();
    }
    return;
    
}


