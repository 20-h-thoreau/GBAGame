
#ifndef definitions_h
#define definitions_h

#include <tonc_types.h>

    
typedef u32 tile[16];
typedef tile TileBlock[256];
typedef u16 ScreenBlock[1024];

#define BACKGROUND_0 0x0100

#define Mem_VRam ((vu16*)0x06000000)
#define Mem_VRamTile4 ((vu16*)0x06010000)
#define Mem_Tile ((TileBlock*)Mem_VRam)
#define PaletteMem ((u16*)0x05000200)
#define BGPaletteMem ((u16*)0x05000000)
#define DisplayController (*(vu16*)0x04000000)
#define Reg_VCount      (*(vu16*)0x04000006)
#define Reg_Bg0_Control        *((vu16*)(0x04000008))
#define REG_BG1_CONTROL        *((vu16*)(0x0400000A))
#define REG_BG2_CONTROL        *((vu16*)(0x0400000C))
#define REG_BG3_CONTROL        *((vu16*)(0x0400000E))

#define VIDEOMODE_0    0x0000
#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040
#define Mem_Bg_Palette ((u16*)(0x05000000))
#define Mem_Obj_Palette ((u16*)(0x05000200))
#define Mem_ScreenBlocks ((ScreenBlock*)0x6000000)
#define Obj_Attributes ((u16*)(0x07000000))
#define Button_Reg ((vu16*)(0x04000130))
#define Mem_OAM       ((volatile ObjectAttributes *)0x07000000)

extern u16 const  down;
extern u16 const  up;
extern u16 const  left;
extern u16 const  right;
extern u16 const  b;
extern u16 const a;
extern u16 const  start;

extern u8 const ScreenWidth;
extern u8 const ScreenHieght;//these were orginally u8 constances


#define startinglevel 0x01





//structs

typedef struct ObjectAttribute{
    vu16 attr0;
    vu16 attr1;
    vu16 attr2;
    vu16 pad;
} __attribute__((packed, aligned(4))) ObjectAttributes;

typedef struct BulletAttribute{

    bool ally; //who shot the bullet, 0=player 1=enemy
    u16 x;
    u16 y; //bullets have constant speed
    u8 BulletTimer; //bullets disapear after some number of frames.
    u16 BulletSpeed;  //could be lower, well see
    u8 angle;
    
} __attribute__((packed, aligned(4))) BulletAttributes;


typedef struct Ship{

    u16 x;
    u16 y;
    u16 speed;
    vu16 button;
    u8 angle;
    u8 turntimer;
    u8 acceleration;
    u8 deceleration;
    u16 maxspeed;
    u16 minspeed;
    u8 bulletactive;
    bool ally;
    bool hit;
} __attribute__((packed, aligned(4))) Ship;


#endif /* definitions_h */
