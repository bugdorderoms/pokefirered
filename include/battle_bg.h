#ifndef GUARD_BATTLE_BG_H
#define GUARD_BATTLE_BG_H

#include "bg.h"
#include "task.h"

struct BattleBackground
{
	const u8 *name;
    const void *tileset;
    const void *tilemap;
    const void *entryTileset;
    const void *entryTilemap;
    const void *palette;
	TaskFunc introSlideFunc;
	u8 secretPowerEffect;
	u8 camouflageType;
	u16 camouflageBlendColor;
	u16 naturePowerMove;
};

extern const struct BgTemplate gBattleBgTemplates[];
extern const struct BattleBackground gBattleTerrainTable[BATTLE_TERRAINS_COUNT];

void LoadBattleTerrainGfx(u8 terrain);
void InitBattleBgsVideo(void);
void DrawBattleEntryBackground(void);
void InitLinkBattleVsScreen(u8 taskId);
void LoadBattleMenuWindowGfx(void);
void LoadBattleTextboxAndBackground(void);
void BattleInitBgsAndWindows(void);
void DrawMainBattleBackground(void);
void CreateBattleMoveInfoWindowsAndArrows(u16 move);
void DestroyBattleMoveInfoWindows(void);

#endif // GUARD_BATTLE_BG_H
