#ifndef GUARD_RIDE_PAGER_H
#define GUARD_RIDE_PAGER_H

void InitRidePager(void);
u32 CountObtainedPokeRides(void);
bool32 PlayerHasObtainedSharpedoPaddle(void);
u32 RideToSpeciesId(u32 ride);
bool32 TryDismountPokeRide(void);
void TryRemoveStrengthFlag(void);
bool32 CheckPlayerInGroundRocks(void);
void MovePlayerOnRide(u32 newDirection, u16 heldKeys);
u32 ApplyRideEncounterRateMod(u32 encounterRate);
void DecreaseTaurosChargeStamina(void);

extern u8 gUsingRideMon; // for summon mon field effect anim

#endif