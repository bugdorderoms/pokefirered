#ifndef GUARD_EVOLUTION_GRAPHICS_H
#define GUARD_EVOLUTION_GRAPHICS_H

#include "global.h"

void LoadEvoSparkleSpriteAndPal(void);

u32 LaunchTask_PreEvoSparklesSet1(u32 arg0);
u32 LaunchTask_PreEvoSparklesSet2(void);
u32 LaunchTask_PostEvoSparklesSet1(void);
u32 LaunchTask_PostEvoSparklesSet2AndFlash(u32 species);
u32 LaunchTask_PostEvoSparklesSet2AndFlash_Trade(u32 species);
u32 LaunchTask_PrePostEvoMonSprites(u32 preEvoSpriteId, u32 postEvoSpriteId);

#define EvoGraphicsTaskEvoStop  data[8]

#endif // GUARD_EVOLUTION_GRAPHICS_H
