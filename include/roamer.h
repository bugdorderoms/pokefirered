#ifndef GUARD_ROAMER_H
#define GUARD_ROAMER_H

#include "global.h"

void ClearRoamerData(void);
void ClearRoamerLocationData(void);
void InitRoamer(void);
void UpdateLocationHistoryForRoamer(void);
void RoamerMoveToOtherLocationSet(void);
void RoamerMove(void);
bool32 TryStartRoamerEncounter(void);
void UpdateRoamerHPStatus(struct Pokemon *mon);
void SetRoamerInactive(void);
u32 GetRoamerLocationMapSectionId(void);
u32 GetRoamerSpecies(void);

#endif // GUARD_ROAMER_H
