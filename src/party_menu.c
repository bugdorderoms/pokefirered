#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "berry_pouch.h"
#include "data.h"
#include "decompress.h"
#include "dns.h"
#include "script_menu.h"
#include "easy_chat.h"
#include "util.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "field_specials.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_fadetransition.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "fldeff.h"
#include "form_change.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "item_use.h"
#include "link.h"
#include "evolution.h"
#include "link_rfu.h"
#include "load_save.h"
#include "mail.h"
#include "mail_data.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "naming_screen.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "party_menu.h"
#include "player_pc.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_jump.h"
#include "pokemon_special_anim.h"
#include "pokemon_summary_screen.h"
#include "region_map.h"
#include "reshow_battle_screen.h"
#include "ride_pager.h"
#include "scanline_effect.h"
#include "script.h"
#include "learn_move.h"
#include "start_menu.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "teachy_tv.h"
#include "text_window.h"
#include "tm_case.h"
#include "trade.h"
#include "union_room.h"
#include "constants/battle.h"
#include "constants/easy_chat.h"
#include "constants/field_effects.h"
#include "constants/form_change.h"
#include "constants/hold_effects.h"
#include "constants/item_effects.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/menu.h"
#include "constants/pokemon.h"
#include "constants/poke_ride.h"
#include "constants/songs.h"

#define PARTY_PAL_SELECTED     (1 << 0)
#define PARTY_PAL_FAINTED      (1 << 1)
#define PARTY_PAL_TO_SWITCH    (1 << 2)
#define PARTY_PAL_MULTI_ALT    (1 << 3)
#define PARTY_PAL_SWITCHING    (1 << 4)
#define PARTY_PAL_TO_SOFTBOIL  (1 << 5)
#define PARTY_PAL_NO_MON       (1 << 6)
#define PARTY_PAL_UNUSED       (1 << 7)

#define MENU_DIR_DOWN     1
#define MENU_DIR_UP      -1
#define MENU_DIR_RIGHT    2
#define MENU_DIR_LEFT    -2

enum
{
    CAN_LEARN_MOVE,
    CANNOT_LEARN_MOVE,
    ALREADY_KNOWS_MOVE,
    CANNOT_LEARN_MOVE_IS_EGG
};

enum
{
	ITEMUSE_STRING_NOTHING, // don't print message
	ITEMUSE_STRING_PP_RESTORED,
	ITEMUSE_STRING_POISON_CURED,
	ITEMUSE_STRING_BURN_HEALED,
	ITEMUSE_STRING_THAWED,
	ITEMUSE_STRING_WOKE_UP,
	ITEMUSE_STRING_SNAPPED_CONFUSION,
	ITEMUSE_STRING_OVER_INFATUATION,
	ITEMUSE_STRING_BECAME_HEALTHY,
	ITEMUSE_STRING_RAISE_DYNAMAX_LEVEL,
	ITEMUSE_STRING_STAT_CHANGED,
	ITEMUSE_STRING_PARALYSIS_CURED,
	ITEMUSE_STRING_LEVELED_UP,
	ITEMUSE_STRING_GAINED_EXP,
	ITEMUSE_STRING_GAINED_EXP_LEVELED_UP,
	ITEMUSE_STRING_PP_INCREASED,
	ITEMUSE_STRING_CHANGE_GMAX_FACTOR,
	ITEMUSE_STRING_CHANGED_TERA_TYPE,
	ITEMUSE_STRING_FRIENDSHIP_CHANGED
};

enum
{
	ITEMUSE_COPY_NOTHING, // don't copy anything
	ITEMUSE_COPY_STAT_NAME,
	ITEMUSE_COPY_EXP_AND_LEVEL,
	ITEMUSE_COPY_GAINED_OR_LOSES,
	ITEMUSE_COPY_TYPE_NAME,
	ITEMUSE_COPY_INCREASED_OR_DECREASED
};

struct PartyMenuBoxInfoRects
{
    void (*blitFunc)(u8 windowId, u8 x, u8 y, u8 width, u8 height, bool8 isEgg);
    u8 dimensions[24];
    u8 descTextLeft;
    u8 descTextTop;
    u8 descTextWidth;
    u8 descTextHeight;
};

struct PartyMenuInternal
{
    TaskFunc task;
    MainCallback exitCallback;
    u32 chooseHalf:1;
    u32 lastSelectedSlot:3;  // Used to return to same slot when going left/right bewtween columns
    u32 spriteIdConfirmPokeball:7;
    u32 spriteIdCancelPokeball:7;
    u32 messageId:14;
    u8 windowId[3];
    u8 actions[10];
    u8 numActions;
    u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];
    s16 data[16];
};

struct PartyMenuBox
{
    const struct PartyMenuBoxInfoRects *infoRects;
    const u8 *spriteCoords;
    u8 windowId;
    u8 monSpriteId;
    u8 itemSpriteId;
    u8 pokeballSpriteId;
    u8 statusSpriteId;
};

struct ItemUseData
{
	u16 stringToPrintId;
	u8 specialStringCopyId;
	bool8 messagePrinted:1;
	bool8 dataCopied:1;
	u8 unused:6;
	u32 stringCopyData;
};

struct MedicineItemData
{
	void (*savedItemUseCB)(u8, TaskFunc);
	s16 oldHP;
	u16 newHP;
	u32 healStatusMask;
	u8 initialLevel;
	u8 finalLevel;
	u16 stringData;
};

struct FormChangeMove
{
	u16 species;
	u16 moveToLearn;
	u16 replaceMove1;
	u16 replaceMove2;
};

static void BlitBitmapToPartyWindow_LeftColumn(u8 windowId, u8 x, u8 y, u8 width, u8 height, bool8 isEgg);
static void BlitBitmapToPartyWindow_RightColumn(u8 windowId, u8 x, u8 y, u8 width, u8 height, bool8 isEgg);
static void CursorCB_Summary(u8 taskId);
static void CursorCB_Nickname(u8 taskId);
static void CursorCB_Moves(u8 taskId);
static void CursorCB_RelearnMove(u8 taskId);
static void CursorCB_DeleteMove(u8 taskId);
static void CursorCB_Switch(u8 taskId);
static void CursorCB_Cancel1(u8 taskId);
static void CursorCB_Item(u8 taskId);
static void CursorCB_Give(u8 taskId);
static void CursorCB_TakeItem(u8 taskId);
static void CursorCB_MoveItem(u8 taskId);
static void CursorCB_Mail(u8 taskId);
static void CursorCB_Read(u8 taskId);
static void CursorCB_TakeMail(u8 taskId);
static void CursorCB_Cancel2(u8 taskId);
static void CursorCB_SendMon(u8 taskId);
static void CursorCB_Enter(u8 taskId);
static void CursorCB_NoEntry(u8 taskId);
static void CursorCB_Store(u8 taskId);
static void CursorCB_Register(u8 taskId);
static void CursorCB_Trade1(u8 taskId);
static void CursorCB_FieldMove(u8 taskId);
static bool8 SetUpFieldMove_Waterfall(void);
static void CB2_InitPartyMenu(void);
static void ResetPartyMenu(void);
static bool8 ShowPartyMenu(void);
static void SetPartyMonsAllowedInMinigame(void);
static void ExitPartyMenu(void);
static bool8 CreatePartyMonSpritesLoop(void);
static bool8 AllocPartyMenuBg(void);
static bool8 AllocPartyMenuBgGfx(void);
static void InitPartyMenuWindows(u8 layout);
static void InitPartyMenuBoxes(u8 layout);
static void LoadPartyMenuPokeballGfx(void);
static void LoadPartyMenuAilmentGfx(void);
static bool8 RenderPartyMenuBoxes(void);
static void CreateCancelConfirmPokeballSprites(void);
static void CreateCancelConfirmWindows(bool8 chooseHalf);
static void Task_ExitPartyMenu(u8 taskId);
static void FreePartyPointers(void);
static void PartyPaletteBufferCopy(u8 offset);
static u16 *GetPartyMenuPalBufferPtr(u8 paletteId);
static void DisplayPartyPokemonDataForMultiBattle(u8 slot);
static void DisplayPartyPokemonDataForChooseHalf(u8 slot);
static void DisplayPartyPokemonDataForMoveTutor(u8 slot);
static bool8 DisplayPartyPokemonDataForTeachMoveOrEvolutionItem(u8 slot);
static void DisplayPartyPokemonData(u8 slot);
static void DisplayPartyPokemonDataForWirelessMinigame(u8 slot);
static void LoadPartyBoxPalette(struct PartyMenuBox *menuBox, u8 palFlags);
static void DrawEmptySlot(u8 windowId);
static void DisplayPartyPokemonNickname(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c);
static void DisplayPartyPokemonLevelCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c);
static void DisplayPartyPokemonGenderBlitFunc(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c);
static void DisplayPartyPokemonHPCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c);
static void DisplayPartyPokemonMaxHPCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c);
static void DisplayPartyPokemonHPBarCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox);
static void DisplayPartyPokemonDescriptionText(u8 stringId, struct PartyMenuBox *menuBox, u8 c);
static bool8 GetBattleEntryEligibility(struct Pokemon *mon);
static void DisplayPartyPokemonDataToTeachMove(u8 slot, u16 move, bool8 isTM);
static u8 CanMonLearnTMOrMoveTutor(struct Pokemon *mon, u16 move, bool8 isTM);
static void DisplayPartyPokemonBarDetail(u8 windowId, u8 fontId, const u8 *str, u8 color, const u8 *align);
static void DisplayPartyPokemonLevel(u8 level, struct PartyMenuBox *menuBox);
static void DisplayPartyPokemonGender(u8 gender, u16 species, u8 *nickname, struct PartyMenuBox *menuBox);
static void DisplayPartyPokemonHP(u16 hp, u16 maxHp, struct PartyMenuBox *menuBox);
static void DisplayPartyPokemonMaxHP(u16 maxhp, struct PartyMenuBox *menuBox);
static void DisplayPartyPokemonHPBar(u16 hp, u16 maxhp, struct PartyMenuBox *menuBox);
static void CreatePartyMonIconSpriteParameterized(u16 species, struct PartyMenuBox *menuBox, u8 priority);
static void CreatePartyMonHeldItemSpriteParameterized(u8 slot, u16 species, u16 item, struct PartyMenuBox *menuBox, u8 priority);
static void CreatePartyMonPokeballSpriteParameterized(u16 species, struct PartyMenuBox *menuBox);
static void CreatePartyMonStatusSpriteParameterized(u16 species, u8 status, struct PartyMenuBox *menuBox);
static void CreatePartyMonIconSprite(struct Pokemon *mon, struct PartyMenuBox *menuBox, u32 slot);
static void CreatePartyMonHeldItemSprite(u8 slot, struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 priority);
static void CreatePartyMonPokeballSprite(struct Pokemon *mon, struct PartyMenuBox *menuBox);
static void CreatePartyMonStatusSprite(struct Pokemon *mon, struct PartyMenuBox *menuBox);
static void CreateCancelConfirmPokeballSprites(void);
static void DrawCancelConfirmButtons(void);
static u8 GetPartyBoxPaletteFlags(u8 slot, u8 animNum);
static void AnimateSelectedPartyIcon(u8 spriteId, u8 animNum);
static void Task_ClosePartyMenuAndSetCB2(u8 taskId);
static void UpdatePartyToFieldOrder(void);
static s8 *GetCurrentPartySlotPtr(void);
static u16 PartyMenuButtonHandler(s8 *slotPtr);
static void HandleChooseMonSelection(u8 taskId, s8 *slotPtr);
static void HandleChooseMonCancel(u8 taskId, s8 *slotPtr);
static void TryTutorSelectedMon(u8 taskId);
static void TryGiveMailToSelectedMon(u8 taskId);
static void SwitchSelectedMons(u8 taskId);
static void TryEnterMonForMinigame(u8 taskId, u8 slot);
static void TryGiveItemOrMailToSelectedMon(u8 taskId);
static void PartyMenuRemoveWindow(u8 *ptr);
static bool8 IsMonAllowedInMinigame(u8 slot);
static void CB2_SetUpExitToBattleScreen(void);
static void Task_ClosePartyMenuAfterText(u8 taskId);
static void FinishTwoMonAction(u8 taskId);
static void CancelParticipationPrompt(u8 taskId);
static void DisplayCancelChooseMonYesNo(u8 taskId);
static void Task_CancelChooseMonYesNo(u8 taskId);
static void Task_HandleCancelChooseMonYesNoInput(u8 taskId);
static void Task_ReturnToChooseMonAfterText(u8 taskId);
static void UpdateCurrentPartySelection(s8 *slotPtr, s8 movementDir);
static void UpdatePartySelectionSingleLayout(s8 *slotPtr, s8 movementDir);
static void UpdatePartySelectionDoubleLayout(s8 *slotPtr, s8 movementDir);
static void Task_PrintAndWaitForText(u8 taskId);
static void Task_CancelParticipationYesNo(u8 taskId);
static void Task_HandleCancelParticipationYesNoInput(u8 taskId);
static void Task_TryCreateSelectionWindow(u8 taskId);
static void CreateSelectionWindow(void);
static void UpdatePartyMonHPBar(u8 spriteId, struct Pokemon *mon);
static void SpriteCB_UpdatePartyMonIcon(struct Sprite *sprite);
static void SpriteCB_BouncePartyMonIcon(struct Sprite *sprite);
static void SpriteCB_HeldItem(struct Sprite *sprite);
static u8 DestroyPartyMonHeldItemSprite(u8 slot, struct PartyMenuBox *menuBox);
static void UpdatePartyMonIconSpecies(struct PartyMenuBox *menuBox, u16 species);
static void UpdatePartyPokemonAilmentGfxAndLevelCheck(struct Pokemon *mon, u8 slot);
static void SetPartyMonAilmentGfx(struct Pokemon *mon, struct PartyMenuBox *menuBox);
static void UpdatePartyMonAilmentGfx(u8 status, struct PartyMenuBox *menuBox);
static void SetPartyMonFieldSelectionActions(struct Pokemon *mons, u8 slotId);
static void Task_HandleSelectionMenuInput(u8 taskId);
static void UpdatePartyToBattleOrder(void);
static void SetPartyIdAtBattleSlot(u8 slot, u8 setVal);
static void SlidePartyMenuBoxOneStep(u8 taskId);
static void Task_SlideSelectedSlotsOffscreen(u8 taskId);
static void SwitchPartyMon(void);
static u8 GetMonAilment(struct Pokemon *mon);
static u8 GetPartyLayoutFromBattleType(void);
static void Task_SlideSelectedSlotsOnscreen(u8 taskId);
static void CB2_WriteMailToGiveMon(void);
static void Task_SwitchHoldItemsPrompt(u8 taskId);
static void Task_UpdateHeldItemSprite(u8 taskId);
static void Task_UpdateHeldItemSpritesAfterMoveItems(u8 taskId);
static void Task_HandleSwitchItemsYesNoInput(u8 taskId);
static void Task_SwitchItemsYesNo(u8 taskId);
static void Task_WriteMailToGiveMonAfterText(u8 taskId);
static void Task_SendMailToPCYesNo(u8 taskId);
static void Task_HandleSendMailToPCYesNoInput(u8 taskId);
static void Task_LoseMailMessageYesNo(u8 taskId);
static void Task_HandleLoseMailMessageYesNoInput(u8 taskId);
static void DisplayCantUseFlashMessage(void);
static void DisplayFieldMoveExitAreaMessage(u8 taskId);
static void Task_WaitFanfareAndDisplayMoveDeletedMessage(u8 taskId);
static void DisplaySwitchedHeldItemMessage(u16 item, u16 item2, bool8 keepOpen);
static void DisplayAlreadyHoldingItemSwitchMessage(struct Pokemon *mon, u16 item, bool8 keepOpen);
static void DisplayGaveHeldItemMessage(struct Pokemon *mon, u16 item, bool8 keepOpen);
static void DisplayLearnMoveMessage(const u8 *str);
static void DisplayLearnMoveMessageAndClose(u8 taskId, const u8 *str);
static void Task_FieldMoveExitAreaYesNo(u8 taskId);
static void Task_HandleFieldMoveExitAreaYesNoInput(u8 taskId);
static void Task_SetSacredAshCB(u8 taskId);
static void CB2_ReturnToBagMenu(void);
static void Task_DisplayHPRestoredMessage(u8 taskId);
static void Task_LearnedMove(u8 taskId);
static void Task_ReplaceMoveYesNo(u8 taskId);
static void Task_DoLearnedMoveFanfareAfterText(u8 taskId);
static void Task_TryLearningNextMove(u8 taskId);
static void Task_LearnNextMoveOrClosePartyMenu(u8 taskId);
static void Task_HandleReplaceMoveYesNoInput(u8 taskId);
static void StopLearningMovePrompt(u8 taskId);
static void DisplayPartyMenuForgotMoveMessage(u8 taskId);
static void Task_StopLearningMoveYesNo(u8 taskId);
static void Task_HandleStopLearningMoveYesNoInput(u8 taskId);
static void Task_DisplayLevelUpStatsPg1(u8 taskId);
static void Task_DisplayLevelUpStatsPg2(u8 taskId);
static void UpdateMonDisplayInfoAfterLevelUp(u8 slot, struct Pokemon *mon);
static void Task_TryLearnNewMoves(u8 taskId);
static void PartyMenuTryEvolution(u8 taskId);
static void DisplayMonNeedsToReplaceMove(u8 taskId);
static void DisplayMonLearnedMove(u8 taskId, u16 move);
static void BufferBagFullCantTakeItemMessage(u16 itemId);
static void Task_SacredAshDisplayHPRestored(u8 taskId);
static void Task_SacredAshLoop(u8 taskId);
static void UseSacredAsh(u8 taskId);
static void CB2_ReturnToBerryPouchMenu(void);
static void CB2_ReturnToTMCaseMenu(void);
static void RemoveItemToGiveFromBag(u16 item);
static void DisplayItemMustBeRemovedFirstMessage(u8 taskId);
static void CB2_WriteMailToGiveMonFromBag(void);
static void Task_UpdateHeldItemSpriteAndClosePartyMenu(u8 taskId);
static void Task_SwitchItemsFromBagYesNo(u8 taskId);
static void GiveItemToBagOrPC(u16 item);
static void Task_DisplayGaveMailFromBagMessage(u8 taskId);
static void Task_HandleSwitchItemsFromBagYesNoInput(u8 taskId);
static void Task_ValidateChosenHalfParty(u8 taskId);
static void Task_ContinueChoosingHalfParty(u8 taskId);
static void Task_InitMultiPartnerPartySlideIn(u8 taskId);
static void Task_WaitAfterMultiPartnerPartySlideIn(u8 taskId);
static void SlideMultiPartyMenuBoxSpritesOneStep(u8 taskId);
static void Task_MultiPartnerPartySlideIn(u8 taskId);
static void Task_FirstBattleEnterParty_WaitFadeIn(u8 taskId);
static void Task_FirstBattleEnterParty_DarkenScreen(u8 taskId);
static void Task_FirstBattleEnterParty_WaitDarken(u8 taskId);
static void Task_FirstBattleEnterParty_CreatePrinter(u8 taskId);
static void Task_FirstBattleEnterParty_RunPrinterMsg1(u8 taskId);
static void Task_FirstBattleEnterParty_LightenFirstMonIcon(u8 taskId);
static void Task_FirstBattleEnterParty_WaitLightenFirstMonIcon(u8 taskId);
static void Task_FirstBattleEnterParty_StartPrintMsg2(u8 taskId);
static void Task_FirstBattleEnterParty_RunPrinterMsg2(u8 taskId);
static void Task_FirstBattleEnterParty_FadeNormal(u8 taskId);
static void Task_FirstBattleEnterParty_WaitFadeNormal(u8 taskId);
static void Task_PartyMenu_Pokedude(u8 taskId);
static void Task_PartyMenu_PokedudeStep(u8 taskId);
static void Task_PartyMenuFromBag_Pokedude(u8 taskId);
static void Task_PartyMenuFromBag_PokedudeStep(u8 taskId);
static void CB2_UseMedicineItem(void);
static void CB2_UseTMItem(void);
static void CB2_UseTMItemAfterForgetingMove(void);
static void CB2_UseEvolutionItem(void);
static bool8 ShouldUseMedicineItemAgain(void);
static void SetMedicineItemFunc(u8 taskId);

static EWRAM_DATA struct PartyMenuInternal *sPartyMenuInternal = NULL;
EWRAM_DATA struct PartyMenu gPartyMenu = {0};
static EWRAM_DATA struct PartyMenuBox *sPartyMenuBoxes = NULL;
static EWRAM_DATA u8 *sPartyBgGfxTilemap = NULL;
static EWRAM_DATA u8 *sPartyBgTilemapBuffer = NULL;
EWRAM_DATA bool8 gPartyMenuUseExitCallback = FALSE;
EWRAM_DATA u8 gSelectedMonPartyId = 0;
EWRAM_DATA MainCallback gPostMenuFieldCallback = NULL;
static EWRAM_DATA u16 *sSlot1TilemapBuffer = NULL; // for switching party slots
static EWRAM_DATA u16 *sSlot2TilemapBuffer = NULL;
EWRAM_DATA u8 gSelectedOrderFromParty[3] = {0};
static EWRAM_DATA u16 sPartyMenuItemId = ITEM_NONE;
ALIGNED(4) EWRAM_DATA u8 gBattlePartyCurrentOrder[PARTY_SIZE / 2] = {0}; // bits 0-3 are the current pos of Slot 1, 4-7 are Slot 2, and so on
static EWRAM_DATA struct MedicineItemData sMedicineItemData = {0};

void (*gItemUseCB)(u8, TaskFunc);

#include "data/pokemon/learnsets/tutor_learnsets.h"
#include "data/party_menu.h"

static const u8 *const sItemUseStrings[] =
{
	[ITEMUSE_STRING_PP_RESTORED - 1] = gText_PPWasRestored,
	[ITEMUSE_STRING_POISON_CURED - 1] = gText_PkmnCuredOfPoison,
	[ITEMUSE_STRING_BURN_HEALED - 1] = gText_PkmnBurnHealed,
	[ITEMUSE_STRING_THAWED - 1] = gText_PkmnThawedOut,
	[ITEMUSE_STRING_WOKE_UP - 1] = gText_PkmnWokeUp2,
	[ITEMUSE_STRING_SNAPPED_CONFUSION - 1] = gText_PkmnSnappedOutOfConfusion,
	[ITEMUSE_STRING_OVER_INFATUATION - 1] = gText_PkmnGotOverInfatuation,
	[ITEMUSE_STRING_BECAME_HEALTHY - 1] = gText_PkmnBecameHealthy,
	[ITEMUSE_STRING_RAISE_DYNAMAX_LEVEL - 1] = gText_PkmnDynamaxLevelRaised,
	[ITEMUSE_STRING_STAT_CHANGED - 1] = gText_PkmnBaseVar2StatVar3,
	[ITEMUSE_STRING_PARALYSIS_CURED - 1] = gText_PkmnCuredOfParalysis,
	[ITEMUSE_STRING_LEVELED_UP - 1] = gText_PkmnElevatedToLvVar2,
	[ITEMUSE_STRING_GAINED_EXP - 1] = gText_PkmnGainedVar3ExpPoints,
	[ITEMUSE_STRING_GAINED_EXP_LEVELED_UP - 1] = gText_PkmnGainedVar3ExpPointsRaisedToLvVar2,
	[ITEMUSE_STRING_PP_INCREASED - 1] = gText_MovesPPIncreased,
	[ITEMUSE_STRING_CHANGE_GMAX_FACTOR - 1] = gText_PkmnVar2TheGMaxFactor,
	[ITEMUSE_STRING_CHANGED_TERA_TYPE - 1] = gText_PkmnTeraTypeBecameVar2,
	[ITEMUSE_STRING_FRIENDSHIP_CHANGED - 1] = gText_PkmnFriendshipWasVar2,
};

static void (*const sItemUseAnimFollowUpCallbacks[])(void) =
{
	[ITEMUSE_FOLLOWUP_MEDICINE] = CB2_UseMedicineItem,
	[ITEMUSE_FOLLOWUP_TM] = CB2_UseTMItem,
	[ITEMUSE_FOLLOWUP_EVOLUTION_ITEM] = CB2_UseEvolutionItem,
};

static const u8 *const sEvItemUseStatNames[] =
{
	[STAT_HP] = gText_HP3,
	[STAT_ATK] = gText_Attack3,
	[STAT_DEF] = gText_Defense3,
	[STAT_SPEED] = gText_Speed2,
	[STAT_SPATK] = gText_SpAtk3,
	[STAT_SPDEF] = gText_SpDef3,
};

static const struct OamData sOamData_ItemIcon =
{
	.y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AffineAnimCmd sAffineAnim_ItemIcon[] =
{
	AFFINEANIMCMD_FRAME(128, 128, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sSpriteAffineAnim_HeldItem[] =
{
	sAffineAnim_ItemIcon,
};

static const struct SpriteTemplate sSpriteTemplate_ItemIcon =
{
	0xD750,
	0xD750,
	&sOamData_ItemIcon,
	gDummySpriteAnimTable,
	NULL,
	sSpriteAffineAnim_HeldItem,
	SpriteCallbackDummy,
};

// Extra args are move to replace 1 and 2
#define FORM_MOVE(_species, _moveToLearn, ...)             \
    {                                                      \
		.species = SPECIES_##_species,                     \
		.moveToLearn = _moveToLearn,                       \
		.replaceMove1 = DEFAULT(MOVE_NONE, __VA_ARGS__),   \
		.replaceMove2 = DEFAULT_2(MOVE_NONE, __VA_ARGS__), \
	}

// Species that learn/forgot moves upon change form
static const struct FormChangeMove sFormChangeMoves[] =
{
	// Pikachu
	FORM_MOVE(PIKACHU_COSPLAY, MOVE_THUNDER_SHOCK),
	FORM_MOVE(PIKACHU_ROCK_STAR, MOVE_METEOR_MASH),
	FORM_MOVE(PIKACHU_BELLE, MOVE_ICICLE_CRASH),
	FORM_MOVE(PIKACHU_POP_STAR, MOVE_DRAINING_KISS),
	FORM_MOVE(PIKACHU_PH_D, MOVE_ELECTRIC_TERRAIN),
	FORM_MOVE(PIKACHU_LIBRE, MOVE_FLYING_PRESS),
	// Rotom
	FORM_MOVE(ROTOM, MOVE_THUNDER_SHOCK),
	FORM_MOVE(ROTOM_HEAT, MOVE_OVERHEAT),
	FORM_MOVE(ROTOM_WASH, MOVE_HYDRO_PUMP),
	FORM_MOVE(ROTOM_FROST, MOVE_BLIZZARD),
	FORM_MOVE(ROTOM_FAN, MOVE_AIR_SLASH),
	FORM_MOVE(ROTOM_MOW, MOVE_LEAF_STORM),
	// Kyurem
	FORM_MOVE(KYUREM, MOVE_GLACIATE, MOVE_FUSION_BOLT, MOVE_FUSION_FLARE),
	FORM_MOVE(KYUREM_WHITE, MOVE_FUSION_FLARE, MOVE_GLACIATE),
	FORM_MOVE(KYUREM_BLACK, MOVE_FUSION_BOLT, MOVE_GLACIATE),
	// Necrozma
	FORM_MOVE(NECROZMA, MOVE_NONE, MOVE_SUNSTEEL_STRIKE, MOVE_MOONGEIST_BEAM),
	FORM_MOVE(NECROZMA_DUSK_MANE, MOVE_SUNSTEEL_STRIKE),
	FORM_MOVE(NECROZMA_DAWN_WINGS, MOVE_MOONGEIST_BEAM),
	// Calyrex
	FORM_MOVE(CALYREX, MOVE_NONE, MOVE_GLACIAL_LANCE, MOVE_ASTRAL_BARRAGE),
	FORM_MOVE(CALYREX_ICE_RIDER, MOVE_GLACIAL_LANCE),
	FORM_MOVE(CALYREX_SHADOW_RIDER, MOVE_ASTRAL_BARRAGE),
};

/////////////////////
// PARTY MENU INIT //
/////////////////////

void InitPartyMenu(u8 menuType, u8 layout, u8 partyAction, bool8 keepCursorPos, u8 messageId, TaskFunc task, MainCallback callback)
{
    u16 i;

    ResetPartyMenu();
	
    sPartyMenuInternal = Alloc(sizeof(struct PartyMenuInternal));
	
    if (sPartyMenuInternal == NULL)
        SetMainCallback2(callback);
    else
    {
        gPartyMenu.menuType = menuType;
        gPartyMenu.exitCallback = callback;
        gPartyMenu.action = partyAction;
        sPartyMenuInternal->messageId = messageId;
        sPartyMenuInternal->task = task;
        sPartyMenuInternal->exitCallback = NULL;
        sPartyMenuInternal->lastSelectedSlot = 0;
		
        if (menuType == PARTY_MENU_TYPE_CHOOSE_HALF)
            sPartyMenuInternal->chooseHalf = TRUE;
        else
            sPartyMenuInternal->chooseHalf = FALSE;
		
        if (layout != KEEP_PARTY_LAYOUT)
            gPartyMenu.layout = layout;
		
        for (i = 0; i < ARRAY_COUNT(sPartyMenuInternal->data); ++i)
            sPartyMenuInternal->data[i] = 0;
		
        for (i = 0; i < ARRAY_COUNT(sPartyMenuInternal->windowId); ++i)
            sPartyMenuInternal->windowId[i] = 0xFF;
		
        if (!keepCursorPos)
            gPartyMenu.slotId = 0;
        else if (gPartyMenu.slotId > PARTY_SIZE - 1 || !GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_SPECIES))
            gPartyMenu.slotId = 0;
		
        gTextFlags.autoScroll = FALSE;
        CalculatePlayerPartyCount();
        SetMainCallback2(CB2_InitPartyMenu);
		gMain.inParty = TRUE;
    }
}

static void CB2_ReturnToPartyMenuWithFollowUpTask(u8 messageId, TaskFunc func)
{
	InitPartyMenu(gPartyMenu.menuType, KEEP_PARTY_LAYOUT, gPartyMenu.action, TRUE, messageId, func, gPartyMenu.exitCallback);
}

void CB2_PartyMenuFromStartMenu(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON, Task_HandleChooseMonInput, CB2_ReturnToFieldWithOpenMenu);
}

// Not used, uses Charizard glide instead
void CB2_ReturnToPartyMenuFromFlyMap(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, TRUE, PARTY_MSG_CHOOSE_MON, Task_HandleChooseMonInput, CB2_ReturnToFieldWithOpenMenu);
}

void CB2_ReturnToPartyMenuFromSummaryScreen(void)
{
    gPaletteFade.bufferTransferDisabled = TRUE;
    gPartyMenu.slotId = GetLastViewedMonIndex();
	CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_DO_WHAT_WITH_MON, Task_TryCreateSelectionWindow);
}

static void Task_DisplayMoveDeleted(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		PlayFanfare(MUS_MOVE_DELETED);
		gTasks[taskId].func = Task_WaitFanfareAndDisplayMoveDeletedMessage;
	}
}

static void CB2_ReturnToPartyMenuFromMoveDelete(void)
{
	CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_NONE, gSpecialVar_0x8005 == MAX_MON_MOVES ? Task_TryCreateSelectionWindow : Task_DisplayMoveDeleted);
}

static void CB2_ReturnToPartyMenuFromReadingMail(void)
{
    gPaletteFade.bufferTransferDisabled = TRUE;
	CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_DO_WHAT_WITH_MON, Task_TryCreateSelectionWindow);
}

static void GiveItemToBagOrPC(u16 item)
{
    if (gPartyMenu.action == PARTY_ACTION_GIVE_ITEM) 
        AddBagItem(item, 1);
    else
        AddPCItem(item, 1);
}

static void CB2_ReturnToPartyOrBagMenuFromWritingMail(void)
{
    struct Pokemon *mon;
    u16 item;

    if (!gSpecialVar_Result) // Canceled writing mail
    {
		mon = &gPlayerParty[gPartyMenu.slotId];
		item = GetMonData(mon, MON_DATA_HELD_ITEM);
		
        TakeMailFromMon(mon);
        SetMonData(mon, MON_DATA_HELD_ITEM, &sPartyMenuItemId);
        RemoveBagItem(sPartyMenuItemId, 1);
        GiveItemToBagOrPC(item);
        SetMainCallback2(gPartyMenu.exitCallback);
    }
    else // Wrote mail
		CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_NONE, Task_DisplayGaveMailFromBagMessage);
}

void ChooseMonToGiveMailFromMailbox(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_GIVE_MAILBOX_MAIL, FALSE, PARTY_MSG_GIVE_TO_WHICH_MON, Task_HandleChooseMonInput, Mailbox_ReturnToMailListAfterDeposit);
}

