#ifndef GUARD_COINS_H
#define GUARD_COINS_H

#include "global.h"

void PrintCoinsString(u32 coinAmount);
void ShowCoinsWindow(u32 coinAmount, u32 x, u32 y);
void HideCoinsWindow(void);
u16 GetCoins(void);
void SetCoins(u16 coinAmount);
bool32 AddCoins(u16 toAdd);
bool32 RemoveCoins(u16 toSub);

#endif // GUARD_COINS_H
