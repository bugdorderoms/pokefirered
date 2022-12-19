#ifndef GUARD_RIDE_PAGER_H
#define GUARD_RIDE_PAGER_H

u8 CountObtainedPokeRides(void);
bool8 PlayerHasObtainedSharpedoPaddle(void);
void Task_InitRidePager(u8 taskId);
bool8 FieldCB_ReturnToFieldUseRidePager(void);
u16 RideToSpeciesId(u8 ride);
bool8 TryDismountPokeRide(void);
void TryRemoveStrengthFlag(void);
bool8 CheckPlayerInGroundRocks(void);
void MovePlayerOnRide(u8 newDirection, u16 heldKeys);

extern u8 gUsingRideMon; // for summon mon field effect anim

#endif