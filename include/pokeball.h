#ifndef GUARD_POKEBALL_H
#define GUARD_POKEBALL_H

#include "global.h"

extern const struct CompressedSpriteSheet gBallSpriteSheets[];
extern const struct CompressedSpritePalette gBallSpritePalettes[];

#define POKEBALL_PLAYER_SENDOUT     0xFF
#define POKEBALL_OPPONENT_SENDOUT   0xFE

#define ITEM_TO_BALL(itemId) (itemId - 1)

u8 ItemIdToBallId(u16 itemId);
void LoadBallGfx(u8 ballId);
void FreeBallGfx(u8 ballId);
u8 CreateBallSprite(u8 ballId, s16 x, s16 y, u8 subpriority);
void DoPokeballSendOutAnimation(u8 battlerId, u8 kindOfThrow);
void CreatePokeballSpriteToReleaseMon(u8 monSpriteId, u8 battlerId, u8 x, u8 y, u8 oamPriority, u8 subpriortiy, u8 g, u32 h);
u8 CreateTradePokeballSprite(u8 monSpriteId, u8 battlerId, u8 x, u8 y, u8 oamPriority, u8 subpriortiy, u8 g, u32 h);
u8 LaunchBallStarsTask(u8 x, u8 y, u8 priority, u8 subpriority, u8 ballId);
u8 LaunchBallFadeMonTask(bool8 unFadeLater, u8 battlerId, u32 arg2, u8 ballId);
void CreateStarsWhenBallClicks(struct Sprite *sprite);

#endif // GUARD_POKEBALL_H
