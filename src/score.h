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
void addtoleaderboard(char l1, char l2, char l3);
u8 leaderboardpositionfinder(void);
bool checkforhighscore();
void openupboardposition();
void addunderline(u8 leterpointer);
#endif /* score_h */
