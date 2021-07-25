
#include "gameoverscreen.h"
#include <tonc_oam.h>
#include "defs.h"
#include "Graphics.h"

void gameoverscreen(void){
    
    bool startpressed=false;
    vu16 button=0xff;
    u8 timer=0;
    bool pressstart=false;

    u8 GameOver[]="GAME OVER";
    char message[11]="PRESS START";

    
    while (startpressed==false){
        vsync();
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