void InitChooseHalfPartyForBattle(u8 battleType)
{
    ClearSelectedPartyOrder();
    InitPartyMenu(PARTY_MENU_TYPE_CHOOSE_HALF, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON, Task_HandleChooseMonInput, gMain.savedCallback);
    gPartyMenu.chooseMonsBattleType = battleType;
    gPartyMenu.task = Task_ValidateChosenHalfParty;
}

void ChooseMonForTradingBoard(u8 menuType, MainCallback callback)
{
    InitPartyMenu(menuType, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON, Task_HandleChooseMonInput, callback);
}

void ChooseMonForMoveTutor(void)
{
	InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_MOVE_TUTOR, FALSE, PARTY_MSG_TEACH_WHICH_MON, Task_HandleChooseMonInput, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void ChooseMonForWirelessMinigame(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_MINIGAME, PARTY_LAYOUT_SINGLE, PARTY_ACTION_MINIGAME, FALSE, PARTY_MSG_CHOOSE_MON_OR_CANCEL, Task_HandleChooseMonInput, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void OpenPartyMenuInTutorialBattle(u8 partyAction)
{
	u8 layout = GetPartyLayoutFromBattleType();
	
    if (!BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_PARTY_MENU) && (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE))
    {
		BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_PARTY_MENU);
		InitPartyMenu(PARTY_MENU_TYPE_IN_BATTLE, layout, partyAction, FALSE, PARTY_MSG_NONE, Task_FirstBattleEnterParty_WaitFadeIn, ReshowBattleScreenAfterMenu);
    }
    else
		InitPartyMenu(PARTY_MENU_TYPE_IN_BATTLE, layout, partyAction, FALSE, PARTY_MSG_CHOOSE_MON, Task_HandleChooseMonInput, ReshowBattleScreenAfterMenu);
	
    UpdatePartyToBattleOrder();
}

void EnterPartyFromItemMenuInBattle(void)
{
	u8 layout = GetPartyLayoutFromBattleType();
	
    if (!BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_PARTY_MENU) && (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE))
    {
		BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_PARTY_MENU);
		InitPartyMenu(PARTY_MENU_TYPE_IN_BATTLE, layout, PARTY_ACTION_USE_ITEM, FALSE, PARTY_MSG_NONE, Task_FirstBattleEnterParty_WaitFadeIn, CB2_BagMenuFromBattle);
    }
    else
    {
		InitPartyMenu(PARTY_MENU_TYPE_IN_BATTLE, layout, PARTY_ACTION_USE_ITEM, FALSE, PARTY_MSG_USE_ON_WHICH_MON, Task_HandleChooseMonInput,
		(ItemId_GetPocket(gSpecialVar_ItemId) == POCKET_BERRY_POUCH) ? CB2_ReturnToBerryPouchMenu : CB2_BagMenuFromBattle);
    }
    UpdatePartyToBattleOrder();
}

void Pokedude_OpenPartyMenuInBattle(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_IN_BATTLE, GetPartyLayoutFromBattleType(), PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON, Task_PartyMenu_Pokedude, ReshowBattleScreenAfterMenu);
    UpdatePartyToBattleOrder();
}

void Pokedude_ChooseMonForInBattleItem(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_IN_BATTLE, GetPartyLayoutFromBattleType(), PARTY_ACTION_REUSABLE_ITEM, FALSE, PARTY_MSG_USE_ON_WHICH_MON, Task_PartyMenuFromBag_Pokedude, CB2_BagMenuFromBattle);
    UpdatePartyToBattleOrder();
}

void ShowPartyMenuToShowcaseMultiBattleParty(void)
{
    InitPartyMenu(PARTY_MENU_TYPE_MULTI_SHOWCASE, PARTY_LAYOUT_MULTI_SHOWCASE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_NONE, Task_InitMultiPartnerPartySlideIn, gMain.savedCallback);
}

static void Task_PartyMenuWaitForFade(u8 taskId)
{
    if (IsWeatherNotFadingIn())
    {
        DestroyTask(taskId);
        ScriptContext2_Disable();
        EnableBothScriptContexts();
    }
}

static bool8 CB2_FadeFromPartyMenu(void)
{
    FadeInFromBlack();
    CreateTask(Task_PartyMenuWaitForFade, 10);
    return TRUE;
}

void ChooseMonForDaycare(void)
{
    gFieldCallback2 = CB2_FadeFromPartyMenu;
    InitPartyMenu(PARTY_MENU_TYPE_DAYCARE, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, FALSE, PARTY_MSG_CHOOSE_MON_2, Task_HandleChooseMonInput, CB2_ReturnToField);
}

static void Task_WaitFadeAndInitChoosePartyPokemon(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gPaletteFade.bufferTransferDisabled = TRUE;
		gFieldCallback2 = CB2_FadeFromPartyMenu;
		InitPartyMenu(gTasks[taskId].data[0], PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_AND_CLOSE, FALSE, PARTY_MSG_CHOOSE_MON, Task_HandleChooseMonInput, CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

void ChoosePartyMon(void)
{
    ScriptContext2_Enable();
    gTasks[CreateTask(Task_WaitFadeAndInitChoosePartyPokemon, 10)].data[0] = PARTY_MENU_TYPE_CHOOSE_MON;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
}

void SelectMoveTutorMon(void)
{
    ScriptContext2_Enable();
    gTasks[CreateTask(Task_WaitFadeAndInitChoosePartyPokemon, 10)].data[0] = PARTY_MENU_TYPE_MOVE_RELEARNER;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
}

static void CB2_UpdatePartyMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_PartyMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_InitPartyMenu(void)
{
    while (TRUE)
    {
        if (sub_80BF748() || ShowPartyMenu() || MenuHelpers_LinkSomething())
            break;
    }
}

static bool8 ShowPartyMenu(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ResetVramOamAndBgCntRegs();
        ClearScheduledBgCopiesToVram();
        ++gMain.state;
        break;
    case 1:
        ScanlineEffect_Stop();
        ++gMain.state;
        break;
    case 2:
        ResetPaletteFade();
        gPaletteFade.bufferTransferDisabled = TRUE;
        ++gMain.state;
        break;
    case 3:
        ResetSpriteData();
        ++gMain.state;
        break;
    case 4:
        FreeAllSpritePalettes();
        ++gMain.state;
        break;
    case 5:
        if (!MenuHelpers_LinkSomething())
            ResetTasks();
        ++gMain.state;
        break;
    case 6:
        SetPartyMonsAllowedInMinigame();
        ++gMain.state;
        break;
    case 7:
        if (!AllocPartyMenuBg())
        {
            ExitPartyMenu();
            return TRUE;
        }
        else
        {
            sPartyMenuInternal->data[0] = 0;
            ++gMain.state;
        }
        break;
    case 8:
        if (AllocPartyMenuBgGfx())
            ++gMain.state;
        break;
    case 9:
        InitPartyMenuWindows(gPartyMenu.layout);
        ++gMain.state;
        break;
    case 10:
        InitPartyMenuBoxes(gPartyMenu.layout);
        sPartyMenuInternal->data[0] = 0;
        ++gMain.state;
        break;
    case 11:
        ++gMain.state;
        break;
    case 12:
        LoadPartyMenuPokeballGfx();
        ++gMain.state;
        break;
    case 13:
        LoadPartyMenuAilmentGfx();
        ++gMain.state;
        break;
    case 14:
        LoadMonIconPalettes();
        ++gMain.state;
        break;
    case 15:
        if (CreatePartyMonSpritesLoop())
        {
            sPartyMenuInternal->data[0] = 0;
            ++gMain.state;
        }
        break;
    case 16:
        if (RenderPartyMenuBoxes())
        {
            sPartyMenuInternal->data[0] = 0;
            ++gMain.state;
        }
        break;
    case 17:
        CreateCancelConfirmPokeballSprites();
        ++gMain.state;
        break;
    case 18:
        CreateCancelConfirmWindows(sPartyMenuInternal->chooseHalf);
        ++gMain.state;
        break;
    case 19:
        CreateTask(sPartyMenuInternal->task, 0);
        DisplayPartyMenuStdMessage(sPartyMenuInternal->messageId);
        ++gMain.state;
        break;
    case 20:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        ++gMain.state;
        break;
    case 21:
        BeginNormalPaletteFade(PALETTES_ALL, -2, 16, 0, RGB_BLACK);
        gPaletteFade.bufferTransferDisabled = FALSE;
        ++gMain.state;
        break;
    default:
        SetVBlankCallback(VBlankCB_PartyMenu);
        SetMainCallback2(CB2_UpdatePartyMenu);
        return TRUE;
    }
    return FALSE;
}

static void ExitPartyMenu(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    CreateTask(Task_ExitPartyMenu, 0);
    SetVBlankCallback(VBlankCB_PartyMenu);
    SetMainCallback2(CB2_UpdatePartyMenu);
}

static void Task_ExitPartyMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(gPartyMenu.exitCallback);
        FreePartyPointers();
        DestroyTask(taskId);
    }
}

static void Task_ClosePartyMenu(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ClosePartyMenuAndSetCB2;
}

static void Task_ClosePartyMenuAndSetCB2(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (gPartyMenu.menuType == PARTY_MENU_TYPE_IN_BATTLE)
            UpdatePartyToFieldOrder();
		
        if (sPartyMenuInternal->exitCallback != NULL)
            SetMainCallback2(sPartyMenuInternal->exitCallback);
        else
            SetMainCallback2(gPartyMenu.exitCallback);
		
        FreePartyPointers();
        DestroyTask(taskId);
		gMain.inParty = FALSE;
    }
}

static void Task_ClosePartyMenuAfterText(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        if (gPartyMenuUseExitCallback == FALSE)
            sPartyMenuInternal->exitCallback = NULL;
		
        Task_ClosePartyMenu(taskId);
    }
}

static void ResetPartyMenu(void)
{
    sPartyMenuInternal = NULL;
    sPartyBgTilemapBuffer = NULL;
    sPartyMenuBoxes = NULL;
    sPartyBgGfxTilemap = NULL;
}

static bool8 AllocPartyMenuBg(void)
{
    ResetAllBgsCoordinatesAndBgCntRegs();
	
    sPartyBgTilemapBuffer = Alloc(0x800);
    if (sPartyBgTilemapBuffer == NULL)
        return FALSE;
	
    memset(sPartyBgTilemapBuffer, 0, 0x800);
	
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sPartyMenuBgTemplates, ARRAY_COUNT(sPartyMenuBgTemplates));
    SetBgTilemapBuffer(1, sPartyBgTilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
	
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
	
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
	
    return TRUE;
}

static bool8 AllocPartyMenuBgGfx(void)
{
    u32 sizeout;

    switch (sPartyMenuInternal->data[0])
    {
    case 0:
        sPartyBgGfxTilemap = MallocAndDecompress(gPartyMenuBg_Gfx, &sizeout);
        LoadBgTiles(1, sPartyBgGfxTilemap, sizeout, 0);
        ++sPartyMenuInternal->data[0];
        break;
    case 1:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            LZDecompressWram(gPartyMenuBg_Tilemap, sPartyBgTilemapBuffer);
            ++sPartyMenuInternal->data[0];
        }
        break;
    case 2:
        LoadCompressedPalette(gPartyMenuBg_Pal, 0, 0x160);
        CpuCopy16(gPlttBufferUnfaded, sPartyMenuInternal->palBuffer, 0x160);
        ++sPartyMenuInternal->data[0];
        break;
    case 3:
        PartyPaletteBufferCopy(4);
        ++sPartyMenuInternal->data[0];
        break;
    case 4:
        PartyPaletteBufferCopy(5);
        ++sPartyMenuInternal->data[0];
        break;
    case 5:
        PartyPaletteBufferCopy(6);
        ++sPartyMenuInternal->data[0];
        break;
    case 6:
        PartyPaletteBufferCopy(7);
        ++sPartyMenuInternal->data[0];
        break;
    case 7:
        PartyPaletteBufferCopy(8);
        ++sPartyMenuInternal->data[0];
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

static void FreePartyPointers(void)
{
    if (sPartyMenuInternal)
        Free(sPartyMenuInternal);
	
    if (sPartyBgTilemapBuffer)
        Free(sPartyBgTilemapBuffer);
	
    if (sPartyBgGfxTilemap)
        Free(sPartyBgGfxTilemap);
	
    if (sPartyMenuBoxes)
        Free(sPartyMenuBoxes);
	
    FreeAllWindowBuffers();
}

static void InitPartyMenuBoxes(u8 layout)
{
    u8 i;

    sPartyMenuBoxes = Alloc(sizeof(struct PartyMenuBox[PARTY_SIZE]));
	
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        sPartyMenuBoxes[i].infoRects = &sPartyBoxInfoRects[PARTY_BOX_RIGHT_COLUMN];
        sPartyMenuBoxes[i].spriteCoords = sPartyMenuSpriteCoords[layout][i];
        sPartyMenuBoxes[i].windowId = i;
    }
    // The first party mon goes in the left column
    sPartyMenuBoxes[0].infoRects = &sPartyBoxInfoRects[PARTY_BOX_LEFT_COLUMN];
	
    if (layout == PARTY_LAYOUT_MULTI_SHOWCASE)
        sPartyMenuBoxes[3].infoRects = &sPartyBoxInfoRects[PARTY_BOX_LEFT_COLUMN];
    else if (layout != PARTY_LAYOUT_SINGLE)
        sPartyMenuBoxes[1].infoRects = &sPartyBoxInfoRects[PARTY_BOX_LEFT_COLUMN];
}

static void RenderPartyMenuBox(u8 slot)
{
    if (gPartyMenu.menuType == PARTY_MENU_TYPE_MULTI_SHOWCASE && slot >= MULTI_PARTY_SIZE)
    {
        DisplayPartyPokemonDataForMultiBattle(slot);
        LoadPartyBoxPalette(&sPartyMenuBoxes[slot], PARTY_PAL_MULTI_ALT);
        CopyWindowToVram(sPartyMenuBoxes[slot].windowId, COPYWIN_GFX);
        PutWindowTilemap(sPartyMenuBoxes[slot].windowId);
        ScheduleBgCopyTilemapToVram(2);
    }
    else
    {
        if (!GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES))
        {
            DrawEmptySlot(sPartyMenuBoxes[slot].windowId);
            CopyWindowToVram(sPartyMenuBoxes[slot].windowId, COPYWIN_GFX);
        }
        else
        {
			switch (gPartyMenu.menuType)
			{
				case PARTY_MENU_TYPE_CHOOSE_HALF:
				    DisplayPartyPokemonDataForChooseHalf(slot);
					break;
				case PARTY_MENU_TYPE_MINIGAME:
				    DisplayPartyPokemonDataForWirelessMinigame(slot);
					break;
				case PARTY_ACTION_MOVE_TUTOR:
				    DisplayPartyPokemonDataForMoveTutor(slot);
					break;
				default:
				    if (!DisplayPartyPokemonDataForTeachMoveOrEvolutionItem(slot))
						DisplayPartyPokemonData(slot);
					break;
			}

            if (gPartyMenu.menuType == PARTY_MENU_TYPE_MULTI_SHOWCASE)
                AnimatePartySlot(slot, 0);
            else if (gPartyMenu.slotId == slot)
                AnimatePartySlot(slot, 1);
            else
                AnimatePartySlot(slot, 0);
        }
        PutWindowTilemap(sPartyMenuBoxes[slot].windowId);
        ScheduleBgCopyTilemapToVram(0);
    }
}

static bool8 RenderPartyMenuBoxes(void)
{
    RenderPartyMenuBox(sPartyMenuInternal->data[0]);
	
    if (++sPartyMenuInternal->data[0] == PARTY_SIZE)
        return TRUE;
    else
        return FALSE;
}

static void InitPartyMenuWindows(u8 layout)
{
    u8 i;

    switch (layout)
    {
    case PARTY_LAYOUT_SINGLE:
        InitWindows(sSinglePartyMenuWindowTemplate);
        break;
    case PARTY_LAYOUT_DOUBLE:
        InitWindows(sDoublePartyMenuWindowTemplate);
        break;
    case PARTY_LAYOUT_MULTI:
        InitWindows(sMultiPartyMenuWindowTemplate);
        break;
    default:
        InitWindows(sShowcaseMultiPartyMenuWindowTemplate);
        break;
    }
    DeactivateAllTextPrinters();
	
    for (i = 0; i < PARTY_SIZE; ++i)
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
	
    TextWindow_SetUserSelectedFrame(0, 0x4F, 0xD0);
    TextWindow_SetStdFrame0_WithPal(0, 0x58, 0xF0);
    LoadPalette(stdpal_get(2), 0xC0, 0x20);
    LoadPalette(stdpal_get(0), 0xE0, 0x20);
}

static void PartyMenuRemoveWindow(u8 *ptr)
{
    if (*ptr != 0xFF)
    {
        ClearStdWindowAndFrameToTransparent(*ptr, 0);
        RemoveWindow(*ptr);
        *ptr = 0xFF;
        ScheduleBgCopyTilemapToVram(2);
    }
}

static void CreateCancelConfirmWindows(bool8 chooseHalf)
{
    u8 confirmWindowId, cancelWindowId, offset;

    if (gPartyMenu.menuType != PARTY_MENU_TYPE_MULTI_SHOWCASE)
    {
        if (chooseHalf)
        {
            confirmWindowId = AddWindow(&sConfirmButtonWindowTemplate);
            FillWindowPixelBuffer(confirmWindowId, PIXEL_FILL(0));
            AddTextPrinterParameterized4(confirmWindowId, 0, (48 - GetStringWidth(0, gMenuText_Confirm, 0)) / 2u, 1, 0, 0, sFontColorTable[0], -1, gMenuText_Confirm);
            PutWindowTilemap(confirmWindowId);
            CopyWindowToVram(confirmWindowId, COPYWIN_GFX);
            cancelWindowId = AddWindow(&sMultiCancelButtonWindowTemplate);
            offset = 0;
        }
        else
        {
            cancelWindowId = AddWindow(&sCancelButtonWindowTemplate);
            offset = 3;
        }
        FillWindowPixelBuffer(cancelWindowId, PIXEL_FILL(0));
		
        offset += (48 - GetStringWidth(0, gFameCheckerText_Cancel, 0)) / 2;
        AddTextPrinterParameterized3(cancelWindowId, 0, offset, 1, sFontColorTable[0], -1, gFameCheckerText_Cancel);
        PutWindowTilemap(cancelWindowId);
        CopyWindowToVram(cancelWindowId, COPYWIN_GFX);
        ScheduleBgCopyTilemapToVram(0);
    }
}

static bool8 IsMonAllowedInPokemonJump(struct Pokemon *mon)
{
    if (!GetMonData(mon, MON_DATA_IS_EGG) && IsSpeciesAllowedInPokemonJump(GetMonData(mon, MON_DATA_SPECIES)))
        return TRUE;
    return FALSE;
}

static bool8 IsMonAllowedInDodrioBerryPicking(struct Pokemon *mon)
{
    if (!GetMonData(mon, MON_DATA_IS_EGG) && GetMonData(mon, MON_DATA_SPECIES) == SPECIES_DODRIO)
        return TRUE;
    return FALSE;
}

static void SetPartyMonsAllowedInMinigame(void)
{
    u16 *ptr;

    if (gPartyMenu.menuType == PARTY_MENU_TYPE_MINIGAME)
    {
        u8 i;

        ptr = &gPartyMenu.data1;
        gPartyMenu.data1 = 0;
		
        if (gSpecialVar_0x8005 == 0)
        {
            for (i = 0; i < gPlayerPartyCount; ++i)
                *ptr += IsMonAllowedInPokemonJump(&gPlayerParty[i]) << i;
        }
        else
        {
            for (i = 0; i < gPlayerPartyCount; ++i)
                *ptr += IsMonAllowedInDodrioBerryPicking(&gPlayerParty[i]) << i;
        }
    }
}

////////////////////
// BLIT FUNCTIONS //
////////////////////

static inline u8 *GetPartyMenuBgTile(u16 tileId)
{
    return &sPartyBgGfxTilemap[tileId << 5];
}

static void BlitBitmapToPartyWindow(u8 windowId, const u8 *b, u8 c, u8 x, u8 y, u8 width, u8 height)
{
    u8 *pixels = AllocZeroed(height * width * 32);
    u8 i, j;

    if (pixels != NULL)
    {
        for (i = 0; i < height; ++i)
		{
            for (j = 0; j < width; ++j)
                CpuCopy16(GetPartyMenuBgTile(b[x + j + ((y + i) * c)]), &pixels[(i * width + j) * 32], 32);
		}
        BlitBitmapToWindow(windowId, pixels, x * 8, y * 8, width * 8, height * 8);
        Free(pixels);
    }
}

static void BlitBitmapToPartyWindow_LeftColumn(u8 windowId, u8 x, u8 y, u8 width, u8 height, bool8 isEgg)
{
    if (width == 0 && height == 0)
    {
        width = 10;
        height = 7;
    }
	BlitBitmapToPartyWindow(windowId, isEgg ? sMainSlotTileNums_Egg : sMainSlotTileNums, 10, x, y, width, height);
}

static void BlitBitmapToPartyWindow_RightColumn(u8 windowId, u8 x, u8 y, u8 width, u8 height, bool8 isEgg)
{
    if (width == 0 && height == 0)
    {
        width = 18;
        height = 3;
    }
	BlitBitmapToPartyWindow(windowId, isEgg ? sOtherSlotsTileNums_Egg : sOtherSlotsTileNums, 18, x, y, width, height);
}

static void DrawEmptySlot(u8 windowId)
{
    BlitBitmapToPartyWindow(windowId, sEmptySlotTileNums, 18, 0, 0, 18, 3);
}

/////////////////////
// MESSAGE DISPLAY //
/////////////////////

// Messages like can use item, allowed in minigame, nickname, etc.
static void DisplayPartyPokemonBarDetail(u8 windowId, u8 fontId, const u8 *str, u8 color, const u8 *align)
{
    AddTextPrinterParameterized3(windowId, fontId, align[0], align[1], sFontColorTable[color], 0, str);
}

static void DisplayPartyPokemonNickname(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    if (GetMonData(mon, MON_DATA_SPECIES))
    {
        if (c == 1)
            menuBox->infoRects->blitFunc(menuBox->windowId, menuBox->infoRects->dimensions[0] >> 3, menuBox->infoRects->dimensions[1] >> 3, menuBox->infoRects->dimensions[2] >> 3, menuBox->infoRects->dimensions[3] >> 3, FALSE);
        
		GetMonNickname(mon, nickname);
        DisplayPartyPokemonBarDetail(menuBox->windowId, GetFontIdToFit(nickname, 0, 0, 50), nickname, 0, menuBox->infoRects->dimensions);
    }
}

static void DisplayPartyPokemonLevelCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c)
{
    if (GetMonData(mon, MON_DATA_SPECIES))
    {
        u8 ailment = GetMonAilment(mon);

        if (ailment == AILMENT_NONE || ailment == AILMENT_PKRS)
        {
            if (c != 0)
                menuBox->infoRects->blitFunc(menuBox->windowId, menuBox->infoRects->dimensions[4] >> 3, (menuBox->infoRects->dimensions[5] >> 3) + 1, menuBox->infoRects->dimensions[6] >> 3, menuBox->infoRects->dimensions[7] >> 3, FALSE);
            if (c != 2)
                DisplayPartyPokemonLevel(GetMonData(mon, MON_DATA_LEVEL), menuBox);
        }
    }
}

static void DisplayPartyPokemonLevel(u8 level, struct PartyMenuBox *menuBox)
{
    ConvertIntToDecimalStringN(gStringVar2, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringCopy(gStringVar1, gText_Lv);
    StringAppend(gStringVar1, gStringVar2);
    DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gStringVar1, 0, &menuBox->infoRects->dimensions[4]);
}

static void DisplayPartyPokemonGenderBlitFunc(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    if (c == 1)
        menuBox->infoRects->blitFunc(menuBox->windowId, menuBox->infoRects->dimensions[8] >> 3, (menuBox->infoRects->dimensions[9] >> 3) + 1, menuBox->infoRects->dimensions[10] >> 3, menuBox->infoRects->dimensions[11] >> 3, FALSE);
    
	GetMonNickname(mon, nickname);
    DisplayPartyPokemonGender(GetMonGender(mon), GetMonData(mon, MON_DATA_SPECIES), nickname, menuBox);
}

static void DisplayPartyPokemonGender(u8 gender, u16 species, u8 *nickname, struct PartyMenuBox *menuBox)
{
    u8 palNum;

    if (!species)
        return;
    
	palNum = GetWindowAttribute(menuBox->windowId, WINDOW_PALETTE_NUM) * 16;
	
    switch (gender)
    {
    case MON_MALE:
        LoadPalette(GetPartyMenuPalBufferPtr(sGenderMalePalIds[0]), sGenderPalOffsets[0] + palNum, 2);
        LoadPalette(GetPartyMenuPalBufferPtr(sGenderMalePalIds[1]), sGenderPalOffsets[1] + palNum, 2);
        DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gText_MaleSymbol, 2, &menuBox->infoRects->dimensions[8]);
        break;
    case MON_FEMALE:
        LoadPalette(GetPartyMenuPalBufferPtr(sGenderFemalePalIds[0]), sGenderPalOffsets[0] + palNum, 2);
        LoadPalette(GetPartyMenuPalBufferPtr(sGenderFemalePalIds[1]), sGenderPalOffsets[1] + palNum, 2);
        DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gText_FemaleSymbol, 2, &menuBox->infoRects->dimensions[8]);
        break;
    }
}

static void DisplayPartyPokemonHPCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c)
{
    if (GetMonData(mon, MON_DATA_SPECIES))
    {
        if (c != 0)
            menuBox->infoRects->blitFunc(menuBox->windowId, menuBox->infoRects->dimensions[12] >> 3, (menuBox->infoRects->dimensions[13] >> 3) + 1, menuBox->infoRects->dimensions[14] >> 3, menuBox->infoRects->dimensions[15] >> 3, FALSE);
        if (c != 2)
            DisplayPartyPokemonHP(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP), menuBox);
    }
}

static void DisplayParty4DigitsHP(struct PartyMenuBox *menuBox, const u8 *origAlings, u8 toSub)
{
	u8 newAligns[4];
	
	memcpy(newAligns, origAlings, sizeof(newAligns));
	
	newAligns[0] -= toSub; // x, so that the hp fits
	
	DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gStringVar1, 0, newAligns);
}

static void DisplayPartyPokemonHP(u16 hp, u16 maxHp, struct PartyMenuBox *menuBox)
{
	bool8 fourDigits = (maxHp >= 1000);
    u8 *strOut = ConvertIntToDecimalStringN(gStringVar1, hp, STR_CONV_MODE_RIGHT_ALIGN, fourDigits ? 4 : 3);

    strOut[0] = CHAR_SLASH;
    strOut[1] = EOS;
	
	if (fourDigits)
		DisplayParty4DigitsHP(menuBox, &menuBox->infoRects->dimensions[12], 10);
	else
		DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gStringVar1, 0, &menuBox->infoRects->dimensions[12]);
}

static void DisplayPartyPokemonMaxHPCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 c)
{
    if (GetMonData(mon, MON_DATA_SPECIES))
    {
        if (c != 0)
            menuBox->infoRects->blitFunc(menuBox->windowId, (menuBox->infoRects->dimensions[16] >> 3) + 1, (menuBox->infoRects->dimensions[17] >> 3) + 1, menuBox->infoRects->dimensions[18] >> 3, menuBox->infoRects->dimensions[19] >> 3, FALSE);
        if (c != 2)
            DisplayPartyPokemonMaxHP(GetMonData(mon, MON_DATA_MAX_HP), menuBox);
    }
}

static void DisplayPartyPokemonMaxHP(u16 maxhp, struct PartyMenuBox *menuBox)
{
	bool8 fourDigits = (maxhp >= 1000);
	
    ConvertIntToDecimalStringN(gStringVar2, maxhp, STR_CONV_MODE_RIGHT_ALIGN, fourDigits ? 4 : 3);
    StringCopy(gStringVar1, gText_Slash);
    StringAppend(gStringVar1, gStringVar2);
	
	if (fourDigits)
		DisplayParty4DigitsHP(menuBox, &menuBox->infoRects->dimensions[16], 5);
	else
		DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gStringVar1, 0, &menuBox->infoRects->dimensions[16]);
}

static void DisplayPartyPokemonHPBarCheck(struct Pokemon *mon, struct PartyMenuBox *menuBox)
{
    if (GetMonData(mon, MON_DATA_SPECIES))
        DisplayPartyPokemonHPBar(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP), menuBox);
}

static void DisplayPartyPokemonHPBar(u16 hp, u16 maxhp, struct PartyMenuBox *menuBox)
{
    u8 palNum = GetWindowAttribute(menuBox->windowId, WINDOW_PALETTE_NUM) * 16;
    u8 hpFraction;

    switch (GetHPBarLevel(hp, maxhp))
    {
    case HP_BAR_GREEN:
    case HP_BAR_FULL:
        LoadPalette(GetPartyMenuPalBufferPtr(sHPBarGreenPalIds[0]), sHPBarPalOffsets[0] + palNum, 2);
        LoadPalette(GetPartyMenuPalBufferPtr(sHPBarGreenPalIds[1]), sHPBarPalOffsets[1] + palNum, 2);
        break;
    case HP_BAR_YELLOW:
        LoadPalette(GetPartyMenuPalBufferPtr(sHPBarYellowPalIds[0]), sHPBarPalOffsets[0] + palNum, 2);
        LoadPalette(GetPartyMenuPalBufferPtr(sHPBarYellowPalIds[1]), sHPBarPalOffsets[1] + palNum, 2);
        break;
    default:
        LoadPalette(GetPartyMenuPalBufferPtr(sHPBarRedPalIds[0]), sHPBarPalOffsets[0] + palNum, 2);
        LoadPalette(GetPartyMenuPalBufferPtr(sHPBarRedPalIds[1]), sHPBarPalOffsets[1] + palNum, 2);
        break;
    }
    hpFraction = GetScaledHPFraction(hp, maxhp, menuBox->infoRects->dimensions[22]);
    FillWindowPixelRect(menuBox->windowId, sHPBarPalOffsets[1], menuBox->infoRects->dimensions[20], menuBox->infoRects->dimensions[21], hpFraction, 1);
    FillWindowPixelRect(menuBox->windowId, sHPBarPalOffsets[0], menuBox->infoRects->dimensions[20], menuBox->infoRects->dimensions[21] + 1, hpFraction, 2);
    
	if (hpFraction != menuBox->infoRects->dimensions[22])
    {
        // This appears to be an alternating fill
        FillWindowPixelRect(menuBox->windowId, 0x0D, menuBox->infoRects->dimensions[20] + hpFraction, menuBox->infoRects->dimensions[21], menuBox->infoRects->dimensions[22] - hpFraction, 1);
        FillWindowPixelRect(menuBox->windowId, 0x02, menuBox->infoRects->dimensions[20] + hpFraction, menuBox->infoRects->dimensions[21] + 1, menuBox->infoRects->dimensions[22] - hpFraction, 2);
    }
    CopyWindowToVram(menuBox->windowId, COPYWIN_GFX);
}

static void DisplayPartyPokemonDescriptionText(u8 stringId, struct PartyMenuBox *menuBox, u8 c)
{
    if (c != 0)
        menuBox->infoRects->blitFunc(menuBox->windowId, menuBox->infoRects->descTextLeft >> 3, menuBox->infoRects->descTextTop >> 3, menuBox->infoRects->descTextWidth >> 3, menuBox->infoRects->descTextHeight >> 3, TRUE);
    
	if (c != 2)
        AddTextPrinterParameterized3(menuBox->windowId, 1, menuBox->infoRects->descTextLeft, menuBox->infoRects->descTextTop, sFontColorTable[0], 0, sDescriptionStringTable[stringId]);
}

static void DisplayPartyPokemonData(u8 slot)
{
	bool8 isEgg = GetMonData(&gPlayerParty[slot], MON_DATA_IS_EGG);
	
	sPartyMenuBoxes[slot].infoRects->blitFunc(sPartyMenuBoxes[slot].windowId, 0, 0, 0, 0, isEgg);
	DisplayPartyPokemonNickname(&gPlayerParty[slot], &sPartyMenuBoxes[slot], 0);
	
    if (!isEgg)
    {
        DisplayPartyPokemonLevelCheck(&gPlayerParty[slot], &sPartyMenuBoxes[slot], 0);
        DisplayPartyPokemonGenderBlitFunc(&gPlayerParty[slot], &sPartyMenuBoxes[slot], 0);
        DisplayPartyPokemonHPCheck(&gPlayerParty[slot], &sPartyMenuBoxes[slot], 0);
        DisplayPartyPokemonMaxHPCheck(&gPlayerParty[slot], &sPartyMenuBoxes[slot], 0);
        DisplayPartyPokemonHPBarCheck(&gPlayerParty[slot], &sPartyMenuBoxes[slot]);
    }
}

