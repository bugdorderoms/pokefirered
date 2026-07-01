#ifndef GUARD_SHOP_H
#define GUARD_SHOP_H

#include "global.h"
#include "menu_helpers.h"

#define INDEX_CANCEL -2

extern EWRAM_DATA struct ItemSlot gUnknown_02039F80[3];

void CreatePokemartMenu(const u16 *itemsForSale);
void CreateDecorationShop1Menu(const u16 *);
void CreateDecorationShop2Menu(const u16 *);
u32 GetMartUnk16_4(void);

// buy_menu_helper
void BuyMenuInitWindows(bool32 isSellingTM);
void BuyMenuPrint(u32 windowId, u32 font, const u8 *text, u32 x, u32 y, u32 letterSpacing, u32 lineSpacing, u32 speed, u32 color);
void BuyMenuDisplayMessage(u32 taskId, const u8 *text, TaskFunc callback);
void BuyMenuConfirmPurchase(u32 taskId, const struct YesNoFuncTable *yesNo);

extern const u8 gText_OhNoICantBuyThat[];
extern const u8 gText_HowManyWouldYouLikeToSell[];
extern const u8 gText_ICanPayThisMuch_WouldThatBeOkay[];
extern const u8 gText_TurnedOverItemsWorthYen[];

#endif // GUARD_SHOP_H
