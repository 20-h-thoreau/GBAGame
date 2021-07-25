

#ifndef enemy_h
#define enemy_h

    #include <tonc_types.h>
    
    vu16 enemyai(u16 PlayerX, u16 PlayerY, u16 PlayerSpeed, u16 EnemyX, u16 EnemyY, u16 EnemySpeed, u8 EnemyAngle, u8 bulletactive, u8 PlayerAngle);
    vu16 anglediferencetobuttonpress(u8 requiredangle,u8 currentangle, u8 bulletactive);
    vu16 speedupslowdown(vu16 button, u16 PlayerX, u16 PlayerY, u16 PlayerSpeed, u16 EnemyX, u16 EnemyY, u16 EnemySpeed, u8     EnemyAngle);
    vu16 enemyattack(vu16 button, s16 difx, s16 dify, u16 EnemyAngle);

#endif