static void DisplayPartyPokemonDescriptionData(u8 slot, u8 stringId)
{
    struct Pokemon *mon = &gPlayerParty[slot];

    sPartyMenuBoxes[slot].infoRects->blitFunc(sPartyMenuBoxes[slot].windowId, 0, 0, 0, 0, TRUE);
    DisplayPartyPokemonNickname(mon, &sPartyMenuBoxes[slot], 0);
	
    if (!GetMonData(mon, MON_DATA_IS_EGG))
    {
        DisplayPartyPokemonLevelCheck(mon, &sPartyMenuBoxes[slot], 0);
        DisplayPartyPokemonGenderBlitFunc(mon, &sPartyMenuBoxes[slot], 0);
    }
    DisplayPartyPokemonDescriptionText(stringId, &sPartyMenuBoxes[slot], 0);
}

static void DisplayPartyPokemonDataForChooseHalf(u8 slot)
{
    u8 i, maxBattlers, *order = gSelectedOrderFromParty;
    struct Pokemon *mon = &gPlayerParty[slot];

    if (!GetBattleEntryEligibility(mon))
        DisplayPartyPokemonDescriptionData(slot, PARTYBOX_DESC_NOT_ABLE);
    else
    {
        if (gPartyMenu.chooseMonsBattleType == CHOOSE_MONS_FOR_UNION_ROOM_BATTLE)
            maxBattlers = 2;
        else
            maxBattlers = 3;
		
        for (i = 0; i < maxBattlers; ++i)
        {
            if (order[i] != 0 && (order[i] - 1) == slot)
            {
                DisplayPartyPokemonDescriptionData(slot, i + PARTYBOX_DESC_FIRST);
                return;
            }
        }
        DisplayPartyPokemonDescriptionData(slot, PARTYBOX_DESC_ABLE_3);
    }
}

static void DisplayPartyPokemonDataForWirelessMinigame(u8 slot)
{
	DisplayPartyPokemonDescriptionData(slot, IsMonAllowedInMinigame(slot) ? PARTYBOX_DESC_ABLE : PARTYBOX_DESC_NOT_ABLE);
}

static void DisplayPartyPokemonDataForMoveTutor(u8 slot)
{
	gSpecialVar_Result = FALSE;
	DisplayPartyPokemonDataToTeachMove(slot, gSpecialVar_0x8005, FALSE);
}

// Returns TRUE if teaching move or cant evolve with item (i.e. description data is shown), FALSE otherwise
static bool8 DisplayPartyPokemonDataForTeachMoveOrEvolutionItem(u8 slot)
{
	if (gPartyMenu.action == PARTY_ACTION_USE_ITEM)
	{
		u16 item = gSpecialVar_ItemId;
		
		switch (GetItemUseAnimFollowUpCBIdByItemType(item))
		{
			case ITEMUSE_FOLLOWUP_TM:
			    DisplayPartyPokemonDataToTeachMove(slot, ItemId_GetHoldEffectParam(item), TRUE);
				break;
			case ITEMUSE_FOLLOWUP_EVOLUTION_ITEM:
			    if (!GetMonData(&gPlayerParty[slot], MON_DATA_IS_EGG) && GetEvolutionTargetSpecies(slot, EVO_MODE_ITEM_CHECK, item, NULL))
					return FALSE;
				DisplayPartyPokemonDescriptionData(slot, PARTYBOX_DESC_NO_USE);
				break;
			default:
			    return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

static void DisplayPartyPokemonDataToTeachMove(u8 slot, u16 move, bool8 isTM)
{
    switch (CanMonLearnTMOrMoveTutor(&gPlayerParty[slot], move, isTM))
    {
    case CANNOT_LEARN_MOVE:
    case CANNOT_LEARN_MOVE_IS_EGG:
        DisplayPartyPokemonDescriptionData(slot, PARTYBOX_DESC_NOT_ABLE_2);
        break;
    case ALREADY_KNOWS_MOVE:
        DisplayPartyPokemonDescriptionData(slot, PARTYBOX_DESC_LEARNED);
        break;
    default:
        DisplayPartyPokemonDescriptionData(slot, PARTYBOX_DESC_ABLE_2);
        break;
    }
}

static void DisplayPartyPokemonDataForMultiBattle(u8 slot)
{
    struct PartyMenuBox *menuBox = &sPartyMenuBoxes[slot];
    u8 actualSlot = slot - (3);

    if (!gMultiPartnerParty[actualSlot].species)
        DrawEmptySlot(menuBox->windowId);
    else
    {
        menuBox->infoRects->blitFunc(menuBox->windowId, 0, 0, 0, 0, FALSE);
		
        StringCopy(gStringVar1, gMultiPartnerParty[actualSlot].nickname);
		StringGet_Nickname(gStringVar1);
        if (StringLength(gStringVar1) <= 5)
            ConvertInternationalString(gStringVar1, 1);
		
        DisplayPartyPokemonBarDetail(menuBox->windowId, 0, gStringVar1, 0, menuBox->infoRects->dimensions);
        DisplayPartyPokemonLevel(gMultiPartnerParty[actualSlot].level, menuBox);
        DisplayPartyPokemonGender(gMultiPartnerParty[actualSlot].gender, gMultiPartnerParty[actualSlot].species, gMultiPartnerParty[actualSlot].nickname, menuBox);
        DisplayPartyPokemonHP(gMultiPartnerParty[actualSlot].hp, gMultiPartnerParty[actualSlot].maxhp, menuBox);
        DisplayPartyPokemonMaxHP(gMultiPartnerParty[actualSlot].maxhp, menuBox);
        DisplayPartyPokemonHPBar(gMultiPartnerParty[actualSlot].hp, gMultiPartnerParty[actualSlot].maxhp, menuBox);
    }
}

static bool8 CanMonLearnTutorMove(struct Pokemon *mon, u16 move)
{
	u8 i;
	u16 species = GetMonData(mon, MON_DATA_SPECIES);
	
	// Check Cape Brink moves
	for (i = 0; i < ARRAY_COUNT(gCapeBrinkCompatibleSpecies); i++)
	{
		if (move == gCapeBrinkCompatibleSpecies[i].move)
			return (species == gCapeBrinkCompatibleSpecies[i].species);
	}
	// Check species's tutor moves table
	return CanSpeciesLearnTutorMove(species, move);
}

static u8 CanMonLearnTMOrMoveTutor(struct Pokemon *mon, u16 move, bool8 isTM)
{
    if (GetMonData(mon, MON_DATA_IS_EGG))
        return CANNOT_LEARN_MOVE_IS_EGG;
	
	if (isTM)
	{
		if (!CanMonLearnTM(mon, move))
			return CANNOT_LEARN_MOVE;
	}
	else
	{
		if (!CanMonLearnTutorMove(mon, move))
			return CANNOT_LEARN_MOVE;
	}
	
	if (FindMoveSlotInMoveset(mon, move) != MAX_MON_MOVES)
		return ALREADY_KNOWS_MOVE;
	
	return CAN_LEARN_MOVE;
}

///////////////////
// PARTY SPRITES //
///////////////////

static void CreatePartyMonSprites(u8 slot)
{
    u8 actualSlot;

    if (gPartyMenu.menuType == PARTY_MENU_TYPE_MULTI_SHOWCASE && slot >= MULTI_PARTY_SIZE)
    {
        actualSlot = slot - MULTI_PARTY_SIZE;
		
        if (gMultiPartnerParty[actualSlot].species)
        {
            CreatePartyMonIconSpriteParameterized(gMultiPartnerParty[actualSlot].species, &sPartyMenuBoxes[slot], 0);
            CreatePartyMonHeldItemSpriteParameterized(slot, gMultiPartnerParty[actualSlot].species, gMultiPartnerParty[actualSlot].heldItem, &sPartyMenuBoxes[slot], 0);
            CreatePartyMonPokeballSpriteParameterized(gMultiPartnerParty[actualSlot].species, &sPartyMenuBoxes[slot]);
            CreatePartyMonStatusSpriteParameterized(gMultiPartnerParty[actualSlot].species, gMultiPartnerParty[actualSlot].hp == 0 ? AILMENT_FNT : GetAilmentFromStatus(gMultiPartnerParty[actualSlot].status), &sPartyMenuBoxes[slot]);
        }
    }
    else if (GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES))
    {
        CreatePartyMonIconSprite(&gPlayerParty[slot], &sPartyMenuBoxes[slot], slot);
        CreatePartyMonHeldItemSprite(slot, &gPlayerParty[slot], &sPartyMenuBoxes[slot], 1);
        CreatePartyMonPokeballSprite(&gPlayerParty[slot], &sPartyMenuBoxes[slot]);
        CreatePartyMonStatusSprite(&gPlayerParty[slot], &sPartyMenuBoxes[slot]);
    }
}

static bool8 CreatePartyMonSpritesLoop(void)
{
    CreatePartyMonSprites(sPartyMenuInternal->data[0]);
	
    if (++sPartyMenuInternal->data[0] == PARTY_SIZE)
        return TRUE;
    else
        return FALSE;
}

static void CreatePartyMonIconSprite(struct Pokemon *mon, struct PartyMenuBox *menuBox, u32 slot)
{
    CreatePartyMonIconSpriteParameterized(GetMonData(mon, MON_DATA_SPECIES2), menuBox, 1);
    UpdatePartyMonHPBar(menuBox->monSpriteId, mon);
}

static void CreatePartyMonIconSpriteParameterized(u16 species, struct PartyMenuBox *menuBox, u8 priority)
{
    if (species)
    {
        menuBox->monSpriteId = CreateMonIcon(species, SpriteCB_MonIcon, menuBox->spriteCoords[0], menuBox->spriteCoords[1], 4);
        gSprites[menuBox->monSpriteId].oam.priority = priority;
    }
}

static void AnimateSelectedPartyIcon(u8 spriteId, u8 animNum)
{
    gSprites[spriteId].data[0] = 0;
	
    if (animNum == 0)
    {
        if (gSprites[spriteId].x == 16)
        {
            gSprites[spriteId].x2 = 0;
            gSprites[spriteId].y2 = -4;
        }
        else
        {
            gSprites[spriteId].x2 = -4;
            gSprites[spriteId].y2 = 0;
        }
        gSprites[spriteId].callback = SpriteCB_UpdatePartyMonIcon;
    }
    else
    {
        gSprites[spriteId].x2 = 0;
        gSprites[spriteId].y2 = 0;
        gSprites[spriteId].callback = SpriteCB_BouncePartyMonIcon;
    }
}

static void SpriteCB_BouncePartyMonIcon(struct Sprite *sprite)
{
    u8 animCmd = UpdateMonIconFrame(sprite);

    if (animCmd != 0)
    {
        if (animCmd & 1) // % 2 also matches
            sprite->y2 = -3;
        else
            sprite->y2 = 1;
    }
}

static void SpriteCB_UpdatePartyMonIcon(struct Sprite *sprite)
{
    UpdateMonIconFrame(sprite);
}

static void UpdatePartyMonIconSpecies(struct PartyMenuBox *menuBox, u16 species)
{
	DestroyMonIcon(&gSprites[menuBox->monSpriteId]);
	CreatePartyMonIconSpriteParameterized(species, menuBox, 1);
}

void UpdateCurrentPartyMonIconSpecies(u16 species)
{
	UpdatePartyMonIconSpecies(&sPartyMenuBoxes[gPartyMenu.slotId], species);
}

static void CreatePartyMonHeldItemSprite(u8 slot, struct Pokemon *mon, struct PartyMenuBox *menuBox, u8 priority)
{
	CreatePartyMonHeldItemSpriteParameterized(slot, GetMonData(mon, MON_DATA_SPECIES), GetMonData(mon, MON_DATA_HELD_ITEM), menuBox, priority);
}

#define GET_ITEM_SPRITE_TAG(slot) (0xD750 + slot)

static void CreatePartyMonHeldItemSpriteParameterized(u8 slot, u16 species, u16 item, struct PartyMenuBox *menuBox, u8 priority)
{
    if (species)
    {
		u16 tag = GET_ITEM_SPRITE_TAG(slot);
		
        menuBox->itemSpriteId = AddItemIconObjectWithCustomObjectTemplate(&sSpriteTemplate_ItemIcon, tag, tag, item);
		gSprites[menuBox->itemSpriteId].x = menuBox->spriteCoords[2] - 4;
		gSprites[menuBox->itemSpriteId].y = menuBox->spriteCoords[3] - 4;
        gSprites[menuBox->itemSpriteId].oam.priority = priority;
		gSprites[menuBox->itemSpriteId].invisible = (item == ITEM_NONE);
    }
}

// Returns the icon's priority. So another sprite can be created preserving the priority.
static u8 DestroyPartyMonHeldItemSprite(u8 slot, struct PartyMenuBox *menuBox)
{
	u8 priority = gSprites[menuBox->itemSpriteId].oam.priority;
	
	FreeSpriteTilesByTag(GET_ITEM_SPRITE_TAG(slot));
	FreeSpritePaletteByTag(GET_ITEM_SPRITE_TAG(slot));
	DestroySprite(&gSprites[menuBox->itemSpriteId]);
	
	return priority;
}

#undef GET_ITEM_SPRITE_TAG

static void CreateHeldItemSpriteForTrade(u8 spriteId, bool8 isMail)
{
	u8 newSpriteId = Create8x8SymbolSprite(250, 170, gSprites[spriteId].subpriority - 1, SYMBOL_HELDITEM + isMail);
	
    gSprites[newSpriteId].x2 = 4;
    gSprites[newSpriteId].y2 = 10;
	gSprites[newSpriteId].oam.priority = 1;
	gSprites[newSpriteId].data[7] = spriteId;
    gSprites[newSpriteId].callback = SpriteCB_HeldItem;
    gSprites[newSpriteId].callback(&gSprites[newSpriteId]);
}

void DrawHeldItemIconsForTrade(u8 *partyCounts, u8 *partySpriteIds, u8 whichParty)
{
	u8 i, addr = whichParty == TRADE_PLAYER ? 0 : PARTY_SIZE;
    struct Pokemon *party = whichParty == TRADE_PLAYER ? gPlayerParty : gEnemyParty;
	
	for (i = 0; i < partyCounts[whichParty]; i++)
	{
		u16 item = GetMonData(&party[i], MON_DATA_HELD_ITEM);
		
		if (item)
			CreateHeldItemSpriteForTrade(partySpriteIds[i + addr], ItemIsMail(item));
	}
}

static void SpriteCB_HeldItem(struct Sprite *sprite)
{
    u8 otherSpriteId = sprite->data[7];

    if (gSprites[otherSpriteId].invisible)
        sprite->invisible = TRUE;
    else
    {
        sprite->invisible = FALSE;
        sprite->x = gSprites[otherSpriteId].x + gSprites[otherSpriteId].x2;
        sprite->y = gSprites[otherSpriteId].y + gSprites[otherSpriteId].y2;
    }
}

#define tClosePartyMenu data[0]
#define tPriority       data[1]
#define tPriority2      data[2]

static void Task_UpdateHeldItemSprites_Step(u8 taskId)
{
	CreatePartyMonHeldItemSprite(gPartyMenu.slotId, &gPlayerParty[gPartyMenu.slotId], &sPartyMenuBoxes[gPartyMenu.slotId], gTasks[taskId].tPriority);
	
	if (gTasks[taskId].tPriority2 != 0xFF)
		CreatePartyMonHeldItemSprite(gPartyMenu.slotId2, &gPlayerParty[gPartyMenu.slotId2], &sPartyMenuBoxes[gPartyMenu.slotId2], gTasks[taskId].tPriority2);
	
	if (gTasks[taskId].tClosePartyMenu)
		Task_ClosePartyMenu(taskId);
	else
		Task_ReturnToChooseMonAfterText(taskId);
}

static void Task_UpdateHeldItemSprite(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
	{
		gTasks[taskId].tPriority = DestroyPartyMonHeldItemSprite(gPartyMenu.slotId, &sPartyMenuBoxes[gPartyMenu.slotId]);
		gTasks[taskId].tPriority2 = 0xFF;
		gTasks[taskId].tClosePartyMenu = FALSE;
		gTasks[taskId].func = Task_UpdateHeldItemSprites_Step;
    }
}

static void Task_UpdateHeldItemSpriteAndClosePartyMenu(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
		gTasks[taskId].tPriority = DestroyPartyMonHeldItemSprite(gPartyMenu.slotId, &sPartyMenuBoxes[gPartyMenu.slotId]);
		gTasks[taskId].tPriority2 = 0xFF;
		gTasks[taskId].tClosePartyMenu = TRUE;
		gTasks[taskId].func = Task_UpdateHeldItemSprites_Step;
    }
}

static void Task_UpdateHeldItemSpritesAfterMoveItems(u8 taskId)
{
	if (!IsPartyMenuTextPrinterActive())
	{
		gTasks[taskId].tPriority = DestroyPartyMonHeldItemSprite(gPartyMenu.slotId, &sPartyMenuBoxes[gPartyMenu.slotId]);
		gTasks[taskId].tPriority2 = DestroyPartyMonHeldItemSprite(gPartyMenu.slotId2, &sPartyMenuBoxes[gPartyMenu.slotId2]);
		gTasks[taskId].tClosePartyMenu = FALSE;
		gTasks[taskId].func = Task_UpdateHeldItemSprites_Step;
    }
}

#undef tClosePartyMenu
#undef tPriority
#undef tPriority2

static inline u8 CreatePartyMonStatusSpriteInternal(struct PartyMenuBox *menuBox)
{
	return CreateSprite(&sSpriteTemplate_StatusIcons, menuBox->spriteCoords[4], menuBox->spriteCoords[5], 0);
}

static void CreatePartyMonStatusSprite(struct Pokemon *mon, struct PartyMenuBox *menuBox)
{
    if (GetMonData(mon, MON_DATA_SPECIES))
    {
        menuBox->statusSpriteId = CreatePartyMonStatusSpriteInternal(menuBox);
        SetPartyMonAilmentGfx(mon, menuBox);
    }
}

static void CreatePartyMonStatusSpriteParameterized(u16 species, u8 status, struct PartyMenuBox *menuBox)
{
    if (species)
    {
        menuBox->statusSpriteId = CreatePartyMonStatusSpriteInternal(menuBox);
        UpdatePartyMonAilmentGfx(status, menuBox);
        gSprites[menuBox->statusSpriteId].oam.priority = 0;
    }
}

static void LoadPartyMenuPokeballGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_MenuPokeball);
    LoadCompressedSpriteSheet(&sSpriteSheet_MenuPokeballSmall);
    LoadCompressedSpritePalette(&sSpritePalette_MenuPokeball);
}

// For Cancel when Confirm isnt present
static u8 CreatePokeballButtonSprite(u8 x, u8 y)
{
    u8 spriteId = CreateSprite(&sSpriteTemplate_MenuPokeball, x, y, 8);
    gSprites[spriteId].oam.priority = 2;
    return spriteId;
}

// For Confirm and Cancel when both are present
static inline u8 CreateSmallPokeballButtonSprite(u8 x, u8 y)
{
    return CreateSprite(&sSpriteTemplate_MenuPokeballSmall, x, y, 8);
}

static void DrawCancelConfirmButtons(void)
{
    CopyToBgTilemapBufferRect_ChangePalette(1, sConfirmButton_Tilemap, 23, 16, 7, 2, 17);
    CopyToBgTilemapBufferRect_ChangePalette(1, sCancelButton_Tilemap, 23, 18, 7, 2, 17);
    ScheduleBgCopyTilemapToVram(1);
}

static void CreateCancelConfirmPokeballSprites(void)
{
    if (gPartyMenu.menuType == PARTY_MENU_TYPE_MULTI_SHOWCASE)
    {
        // The showcase has no Cancel/Confirm buttons
        FillBgTilemapBufferRect(1, 14, 23, 17, 7, 2, 1);
    }
    else
    {
        if (sPartyMenuInternal->chooseHalf)
        {
            sPartyMenuInternal->spriteIdConfirmPokeball = CreateSmallPokeballButtonSprite(0xBF, 0x88);
            DrawCancelConfirmButtons();
            sPartyMenuInternal->spriteIdCancelPokeball = CreateSmallPokeballButtonSprite(0xBF, 0x98);
        }
        else
            sPartyMenuInternal->spriteIdCancelPokeball = CreatePokeballButtonSprite(198, 148);

        AnimatePartySlot(gPartyMenu.slotId, 1);
    }
}

static inline u8 CreatePartyMonPokeballSpriteInternal(struct PartyMenuBox *menuBox)
{
	return CreateSprite(&sSpriteTemplate_MenuPokeball, menuBox->spriteCoords[6], menuBox->spriteCoords[7], 8);
}

static void CreatePartyMonPokeballSprite(struct Pokemon *mon, struct PartyMenuBox *menuBox)
{
    if (GetMonData(mon, MON_DATA_SPECIES))
        menuBox->pokeballSpriteId = CreatePartyMonPokeballSpriteInternal(menuBox);
}

static void CreatePartyMonPokeballSpriteParameterized(u16 species, struct PartyMenuBox *menuBox)
{
    if (species)
    {
		menuBox->pokeballSpriteId = CreatePartyMonPokeballSpriteInternal(menuBox);
        gSprites[menuBox->pokeballSpriteId].oam.priority = 0;
    }
}

static void LoadPartyMenuAilmentGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_StatusIcons);
    LoadCompressedSpritePalette(&sSpritePalette_StatusIcons);
}

static void UpdatePartyPokemonAilmentGfxAndLevelCheck(struct Pokemon *mon, u8 slot)
{
	struct PartyMenuBox *menuBox = &sPartyMenuBoxes[slot];
	
	SetPartyMonAilmentGfx(mon, menuBox);
	
	if (gSprites[menuBox->statusSpriteId].invisible)
		DisplayPartyPokemonLevelCheck(mon, menuBox, 1);
}

static void SetPartyMonAilmentGfx(struct Pokemon *mon, struct PartyMenuBox *menuBox)
{
    UpdatePartyMonAilmentGfx(GetMonAilment(mon), menuBox);
}

static void UpdatePartyMonAilmentGfx(u8 status, struct PartyMenuBox *menuBox)
{
    switch (status)
    {
    case AILMENT_NONE:
    case AILMENT_PKRS:
        gSprites[menuBox->statusSpriteId].invisible = TRUE;
        break;
    default:
        StartSpriteAnim(&gSprites[menuBox->statusSpriteId], status - 1);
        gSprites[menuBox->statusSpriteId].invisible = FALSE;
        break;
    }
}

static void UpdateMonDisplayInfoAfterLevelUp(u8 slot, struct Pokemon *mon)
{
	struct PartyMenuBox *menuBox = &sPartyMenuBoxes[slot];
	
	UpdatePartyPokemonAilmentGfxAndLevelCheck(mon, slot);
    DisplayPartyPokemonHPCheck(mon, menuBox, 1);
    DisplayPartyPokemonMaxHPCheck(mon, menuBox, 1);
    DisplayPartyPokemonHPBarCheck(mon, menuBox);
    UpdatePartyMonHPBar(menuBox->monSpriteId, mon);
    AnimatePartySlot(slot, 1);
    ScheduleBgCopyTilemapToVram(0);
}

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

static struct Pokemon* GetBaseMonForFusedSpecies(void)
{
	return &gSaveBlock1Ptr->fusions[gSpecialVar_0x8001];
}

void UpdatePartyAfterPokemonFusion(void)
{
	*GetBaseMonForFusedSpecies() = gPlayerParty[gPartyMenu.slotId2];
	ZeroMonData(&gPlayerParty[gPartyMenu.slotId2]);
	FillWindowPixelBuffer(sPartyMenuBoxes[gPartyMenu.slotId2].windowId, PIXEL_FILL(0));
	CopyWindowToVram(sPartyMenuBoxes[gPartyMenu.slotId2].windowId, COPYWIN_BOTH);
	PutWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId2].windowId);
	RenderPartyMenuBox(gPartyMenu.slotId2);
	DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].pokeballSpriteId]);
	DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].itemSpriteId]);
	DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].monSpriteId]);
	DestroySprite(&gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].statusSpriteId]);
	CompactPartySlots();
}

void CreatePartyMonAfterDefusing(void)
{
	struct Pokemon *baseMon = GetBaseMonForFusedSpecies();
	gPlayerParty[gPlayerPartyCount] = *baseMon;
	memset(baseMon, 0, sizeof(*baseMon));
	CreatePartyMonSprites(gPlayerPartyCount);
	RenderPartyMenuBox(gPlayerPartyCount);
}

static inline bool8 IsMultiPartnerSlot(u8 slot)
{
	return (slot == 1 || slot == 4 || slot == 5);
}

static bool8 SelectedPartnerSlotInMulti(u8 slot)
{
	if (IsMultiBattle() && IsMultiPartnerSlot(slot))
		return TRUE;
	return FALSE;
}

static u8 GetPartyLayoutFromBattleType(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        return PARTY_LAYOUT_SINGLE;
    else if (IsMultiBattle())
        return PARTY_LAYOUT_MULTI;
	else
		return PARTY_LAYOUT_DOUBLE;
}

static void SwapPartyPokemon(struct Pokemon *mon1, struct Pokemon *mon2)
{
    struct Pokemon *buffer = Alloc(sizeof(struct Pokemon));
    *buffer = *mon1;
    *mon1 = *mon2;
    *mon2 = *buffer;
    Free(buffer);
}

static void GiveItemToMon(struct Pokemon *mon, u16 item)
{
    u8 itemBytes[2];

    if (ItemIsMail(item))
    {
        if (GiveMailToMon(mon, item) == 0xFF)
            return;
    }
    itemBytes[0] = item;
    itemBytes[1] = item >> 8;
    SetMonData(mon, MON_DATA_HELD_ITEM, itemBytes);
}

static u8 TryTakeMonItem(struct Pokemon *mon, u16 item)
{
    if (!item)
        return 0;
    else if (!AddBagItem(item, 1))
        return 1;
	
    item = ITEM_NONE;
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
	
    return 2;
}

u8 GetCursorSelectionMonId(void)
{
    return gPartyMenu.slotId;
}

void IsSelectedMonEgg(void)
{
    if (GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_IS_EGG))
        gSpecialVar_Result = TRUE;
    else
        gSpecialVar_Result = FALSE;
}

u8 GetPartyMenuType(void)
{
    return gPartyMenu.menuType;
}

static s8 *GetCurrentPartySlotPtr(void)
{
    if (gPartyMenu.action == PARTY_ACTION_SWITCH || gPartyMenu.action == PARTY_ACTION_SOFTBOILED)
        return &gPartyMenu.slotId2;
    else
        return &gPartyMenu.slotId;
}

static void PartyMenuDisplayYesNoMenu(void)
{
    CreateYesNoMenu(&sPartyMenuYesNoWindowTemplate, 2, 0, 2, 0x4F, 13, 0);
}

static void ResetMedicineItemData(u8 initialLevel)
{
	memset(&sMedicineItemData, 0, sizeof(sMedicineItemData));
	sMedicineItemData.oldHP = -1;
	sMedicineItemData.initialLevel = initialLevel;
	sMedicineItemData.savedItemUseCB = gItemUseCB;
}

static void CB2_SetUpExitToBattleScreen(void)
{
    CB2_SetUpReshowBattleScreenAfterMenu();
    SetMainCallback2(ReshowBattleScreenAfterMenu);
}

u8 GetAilmentFromStatus(u32 status)
{
    if (status & STATUS1_PSN_ANY)
        return AILMENT_PSN;
    else if (status & STATUS1_PARALYSIS)
        return AILMENT_PRZ;
    else if (status & STATUS1_SLEEP)
        return AILMENT_SLP;
    else if (status & STATUS1_FREEZE)
        return AILMENT_FRZ;
    else if (status & STATUS1_BURN)
        return AILMENT_BRN;
	
    return AILMENT_NONE;
}

static u8 GetMonAilment(struct Pokemon *mon)
{
    u8 ailment;

    if (!GetMonData(mon, MON_DATA_HP))
        return AILMENT_FNT;
	
    ailment = GetAilmentFromStatus(GetMonData(mon, MON_DATA_STATUS));
    if (ailment != AILMENT_NONE)
        return ailment;
	
    if (CheckPartyPokerus(mon, 0))
        return AILMENT_PKRS;
	
    return AILMENT_NONE;
}

static bool8 IsMonAllowedInMinigame(u8 slot)
{
    if (!((gPartyMenu.data1 >> slot) & 1))
        return FALSE;
    return TRUE;
}

static u8 CreateLevelUpStatsWindow(void)
{
    sPartyMenuInternal->windowId[0] = AddWindow(&sLevelUpStatsWindowTemplate);
    DrawStdFrameWithCustomTileAndPalette(sPartyMenuInternal->windowId[0], FALSE, 0x4F, 13);
    return sPartyMenuInternal->windowId[0];
}

static void RemoveLevelUpStatsWindow(void)
{
    ClearWindowTilemap(sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
}

static void DisplayLevelUpStatsPg1(u8 taskId)
{
    s16 *arrayPtr = sPartyMenuInternal->data;
	u8 windowIdArrayId = NUM_STATS + NUM_STATS;
	
    arrayPtr[windowIdArrayId] = CreateLevelUpStatsWindow();
    DrawLevelUpWindowPg1(arrayPtr[windowIdArrayId], arrayPtr, &arrayPtr[NUM_STATS], 1, 2, 3);
    CopyWindowToVram(arrayPtr[windowIdArrayId], COPYWIN_GFX);
    ScheduleBgCopyTilemapToVram(2);
}

static void Task_DisplayLevelUpStatsPg1(u8 taskId)
{
    if (WaitFanfare(FALSE) && !IsPartyMenuTextPrinterActive() && (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON)))
    {
        PlaySE(SE_SELECT);
        DisplayLevelUpStatsPg1(taskId);
        gTasks[taskId].func = Task_DisplayLevelUpStatsPg2;
    }
}

static void DisplayLevelUpStatsPg2(u8 taskId)
{
    s16 *arrayPtr = sPartyMenuInternal->data;
	u8 windowIdArrayId = NUM_STATS + NUM_STATS;

    DrawLevelUpWindowPg2(arrayPtr[windowIdArrayId], &arrayPtr[NUM_STATS], 1, 2, 3);
    CopyWindowToVram(arrayPtr[windowIdArrayId], COPYWIN_GFX);
    ScheduleBgCopyTilemapToVram(2);
}

static void Task_DisplayLevelUpStatsPg2(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DisplayLevelUpStatsPg2(taskId);
		++sMedicineItemData.initialLevel;
        gTasks[taskId].func = Task_TryLearnNewMoves;
    }
}

static void Task_TryLearnNewMoves(u8 taskId)
{
    u8 ret;

    if (WaitFanfare(0) && (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON)))
    {
        RemoveLevelUpStatsWindow();
		
		for (; sMedicineItemData.initialLevel <= sMedicineItemData.finalLevel; sMedicineItemData.initialLevel++)
		{
			SetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_LEVEL, &sMedicineItemData.initialLevel);
			ret = MonTryLearningNewMove(&gPlayerParty[gPartyMenu.slotId], TRUE);
			gPartyMenu.learnMoveState = 1;
			
			switch (ret)
			{
				case MON_DONT_FIND_MOVE_TO_LEARN:
				    if (sMedicineItemData.initialLevel >= sMedicineItemData.finalLevel)
						PartyMenuTryEvolution(taskId);
					break;
				case MON_HAS_MAX_MOVES:
				    DisplayMonNeedsToReplaceMove(taskId);
					break;
				case MON_ALREADY_KNOWS_MOVE:
				    gTasks[taskId].func = Task_TryLearningNextMove;
					break;
				default:
				    DisplayMonLearnedMove(taskId, gMoveToLearn);
					break;
			}
			if (ret != MON_DONT_FIND_MOVE_TO_LEARN)
				break;
		}
    }
}

static void Task_TryLearningNextMove(u8 taskId)
{
    u8 ret;
	
	for (; sMedicineItemData.initialLevel <= sMedicineItemData.finalLevel; sMedicineItemData.initialLevel++)
	{
		SetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_LEVEL, &sMedicineItemData.initialLevel);
		ret = MonTryLearningNewMove(&gPlayerParty[gPartyMenu.slotId], FALSE);
		
		switch (ret)
		{
			case MON_DONT_FIND_MOVE_TO_LEARN:
			    if (sMedicineItemData.initialLevel >= sMedicineItemData.finalLevel)
					PartyMenuTryEvolution(taskId);
			    break;
			case MON_HAS_MAX_MOVES:
			    DisplayMonNeedsToReplaceMove(taskId);
				break;
			case MON_ALREADY_KNOWS_MOVE:
			    gTasks[taskId].func = Task_TryLearningNextMove;
			    return;
			default:
			    DisplayMonLearnedMove(taskId, gMoveToLearn);
				break;
		}
		if (ret != MON_DONT_FIND_MOVE_TO_LEARN)
			break;
	}
}

