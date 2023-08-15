#ifndef GUARD_EVOLUTION_SCENE_H
#define GUARD_EVOLUTION_SCENE_H

#include "global.h"

#define TASK_BIT_CAN_STOP   0x1
#define TASK_BIT_LEARN_MOVE 0x80

extern void (*gCB2_AfterEvolution)(void);

void BeginEvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, bool8 canStopEvo, u8 partyId);
void EvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, u8 bits, u8 partyId);
void TradeEvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, u8 preEvoSpriteId, u8 partyId);
void IsMovingBackgroundTaskRunning(void);

#endif // GUARD_EVOLUTION_SCENE_H
