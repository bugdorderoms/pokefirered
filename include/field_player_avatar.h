#ifndef GUARD_FIELD_PLAYER_AVATAR_H
#define GUARD_FIELD_PLAYER_AVATAR_H

#include "global.h"

enum
{
	SPIN_DIRECTION_NONE,
    SPIN_DIRECTION_CLOCKWISE,
    SPIN_DIRECTION_COUNTER_CLOCKWISE,
};

// For readability
#define SPIN_DIRECTION_ANY SPIN_DIRECTION_NONE

void ClearPlayerAvatarInfo(void);
void SetPlayerAvatarExtraStateTransition(u32, u32);
u32 GetPlayerAvatarGenderByGraphicsId(u32);
void PlayerGetDestCoords(s16 *, s16 *);
u32 GetPlayerFacingDirection(void);

void MovementType_Player(struct Sprite * sprite);
void HandleEnforcedLookDirectionOnPlayerStopMoving(void);
void StopPlayerAvatar(void);
void GetXYCoordsOneStepInFrontOfPlayer(s16 *xPtr, s16 *yPtr);
u32 GetPlayerAvatarGraphicsIdByStateId(u32);
void SetPlayerAvatarStateMask(u32 mask);
void SavePlayerFacingDirectionForTeleport(u32 direction);
void SetPlayerAvatarTransitionFlags(u32 flags);
bool32 IsPlayerFacingSurfableFishableWater(void);
void StartFishing(u32 secondaryId);
u32 GetPlayerAvatarGraphicsIdByCurrentState(void);
void StartPlayerAvatarSummonMonForFieldMoveAnim(void);
void SetPlayerInvisibility(bool32);
void StartTeleportInPlayerAnim(void);
void StartTeleportWarpOutPlayerAnim(void);
bool32 WaitTeleportWarpOutPlayerAnim(void);
bool32 WaitTeleportInPlayerAnim(void);
bool32 IsPlayerSurfingNorth(void);
bool32 player_get_pos_including_state_based_drift(s16 *x, s16 *y);
void StartPlayerAvatarVsSeekerAnim(void);
u32 GetPlayerAvatarGraphicsIdByStateIdAndGender(u32 state, u32 gender);
u32 CheckForObjectEventCollision(struct ObjectEvent *objectEvent, s16 x, s16 y, u32 direction, u32 metatileBehavior);
void PlayerGoSlowest(u32 direction);
void PlayerGoSlow(u32 direction);
void PlayerGoSpeed1(u32 direction);
void PlayerGoSpeed2(u32 direction);
void PlayerRideWaterCurrent(u32 direction);
void PlayerGoSpeed4(u32 direction);
void PlayerRun(u32 direction);
void PlayerRunSlow(u32 direction);
void PlayerOnBikeCollide(u32 direction);
void PlayerNotOnBikeCollide(u32 direction);
void PlayerFaceDirection(u32 direction);
void PlayerFaceDirectionFast(u32 direction);
void PlayerTurnInPlace(u32 direction);
void PlayerJumpLedge(u32 direction);
void player_step(u32 direction, u16 newKeys, u16 heldKeys);
bool32 PlayerIsMovingOnRockStairs(u32 direction);
void UpdatePlayerAvatarTransitionState(void);
void InitPlayerAvatar(s16 x, s16 y, u32 direction, u32 gender);
void PlayerUseAcroBikeOnBumpySlope(u32 direction);
u32 GetRSAvatarGraphicsIdByGender(u32 gender);
bool32 ForcedMovement_None(void);
void UpdateSpinData(void);
void ResetSpinTimer(void);
bool32 CanTriggerSpinEvolution(void);

static inline u32 GetPlayerMovementDirection(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].movementDirection;
}

static inline u32 PlayerGetZCoord(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].previousElevation;
}

static inline bool32 TestPlayerAvatarFlags(u32 bm)
{
	if (gPlayerAvatar.flags & bm)
		return TRUE;
	else
		return FALSE;
}

#endif //GUARD_FIELD_PLAYER_AVATAR_H