static void Task_TryLearnPostFormeChangeMove(u8 taskId)
{
	u32 i;
	u8 movePos;
	struct Pokemon *mon;
	u16 replaceMove, replaceMove2;
	
	if (!IsPartyMenuTextPrinterActive())
	{
		mon = &gPlayerParty[gPartyMenu.slotId];
		replaceMove = MOVE_NONE;
		replaceMove2 = MOVE_NONE;
		gMoveToLearn = MOVE_NONE;
		
		gPartyMenuUseExitCallback = FALSE;
		
		for (i = 0; i < ARRAY_COUNT(sFormChangeMoves); i++)
		{
			if (GetMonData(mon, MON_DATA_SPECIES) == sFormChangeMoves[i].species)
			{
				gMoveToLearn = sFormChangeMoves[i].moveToLearn;
				replaceMove = sFormChangeMoves[i].replaceMove1;
				replaceMove2 = sFormChangeMoves[i].replaceMove2;
				break;
			}
		}

		if (gMoveToLearn)
		{
			if (replaceMove)
			{
				movePos = FindMoveSlotInMoveset(mon, replaceMove);
				
				if (movePos < MAX_MON_MOVES)
				{
					SetMonMoveSlot(mon, gMoveToLearn, movePos);
					DisplayMonLearnedMove(taskId, gMoveToLearn);
					return;
				}
			}
			if (replaceMove2)
			{
				movePos = FindMoveSlotInMoveset(mon, replaceMove2);
				
				if (movePos < MAX_MON_MOVES)
				{
					SetMonMoveSlot(mon, gMoveToLearn, movePos);
					DisplayMonLearnedMove(taskId, gMoveToLearn);
					return;
				}
			}
			movePos = FindMoveSlotInMoveset(mon, MOVE_NONE); // Find first free slot
			
			if (movePos < MAX_MON_MOVES)
			{
				SetMonMoveSlot(mon, gMoveToLearn, movePos);
				DisplayMonLearnedMove(taskId, gMoveToLearn);
				return;
			}
			DisplayMonNeedsToReplaceMove(taskId);
		}
		else
		{
			if (replaceMove)
			{
				movePos = FindMoveSlotInMoveset(mon, replaceMove);
				if (movePos < MAX_MON_MOVES)
					DeleteMonMove(mon, movePos);
			}
			if (replaceMove2)
			{
				movePos = FindMoveSlotInMoveset(mon, replaceMove2);
				if (movePos < MAX_MON_MOVES)
					DeleteMonMove(mon, movePos);
			}
			Task_ClosePartyMenuAfterText(taskId);
		}
	}
}

static void PartyMenuTryEvolution(u8 taskId)
{
    u16 targetSpecies = GetEvolutionTargetSpecies(gPartyMenu.slotId, EVO_MODE_NORMAL, ITEM_NONE, NULL);
	
	sMedicineItemData.initialLevel = sMedicineItemData.finalLevel = 0;
	
    if (targetSpecies)
    {
        FreePartyPointers();
        gCB2_AfterEvolution = gPartyMenu.exitCallback;
        BeginEvolutionScene(&gPlayerParty[gPartyMenu.slotId], targetSpecies, TRUE, gPartyMenu.slotId);
        DestroyTask(taskId);
    }
    else
		SetMedicineItemFunc(taskId);
}

static bool8 HasPartySlotAlreadyBeenSelected(u8 slot)
{
    u8 i;

    for (i = 0; i < ARRAY_COUNT(gSelectedOrderFromParty); ++i)
	{
        if (gSelectedOrderFromParty[i] == slot)
            return TRUE;
	}
    return FALSE;
}

// Returns where the mon is eligible or not to a half party's battle
static bool8 GetBattleEntryEligibility(struct Pokemon *mon)
{
    if (GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
	
    switch (gPartyMenu.chooseMonsBattleType)
    {
		case CHOOSE_MONS_FOR_UNION_ROOM_BATTLE:
			if (GetMonData(mon, MON_DATA_LEVEL) > 30)
				return FALSE;
			break;
		case CHOOSE_MONS_FOR_CABLE_CLUB_BATTLE:
			if (!GetMonData(mon, MON_DATA_HP))
				return FALSE;
			break;
		default:
		    return FALSE;
    }
    return TRUE;
}

void ClearSelectedPartyOrder(void)
{
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
}

// Unused
static void FieldCallback_Waterfall(void)
{
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    FieldEffectStart(FLDEFF_USE_WATERFALL);
}

// Unused
static bool8 SetUpFieldMove_Waterfall(void)
{
    s16 x, y;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
	
    if (MetatileBehavior_IsWaterfall(MapGridGetMetatileBehaviorAt(x, y)) && IsPlayerSurfingNorth())
    {
        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
        gPostMenuFieldCallback = FieldCallback_Waterfall;
        return TRUE;
    }
    return FALSE;
}

//////////////////
// BOX PALETTES //
//////////////////

// 0 - normal palette, 1 - selected slot palette
void AnimatePartySlot(u8 slot, u8 animNum)
{
    u8 spriteId;

    switch (slot)
    {
    default:
        if (GetMonData(&gPlayerParty[slot], MON_DATA_SPECIES))
        {
            LoadPartyBoxPalette(&sPartyMenuBoxes[slot], GetPartyBoxPaletteFlags(slot, animNum));
            AnimateSelectedPartyIcon(sPartyMenuBoxes[slot].monSpriteId, animNum);
			StartSpriteAnim(&gSprites[sPartyMenuBoxes[slot].pokeballSpriteId], animNum);
        }
        return;
    case PARTY_SIZE: // Confirm
	    SetBgTilemapPalette(1, 23, 16, 7, 2, animNum == 0 ? 1 : 2);
        spriteId = sPartyMenuInternal->spriteIdConfirmPokeball;
        break;
    case PARTY_SIZE + 1: // Cancel
        // The position of the Cancel button changes if Confirm is present
        if (!sPartyMenuInternal->chooseHalf)
			SetBgTilemapPalette(1, 23, 17, 7, 2, animNum == 0 ? 1 : 2);
        else
            SetBgTilemapPalette(1, 23, 18, 7, 2, animNum == 0 ? 1 : 2);
		
        spriteId = sPartyMenuInternal->spriteIdCancelPokeball;
        break;
    }
    StartSpriteAnim(&gSprites[spriteId], animNum);
    ScheduleBgCopyTilemapToVram(1);
}

static u8 GetPartyBoxPaletteFlags(u8 slot, u8 animNum)
{
    u8 palFlags = 0;

    if (animNum == 1)
        palFlags |= PARTY_PAL_SELECTED;
	
    if (GetMonData(&gPlayerParty[slot], MON_DATA_HP) == 0)
        palFlags |= PARTY_PAL_FAINTED;
	
    if (gPartyMenu.layout == PARTY_LAYOUT_MULTI && IsMultiPartnerSlot(slot))
        palFlags |= PARTY_PAL_MULTI_ALT;
	
    if (gPartyMenu.action == PARTY_ACTION_SWITCHING)
        palFlags |= PARTY_PAL_SWITCHING;
	
    if (gPartyMenu.action == PARTY_ACTION_SWITCH)
    {
        if (slot == gPartyMenu.slotId || slot == gPartyMenu.slotId2)
            palFlags |= PARTY_PAL_TO_SWITCH;
    }
    if (gPartyMenu.action == PARTY_ACTION_SOFTBOILED && slot == gPartyMenu.slotId)
        palFlags |= PARTY_PAL_TO_SOFTBOIL;
	
    return palFlags;
}

static void PartyPaletteBufferCopy(u8 offset)
{
    offset *= 16;
    CpuCopy16(&gPlttBufferUnfaded[0x30], &gPlttBufferUnfaded[offset], 32);
    CpuCopy16(&gPlttBufferUnfaded[0x30], &gPlttBufferFaded[offset], 32);
}

static u16 *GetPartyMenuPalBufferPtr(u8 paletteId)
{
    return &sPartyMenuInternal->palBuffer[paletteId];
}

static void LoadPartyBoxPaletteLoader(const u8 *paletteIds, const u8 *paletteOffsets, u8 palNum)
{
	LoadPalette(GetPartyMenuPalBufferPtr(paletteIds[0]), paletteOffsets[0] + palNum, 2);
	LoadPalette(GetPartyMenuPalBufferPtr(paletteIds[1]), paletteOffsets[1] + palNum, 2);
	LoadPalette(GetPartyMenuPalBufferPtr(paletteIds[2]), paletteOffsets[2] + palNum, 2);
}

static void LoadPartyBoxPalette(struct PartyMenuBox *menuBox, u8 palFlags)
{
    u8 palNum = GetWindowAttribute(menuBox->windowId, WINDOW_PALETTE_NUM) * 16;

    if (palFlags & PARTY_PAL_TO_SOFTBOIL)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2, palNum);
        }
        else
        {
            LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2, palNum);
        }
    }
    else if (palFlags & PARTY_PAL_SWITCHING)
    {
        LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1, palNum);
        LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2, palNum);
    }
    else if (palFlags & PARTY_PAL_TO_SWITCH)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2, palNum);
        }
        else
        {
            LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2, palNum);
        }
    }
    else if (palFlags & PARTY_PAL_FAINTED)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionFaintedPalIds, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2, palNum);
        }
        else
        {
            LoadPartyBoxPaletteLoader(sPartyBoxFaintedPalIds1, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxFaintedPalIds2, sPartyBoxPalOffsets2, palNum);
        }
    }
    else if (palFlags & PARTY_PAL_MULTI_ALT)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionMultiPalIds, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2, palNum);
        }
        else
        {
            LoadPartyBoxPaletteLoader(sPartyBoxMultiPalIds1, sPartyBoxPalOffsets1, palNum);
            LoadPartyBoxPaletteLoader(sPartyBoxMultiPalIds2, sPartyBoxPalOffsets2, palNum);
        }
    }
    else if (palFlags & PARTY_PAL_SELECTED)
    {
        LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionPalIds1, sPartyBoxPalOffsets1, palNum);
        LoadPartyBoxPaletteLoader(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2, palNum);
    }
    else
    {
        LoadPartyBoxPaletteLoader(sPartyBoxEmptySlotPalIds1, sPartyBoxPalOffsets1, palNum);
        LoadPartyBoxPaletteLoader(sPartyBoxEmptySlotPalIds2, sPartyBoxPalOffsets2, palNum);
    }
}

/////////////////
// INPUT TASKS //
/////////////////

static void MoveCursorToConfirm(void)
{
    AnimatePartySlot(gPartyMenu.slotId, 0);
    gPartyMenu.slotId = PARTY_SIZE;
    AnimatePartySlot(gPartyMenu.slotId, 1);
}

void Task_HandleChooseMonInput(u8 taskId)
{
    if (!gPaletteFade.active && !sub_80BF748())
    {
        s8 *slotPtr = GetCurrentPartySlotPtr();

        switch (PartyMenuButtonHandler(slotPtr))
        {
        case 1: // Selected mon
            HandleChooseMonSelection(taskId, slotPtr);
            break;
        case 2: // Selected Cancel
            HandleChooseMonCancel(taskId, slotPtr);
            break;
        case 8: // Start button
            if (sPartyMenuInternal->chooseHalf)
            {
                PlaySE(SE_SELECT);
                MoveCursorToConfirm();
            }
            break;
        }
    }
}

static bool8 IsSelectedMonNotEgg(u8 *slotPtr)
{
    if (GetMonData(&gPlayerParty[*slotPtr], MON_DATA_IS_EGG))
    {
        PlaySE(SE_FAILURE);
        return FALSE;
    }
    return TRUE;
}

static void HandleChooseMonSelection(u8 taskId, s8 *slotPtr)
{
    if (*slotPtr == PARTY_SIZE)
        gPartyMenu.task(taskId);
    else
    {
        switch (gPartyMenu.action)
        {
        case PARTY_ACTION_SOFTBOILED:
            if (IsSelectedMonNotEgg((u8 *)slotPtr))
                Task_TryUseSoftboiledOnPartyMon(taskId);
            break;
        case PARTY_ACTION_USE_ITEM:
            if (IsSelectedMonNotEgg((u8 *)slotPtr))
            {
                if (gPartyMenu.menuType == PARTY_MENU_TYPE_IN_BATTLE)
                    sPartyMenuInternal->exitCallback = CB2_SetUpExitToBattleScreen;
				
                gItemUseCB(taskId, Task_ClosePartyMenuAfterText);
            }
            break;
        case PARTY_ACTION_MOVE_TUTOR:
            if (IsSelectedMonNotEgg((u8 *)slotPtr))
            {
                PlaySE(SE_SELECT);
                TryTutorSelectedMon(taskId);
            }
            break;
        case PARTY_ACTION_GIVE_MAILBOX_MAIL:
            if (IsSelectedMonNotEgg((u8 *)slotPtr))
            {
                PlaySE(SE_SELECT);
                TryGiveMailToSelectedMon(taskId);
            }
            break;
        case PARTY_ACTION_GIVE_ITEM:
        case PARTY_ACTION_GIVE_PC_ITEM:
            if (IsSelectedMonNotEgg((u8 *)slotPtr))
            {
                PlaySE(SE_SELECT);
                TryGiveItemOrMailToSelectedMon(taskId);
            }
            break;
        case PARTY_ACTION_SWITCH:
            PlaySE(SE_SELECT);
            SwitchSelectedMons(taskId);
            break;
        case PARTY_ACTION_CHOOSE_AND_CLOSE:
            PlaySE(SE_SELECT);
            gSpecialVar_0x8004 = *slotPtr;
			
            if (gPartyMenu.menuType == PARTY_MENU_TYPE_MOVE_RELEARNER)
                gSpecialVar_0x8005 = GetNumberOfRelearnableMoves(&gPlayerParty[*slotPtr]);
			
            Task_ClosePartyMenu(taskId);
            break;
        case PARTY_ACTION_MINIGAME:
            if (IsSelectedMonNotEgg((u8 *)slotPtr))
                TryEnterMonForMinigame(taskId, (u8)*slotPtr);
            break;
        default:
            PlaySE(SE_SELECT);
            Task_TryCreateSelectionWindow(taskId);
            break;
        }
    }
}

static void TryEnterMonForMinigame(u8 taskId, u8 slot)
{
    if (IsMonAllowedInMinigame(slot))
    {
        PlaySE(SE_SELECT);
        gSpecialVar_0x8004 = slot;
        Task_ClosePartyMenu(taskId);
    }
    else
    {
        PlaySE(SE_FAILURE);
        DisplayPartyMenuMessage(gText_PkmnCantParticipate, FALSE);
        ScheduleBgCopyTilemapToVram(2);
        gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
    }
}

static void TryTutorSelectedMon(u8 taskId)
{
    struct Pokemon *mon;

    if (!gPaletteFade.active)
    {
        mon = &gPlayerParty[gPartyMenu.slotId];
		GetMonNickname(mon, gStringVar1);
		
		gPartyMenu.data1 = gSpecialVar_0x8005;
		StringCopy(gStringVar2, gBattleMoves[gPartyMenu.data1].name);
		gPartyMenu.learnMoveState = 2;
		
		switch (CanMonLearnTMOrMoveTutor(mon, gPartyMenu.data1, FALSE))
		{
			case CANNOT_LEARN_MOVE:
			    DisplayLearnMoveMessageAndClose(taskId, gText_PkmnCantLearnMove);
				break;
			case ALREADY_KNOWS_MOVE:
			    DisplayLearnMoveMessageAndClose(taskId, gText_PkmnAlreadyKnows);
				break;
			default:
			    if (GiveMoveToMon(mon, gPartyMenu.data1) != MON_HAS_MAX_MOVES)
					Task_LearnedMove(taskId);
				else
				{
					DisplayLearnMoveMessage(gText_PkmnNeedsToReplaceMove);
					gTasks[taskId].func = Task_ReplaceMoveYesNo;
				}
				break;
		}
    }
}

static void RemoveItemToGiveFromBag(u16 item)
{
    if (gPartyMenu.action == PARTY_ACTION_GIVE_PC_ITEM) // Unused, never occurs
        RemovePCItem(item, 1);
    else
        RemoveBagItem(item, 1);
}

static void GiveItemToSelectedMon(u8 taskId)
{
    u16 item;

    if (!gPaletteFade.active)
    {
        item = gPartyMenu.bagItem;
        DisplayGaveHeldItemMessage(&gPlayerParty[gPartyMenu.slotId], item, FALSE);
        GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], item);
        RemoveItemToGiveFromBag(item);
        gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
    }
}

static void CB2_WriteMailToGiveMonFromBag(void)
{
    GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], gPartyMenu.bagItem);
    DoEasyChatScreen(EASY_CHAT_TYPE_MAIL, gSaveBlock1Ptr->mail[GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_MAIL)].words, CB2_ReturnToPartyOrBagMenuFromWritingMail);
}

static void GiveItemOrMailToSelectedMon(u8 taskId)
{
    if (ItemIsMail(gPartyMenu.bagItem))
    {
        RemoveItemToGiveFromBag(gPartyMenu.bagItem);
        sPartyMenuInternal->exitCallback = CB2_WriteMailToGiveMonFromBag;
        Task_ClosePartyMenu(taskId);
    }
    else
        GiveItemToSelectedMon(taskId);
}

static void TryGiveItemOrMailToSelectedMon(u8 taskId)
{
    sPartyMenuItemId = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM);
	
    if (!sPartyMenuItemId)
        GiveItemOrMailToSelectedMon(taskId);
    else if (ItemIsMail(sPartyMenuItemId))
        DisplayItemMustBeRemovedFirstMessage(taskId);
    else
    {
        DisplayAlreadyHoldingItemSwitchMessage(&gPlayerParty[gPartyMenu.slotId], sPartyMenuItemId, TRUE);
        gTasks[taskId].func = Task_SwitchItemsFromBagYesNo;
    }
}

static void TryGiveMailToSelectedMon(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    struct Mail *mail;

    gPartyMenuUseExitCallback = FALSE;
	
    if (GetMonData(mon, MON_DATA_HELD_ITEM))
        DisplayPartyMenuMessage(gText_PkmnHoldingItemCantHoldMail, TRUE);
    else
    {
		mail = &gSaveBlock1Ptr->mail[gPlayerPcMenuManager.cursorPos + 6 + gPlayerPcMenuManager.itemsAbove];
		
        GiveMailToMon2(mon, mail);
        ClearMailStruct(mail);
        DisplayPartyMenuMessage(gText_MailTransferredFromMailbox, TRUE);
    }
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
}

static void Task_SwitchItemsFromBagYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleSwitchItemsFromBagYesNoInput;
    }
}

static void Task_HandleSwitchItemsFromBagYesNoInput(u8 taskId)
{
    u16 item;

    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes, switch items
        item = gPartyMenu.bagItem;
		
        RemoveItemToGiveFromBag(item);
		
        if (!AddBagItem(sPartyMenuItemId, 1))
        {
            GiveItemToBagOrPC(item);
            BufferBagFullCantTakeItemMessage(sPartyMenuItemId);
            DisplayPartyMenuMessage(gStringVar4, FALSE);
            gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
        }
        else if (ItemIsMail(item))
        {
            sPartyMenuInternal->exitCallback = CB2_WriteMailToGiveMonFromBag;
            Task_ClosePartyMenu(taskId);
        }
        else
        {
            GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], item);
            DisplaySwitchedHeldItemMessage(item, sPartyMenuItemId, TRUE);
            gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
        }
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1: // No, dont switch items
        gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
        break;
    }
}

static void HandleChooseMonCancel(u8 taskId, s8 *slotPtr)
{
    switch (gPartyMenu.action)
    {
    case PARTY_ACTION_SEND_OUT:
        PlaySE(SE_FAILURE);
        break;
    case PARTY_ACTION_SWITCH:
    case PARTY_ACTION_SOFTBOILED:
        PlaySE(SE_SELECT);
        FinishTwoMonAction(taskId);
        break;
    case PARTY_ACTION_MINIGAME:
        PlaySE(SE_SELECT);
        CancelParticipationPrompt(taskId);
        break;
    default:
        PlaySE(SE_SELECT);
		
        if (gPartyMenu.menuType == PARTY_MENU_TYPE_CHOOSE_HALF)
            DisplayCancelChooseMonYesNo(taskId);
        else
        {
            if (!MenuHelpers_LinkSomething())
                gSpecialVar_0x8004 = PARTY_SIZE + 1;
			
            gPartyMenuUseExitCallback = FALSE;
            *slotPtr = PARTY_SIZE + 1;
            Task_ClosePartyMenu(taskId);
        }
        break;
    }
}

static u16 PartyMenuButtonHandler(s8 *slotPtr)
{
    s8 movementDir;

    switch (gMain.newAndRepeatedKeys)
    {
    case DPAD_UP:
    case L_BUTTON:
        movementDir = MENU_DIR_UP;
        break;
    case DPAD_DOWN:
    case R_BUTTON:
        movementDir = MENU_DIR_DOWN;
        break;
    case DPAD_LEFT:
        movementDir = MENU_DIR_LEFT;
        break;
    case DPAD_RIGHT:
        movementDir = MENU_DIR_RIGHT;
        break;
    default:
        movementDir = 0;
		break;
    }
	
    if (JOY_NEW(START_BUTTON))
        return 8;
	
    if (movementDir)
    {
        UpdateCurrentPartySelection(slotPtr, movementDir);
        return 0;
    }
    // Pressed Cancel
    if (JOY_NEW(A_BUTTON) && *slotPtr == PARTY_SIZE + 1)
        return 2;
	
    return JOY_NEW(A_BUTTON | B_BUTTON);
}

static void UpdateCurrentPartySelection(s8 *slotPtr, s8 movementDir)
{
    s8 newSlotId = *slotPtr;
    u8 layout = gPartyMenu.layout;

    if (layout == PARTY_LAYOUT_SINGLE)
        UpdatePartySelectionSingleLayout(slotPtr, movementDir);
    else
        UpdatePartySelectionDoubleLayout(slotPtr, movementDir);
	
    if (*slotPtr != newSlotId)
    {
        PlaySE(SE_SELECT);
        AnimatePartySlot(newSlotId, 0);
        AnimatePartySlot(*slotPtr, 1);
    }
}

static void UpdatePartySelectionSingleLayout(s8 *slotPtr, s8 movementDir)
{
    // PARTY_SIZE + 1 is Cancel, PARTY_SIZE is Confirm
    switch (movementDir)
    {
    case MENU_DIR_UP:
        if (*slotPtr == 0)
            *slotPtr = PARTY_SIZE + 1;
        else if (*slotPtr == PARTY_SIZE)
            *slotPtr = gPlayerPartyCount - 1;
        else if (*slotPtr == PARTY_SIZE + 1)
        {
            if (sPartyMenuInternal->chooseHalf)
                *slotPtr = PARTY_SIZE;
            else
                *slotPtr = gPlayerPartyCount - 1;
        }
        else
            --*slotPtr;
        break;
    case MENU_DIR_DOWN:
        if (*slotPtr == PARTY_SIZE + 1)
            *slotPtr = 0;
        else
        {
            if (*slotPtr == gPlayerPartyCount - 1)
            {
                if (sPartyMenuInternal->chooseHalf)
                    *slotPtr = PARTY_SIZE;
                else
                    *slotPtr = PARTY_SIZE + 1;
            }
            else
                ++*slotPtr;
        }
        break;
    case MENU_DIR_RIGHT:
        if (gPlayerPartyCount != 1 && *slotPtr == 0)
        {
            if (sPartyMenuInternal->lastSelectedSlot == 0)
                *slotPtr = 1;
            else
                *slotPtr = sPartyMenuInternal->lastSelectedSlot;
        }
        break;
    case MENU_DIR_LEFT:
        if (*slotPtr != 0 && *slotPtr != PARTY_SIZE && *slotPtr != PARTY_SIZE + 1)
        {
            sPartyMenuInternal->lastSelectedSlot = *slotPtr;
            *slotPtr = 0;
        }
        break;
    }
}

static s8 GetNewSlotDoubleLayout(s8 slotId, s8 movementDir)
{
    while (TRUE)
    {
        slotId += movementDir;
		
        if ((u8)slotId >= PARTY_SIZE)
            return -1;
        else if (GetMonData(&gPlayerParty[slotId], MON_DATA_SPECIES))
            return slotId;
    }
}

static void UpdatePartySelectionDoubleLayout(s8 *slotPtr, s8 movementDir)
{
    // PARTY_SIZE + 1 is Cancel, PARTY_SIZE is Confirm
    // newSlot is used temporarily as a movement direction during its later assignment
    s8 newSlot = movementDir;

    switch (movementDir)
    {
    case MENU_DIR_UP:
        if (*slotPtr == 0)
        {
            *slotPtr = PARTY_SIZE + 1;
            break;
        }
        else if (*slotPtr == PARTY_SIZE)
        {
            *slotPtr = gPlayerPartyCount - 1;
            break;
        }
        else if (*slotPtr == PARTY_SIZE + 1)
        {
            if (sPartyMenuInternal->chooseHalf)
            {
                *slotPtr = PARTY_SIZE;
                break;
            }
            --*slotPtr;
        }
        newSlot = GetNewSlotDoubleLayout(*slotPtr, newSlot);
        if (newSlot != -1)
            *slotPtr = newSlot;
        break;
    case MENU_DIR_DOWN:
        if (*slotPtr == PARTY_SIZE)
            *slotPtr = PARTY_SIZE + 1;
        else if (*slotPtr == PARTY_SIZE + 1)
            *slotPtr = 0;
        else
        {
            newSlot = GetNewSlotDoubleLayout(*slotPtr, MENU_DIR_DOWN);
			
            if (newSlot == -1)
            {
                if (sPartyMenuInternal->chooseHalf)
                    *slotPtr = PARTY_SIZE;
                else
                    *slotPtr = PARTY_SIZE + 1;
            }
            else
                *slotPtr = newSlot;
        }
        break;
    case MENU_DIR_RIGHT:
        if (*slotPtr == 0)
        {
            if (sPartyMenuInternal->lastSelectedSlot == 3)
            {
                if (GetMonData(&gPlayerParty[3], MON_DATA_SPECIES))
                    *slotPtr = 3;
            }
            else if (GetMonData(&gPlayerParty[2], MON_DATA_SPECIES))
                *slotPtr = 2;
        }
        else if (*slotPtr == 1)
        {
            if (sPartyMenuInternal->lastSelectedSlot == 5)
            {
                if (GetMonData(&gPlayerParty[5], MON_DATA_SPECIES))
                    *slotPtr = 5;
            }
            else if (GetMonData(&gPlayerParty[4], MON_DATA_SPECIES))
                *slotPtr = 4;
        }
        break;
    case MENU_DIR_LEFT:
        if (*slotPtr == 2 || *slotPtr == 3)
        {
            sPartyMenuInternal->lastSelectedSlot = *slotPtr;
            *slotPtr = 0;
        }
        else if (*slotPtr == 4 || *slotPtr == 5)
        {
            sPartyMenuInternal->lastSelectedSlot = *slotPtr;
            *slotPtr = 1;
        }
        break;
    }
}

static void ToggleFieldMoveDescriptionWindow(u8 action)
{
    struct PartyMenuInternal *ptr = sPartyMenuInternal;

    if (action <= MENU_FIELD_MOVES)
    {
        if (ptr->windowId[2] != 0xFF)
        {
            ClearWindowTilemap(ptr->windowId[2]);
            RemoveWindow(ptr->windowId[2]);
            ptr->windowId[2] = 0xFF;
            ScheduleBgCopyTilemapToVram(2);
        }
    }
    else
    {
        if (ptr->windowId[2] == 0xFF)
            ptr->windowId[2] = AddWindow(&gUnknown_845A178);
		
        FillHelpMessageWindow(ptr->windowId[2]);
        AddTextPrinterParameterized4(ptr->windowId[2], 2, 3, 6, GetFontAttribute(2, FONTATTR_LETTER_SPACING), 0, sFontColorTable[5], 0, sHMDescriptionTable[action - MENU_FIELD_MOVES]);
        PutWindowTilemap(ptr->windowId[2]);
        ScheduleBgCopyTilemapToVram(2);
    }
}

static void Task_HandleSelectionMenuInput(u8 taskId)
{
    if (!gPaletteFade.active && !sub_80BF748())
    {
        s8 input;
        s16 *data = gTasks[taskId].data;

        if (sPartyMenuInternal->numActions <= 3)
            input = Menu_ProcessInputNoWrapAround_other();
        else
            input = Menu_ProcessInput_other();
		
        if (data[0] != Menu_GetCursorPos())
            ToggleFieldMoveDescriptionWindow(sPartyMenuInternal->actions[Menu_GetCursorPos()]);
		
        data[0] = Menu_GetCursorPos();
		
        switch (input)
        {
        case MENU_NOTHING_CHOSEN:
            break;
        case MENU_B_PRESSED:
            PlaySE(SE_SELECT);
            PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[2]);
            sCursorOptions[sPartyMenuInternal->actions[sPartyMenuInternal->numActions - 1]].func(taskId);
            break;
        default:
            PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[2]);
            sCursorOptions[sPartyMenuInternal->actions[input]].func(taskId);
            break;
        }
    }
}

static void DisplayCancelChooseMonYesNo(u8 taskId)
{
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    StringExpandPlaceholders(gStringVar4, gUnknown_84176CF);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    gTasks[taskId].func = Task_CancelChooseMonYesNo;
}

static void Task_CancelChooseMonYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleCancelChooseMonYesNoInput;
    }
}

static void Task_HandleCancelChooseMonYesNoInput(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        gPartyMenuUseExitCallback = FALSE;
        gPartyMenu.slotId = PARTY_SIZE + 1;
        ClearSelectedPartyOrder();
        Task_ClosePartyMenu(taskId);
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        Task_ReturnToChooseMonAfterText(taskId);
        break;
    }
}

static bool8 HandleMedicineItemYesNoInput(u8 taskId)
{
	switch (Menu_ProcessInputNoWrapClearOnChoose())
	{
		case 0:
			return TRUE;
		case 1:
		case MENU_B_PRESSED:
		    SetMedicineItemFunc(taskId);
			break;
	}
	return FALSE;
}

static void Task_SwitchHoldItemsPrompt(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DisplayAlreadyHoldingItemSwitchMessage(&gPlayerParty[gPartyMenu.slotId], sPartyMenuItemId, TRUE);
        gTasks[taskId].func = Task_SwitchItemsYesNo;
    }
}

static void Task_SwitchItemsYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleSwitchItemsYesNoInput;
    }
}

static void Task_HandleSwitchItemsYesNoInput(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes, switch items
        RemoveBagItem(gSpecialVar_ItemId, 1);
        
        if (!AddBagItem(sPartyMenuItemId, 1)) // No room to return held item to bag
        {
            AddBagItem(gSpecialVar_ItemId, 1);
            BufferBagFullCantTakeItemMessage(sPartyMenuItemId);
            DisplayPartyMenuMessage(gStringVar4, FALSE);
            gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        }
		else
		{
			GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], gSpecialVar_ItemId);
			
			if (ItemIsMail(gSpecialVar_ItemId)) // Giving mail
			    gTasks[taskId].func = Task_WriteMailToGiveMonAfterText;
			else // Giving item
			{
				DisplaySwitchedHeldItemMessage(gSpecialVar_ItemId, sPartyMenuItemId, TRUE);
				gTasks[taskId].func = Task_UpdateHeldItemSprite;
			}
		}
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1: // No
        gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        break;
    }
}

static void Task_WriteMailToGiveMonAfterText(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        sPartyMenuInternal->exitCallback = CB2_WriteMailToGiveMon;
        Task_ClosePartyMenu(taskId);
    }
}

static void Task_SendMailToPCYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleSendMailToPCYesNoInput;
    }
}

static void Task_HandleSendMailToPCYesNoInput(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes, send to PC
        if (TakeMailFromMon2(&gPlayerParty[gPartyMenu.slotId]) != 0xFF)
        {
            DisplayPartyMenuMessage(gText_MailSentToPC, FALSE);
            gTasks[taskId].func = Task_UpdateHeldItemSprite;
        }
        else
        {
            DisplayPartyMenuMessage(gText_PCMailboxFull, FALSE);
            gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        }
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        DisplayPartyMenuMessage(gText_MailMessageWillBeLost, TRUE);
        gTasks[taskId].func = Task_LoseMailMessageYesNo;
        break;
    }
}

