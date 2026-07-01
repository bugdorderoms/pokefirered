#ifndef GUARD_MONEY_H
#define GUARD_MONEY_H

#include "global.h"

u32 GetMoney(u32* moneyPtr);
void SetMoney(u32* moneyPtr, u32 newValue);
bool32 IsEnoughMoney(u32* moneyPtr, u32 cost);
void AddMoney(u32* moneyPtr, u32 toAdd);
void RemoveMoney(u32* moneyPtr, u32 toSub);
bool32 IsEnoughForCostInVar0x8005(void);
void SubtractMoneyFromVar0x8005(void);
void PrintMoneyAmountInMoneyBox(u32 windowId, u32 amount, u32 speed);
void PrintMoneyAmountInMoneyBoxWithBorder(u32 windowId, u32 tileStart, u32 pallete, u32 amount);
void ChangeAmountInMoneyBox(u32 amount);
void DrawMoneyBox(u32 amount, u32 x, u32 y);
void HideMoneyBox(void);
void PrintMoneyAmount(u32 windowId, s32 x, u32 y, u32 amount, u32 speed);

extern const u8 gText_PokedollarSymbol[];
extern const u8 gText_PokedollarVar1[];

#endif // GUARD_MONEY_H
