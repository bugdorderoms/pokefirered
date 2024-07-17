#ifndef GUARD_RIDE_PAGER_H
#define GUARD_RIDE_PAGER_H

void InitRidePager(void);
u8 CountObtainedPokeRides(void);
bool8 PlayerHasObtainedSharpedoPaddle(void);
u16 RideToSpeciesId(u8 ride);
bool8 TryDismountPokeRide(void);
void TryRemoveStrengthFlag(void);
bool8 CheckPlayerInGroundRocks(void);
void MovePlayerOnRide(u8 newDirection, u16 heldKeys);
u32 ApplyRideEncounterRateMod(u32 encounterRate);

extern u8 gUsingRideMon; // for summon mon field effect anim

#endif