static void Task_LoseMailMessageYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleLoseMailMessageYesNoInput;
    }
}

static void Task_HandleLoseMailMessageYesNoInput(u8 taskId)
{
    u16 item;

    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes, lose mail message
        item = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM);
		
        if (AddBagItem(item, 1))
        {
            TakeMailFromMon(&gPlayerParty[gPartyMenu.slotId]);
            DisplayPartyMenuMessage(gText_MailTakenFromPkmn, FALSE);
            gTasks[taskId].func = Task_UpdateHeldItemSprite;
        }
        else
        {
            BufferBagFullCantTakeItemMessage(item);
            DisplayPartyMenuMessage(gStringVar4, FALSE);
            gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        }
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        break;
    }
}

static void DisplayFieldMoveExitAreaMessage(u8 taskId)
{
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    gTasks[taskId].func = Task_FieldMoveExitAreaYesNo;
}

static void Task_FieldMoveExitAreaYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleFieldMoveExitAreaYesNoInput;
    }
}

static void Task_HandleFieldMoveExitAreaYesNoInput(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        gPartyMenu.exitCallback = CB2_ReturnToField;
        Task_ClosePartyMenu(taskId);
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        gFieldCallback2 = NULL;
        gPostMenuFieldCallback = NULL;
        Task_ReturnToChooseMonAfterText(taskId);
        break;
    }
}

static void CancelParticipationPrompt(u8 taskId)
{
    DisplayPartyMenuMessage(gText_CancelParticipation, TRUE);
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = Task_CancelParticipationYesNo;
}

static void Task_CancelParticipationYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleCancelParticipationYesNoInput;
    }
}

static void Task_HandleCancelParticipationYesNoInput(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        gSpecialVar_0x8004 = PARTY_SIZE + 1;
        Task_ClosePartyMenu(taskId);
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        break;
    }
}

static void Task_ReplaceMoveYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleReplaceMoveYesNoInput;
    }
}

static void Task_ReturnToPartyMenuWhileLearningMove(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (GetMoveSlotToReplace() != MAX_MON_MOVES)
            DisplayPartyMenuForgotMoveMessage(taskId);
        else
            StopLearningMovePrompt(taskId);
    }
}

static void ItemUseCB_ReplaceMove(u8 taskId, UNUSED TaskFunc func)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    u8 moveIdx = GetMoveSlotToReplace();
    
    GetMonNickname(mon, gStringVar1);
    StringCopy(gStringVar2, gBattleMoves[GetMonData(mon, moveIdx + MON_DATA_MOVE1)].name);
    RemoveMonPPBonus(mon, moveIdx);
    SetMonMoveSlot(mon, gPartyMenu.data1, moveIdx);
    Task_LearnedMove(taskId);
}

static void CB2_ReturnToPartyMenuWhileLearningMove(void)
{
    u8 moveIdx = GetMoveSlotToReplace();
    
	if (sMedicineItemData.finalLevel)
		SetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_LEVEL, &sMedicineItemData.finalLevel);
	
    if (gPartyMenu.learnMoveState == 0 && moveIdx != MAX_MON_MOVES)
    {
        StartUseItemAnim_ForgetMoveAndLearnTMorHM(gPartyMenu.slotId, gSpecialVar_ItemId, GetMonData(&gPlayerParty[gPartyMenu.slotId], moveIdx + MON_DATA_MOVE1), CB2_UseTMItemAfterForgetingMove);
        gItemUseCB = ItemUseCB_ReplaceMove;
        gPartyMenu.action = gPartyMenu.learnMoveState;
    }
    else
        InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_CHOOSE_MON, TRUE, PARTY_MSG_NONE, Task_ReturnToPartyMenuWhileLearningMove, gPartyMenu.exitCallback);
}

static void CB2_ShowSummaryScreenToForgetMove(void)
{
    ShowSelectMovePokemonSummaryScreen(gPlayerParty, gPartyMenu.slotId, gPlayerPartyCount - 1, CB2_ReturnToPartyMenuWhileLearningMove, gPartyMenu.data1);
}

static void Task_ShowSummaryScreenToForgetMove(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        sPartyMenuInternal->exitCallback = CB2_ShowSummaryScreenToForgetMove;
        Task_ClosePartyMenu(taskId);
    }
}

static void Task_HandleReplaceMoveYesNoInput(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        DisplayPartyMenuMessage(gText_WhichMoveToForget, TRUE);
        gTasks[taskId].func = Task_ShowSummaryScreenToForgetMove;
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        StopLearningMovePrompt(taskId);
        break;
    }
}

static void StopLearningMovePrompt(u8 taskId)
{
    StringCopy(gStringVar2, gBattleMoves[gPartyMenu.data1].name);
    StringExpandPlaceholders(gStringVar4, gText_StopLearningMove2);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = Task_StopLearningMoveYesNo;
}

static void Task_StopLearningMoveYesNo(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PartyMenuDisplayYesNoMenu();
        gTasks[taskId].func = Task_HandleStopLearningMoveYesNoInput;
    }
}

static void Task_TryLearningNextMoveAfterText(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
        Task_TryLearningNextMove(taskId);
}

static void Task_HandleStopLearningMoveYesNoInput(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];

    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        GetMonNickname(mon, gStringVar1);
        StringCopy(gStringVar2, gBattleMoves[gPartyMenu.data1].name);
        StringExpandPlaceholders(gStringVar4, gText_MoveNotLearned);
        DisplayPartyMenuMessage(gStringVar4, TRUE);
		
        if (gPartyMenu.learnMoveState == 1)
            gTasks[taskId].func = Task_TryLearningNextMoveAfterText;
        else
        {
            if (gPartyMenu.learnMoveState == 2) // never occurs
                gSpecialVar_Result = FALSE;
				
            gTasks[taskId].func = Task_ClosePartyMenuAfterText;
        }
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        // fallthrough
    case 1:
        GetMonNickname(mon, gStringVar1);
        StringCopy(gStringVar2, gBattleMoves[gPartyMenu.data1].name);
        DisplayLearnMoveMessage(gText_PkmnNeedsToReplaceMove);
        gTasks[taskId].func = Task_ReplaceMoveYesNo;
        break;
    }
}

static u8 CheckBattleEntriesAndGetMessage(void)
{
    switch (gPartyMenu.chooseMonsBattleType)
    {
		case CHOOSE_MONS_FOR_UNION_ROOM_BATTLE:
			if (gSelectedOrderFromParty[1] == 0)
				return PARTY_MSG_TWO_MONS_ARE_NEEDED;
			break;
    }
    return 0xFF;
}

static void Task_ValidateChosenHalfParty(u8 taskId)
{
    u8 msgId = CheckBattleEntriesAndGetMessage();

    if (msgId != 0xFF)
    {
        PlaySE(SE_FAILURE);
        DisplayPartyMenuStdMessage(msgId);
        gTasks[taskId].func = Task_ContinueChoosingHalfParty;
    }
    else
    {
        if (gSelectedOrderFromParty[0] != 0)
        {
            PlaySE(SE_SELECT);
            Task_ClosePartyMenu(taskId);
        }
        else
        {
            PlaySE(SE_FAILURE);
            DisplayPartyMenuStdMessage(PARTY_MSG_NO_MON_FOR_BATTLE);
            gTasks[taskId].func = Task_ContinueChoosingHalfParty;
        }
    }
}

static void Task_ContinueChoosingHalfParty(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DisplayPartyMenuStdMessage(PARTY_MSG_CHOOSE_MON);
        gTasks[taskId].func = Task_HandleChooseMonInput;
    }
}

/////////////////////
// PARTY MENU TEXT //
/////////////////////

// Messages of the dialogue and std box

static void PartyMenuPrintText(const u8 *text)
{
    DrawStdFrameWithCustomTileAndPalette(6, FALSE, 0x4F, 13);
    gTextFlags.canABSpeedUpPrint = TRUE;
    AddTextPrinterParameterized2(6, 2, text, GetTextSpeedSetting(), 0, 2, 1, 3);
}

#define tKeepOpen  data[0]

u8 DisplayPartyMenuMessage(const u8 *str, bool8 keepOpen)
{
    u8 taskId;

    PartyMenuPrintText(str);
    taskId = CreateTask(Task_PrintAndWaitForText, 1);
    gTasks[taskId].tKeepOpen = keepOpen;
    return taskId;
}

static void Task_PrintAndWaitForText(u8 taskId)
{
    if (!RunTextPrinters_CheckActive(6))
    {
        if (!gTasks[taskId].tKeepOpen)
        {
            ClearStdWindowAndFrameToTransparent(6, 0);
            ClearWindowTilemap(6);
        }
        DestroyTask(taskId);
    }
}

#undef tKeepOpen

bool8 IsPartyMenuTextPrinterActive(void)
{
    return FuncIsActiveTask(Task_PrintAndWaitForText);
}

static void Task_WaitForLinkAndReturnToChooseMon(u8 taskId)
{
    if (!sub_80BF748())
    {
        DisplayPartyMenuStdMessage(PARTY_MSG_CHOOSE_MON);
        gTasks[taskId].func = Task_HandleChooseMonInput;
    }
}

static void Task_ReturnToChooseMonAfterText(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        ClearStdWindowAndFrameToTransparent(6, 0);
        ClearWindowTilemap(6);
		
        if (MenuHelpers_LinkSomething())
            gTasks[taskId].func = Task_WaitForLinkAndReturnToChooseMon;
        else
        {
            DisplayPartyMenuStdMessage(PARTY_MSG_CHOOSE_MON);
            gTasks[taskId].func = Task_HandleChooseMonInput;
        }
    }
}

static void Task_WaitForLinkAndReturnToChooseMonForUseMedicineItem(u8 taskId)
{
	if (!sub_80BF748())
	{
		DisplayPartyMenuStdMessage(PARTY_MSG_USE_ON_WHICH_MON);
        gTasks[taskId].func = Task_HandleChooseMonInput;
	}
}

static void Task_ReturnToChooseMonForUseMedicineItem(u8 taskId)
{
	if (!IsPartyMenuTextPrinterActive())
    {
        ClearStdWindowAndFrameToTransparent(6, 0);
        ClearWindowTilemap(6);
		gItemUseCB = sMedicineItemData.savedItemUseCB;
		
		if (MenuHelpers_LinkSomething())
			gTasks[taskId].func = Task_WaitForLinkAndReturnToChooseMonForUseMedicineItem;
		else
		{
			DisplayPartyMenuStdMessage(PARTY_MSG_USE_ON_WHICH_MON);
			gTasks[taskId].func = Task_HandleChooseMonInput;
		}
    }
}

static void DisplayGaveHeldItemMessage(struct Pokemon *mon, u16 item, bool8 keepOpen)
{
    GetMonNickname(mon, gStringVar1);
    CopyItemName(item, gStringVar2);
    StringExpandPlaceholders(gStringVar4, gText_PkmnWasGivenItem);
    DisplayPartyMenuMessage(gStringVar4, keepOpen);
    ScheduleBgCopyTilemapToVram(2);
}

static void DisplayTookHeldItemMessage(struct Pokemon *mon, u16 item, bool8 keepOpen)
{
    GetMonNickname(mon, gStringVar1);
    CopyItemName(item, gStringVar2);
    StringExpandPlaceholders(gStringVar4, gText_ReceivedItemFromPkmn);
    DisplayPartyMenuMessage(gStringVar4, keepOpen);
    ScheduleBgCopyTilemapToVram(2);
}

static void DisplayAlreadyHoldingItemSwitchMessage(struct Pokemon *mon, u16 item, bool8 keepOpen)
{
    GetMonNickname(mon, gStringVar1);
    CopyItemName(item, gStringVar2);
    StringExpandPlaceholders(gStringVar4, gText_PkmnAlreadyHoldingItemSwitch);
    DisplayPartyMenuMessage(gStringVar4, keepOpen);
    ScheduleBgCopyTilemapToVram(2);
}

static void DisplaySwitchedHeldItemMessage(u16 item, u16 item2, bool8 keepOpen)
{
    CopyItemName(item, gStringVar1);
    CopyItemName(item2, gStringVar2);
    StringExpandPlaceholders(gStringVar4, gText_SwitchedPkmnItem);
    DisplayPartyMenuMessage(gStringVar4, keepOpen);
    ScheduleBgCopyTilemapToVram(2);
}

static void ItemUseDoEffectsAndDisplayMessage(u8 taskId, const u8 *str)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];

	PlaySE(SE_USE_ITEM);
	RemoveBagItem(gSpecialVar_ItemId, 1);
	
	gPartyMenuUseExitCallback = TRUE;
	GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
	StringExpandPlaceholders(gStringVar4, str);
	DisplayPartyMenuMessage(gStringVar4, TRUE);
	ScheduleBgCopyTilemapToVram(2);
	
	SetMedicineItemFunc(taskId);
}

static void BufferBagFullCantTakeItemMessage(u16 itemId)
{
    const u8 *string;

    switch (ItemId_GetPocket(itemId))
    {
    default:
        string = gStartMenuText_Bag;
        break;
    case POCKET_TM_CASE:
        string = ItemId_GetName(ITEM_TM_CASE);
        break;
    case POCKET_BERRY_POUCH:
        string = ItemId_GetName(ITEM_BERRY_POUCH);
        break;
    }
    StringCopy(gStringVar1, string);
    StringExpandPlaceholders(gStringVar4, gText_BagFullCouldNotRemoveItem);
}

static void DisplayCantUseFlashMessage(void)
{
    if (FlagGet(FLAG_SYS_FLASH_ACTIVE))
        DisplayPartyMenuStdMessage(PARTY_MSG_ALREADY_IN_USE);
    else
        DisplayPartyMenuStdMessage(PARTY_MSG_CANT_USE_HERE);
}

static void Task_WaitFanfareAndDisplayMoveDeletedMessage(u8 taskId)
{
	if (IsFanfareTaskInactive())
	{
		struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	    GetMonNickname(mon, gStringVar1);
		StringCopy(gStringVar2, gBattleMoves[GetMonData(mon, MON_DATA_MOVE1 + gSpecialVar_0x8005)].name);
		DeleteMonMove(mon, gSpecialVar_0x8005);
		StringExpandPlaceholders(gStringVar4, gText_PkmnForgotMove);
		DisplayPartyMenuMessage(gStringVar4, FALSE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
	}
}

static void DisplayHPRestoredMessage(void)
{
	GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_PkmnHPRestoredByVar2);
    DisplayPartyMenuMessage(gStringVar4, FALSE);
    ScheduleBgCopyTilemapToVram(2);
}

static void Task_DisplayHPRestoredMessage(u8 taskId)
{
    DisplayHPRestoredMessage();
    HandleBattleLowHpMusicChange();
	SetMedicineItemFunc(taskId);
}

static void Task_SacredAshDisplayHPRestored(u8 taskId)
{
    DisplayHPRestoredMessage();
    gTasks[taskId].func = Task_SacredAshLoop;
}

static void DisplayLearnMoveMessage(const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    ScheduleBgCopyTilemapToVram(2);
}

static void DisplayLearnMoveMessageAndClose(u8 taskId, const u8 *str)
{
    DisplayLearnMoveMessage(str);
    gTasks[taskId].func = Task_ClosePartyMenuAfterText;
}

static void DisplayMonNeedsToReplaceMove(u8 taskId)
{
    GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
    StringCopy(gStringVar2, gBattleMoves[gMoveToLearn].name);
    StringExpandPlaceholders(gStringVar4, gText_PkmnNeedsToReplaceMove);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    ScheduleBgCopyTilemapToVram(2);
    gPartyMenu.data1 = gMoveToLearn;
    gTasks[taskId].func = Task_ReplaceMoveYesNo;
}

static void DisplayMonLearnedMove(u8 taskId, u16 move)
{
    GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
    StringCopy(gStringVar2, gBattleMoves[move].name);
    StringExpandPlaceholders(gStringVar4, gText_PkmnLearnedMove3);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    ScheduleBgCopyTilemapToVram(2);
    gPartyMenu.data1 = move;
    gTasks[taskId].func = Task_DoLearnedMoveFanfareAfterText;
}

static void Task_DoLearnedMoveFanfareAfterText(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        PlayFanfare(MUS_LEVEL_UP);
        gTasks[taskId].func = Task_LearnNextMoveOrClosePartyMenu;
    }
}

static void Task_LearnNextMoveOrClosePartyMenu(u8 taskId)
{
    if (IsFanfareTaskInactive() && ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & B_BUTTON)))
    {
        if (gPartyMenu.learnMoveState == 1)
            Task_TryLearningNextMove(taskId);
        else
        {
            if (gPartyMenu.learnMoveState == 2) // never occurs
                gSpecialVar_Result = TRUE;
				
            Task_ClosePartyMenu(taskId);
        }
    }
}

static void Task_PartyMenuReplaceMove(u8 taskId)
{
    struct Pokemon *mon;
    u8 oldPP, moveSlot;

    if (!IsPartyMenuTextPrinterActive())
    {
        mon = &gPlayerParty[gPartyMenu.slotId];
		moveSlot = GetMoveSlotToReplace();
		
        RemoveMonPPBonus(mon, moveSlot);
		
        oldPP = GetMonData(mon, MON_DATA_PP1 + moveSlot, NULL);

        SetMonMoveSlot(mon, gPartyMenu.data1, moveSlot);
		
#if DONT_REPLENISH_MOVE_PP
        if (GetMonData(mon, MON_DATA_PP1 + moveSlot, NULL) > oldPP)
            SetMonData(mon, MON_DATA_PP1 + moveSlot, &oldPP);
#endif

        Task_LearnedMove(taskId);
    }
}

static void DisplayPartyMenuForgotMoveMessage(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    GetMonNickname(mon, gStringVar1);
    StringCopy(gStringVar2, gBattleMoves[GetMonData(mon, MON_DATA_MOVE1 + GetMoveSlotToReplace())].name);
    DisplayLearnMoveMessage(gText_12PoofForgotMove);
    gTasks[taskId].func = Task_PartyMenuReplaceMove;
}

static void DisplayItemMustBeRemovedFirstMessage(u8 taskId)
{
    DisplayPartyMenuMessage(gText_RemoveMailBeforeItem, TRUE);
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
}

static bool8 ShouldUseChooseMonText(void)
{
    struct Pokemon *party = gPlayerParty;
    u8 i, numAliveMons = 0;

    if (gPartyMenu.action == PARTY_ACTION_SEND_OUT)
        return TRUE;
	
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        if (GetMonData(&party[i], MON_DATA_SPECIES) && (GetMonData(&party[i], MON_DATA_HP) || GetMonData(&party[i], MON_DATA_IS_EGG)))
            ++numAliveMons;
		
        if (numAliveMons > 1)
            return TRUE;
    }
    return FALSE;
}

void DisplayPartyMenuStdMessage(u32 stringId)
{
    u8 *windowPtr = &sPartyMenuInternal->windowId[1];

    if (*windowPtr != 0xFF)
        PartyMenuRemoveWindow(windowPtr);

    if (stringId != PARTY_MSG_NONE)
    {
        switch (stringId)
        {
        case PARTY_MSG_DO_WHAT_WITH_MON:
            *windowPtr = AddWindow(&sDoWhatWithMonMsgWindowTemplate);
            break;
        case PARTY_MSG_DO_WHAT_WITH_ITEM:
		case PARTY_MSG_DO_WHAT_WITH_MOVES:
            *windowPtr = AddWindow(&sDoWhatWithItemMsgWindowTemplate);
            break;
        case PARTY_MSG_DO_WHAT_WITH_MAIL:
            *windowPtr = AddWindow(&sDoWhatWithMailMsgWindowTemplate);
            break;
        case PARTY_MSG_RESTORE_WHICH_MOVE:
        case PARTY_MSG_BOOST_PP_WHICH_MOVE:
            *windowPtr = AddWindow(&sWhichMoveMsgWindowTemplate);
            break;
        default:
            *windowPtr = AddWindow(&sDefaultPartyMsgWindowTemplate);
            break;
        }

        if (stringId == PARTY_MSG_CHOOSE_MON)
        {
            if (sPartyMenuInternal->chooseHalf)
                stringId = PARTY_MSG_CHOOSE_MON_AND_CONFIRM;
            else if (!ShouldUseChooseMonText())
                stringId = PARTY_MSG_CHOOSE_MON_OR_CANCEL;
        }
        DrawStdFrameWithCustomTileAndPalette(*windowPtr, FALSE, 0x58, 0xF);
        StringExpandPlaceholders(gStringVar4, sActionStringTable[stringId]);
        AddTextPrinterParameterized(*windowPtr, 2, gStringVar4, 0, 2, 0, 0);
        ScheduleBgCopyTilemapToVram(2);
    }
}

///////////////
// HP BUFFER //
///////////////

// Updates the Hp bar graphics

#define tHP           data[0]
#define tMaxHP        data[1]
#define tHPIncrement  data[2]
#define tHPToAdd      data[3]
#define tPartyId      data[4]
#define tStartHP      data[5]

static void Task_PartyMenuModifyHP(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    tHP += tHPIncrement;
    --tHPToAdd;
	
    SetMonData(&gPlayerParty[tPartyId], MON_DATA_HP, &tHP);
    DisplayPartyPokemonHPCheck(&gPlayerParty[tPartyId], &sPartyMenuBoxes[tPartyId], 1);
    DisplayPartyPokemonHPBarCheck(&gPlayerParty[tPartyId], &sPartyMenuBoxes[tPartyId]);
	
    if (tHPToAdd == 0 || tHP == 0 || tHP == tMaxHP)
    {
        // If HP was recovered, buffer the amount recovered
        if (tHP > tStartHP)
            ConvertIntToDecimalStringN(gStringVar2, tHP - tStartHP, STR_CONV_MODE_LEFT_ALIGN, 3);
		
        SwitchTaskToFollowupFunc(taskId);
    }
}

void PartyMenuModifyHP(u8 taskId, u8 slot, s8 hpIncrement, s16 hpDifference, TaskFunc task)
{
    struct Pokemon *mon = &gPlayerParty[slot];
    s16 *data = gTasks[taskId].data;

    tHP = GetMonData(mon, MON_DATA_HP);
    tMaxHP = GetMonData(mon, MON_DATA_MAX_HP);
    tHPIncrement = hpIncrement;
    tHPToAdd = hpDifference;
    tPartyId = slot;
    tStartHP = tHP;
    SetTaskFuncWithFollowupFunc(taskId, Task_PartyMenuModifyHP, task);
}

static void ResetHPTaskData(u8 taskId, u16 hp)
{
	gTasks[taskId].tHP = hp;
	gTasks[taskId].tStartHP = hp;
}

static void UpdateHPBar(u8 spriteId, u16 hp, u16 maxhp)
{
    switch (GetHPBarLevel(hp, maxhp))
    {
    case HP_BAR_FULL:
        SetPartyHPBarSprite(&gSprites[spriteId], 0);
        break;
    case HP_BAR_GREEN:
        SetPartyHPBarSprite(&gSprites[spriteId], 1);
        break;
    case HP_BAR_YELLOW:
        SetPartyHPBarSprite(&gSprites[spriteId], 2);
        break;
    case HP_BAR_RED:
        SetPartyHPBarSprite(&gSprites[spriteId], 3);
        break;
    default:
        SetPartyHPBarSprite(&gSprites[spriteId], 4);
        break;
    }
}

static void UpdatePartyMonHPBar(u8 spriteId, struct Pokemon *mon)
{
    UpdateHPBar(spriteId, GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
}

#undef tHP
#undef tMaxHP
#undef tHPIncrement
#undef tHPToAdd
#undef tPartyId
#undef tStartHP

//////////////////////////////
// TUTORIAL BATTLE MESSAGES //
//////////////////////////////

static void PartyMenu_Oak_PrintText(u8 windowId, const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    gTextFlags.canABSpeedUpPrint = TRUE;
    AddTextPrinterParameterized2(windowId, 4, gStringVar4, GetTextSpeedSetting(), NULL, 2, 1, 3);
}

static u8 FirstBattleEnterParty_CreateWindowAndMsg1Printer(void)
{
    u8 windowId = AddWindow(&sWindowTemplate_FirstBattleOakVoiceover);

    TextWindow_LoadResourcesStdFrame0(windowId, 0x4F, 0xE0);
    DrawDialogFrameWithCustomTileAndPalette(windowId, 1, 0x4F, 0xE);
    PartyMenu_Oak_PrintText(windowId, gText_OakImportantToGetToKnowPokemonThroughly);
	
    return windowId;
}

static void FirstBattleEnterParty_DestroyVoiceoverWindow(u8 windowId)
{
    ClearWindowTilemap(windowId);
    ClearDialogWindowAndFrameToTransparent(windowId, FALSE);
    RemoveWindow(windowId);
    ScheduleBgCopyTilemapToVram(2);
}

static void Task_FirstBattleEnterParty_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_FirstBattleEnterParty_DarkenScreen;
}

static void Task_FirstBattleEnterParty_DarkenScreen(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFF1FFF, 4, 0, 6, RGB_BLACK);
    gTasks[taskId].func = Task_FirstBattleEnterParty_WaitDarken;
}

static void Task_FirstBattleEnterParty_WaitDarken(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_FirstBattleEnterParty_CreatePrinter;
}

static void Task_FirstBattleEnterParty_CreatePrinter(u8 taskId)
{
    gTasks[taskId].data[0] = FirstBattleEnterParty_CreateWindowAndMsg1Printer();
    gTasks[taskId].func = Task_FirstBattleEnterParty_RunPrinterMsg1;
}

static void Task_FirstBattleEnterParty_RunPrinterMsg1(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!RunTextPrinters_CheckActive((u8)data[0]))
        gTasks[taskId].func = Task_FirstBattleEnterParty_LightenFirstMonIcon;
}

static void Task_FirstBattleEnterParty_LightenFirstMonIcon(u8 taskId)
{
    BeginNormalPaletteFade(0xFFFF0008, 4, 6, 0, RGB_BLACK);
    gTasks[taskId].func = Task_FirstBattleEnterParty_WaitLightenFirstMonIcon;
}

static void Task_FirstBattleEnterParty_WaitLightenFirstMonIcon(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_FirstBattleEnterParty_StartPrintMsg2;
}

static void Task_FirstBattleEnterParty_StartPrintMsg2(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    PartyMenu_Oak_PrintText(data[0], gText_OakThisIsListOfPokemon);
    gTasks[taskId].func = Task_FirstBattleEnterParty_RunPrinterMsg2;
}

static void Task_FirstBattleEnterParty_RunPrinterMsg2(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!RunTextPrinters_CheckActive((u8)data[0]))
    {
        FirstBattleEnterParty_DestroyVoiceoverWindow((u8)data[0]);
        gTasks[taskId].func = Task_FirstBattleEnterParty_FadeNormal;
    }
}

static void Task_FirstBattleEnterParty_FadeNormal(u8 taskId)
{
    BeginNormalPaletteFade(0x0000FFF7, 4, 6, 0, RGB_BLACK);
    gTasks[taskId].func = Task_FirstBattleEnterParty_WaitFadeNormal;
}

static void Task_FirstBattleEnterParty_WaitFadeNormal(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        TextWindow_SetUserSelectedFrame(0, 0x4F, 0xD0);
        TextWindow_SetStdFrame0_WithPal(0, 0x58, 0xF0);
		
		DisplayPartyMenuStdMessage(gPartyMenu.action == PARTY_ACTION_USE_ITEM ? PARTY_MSG_USE_ON_WHICH_MON : PARTY_MSG_CHOOSE_MON);
		
        gTasks[taskId].func = Task_HandleChooseMonInput;
    }
}

//////////////////////
// POKEDUDE ACTIONS //
//////////////////////

static void PartyMenuHandlePokedudeCancel(void)
{
    FreeRestoreBattleData();
    LoadPlayerParty();
    SetTeachyTvControllerModeToResume();
    SetMainCallback2(CB2_ReturnToTeachyTV);
}

static bool8 PartyMenuPokedudeIsCancelled(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        sPartyMenuInternal->exitCallback = PartyMenuHandlePokedudeCancel;
        Task_ClosePartyMenu(taskId);
        return TRUE;
    }
    return FALSE;
}

#define tStep data[0]

// Pokedude switches Pokemon
static void Task_PartyMenu_Pokedude(u8 taskId)
{
    gTasks[taskId].tStep = 0;
    gTasks[taskId].func = Task_PartyMenu_PokedudeStep;
}

static void Task_PartyMenu_PokedudeStep(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active && !PartyMenuPokedudeIsCancelled(taskId))
    {
        switch (tStep)
        {
        case 80:
            UpdateCurrentPartySelection(&gPartyMenu.slotId, MENU_DIR_RIGHT);
            break;
        case 160:
            PlaySE(SE_SELECT);
            CreateSelectionWindow();
            break;
        case 240:
            PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[2]);
            sCursorOptions[sPartyMenuInternal->actions[0]].func(taskId);
            break;
        }
        ++tStep;
    }
}

// Pokedude uses item on his own Pokemon
static void Task_PartyMenuFromBag_Pokedude(u8 taskId)
{
    gTasks[taskId].tStep = 0;
    gTasks[taskId].func = Task_PartyMenuFromBag_PokedudeStep;
}

static void Task_PartyMenuFromBag_PokedudeStep(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active && !PartyMenuPokedudeIsCancelled(taskId))
    {
        if (tStep != 80)
            ++tStep;
        else
        {
            sPartyMenuInternal->exitCallback = CB2_SetUpExitToBattleScreen;
            gItemUseCB(taskId, Task_ClosePartyMenuAfterText);
        }
    }
}

#undef tStep

/////////////////////
// ACTIONS SUBMENU //
/////////////////////

static u8 DisplaySelectionWindow(u8 windowType)
{
    struct WindowTemplate window;
    u8 i, cursorDimension, fontAttribute;

    switch (windowType)
    {
    case SELECTWINDOW_ACTIONS:
        window = SetWindowTemplateFields(2, 19, 19 - (sPartyMenuInternal->numActions * 2), 10, sPartyMenuInternal->numActions * 2, 14, 0x2BF);
        break;
    case SELECTWINDOW_ITEM:
        window = sItemGiveTakeWindowTemplate;
        break;
    case SELECTWINDOW_MAIL:
        window = sMailReadTakeWindowTemplate;
        break;
    case SELECTWINDOW_MOVES:
        window = sMoveSelectWindowTemplate;
        break;
	case SELECTWINDOW_MOVE:
	    window = sMoveRelearnDeleteWindowTemplate;
		break;
    }
    sPartyMenuInternal->windowId[0] = AddWindow(&window);
    DrawStdFrameWithCustomTileAndPalette(sPartyMenuInternal->windowId[0], FALSE, 0x4F, 13);
	
    if (windowType == SELECTWINDOW_MOVES)
        return sPartyMenuInternal->windowId[0];
	
    cursorDimension = GetMenuCursorDimensionByFont(2, 0);
    fontAttribute = GetFontAttribute(2, FONTATTR_LETTER_SPACING);
	
    for (i = 0; i < sPartyMenuInternal->numActions; ++i)
        AddTextPrinterParameterized4(sPartyMenuInternal->windowId[0], 2, cursorDimension, (i * 16) + 2, fontAttribute, 0, sFontColorTable[(sPartyMenuInternal->actions[i] >= MENU_FIELD_MOVES) ? 4 : 3], 0, sCursorOptions[sPartyMenuInternal->actions[i]].text);

    Menu_InitCursorInternal(sPartyMenuInternal->windowId[0], 2, 0, 2, 16, sPartyMenuInternal->numActions, 0, 1);
    ScheduleBgCopyTilemapToVram(2);
	
    return sPartyMenuInternal->windowId[0];
}

static void SetPartyMonSelectionActions(struct Pokemon *mons, u8 slotId, u8 action)
{
    u8 i;

    if (action == ACTIONS_NONE)
        SetPartyMonFieldSelectionActions(mons, slotId);
    else
    {
        sPartyMenuInternal->numActions = sPartyMenuActionCounts[action];
		
        for (i = 0; i < sPartyMenuInternal->numActions; ++i)
            sPartyMenuInternal->actions[i] = sPartyMenuActions[action][i];
    }
}

static void AppendToList(u8 *list, u8 *cursor, u8 newEntry)
{
	list[*cursor] = newEntry;
    (*cursor)++;
}

