#ifndef GUARD_POKEMON_SPECIAL_ANIM_H
#define GUARD_POKEMON_SPECIAL_ANIM_H

#include "global.h"

void CreateLevelUpVerticalSpritesTask(u16 x, u16 y, u32 tileTag, u32 paletteTag, u32 priority, u32 subpriority);
bool32 LevelUpVerticalSpritesTaskIsRunning(void);
void GetMonLevelUpWindowStats(struct Pokemon *mon, u16 *currStats);
void DrawLevelUpWindowPg1(u32 windowId, u16 *statsBefore, u16 *statsAfter, u32 bgClr, u32 fgClr, u32 shadowClr);
void DrawLevelUpWindowPg2(u32 windowId, u16 *currStats, u32 bgClr, u32 fgClr, u32 shadowClr);
void StartUseItemAnim_Normal(u32 slotId, u32 itemId, MainCallback callback);
bool32 PSA_IsCancelDisabled(void);
void StartUseItemAnim_ForgetMoveAndLearnTMorHM(u32 slotId, u32 itemId, u32 moveId, MainCallback callback);

#endif // GUARD_POKEMON_SPECIAL_ANIM_H
