

#include "SpritePositionFunctions.h"

extern u32 xy;

u32 xyoverflow(u16 x, u16 y){
    u32 xy;
    while(x>(240)<<8 && x<(255)<<8){
        if (x<(247)<<8){
            x=0;
        } else{
            x=(240)<<8;
        }
    }

    while (y>(160)<<8 && y<(255)<<8){
        if (y>(210)<<8) {
            y=(160)<<8;
        } else{
            y=0;
        }
    }
    xy=((x<<16)|(y));
    return xy;
}

u32 anglespeed(u8 angle,u16 speed, u16 x, u16 y){
    u32 xy;
    u16 xout;
    u16 yout;
    if (angle==0){
        xout=x+speed;
        yout=y+speed;
        xy=xyoverflow(xout,yout);
        return xy;
    }
    if (angle==1){
        xout=x-speed;
        yout=y+speed;
        xy=xyoverflow(xout,yout);
        return xy;
    }
    if (angle==2){
        xout=x-speed;
        yout=y-speed;
        xy=xyoverflow(xout,yout);
        return xy;
    }
    if (angle==3){
        xout=x+speed;
        yout=y-speed;
        xy=xyoverflow(xout,yout);
        return xy;
        
    }
    else return 0;
            
}
