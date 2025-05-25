#ifndef GUARD_FIELD_SPECIALS_H
#define GUARD_FIELD_SPECIALS_H

#include "global.h"

enum HiddenItemAttr
{
    HIDDEN_ITEM_ID = 0,
    HIDDEN_ITEM_FLAG,
    HIDDEN_ITEM_QUANTITY,
    HIDDEN_ITEM_UNDERFOOT
};

enum
{
    SYMBOL_YELLOWSTAR,
    SYMBOL_WHITESTAR,
    SYMBOL_POKEBALL,
    SYMBOL_HELDITEM,
    SYMBOL_HELDMAIL,
    SYMBOLS_COUNT
};

struct CapeBrinkTutor
{
    u16 species;
    u16 move;
    u16 flagId;
};

#define TAG_8x8_SYMBOLS 0x0066

u32 GetLeadMonIndex(void);
s32 CountDigits(s32 number);
void TV_PrintIntToStringVar(u32 varidx, s32 number);
u32 GetStarterSpecies(void);
void StopPokemonLeagueLightingEffectTask(void);
u32 ContextNpcGetTextColor(void);
void SetPCBoxToSendMon(u32);
u32 GetPCBoxToSendMon(void);
bool32 IsDestinationBoxFull(void);
bool32 ShouldShowBoxWasFullMessage(void);
u16 GetHiddenItemAttr(u32 hiddenItem, u32 attr);
u32 GetUnlockedSeviiAreas(void);
u32 GetPlayerTrainerId(void);
bool32 CutMoveRuinValleyCheck(void);
void CutMoveOpenDottedHoleDoor(void);
void RunMassageCooldownStepCounter(void);
void IncrementResortGorgeousStepCounter(void);
void IncrementBirthIslandRockStepCount(void);
void ResetFieldTasksArgs(void);
bool32 UsedPokemonCenterWarp(void);
void UpdateTrainerCardPhotoIcons(void);
void UpdateTrainerFansAfterLinkBattle(void);
void ResetTrainerFanClub(void);
void LoadSymbolsIconGraphics(void);
void FreeSymbolsIconGraphics(void);
u32 Create8x8SymbolSprite(s16 x, s16 y, u32 subpriority, u32 symbolId);
void DisablePlayerBag(void);
void EnablePlayerBag(void);

extern const struct CapeBrinkTutor gCapeBrinkCompatibleSpecies[3];

#endif // GUARD_FIELD_SPECIALS_H
