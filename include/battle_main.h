#ifndef GUARD_BATTLE_MAIN_H
#define GUARD_BATTLE_MAIN_H

#include "math_util.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"

#define INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER Bit(0)
#define INTRO_SLIDE_FLAG_SLIDE_IN_STARTED Bit(1)

// defines for the type effectiveness multipliers
#define TYPE_MUL_NO_EFFECT       UQ_4_12(0.0)
#define TYPE_MUL_NOT_EFFECTIVE   UQ_4_12(0.5)
#define TYPE_MUL_NORMAL          UQ_4_12(1.0)
#define TYPE_MUL_SUPER_EFFECTIVE UQ_4_12(2.0)

// defines for the 'DoBounceEffect' function
#define BOUNCE_MON       0x0
#define BOUNCE_HEALTHBOX 0x1

// Return values of GetWhoStrikesFirst
enum
{
    BATTLER1_STRIKES_FIRST,
    BATTLER2_STRIKES_FIRST,
    SPEED_TIE,
};

struct NaturalGift
{
    u8 type;
    u8 power;
};

struct NonVolatileStatus
{
    const u8 *placeholder;
    u8 ailmentId;
    u8 healthboxStatusGfx;
    u16 healthboxStatusPal;
    u8 animationId;
    u8 statusCuredByItemMsgId;
    u8 statusCuredByMoveMultistringId;
};

struct TrainerClass
{
    u8 name[13];
    u8 moneyMultiplier;
    u8 pokeball;
};

struct MultiBattlePokemonTx
{
    /*0x00*/ u16 species;
    /*0x02*/ u16 heldItem;
    /*0x04*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
    /*0x11*/ u8 level;
    /*0x12*/ u16 hp;
    /*0x14*/ u16 maxhp;
    /*0x16*/ u8 gender;
    /*0x17*/ u8 language;
    /*0x18*/ u32 personality;
    /*0x1C*/ struct Status1 status;
};

extern const struct OamData gOamData_BattlerOpponent;
extern const struct OamData gOamData_BattlerPlayer;
extern const u8 gCategoryNames[NUM_MOVE_SPLITS][CATEGORY_NAME_LENGTH + 1];
extern const u32 gTypeEffectivenessTable[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES];
extern const struct TrainerClass gTrainerClassTable[];
extern const struct Ability gAbilities[ABILITIES_COUNT];
extern const struct NaturalGift gNaturalGiftTable[ITEM_TO_BERRY(BERRY_ITEMS_END) + 1];
extern const struct NonVolatileStatus gNonVolatileStatusConditions[];

void CB2_InitBattle(void);
void BattleMainCB2(void);
void CB2_SosCall(void);
void FreeRestoreBattleData(void);
void VBlankCB_Battle(void);
void SpriteCB_VsLetterInit(struct Sprite *sprite);
void CB2_InitEndLinkBattle(void);
u32 GetBattleBgAttribute(u32 arrayId, u32 caseId);
void SpriteCB_EnemyMon(struct Sprite *sprite);
void SpriteCallbackDummy2(struct Sprite *sprite);
void SpriteCB_FaintOpponentMon(struct Sprite *sprite);
void SpriteCb_ShowAsMoveTarget(struct Sprite *sprite);
void SpriteCb_HideAsMoveTarget(struct Sprite *sprite);
void SpriteCB_AllyMon(struct Sprite *sprite);
void SpriteCB_SetToDummy3(struct Sprite *sprite);
void SpriteCB_FaintSlideAnim(struct Sprite *sprite);
void DoBounceEffect(u32 battler, u32 which, s8 delta, s8 amplitude);
void EndBounceEffect(u32 battler, u32 which);
void SpriteCB_PlayerThrowInit(struct Sprite *sprite);
void UpdatePlayerPosInThrowAnim(struct Sprite *sprite);
void BattleDummy(void);
void BeginBattleIntro(void);
void SwitchInClearSetData(u32 battlerId);
void FaintClearSetData(u32 battlerId);
void BattleTurnPassed(void);
u32 IsRunningFromBattleImpossible(u32 battlerId, bool32 checkIngrain);
void UpdatePartyOwnerOnSwitch_NonMulti(u32 battler);
void SwitchPartyOrderInGameMulti(u32 battler, u32 monToSwitchIntoId);
void SwapTurnOrder(u32 id1, u32 id2);
u32 GetWhoStrikesFirst(u32 battler1, u32 battler2, bool32 ignoreChosenMoves);
s8 GetMovePriority(u32 battler, u32 move);
u32 GetBattlerTotalSpeed(u32 battler);
void RunBattleScriptCommands_PopCallbacksStack(void);
void RunBattleScriptCommands(void);
bool32 TryRunFromBattle(u32 battler);
void CB2_QuitRecordedBattle(void);
void FillPartnerParty(u32 trainerId);

#endif // GUARD_BATTLE_MAIN_H
