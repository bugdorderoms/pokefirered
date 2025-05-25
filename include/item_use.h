#ifndef GUARD_ITEM_USE_H
#define GUARD_ITEM_USE_H

#include "global.h"

void Task_ItemUse_CloseMessageBoxAndReturnToField(u32);

void FieldUseFunc_OrangeMail(u32 taskId);
void FieldUseFunc_MachBike(u32 taskId);
void FieldUseFunc_OldRod(u32 taskId);
void FieldUseFunc_CoinCase(u32 taskId);
void FieldUseFunc_PokeFlute(u32 taskId);
void FieldUseFunc_Medicine(u32 taskId);
void FieldUseFunc_Ether(u32 taskId);
void FieldUseFunc_Elixir(u32 taskId);
void FieldUseFunc_PpUp(u32 taskId);
void FieldUseFunc_EvoItem(u32 taskId);
void FieldUseFunc_SacredAsh(u32 taskId);
void FieldUseFunc_TmCase(u32 taskId);
void FieldUseFunc_BerryPouch(u32 taskId);
void FieldUseFunc_TeachyTv(u32 taskId);
void FieldUseFunc_SuperRepel(u32 taskId);
void FieldUseFunc_BlackFlute(u32 taskId);
void FieldUseFunc_TownMap(u32 taskId);
void FieldUseFunc_FameChecker(u32 taskId);
void FieldUseFunc_VsSeeker(u32 taskId);
void FieldUseFunc_Honey(u32 taskId);
void FieldUseFunc_ExpShare(u32 taskId);
void FieldUseFunc_EvIvDisplay(u32 taskId);
void FieldUseFunc_FormChangeItem(u32 taskId);
void FieldUseFunc_FormChangeItemListMenu(u32 taskId);
void FieldUseFunc_FusionItem(u32 taskId);
void FieldUseFunc_ChangeAbility(u32 taskId);
void FieldUseFunc_Mint(u32 taskId);
void BattleUseFunc_BerryPouch(u32 taskId);
void ItemUseInBattle_BagMenu(u32 taskId);
void ItemUseInBattle_PartyMenu(u32 taskId);
void ItemUseInBattle_PartyMenuChooseMove(u32 taskId);
void FieldUseFunc_OakStopsYou(u32 taskId);
void ItemUseOutOfBattle_EscapeRope(u32 taskId);
void ItemUseOutOfBattle_Itemfinder(u32 taskId);
void sub_80A1C44(u32 taskId);
bool32 CanUseEscapeRopeOnCurrMap(void);
bool32 CanUseItemInBattle(bool32 fromBagMenu, u32 itemId);
bool32 ExecuteTableBasedItemEffect(u32 partyMonIndex, u32 item);

#endif //GUARD_ITEM_USE_H
