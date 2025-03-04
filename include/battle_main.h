#ifndef GUARD_BATTLE_MAIN_H
#define GUARD_BATTLE_MAIN_H

#include "constants/abilities.h"
#include "constants/pokemon.h"

#define INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER (1 << 0)
#define INTRO_SLIDE_FLAG_SLIDE_IN_STARTED (1 << 1)

// defines for the type effectiveness multipliers
#define TYPE_MUL_NO_EFFECT       0
#define TYPE_MUL_NOT_EFFECTIVE   5
#define TYPE_MUL_NORMAL          10
#define TYPE_MUL_SUPER_EFFECTIVE 20

// defines for the 'DoBounceEffect' function
#define BOUNCE_MON       0x0
#define BOUNCE_HEALTHBOX 0x1

// Return values of GetWhoStrikesFirst
enum
{
	ATTACKER_STRIKES_FIRST,
	DEFENDER_STRIKES_FIRST,
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
extern const u8 gTypeEffectivenessTable[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES];
extern const struct TrainerClass gTrainerClassTable[];
extern const struct Ability gAbilities[ABILITIES_COUNT];
extern const struct NaturalGift gNaturalGiftTable[ITEM_TO_BERRY(BERRY_ITEMS_END) + 1];
extern const struct NonVolatileStatus gNonVolatileStatusConditions[];

void CB2_InitBattle(void);
void BattleMainCB2(void);
void CB2_SosCall(void);
void FreeRestoreBattleData(void);
void VBlankCB_Battle(void);
void SpriteCB_VsLetterDummy(struct Sprite *sprite);
void SpriteCB_VsLetterInit(struct Sprite *sprite);
void CB2_InitEndLinkBattle(void);
u32 GetBattleBgAttribute(u8 arrayId, u8 caseId);
void SpriteCB_EnemyMon(struct Sprite *sprite);
void SpriteCallbackDummy2(struct Sprite *sprite);
void SpriteCB_FaintOpponentMon(struct Sprite *sprite);
void SpriteCb_ShowAsMoveTarget(struct Sprite *sprite);
void SpriteCb_HideAsMoveTarget(struct Sprite *sprite);
void SpriteCB_AllyMon(struct Sprite *sprite);
void SpriteCB_SetToDummy3(struct Sprite *sprite);
void SpriteCB_FaintSlideAnim(struct Sprite *sprite);
void DoBounceEffect(u8 battler, u8 which, s8 delta, s8 amplitude);
void EndBounceEffect(u8 battler, u8 which);
void SpriteCB_PlayerThrowInit(struct Sprite *sprite);
void UpdatePlayerPosInThrowAnim(struct Sprite *sprite);
void BattleDummy(void);
void BeginBattleIntro(void);
void SwitchInClearSetData(u8 battlerId);
void FaintClearSetData(u8 battlerId);
void BattleTurnPassed(void);
u8 IsRunningFromBattleImpossible(u8 battlerId, bool8 checkIngrain);
void UpdatePartyOwnerOnSwitch_NonMulti(u8 battler);
void SwapTurnOrder(u8 id1, u8 id2);
u8 GetWhoStrikesFirst(u8 battler1, u8 battler2, bool8 ignoreChosenMoves);
s8 GetMovePriority(u8 battler, u16 move);
u32 GetBattlerTotalSpeed(u8 battler);
void RunBattleScriptCommands_PopCallbacksStack(void);
void RunBattleScriptCommands(void);
bool8 TryRunFromBattle(u8 battler);

#endif // GUARD_BATTLE_MAIN_H
