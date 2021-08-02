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
void savescore();
void loadscore();
bool verifysave();
void sram_memcpy(volatile unsigned char *dst, const volatile unsigned char *src, size_t size);
bool sram_memcmp(const volatile unsigned char *dst, const volatile unsigned char *src, size_t size);
#endif /* score_h */
