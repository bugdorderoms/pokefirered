#ifndef GUARD_ITEM_MENU_H
#define GUARD_ITEM_MENU_H

#include "global.h"
#include "task.h"
#include "constants/item_menu.h"

// Exported type declarations

// Exported RAM declarations

struct BagStruct
{
    MainCallback bagCallback;
    u8 location;
    bool8 bagOpen;
    u16 pocket;
    u16 itemsAbove[3];
    u16 cursorPos[3];
};

extern struct BagStruct gBagMenuState;

extern u16 gSpecialVar_ItemId;

// Exported ROM declarations
void CB2_SetUpReshowBattleScreenAfterMenu(void);
void ResetBagCursorPositions(void);
void InitPokedudeBag(u32);
void CB2_BagMenuFromStartMenu(void);
void MoveItemSlotInList(struct ItemSlot * itemSlots, u32 from, u32 to);
void ItemMenu_StartFadeToExitCallback(u32 taskId);
void Bag_BeginCloseWin0Animation(void);
void ItemMenu_SetExitCallback(void (*)(void));
void DisplayItemMessageInBag(u32 taskId, u32 fontId, const u8 * string, TaskFunc followUpFunc);
void Task_ReturnToBagFromContextMenu(u32 taskId);
void CB2_BagMenuFromBattle(void);
void CB2_ChooseBall(void);
void InitOldManBag(void);
void Pocket_CalculateNItemsAndMaxShowed(u32 pocketId);
void PocketCalculateInitialCursorPosAndItemsAbove(u32 pocketId);
void GoToBagMenu(u32 menuType, u32 pocket, MainCallback callback);
bool32 UseRegisteredKeyItemOnField(void);
void ItemUseInBattle(u32 taskId);

extern const u8 gText_ItemMenuUse[];
extern const u8 gText_ItemMenuGive[];
extern const u8 gText_ItemMenuToss[];

extern const u8 gText_ThereIsNoPokemon[];
extern const u8 gText_Var1IsSelected[];
extern const u8 gOtherText_WhereShouldTheStrVar1BePlaced[];
extern const u8 gText_TossOutHowManyStrVar1s[];
extern const u8 gText_ThrewAwayStrVar2StrVar1s[];
extern const u8 gText_ThrowAwayStrVar2OfThisItemQM[];

#endif //GUARD_ITEM_MENU_H
