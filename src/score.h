//
//  score.h
//  hellogba
//
//  Created by Thoreau Kilbourne on 7/27/21.
//

#ifndef score_h
#define score_h

#include <tonc_types.h>
void displayscore(void);
void savescore();
void initleaderboard();
void leaderboardpreset();
void displayscoreboard();
void displaynumber(u16 number, u8 x, u8 y, u8 sprite);
#endif /* score_h */
