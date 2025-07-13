#ifndef GUARD_PARTY_MENU_H
#define GUARD_PARTY_MENU_H

#include "main.h"
#include "task.h"
#include "constants/party_menu.h"

// Return values of GetItemUseAnimFollowUpCBIdByItemType
enum
{
    ITEMUSE_FOLLOWUP_MEDICINE,
    ITEMUSE_FOLLOWUP_TM,
    ITEMUSE_FOLLOWUP_EVOLUTION_ITEM,
};

struct PartyMenu
{
    MainCallback exitCallback;
    TaskFunc task;
    u8 menuType:4;
    u8 layout:2;
    u8 chooseMonsBattleType:2; // Choose half party type
    s8 slotId;
    s8 slotId2;
    u8 action;
    u16 bagItem;
    s16 data1;           // used variously as a moveId, counter, moveSlotId, or cursorPos
    s16 learnMoveState;
};

extern struct PartyMenu gPartyMenu;
extern bool8 gPartyMenuUseExitCallback;
extern u8 gSelectedMonPartyId;
extern MainCallback gPostMenuFieldCallback;
extern u8 gSelectedOrderFromParty[3];
extern u8 gBattlePartyCurrentOrder[PARTY_SIZE / 2];

extern void (*gItemUseCB)(u32, TaskFunc);

void InitPartyMenu(u32 menuType, u32 layout, u32 partyAction, bool32 keepCursorPos, u32 messageId, TaskFunc task, MainCallback callback);
void AnimatePartySlot(u32 slot, u32 animNum);
u32 GetCursorSelectionMonId(void);
u32 GetPartyMenuType(void);
void Task_HandleChooseMonInput(u32 taskId);
void DisplayPartyMenuMessage(const u8 *str, bool32 keepOpen);
bool32 IsPartyMenuTextPrinterActive(void);
void PartyMenuModifyHP(u32 taskId, u32 slot, s8 hpIncrement, s16 hpDifference, TaskFunc task);
u32 GetAilmentFromStatus(u32 statusId);
void DisplayPartyMenuStdMessage(u32 stringId);
void DrawHeldItemIconsForTrade(u8 *partyCounts, u8 *partySpriteIds, u32 whichParty);
void CB2_SelectBagItemToGive(void);
void CB2_GiveHoldItem(void);
void CB2_ReturnToPartyMenuFromFlyMap(void);
void CB2_ShowPartyMenuForItemUse(void);
void ItemUseCB_BattleScript(u32 taskId, TaskFunc task);
void ItemUseCB_Medicine(u32 taskId, TaskFunc func);
void ItemUseCB_PPRecoveryOneMove(u32 taskId, UNUSED TaskFunc func);
void ItemUseCB_PPRecoveryAllMoves(u32 taskId, UNUSED TaskFunc func);
void ItemUseCB_PPUp(u32 taskId, UNUSED TaskFunc func);
void ItemUseCB_TM(u32 taskId, UNUSED TaskFunc func);
void ItemUseCB_SacredAsh(u32 taskId, UNUSED TaskFunc func);
void ItemUseCB_EvolutionStone(u32 taskId, TaskFunc func);
void ItemUseCB_ChangeAbility(u32 taskId, TaskFunc func);
void ItemUseCB_Mint(u32 taskId, TaskFunc func);
void ItemUseCB_FormChange(u32 taskId, TaskFunc func);
void ItemUseCB_FormChangeListMenu(u32 taskId, TaskFunc func);
void ItemUseCB_Fusion(u32 taskId, TaskFunc func);
void CB2_PartyMenuFromStartMenu(void);
void CB2_ChooseMonToGiveItem(void);
void ChooseMonToGiveMailFromMailbox(void);
void InitChooseHalfPartyForBattle(u32 battleType);
void ClearSelectedPartyOrder(void);
void ChooseMonForTradingBoard(u32 menuType, MainCallback callback);
void ChooseMonForMoveTutor(void);
void ChooseMonForWirelessMinigame(void);
void OpenPartyMenuInTutorialBattle(u32 partyAction);
void Pokedude_OpenPartyMenuInBattle(void);
void Pokedude_ChooseMonForInBattleItem(void);
void EnterPartyFromItemMenuInBattle(void);
void BufferBattlePartyCurrentOrder(void);
void BufferBattlePartyCurrentOrderBySide(u32 battlerId, u32 flankId);
void SwitchPartyOrderLinkMulti(u32 battlerId, u32 slot, u32 slot2);
void SwitchPartyMonSlots(u32 slot, u32 slot2);
u32 GetPartyIdFromBattlePartyId(u32 battlePartyId);
void ShowPartyMenuToShowcaseMultiBattleParty(void);
void ChooseMonForDaycare(void);
void UpdateCurrentPartyMonIconSpecies(u32 species);
void UpdatePartyAfterPokemonFusion(void);
void CreatePartyMonAfterDefusing(void);
void SetBattlerUsedItemForBattleScript(u32 item, bool32 fromBagMenu);
void CB2_ReturnToPartyMenuFromSummaryScreen(void);
u8 GetPartyIdFromBattleSlot(u32 slot);
u32 GetItemUseAnimFollowUpCBIdByItemType(u32 itemId);
void SwapPartyPokemon(struct Pokemon *mon1, struct Pokemon *mon2);

#endif // GUARD_PARTY_MENU_H
