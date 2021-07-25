//
//  Graphics.h
//  hellogba
//
//  Created by Thoreau Kilbourne on 7/24/21.
//

#ifndef Graphics_h
#define Graphics_h

#include <tonc_types.h>
#include "defs.h"

inline void vsync()
{
    while (Reg_VCount >= 160);
    while (Reg_VCount < 160);
}

void createbullet(u16 x, u16 y, u16 sprite, u8 direction, bool ally);
void createenemybullet(u16 x, u16 y, u16 sprite, u8 direction, bool ally);
void createship(u16 x, u16 y, u16 sprite, u8 direction,bool ally);


void displaytext(char string[], u8 length, u16 StartingSprite, u8 StartingX, u8 StartingY, u8 Pallete);

#include <stdio.h>

#endif /* Graphics_h */
