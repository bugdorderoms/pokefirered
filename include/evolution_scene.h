#ifndef GUARD_EVOLUTION_SCENE_H
#define GUARD_EVOLUTION_SCENE_H

#include "global.h"

#define TASK_BIT_CAN_STOP   0x1
#define TASK_BIT_LEARN_MOVE 0x80

extern void (*gCB2_AfterEvolution)(void);

void BeginEvolutionScene(struct Pokemon* mon, u32 speciesToEvolve, bool32 canStopEvo, u32 partyId);
void EvolutionScene(struct Pokemon* mon, u32 speciesToEvolve, u32 bits, u32 partyId);
void TradeEvolutionScene(struct Pokemon* mon, u32 speciesToEvolve, u32 preEvoSpriteId, u32 partyId);
void IsMovingBackgroundTaskRunning(void);

#endif // GUARD_EVOLUTION_SCENE_H
