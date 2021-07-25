#include "Graphics.h"

void displaytext(char string[], u8 length, u16 StartingSprite, u8 StartingX, u8 StartingY, u8 Pallete){
    StartingSprite=StartingSprite*4;
    for(u8 i=0; i<=length; ++i){
        //draw letter of string
        Obj_Attributes[StartingSprite+(i*4)]=0b0000000000000000+(StartingY);
        Obj_Attributes[StartingSprite+(i*4)+1]=0b0000000000000000+(StartingX+(i*8));
        Obj_Attributes[StartingSprite+(i*4)+2]=0b0000000000000000+(string[i]+(Pallete<<0xc));
        
    }
    return;
}


void createship(u16 x, u16 y, u16 sprite, u8 direction, bool ally){
    sprite=sprite*4;
    u8 pallete;
    if (ally==true){
        pallete=0;
    }
    else{
        pallete=0x1;
    }
    Obj_Attributes[sprite]=0b0000000000000000+(y>>8);
    Obj_Attributes[sprite+1]=0b0000000000000000+(x>>8);
    Obj_Attributes[sprite+2]=0b0000000000000000+(direction+(pallete<<0xc)+2);
    
}


void createbullet(u16 x, u16 y, u16 sprite, u8 direction,bool ally){
    u8 pallete;
    
    if (ally==true){
        pallete=0;
    }
    else{
        pallete=0x1;
    }

    //turnthisinto a seperate fucntion called draw bullet
    u16 OamLocation;
    OamLocation=(sprite * 4);
    Obj_Attributes[OamLocation]=0b0000000000000000+(y>>8);
    Obj_Attributes[OamLocation+1]=0b0000000000000000+(x>>8);
    Obj_Attributes[OamLocation+2]=0b0000000000000000+(4<<1)+(pallete<<0xc);
    
}



