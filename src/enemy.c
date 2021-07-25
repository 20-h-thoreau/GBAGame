//this is the code used for enemy ai and controlls and so forth. it does not include create enemy bullet since that will likely be foled into the regualr create bullet at some point sone

#include "enemy.h"
#include <tonc_math.h>
#include <tonc_core.h>
#include <tonc_bios.h>
#include "defs.h"

extern u8 level;
vu16 enemyai(u16 PlayerX, u16 PlayerY, u16 PlayerSpeed, u16 EnemyX, u16 EnemyY, u16 EnemySpeed, u8 EnemyAngle, u8 bulletactive){
    vu16 button=0xffff;
    u8 requiredangle=0;
    s16 XDiference=PlayerX-EnemyX;
    s16 YDiference=PlayerY-EnemyY;
    if (XDiference<0 && YDiference<0){
        requiredangle=2;
    }
    else{
        if (XDiference<0 && YDiference>0){
            requiredangle=1;
        }
        else{
            if(XDiference>0 && YDiference<0){
                requiredangle=3;
            }
            //else{
                //if (XDiference>0 && YDiference>0){
                    //requiredangle=0;
                
                //}
                
            //}
        }
    }
    
    button=anglediferencetobuttonpress(requiredangle,EnemyAngle, bulletactive);
    button=speedupslowdown(button,PlayerX,PlayerY,PlayerSpeed,EnemyX,EnemyY,EnemySpeed,EnemyAngle);

    return button;
    
}

vu16 anglediferencetobuttonpress(u8 requiredangle,u8 currentangle, u8 bulletactive){
    vu16 buttonout;
    if (requiredangle!=currentangle){
        if (requiredangle==((currentangle-1)&0x03)){
            //turn right
            buttonout=(0xffff^(1<<left));
        }
        else{
            if (requiredangle==((currentangle+1)&0x03)){
                buttonout=(0xffff^(1<<right));
            }
            
        }
        
    }
    else {
        buttonout=0xffff;
        if (((bulletactive&0x80)==0 && (qran_range(0x00,level)==0)) || (qran_range(0x00,level*2)==0) ){
            buttonout=buttonout^(1<<b);
        }
        
    }
    return buttonout;
}

vu16 speedupslowdown(vu16 button, u16 PlayerX, u16 PlayerSpeed, u16 PlayerY, u16 EnemyX, u16 EnemyY, u16 EnemySpeed, u8 EnemyAngle){
    s16 difx=EnemyX-PlayerX;
    s16 dify=EnemyY-PlayerY;
    u32 pythagdif=Sqrt((difx*difx)+(dify*dify));
    
    if( ((pythagdif<(0xc000))&&(EnemySpeed+0x10>PlayerSpeed)) || (pythagdif<(0x10000)) ){
        button=button^(1<<down);
    }
    else if((pythagdif>0x20000) ||(PlayerSpeed>EnemySpeed) ){
        button=button^(1<<up);
    }

    return button;
}


