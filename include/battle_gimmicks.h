#ifndef GUARD_BATTLE_GIMMICKS_H
#define GUARD_BATTLE_GIMMICKS_H

#include "constants/battle_gimmicks.h"

// Indicators. Some gimmicks doesn't have an indicator sprite
enum
{
    GIMMICK_INDICATOR_NONE,
    GIMMICK_INDICATOR_MEGA,
    GIMMICK_INDICATOR_OMEGA,
    GIMMICK_INDICATOR_ALPHA,
    GIMMICK_INDICATOR_ULTRA_BURST,
    GIMMICK_INDICATOR_DYNAMAX,
    GIMMICK_INDICATOR_TERA,
    GIMMICK_INDICATOR_TOTEM,
    GIMMICK_INDICATORS_COUNT,
};

#define NUM_GIMMICK_BITS ROUND_BITS_TO_BYTES(GIMMICKS_COUNT)

// General
void AssignUsableGimmick(u32 battler);
void SetActiveGimmick(u32 battler, u32 gimmick);
bool32 ShouldRemoveActiveGimmick(u32 battler, u32 state);
u32 GetActiveGimmick(u32 battler);
bool32 IsGimmickActiveOrSelected(u32 battler, u32 gimmick);
bool32 HasTrainerUsedGimmick(u32 battler, u32 gimmick);
void ActivateGimmick(u32 battler, u32 gimmick);
const u8 *GetGimmickActivationScript(u32 gimmick);
void GetGimmicksKeyItemsBits(u32 battler, u8 *data);

// Trigger
void CreateGimmickTriggerSprite(u32 battler);
void HideGimmickTriggerSprite(void);
bool32 IsGimmickTriggerSpriteActive(void);
void ChangeGimmickTriggerSprite(u32 triggerSpriteId, bool32 lightUp);

// Indicator
u32 CreateGimmickIndicatorSprite(u32 battler);
void SetGimmickIndicatorSpriteVisibility(u32 indicatorSpriteId, bool32 invisible);
void UpdateIndicatorLevelData(u32 indicatorSpriteId, u32 level);
u32 GetGimmickIndicatorId(u32 battler);

extern const struct SpritePalette gSpritePalette_GimmickIndicator;

// Mega
bool32 HasMegaBracelet(u32 battler);
bool32 CanMegaEvolve(u32 battler);
void ActivateMegaEvolution(u32 battler);

// Ultra Burst
bool32 CanUltraBurst(u32 battler);
void ActivateUltraBurst(u32 battler);

// Z-Move
bool32 HasZRing(u32 battler);
bool32 IsZMove(u32 move);
bool32 IsTypeBasedZMove(u32 move);

// Dynamax
bool32 HasDynamaxBand(u32 battler);
bool32 CanDynamax(u32 battler);
void ActivateDynamax(u32 battler);
bool32 IsGigantamaxed(u32 battler);
bool32 TryRevertGigantamax(u32 battler);
bool32 CalcBattlerDynamaxHP(u32 battler, u16 *hp, u16 *maxHP, bool32 inverseMultiplier);
u32 GetNonDynamaxHP(u32 battler);
u32 GetNonDynamaxMaxHP(u32 battler);
u32 GetMaxMove(u32 battler, u32 species, u32 baseMove, u32 moveType, u32 moveSplit);
u32 GetTypeBasedMaxMove(u32 battler, u32 species, u32 type);
u32 GetMaxMovePower(u32 baseMove, u32 maxMove, u32 moveType);
bool32 IsMaxMove(u32 move);
bool32 IsMoveBlockedByDynamax(u32 move);

// Tera
bool32 HasTeraOrb(u32 battler);
bool32 CanTerastallize(u32 battler);
void ActivateTera(u32 battler);
u32 GetBattlerTeraType(u32 battler);
bool32 IsTypeStellarBoosted(u32 battler, u32 type);
void TryExpendTypeStellarBoost(u32 battler, u32 type);

#endif // GUARD_BATTLE_GIMMICKS_H