static void SetPartyMonFieldSelectionActions(struct Pokemon *mons, u8 slotId)
{
    u8 i;

    sPartyMenuInternal->numActions = 0;
    AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_SUMMARY);
    
    if (!IsTradedMon(&mons[slotId]))
        AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_NICKNAME);
    
    // Add field moves to action list
	for (i = 0; sFieldMoves[i] != FIELD_MOVE_END; ++i)
	{
		if (FindMoveSlotInMoveset(&mons[slotId], sFieldMoves[i]) != MAX_MON_MOVES)
			AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, i + MENU_FIELD_MOVES);
	}
	
    if (GetMonData(&mons[1], MON_DATA_SPECIES))
        AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_SWITCH);
	
    if (ItemIsMail(GetMonData(&mons[slotId], MON_DATA_HELD_ITEM)))
        AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_MAIL);
    else
        AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_ITEM);
	
	AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_MOVES);
	
    AppendToList(sPartyMenuInternal->actions, &sPartyMenuInternal->numActions, MENU_CANCEL1);
}

static u8 GetPartySlotEntryStatus(s8 slot)
{
    if (!GetBattleEntryEligibility(&gPlayerParty[slot]))
        return 2;
    else if (HasPartySlotAlreadyBeenSelected(slot + 1))
        return 1;
	else
		return 0;
}

static u8 GetPartyMenuActionsTypeInBattle(struct Pokemon *mon)
{
    if (!GetMonData(&gPlayerParty[1], MON_DATA_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG))
        return ACTIONS_SUMMARY_ONLY;
    else if (gPartyMenu.action == PARTY_ACTION_SEND_OUT)
        return ACTIONS_SEND_OUT;
    else
        return ACTIONS_SHIFT;
}

static u8 GetPartyMenuActionsType(struct Pokemon *mon)
{
    u8 actionType;

    switch (gPartyMenu.menuType)
    {
    case PARTY_MENU_TYPE_FIELD:
        if (GetMonData(mon, MON_DATA_IS_EGG))
            actionType = ACTIONS_SWITCH;
        else
            actionType = ACTIONS_NONE; // actions populated by SetPartyMonFieldSelectionActions
        break;
    case PARTY_MENU_TYPE_IN_BATTLE:
        actionType = GetPartyMenuActionsTypeInBattle(mon);
        break;
    case PARTY_MENU_TYPE_CHOOSE_HALF:
        switch (GetPartySlotEntryStatus(gPartyMenu.slotId))
        {
        default: // Not eligible
            actionType = ACTIONS_SUMMARY_ONLY;
            break;
        case 0: // Eligible
            actionType = ACTIONS_ENTER;
            break;
        case 1: // Already selected
            actionType = ACTIONS_NO_ENTRY;
            break;
        }
        break;
    case PARTY_MENU_TYPE_DAYCARE:
        actionType = GetMonData(mon, MON_DATA_IS_EGG) ? ACTIONS_SUMMARY_ONLY : ACTIONS_STORE;
        break;
    case PARTY_MENU_TYPE_UNION_ROOM_REGISTER:
        actionType = ACTIONS_REGISTER;
        break;
    case PARTY_MENU_TYPE_UNION_ROOM_TRADE:
        actionType = ACTIONS_TRADE;
        break;
    // The following have no selection actions (i.e. they exit immediately upon selection)
    default:
        actionType = ACTIONS_NONE;
        break;
    }
    return actionType;
}

static void CreateSelectionWindow(void)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];

    GetMonNickname(mon, gStringVar1);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    SetPartyMonSelectionActions(gPlayerParty, gPartyMenu.slotId, GetPartyMenuActionsType(mon));
    DisplaySelectionWindow(SELECTWINDOW_ACTIONS);
    DisplayPartyMenuStdMessage(PARTY_MSG_DO_WHAT_WITH_MON);
}

static void Task_TryCreateSelectionWindow(u8 taskId)
{
    CreateSelectionWindow();
    gTasks[taskId].data[0] = 0xFF;
    gTasks[taskId].func = Task_HandleSelectionMenuInput;
}

static void ShowMoveSelectWindow(u8 slot)
{
    u8 i, windowId = DisplaySelectionWindow(SELECTWINDOW_MOVES);
    u8 moveCount = 0, fontId = 2;
    u16 move;

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        move = GetMonData(&gPlayerParty[slot], MON_DATA_MOVE1 + i);
		
        AddTextPrinterParameterized(windowId, fontId, gBattleMoves[move].name,
                                    GetFontAttribute(fontId, FONTATTR_MAX_LETTER_WIDTH) + GetFontAttribute(fontId, FONTATTR_LETTER_SPACING),
                                    (i * 16) + 2, TEXT_SPEED_FF, NULL);
									
        if (move)
            ++moveCount;
    }
    Menu_InitCursor(windowId, fontId, 0, 2, 16, moveCount, FALSE);
    ScheduleBgCopyTilemapToVram(2);
}

static void CB2_ShowPokemonSummaryScreen(void)
{
    if (gPartyMenu.menuType == PARTY_MENU_TYPE_IN_BATTLE)
        UpdatePartyToBattleOrder();
	
    ShowPokemonSummaryScreen(gPlayerParty, gPartyMenu.slotId, gPlayerPartyCount - 1, CB2_ReturnToPartyMenuFromSummaryScreen, PSS_MODE_NORMAL);
}

static void CursorCB_Summary(u8 taskId)
{
    PlaySE(SE_SELECT);
    sPartyMenuInternal->exitCallback = CB2_ShowPokemonSummaryScreen;
    Task_ClosePartyMenu(taskId);
}

static void CB2_ChangePokemonNicknamePartyScreen(void)
{
    SetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_NICKNAME, gStringVar2);
    CB2_ReturnToPartyMenuFromSummaryScreen();
}

static void CB2_ShowNicknameScreen(void)
{
    GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_NICKNAME, gStringVar3);
	GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_NICKNAME, gStringVar2);
	DoNamingScreen(NAMING_SCREEN_NAME_RATER, gStringVar2, GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_SPECIES), GetMonGender(&gPlayerParty[gPartyMenu.slotId]), CB2_ChangePokemonNicknamePartyScreen);
}

static void CursorCB_Nickname(u8 taskId)
{
	PlaySE(SE_SELECT);
	sPartyMenuInternal->exitCallback = CB2_ShowNicknameScreen;
	Task_ClosePartyMenu(taskId);
}

static void CursorCB_Moves(u8 taskId)
{
	PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    SetPartyMonSelectionActions(gPlayerParty, gPartyMenu.slotId, ACTIONS_MOVES);
    DisplaySelectionWindow(SELECTWINDOW_MOVE);
    DisplayPartyMenuStdMessage(PARTY_MSG_DO_WHAT_WITH_MOVES);
    gTasks[taskId].data[0] = 0xFF;
    gTasks[taskId].func = Task_HandleSelectionMenuInput;
}

static void CB2_MoveRelearnerFromPartyMenu(void)
{
	ShowMoveTutorMenu(TRUE);
}

static void CursorCB_RelearnMove(u8 taskId)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u8 numRelearnableMoves = GetNumberOfRelearnableMoves(mon);
	
    PlaySE(SE_SELECT);
	
	if (!numRelearnableMoves)
	{
		PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
		PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_PkmnNoMovesToRemember);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
	}
	else
	{
		gSpecialVar_0x8004 = gPartyMenu.slotId;
		gSpecialVar_0x8005 = numRelearnableMoves;
		sPartyMenuInternal->exitCallback = CB2_MoveRelearnerFromPartyMenu;
		Task_ClosePartyMenu(taskId);
	}
}

static void CB2_DeleteMoveFromPartyMenu(void)
{
	ShowSelectMovePokemonSummaryScreen(gPlayerParty, gPartyMenu.slotId, gPlayerPartyCount - 1, CB2_ReturnToPartyMenuFromMoveDelete, 0);
	SetPokemonSummaryScreenMode(PSS_MODE_FORGET_MOVE);
}

static void CursorCB_DeleteMove(u8 taskId)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	
	PlaySE(SE_SELECT);
	
	if (FindMoveSlotInMoveset(mon, MOVE_NONE) == 1) // Only knows one move
	{
		PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
	    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	    GetMonNickname(mon, gStringVar1);
	    StringExpandPlaceholders(gStringVar4, gText_PkmnCantForget);
	    DisplayPartyMenuMessage(gStringVar4, TRUE);
	    ScheduleBgCopyTilemapToVram(2);
	    gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
	}
	else
	{
		sPartyMenuInternal->exitCallback = CB2_DeleteMoveFromPartyMenu;
		Task_ClosePartyMenu(taskId);
	}
}

static void CursorCB_Switch(u8 taskId)
{
    PlaySE(SE_SELECT);
    gPartyMenu.action = PARTY_ACTION_SWITCH;
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    DisplayPartyMenuStdMessage(PARTY_MSG_MOVE_TO_WHERE);
    AnimatePartySlot(gPartyMenu.slotId, 1);
    gPartyMenu.slotId2 = gPartyMenu.slotId;
    gTasks[taskId].func = Task_HandleChooseMonInput;
}

static void CursorCB_Cancel1(u8 taskId)
{
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	DisplayPartyMenuStdMessage(gPartyMenu.menuType == PARTY_MENU_TYPE_DAYCARE ? PARTY_MSG_CHOOSE_MON_2 : PARTY_MSG_CHOOSE_MON);
    gTasks[taskId].func = Task_HandleChooseMonInput;
}

static void CursorCB_Item(u8 taskId)
{
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    SetPartyMonSelectionActions(gPlayerParty, gPartyMenu.slotId, ACTIONS_ITEM);
    DisplaySelectionWindow(SELECTWINDOW_ITEM);
    DisplayPartyMenuStdMessage(PARTY_MSG_DO_WHAT_WITH_ITEM);
    gTasks[taskId].data[0] = 0xFF;
    gTasks[taskId].func = Task_HandleSelectionMenuInput;
}

static void Task_GiveHoldItem(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DisplayGaveHeldItemMessage(&gPlayerParty[gPartyMenu.slotId], gSpecialVar_ItemId, FALSE);
        GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], gSpecialVar_ItemId);
        RemoveBagItem(gSpecialVar_ItemId, 1);
        gTasks[taskId].func = Task_UpdateHeldItemSprite;
    }
}

void CB2_GiveHoldItem(void)
{
    if (!gSpecialVar_ItemId)
		CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_NONE, Task_TryCreateSelectionWindow);
    else
    {
        sPartyMenuItemId = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM);
        
        if (sPartyMenuItemId) // Already holding item
			CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_NONE, Task_SwitchHoldItemsPrompt);
        else if (ItemIsMail(gSpecialVar_ItemId)) // Give mail
        {
            RemoveBagItem(gSpecialVar_ItemId, 1);
            GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], gSpecialVar_ItemId);
            CB2_WriteMailToGiveMon();
        }
        else // Give item
			CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_NONE, Task_GiveHoldItem);
    }
}

static void Task_DisplayGaveMailFromPartyMessage(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (!sPartyMenuItemId)
            DisplayGaveHeldItemMessage(&gPlayerParty[gPartyMenu.slotId], gSpecialVar_ItemId, FALSE);
        else
            DisplaySwitchedHeldItemMessage(gSpecialVar_ItemId, sPartyMenuItemId, FALSE);
		
        gTasks[taskId].func = Task_UpdateHeldItemSprite;
    }
}

static void Task_DisplayGaveMailFromBagMessage(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (sPartyMenuItemId)
            DisplaySwitchedHeldItemMessage(gPartyMenu.bagItem, sPartyMenuItemId, FALSE);
        else
            DisplayGaveHeldItemMessage(&gPlayerParty[gPartyMenu.slotId], gPartyMenu.bagItem, FALSE);
		
        gTasks[taskId].func = Task_UpdateHeldItemSpriteAndClosePartyMenu;
    }
}

static void CB2_ReturnToPartyMenuFromWritingMail(void)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    TaskFunc func;
	
    if (!gSpecialVar_Result) // Canceled writing mail
    {
		u16 item = GetMonData(mon, MON_DATA_HELD_ITEM);
		
        TakeMailFromMon(mon);
        SetMonData(mon, MON_DATA_HELD_ITEM, &sPartyMenuItemId);
        RemoveBagItem(sPartyMenuItemId, 1);
        AddBagItem(item, 1);
		func = Task_TryCreateSelectionWindow;
    }
    else // Wrote mail
        func = Task_DisplayGaveMailFromPartyMessage;
		
	CB2_ReturnToPartyMenuWithFollowUpTask(PARTY_MSG_CHOOSE_MON, func);
}

static void CB2_WriteMailToGiveMon(void)
{
    DoEasyChatScreen(EASY_CHAT_TYPE_MAIL, gSaveBlock1Ptr->mail[GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_MAIL)].words, CB2_ReturnToPartyMenuFromWritingMail);
}

void CB2_SelectBagItemToGive(void)
{
    GoToBagMenu(ITEMMENULOCATION_PARTY, OPEN_BAG_LAST, CB2_GiveHoldItem);
}

static void CursorCB_Give(u8 taskId)
{
    PlaySE(SE_SELECT);
    sPartyMenuInternal->exitCallback = CB2_SelectBagItemToGive;
    Task_ClosePartyMenu(taskId);
}

static void CursorCB_TakeItem(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    u16 item = GetMonData(mon, MON_DATA_HELD_ITEM);

    PlaySE(SE_SELECT);
	
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	
    switch (TryTakeMonItem(mon, item))
    {
    case 0: // Not holding item
        GetMonNickname(mon, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
        DisplayPartyMenuMessage(gStringVar4, TRUE);
        break;
    case 1: // No room to take item
        BufferBagFullCantTakeItemMessage(item);
        DisplayPartyMenuMessage(gStringVar4, TRUE);
        break;
    default: // Took item
        DisplayTookHeldItemMessage(mon, item, TRUE);
        break;
    }
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = Task_UpdateHeldItemSprite;
}

static void Task_MoveItem(u8 taskId)
{
    s8 *slotPtr;
    u8 buffer[100];
    u16 species, newSpecies, item1, item2;
	
    if (gPaletteFade.active || sub_80BF748())
        return;
	
    slotPtr = GetCurrentPartySlotPtr();
	
    switch (PartyMenuButtonHandler(slotPtr))
    {
    case 1: // Selected mon
        if (GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_IS_EGG) || gPartyMenu.slotId == gPartyMenu.slotId2)
		{
			PlaySE(SE_FAILURE);
			return;
		}
		PlaySE(SE_SELECT);
	
		gPartyMenu.action = PARTY_ACTION_CHOOSE_MON;
		
		item1 = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM);
		item2 = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM);
		
		species = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_SPECIES);
	
		if (ItemIsMail(item2))
			TakeMailFromMon(&gPlayerParty[gPartyMenu.slotId2]);
		    
		GiveItemToMon(&gPlayerParty[gPartyMenu.slotId], item2);
		
		gMain.inParty = FALSE; // Temporary disable it for second mon sprite icon reload
		GiveItemToMon(&gPlayerParty[gPartyMenu.slotId2], item1);
		newSpecies = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_SPECIES);
	
		if (newSpecies != species)
		{
			UpdatePartyMonIconSpecies(&sPartyMenuBoxes[gPartyMenu.slotId2], newSpecies); // Update second mon species
			PlayCry1(newSpecies, 0);
		}
	
		gMain.inParty = TRUE;
	
		if (!item2)
			DisplayGaveHeldItemMessage(&gPlayerParty[gPartyMenu.slotId2], item1, TRUE);
		else
		{
			GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
			CopyItemName(item1, gStringVar2);
			StringExpandPlaceholders(buffer, gText_XsYAnd);
			StringAppend(buffer, gText_XsYWereSwapped);
			GetMonNickname(&gPlayerParty[gPartyMenu.slotId2], gStringVar1);
			CopyItemName(item2, gStringVar2);
			StringExpandPlaceholders(gStringVar4, buffer);
			DisplayPartyMenuMessage(gStringVar4, TRUE);
			ScheduleBgCopyTilemapToVram(2);
		}
		AnimatePartySlot(gPartyMenu.slotId2, 0);
		AnimatePartySlot(gPartyMenu.slotId, 1);
		
		gTasks[taskId].func = Task_UpdateHeldItemSpritesAfterMoveItems;
		break;
    case 2: // Selected Cancel
        HandleChooseMonCancel(taskId, slotPtr);
		break;
    }
}

static void CursorCB_MoveItem(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    
    if (GetMonData(mon, MON_DATA_HELD_ITEM))
    {
        gPartyMenu.action = PARTY_ACTION_SWITCH;
		DisplayPartyMenuStdMessage(PARTY_MSG_MOVE_TO_WHERE);
		AnimatePartySlot(gPartyMenu.slotId, 1);
		gPartyMenu.slotId2 = gPartyMenu.slotId;
		gTasks[taskId].func = Task_MoveItem;
    }
    else
    {
        GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_UpdateHeldItemSprite;
    }
}

static void CursorCB_Mail(u8 taskId)
{
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    SetPartyMonSelectionActions(gPlayerParty, gPartyMenu.slotId, ACTIONS_MAIL);
    DisplaySelectionWindow(SELECTWINDOW_MAIL);
    DisplayPartyMenuStdMessage(PARTY_MSG_DO_WHAT_WITH_MAIL);
    gTasks[taskId].data[0] = 0xFF;
    gTasks[taskId].func = Task_HandleSelectionMenuInput;
}

static void CB2_ReadHeldMail(void)
{
    ReadMail(&gSaveBlock1Ptr->mail[GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_MAIL)], CB2_ReturnToPartyMenuFromReadingMail, 1);
}

static void CursorCB_Read(u8 taskId)
{
    PlaySE(SE_SELECT);
    sPartyMenuInternal->exitCallback = CB2_ReadHeldMail;
    Task_ClosePartyMenu(taskId);
}

static void CursorCB_TakeMail(u8 taskId)
{
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    DisplayPartyMenuMessage(gText_SendMailToPC, TRUE);
    gTasks[taskId].func = Task_SendMailToPCYesNo;
}

static void CursorCB_Cancel2(u8 taskId)
{
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    SetPartyMonSelectionActions(gPlayerParty, gPartyMenu.slotId, GetPartyMenuActionsType(&gPlayerParty[gPartyMenu.slotId]));
    DisplaySelectionWindow(SELECTWINDOW_ACTIONS);
    DisplayPartyMenuStdMessage(PARTY_MSG_DO_WHAT_WITH_MON);
    gTasks[taskId].data[0] = 0xFF;
    gTasks[taskId].func = Task_HandleSelectionMenuInput;
}

static bool8 TrySwitchInPokemon(void)
{
    u8 i, newSlot, battlePartySlot, slot = gPartyMenu.slotId;

    if (SelectedPartnerSlotInMulti(slot))
    {
        StringCopy(gStringVar1, GetTrainerPartnerName());
        StringExpandPlaceholders(gStringVar4, gText_CantSwitchWithAlly);
        return FALSE;
    }
    else if (!GetMonData(&gPlayerParty[slot], MON_DATA_HP))
    {
        GetMonNickname(&gPlayerParty[slot], gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnHasNoEnergy);
        return FALSE;
    }
	battlePartySlot = GetPartyIdFromBattleSlot(slot);
	
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (GetBattlerSide(i) == B_SIDE_PLAYER && battlePartySlot == gBattlerPartyIndexes[i])
        {
            GetMonNickname(&gPlayerParty[slot], gStringVar1);
            StringExpandPlaceholders(gStringVar4, gText_PkmnAlreadyInBattle);
            return FALSE;
        }
    }
	
    if (GetMonData(&gPlayerParty[slot], MON_DATA_IS_EGG))
    {
        StringExpandPlaceholders(gStringVar4, gText_EggCantBattle);
        return FALSE;
    }
    else if (battlePartySlot == gBattleStruct->playerPartyIdx)
    {
        GetMonNickname(&gPlayerParty[slot], gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnAlreadySelected);
        return FALSE;
    }
    else if (gPartyMenu.action == PARTY_ACTION_ABILITY_PREVENTS)
    {
        SetMonPreventsSwitchingString();
        return FALSE;
    }
    else if (gPartyMenu.action == PARTY_ACTION_CANT_SWITCH)
    {
        GetMonNickname(&gPlayerParty[GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[gBattlerInMenuId])], gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnCantSwitchOut);
        return FALSE;
    }
	else
	{
		gSelectedMonPartyId = battlePartySlot;
		gPartyMenuUseExitCallback = TRUE;
		newSlot = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[gBattlerInMenuId]);
		SwitchPartyMonSlots(newSlot, slot);
		SwapPartyPokemon(&gPlayerParty[newSlot], &gPlayerParty[slot]);
		return TRUE;
	}
}

static void CursorCB_SendMon(u8 taskId)
{
    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
	
    if (TrySwitchInPokemon())
        Task_ClosePartyMenu(taskId);
    else
    {
        // gStringVar4 below is the error message buffered by TrySwitchInPokemon
        PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
        DisplayPartyMenuMessage(gStringVar4, TRUE);
        gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
    }
}

static void CursorCB_Enter(u8 taskId)
{
    u8 i, maxBattlers;
    const u8 *str;
    
    if (gPartyMenu.chooseMonsBattleType == CHOOSE_MONS_FOR_UNION_ROOM_BATTLE)
    {
        maxBattlers = 2;
        str = gUnknown_8416B3E;
    }
    else
    {
        maxBattlers = 3;
        str = gUnknown_8416B16;
    }
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	
    for (i = 0; i < maxBattlers; ++i)
    {
        if (gSelectedOrderFromParty[i] == 0)
        {
            PlaySE(SE_SELECT);
			
            gSelectedOrderFromParty[i] = gPartyMenu.slotId + 1;
			
            DisplayPartyPokemonDescriptionText(i + PARTYBOX_DESC_FIRST, &sPartyMenuBoxes[gPartyMenu.slotId], 1);
			
            if (i == (maxBattlers - 1))
                MoveCursorToConfirm();
			
            DisplayPartyMenuStdMessage(PARTY_MSG_CHOOSE_MON);
            gTasks[taskId].func = Task_HandleChooseMonInput;
            return;
        }
    }
    PlaySE(SE_FAILURE);
    DisplayPartyMenuMessage(str, TRUE);
    gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
}

static void CursorCB_NoEntry(u8 taskId)
{
    u8 i;

    PlaySE(SE_SELECT);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	
    for (i = 0; i < 3; ++i)
    {
        if (gSelectedOrderFromParty[i] ==  gPartyMenu.slotId + 1)
        {
            gSelectedOrderFromParty[i] = 0;
			
            switch (i)
            {
            case 0:
                gSelectedOrderFromParty[0] = gSelectedOrderFromParty[1];
                gSelectedOrderFromParty[1] = gSelectedOrderFromParty[2];
                gSelectedOrderFromParty[2] = 0;
                break;
            case 1:
                gSelectedOrderFromParty[1] = gSelectedOrderFromParty[2];
                gSelectedOrderFromParty[2] = 0;
                break;
            }
            break;
        }
    }
    DisplayPartyPokemonDescriptionText(PARTYBOX_DESC_ABLE_3, &sPartyMenuBoxes[gPartyMenu.slotId], 1);
	
    if (gSelectedOrderFromParty[0] != 0)
        DisplayPartyPokemonDescriptionText(PARTYBOX_DESC_FIRST, &sPartyMenuBoxes[gSelectedOrderFromParty[0] - 1], 1);
	
    if (gSelectedOrderFromParty[1] != 0)
        DisplayPartyPokemonDescriptionText(1 + PARTYBOX_DESC_FIRST, &sPartyMenuBoxes[gSelectedOrderFromParty[1] - 1], 1);
	
    DisplayPartyMenuStdMessage(PARTY_MSG_CHOOSE_MON);
    gTasks[taskId].func = Task_HandleChooseMonInput;
}

static void CursorCB_Store(u8 taskId)
{
    PlaySE(SE_SELECT);
    gSpecialVar_0x8004 = gPartyMenu.slotId;
    Task_ClosePartyMenu(taskId);
}

// Register mon for the Trading Board in Union Room
static void CursorCB_Register(u8 taskId)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	
    switch (CanRegisterMonForTradingBoard(*(struct GFtgtGnameSub *)GetHostRFUtgtGname(), GetMonData(mon, MON_DATA_SPECIES2), GetMonData(mon, MON_DATA_SPECIES)))
    {
    case CANT_REGISTER_MON:
        StringExpandPlaceholders(gStringVar4, gText_PkmnCantBeTradedNow);
        break;
    case CANT_REGISTER_EGG:
        StringExpandPlaceholders(gStringVar4, gText_EggCantBeTradedNow);
        break;
    default:
        PlaySE(SE_SELECT);
        Task_ClosePartyMenu(taskId);
        return;
    }
    PlaySE(SE_FAILURE);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    StringAppend(gStringVar4, gText_PauseUntilPress);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
}

static void CursorCB_Trade1(u8 taskId)
{
    u16 species2 = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_SPECIES2);
    u16 species = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_SPECIES);
    u32 stringId = GetUnionRoomTradeMessageId(*(struct GFtgtGnameSub *)GetHostRFUtgtGname(), gPartnerTgtGnameSub, species2, gUnionRoomOfferedSpecies, gUnionRoomRequestedMonType, species);

    if (stringId != UR_TRADE_MSG_NONE)
    {
        StringExpandPlaceholders(gStringVar4, sUnionRoomTradeMessages[stringId - 1]);
        PlaySE(SE_FAILURE);
        PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
        PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
        StringAppend(gStringVar4, gText_PauseUntilPress);
        DisplayPartyMenuMessage(gStringVar4, TRUE);
        gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
    }
    else
    {
        PlaySE(SE_SELECT);
        Task_ClosePartyMenu(taskId);
    }
}

static void Task_CancelAfterAorBPress(u8 taskId)
{
    if ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & B_BUTTON))
        CursorCB_Cancel1(taskId);
}

static void CursorCB_FieldMove(u8 taskId)
{
    u8 fieldMove = sPartyMenuInternal->actions[Menu_GetCursorPos()] - MENU_FIELD_MOVES;

    PlaySE(SE_SELECT);
	
    if (sFieldMoveCursorCallbacks[fieldMove].fieldMoveFunc == NULL)
        return;
	
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
	
    if (MenuHelpers_LinkSomething() || InUnionRoom())
    {
        if (fieldMove == FIELD_MOVE_MILK_DRINK || fieldMove == FIELD_MOVE_SOFT_BOILED)
            DisplayPartyMenuStdMessage(PARTY_MSG_CANT_USE_HERE);
        else
            DisplayPartyMenuStdMessage(sFieldMoveCursorCallbacks[fieldMove].msgId);
		
        gTasks[taskId].func = Task_CancelAfterAorBPress;
    }
    else
    {
        // All field moves before WATERFALL are HMs.
        if (fieldMove < FIELD_MOVE_WATERFALL && !FlagGet(FLAG_BADGE01_GET + fieldMove))
        {
            DisplayPartyMenuMessage(gText_CantUseUntilNewBadge, TRUE);
            gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
        }
        else if (sFieldMoveCursorCallbacks[fieldMove].fieldMoveFunc()) // Can use the field move
        {
            switch (fieldMove)
            {
            case FIELD_MOVE_MILK_DRINK:
            case FIELD_MOVE_SOFT_BOILED:
                ChooseMonForSoftboiled(taskId);
                break;
            case FIELD_MOVE_TELEPORT:
                GetMapName(gStringVar1, Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->lastHealLocation.mapGroup, gSaveBlock1Ptr->lastHealLocation.mapNum)->regionMapSectionId);
                StringExpandPlaceholders(gStringVar4, gText_ReturnToHealingSpot);
                DisplayFieldMoveExitAreaMessage(taskId);
                sPartyMenuInternal->data[0] = fieldMove;
                break;
            case FIELD_MOVE_DIG:
                GetMapName(gStringVar1, Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->escapeWarp.mapGroup, gSaveBlock1Ptr->escapeWarp.mapNum)->regionMapSectionId);
                StringExpandPlaceholders(gStringVar4, gText_EscapeFromHereAndReturnTo);
                DisplayFieldMoveExitAreaMessage(taskId);
                sPartyMenuInternal->data[0] = fieldMove;
                break;
            default:
                gPartyMenu.exitCallback = CB2_ReturnToField;
                Task_ClosePartyMenu(taskId);
                break;
            }
        }
        else // Cant use field move
        {
            switch (fieldMove)
            {
            case FIELD_MOVE_FLASH:
                DisplayCantUseFlashMessage();
                break;
            default:
                DisplayPartyMenuStdMessage(sFieldMoveCursorCallbacks[fieldMove].msgId);
                break;
            }
            gTasks[taskId].func = Task_CancelAfterAorBPress;
        }
    }
}

/////////////////////
// SWITCHING SLOTS //
/////////////////////

#define tSlot1Left     data[0]
#define tSlot1Top      data[1]
#define tSlot1Width    data[2]
#define tSlot1Height   data[3]
#define tSlot2Left     data[4]
#define tSlot2Top      data[5]
#define tSlot2Width    data[6]
#define tSlot2Height   data[7]
#define tSlot1Offset   data[8]
#define tSlot2Offset   data[9]
#define tSlot1SlideDir data[10]
#define tSlot2SlideDir data[11]

static void SwitchSelectedMons(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 windowIds[2];

    if (gPartyMenu.slotId2 == gPartyMenu.slotId)
        FinishTwoMonAction(taskId);
    else
    {
        // Initialize switching party mons slide animation
        windowIds[0] = sPartyMenuBoxes[gPartyMenu.slotId].windowId;
        tSlot1Left = GetWindowAttribute(windowIds[0], WINDOW_TILEMAP_LEFT);
        tSlot1Top = GetWindowAttribute(windowIds[0], WINDOW_TILEMAP_TOP);
        tSlot1Width = GetWindowAttribute(windowIds[0], WINDOW_WIDTH);
        tSlot1Height = GetWindowAttribute(windowIds[0], WINDOW_HEIGHT);
        tSlot1Offset = 0;
        if (tSlot1Width == 10)
            tSlot1SlideDir = -1;
        else
            tSlot1SlideDir = 1;
		
        windowIds[1] = sPartyMenuBoxes[gPartyMenu.slotId2].windowId;
        tSlot2Left = GetWindowAttribute(windowIds[1], WINDOW_TILEMAP_LEFT);
        tSlot2Top = GetWindowAttribute(windowIds[1], WINDOW_TILEMAP_TOP);
        tSlot2Width = GetWindowAttribute(windowIds[1], WINDOW_WIDTH);
        tSlot2Height = GetWindowAttribute(windowIds[1], WINDOW_HEIGHT);
        tSlot2Offset = 0;
        if (tSlot2Width == 10)
            tSlot2SlideDir = -1;
        else
            tSlot2SlideDir = 1;
		
        sSlot1TilemapBuffer = Alloc(tSlot1Width * (tSlot1Height << 1));
        sSlot2TilemapBuffer = Alloc(tSlot2Width * (tSlot2Height << 1));
		
        CopyToBufferFromBgTilemap(0, sSlot1TilemapBuffer, tSlot1Left, tSlot1Top, tSlot1Width, tSlot1Height);
        CopyToBufferFromBgTilemap(0, sSlot2TilemapBuffer, tSlot2Left, tSlot2Top, tSlot2Width, tSlot2Height);
        
		ClearWindowTilemap(windowIds[0]);
        ClearWindowTilemap(windowIds[1]);
		
        gPartyMenu.action = PARTY_ACTION_SWITCHING;
		
        AnimatePartySlot(gPartyMenu.slotId, 1);
        AnimatePartySlot(gPartyMenu.slotId2, 1);
		
        SlidePartyMenuBoxOneStep(taskId);
		
        gTasks[taskId].func = Task_SlideSelectedSlotsOffscreen;
    }
}

void SwitchPartyMonSlots(u8 slot, u8 slot2)
{
    u8 partyId = GetPartyIdFromBattleSlot(slot);
    SetPartyIdAtBattleSlot(slot, GetPartyIdFromBattleSlot(slot2));
    SetPartyIdAtBattleSlot(slot2, partyId);
}

// returns FALSE if the slot has slid fully offscreen / back onscreen
static bool8 TryMovePartySlot(s16 x, s16 width, u8 *leftMove, u8 *newX, u8 *newWidth)
{
    if ((x + width) < 0)
        return FALSE;
    else if (x > 31)
        return FALSE;
	
    if (x < 0)
    {
        *leftMove = x * -1;
        *newX = 0;
        *newWidth = width + x;
    }
    else
    {
        *leftMove = 0;
        *newX = x;
        if ((x + width) > 31)
            *newWidth = 32 - x;
        else
            *newWidth = width;
    }
    return TRUE;
}

static void MoveAndBufferPartySlot(const void *rectSrc, s16 x, s16 y, s16 width, s16 height, s16 dir)
{
    // The use of the dimension parameters here is a mess
    u8 leftMove, newX, newWidth; // leftMove is used as a srcX, newX is used as both x and srcHeight, newWidth is used as both width and destY

    if (TryMovePartySlot(x, width, &leftMove, &newX, &newWidth))
    {
        FillBgTilemapBufferRect_Palette0(0, 0, newX, y, newWidth, height);
		
        if (TryMovePartySlot(x + dir, width, &leftMove, &newX, &newWidth))
            CopyRectToBgTilemapBufferRect(0, rectSrc, leftMove, 0, width, height, newX, y, newWidth, height, 17, 0, 0);
    }
}

