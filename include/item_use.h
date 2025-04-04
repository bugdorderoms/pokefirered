#ifndef GUARD_ITEM_USE_H
#define GUARD_ITEM_USE_H

#include "global.h"

void Task_ItemUse_CloseMessageBoxAndReturnToField(u8);

void FieldUseFunc_OrangeMail(u8 taskId);
void FieldUseFunc_MachBike(u8 taskId);
void FieldUseFunc_OldRod(u8 taskId);
void FieldUseFunc_CoinCase(u8 taskId);
void FieldUseFunc_PokeFlute(u8 taskId);
void FieldUseFunc_Medicine(u8 taskId);
void FieldUseFunc_Ether(u8 taskId);
void FieldUseFunc_Elixir(u8 taskId);
void FieldUseFunc_PpUp(u8 taskId);
void FieldUseFunc_EvoItem(u8 taskId);
void FieldUseFunc_SacredAsh(u8 taskId);
void FieldUseFunc_TmCase(u8 taskId);
void FieldUseFunc_BerryPouch(u8 taskId);
void FieldUseFunc_TeachyTv(u8 taskId);
void FieldUseFunc_SuperRepel(u8 taskId);
void FieldUseFunc_BlackFlute(u8 taskId);
void FieldUseFunc_TownMap(u8 taskId);
void FieldUseFunc_FameChecker(u8 taskId);
void FieldUseFunc_VsSeeker(u8 taskId);
void FieldUseFunc_Honey(u8 taskId);
void FieldUseFunc_ExpShare(u8 taskId);
void FieldUseFunc_EvIvDisplay(u8 taskId);
void FieldUseFunc_FormChangeItem(u8 taskId);
void FieldUseFunc_FormChangeItemListMenu(u8 taskId);
void FieldUseFunc_FusionItem(u8 taskId);
void FieldUseFunc_ChangeAbility(u8 taskId);
void FieldUseFunc_Mint(u8 taskId);
void BattleUseFunc_BerryPouch(u8 taskId);
void ItemUseInBattle_BagMenu(u32 taskId);
void ItemUseInBattle_PartyMenu(u32 taskId);
void ItemUseInBattle_PartyMenuChooseMove(u32 taskId);
void FieldUseFunc_OakStopsYou(u8 taskId);
void ItemUseOutOfBattle_EscapeRope(u8 taskId);
void ItemUseOutOfBattle_Itemfinder(u8 taskId);
void sub_80A1C44(u8 taskId);
bool32 CanUseEscapeRopeOnCurrMap(void);
bool32 CanUseItemInBattle(bool32 fromBagMenu, u32 itemId);
bool32 ExecuteTableBasedItemEffect(u32 partyMonIndex, u32 item);

#endif //GUARD_ITEM_USE_H
