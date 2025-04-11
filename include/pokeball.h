#ifndef GUARD_POKEBALL_H
#define GUARD_POKEBALL_H

#include "global.h"

#define POKEBALL_PLAYER_SENDOUT     0xFF
#define POKEBALL_OPPONENT_SENDOUT   0xFE

#define ITEM_TO_BALL(itemId) (itemId - 1)

u32 ItemIdToBallId(u32 itemId);
void LoadBallGfx(u32 ballId);
void FreeBallGfx(u32 ballId);
u32 CreateBallSprite(u32 ballId, s16 x, s16 y, u32 subpriority);
void DoPokeballSendOutAnimation(u32 battlerId, u32 kindOfThrow);
u32 GetBattlerPokeballItemId(u32 battlerId);
void CreatePokeballSpriteToReleaseMon(u32 monSpriteId, u32 battlerId, u32 x, u32 y, u32 oamPriority, u32 subpriortiy, u32 g, u32 h);
u32 CreateTradePokeballSprite(u32 monSpriteId, u32 battlerId, u32 x, u32 y, u32 oamPriority, u32 subpriortiy, u32 g, u32 h);
u32 LaunchBallStarsTask(u8 x, u8 y, u32 priority, u32 subpriority, u32 ballId);
u32 LaunchBallFadeMonTask(bool32 unFadeLater, u32 battlerId, u32 arg2, u32 ballId);
void CreateStarsWhenBallClicks(struct Sprite *sprite);

#endif // GUARD_POKEBALL_H
