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

extern void (*gItemUseCB)(u8, TaskFunc);

void InitPartyMenu(u8 menuType, u8 layout, u8 partyAction, bool8 keepCursorPos, u8 messageId, TaskFunc task, MainCallback callback);
void AnimatePartySlot(u8 slot, u8 animNum);
u8 GetCursorSelectionMonId(void);
u8 GetPartyMenuType(void);
void Task_HandleChooseMonInput(u8 taskId);
u8 DisplayPartyMenuMessage(const u8 *str, bool8 keepOpen);
bool8 IsPartyMenuTextPrinterActive(void);
void PartyMenuModifyHP(u8 taskId, u8 slot, s8 hpIncrement, s16 hpDifference, TaskFunc task);
u8 GetAilmentFromStatus(u8 statusId);
void DisplayPartyMenuStdMessage(u32 stringId);
void DrawHeldItemIconsForTrade(u8 *partyCounts, u8 *partySpriteIds, u8 whichParty);
void SpriteCB_BounceConfirmCancelButton(u8 spriteId, u8 spriteId2, u8 animNum);
void CB2_SelectBagItemToGive(void);
void CB2_GiveHoldItem(void);
void CB2_ReturnToPartyMenuFromFlyMap(void);
void CB2_ShowPartyMenuForItemUse(void);
void ItemUseCB_BattleScript(u8 taskId, TaskFunc task);
void ItemUseCB_Medicine(u8 taskId, TaskFunc func);
void ItemUseCB_PPRecoveryOneMove(u8 taskId, UNUSED TaskFunc func);
void ItemUseCB_PPRecoveryAllMoves(u8 taskId, UNUSED TaskFunc func);
void ItemUseCB_PPUp(u8 taskId, UNUSED TaskFunc func);
void ItemUseCB_TM(u8 taskId, UNUSED TaskFunc func);
void ItemUseCB_SacredAsh(u8 taskId, UNUSED TaskFunc func);
void ItemUseCB_EvolutionStone(u8 taskId, TaskFunc func);
void ItemUseCB_ChangeAbility(u8 taskId, TaskFunc func);
void ItemUseCB_Mint(u8 taskId, TaskFunc func);
void ItemUseCB_FormChange(u8 taskId, TaskFunc func);
void ItemUseCB_FormChangeListMenu(u8 taskId, TaskFunc func);
void ItemUseCB_Fusion(u8 taskId, TaskFunc func);
void CB2_PartyMenuFromStartMenu(void);
void CB2_ChooseMonToGiveItem(void);
void ChooseMonToGiveMailFromMailbox(void);
void InitChooseHalfPartyForBattle(u8 battleType);
void ClearSelectedPartyOrder(void);
void ChooseMonForTradingBoard(u8 menuType, MainCallback callback);
void ChooseMonForMoveTutor(void);
void ChooseMonForWirelessMinigame(void);
void OpenPartyMenuInTutorialBattle(u8 partyAction);
void Pokedude_OpenPartyMenuInBattle(void);
void Pokedude_ChooseMonForInBattleItem(void);
void EnterPartyFromItemMenuInBattle(void);
void BufferBattlePartyCurrentOrder(void);
void BufferBattlePartyCurrentOrderBySide(u8 battlerId, u8 flankId);
void SwitchPartyOrderLinkMulti(u8 battlerId, u8 slot, u8 slot2);
void SwitchPartyMonSlots(u8 slot, u8 slot2);
u8 GetPartyIdFromBattlePartyId(u8 battlePartyId);
void ShowPartyMenuToShowcaseMultiBattleParty(void);
void ChooseMonForDaycare(void);
void UpdateCurrentPartyMonIconSpecies(u16 species);
void UpdatePartyAfterPokemonFusion(void);
void CreatePartyMonAfterDefusing(void);
void SetBattlerUsedItemForBattleScript(u16 item, bool8 fromBagMenu);
void CB2_ReturnToPartyMenuFromSummaryScreen(void);
u8 GetPartyIdFromBattleSlot(u8 slot);
u8 GetItemUseAnimFollowUpCBIdByItemType(u16 itemId);

#endif // GUARD_PARTY_MENU_H