static void MovePartyMenuBoxSprites(struct PartyMenuBox *menuBox, s16 offset)
{
    gSprites[menuBox->pokeballSpriteId].x2 += offset * 8;
    gSprites[menuBox->itemSpriteId].x2 += offset * 8;
    gSprites[menuBox->monSpriteId].x2 += offset * 8;
    gSprites[menuBox->statusSpriteId].x2 += offset * 8;
}

static void SlidePartyMenuBoxSpritesOneStep(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (tSlot1SlideDir != 0)
        MovePartyMenuBoxSprites(&sPartyMenuBoxes[gPartyMenu.slotId], tSlot1SlideDir);
	
    if (tSlot2SlideDir != 0)
        MovePartyMenuBoxSprites(&sPartyMenuBoxes[gPartyMenu.slotId2], tSlot2SlideDir);
}

static void SlidePartyMenuBoxOneStep(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (tSlot1SlideDir != 0)
        MoveAndBufferPartySlot(sSlot1TilemapBuffer, tSlot1Left + tSlot1Offset, tSlot1Top, tSlot1Width, tSlot1Height, tSlot1SlideDir);
	
    if (tSlot2SlideDir != 0)
        MoveAndBufferPartySlot(sSlot2TilemapBuffer, tSlot2Left + tSlot2Offset, tSlot2Top, tSlot2Width, tSlot2Height, tSlot2SlideDir);
	
    ScheduleBgCopyTilemapToVram(0);
}

static void Task_SlideSelectedSlotsOffscreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 slidingSlotPositions[2];

    SlidePartyMenuBoxOneStep(taskId);
    SlidePartyMenuBoxSpritesOneStep(taskId);
	
    tSlot1Offset += tSlot1SlideDir;
    tSlot2Offset += tSlot2SlideDir;
	
    slidingSlotPositions[0] = tSlot1Left + tSlot1Offset;
    slidingSlotPositions[1] = tSlot2Left + tSlot2Offset;
	
    // Both slots have slid offscreen
    if (slidingSlotPositions[0] > 33 && slidingSlotPositions[1] > 33)
    {
        tSlot1SlideDir *= -1;
        tSlot2SlideDir *= -1;
		
        SwitchPartyMon();
		
        DisplayPartyPokemonData(gPartyMenu.slotId);
        DisplayPartyPokemonData(gPartyMenu.slotId2);
		
        PutWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId].windowId);
        PutWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId2].windowId);
		
        CopyToBufferFromBgTilemap(0, sSlot1TilemapBuffer, tSlot1Left, tSlot1Top, tSlot1Width, tSlot1Height);
        CopyToBufferFromBgTilemap(0, sSlot2TilemapBuffer, tSlot2Left, tSlot2Top, tSlot2Width, tSlot2Height);
		
        ClearWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId].windowId);
        ClearWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId2].windowId);
		
        gTasks[taskId].func = Task_SlideSelectedSlotsOnscreen;
    }
}

static void Task_SlideSelectedSlotsOnscreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    SlidePartyMenuBoxOneStep(taskId);
    SlidePartyMenuBoxSpritesOneStep(taskId);

    // Both slots have slide back onscreen
    if (tSlot1SlideDir == 0 && tSlot2SlideDir == 0)
    {
        PutWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId].windowId);
        PutWindowTilemap(sPartyMenuBoxes[gPartyMenu.slotId2].windowId);
		
        ScheduleBgCopyTilemapToVram(0);
		
        Free(sSlot1TilemapBuffer);
        Free(sSlot2TilemapBuffer);
		
        FinishTwoMonAction(taskId);
    }
    else // Continue sliding
    {
        tSlot1Offset += tSlot1SlideDir;
        tSlot2Offset += tSlot2SlideDir;
		
        if (tSlot1Offset == 0)
            tSlot1SlideDir = 0;
		
        if (tSlot2Offset == 0)
            tSlot2SlideDir = 0;
    }
}

static void SwitchMenuBoxSprites(u8 *spriteIdPtr1, u8 *spriteIdPtr2)
{
    u8 spriteIdBuffer = *spriteIdPtr1;
    u16 xBuffer1, yBuffer1, xBuffer2, yBuffer2;

    *spriteIdPtr1 = *spriteIdPtr2;
    *spriteIdPtr2 = spriteIdBuffer;
	
    xBuffer1 = gSprites[*spriteIdPtr1].x;
    yBuffer1 = gSprites[*spriteIdPtr1].y;
	
    xBuffer2 = gSprites[*spriteIdPtr1].x2;
    yBuffer2 = gSprites[*spriteIdPtr1].y2;
	
    gSprites[*spriteIdPtr1].x = gSprites[*spriteIdPtr2].x;
    gSprites[*spriteIdPtr1].y = gSprites[*spriteIdPtr2].y;
	
    gSprites[*spriteIdPtr1].x2 = gSprites[*spriteIdPtr2].x2;
    gSprites[*spriteIdPtr1].y2 = gSprites[*spriteIdPtr2].y2;
	
    gSprites[*spriteIdPtr2].x = xBuffer1;
    gSprites[*spriteIdPtr2].y = yBuffer1;
	
    gSprites[*spriteIdPtr2].x2 = xBuffer2;
    gSprites[*spriteIdPtr2].y2 = yBuffer2;
}

static void SwitchPartyMon(void)
{
    struct PartyMenuBox *menuBoxes[2];
    struct Pokemon *mon1, *mon2;
    struct Pokemon *monBuffer;

    menuBoxes[0] = &sPartyMenuBoxes[gPartyMenu.slotId];
    menuBoxes[1] = &sPartyMenuBoxes[gPartyMenu.slotId2];
	
    mon1 = &gPlayerParty[gPartyMenu.slotId];
    mon2 = &gPlayerParty[gPartyMenu.slotId2];
	
    monBuffer = Alloc(sizeof(struct Pokemon));
    *monBuffer = *mon1;
    *mon1 = *mon2;
    *mon2 = *monBuffer;
    Free(monBuffer);
	
    SwitchMenuBoxSprites(&menuBoxes[0]->pokeballSpriteId, &menuBoxes[1]->pokeballSpriteId);
    SwitchMenuBoxSprites(&menuBoxes[0]->itemSpriteId, &menuBoxes[1]->itemSpriteId);
    SwitchMenuBoxSprites(&menuBoxes[0]->monSpriteId, &menuBoxes[1]->monSpriteId);
    SwitchMenuBoxSprites(&menuBoxes[0]->statusSpriteId, &menuBoxes[1]->statusSpriteId);
}

// Finish switching mons or using Softboiled
static void FinishTwoMonAction(u8 taskId)
{
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    gPartyMenu.action = PARTY_ACTION_CHOOSE_MON;
    AnimatePartySlot(gPartyMenu.slotId, 0);
    gPartyMenu.slotId = gPartyMenu.slotId2;
    AnimatePartySlot(gPartyMenu.slotId2, 1);
    DisplayPartyMenuStdMessage(PARTY_MSG_CHOOSE_MON);
    gTasks[taskId].func = Task_HandleChooseMonInput;
}

#undef tSlot1Left
#undef tSlot1Top
#undef tSlot1Width
#undef tSlot1Height
#undef tSlot2Left
#undef tSlot2Top
#undef tSlot2Width
#undef tSlot2Height
#undef tSlot1Offset
#undef tSlot2Offset
#undef tSlot1SlideDir
#undef tSlot2SlideDir

//////////////
// ITEM USE //
//////////////

void CB2_ShowPartyMenuForItemUse(void)
{
    MainCallback callback = CB2_ReturnToBagMenu;
    u8 i, partyLayout, menuType, msgId;
    TaskFunc task;

    if (gMain.inBattle)
    {
        menuType = PARTY_MENU_TYPE_IN_BATTLE;
        partyLayout = GetPartyLayoutFromBattleType();
    }
    else
    {
        menuType = PARTY_MENU_TYPE_FIELD;
        partyLayout = PARTY_LAYOUT_SINGLE;
    }

    if (ItemId_GetFieldFunc(gSpecialVar_ItemId) == FieldUseFunc_SacredAsh)
    {
        gPartyMenu.slotId = 0;
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) && !GetMonData(&gPlayerParty[i], MON_DATA_HP))
            {
                gPartyMenu.slotId = i;
                break;
            }
        }
		
        if (ItemId_GetPocket(gSpecialVar_ItemId) == POCKET_BERRY_POUCH)
            callback = CB2_ReturnToBerryPouchMenu;
		
        task = Task_SetSacredAshCB;
        msgId = PARTY_MSG_NONE;
    }
    else
    {
        switch (ItemId_GetPocket(gSpecialVar_ItemId))
        {
        default:
            msgId = PARTY_MSG_USE_ON_WHICH_MON;
            break;
        case POCKET_TM_CASE:
            msgId = PARTY_MSG_TEACH_WHICH_MON;
            callback = CB2_ReturnToTMCaseMenu;
            break;
        case POCKET_BERRY_POUCH:
            msgId = PARTY_MSG_USE_ON_WHICH_MON;
            callback = CB2_ReturnToBerryPouchMenu;
            break;
        }
        task = Task_HandleChooseMonInput;
    }
    InitPartyMenu(menuType, partyLayout, PARTY_ACTION_USE_ITEM, TRUE, msgId, task, callback);
}

u8 GetItemUseAnimFollowUpCBIdByItemType(u16 itemId)
{
	if (ItemId_GetPocket(itemId) == POCKET_TM_CASE)
		return ITEMUSE_FOLLOWUP_TM;
	else if (ItemId_GetFieldFunc(itemId) == FieldUseFunc_EvoItem)
		return ITEMUSE_FOLLOWUP_EVOLUTION_ITEM;
	else
		return ITEMUSE_FOLLOWUP_MEDICINE;
}

static void CB2_DoUseItemAnim(void)
{
	StartUseItemAnim_Normal(gPartyMenu.slotId, gSpecialVar_ItemId, sItemUseAnimFollowUpCallbacks[GetItemUseAnimFollowUpCBIdByItemType(gSpecialVar_ItemId)]);
}

static void Task_DoUseItemAnim(u8 taskId)
{
    sPartyMenuInternal->exitCallback = CB2_DoUseItemAnim;
    Task_ClosePartyMenu(taskId);
}

static void CB2_UseMedicineItem(void)
{
	InitPartyMenu(gPartyMenu.menuType, KEEP_PARTY_LAYOUT, ShouldUseMedicineItemAgain() ? PARTY_ACTION_USE_ITEM : PARTY_ACTION_CHOOSE_MON, gPartyMenu.slotId, PARTY_MSG_NONE, Task_SetSacredAshCB, gPartyMenu.exitCallback);
}

static void CB2_UseTMItem(void)
{
    if (PSA_IsCancelDisabled())
    {
        AdjustFriendship(&gPlayerParty[gPartyMenu.slotId], FRIENDSHIP_EVENT_LEARN_TMHM);
        SetMainCallback2(gPartyMenu.exitCallback);
    }
    else
		CB2_UseMedicineItem();
}

static void CB2_UseTMItemAfterForgetingMove(void)
{
	if (PSA_IsCancelDisabled())
	{
		struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
		u8 moveIdx = GetMoveSlotToReplace();
		
		RemoveMonPPBonus(mon, moveIdx);
		SetMonMoveSlot(mon, ItemId_GetHoldEffectParam(gSpecialVar_ItemId), moveIdx);
		AdjustFriendship(mon, FRIENDSHIP_EVENT_LEARN_TMHM);
		SetMainCallback2(gPartyMenu.exitCallback);
	}
	else
		InitPartyMenu(gPartyMenu.menuType, KEEP_PARTY_LAYOUT, gPartyMenu.action, gPartyMenu.slotId, PARTY_MSG_NONE, Task_SetSacredAshCB, gPartyMenu.exitCallback);
}

static void CB2_UseEvolutionItem(void)
{
	u8 slot = gPartyMenu.slotId;
	gCB2_AfterEvolution = gPartyMenu.exitCallback;
	BeginEvolutionScene(&gPlayerParty[slot], GetEvolutionTargetSpecies(slot, EVO_MODE_ITEM_USE, gSpecialVar_ItemId, NULL), FALSE, slot);
	RemoveBagItem(gSpecialVar_ItemId, 1);
}

static void Task_WaitFormChangeAnim(u8 taskId)
{
	struct Pokemon *mon;
	
	if (IsFormChangeAnimFinished())
	{
		mon = (struct Pokemon*)GetWordTaskArg(taskId, 1);
		
		SetMonData(mon, MON_DATA_SPECIES, &gTasks[taskId].data[0]);
		CalculateMonStats(mon);
		TrySetMonFormChangeCountdown(mon);
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, gText_FormChangeItem);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = Task_TryLearnPostFormeChangeMove;
	}
}

// fusionState's
// 0 = no fusion
// 1 = fuse
// 2 = defuse
static void DoUseItemFormChangeAnim(u8 taskId, struct Pokemon *mon, u16 newSpecies, u8 fusionState)
{
	struct Sprite *sprite1 = &gSprites[sPartyMenuBoxes[gPartyMenu.slotId].monSpriteId];
	struct Sprite *sprite2 = fusionState == 1 ? &gSprites[sPartyMenuBoxes[gPartyMenu.slotId2].monSpriteId] : NULL;
	
	DoFormChangeAnim(ItemId_GetHoldEffectParam(gSpecialVar_ItemId), newSpecies, (fusionState != 0), sprite1, sprite2);
	gTasks[taskId].data[0] = newSpecies;
	SetWordTaskArg(taskId, 1, (u32)mon);
	gTasks[taskId].func = Task_WaitFormChangeAnim;
}

static void Task_SetSacredAshCB(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (gPartyMenu.menuType == PARTY_MENU_TYPE_IN_BATTLE)
            sPartyMenuInternal->exitCallback = CB2_SetUpExitToBattleScreen;
		
        gItemUseCB(taskId, Task_ClosePartyMenuAfterText);
    }
}

// Restore mon's HP
static void ItemUseDoHPHeal(struct Pokemon *mon, u16 oldHP, u16 newHP, u16 healAmount, u16 item, u8 battleMon)
{
	if (battleMon != MAX_BATTLERS_COUNT) // Use HP heal item in battle, hp restore are handled in the battle script
		gBattleMoveDamage = -healAmount;
	else
	{
		if (newHP > GetMonData(mon, MON_DATA_MAX_HP))
			newHP = GetMonData(mon, MON_DATA_MAX_HP);
		
		if (sMedicineItemData.savedItemUseCB == ItemUseCB_Medicine && !gMain.inBattle)
		{
			sMedicineItemData.oldHP = oldHP;
			sMedicineItemData.newHP = newHP;
		}
		else // Basically for Sacred Ash
			SetMonData(mon, MON_DATA_HP, &newHP);
	}
}

// Restore mon's move PP
static bool8 ItemUseRestoreMovePP(struct Pokemon *mon, u8 moveIndex, u8 amount, u8 battleMon)
{
	u16 move = GetMonData(mon, MON_DATA_MOVE1 + moveIndex);
	u8 pp = GetMonData(mon, MON_DATA_PP1 + moveIndex), ppWithBonus = CalculatePPWithBonus(move, GetMonData(mon, MON_DATA_PP_BONUSES), moveIndex);
	
	if (pp != ppWithBonus)
	{
		pp += amount;
		if (pp > ppWithBonus)
			pp = ppWithBonus;
		
		SetMonData(mon, MON_DATA_PP1 + moveIndex, &pp);
		
		if (battleMon != MAX_BATTLERS_COUNT && MOVE_IS_PERMANENT(battleMon, moveIndex))
		    gBattleMons[battleMon].pp[moveIndex] = pp;
		
		return TRUE;
	}
	return FALSE;
}

// Try cure mon's primary status
static bool8 ItemUseTryCureStatus(struct Pokemon *mon, u32 healMask, u8 battlerId)
{
	if (GetMonData(mon, MON_DATA_STATUS) & healMask)
	{
		if (battlerId == MAX_BATTLERS_COUNT) // Heal the status on the battle script otherwise
			sMedicineItemData.healStatusMask = healMask;
			
		return TRUE;
	}
	return FALSE;
}

#define SET_STRING_TO_PRINT(stringId)        \
	if (itemUse->messagePrinted == FALSE)    \
	{                                        \
		itemUse->messagePrinted = TRUE;      \
		itemUse->stringToPrintId = stringId; \
	}

#define SET_STRING_TO_COPY_DATA(stringId, data)  \
	if (itemUse->dataCopied == FALSE)            \
	{                                            \
		itemUse->dataCopied = TRUE;              \
		itemUse->specialStringCopyId = stringId; \
		itemUse->stringCopyData = data;          \
	}

bool8 PokemonUseItemEffects(struct Pokemon *mon, u16 item, u8 partyIndex, u8 battleMonId)
{
	struct ItemUseData *itemUse = AllocZeroed(sizeof(struct ItemUseData));
	const u8 *effectTable = GetItemEffect(item);
	u8 i;
	u16 hp, holdEffectParam;
	bool8 increaseStat, caseWorked, failed = TRUE;
	// datas for general purpose
	u8 byte;
	u16 hword;
	u32 word;
	s8 signedbyte;
	s16 signedhword;
	s32 signedword;
	
	ResetMedicineItemData(GetMonData(mon, MON_DATA_LEVEL));
	
	if (effectTable != NULL)
	{
		holdEffectParam = ItemId_GetHoldEffectParam(item);
		
		for (i = 0; effectTable[i] != ITEMEFFECT_END; i++)
		{
			hp = GetMonData(mon, MON_DATA_HP);
			caseWorked = FALSE; // general purpose case flag
			
			switch (effectTable[i])
			{
				case ITEMEFFECT_HEAL_HP:
					if (hp && hp != GetMonData(mon, MON_DATA_MAX_HP))
					{
						switch (holdEffectParam)
						{
							case 0xFF: // restore full HP
							    word = hword = GetMonData(mon, MON_DATA_MAX_HP);
								break;
							default: // restore HP amount
							    hword = holdEffectParam;
								word = hp + hword;
								break;
						}
						ItemUseDoHPHeal(mon, hp, word, hword, item, battleMonId);
						itemUse->messagePrinted = TRUE; // string are handled separated
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_HEAL_HP_PERCENT:
				    if (hp && hp != GetMonData(mon, MON_DATA_MAX_HP))
					{
						// percent HP amount
						hword = (GetMonData(mon, MON_DATA_MAX_HP) * holdEffectParam) / 100;
						if (hword == 0)
							hword = 1;
						
						ItemUseDoHPHeal(mon, hp, hp + hword, hword, item, battleMonId);
						itemUse->messagePrinted = TRUE; // string are handled separated
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_REVIVE:
				    if (!hp)
					{
						if (battleMonId != MAX_BATTLERS_COUNT)
						{
							if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
								gBattleStruct->usedReviveItemBattler |= gBitTable[gBattlerInMenuId]; // for revive mon in battle
						}
						hword = GetMonData(mon, MON_DATA_MAX_HP) / holdEffectParam;
						if (hword == 0)
							hword = 1;
						
						ItemUseDoHPHeal(mon, hp, hword, hword, item, battleMonId);
						itemUse->messagePrinted = TRUE; // string are handled separated
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_CURE_STATUS:
					byte = effectTable[++i]; // status flag to cure
					
					// stored as bit flags to be able to cure diferent status at a time
					if ((byte & ITEMEFFECT_STATUS_POISON) && ItemUseTryCureStatus(mon, (STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER), battleMonId))
					{
						hword = ITEMUSE_STRING_POISON_CURED;
						caseWorked = TRUE;
					}
					if ((byte & ITEMEFFECT_STATUS_BURN) && ItemUseTryCureStatus(mon, STATUS1_BURN, battleMonId))
					{
						hword = ITEMUSE_STRING_BURN_HEALED;
						caseWorked = TRUE;
					}
					if ((byte & ITEMEFFECT_STATUS_FREEZE) && ItemUseTryCureStatus(mon, STATUS1_FREEZE, battleMonId))
					{
						hword = ITEMUSE_STRING_THAWED;
						caseWorked = TRUE;
					}
					if ((byte & ITEMEFFECT_STATUS_SLEEP) && ItemUseTryCureStatus(mon, STATUS1_SLEEP, battleMonId))
					{
						hword = ITEMUSE_STRING_WOKE_UP;
						caseWorked = TRUE;
					}
					if ((byte & ITEMEFFECT_STATUS_PARALYSIS) && ItemUseTryCureStatus(mon, STATUS1_PARALYSIS, battleMonId))
					{
						hword = ITEMUSE_STRING_PARALYSIS_CURED;
						caseWorked = TRUE;
					}
					// battle status cure effects
					if (battleMonId != MAX_BATTLERS_COUNT)
					{
						if ((byte & ITEMEFFECT_STATUS_CONFUSION) && (gBattleMons[battleMonId].status2 & STATUS2_CONFUSION))
						{
							gBattleMons[battleMonId].status2 &= ~(STATUS2_CONFUSION);
							hword = ITEMUSE_STRING_SNAPPED_CONFUSION;
							caseWorked = TRUE;
						}
						if ((byte & ITEMEFFECT_STATUS_LOVE) && (gBattleMons[battleMonId].status2 & STATUS2_INFATUATION))
						{
							gBattleMons[battleMonId].status2 &= ~(STATUS2_INFATUATION);
							hword = ITEMUSE_STRING_OVER_INFATUATION;
							caseWorked = TRUE;
						}
					}
					if (byte == ITEMEFFECT_STATUS_ALL) // print diferent string if tries to cure all main status + confusion at a time
					    hword = ITEMUSE_STRING_BECAME_HEALTHY;
					
					if (caseWorked)
					{
						SET_STRING_TO_PRINT(hword);
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_CHANGE_FRIENDSHIP:
					byte = effectTable[++i];
					hword = (byte & ITEMEFFECT_FRIENDSHIP_MAIN); // if it's main use
					
					if (!gMain.inBattle && (hword || !failed)) // if it's use ins't the main use(like the Pomeg Berry), only apply it's effect if was't failed
					{
						if ((byte & ITEMEFFECT_FRIENDSHIP_LOW) && GetMonData(mon, MON_DATA_FRIENDSHIP) < 100
						&& ModifyMonFriendship(mon, (signedbyte = effectTable[i + 1])))
							caseWorked = TRUE;
						else if ((byte & ITEMEFFECT_FRIENDSHIP_MID) && GetMonData(mon, MON_DATA_FRIENDSHIP) >= 100
						&& ModifyMonFriendship(mon, (signedbyte = effectTable[i + 2])))
							caseWorked = TRUE;
						else if ((byte & ITEMEFFECT_FRIENDSHIP_HIGH) && GetMonData(mon, MON_DATA_FRIENDSHIP) >= 200
						&& ModifyMonFriendship(mon, (signedbyte = effectTable[i + 3])))
							caseWorked = TRUE;
							
						if (caseWorked && hword)
						{
							SET_STRING_TO_PRINT(ITEMUSE_STRING_FRIENDSHIP_CHANGED);
							SET_STRING_TO_COPY_DATA(ITEMUSE_COPY_INCREASED_OR_DECREASED, signedbyte > 0 ? TRUE : FALSE);
							failed = FALSE;
						}
					}
					i += 3;
					break;
				case ITEMEFFECT_RESTORE_PP:
					if (gPartyMenu.data1 == MAX_MON_MOVES) // Restore all moves PP
					{
						for (byte = 0; byte < MAX_MON_MOVES; byte++)
						{
							if (ItemUseRestoreMovePP(mon, byte, holdEffectParam, battleMonId))
								caseWorked = TRUE;
						}
					}
					else if (ItemUseRestoreMovePP(mon, gPartyMenu.data1, holdEffectParam, battleMonId)) // Restore one move PP
						caseWorked = TRUE;
					
					if (caseWorked)
					{
						SET_STRING_TO_PRINT(ITEMUSE_STRING_PP_RESTORED);
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_UP_DYNAMAX_LEVEL:
				    byte = GetMonData(mon, MON_DATA_DYNAMAX_LEVEL);
				
				    if (byte < MAX_DYNAMAX_LEVEL)
					{
						byte += holdEffectParam;
						if (byte > MAX_DYNAMAX_LEVEL)
							byte = MAX_DYNAMAX_LEVEL;
						SetMonData(mon, MON_DATA_DYNAMAX_LEVEL, &byte);
						
						SET_STRING_TO_PRINT(ITEMUSE_STRING_RAISE_DYNAMAX_LEVEL);
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_CHANGE_EV:
					signedhword = effectTable[++i]; // amount
					hword = GetMonEVCount(mon); // total Evs mon has
					
					if (gMain.inBattle || hword >= MAX_TOTAL_EVS || (holdEffectParam == STAT_HP && GetMonData(mon, MON_DATA_SPECIES) == SPECIES_SHEDINJA))
						break;
					
					byte = GetMonData(mon, MON_DATA_HP_EV + holdEffectParam);
					
					if (byte < MAX_PER_VITAMIN_EVS)
					{
						if (byte + signedhword > MAX_PER_VITAMIN_EVS)
							word = MAX_PER_VITAMIN_EVS - (byte + signedhword) + signedhword;
						else
							word = signedhword;
						
						if (hword + word > MAX_TOTAL_EVS)
							word += MAX_TOTAL_EVS - (hword + word);
						
						byte += word;
						SetMonData(mon, MON_DATA_HP_EV + holdEffectParam, &byte);
						CalculateMonStats(mon);
						
						SET_STRING_TO_PRINT(ITEMUSE_STRING_STAT_CHANGED);
						SET_STRING_TO_COPY_DATA(ITEMUSE_COPY_STAT_NAME, holdEffectParam);
						increaseStat = (signedhword > 0);
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_GIVE_EXPERIENCE:
					if (sMedicineItemData.initialLevel != MAX_LEVEL)
					{
						// save the mon old stats
						GetMonLevelUpWindowStats(mon, sPartyMenuInternal->data);
						
						if (holdEffectParam == 0) // level up
						{
							signedword = ITEMUSE_STRING_LEVELED_UP;
							word = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][sMedicineItemData.initialLevel + 1];
						}
						else // add amount of exp
						{
							signedword = ITEMUSE_STRING_GAINED_EXP;
							
							word = GetMonData(mon, MON_DATA_EXP) + holdEffectParam;
							if (word > gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][MAX_LEVEL])
								word = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][MAX_LEVEL];
						}
						SetMonData(mon, MON_DATA_EXP, &word);
						CalculateMonStats(mon);
						
						// if leveled up gaining a fixed amount of exp, print a diferent string
						if (sMedicineItemData.initialLevel != GetMonData(mon, MON_DATA_LEVEL) && holdEffectParam != 0)
							signedword = ITEMUSE_STRING_GAINED_EXP_LEVELED_UP;
						
						SET_STRING_TO_PRINT(signedword);
						SET_STRING_TO_COPY_DATA(ITEMUSE_COPY_EXP_AND_LEVEL, holdEffectParam);
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_INCREASE_PP:
					signedbyte = GetMonData(mon, MON_DATA_PP_BONUSES);
					word = (signedbyte & gPPUpGetMask[gPartyMenu.data1]) >> (gPartyMenu.data1 * 2);
					hword = GetMonData(mon, MON_DATA_MOVE1 + gPartyMenu.data1); // move id
					byte = CalculatePPWithBonus(hword, signedbyte, gPartyMenu.data1);
					
					if (holdEffectParam) // PP max
					{
						if (word < 3)
						{
							word = (signedbyte & gPPUpSetMask[gPartyMenu.data1]);
							word += gPPUpAddMask[gPartyMenu.data1] * 3;
							caseWorked = TRUE;
						}
					}
					else // PP up
					{
						if (word < 3 && byte > 4)
						{
							word = signedbyte + gPPUpAddMask[gPartyMenu.data1];
							caseWorked = TRUE;
						}
					}
					if (caseWorked)
					{
						SetMonData(mon, MON_DATA_PP_BONUSES, &word);
						word = (CalculatePPWithBonus(hword, word, gPartyMenu.data1) - byte) + GetMonData(mon, MON_DATA_PP1 + gPartyMenu.data1);
						SetMonData(mon, MON_DATA_PP1 + gPartyMenu.data1, &word);
						
						SET_STRING_TO_PRINT(ITEMUSE_STRING_PP_INCREASED);
						failed = FALSE;
					}
					break;
				case ITEMEFFECT_GIVE_GMAX_FACTOR:
					caseWorked = GetMonData(mon, MON_DATA_HAS_GMAX_FACTOR) ^ TRUE; // switch g max factor bit
					SetMonData(mon, MON_DATA_HAS_GMAX_FACTOR, &caseWorked);
					SET_STRING_TO_PRINT(ITEMUSE_STRING_CHANGE_GMAX_FACTOR);
					SET_STRING_TO_COPY_DATA(ITEMUSE_COPY_GAINED_OR_LOSES, caseWorked);
					failed = FALSE;
					break;
				case ITEMEFFECT_SET_TERA_TYPE:
				    if (GetMonData(mon, MON_DATA_TERA_TYPE) != holdEffectParam)
					{
						SetMonData(mon, MON_DATA_TERA_TYPE, &holdEffectParam);
						SET_STRING_TO_PRINT(ITEMUSE_STRING_CHANGED_TERA_TYPE);
						SET_STRING_TO_COPY_DATA(ITEMUSE_COPY_TYPE_NAME, holdEffectParam);
						failed = FALSE;
					}
					break;
			}
		}
	}
	if (!gMain.inBattle && itemUse->stringToPrintId != ITEMUSE_STRING_NOTHING)
	{
		switch (itemUse->specialStringCopyId)
		{
			case ITEMUSE_COPY_STAT_NAME: // copy stat id name
			    StringCopy(gStringVar2, sEvItemUseStatNames[itemUse->stringCopyData]);
				StringCopy(gStringVar3, increaseStat ? gText_Increased : gText_Decreased);
				break;
			case ITEMUSE_COPY_EXP_AND_LEVEL: // copy exp amount and level
			    ConvertIntToDecimalStringN(gStringVar2, sMedicineItemData.finalLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
				ConvertIntToDecimalStringN(gStringVar3, itemUse->stringCopyData, STR_CONV_MODE_LEFT_ALIGN, 5);
				break;
			case ITEMUSE_COPY_GAINED_OR_LOSES: // copy "gained" or "loses"
			    StringCopy(gStringVar2, itemUse->stringCopyData ? gText_Gained : gText_Loses);
				break;
			case ITEMUSE_COPY_TYPE_NAME: // copy type name
			    StringCopy(gStringVar2, gTypesInfo[itemUse->stringCopyData].name);
				break;
			case ITEMUSE_COPY_INCREASED_OR_DECREASED:
			    StringCopy(gStringVar2, itemUse->stringCopyData ? gText_Increased : gText_Decreased);
				break;
		}
		GetMonNickname(mon, gStringVar1);
		StringExpandPlaceholders(gStringVar4, sItemUseStrings[itemUse->stringToPrintId - 1]);
	}
	Free(itemUse);
	
	return failed;
}

static void ItemUseCB_MedicineStep(u8 taskId, TaskFunc func)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u16 newHp = sMedicineItemData.newHP;
	
	gPartyMenuUseExitCallback = TRUE;
	
	if (gSpecialVar_ItemId == ITEM_BLUE_FLUTE || gSpecialVar_ItemId == ITEM_RED_FLUTE || gSpecialVar_ItemId == ITEM_YELLOW_FLUTE)
		PlaySE(SE_GLASS_FLUTE);
	else
		PlaySE(SE_USE_ITEM);
	
	// For pokedude battles
	if (gPartyMenu.action != PARTY_ACTION_REUSABLE_ITEM)
		RemoveBagItem(gSpecialVar_ItemId, 1);
	
	// HP restored
	if (newHp)
		SetMonData(mon, MON_DATA_HP, &newHp);
	
	// Status cured
	if (sMedicineItemData.healStatusMask)
	{
		sMedicineItemData.healStatusMask = (GetMonData(mon, MON_DATA_STATUS) & ~(sMedicineItemData.healStatusMask));
		SetMonData(mon, MON_DATA_STATUS, &sMedicineItemData.healStatusMask);
	}
	UpdatePartyPokemonAilmentGfxAndLevelCheck(mon, gPartyMenu.slotId);
	
	// Slot was revived
	if (sMedicineItemData.oldHP == 0)
		AnimatePartySlot(gPartyMenu.slotId, 1);
	
	if (newHp)
	{
		PartyMenuModifyHP(taskId, gPartyMenu.slotId, 1, GetMonData(mon, MON_DATA_HP) - sMedicineItemData.oldHP, Task_DisplayHPRestoredMessage);
		ResetHPTaskData(taskId, sMedicineItemData.oldHP);
	}
	else
	{
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		
		// Leveled up
		if (sMedicineItemData.initialLevel != GetMonData(mon, MON_DATA_LEVEL))
		{
			sMedicineItemData.finalLevel = GetMonData(mon, MON_DATA_LEVEL);
			
			GetMonLevelUpWindowStats(mon, &sPartyMenuInternal->data[NUM_STATS]);
			PlayFanfareByFanfareNum(0);
			UpdateMonDisplayInfoAfterLevelUp(gPartyMenu.slotId, mon);
			
			gTasks[taskId].func = Task_DisplayLevelUpStatsPg1;
		}
		else
			SetMedicineItemFunc(taskId);
	}
}

void ItemUseCB_Medicine(u8 taskId, TaskFunc func)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    u16 item = gSpecialVar_ItemId;
	
	PlaySE(SE_SELECT);
	
	if (ExecuteTableBasedItemEffect(gPartyMenu.slotId, item))
	{
		sMedicineItemData.initialLevel = sMedicineItemData.finalLevel = 0;
		gPartyMenuUseExitCallback = FALSE;
        DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		SetMedicineItemFunc(taskId);
	}
	else
	{
		if (gMain.inBattle)
		{
			SetBattlerUsedItemForBattleScript(item, FALSE);
			gTasks[taskId].func = Task_ClosePartyMenuAfterText;
		}
		else
		{
			Task_DoUseItemAnim(taskId);
			gItemUseCB = ItemUseCB_MedicineStep;
		}
	}
}

static bool8 ShouldUseMedicineItemAgain(void)
{
#if REPEATED_MEDICINE_USE
	if (gPartyMenu.menuType == PARTY_MENU_TYPE_FIELD && CheckBagHasItem(gSpecialVar_ItemId, 1))
		return TRUE;
#endif
	return FALSE;
}

// Sets return to choose mon or close party menu task based if can or not use an medicine item again
static void SetMedicineItemFunc(u8 taskId)
{
	gTasks[taskId].func = ShouldUseMedicineItemAgain() ? Task_ReturnToChooseMonForUseMedicineItem : Task_ClosePartyMenuAfterText;
}

// Battle scripts called in HandleAction_UseItem
void ItemUseCB_BattleScript(u8 taskId, TaskFunc func)
{
	PlaySE(SE_SELECT);
	
	if (!CanUseItemInBattle(FALSE, gSpecialVar_ItemId))
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
	}
	else
	{
		SetBattlerUsedItemForBattleScript(gSpecialVar_ItemId, FALSE);
		gPartyMenuUseExitCallback = TRUE;
	}
	gTasks[taskId].func = func;
}

void SetBattlerUsedItemForBattleScript(u16 item, bool8 fromBagMenu)
{
	if (fromBagMenu)
		gBattleStruct->itemPartyIndex[gBattlerInMenuId] = gBattlerPartyIndexes[gBattlerInMenuId]; // It's always used on self
	else
		gBattleStruct->itemPartyIndex[gBattlerInMenuId] = GetPartyIdFromBattleSlot(gPartyMenu.slotId);
	
	RemoveBagItem(item, 1);
}

static void ReturnToUseOnWhichMon(u8 taskId)
{
    gTasks[taskId].func = Task_HandleChooseMonInput;
    sPartyMenuInternal->exitCallback = NULL;
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
    DisplayPartyMenuStdMessage(PARTY_MSG_USE_ON_WHICH_MON);
}

static void SetSelectedMoveForItem(u8 taskId)
{
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);
	gPartyMenu.data1 = Menu_GetCursorPos();
	gItemUseCB = ItemUseCB_Medicine;
}

static void Task_HandleWhichMoveInput(u8 taskId)
{
    s8 input = Menu_ProcessInput();

    if (input != MENU_NOTHING_CHOSEN)
    {
        if (input == MENU_B_PRESSED)
        {
            PlaySE(SE_SELECT);
            ReturnToUseOnWhichMon(taskId);
        }
        else
            SetSelectedMoveForItem(taskId);
    }
}

void ItemUseCB_PPRecoveryOneMove(u8 taskId, UNUSED TaskFunc func)
{
	PlaySE(SE_SELECT);
	DisplayPartyMenuStdMessage(PARTY_MSG_RESTORE_WHICH_MOVE);
	ShowMoveSelectWindow(gPartyMenu.slotId);
	gTasks[taskId].func = Task_HandleWhichMoveInput;
}

void ItemUseCB_PPRecoveryAllMoves(u8 taskId, UNUSED TaskFunc func)
{
	gPartyMenu.data1 = MAX_MON_MOVES;
	gItemUseCB = ItemUseCB_Medicine;
}

void ItemUseCB_PPUp(u8 taskId, UNUSED TaskFunc func)
{
    PlaySE(SE_SELECT);
    DisplayPartyMenuStdMessage(PARTY_MSG_BOOST_PP_WHICH_MOVE);
    ShowMoveSelectWindow(gPartyMenu.slotId);
    gTasks[taskId].func = Task_HandleWhichMoveInput;
}

static void Task_LearnedMove(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	
    if (gPartyMenu.learnMoveState == 0)
        AdjustFriendship(mon, FRIENDSHIP_EVENT_LEARN_TMHM);
	
    GetMonNickname(mon, gStringVar1);
    StringCopy(gStringVar2, gBattleMoves[gPartyMenu.data1].name);
    StringExpandPlaceholders(gStringVar4, gText_PkmnLearnedMove3);
    DisplayPartyMenuMessage(gStringVar4, TRUE);
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = Task_DoLearnedMoveFanfareAfterText;
}

static void ItemUseCB_TMStep(u8 taskId, UNUSED TaskFunc func)
{
	Task_LearnedMove(taskId);
}

void ItemUseCB_TM(u8 taskId, UNUSED TaskFunc func)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u16 move = ItemId_GetHoldEffectParam(gSpecialVar_ItemId);
	
    PlaySE(SE_SELECT);
	
    GetMonNickname(mon, gStringVar1);
	StringCopy(gStringVar2, gBattleMoves[move].name);
	
	switch (CanMonLearnTMOrMoveTutor(mon, move, TRUE))
	{
		case CANNOT_LEARN_MOVE:
		    DisplayLearnMoveMessageAndClose(taskId, gText_PkmnCantLearnMove);
			break;
		case ALREADY_KNOWS_MOVE:
		    DisplayLearnMoveMessageAndClose(taskId, gText_PkmnAlreadyKnows);
			break;
		default:
		    gPartyMenu.data1 = move;
			gPartyMenu.learnMoveState = 0;
		
		    if (GiveMoveToMon(mon, move) != MON_HAS_MAX_MOVES)
			{
				Task_DoUseItemAnim(taskId);
				gItemUseCB = ItemUseCB_TMStep;
			}
			else
			{
				DisplayLearnMoveMessage(gText_PkmnNeedsToReplaceMove);
				gTasks[taskId].func = Task_ReplaceMoveYesNo;
			}
			break;
	}
}

#define tUsedOnSlot   data[0]
#define tHadEffect    data[1]
#define tLastSlotUsed data[2]

static void Task_SacredAshLoop(u8 taskId)
{
    if (!IsPartyMenuTextPrinterActive())
    {
        if (sPartyMenuInternal->tUsedOnSlot)
        {
            sPartyMenuInternal->tUsedOnSlot = FALSE;
            sPartyMenuInternal->tLastSlotUsed = gPartyMenu.slotId;
        }
		
        if (++gPartyMenu.slotId == PARTY_SIZE)
        {
            if (!sPartyMenuInternal->tHadEffect)
            {
                gPartyMenuUseExitCallback = FALSE;
                DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
                ScheduleBgCopyTilemapToVram(2);
            }
            else
            {
                gPartyMenuUseExitCallback = TRUE;
                RemoveBagItem(gSpecialVar_ItemId, 1);
            }
            gTasks[taskId].func = Task_ClosePartyMenuAfterText;
            gPartyMenu.slotId = 0;
        }
        else
            UseSacredAsh(taskId);
    }
}

static void UseSacredAsh(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    u16 hp;

    if (!GetMonData(mon, MON_DATA_SPECIES))
        gTasks[taskId].func = Task_SacredAshLoop;
	else
	{
		hp = GetMonData(mon, MON_DATA_HP); // Hp before it was restored
		
		if (PokemonUseItemEffects(mon, gSpecialVar_ItemId, gPartyMenu.slotId, MAX_BATTLERS_COUNT))
			gTasks[taskId].func = Task_SacredAshLoop;
		else
		{
			PlaySE(SE_USE_ITEM);
			
			UpdatePartyPokemonAilmentGfxAndLevelCheck(mon, gPartyMenu.slotId);
			
			AnimatePartySlot(sPartyMenuInternal->tLastSlotUsed, 0);
			AnimatePartySlot(gPartyMenu.slotId, 1);
			
			PartyMenuModifyHP(taskId, gPartyMenu.slotId, 1, GetMonData(mon, MON_DATA_HP) - hp, Task_SacredAshDisplayHPRestored);
			ResetHPTaskData(taskId, hp);
			
			sPartyMenuInternal->tUsedOnSlot = TRUE;
			sPartyMenuInternal->tHadEffect = TRUE;
		}
	}
}

void ItemUseCB_SacredAsh(u8 taskId, UNUSED TaskFunc func)
{
    sPartyMenuInternal->tUsedOnSlot = FALSE;
    sPartyMenuInternal->tHadEffect = FALSE;
    sPartyMenuInternal->tLastSlotUsed = gPartyMenu.slotId;
    UseSacredAsh(taskId);
}

#undef tUsedOnSlot
#undef tHadEffect
#undef tLastSlotUsed

void ItemUseCB_EvolutionStone(u8 taskId, TaskFunc func)
{
    PlaySE(SE_SELECT);
	
	if (!GetEvolutionTargetSpecies(gPartyMenu.slotId, EVO_MODE_ITEM_USE, gSpecialVar_ItemId, NULL))
    {
        gPartyMenuUseExitCallback = FALSE;
        DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
        ScheduleBgCopyTilemapToVram(2);
        gTasks[taskId].func = func;
    }
    else
		Task_DoUseItemAnim(taskId);
}

void ItemUseCB_FormChange(u8 taskId, TaskFunc func)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
    u16 species = GetMonData(mon, MON_DATA_SPECIES), newSpecies = GetMonFormChangeSpecies(mon, species, FORM_CHANGE_USE_ITEM);
	
	PlaySE(SE_SELECT);
	
	if (newSpecies != species)
	{
		switch (gSpecialVar_ItemId)
		{
			case ITEM_GRACIDEA:
			    if (GetDNSTimeLapseDayOrNight() == TIME_NIGHT || GetAilmentFromStatus(GetMonData(mon, MON_DATA_STATUS)) == AILMENT_FRZ)
					goto NO_EFFECT;
				break;
		}
		if (ItemId_GetPocket(gSpecialVar_ItemId) != POCKET_KEY_ITEMS) // comsume items that are not key items, like nectars
			RemoveBagItem(gSpecialVar_ItemId, 1);

		DoUseItemFormChangeAnim(taskId, mon, newSpecies, 0);
		return;
	}
	NO_EFFECT:
	gPartyMenuUseExitCallback = FALSE;
	DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
	ScheduleBgCopyTilemapToVram(2);
	gTasks[taskId].func = func;
}

