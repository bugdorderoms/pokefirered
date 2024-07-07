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

u8 GetLeadMonIndex(void);
s32 CountDigits(s32 number);
void TV_PrintIntToStringVar(u8 varidx, s32 number);
u16 GetStarterSpecies(void);
void StopPokemonLeagueLightingEffectTask(void);
u8 ContextNpcGetTextColor(void);
void SetPCBoxToSendMon(u8);
u16 GetPCBoxToSendMon(void);
bool8 IsDestinationBoxFull(void);
bool8 ShouldShowBoxWasFullMessage(void);
u16 GetHiddenItemAttr(u32 hiddenItem, u8 attr);
u8 GetUnlockedSeviiAreas(void);
u32 GetPlayerTrainerId(void);
bool8 CutMoveRuinValleyCheck(void);
void CutMoveOpenDottedHoleDoor(void);
void RunMassageCooldownStepCounter(void);
void IncrementResortGorgeousStepCounter(void);
void IncrementBirthIslandRockStepCount(void);
void ResetFieldTasksArgs(void);
bool8 UsedPokemonCenterWarp(void);
void UpdateTrainerCardPhotoIcons(void);
void UpdateTrainerFansAfterLinkBattle(void);
void ResetTrainerFanClub(void);
void LoadSymbolsIconGraphics(void);
void FreeSymbolsIconGraphics(void);
u8 Create8x8SymbolSprite(s16 x, s16 y, u8 subpriority, u8 symbolId);

extern const struct CapeBrinkTutor gCapeBrinkCompatibleSpecies[3];

#endif // GUARD_FIELD_SPECIALS_H