static void Task_FormChangeListMenu_HandleInput(u8 taskId)
{
	s32 input = ListMenu_ProcessInput(gTasks[taskId].data[14]);
	
    switch (input)
    {
    case -1:
        break;
    case -2:
	    input = 0x7F;
		// fallthrough
	default:
	    gSpecialVar_Result = input;
	    PlaySE(SE_SELECT);
		Task_DestroyListMenu(taskId, FALSE);
        break;
    }
}

static void Task_FormChangeListMenu(u8 taskId)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u16 newSpecies;
	
	switch (gTasks[taskId].data[0])
	{
		case 0:
		    PlaySE(SE_SELECT);
			
			if (GetMonFormChangeSpecies(mon, GetMonData(mon, MON_DATA_SPECIES), FORM_CHANGE_USE_ITEM))
			{
				gPartyMenuUseExitCallback = TRUE;
				GetMonNickname(mon, gStringVar1);
				StringExpandPlaceholders(gStringVar4, gText_ChooseForm);
				DisplayPartyMenuMessage(gStringVar4, TRUE);
				ScheduleBgCopyTilemapToVram(2);
				++gTasks[taskId].data[0];
				return;
			}
			break;
		case 1:
		    if (!IsPartyMenuTextPrinterActive())
			{
				gTasks[InitFormChangeListMenu(gSpecialVar_0x8000)].func = Task_FormChangeListMenu_HandleInput;
				++gTasks[taskId].data[0];
			}
			return;
		case 2:
		    if (FuncIsActiveTask(Task_FormChangeListMenu_HandleInput))
				return;
			
			switch (gSpecialVar_Result)
			{
				case 0x7F:
				    Task_ReturnToChooseMonAfterText(taskId);
				    return;
				default:
				    newSpecies = GetFormChangeListMenuSpecies(gSpecialVar_0x8000);
					
					if (GetMonData(mon, MON_DATA_SPECIES) != newSpecies)
					{
						DoUseItemFormChangeAnim(taskId, mon, newSpecies, 0);
						return;
					}
					break;
			}
			break;
	}
	gPartyMenuUseExitCallback = FALSE;
	DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
	ScheduleBgCopyTilemapToVram(2);
	gTasks[taskId].func = Task_ReturnToChooseMonAfterText;
}

void ItemUseCB_FormChangeListMenu(u8 taskId, TaskFunc func)
{
	gTasks[taskId].data[0] = 0;
	gTasks[taskId].func = Task_FormChangeListMenu;
}

static u16 GetFusionData(u8 caseId, u16 data)
{
	u32 i;
	
	for (i = 0; gFusionsTable[i].masterSpecies != SPECIES_NONE; i++)
	{
		if (gFusionsTable[i].item == gSpecialVar_ItemId)
		{
			gSpecialVar_0x8001 = gFusionsTable[i].id;
			
			switch (caseId)
			{
				case 0: // Is master species
				    if (gFusionsTable[i].masterSpecies == data)
						return TRUE;
					break;
				case 1: // Is fusion species
				    if (gFusionsTable[i].targetSpecies == data)
						return gFusionsTable[i].masterSpecies;
					break;
				case 2: // Is the other species
				    if (gFusionsTable[i].masterSpecies == gSpecialVar_0x8000 && gFusionsTable[i].otherSpecies == data)
						return gFusionsTable[i].targetSpecies;
					break;
			}
		}
	}
	return SPECIES_NONE;
}

static bool8 CheckMonAlreadyFusedWithSpecies(u16 masterSpecies, u16 otherSpecies)
{
	struct Pokemon *mon = GetBaseMonForFusedSpecies();
	
	if (mon != NULL && GetMonData(mon, MON_DATA_SPECIES))
	{
		gPartyMenuUseExitCallback = FALSE;
		GetSpeciesName(gStringVar1, masterSpecies);
		GetSpeciesName(gStringVar2, otherSpecies);
		StringExpandPlaceholders(gStringVar4, gText_AlreadyHaveFused);
		DisplayPartyMenuMessage(gStringVar4, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		return TRUE;
	}
	return FALSE;
}

static void Task_FusionItemStep(u8 taskId)
{
	u16 species, newSpecies;
	
	if (!gPaletteFade.active && !sub_80BF748())
	{
		switch (PartyMenuButtonHandler(GetCurrentPartySlotPtr()))
		{
			case 1:
				PlaySE(SE_SELECT);
				gPartyMenu.action = PARTY_ACTION_CHOOSE_MON;
				AnimatePartySlot(gPartyMenu.slotId, 1);
				AnimatePartySlot(gPartyMenu.slotId2, 0);
				species = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_SPECIES);
				
				if (!GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_IS_EGG) && (newSpecies = GetFusionData(2, species)))
				{
					if (!CheckMonAlreadyFusedWithSpecies(gSpecialVar_0x8000, species))
					{
						DoUseItemFormChangeAnim(taskId, &gPlayerParty[gPartyMenu.slotId], newSpecies, 1);
						return;
					}
					else
						goto END;
				}
				gPartyMenuUseExitCallback = FALSE;
				DisplayPartyMenuMessage(gText_CantFuseWithPokemon, TRUE);
				ScheduleBgCopyTilemapToVram(2);
				// fallthrough
			case 2:
			END:
			    gTasks[taskId].func = Task_ClosePartyMenuAfterText;
				break;
		}
	}
}

void ItemUseCB_Fusion(u8 taskId, TaskFunc func)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u16 masterSpecies, species = GetMonData(mon, MON_DATA_SPECIES);
	
	PlaySE(SE_SELECT);
	
	if (GetFusionData(0, species)) // try fuse
	{
		gSpecialVar_0x8000 = species;
		gPartyMenu.action = PARTY_ACTION_SWITCH;
		DisplayPartyMenuStdMessage(PARTY_MSG_FUSE_WITH_WHICH);
		AnimatePartySlot(gPartyMenu.slotId, 1);
		gPartyMenu.slotId2 = gPartyMenu.slotId;
		gTasks[taskId].func = Task_FusionItemStep;
	}
	else if ((masterSpecies = GetFusionData(1, species)) && GetMonData(GetBaseMonForFusedSpecies(), MON_DATA_SPECIES)) // try defuse
	{
		if (gPlayerPartyCount == PARTY_SIZE)
		{
			GetMonNickname(mon, gStringVar1);
			StringExpandPlaceholders(gStringVar4, gText_NoSpaceForDefuse);
			DisplayPartyMenuMessage(gStringVar4, TRUE);
			ScheduleBgCopyTilemapToVram(2);
			gTasks[taskId].func = func;
		}
		else
			DoUseItemFormChangeAnim(taskId, mon, masterSpecies, 2);
	}
	else
	{
		gPartyMenuUseExitCallback = FALSE;
		DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
		ScheduleBgCopyTilemapToVram(2);
		gTasks[taskId].func = func;
	}
}

#define tStep data[0]

static void ItemUseCB_ChangeAbilityStep(u8 taskId, TaskFunc func)
{
	StringCopy(gStringVar2, gAbilities[sMedicineItemData.stringData].name);
	ItemUseDoEffectsAndDisplayMessage(taskId, gText_AbilityChanged);
}

static void Task_AbilityChange(u8 taskId)
{
	u16 species;
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	bool8 failed, toHidden = ItemId_GetHoldEffectParam(gSpecialVar_ItemId), isHidden = GetMonData(mon, MON_DATA_ABILITY_HIDDEN);
	u8 abilityNum = GetMonData(mon, MON_DATA_ABILITY_NUM), newAbilityNum = abilityNum ^ 1;
	
	switch (sPartyMenuInternal->tStep)
	{
		case 0:
		    PlaySE(SE_SELECT);
			
			species = GetMonData(mon, MON_DATA_SPECIES);
			
			if (!toHidden && (gSpeciesInfo[species].abilities[0] == gSpeciesInfo[species].abilities[1]))
				failed = TRUE; // Ability Capsule fail check
			else if (toHidden && !gSpeciesInfo[species].hiddenAbility)
				failed = TRUE; // Ability Patch fail check
			else
				failed = FALSE;
			
			if (failed)
			{
				gPartyMenuUseExitCallback = FALSE;
				DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
				SetMedicineItemFunc(taskId);
			}
			else
			{
				sMedicineItemData.stringData = GetAbilityBySpecies(species, toHidden ? (isHidden ? 0 : abilityNum) : newAbilityNum, toHidden ? isHidden ^ TRUE : FALSE);
				
				gPartyMenuUseExitCallback = TRUE;
				GetMonNickname(mon, gStringVar1);
				StringCopy(gStringVar2, gAbilities[sMedicineItemData.stringData].name);
				StringExpandPlaceholders(gStringVar4, gText_ChangeAbility);
				DisplayPartyMenuMessage(gStringVar4, TRUE);
				++sPartyMenuInternal->tStep;
			}
			ScheduleBgCopyTilemapToVram(2);
			break;
		case 1:
		    if (!IsPartyMenuTextPrinterActive())
			{
				PartyMenuDisplayYesNoMenu();
				++sPartyMenuInternal->tStep;
			}
			break;
		case 2:
		    if (HandleMedicineItemYesNoInput(taskId))
			{
				if (toHidden) // Toggle betwen ability hidden and first slot
				{
					isHidden ^= TRUE;
					SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &isHidden);
					
					if (!isHidden)
					{
						newAbilityNum = 0; // If changed from hidden, set ability num to 0 by default
						SetMonData(mon, MON_DATA_ABILITY_NUM, &newAbilityNum);
					}
				}
				else
					SetMonData(mon, MON_DATA_ABILITY_NUM, &newAbilityNum);
				
				Task_DoUseItemAnim(taskId);
				gItemUseCB = ItemUseCB_ChangeAbilityStep;
			}
			break;
	}
}

void ItemUseCB_ChangeAbility(u8 taskId, TaskFunc func)
{
	ResetMedicineItemData(0);
	sPartyMenuInternal->tStep = 0;
	gTasks[taskId].func = Task_AbilityChange;
}

static void ItemUseCB_NatureChangeStep(u8 taskId, TaskFunc func)
{
	StringCopy(gStringVar2, gNaturesInfo[sMedicineItemData.stringData].name);
	ItemUseDoEffectsAndDisplayMessage(taskId, gText_NatureChanged);
}

static void Task_NatureChange(u8 taskId)
{
	struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];
	u8 newNature = ItemId_GetHoldEffectParam(gSpecialVar_ItemId);
	
	switch (sPartyMenuInternal->tStep)
	{
		case 0:
		    PlaySE(SE_SELECT);
			
		    if (GetMonData(mon, MON_DATA_NATURE) == newNature)
			{
				gPartyMenuUseExitCallback = FALSE;
				DisplayPartyMenuMessage(gText_WontHaveEffect, TRUE);
				SetMedicineItemFunc(taskId);
			}
			else
			{
				sMedicineItemData.stringData = newNature;
				
				gPartyMenuUseExitCallback = TRUE;
				GetMonNickname(mon, gStringVar1);
				StringCopy(gStringVar2, gNaturesInfo[newNature].name);
				StringExpandPlaceholders(gStringVar4, gText_ChangeNature);
				DisplayPartyMenuMessage(gStringVar4, TRUE);
				++sPartyMenuInternal->tStep;
			}
			ScheduleBgCopyTilemapToVram(2);
			break;
		case 1:
		    if (!IsPartyMenuTextPrinterActive())
			{
				PartyMenuDisplayYesNoMenu();
				++sPartyMenuInternal->tStep;
			}
			break;
		case 2:
			if (HandleMedicineItemYesNoInput(taskId))
			{
				SetMonData(mon, MON_DATA_NATURE, &newNature);
				CalculateMonStats(mon);
				
				Task_DoUseItemAnim(taskId);
				gItemUseCB = ItemUseCB_NatureChangeStep;
			}
			break;
	}
}

void ItemUseCB_Mint(u8 taskId, TaskFunc func)
{
	ResetMedicineItemData(0);
	sPartyMenuInternal->tStep = 0;
	gTasks[taskId].func = Task_NatureChange;
}

#undef tStep

//////////////////////
// EXTERN FUNCTIONS //
//////////////////////

// Giving an item by selecting Give from the bag menu
// As opposted to by selecting Give in the party menu, which is handled by CursorCB_Give
void CB2_ChooseMonToGiveItem(void)
{
    MainCallback callback;

    switch (ItemId_GetPocket(gSpecialVar_ItemId))
    {
    default:
        callback = CB2_ReturnToBagMenu;
        break;
    case POCKET_TM_CASE:
        callback = CB2_ReturnToTMCaseMenu;
        break;
    case POCKET_BERRY_POUCH:
        callback = CB2_ReturnToBerryPouchMenu;
        break;
    }
    InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_GIVE_ITEM, FALSE, PARTY_MSG_GIVE_TO_WHICH_MON, Task_HandleChooseMonInput, callback);
    gPartyMenu.bagItem = gSpecialVar_ItemId;
}

static void CB2_ReturnToBagMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_LAST, OPEN_BAG_LAST, NULL);
}

static void CB2_ReturnToTMCaseMenu(void)
{
    InitTMCase(TMCASE_NA, NULL, 0xFF);
}

static void CB2_ReturnToBerryPouchMenu(void)
{
    InitBerryPouch(BERRYPOUCH_NA, NULL, 0xFF);
}

/////////////////
// PARTY ORDER //
/////////////////

static void BufferBattlePartyCurrentOrderInternal(u8 *partyBattleOrder, u8 flankId, u8 leftBattler, u8 rightBattler)
{
	u8 i, j, partyIds[PARTY_SIZE];

    if (IsMultiBattle())
    {
        // Party ids are packed in 4 bits at a time
        // i.e. the party id order below would be 0, 3, 5, 4, 2, 1, and the two parties would be 0,5,4 and 3,2,1
        if (flankId != 0)
        {
            partyBattleOrder[0] = 0 | (3 << 4);
            partyBattleOrder[1] = 5 | (4 << 4);
            partyBattleOrder[2] = 2 | (1 << 4);
        }
        else
        {
            partyBattleOrder[0] = 3 | (0 << 4);
            partyBattleOrder[1] = 2 | (1 << 4);
            partyBattleOrder[2] = 5 | (4 << 4);
        }
        return;
    }
    else if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        partyIds[0] = gBattlerPartyIndexes[leftBattler];
		
        for (i = 0, j = 1; i < PARTY_SIZE; ++i)
        {
            if (i != partyIds[0])
            {
                partyIds[j] = i;
                ++j;
            }
        }
    }
    else
    {
        partyIds[0] = gBattlerPartyIndexes[leftBattler];
        partyIds[1] = gBattlerPartyIndexes[rightBattler];
		
        for (i = 0, j = 2; i < PARTY_SIZE; ++i)
        {
            if (i != partyIds[0] && i != partyIds[1])
            {
                partyIds[j] = i;
                ++j;
            }
        }
    }
    for (i = 0; i < ARRAY_COUNT(gBattlePartyCurrentOrder); ++i)
        partyBattleOrder[i] = (partyIds[0 + (i * 2)] << 4) | partyIds[1 + (i * 2)];
}

// This is only used for the player party
void BufferBattlePartyCurrentOrder(void)
{
    BufferBattlePartyCurrentOrderInternal(gBattlePartyCurrentOrder, GetPlayerFlankId(), GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
}

void BufferBattlePartyCurrentOrderBySide(u8 battlerId, u8 flankId)
{
	u8 leftBattler, rightBattler;
	
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
	{
		leftBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
		rightBattler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
	}
	else
	{
		leftBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        rightBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}
	BufferBattlePartyCurrentOrderInternal(gBattleStruct->battlerPartyOrders[battlerId], flankId, leftBattler, rightBattler);
}

void SwitchPartyOrderLinkMulti(u8 battlerId, u8 slot, u8 slot2)
{
    u8 i, j, partyIds[PARTY_SIZE];
    u8 partyIdBuffer, tempSlot = 0;
    u8 *partyBattleOrder;

    if (IsMultiBattle())
    {
        partyBattleOrder = gBattleStruct->battlerPartyOrders[battlerId];
		
        for (i = j = 0; i < ARRAY_COUNT(gBattlePartyCurrentOrder); ++j, ++i)
        {
            partyIds[j] = partyBattleOrder[i] >> 4;
            ++j;
            partyIds[j] = partyBattleOrder[i] & 0xF;
        }
        partyIdBuffer = partyIds[slot2];
		
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (partyIds[i] == slot)
            {
                tempSlot = partyIds[i];
                partyIds[i] = partyIdBuffer;
                break;
            }
        }
		
        if (i != PARTY_SIZE)
        {
            partyIds[slot2] = tempSlot;
            partyBattleOrder[0] = (partyIds[0] << 4) | partyIds[1];
            partyBattleOrder[1] = (partyIds[2] << 4) | partyIds[3];
            partyBattleOrder[2] = (partyIds[4] << 4) | partyIds[5];
        }
    }
}

u8 GetPartyIdFromBattleSlot(u8 slot)
{
	u8 partyId = gBattlePartyCurrentOrder[slot / 2];
	return (slot & 1) ? partyId & 0xF : partyId >> 4;
}

static void SetPartyIdAtBattleSlot(u8 slot, u8 setVal)
{	
    bool8 modResult = (slot & 1);
	u8 partyId;
	
    slot /= 2;
	partyId = gBattlePartyCurrentOrder[slot];
	
	gBattlePartyCurrentOrder[slot] = modResult ? (partyId & 0xF0) | setVal : (partyId & 0xF) | (setVal << 4);
}

u8 GetPartyIdFromBattlePartyId(u8 battlePartyId)
{
    u8 i, j;

    for (j = i = 0; i < ARRAY_COUNT(gBattlePartyCurrentOrder); ++j, ++i)
    {
        if ((gBattlePartyCurrentOrder[i] >> 4) != battlePartyId)
        {
            ++j;
            if ((gBattlePartyCurrentOrder[i] & 0xF) == battlePartyId)
                return j;
        }
        else
            return j;
    }
    return 0;
}

static void UpdatePartyToBattleOrder(void)
{
    struct Pokemon *partyBuffer = Alloc(sizeof(gPlayerParty));
    u8 i;

    memcpy(partyBuffer, gPlayerParty, sizeof(gPlayerParty));
	
    for (i = 0; i < PARTY_SIZE; ++i)
        memcpy(&gPlayerParty[GetPartyIdFromBattlePartyId(i)], &partyBuffer[i], sizeof(struct Pokemon));
	
    Free(partyBuffer);
}

static void UpdatePartyToFieldOrder(void)
{
    struct Pokemon *partyBuffer = Alloc(sizeof(gPlayerParty));
    u8 i;

    memcpy(partyBuffer, gPlayerParty, sizeof(gPlayerParty));
	
    for (i = 0; i < PARTY_SIZE; ++i)
        memcpy(&gPlayerParty[GetPartyIdFromBattleSlot(i)], &partyBuffer[i], sizeof(struct Pokemon));
	
    Free(partyBuffer);
}

//////////////////////////////////
// MULTI PARTNER PARTY SLIDE IN //
//////////////////////////////////

#define tXPos  data[0]

static void Task_InitMultiPartnerPartySlideIn(u8 taskId)
{
    // The first slide step also sets the sprites offscreen
    gTasks[taskId].tXPos = 256;
    SlideMultiPartyMenuBoxSpritesOneStep(taskId);
    ChangeBgX(2, 0x10000, 0);
    gTasks[taskId].func = Task_MultiPartnerPartySlideIn;
}

static void Task_MultiPartnerPartySlideIn(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i;

    if (!gPaletteFade.active)
    {
        tXPos -= 8;
        SlideMultiPartyMenuBoxSpritesOneStep(taskId);
		
        if (tXPos == 0)
        {
            for (i = 3; i < PARTY_SIZE; ++i)
            {
                if (gMultiPartnerParty[i - MULTI_PARTY_SIZE].species)
                    AnimateSelectedPartyIcon(sPartyMenuBoxes[i].monSpriteId, 0);
            }
            PlaySE(SE_M_HARDEN); // The Harden SE plays once the partners party mons have slid on screen
            gTasks[taskId].func = Task_WaitAfterMultiPartnerPartySlideIn;
        }
    }
}

static void Task_WaitAfterMultiPartnerPartySlideIn(u8 taskId)
{
    // data[0] used as a timer afterwards rather than the x pos
    if (++ gTasks[taskId].data[0] == 256)
        Task_ClosePartyMenu(taskId);
}

static void MoveMultiPartyMenuBoxSprite(u8 spriteId, s16 x)
{
    if (x >= 0)
        gSprites[spriteId].x2 = x;
}

static void SlideMultiPartyMenuBoxSpritesOneStep(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i;

    for (i = 3; i < PARTY_SIZE; ++i)
    {
        if (gMultiPartnerParty[i - MULTI_PARTY_SIZE].species)
        {
            MoveMultiPartyMenuBoxSprite(sPartyMenuBoxes[i].monSpriteId, tXPos - 8);
            MoveMultiPartyMenuBoxSprite(sPartyMenuBoxes[i].itemSpriteId, tXPos - 8);
            MoveMultiPartyMenuBoxSprite(sPartyMenuBoxes[i].pokeballSpriteId, tXPos - 8);
            MoveMultiPartyMenuBoxSprite(sPartyMenuBoxes[i].statusSpriteId, tXPos - 8);
        }
    }
    ChangeBgX(2, 0x800, 1);
}

#undef tXpos
