#include "global.h"
#include "gflib.h"
#include "bike.h"
#include "dexnav.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_control_avatar.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_player_avatar.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "random.h"
#include "ride_pager.h"
#include "script.h"
#include "strings.h"
#include "wild_encounter.h"
#include "constants/abilities.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/songs.h"
#include "constants/metatile_behaviors.h"
#include "constants/moves.h"
#include "constants/trainer_types.h"

static EWRAM_DATA u8 sTeleportSavedFacingDirection = DIR_NONE;
EWRAM_DATA struct ObjectEvent gObjectEvents[OBJECT_EVENTS_COUNT] = {};
EWRAM_DATA struct PlayerAvatar gPlayerAvatar = {};

static u8 ObjectEventCB2_NoMovement2(struct ObjectEvent * object, struct Sprite * sprite);
static bool8 TryUpdatePlayerSpinDirection(void);
static void npc_clear_strange_bits(struct ObjectEvent * playerObjEvent);
static bool8 TryDoMetatileBehaviorForcedMovement(void);
static void MovePlayerAvatarUsingKeypadInput(u8 direction, u16 newKeys, u16 heldKeys);
static void PlayerAllowForcedMovementIfMovingSameDirection(void);
static bool8 ForcedMovement_Slip(void);
static bool8 ForcedMovement_WalkSouth(void);
static bool8 ForcedMovement_WalkNorth(void);
static bool8 ForcedMovement_WalkWest(void);
static bool8 ForcedMovement_WalkEast(void);
static bool8 ForcedMovement_SpinRight(void);
static bool8 ForcedMovement_SpinLeft(void);
static bool8 ForcedMovement_SpinUp(void);
static bool8 ForcedMovement_SpinDown(void);
static bool8 ForcedMovement_PushedSouthByCurrent(void);
static bool8 ForcedMovement_PushedNorthByCurrent(void);
static bool8 ForcedMovement_PushedWestByCurrent(void);
static bool8 ForcedMovement_PushedEastByCurrent(void);
static bool8 ForcedMovement_SlideSouth(void);
static bool8 ForcedMovement_SlideNorth(void);
static bool8 ForcedMovement_SlideWest(void);
static bool8 ForcedMovement_SlideEast(void);
static bool8 ForcedMovement_0xBB(void);
static bool8 ForcedMovement_0xBC(void);
static void PlayerNotOnBikeNotMoving(u8 direction, u16 heldKeys);
static void PlayerNotOnBikeTurningInPlace(u8 direction, u16 heldKeys);
static void PlayerNotOnBikeMoving(u8 direction, u16 heldKeys);
static u8 CheckForPlayerAvatarCollision(u8 direction);
static bool8 CanStopSurfing(s16 x, s16 y, u8 direction);
static bool8 TryPushBoulder(s16 x, s16 y, u8 direction);
static void CheckAcroBikeCollision(s16 x, s16 y, u8 metatileBehavior, u8 *collision);
static void CheckGroundRocksCollision(u8 metatileBehavior, u8 *collision);
static void DoPlayerAvatarTransition(void);
static void PlayerAvatarTransition_Dummy(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_Normal(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_Bike(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_Surfing(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_Underwater(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_ReturnToField(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_TaurosRide(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_StoutlandRide(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_MudsdaleRide(struct ObjectEvent * playerObject);
static void PlayerAvatarTransition_MachampRide(struct ObjectEvent * playerObject);
static bool8 PlayerIsAnimActive(void);
static bool8 PlayerCheckIfAnimFinishedOrInactive(void);
static bool8 PlayerAnimIsMultiFrameStationary(void);
static bool8 PlayerAnimIsMultiFrameStationaryAndStateNotTurning(void);
static void PlayCollisionSoundIfNotFacingWarp(u8 direction);
static void PlayerGoSpin(u8 direction);
static void PlayerApplyTileForcedMovement(u8 metatileBehavior);
static bool8 MetatileAtCoordsIsWaterTile(s16 x, s16 y);
static void HandleWarpArrowSpriteHideShow(struct ObjectEvent * playerObjEvent);
static void StartStrengthAnim(u8 objectEventId, u8 direction);
static void Task_BumpBoulder(u8 taskId);
static bool8 DoBoulderInit(struct Task * task, struct ObjectEvent * playerObj, struct ObjectEvent * boulderObj);
static bool8 DoBoulderDust(struct Task * task, struct ObjectEvent * playerObj, struct ObjectEvent * boulderObj);
static bool8 DoBoulderFinish(struct Task * task, struct ObjectEvent * playerObj, struct ObjectEvent * boulderObj);
static void DoPlayerMatJump(void);
static void DoPlayerAvatarSecretBaseMatJump(u8 taskId);
static bool8 PlayerAvatar_DoSecretBaseMatJump(struct Task * task, struct ObjectEvent * playerObj);
static void DoPlayerMatSpin(void);
static void PlayerAvatar_DoSecretBaseMatSpin(u8 taskId);
static bool8 PlayerAvatar_SecretBaseMatSpinStep0(struct Task * task, struct ObjectEvent * playerObj);
static bool8 PlayerAvatar_SecretBaseMatSpinStep1(struct Task * task, struct ObjectEvent * playerObj);
static bool8 PlayerAvatar_SecretBaseMatSpinStep2(struct Task * task, struct ObjectEvent * playerObj);
static bool8 PlayerAvatar_SecretBaseMatSpinStep3(struct Task * task, struct ObjectEvent * playerObj);
static void CreateStopSurfingTask(u8 direction);
static void Task_StopSurfingInit(u8 taskId);
static void Task_WaitStopSurfing(u8 taskId);
static void Task_Fishing(u8 taskId);
static bool8 Fishing1(struct Task * task);
static bool8 Fishing2(struct Task * task);
static bool8 Fishing3(struct Task * task);
static bool8 Fishing4(struct Task * task);
static bool8 Fishing5(struct Task * task);
static bool8 Fishing6(struct Task * task);
static bool8 Fishing7(struct Task * task);
static bool8 Fishing8(struct Task * task);
static bool8 Fishing9(struct Task * task);
static bool8 Fishing10(struct Task * task);
static bool8 Fishing11(struct Task * task);
static bool8 Fishing12(struct Task * task);
static bool8 Fishing13(struct Task * task);
static bool8 Fishing14(struct Task * task);
static bool8 Fishing15(struct Task * task);
static bool8 Fishing16(struct Task * task);
static void Task_TeleportWarpOutPlayerAnim(u8 taskId);
static void Task_TeleportWarpInPlayerAnim(u8 taskId);
static u8 TeleportAnim_RotatePlayer(struct ObjectEvent * object, s16 *timer);
static void MovePlayerNotOnBike(u8 direction, u16 heldKeys);

/////////////////////////////
// PLAYER FORCED MOVEMENTS //
/////////////////////////////

// Player forced movement actions
static const struct {
    bool8 (*check)(u8 metatileBehavior);
    bool8 (*apply)(void);
} sForcedMovementFuncs[] = {
    {MetatileBehavior_IsUnknownMovement48, ForcedMovement_Slip},
    {MetatileBehavior_IsIce_2,             ForcedMovement_Slip},
    {MetatileBehavior_IsWalkSouth,         ForcedMovement_WalkSouth},
    {MetatileBehavior_IsWalkNorth,         ForcedMovement_WalkNorth},
    {MetatileBehavior_IsWalkWest,          ForcedMovement_WalkWest},
    {MetatileBehavior_IsWalkEast,          ForcedMovement_WalkEast},
    {MetatileBehavior_IsSouthwardCurrent,  ForcedMovement_PushedSouthByCurrent},
    {MetatileBehavior_IsNorthwardCurrent,  ForcedMovement_PushedNorthByCurrent},
    {MetatileBehavior_IsWestwardCurrent,   ForcedMovement_PushedWestByCurrent},
    {MetatileBehavior_IsEastwardCurrent,   ForcedMovement_PushedEastByCurrent},
    {MetatileBehavior_IsSpinRight,         ForcedMovement_SpinRight},
    {MetatileBehavior_IsSpinLeft,          ForcedMovement_SpinLeft},
    {MetatileBehavior_IsSpinUp,            ForcedMovement_SpinUp},
    {MetatileBehavior_IsSpinDown,          ForcedMovement_SpinDown},
    {MetatileBehavior_IsSlideSouth,        ForcedMovement_SlideSouth},
    {MetatileBehavior_IsSlideNorth,        ForcedMovement_SlideNorth},
    {MetatileBehavior_IsSlideWest,         ForcedMovement_SlideWest},
    {MetatileBehavior_IsSlideEast,         ForcedMovement_SlideEast},
    {MetatileBehavior_IsWaterfall,         ForcedMovement_PushedSouthByCurrent},
    {MetatileBehavior_UnusedReturnFalse_7, ForcedMovement_0xBB},
    {MetatileBehavior_UnusedReturnFalse_8, ForcedMovement_0xBC},
    {NULL,                                 NULL}, // End of table
};

void MovementType_Player(struct Sprite *sprite)
{
    UpdateObjectEventCurrentMovement(&gObjectEvents[sprite->data[0]], sprite, ObjectEventCB2_NoMovement2);
}

static u8 ObjectEventCB2_NoMovement2(struct ObjectEvent * object, struct Sprite * sprite)
{
    return 0;
}

static bool8 TryInterruptObjectEventSpecialAnim(struct ObjectEvent *playerObjEvent, u8 direction)
{
    if (ObjectEventIsMovementOverridden(playerObjEvent) && !ObjectEventClearHeldMovementIfFinished(playerObjEvent))
    {
        u8 heldMovementActionId = ObjectEventGetHeldMovementActionId(playerObjEvent);
		
        if (heldMovementActionId > MOVEMENT_ACTION_WALK_FAST_RIGHT && heldMovementActionId < MOVEMENT_ACTION_WALK_IN_PLACE_NORMAL_DOWN)
        {
            if (direction != DIR_NONE && playerObjEvent->movementDirection != direction)
            {
                ObjectEventClearHeldMovement(playerObjEvent);
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

void player_step(u8 direction, u16 newKeys, u16 heldKeys)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    HandleWarpArrowSpriteHideShow(playerObjEvent);
	
    if (!gPlayerAvatar.preventStep && !TryUpdatePlayerSpinDirection())
    {
        if (!TryInterruptObjectEventSpecialAnim(playerObjEvent, direction))
        {
            npc_clear_strange_bits(playerObjEvent);
            DoPlayerAvatarTransition();
			
            if (!TryDoMetatileBehaviorForcedMovement())
            {
                MovePlayerAvatarUsingKeypadInput(direction, newKeys, heldKeys);
                PlayerAllowForcedMovementIfMovingSameDirection();
            }
        }
    }
}

static void MovePlayerAvatarUsingKeypadInput(u8 direction, u16 newKeys, u16 heldKeys)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        MovePlayerOnBike(direction, newKeys, heldKeys);
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_RIDE_ANY))
		MovePlayerOnRide(direction, heldKeys);
    else
        MovePlayerNotOnBike(direction, heldKeys);
}

static void npc_clear_strange_bits(struct ObjectEvent *objEvent)
{
    objEvent->inanimate = FALSE;
    objEvent->disableAnim = FALSE;
    objEvent->facingDirectionLocked = FALSE;
    gPlayerAvatar.flags &= ~(PLAYER_AVATAR_FLAG_DASH);
}

static void PlayerAllowForcedMovementIfMovingSameDirection(void)
{
    if (gPlayerAvatar.runningState == MOVING)
        gPlayerAvatar.flags &= ~(PLAYER_AVATAR_FLAG_CONTROLLABLE);
}

static bool8 TryUpdatePlayerSpinDirection(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_FORCED) && MetatileBehavior_IsSpinTile(gPlayerAvatar.lastSpinTile))
    {
		struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
		
        if (playerObjEvent->heldMovementFinished)
        {
            if (MetatileBehavior_IsStopSpinning(playerObjEvent->currentMetatileBehavior))
                return FALSE;
			
            if (MetatileBehavior_IsSpinTile(playerObjEvent->currentMetatileBehavior))
                gPlayerAvatar.lastSpinTile = playerObjEvent->currentMetatileBehavior;
			
            ObjectEventClearHeldMovement(playerObjEvent);
            PlayerApplyTileForcedMovement(gPlayerAvatar.lastSpinTile);
        }
        return TRUE;
    }
    return FALSE;
}

static bool8 TryDoMetatileBehaviorForcedMovement(void)
{
    u32 i;
    u8 behavior;
	
    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_CONTROLLABLE))
    {
        behavior = gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior;
		
        for (i = 0; sForcedMovementFuncs[i].check != NULL; i++)
        {
            if (sForcedMovementFuncs[i].check(behavior))
            {
                gPlayerAvatar.lastSpinTile = behavior;
                return sForcedMovementFuncs[i].apply();
            }
        }
    }
	return ForcedMovement_None();
}

static void PlayerApplyTileForcedMovement(u8 metatileBehavior)
{
    u32 i;

    for (i = 0; sForcedMovementFuncs[i].check != NULL; i++)
    {
        if (sForcedMovementFuncs[i].check(metatileBehavior))
            sForcedMovementFuncs[i].apply();
    }
}

bool8 ForcedMovement_None(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_FORCED))
    {
        struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

        playerObjEvent->facingDirectionLocked = FALSE;
        playerObjEvent->enableAnim = TRUE;
        SetObjectEventDirection(playerObjEvent, playerObjEvent->facingDirection);
        gPlayerAvatar.flags &= ~(PLAYER_AVATAR_FLAG_FORCED);
    }
    return FALSE;
}

static u8 DoForcedMovement(u8 direction, MovementAction movementAction)
{
    struct PlayerAvatar *playerAvatar = &gPlayerAvatar;
    u8 collision = CheckForPlayerAvatarCollision(direction);

    playerAvatar->flags |= PLAYER_AVATAR_FLAG_FORCED;
	
    if (collision)
    {
        ForcedMovement_None();
		
        if (collision < COLLISION_STOP_SURFING)
            return 0;
        else
        {
            if (collision == COLLISION_LEDGE_JUMP)
                PlayerJumpLedge(direction);
			
            playerAvatar->flags |= PLAYER_AVATAR_FLAG_FORCED;
            playerAvatar->runningState = MOVING;
            return 1;
        }
    }
    else
    {
        playerAvatar->runningState = MOVING;
        movementAction(direction);
        return 1;
    }
}

static u8 DoForcedMovementInCurrentDirection(MovementAction movementAction)
{
    gObjectEvents[gPlayerAvatar.objectEventId].disableAnim = TRUE;
    return DoForcedMovement(gObjectEvents[gPlayerAvatar.objectEventId].movementDirection, movementAction);
}

static bool8 ForcedMovement_Slip(void)
{
    return DoForcedMovementInCurrentDirection(PlayerGoSpeed2);
}

static bool8 ForcedMovement_WalkSouth(void)
{
    return DoForcedMovement(DIR_SOUTH, PlayerGoSpeed1);
}

static bool8 ForcedMovement_WalkNorth(void)
{
    return DoForcedMovement(DIR_NORTH, PlayerGoSpeed1);
}

static bool8 ForcedMovement_WalkWest(void)
{
    return DoForcedMovement(DIR_WEST, PlayerGoSpeed1);
}

static bool8 ForcedMovement_WalkEast(void)
{
    return DoForcedMovement(DIR_EAST, PlayerGoSpeed1);
}

static inline void PlaySpinSound(void)
{
	PlaySE(SE_M_RAZOR_WIND2);
}

static bool8 ForcedMovement_SpinRight(void)
{
    PlaySpinSound();
    return DoForcedMovement(DIR_EAST, PlayerGoSpin);
}

static bool8 ForcedMovement_SpinLeft(void)
{
    PlaySpinSound();
    return DoForcedMovement(DIR_WEST, PlayerGoSpin);
}

static bool8 ForcedMovement_SpinUp(void)
{
    PlaySpinSound();
    return DoForcedMovement(DIR_NORTH, PlayerGoSpin);
}

static bool8 ForcedMovement_SpinDown(void)
{
    PlaySpinSound();
    return DoForcedMovement(DIR_SOUTH, PlayerGoSpin);
}

static bool8 ForcedMovement_PushedSouthByCurrent(void)
{
    return DoForcedMovement(DIR_SOUTH, PlayerRideWaterCurrent);
}

static bool8 ForcedMovement_PushedNorthByCurrent(void)
{
    return DoForcedMovement(DIR_NORTH, PlayerRideWaterCurrent);
}

static bool8 ForcedMovement_PushedWestByCurrent(void)
{
    return DoForcedMovement(DIR_WEST, PlayerRideWaterCurrent);
}

static bool8 ForcedMovement_PushedEastByCurrent(void)
{
    return DoForcedMovement(DIR_EAST, PlayerRideWaterCurrent);
}

static u8 ForcedMovement_Slide(u8 direction, MovementAction movementAction)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    playerObjEvent->disableAnim = TRUE;
    playerObjEvent->facingDirectionLocked = TRUE;
    return DoForcedMovement(direction, movementAction);
}

static bool8 ForcedMovement_SlideSouth(void)
{
    return ForcedMovement_Slide(DIR_SOUTH, PlayerGoSpeed2);
}

static bool8 ForcedMovement_SlideNorth(void)
{
    return ForcedMovement_Slide(DIR_NORTH, PlayerGoSpeed2);
}

static bool8 ForcedMovement_SlideWest(void)
{
    return ForcedMovement_Slide(DIR_WEST, PlayerGoSpeed2);
}

static bool8 ForcedMovement_SlideEast(void)
{
    return ForcedMovement_Slide(DIR_EAST, PlayerGoSpeed2);
}

static bool8 ForcedMovement_0xBB(void)
{
    DoPlayerMatJump();
    return TRUE;
}

static bool8 ForcedMovement_0xBC(void)
{
    DoPlayerMatSpin();
    return TRUE;
}

///////////////////////////
// PLAYER INPUT HANDLERS //
///////////////////////////

static void (*const sPlayerInputHandlers[])(u8, u16) = {
    PlayerNotOnBikeNotMoving,
    PlayerNotOnBikeTurningInPlace,
    PlayerNotOnBikeMoving
};

static u8 CheckMovementInputNotOnBike(u8 direction)
{
    if (direction == DIR_NONE)
    {
        gPlayerAvatar.runningState = NOT_MOVING;
        return 0;
    }
    else if (direction != GetPlayerMovementDirection() && gPlayerAvatar.runningState != MOVING)
    {
        gPlayerAvatar.runningState = TURN_DIRECTION;
        return 1;
    }
    else
    {
        gPlayerAvatar.runningState = MOVING;
        return 2;
    }
}

static void MovePlayerNotOnBike(u8 direction, u16 heldKeys)
{
    sPlayerInputHandlers[CheckMovementInputNotOnBike(direction)](direction, heldKeys);
}

static void PlayerNotOnBikeNotMoving(u8 direction, u16 heldKeys)
{
    PlayerFaceDirection(GetPlayerFacingDirection());
}

static void PlayerNotOnBikeTurningInPlace(u8 direction, u16 heldKeys)
{
    PlayerTurnInPlace(direction);
}

static void PlayerNotOnBikeMoving(u8 direction, u16 heldKeys)
{
    u8 collision = CheckForPlayerAvatarCollision(direction);

    if (collision != COLLISION_NONE)
    {
        if (collision == COLLISION_LEDGE_JUMP)
            PlayerJumpLedge(direction);
        else if (collision == COLLISION_ROTATING_GATE)
            PlayerFaceDirection(direction);
        else if (collision != COLLISION_STOP_SURFING && collision != COLLISION_LEDGE_JUMP && collision != COLLISION_PUSHED_BOULDER && collision != COLLISION_ROTATING_GATE)
            PlayerNotOnBikeCollide(direction);

        return;
    }
    gPlayerAvatar.creeping = FALSE;

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
    {
#if DEXNAV_CREEPING_SEARCH
		if (IsDexNavSearchActive() && (heldKeys & A_BUTTON))
		{
			gPlayerAvatar.creeping = TRUE;
			PlayerGoSlow(direction);
		}
		else // speed 2 is fast, same speed as running
#endif
			PlayerGoSpeed2(direction);
		
        return;
    }

    if ((heldKeys & B_BUTTON || gSaveBlock2Ptr->autoRun) && FlagGet(FLAG_SYS_B_DASH) && !MetatileBehaviorForbidsBiking(gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior))
    {
        if (PlayerIsMovingOnRockStairs(direction))
		{
			if ((heldKeys & B_BUTTON) && gSaveBlock2Ptr->autoRun)
				PlayerGoSlow(direction);
			else
				PlayerRunSlow(direction);
		}
        else
		{
			if ((heldKeys & B_BUTTON) && gSaveBlock2Ptr->autoRun)
				PlayerGoSpeed1(direction);
			else
				PlayerRun(direction);
		}
        gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_DASH;
        return;
    }
#if DEXNAV_CREEPING_SEARCH
    else if (IsDexNavSearchActive() && (heldKeys & A_BUTTON))
    {
	    gPlayerAvatar.creeping = TRUE;
	    PlayerGoSlow(direction);
    }
#endif
    else
    {
        if (PlayerIsMovingOnRockStairs(direction))
            PlayerGoSlow(direction);
        else
            PlayerGoSpeed1(direction);
    }
}

bool32 PlayerIsMovingOnRockStairs(u8 direction)
{
    s16 x = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x, y = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;

    switch (direction)
    {
	case DIR_SOUTH:
        MoveCoords(DIR_SOUTH, &x, &y);
		// fall
    case DIR_NORTH:
        return MetatileBehavior_IsRockStairs(MapGridGetMetatileBehaviorAt(x, y));
    }
	return FALSE;
}

static u8 CheckForPlayerAvatarCollision(u8 direction)
{
	struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x = playerObjEvent->currentCoords.x, y = playerObjEvent->currentCoords.y;

    if (IsDirectionalStairWarpMetatileBehavior(MapGridGetMetatileBehaviorAt(x, y), direction))
        return COLLISION_ROTATING_GATE;
	
    MoveCoords(direction, &x, &y);
	
    return CheckForObjectEventCollision(playerObjEvent, x, y, direction, MapGridGetMetatileBehaviorAt(x, y));
}

static inline bool8 ShouldJumpLedge(s16 x, s16 y, u8 z)
{
	return (GetLedgeJumpDirection(x, y, z) != 0);
}

u8 CheckForObjectEventCollision(struct ObjectEvent *objectEvent, s16 x, s16 y, u8 direction, u8 metatileBehavior)
{
    u8 collision = GetCollisionAtCoords(objectEvent, x, y, direction);
	
    if (collision == COLLISION_ELEVATION_MISMATCH && CanStopSurfing(x, y, direction))
        return COLLISION_STOP_SURFING;

    if (ShouldJumpLedge(x, y, direction))
    {
        IncrementGameStat(GAME_STAT_JUMPED_DOWN_LEDGES);
        return COLLISION_LEDGE_JUMP;
    }
	
    if (collision == COLLISION_OBJECT_EVENT && TryPushBoulder(x, y, direction))
        return COLLISION_PUSHED_BOULDER;

    if (collision == COLLISION_NONE)
        CheckAcroBikeCollision(x, y, metatileBehavior, &collision);

    if (collision == COLLISION_NONE)
	    CheckGroundRocksCollision(metatileBehavior, &collision);
	
    return collision;
}

static bool8 CanStopSurfing(s16 x, s16 y, u8 direction)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && MapGridGetZCoordAt(x, y) == 3 && GetObjectEventIdByXYZ(x, y, 3) == OBJECT_EVENTS_COUNT && !MetatileBehavior_IsGroundRocks(MapGridGetMetatileBehaviorAt(x, y)))
    {
        CreateStopSurfingTask(direction);
        return TRUE;
    }
    else
        return FALSE;
}

static bool8 TryPushBoulder(s16 x, s16 y, u8 direction)
{
    u8 objectEventId, behavior;
	
    if (!FlagGet(FLAG_SYS_USE_STRENGTH))
        return FALSE;

    objectEventId = GetObjectEventIdByXY(x, y);
	
    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].graphicsId != OBJ_EVENT_GFX_PUSHABLE_BOULDER)
        return FALSE;

    x = gObjectEvents[objectEventId].currentCoords.x;
    y = gObjectEvents[objectEventId].currentCoords.y;
	
    MoveCoords(direction, &x, &y);
    behavior = MapGridGetMetatileBehaviorAt(x, y);
	
    if (behavior == MB_FALL_WARP || behavior == MB_STRENGTH_HOLE || (GetCollisionAtCoords(&gObjectEvents[objectEventId], x, y, direction) == COLLISION_NONE
	&& !MetatileBehavior_IsNonAnimDoor(behavior) && !MetatileBehavior_IsGroundRocks(behavior)))
    {
        StartStrengthAnim(objectEventId, direction);
        return TRUE;
    }
    else
        return FALSE;
}

//////////////////////
// PLAYER COLLISION //
//////////////////////

static bool8 (*const sAcroBikeTrickMetatiles[])(u8) = {
    MetatileBehavior_IsBumpySlope,
    MetatileBehavior_IsIsolatedVerticalRail,
    MetatileBehavior_IsIsolatedHorizontalRail,
    MetatileBehavior_IsVerticalRail,
    MetatileBehavior_IsHorizontalRail
};

static const u8 sAcroBikeTrickCollisionTypes[] = {
    COLLISION_WHEELIE_HOP,
    COLLISION_ISOLATED_VERTICAL_RAIL,
    COLLISION_ISOLATED_HORIZONTAL_RAIL,
    COLLISION_VERTICAL_RAIL,
    COLLISION_HORIZONTAL_RAIL,
};

static void CheckAcroBikeCollision(s16 x, s16 y, u8 metatileBehavior, u8 *collision)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sAcroBikeTrickMetatiles); i++)
    {
        if (sAcroBikeTrickMetatiles[i](metatileBehavior))
        {
            *collision = sAcroBikeTrickCollisionTypes[i];
            return;
        }
    }
}

static void CheckGroundRocksCollision(u8 metatileBehavior, u8 *collision)
{
	if (MetatileBehavior_IsGroundRocks(metatileBehavior))
		*collision = COLLISION_GROUND_ROCKS;
}	

//////////////////////////////
// PLAYER AVATAR TRANSITION //
//////////////////////////////

static void (*const sPlayerAvatarTransitionFuncs[])(struct ObjectEvent *) =
{
    [PLAYER_AVATAR_STATE_NORMAL]         = PlayerAvatarTransition_Normal,
    [PLAYER_AVATAR_STATE_MACH_BIKE]      = PlayerAvatarTransition_Bike,
    [PLAYER_AVATAR_STATE_ACRO_BIKE]      = PlayerAvatarTransition_Bike,
    [PLAYER_AVATAR_STATE_SURFING]        = PlayerAvatarTransition_Surfing,
    [PLAYER_AVATAR_STATE_UNDERWATER]     = PlayerAvatarTransition_Underwater,
    [PLAYER_AVATAR_STATE_CONTROLLABLE]   = PlayerAvatarTransition_ReturnToField,
    [PLAYER_AVATAR_STATE_FORCED]         = PlayerAvatarTransition_Dummy,
    [PLAYER_AVATAR_STATE_DASH]           = PlayerAvatarTransition_Dummy,
    [PLAYER_AVATAR_STATE_TAUROS_RIDE]    = PlayerAvatarTransition_TaurosRide,
    [PLAYER_AVATAR_STATE_STOUTLAND_RIDE] = PlayerAvatarTransition_StoutlandRide,
    [PLAYER_AVATAR_STATE_MUDSDALE_RIDE]  = PlayerAvatarTransition_MudsdaleRide,
    [PLAYER_AVATAR_STATE_MACHAMP_RIDE]   = PlayerAvatarTransition_MachampRide
};

void SetPlayerAvatarTransitionFlags(u16 flags)
{
    gPlayerAvatar.transitionFlags |= flags;
    DoPlayerAvatarTransition();
}

static void DoPlayerAvatarTransition(void)
{
    u8 i;
    u16 flags = gPlayerAvatar.transitionFlags;

    if (flags != 0)
    {
        for (i = 0; i < ARRAY_COUNT(sPlayerAvatarTransitionFuncs); i++, flags >>= 1)
        {
            if (flags & 1)
                sPlayerAvatarTransitionFuncs[i](&gObjectEvents[gPlayerAvatar.objectEventId]);
        }
        gPlayerAvatar.transitionFlags = 0;
    }
}

static void UpdatePlayerSprite(struct ObjectEvent * playerObjEvent, u8 state, u16 mask)
{
	ObjectEventSetGraphicsId(playerObjEvent, GetPlayerAvatarGraphicsIdByStateId(state));
	ObjectEventTurn(playerObjEvent, playerObjEvent->movementDirection);
	SetPlayerAvatarStateMask(mask);
}

static void PlayerAvatarTransition_Dummy(struct ObjectEvent * playerObjEvent)
{

}

static void PlayerAvatarTransition_Normal(struct ObjectEvent * playerObjEvent)
{
	UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_NORMAL, PLAYER_AVATAR_FLAG_ON_FOOT);
}

static void PlayerAvatarTransition_Bike(struct ObjectEvent * playerObjEvent)
{
    UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_BIKE, PLAYER_AVATAR_FLAG_MACH_BIKE);
    BikeClearState(0, 0);
}

static void PlayerAvatarTransition_Surfing(struct ObjectEvent * playerObjEvent)
{
	u8 fieldEffectId;
	
	if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
	{
		UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_RIDE, PLAYER_AVATAR_FLAG_SURFING);
		
		gFieldEffectArguments[0] = playerObjEvent->currentCoords.x;
		gFieldEffectArguments[1] = playerObjEvent->currentCoords.y;
		gFieldEffectArguments[2] = gPlayerAvatar.objectEventId;
		fieldEffectId = FieldEffectStart(FLDEFF_SURF_BLOB);
		playerObjEvent->fieldEffectSpriteId = fieldEffectId;
		SetSurfBlob_BobState(fieldEffectId, 1);
	}
}

static void PlayerAvatarTransition_Underwater(struct ObjectEvent * playerObjEvent)
{

}

static void PlayerAvatarTransition_ReturnToField(struct ObjectEvent * playerObjEvent)
{
    gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_CONTROLLABLE;
}

static void PlayerAvatarTransition_TaurosRide(struct ObjectEvent * playerObjEvent)
{
	UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_TAUROS_RIDE, PLAYER_AVATAR_FLAG_TAUROS_RIDE);
	BikeClearState(0, 0);
}

static void PlayerAvatarTransition_StoutlandRide(struct ObjectEvent * playerObjEvent)
{
	UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_STOUTLAND_RIDE, PLAYER_AVATAR_FLAG_STOUTLAND_RIDE);
	BikeClearState(0, 0);
}

static void PlayerAvatarTransition_MudsdaleRide(struct ObjectEvent * playerObjEvent)
{
	UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_MUDSDALE_RIDE, PLAYER_AVATAR_FLAG_MUDSDALE_RIDE);
	BikeClearState(0, 0);
}

static void PlayerAvatarTransition_MachampRide(struct ObjectEvent * playerObjEvent)
{
	UpdatePlayerSprite(playerObjEvent, PLAYER_AVATAR_GFX_MACHAMP_RIDE, PLAYER_AVATAR_FLAG_MACHAMP_RIDE);
	BikeClearState(0, 0);
}
	
void UpdatePlayerAvatarTransitionState(void)
{
    gPlayerAvatar.tileTransitionState = T_NOT_MOVING;
	
    if (PlayerIsAnimActive())
    {
        if (!PlayerCheckIfAnimFinishedOrInactive())
        {
            if (!PlayerAnimIsMultiFrameStationary())
                gPlayerAvatar.tileTransitionState = T_TILE_TRANSITION;
        }
        else
        {
            if (!PlayerAnimIsMultiFrameStationaryAndStateNotTurning())
                gPlayerAvatar.tileTransitionState = T_TILE_CENTER;
        }
    }
}

static bool8 PlayerAnimIsMultiFrameStationary(void)
{
    u8 movementActionId = gObjectEvents[gPlayerAvatar.objectEventId].movementActionId;

    if (movementActionId <= MOVEMENT_ACTION_FACE_RIGHT_FAST || (movementActionId >= MOVEMENT_ACTION_DELAY_1 && movementActionId <= MOVEMENT_ACTION_DELAY_16)
	|| (movementActionId >= MOVEMENT_ACTION_WALK_IN_PLACE_SLOW_DOWN && movementActionId <= MOVEMENT_ACTION_WALK_IN_PLACE_FASTEST_RIGHT)
	|| (movementActionId >= MOVEMENT_ACTION_ACRO_WHEELIE_FACE_DOWN && movementActionId <= MOVEMENT_ACTION_ACRO_END_WHEELIE_FACE_RIGHT)
	|| (movementActionId >= MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_DOWN && movementActionId <= MOVEMENT_ACTION_ACRO_WHEELIE_IN_PLACE_RIGHT))
        return TRUE;
    else
        return FALSE;
}

static bool8 PlayerAnimIsMultiFrameStationaryAndStateNotTurning(void)
{
	return (PlayerAnimIsMultiFrameStationary() && gPlayerAvatar.runningState != TURN_DIRECTION);
}

///////////////////////
// PLAYER ANIMATIONS //
///////////////////////

static bool8 PlayerIsAnimActive(void)
{
    return ObjectEventIsMovementOverridden(&gObjectEvents[gPlayerAvatar.objectEventId]);
}

static bool8 PlayerCheckIfAnimFinishedOrInactive(void)
{
    return ObjectEventCheckHeldMovementStatus(&gObjectEvents[gPlayerAvatar.objectEventId]);
}

static void PlayerForceSetHeldMovement(u8 a)
{
    ObjectEventForceSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], a);
}

static void PlayerSetAnimId(u8 movementActionId, u8 copyableMovement)
{
    if (!PlayerIsAnimActive())
    {
        gObjectEvents[gPlayerAvatar.objectEventId].playerCopyableMovement = copyableMovement;
        ObjectEventSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], movementActionId);
    }
}

void PlayerGoSlowest(u8 direction)
{
    PlayerSetAnimId(GetWalkSlowestMovementAction(direction), 2);
}

void PlayerGoSlow(u8 direction)
{
    PlayerSetAnimId(GetWalkSlowMovementAction(direction), 2);
}

void PlayerGoSpeed1(u8 direction)
{
    PlayerSetAnimId(GetWalkNormalMovementAction(direction), 2);
}

void PlayerGoSpeed2(u8 direction)
{
    PlayerSetAnimId(GetWalkFastMovementAction(direction), 2);
}

void PlayerRideWaterCurrent(u8 direction)
{
    PlayerSetAnimId(GetRideWaterCurrentMovementAction(direction), 2);
}

// fastest speed (4 speed)
void PlayerGoSpeed4(u8 direction)
{
    PlayerSetAnimId(GetWalkFastestMovementAction(direction), 2);
}

void PlayerRun(u8 direction)
{
    PlayerSetAnimId(GetPlayerRunMovementAction(direction), 2);
}

void PlayerRunSlow(u8 direction)
{
    PlayerSetAnimId(GetPlayerRunSlowMovementAction(direction), 2);
}

void PlayerOnBikeCollide(u8 direction)
{
    PlayCollisionSoundIfNotFacingWarp(direction);
    PlayerSetAnimId(GetWalkInPlaceNormalMovementAction(direction), 2);
}

void PlayerNotOnBikeCollide(u8 direction)
{
    PlayCollisionSoundIfNotFacingWarp(direction);
    PlayerSetAnimId(GetStepInPlaceDelay32AnimId(direction), 2);
}

void PlayerFaceDirection(u8 direction)
{
    PlayerSetAnimId(GetFaceDirectionMovementAction(direction), 1);
}

void PlayerFaceDirectionFast(u8 direction)
{
    PlayerSetAnimId(GetFaceDirectionFastMovementAction(direction), 1);
}

void PlayerTurnInPlace(u8 direction)
{
    PlayerSetAnimId(GetWalkInPlaceSlowMovementAction(direction), 1);
}

void PlayerJumpLedge(u8 direction)
{
    PlaySE(SE_LEDGE);
    PlayerSetAnimId(GetJump2MovementAction(direction), 8);
}

void HandleEnforcedLookDirectionOnPlayerStopMoving(void)
{
    if (gPlayerAvatar.tileTransitionState == T_TILE_CENTER || gPlayerAvatar.tileTransitionState == T_NOT_MOVING)
    {
        if (IsPlayerNotUsingAcroBikeOnBumpySlope())
            PlayerForceSetHeldMovement(GetFaceDirectionMovementAction(gObjectEvents[gPlayerAvatar.objectEventId].facingDirection));
    }
}

static void PlayerGoSpin(u8 direction)
{
    PlayerSetAnimId(GetSpinMovementAction(direction), 3);
}

static void PlayerIdleWheelie(u8 direction)
{
    PlayerSetAnimId(GetAcroWheelieFaceDirectionMovementAction(direction), 1);
}

static void PlayerStartWheelie(u8 direction)
{
    PlayerSetAnimId(GetAcroPopWheelieFaceDirectionMovementAction(direction), 1);
}

static void PlayerEndWheelie(u8 direction)
{
    PlayerSetAnimId(GetAcroEndWheelieFaceDirectionMovementAction(direction), 1);
}

static inline void PlayBikeHopSound(void)
{
	PlaySE(SE_BIKE_HOP);
}

static void PlayerStandingHoppingWheelie(u8 direction)
{
    PlayBikeHopSound();
    PlayerSetAnimId(GetAcroWheelieHopFaceDirectionMovementAction(direction), 1);
}

static void PlayerMovingHoppingWheelie(u8 direction)
{
    PlayBikeHopSound();
    PlayerSetAnimId(GetAcroWheelieHopDirectionMovementAction(direction), 2);
}

static void PlayerLedgeHoppingWheelie(u8 direction)
{
    PlayBikeHopSound();
    PlayerSetAnimId(GetAcroWheelieJumpDirectionMovementAction(direction), 8);
}

static void PlayerAcroTurnJump(u8 direction)
{
    PlayBikeHopSound();
    PlayerSetAnimId(GetJumpInPlaceTurnAroundMovementAction(direction), 1);
}

static void PlayerAcroWheelieCollide(u8 direction)
{
    PlaySE(SE_WALL_HIT);
    PlayerSetAnimId(GetAcroWheelieInPlaceDirectionMovementAction(direction), 2);
}

static void PlayerAcroPopWheelie(u8 direction)
{
    PlayerSetAnimId(GetAcroPopWheelieMoveDirectionMovementAction(direction), 2);
}

static void PlayerAcroWheelieMove(u8 direction)
{
    PlayerSetAnimId(GetAcroWheelieMoveDirectionMovementAction(direction), 2);
}

static bool8 (*const sArrowWarpMetatileBehaviorChecks[])(u8) = {
    MetatileBehavior_IsSouthArrowWarp,
    MetatileBehavior_IsNorthArrowWarp,
    MetatileBehavior_IsWestArrowWarp,
    MetatileBehavior_IsEastArrowWarp
};

static void PlayCollisionSoundIfNotFacingWarp(u8 direction)
{
    s16 x, y;
    u8 metatileBehavior = gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior;

    if (!sArrowWarpMetatileBehaviorChecks[direction - 1](metatileBehavior))
    {
        if (direction == DIR_WEST)
        {
            if (MetatileBehavior_IsDirectionalUpLeftStairWarp(metatileBehavior) || MetatileBehavior_IsDirectionalDownLeftStairWarp(metatileBehavior))
                return;
        }
        if (direction == DIR_EAST)
        {
            if (MetatileBehavior_IsDirectionalUpRightStairWarp(metatileBehavior) || MetatileBehavior_IsDirectionalDownRightStairWarp(metatileBehavior))
                return;
        }
        if (direction == DIR_NORTH)
        {
            PlayerGetDestCoords(&x, &y);
            MoveCoords(direction, &x, &y);

            if (MetatileBehavior_IsWarpDoor(MapGridGetMetatileBehaviorAt(x, y)))
                return;
        }
        PlaySE(SE_WALL_HIT);
    }
}

void GetXYCoordsOneStepInFrontOfPlayer(s16 *x, s16 *y)
{
    PlayerGetDestCoords(x, y);
    MoveCoords(GetPlayerFacingDirection(), x, y);
}

void PlayerGetDestCoords(s16 *x, s16 *y)
{
    *x = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.x;
    *y = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords.y;
}

u8 player_get_pos_including_state_based_drift(s16 *x, s16 *y)
{
    struct ObjectEvent *object = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (object->heldMovementActive && !object->heldMovementFinished && !gSprites[object->spriteId].data[2])
    {
        *x = object->currentCoords.x;
        *y = object->currentCoords.y;

        switch (object->movementActionId)
        {
        case MOVEMENT_ACTION_WALK_NORMAL_DOWN:
        case MOVEMENT_ACTION_PLAYER_RUN_DOWN:
            (*y)++;
            return TRUE;
        case MOVEMENT_ACTION_WALK_NORMAL_UP:
        case MOVEMENT_ACTION_PLAYER_RUN_UP:
            (*y)--;
            return TRUE;
        case MOVEMENT_ACTION_WALK_NORMAL_LEFT:
        case MOVEMENT_ACTION_PLAYER_RUN_LEFT:
            (*x)--;
            return TRUE;
        case MOVEMENT_ACTION_WALK_NORMAL_RIGHT:
        case MOVEMENT_ACTION_PLAYER_RUN_RIGHT:
            (*x)++;
            return TRUE;
        }
    }
    *x = -1;
    *y = -1;
    return FALSE;
}

u8 GetPlayerFacingDirection(void)
{
    return gObjectEvents[gPlayerAvatar.objectEventId].facingDirection;
}

void MovePlayerToMapCoords(s16 x, s16 y)
{
    MoveObjectEventToMapCoords(&gObjectEvents[gPlayerAvatar.objectEventId], x, y);
}

void StopPlayerAvatar(void)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    npc_clear_strange_bits(playerObjEvent);
    SetObjectEventDirection(playerObjEvent, playerObjEvent->facingDirection);
	
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
    {
        Bike_HandleBumpySlopeJump();
        Bike_UpdateBikeCounterSpeed(0);
    }
}

///////////////////////
// PLAYER AVATAR GFX //
///////////////////////

static const u8 sPlayerAvatarGfxIds[][GENDER_COUNT] = {
    [PLAYER_AVATAR_GFX_NORMAL]         = {OBJ_EVENT_GFX_RED_NORMAL,     OBJ_EVENT_GFX_GREEN_NORMAL},
    [PLAYER_AVATAR_GFX_BIKE]           = {OBJ_EVENT_GFX_RED_BIKE,       OBJ_EVENT_GFX_GREEN_BIKE},
    [PLAYER_AVATAR_GFX_RIDE]           = {OBJ_EVENT_GFX_RED_SURF,       OBJ_EVENT_GFX_GREEN_SURF},
    [PLAYER_AVATAR_GFX_FIELD_MOVE]     = {OBJ_EVENT_GFX_RED_FIELD_MOVE, OBJ_EVENT_GFX_GREEN_FIELD_MOVE},
    [PLAYER_AVATAR_GFX_FISH]           = {OBJ_EVENT_GFX_RED_FISH,       OBJ_EVENT_GFX_GREEN_FISH},
    [PLAYER_AVATAR_GFX_VSSEEKER]       = {OBJ_EVENT_GFX_RED_VS_SEEKER,  OBJ_EVENT_GFX_GREEN_VS_SEEKER},
    [PLAYER_AVATAR_GFX_TAUROS_RIDE]    = {OBJ_EVENT_GFX_RED_TAUROS,     OBJ_EVENT_GFX_GREEN_TAUROS},
    [PLAYER_AVATAR_GFX_STOUTLAND_RIDE] = {OBJ_EVENT_GFX_RED_STOUTLAND,  OBJ_EVENT_GFX_GREEN_STOUTLAND},
    [PLAYER_AVATAR_GFX_MUDSDALE_RIDE]  = {OBJ_EVENT_GFX_RED_MUDSDALE,   OBJ_EVENT_GFX_GREEN_MUDSDALE},
    [PLAYER_AVATAR_GFX_MACHAMP_RIDE]   = {OBJ_EVENT_GFX_RED_MACHAMP,    OBJ_EVENT_GFX_GREEN_MACHAMP},
};

static const u8 sHoennLinkPartnerGfxIds[] = {
    OBJ_EVENT_GFX_RS_BRENDAN,
    OBJ_EVENT_GFX_RS_MAY
};

u8 GetPlayerAvatarGraphicsIdByStateIdAndGender(u8 state, u8 gender)
{
    return sPlayerAvatarGfxIds[state][gender];
}

u8 GetRSAvatarGraphicsIdByGender(u8 gender)
{
    return sHoennLinkPartnerGfxIds[gender];
}

u8 GetPlayerAvatarGraphicsIdByStateId(u8 state)
{
    return GetPlayerAvatarGraphicsIdByStateIdAndGender(state, gPlayerAvatar.gender);
}

u8 GetPlayerAvatarGenderByGraphicsId(u8 gfxId)
{
	u32 i;
	
	for (i = 0; i < ARRAY_COUNT(sPlayerAvatarGfxIds); i++)
	{
		if (sPlayerAvatarGfxIds[i][FEMALE] == gfxId)
			return FEMALE;
	}
	return MALE;
}

bool8 IsPlayerSurfingNorth(void)
{
	return (GetPlayerMovementDirection() == DIR_NORTH && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING));
}

bool8 IsPlayerFacingSurfableFishableWater(void)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x = playerObjEvent->currentCoords.x;
    s16 y = playerObjEvent->currentCoords.y;

    MoveCoords(playerObjEvent->facingDirection, &x, &y);
	
    if (GetCollisionAtCoords(playerObjEvent, x, y, playerObjEvent->facingDirection) == COLLISION_ELEVATION_MISMATCH && PlayerGetZCoord() == 3 && MetatileAtCoordsIsWaterTile(x, y))
        return TRUE;
    else
        return FALSE;
}

bool8 MetatileAtCoordsIsWaterTile(s16 x, s16 y)
{
    return TestMetatileAttributeBit(MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_TERRAIN), TILE_TERRAIN_WATER);
}

void ClearPlayerAvatarInfo(void)
{
    gPlayerAvatar = (struct PlayerAvatar){};
}

void SetPlayerAvatarStateMask(u16 flags)
{
    gPlayerAvatar.flags &= (PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_FORCED | PLAYER_AVATAR_FLAG_CONTROLLABLE);
    gPlayerAvatar.flags |= flags;
}

static const u16 sPlayerAvatarGfxToStateFlag[][7][GENDER_COUNT] = {
    [MALE] = {
        {OBJ_EVENT_GFX_RED_NORMAL,    PLAYER_AVATAR_FLAG_ON_FOOT},
        {OBJ_EVENT_GFX_RED_BIKE,      PLAYER_AVATAR_FLAG_MACH_BIKE},
        {OBJ_EVENT_GFX_RED_SURF,      PLAYER_AVATAR_FLAG_SURFING},
		{OBJ_EVENT_GFX_RED_TAUROS,    PLAYER_AVATAR_FLAG_TAUROS_RIDE},
		{OBJ_EVENT_GFX_RED_STOUTLAND, PLAYER_AVATAR_FLAG_STOUTLAND_RIDE},
		{OBJ_EVENT_GFX_RED_MUDSDALE,  PLAYER_AVATAR_FLAG_MUDSDALE_RIDE},
		{OBJ_EVENT_GFX_RED_MACHAMP,   PLAYER_AVATAR_FLAG_MACHAMP_RIDE}
    },
    [FEMALE] = {
        {OBJ_EVENT_GFX_GREEN_NORMAL,    PLAYER_AVATAR_FLAG_ON_FOOT},
        {OBJ_EVENT_GFX_GREEN_BIKE,      PLAYER_AVATAR_FLAG_MACH_BIKE},
        {OBJ_EVENT_GFX_GREEN_SURF,      PLAYER_AVATAR_FLAG_SURFING},
		{OBJ_EVENT_GFX_GREEN_TAUROS,    PLAYER_AVATAR_FLAG_TAUROS_RIDE},
		{OBJ_EVENT_GFX_GREEN_STOUTLAND, PLAYER_AVATAR_FLAG_STOUTLAND_RIDE},
		{OBJ_EVENT_GFX_GREEN_MUDSDALE,  PLAYER_AVATAR_FLAG_MUDSDALE_RIDE},
		{OBJ_EVENT_GFX_GREEN_MACHAMP,   PLAYER_AVATAR_FLAG_MACHAMP_RIDE}
    }
};

u16 GetPlayerAvatarStateTransitionByGraphicsId(u8 graphicsId, u8 gender)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(*sPlayerAvatarGfxToStateFlag); i++)
    {
        if (sPlayerAvatarGfxToStateFlag[gender][i][0] == graphicsId)
            return sPlayerAvatarGfxToStateFlag[gender][i][1];
    }
    return 1;
}

u8 GetPlayerAvatarGraphicsIdByCurrentState(void)
{
    u8 i;
    u16 flags = gPlayerAvatar.flags;

    for (i = 0; i < ARRAY_COUNT(*sPlayerAvatarGfxToStateFlag); i++)
    {
        if (sPlayerAvatarGfxToStateFlag[gPlayerAvatar.gender][i][1] & flags)
            return sPlayerAvatarGfxToStateFlag[gPlayerAvatar.gender][i][0];
    }
    return 0;
}

void SetPlayerAvatarExtraStateTransition(u8 graphicsId, u8 extras)
{
    gPlayerAvatar.transitionFlags |= GetPlayerAvatarStateTransitionByGraphicsId(graphicsId, gPlayerAvatar.gender) | extras;
    DoPlayerAvatarTransition();
}

void InitPlayerAvatar(s16 x, s16 y, u8 direction, u8 gender)
{
    struct ObjectEventTemplate playerObjEventTemplate;
    u8 objectEventId;
    struct ObjectEvent *objectEvent;

    playerObjEventTemplate.localId = OBJ_EVENT_ID_PLAYER;
    playerObjEventTemplate.graphicsId = GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_GFX_NORMAL, gender);
    playerObjEventTemplate.x = x - 7;
    playerObjEventTemplate.y = y - 7;
    playerObjEventTemplate.objUnion.normal.elevation = 0;
    playerObjEventTemplate.objUnion.normal.movementType = MOVEMENT_TYPE_PLAYER;
    playerObjEventTemplate.objUnion.normal.movementRangeX = 0;
    playerObjEventTemplate.objUnion.normal.movementRangeY = 0;
    playerObjEventTemplate.objUnion.normal.trainerType = TRAINER_TYPE_NONE;
    playerObjEventTemplate.objUnion.normal.trainerRange_berryTreeId = 0;
    playerObjEventTemplate.script = NULL;
    playerObjEventTemplate.flagId = 0;
	
    objectEventId = SpawnSpecialObjectEvent(&playerObjEventTemplate);
    objectEvent = &gObjectEvents[objectEventId];
    objectEvent->isPlayer = TRUE;
    objectEvent->warpArrowSpriteId = CreateWarpArrowSprite();
    ObjectEventTurn(objectEvent, direction);
	
    ClearPlayerAvatarInfo();
    gPlayerAvatar.runningState = NOT_MOVING;
    gPlayerAvatar.tileTransitionState = T_NOT_MOVING;
    gPlayerAvatar.objectEventId = objectEventId;
    gPlayerAvatar.spriteId = objectEvent->spriteId;
    gPlayerAvatar.gender = gender;
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_CONTROLLABLE | PLAYER_AVATAR_FLAG_ON_FOOT);
}

void SetPlayerInvisibility(bool8 invisible)
{
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = invisible;
	
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        gSprites[gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId].invisible = invisible;
}

void StartPlayerAvatarSummonMonForFieldMoveAnim(void)
{
    ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_FIELD_MOVE));
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], ANIM_FIELD_MOVE);
}

static const u8 sPlayerAvatarVsSeekerBikeGfxIds[] = {
    OBJ_EVENT_GFX_RED_VS_SEEKER_BIKE,
    OBJ_EVENT_GFX_GREEN_VS_SEEKER_BIKE
};

u8 GetPlayerAvatarVsSeekerGfxId(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        return sPlayerAvatarVsSeekerBikeGfxIds[gPlayerAvatar.gender];
    else
        return GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_VSSEEKER);
}

void StartPlayerAvatarVsSeekerAnim(void)
{
    ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], GetPlayerAvatarVsSeekerGfxId());
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], ANIM_VS_SEEKER);
}

void StartPlayerAvatarFishAnim(u8 direction)
{
	struct ObjectEvent *objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
	ObjectEventSetGraphicsId(objectEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_FISH));
	StartSpriteAnim(&gSprites[objectEvent->spriteId], GetFishingDirectionAnimNum(objectEvent->facingDirection));
}

// Stubbed from R/S
void PlayerUseAcroBikeOnBumpySlope(u8 direction)
{

}

////////////////////////
// PLAYER WARP ARROWS //
////////////////////////

static bool8 (*const sArrowWarpMetatileBehaviorChecks2[])(u8) = {
    MetatileBehavior_IsSouthArrowWarp,
    MetatileBehavior_IsNorthArrowWarp,
    MetatileBehavior_IsWestArrowWarp,
    MetatileBehavior_IsEastArrowWarp
};

static void HandleWarpArrowSpriteHideShow(struct ObjectEvent *objectEvent)
{
    s16 x;
    s16 y;
    u8 direction, metatileBehavior = objectEvent->currentMetatileBehavior;

    for (x = 0, direction = DIR_SOUTH; x < 4; x++, direction++)
    {
        if (sArrowWarpMetatileBehaviorChecks2[x](metatileBehavior) && direction == objectEvent->movementDirection)
        {
            x = objectEvent->currentCoords.x;
            y = objectEvent->currentCoords.y;
            MoveCoords(direction, &x, &y);
            ShowWarpArrowSprite(objectEvent->warpArrowSpriteId, direction, x, y);
            return;
        }
    }
    gSprites[objectEvent->warpArrowSpriteId].invisible = TRUE;
}

//////////////////////
// STRENGTH BOULDER //
//////////////////////

static bool8 (*const sBoulderTaskSteps[])(struct Task * task, struct ObjectEvent * playerObj, struct ObjectEvent * boulderObj) = {
    DoBoulderInit,
    DoBoulderDust,
    DoBoulderFinish
};

static void StartStrengthAnim(u8 a, u8 b)
{
    u8 taskId = CreateTask(Task_BumpBoulder, 0xFF);
    gTasks[taskId].data[1] = a;
    gTasks[taskId].data[2] = b;
    Task_BumpBoulder(taskId);
}

static void Task_BumpBoulder(u8 taskId)
{
    while (sBoulderTaskSteps[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId], &gObjectEvents[gTasks[taskId].data[1]]));
}

static bool8 DoBoulderInit(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject)
{
    ScriptContext2_Enable();
    gPlayerAvatar.preventStep = TRUE;
    task->data[0]++;
    return FALSE;
}

static bool8 DoBoulderDust(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject)
{
    if (!ObjectEventIsMovementOverridden(playerObject) && !ObjectEventIsMovementOverridden(strengthObject))
    {
        ObjectEventClearHeldMovementIfFinished(playerObject);
        ObjectEventClearHeldMovementIfFinished(strengthObject);
		
		ObjectEventSetHeldMovement(playerObject, GetWalkInPlaceNormalMovementAction((u8)task->data[2]));
        ObjectEventSetHeldMovement(strengthObject, GetWalkSlowestMovementAction((u8)task->data[2]));
		
        gFieldEffectArguments[0] = strengthObject->currentCoords.x;
        gFieldEffectArguments[1] = strengthObject->currentCoords.y;
        gFieldEffectArguments[2] = strengthObject->previousElevation;
        gFieldEffectArguments[3] = gSprites[strengthObject->spriteId].oam.priority;
        FieldEffectStart(FLDEFF_DUST);
        PlaySE(SE_M_STRENGTH);
        task->data[0]++;
    }
    return FALSE;
}

static bool8 DoBoulderFinish(struct Task *task, struct ObjectEvent *playerObject, struct ObjectEvent *strengthObject)
{
    if (ObjectEventCheckHeldMovementStatus(playerObject) && ObjectEventCheckHeldMovementStatus(strengthObject))
    {
        ObjectEventClearHeldMovementIfFinished(playerObject);
        ObjectEventClearHeldMovementIfFinished(strengthObject);
		
        HandleBoulderFallThroughOrCoverHole(strengthObject);
        HandleBoulderActivateVictoryRoadSwitch(strengthObject->currentCoords.x, strengthObject->currentCoords.y);
		
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        DestroyTask(FindTaskIdByFunc(Task_BumpBoulder));
    }
    return FALSE;
}

///////////////////////////////////
// SECRET BASE MAT JUMP AND SPIN //
///////////////////////////////////

static bool8 (*const sPlayerAvatarSecretBaseMatJump[])(struct Task *, struct ObjectEvent *) = {
    PlayerAvatar_DoSecretBaseMatJump
};

static void DoPlayerMatJump(void)
{
    DoPlayerAvatarSecretBaseMatJump(CreateTask(DoPlayerAvatarSecretBaseMatJump, 0xFF));
}

static void DoPlayerAvatarSecretBaseMatJump(u8 taskId)
{
    while (sPlayerAvatarSecretBaseMatJump[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId]));
}

// because data[0] is used to call this, it can be inferred that there may have been multiple mat jump functions at one point, so the name for these groups of functions is appropriate in assuming the sole use of mat jump.
static bool8 PlayerAvatar_DoSecretBaseMatJump(struct Task *task, struct ObjectEvent *objectEvent)
{
    gPlayerAvatar.preventStep = TRUE;
	
    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        PlaySE(SE_LEDGE);
        ObjectEventSetHeldMovement(objectEvent, GetJumpInPlaceMovementAction(objectEvent->facingDirection));

        if (++task->data[1] > 1)
        {
            gPlayerAvatar.preventStep = FALSE;
            gPlayerAvatar.transitionFlags |= PLAYER_AVATAR_FLAG_CONTROLLABLE;
            DestroyTask(FindTaskIdByFunc(DoPlayerAvatarSecretBaseMatJump));
        }
    }
    return FALSE;
}

static bool8 (*const sPlayerAvatarSecretBaseMatSpin[])(struct Task * task, struct ObjectEvent * playerObj) = {
    PlayerAvatar_SecretBaseMatSpinStep0,
    PlayerAvatar_SecretBaseMatSpinStep1,
    PlayerAvatar_SecretBaseMatSpinStep2,
    PlayerAvatar_SecretBaseMatSpinStep3,
};

static void DoPlayerMatSpin(void)
{
    PlayerAvatar_DoSecretBaseMatSpin(CreateTask(PlayerAvatar_DoSecretBaseMatSpin, 0xFF));
}

static void PlayerAvatar_DoSecretBaseMatSpin(u8 taskId)
{
    while (sPlayerAvatarSecretBaseMatSpin[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId]));
}

static bool8 PlayerAvatar_SecretBaseMatSpinStep0(struct Task *task, struct ObjectEvent *objectEvent)
{
    task->data[0]++;
    task->data[1] = objectEvent->movementDirection;
    gPlayerAvatar.preventStep = TRUE;
    ScriptContext2_Enable();
    PlaySE(SE_WARP_IN);
    return TRUE;
}

static bool8 PlayerAvatar_SecretBaseMatSpinStep1(struct Task *task, struct ObjectEvent *objectEvent)
{
    u8 directions[] = {DIR_WEST, DIR_EAST, DIR_NORTH, DIR_SOUTH};

    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        u8 direction;

        ObjectEventSetHeldMovement(objectEvent, GetFaceDirectionMovementAction(direction = directions[objectEvent->movementDirection - 1]));
		
        if (direction == (u8)task->data[1])
            task->data[2]++;
		
        task->data[0]++;
		
        if (task->data[2] > 3 && direction == GetOppositeDirection(task->data[1]))
            task->data[0]++;
    }
    return FALSE;
}

static bool8 PlayerAvatar_SecretBaseMatSpinStep2(struct Task *task, struct ObjectEvent *objectEvent)
{
    const u8 actions[] = {
        MOVEMENT_ACTION_DELAY_1,
        MOVEMENT_ACTION_DELAY_1,
        MOVEMENT_ACTION_DELAY_2,
        MOVEMENT_ACTION_DELAY_4,
        MOVEMENT_ACTION_DELAY_8,
    };

    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        ObjectEventSetHeldMovement(objectEvent, actions[task->data[2]]);
        task->data[0] = 1;
    }
    return FALSE;
}

static bool8 PlayerAvatar_SecretBaseMatSpinStep3(struct Task *task, struct ObjectEvent *objectEvent)
{
    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        ObjectEventSetHeldMovement(objectEvent, GetWalkSlowestMovementAction(GetOppositeDirection(task->data[1])));
        ScriptContext2_Disable();
        gPlayerAvatar.preventStep = FALSE;
        DestroyTask(FindTaskIdByFunc(PlayerAvatar_DoSecretBaseMatSpin));
    }
    return FALSE;
}

/////////////////////////
// PLAYER STOP SURFING //
/////////////////////////

static void CreateStopSurfingTask(u8 direction)
{
    u8 taskId;

    ScriptContext2_Enable();
    FreezeObjectEvents();
	
    Overworld_ClearSavedMusic();
    Overworld_ChangeMusicToDefault();
	
    gPlayerAvatar.flags &= ~(PLAYER_AVATAR_FLAG_SURFING);
    gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_ON_FOOT;
    gPlayerAvatar.preventStep = TRUE;
	
    taskId = CreateTask(Task_StopSurfingInit, 0xFF);
    gTasks[taskId].data[0] = direction;
    Task_StopSurfingInit(taskId);
}

void SeafoamIslandsB4F_CurrentDumpsPlayerOnLand(void)
{
	CreateStopSurfingTask(DIR_NORTH);
}

static void Task_StopSurfingInit(u8 taskId)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (ObjectEventIsMovementOverridden(playerObjEvent))
    {
        if (!ObjectEventClearHeldMovementIfFinished(playerObjEvent))
            return;
    }
    SetSurfBlob_BobState(playerObjEvent->fieldEffectSpriteId, 2);
    ObjectEventSetHeldMovement(playerObjEvent, sub_80641EC((u8)gTasks[taskId].data[0]));
    gTasks[taskId].func = Task_WaitStopSurfing;
}

static void Task_WaitStopSurfing(u8 taskId)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (ObjectEventClearHeldMovementIfFinished(playerObjEvent))
    {
        ObjectEventSetGraphicsId(playerObjEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_NORMAL));
        ObjectEventSetHeldMovement(playerObjEvent, GetFaceDirectionMovementAction(playerObjEvent->facingDirection));
		
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
		
        DestroySprite(&gSprites[playerObjEvent->fieldEffectSpriteId]);
        DestroyTask(taskId);
    }
}

////////////////////
// PLAYER FISHING //
////////////////////

static bool8 (*const sFishingStateFuncs[])(struct Task *) =
{
    Fishing1,
    Fishing2,
    Fishing3,
    Fishing4,
    Fishing5,
    Fishing6,
    Fishing7,
    Fishing8,
    Fishing9,
    Fishing10,
    Fishing11,
    Fishing12,
    Fishing13,
    Fishing14,
    Fishing15,
    Fishing16,
};

#define tStep              data[0]
#define tFrameCounter      data[1]
#define tNumDots           data[2]
#define tDotsRequired      data[3]
#define tRoundsPlayed      data[12]
#define tMinRoundsRequired data[13]
#define tPlayerGfxId       data[14]
#define tFishingRod        data[15]

#define FISHING_START_ROUND 3
#define FISHING_GOT_BITE 6
#define FISHING_ON_HOOK 9
#define FISHING_NO_BITE 11
#define FISHING_GOT_AWAY 12
#define FISHING_SHOW_RESULT 13

void StartFishing(u8 rod)
{
    u8 taskId = CreateTask(Task_Fishing, 0xFF);
    gTasks[taskId].tFishingRod = rod;
    Task_Fishing(taskId);
}

static void Task_Fishing(u8 taskId)
{
    while (sFishingStateFuncs[gTasks[taskId].tStep](&gTasks[taskId]));
}

static bool8 Fishing1(struct Task *task)
{
    ScriptContext2_Enable();
    gPlayerAvatar.preventStep = TRUE;
    task->tStep++;
    return FALSE;
}

static bool8 Fishing2(struct Task *task)
{
    struct ObjectEvent *playerObjEvent;
    const s16 arr1[] = {1, 1, 1};
    const s16 arr2[] = {1, 3, 6};

    task->tRoundsPlayed = 0;
    task->tMinRoundsRequired = arr1[task->tFishingRod] + RandomMax(arr2[task->tFishingRod]);
    task->tPlayerGfxId = gObjectEvents[gPlayerAvatar.objectEventId].graphicsId;
	
    playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    ObjectEventClearHeldMovementIfActive(playerObjEvent);
    playerObjEvent->enableAnim = 1;
    StartPlayerAvatarFishAnim(playerObjEvent->facingDirection);
    task->tStep++;
	
    return FALSE;
}

static void AlignFishingAnimationFrames(struct Sprite * playerSprite)
{
    u8 animCmdIndex;
    u8 animType;

    AnimateSprite(playerSprite);
	
    playerSprite->x2 = 0;
    playerSprite->y2 = 0;
	
    animCmdIndex = playerSprite->animCmdIndex;
	
    if (playerSprite->anims[playerSprite->animNum][animCmdIndex].type == -1)
        animCmdIndex--;
    else
    {
        playerSprite->animDelayCounter++;
		
        if (playerSprite->anims[playerSprite->animNum][animCmdIndex].type == -1)
            animCmdIndex--;
    }
    animType = playerSprite->anims[playerSprite->animNum][animCmdIndex].type;
	
    if (animType == 1 || animType == 2 || animType == 3)
    {
        playerSprite->x2 = 8;
		
        if (GetPlayerFacingDirection() == 3)
            playerSprite->x2 = -8;
    }
	
    if (animType == 5)
        playerSprite->y2 = -8;
    else if (animType == 10 || animType == 11)
        playerSprite->y2 = 8;
	
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        SetSurfBlob_PlayerOffset(gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId, 1, playerSprite->y2);
}

static bool8 Fishing3(struct Task *task)
{
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);

    // Wait one second
    if (++task->tFrameCounter >= 60)
        task->tStep++;
	
    return FALSE;
}

static bool8 Fishing4(struct Task *task)
{
    u32 randVal;

    LoadMessageBoxAndFrameGfx(0, TRUE);
	
    task->tStep++;
    task->tFrameCounter = 0;
    task->tNumDots = 0;
    randVal = RandomMax(10);
    task->tDotsRequired = randVal + 1;
	
    if (task->tRoundsPlayed == 0)
        task->tDotsRequired = randVal + 4;
	
    if (task->tDotsRequired >= 10)
        task->tDotsRequired = 10;
	
    return TRUE;
}

// Play a round of the dot game
static bool8 Fishing5(struct Task *task)
{
    static const u8 dot[] = _("·");

    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);

    if (++task->tFrameCounter >= 20)
    {
        task->tFrameCounter = 0;
		
        if (task->tNumDots >= task->tDotsRequired)
        {
            task->tStep++;
            if (task->tRoundsPlayed != 0)
                task->tStep++;
			
            task->tRoundsPlayed++;
        }
        else
        {
            AddTextPrinterParameterized(0, 2, dot, task->tNumDots * 12, 1, 0, NULL);
            task->tNumDots++;
        }
    }
    return FALSE;
}

// Determine if fish bites
static bool8 DoesFishingBiteOccours(void)
{	
	if (IsMonValidSpecies(&gPlayerParty[0]))
	{
		switch (GetMonAbility(&gPlayerParty[0]))
		{
			case ABILITY_SUCTION_CUPS:
			case ABILITY_STICKY_HOLD:
				return !RandomPercent(15);
		}
	}
	return RandomPercent(50);
}

static bool8 Fishing6(struct Task *task)
{
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);

    if (!DoesCurrentMapHaveFishingMons() || !DoesFishingBiteOccours())
        task->tStep = FISHING_NO_BITE;
    else
    {
        StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFishingBiteDirectionAnimNum(GetPlayerFacingDirection()));
		task->tStep++;
    }
    return TRUE;
}

// Oh! A Bite!
static bool8 Fishing7(struct Task *task)
{
    task->tStep += 3;
    return FALSE;
}

// We have a bite. Now, wait for the player to press A, or the timer to expire.
static bool8 Fishing8(struct Task *task)
{
    const s16 reelTimeouts[3] = {36, 33, 30};

    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);

    if (++task->tFrameCounter >= reelTimeouts[task->tFishingRod])
        task->tStep = FISHING_GOT_AWAY;
    else if (gMain.newKeys & A_BUTTON)
        task->tStep++;
	
    return FALSE;
}

// Determine if we're going to play the dot game again
static bool8 Fishing9(struct Task *task)
{
    const s16 arr[][2] =
    {
    {0, 0},
    {40, 10},
    {70, 30}
    };

    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);
	
    task->tStep++;
	
    if (task->tRoundsPlayed < task->tMinRoundsRequired)
        task->tStep = FISHING_START_ROUND;
    else if (task->tRoundsPlayed < 2)
    {
        // probability of having to play another round
        s16 probability = RandomMax(100);

        if (arr[task->tFishingRod][task->tRoundsPlayed] > probability)
            task->tStep = FISHING_START_ROUND;
    }
    return FALSE;
}

static bool8 Fishing10(struct Task *task)
{
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    AddTextPrinterParameterized2(0, 2, COMPOUND_STRING("A Pokémon's on the hook!{PAUSE_UNTIL_PRESS}"), 1, 0, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    task->tStep++;
    task->tFrameCounter = 0;
    return FALSE;
}

static bool8 Fishing11(struct Task *task)
{
    if (task->tFrameCounter == 0)
        AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);

    RunTextPrinters();

    if (task->tFrameCounter == 0)
    {
        if (!IsTextPrinterActive(0))
        {
            struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

            ObjectEventSetGraphicsId(playerObjEvent, task->tPlayerGfxId);
            ObjectEventTurn(playerObjEvent, playerObjEvent->movementDirection);
			
            if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
                SetSurfBlob_PlayerOffset(gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId, 0, 0);
			
            gSprites[gPlayerAvatar.spriteId].x2 = 0;
            gSprites[gPlayerAvatar.spriteId].y2 = 0;
            ClearDialogWindowAndFrame(0, TRUE);
            task->tFrameCounter++;
            return FALSE;
        }
    }

    if (task->tFrameCounter != 0)
    {
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        FishingWildEncounter(task->tFishingRod);
        DestroyTask(FindTaskIdByFunc(Task_Fishing));
    }
    return FALSE;
}

// Not even a nibble
static bool8 Fishing12(struct Task *task)
{
    gChainFishingStreak = 0;
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFishingNoCatchDirectionAnimNum(GetPlayerFacingDirection()));
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    AddTextPrinterParameterized2(0, 2, COMPOUND_STRING("Not even a nibble‥{PAUSE_UNTIL_PRESS}"), 1, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    task->tStep = FISHING_SHOW_RESULT;
    return TRUE;
}

// It got away
static bool8 Fishing13(struct Task *task)
{
    gChainFishingStreak = 0;
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], GetFishingNoCatchDirectionAnimNum(GetPlayerFacingDirection()));
    AddTextPrinterParameterized2(0, 2, COMPOUND_STRING("It got away‥{PAUSE_UNTIL_PRESS}"), 1, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    task->tStep++;
    return TRUE;
}

// Wait one second
static bool8 Fishing14(struct Task *task)
{
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);
    task->tStep++;
    return FALSE;
}

static bool8 Fishing15(struct Task *task)
{
    AlignFishingAnimationFrames(&gSprites[gPlayerAvatar.spriteId]);
	
    if (gSprites[gPlayerAvatar.spriteId].animEnded)
    {
        struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

        ObjectEventSetGraphicsId(playerObjEvent, task->tPlayerGfxId);
        ObjectEventTurn(playerObjEvent, playerObjEvent->movementDirection);
		
        if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
            SetSurfBlob_PlayerOffset(gObjectEvents[gPlayerAvatar.objectEventId].fieldEffectSpriteId, 0, 0);
		
        gSprites[gPlayerAvatar.spriteId].x2 = 0;
        gSprites[gPlayerAvatar.spriteId].y2 = 0;
        task->tStep++;
    }
    return FALSE;
}

static bool8 Fishing16(struct Task *task)
{
    RunTextPrinters();
	
    if (!IsTextPrinterActive(0))
    {
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
        ClearDialogWindowAndFrame(0, TRUE);
        DestroyTask(FindTaskIdByFunc(Task_Fishing));
    }
    return FALSE;
}

#undef tStep
#undef tFrameCounter
#undef tFishingRod

#define tState data[0]
#define tRotationTimer data[1]
#define tDeltaY data[2]
#define tYdeflection data[3]
#define tYpos data[4]
#define tFinalFacingDirection data[5]
#define tPriority data[6]
#define tSubpriority data[7]
#define tLandingDelay data[8]

////////////////////////////
// PLAYER TELEPORT EFFECT //
////////////////////////////

static const u8 sTeleportFacingDirectionSequence[] = {
    [DIR_SOUTH] = DIR_WEST,
    [DIR_WEST]  = DIR_NORTH,
    [DIR_NORTH] = DIR_EAST,
    [DIR_EAST]  = DIR_SOUTH,
    [DIR_NONE]  = DIR_SOUTH,
};

void StartTeleportWarpOutPlayerAnim(void)
{
    Task_TeleportWarpOutPlayerAnim(CreateTask(Task_TeleportWarpOutPlayerAnim, 0));
}

bool32 WaitTeleportWarpOutPlayerAnim(void)
{
    return FuncIsActiveTask(Task_TeleportWarpOutPlayerAnim);
}

void SavePlayerFacingDirectionForTeleport(u8 direction)
{
    sTeleportSavedFacingDirection = direction;
}

static u8 GetTeleportSavedFacingDirection(void)
{
	return sTeleportSavedFacingDirection == DIR_NONE ? DIR_SOUTH : sTeleportSavedFacingDirection;
}

static void Task_TeleportWarpOutPlayerAnim(u8 taskId)
{
    struct ObjectEvent *object = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite *sprite = &gSprites[object->spriteId];
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        if (!ObjectEventClearHeldMovementIfFinished(object))
            return;

        SavePlayerFacingDirectionForTeleport(object->facingDirection);
        tRotationTimer = 0;
        tDeltaY = 1;
        tYdeflection = (u16)(sprite->y + sprite->y2) * 16;
        sprite->y2 = 0;
        CameraObjectReset2();
        object->fixedPriority = TRUE;
        sprite->oam.priority = 0;
        sprite->subpriority = 0;
        sprite->subspriteMode = SUBSPRITES_OFF;
        tState++;
    case 1:
        TeleportAnim_RotatePlayer(object, &tRotationTimer);
        tYdeflection -= tDeltaY;
        tDeltaY += 3;
        sprite->y = tYdeflection >> 4;
		
        if (sprite->y + (s16)gTotalCameraPixelOffsetY < -32)
            tState++;

        break;
    case 2:
        DestroyTask(taskId);
        break;
    }
}

void StartTeleportInPlayerAnim(void)
{
    Task_TeleportWarpInPlayerAnim(CreateTask(Task_TeleportWarpInPlayerAnim, 0));
}

bool32 WaitTeleportInPlayerAnim(void)
{
    return FuncIsActiveTask(Task_TeleportWarpInPlayerAnim);
}

static void Task_TeleportWarpInPlayerAnim(u8 taskId)
{
    struct ObjectEvent *object = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite *sprite = &gSprites[object->spriteId];
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        tFinalFacingDirection = GetTeleportSavedFacingDirection();
        ObjectEventForceSetHeldMovement(object, GetFaceDirectionMovementAction(sTeleportFacingDirectionSequence[tFinalFacingDirection]));
        tRotationTimer = 0;
        tDeltaY = 116;
        tYpos = sprite->y;
        tPriority = sprite->oam.priority;
        tSubpriority = sprite->subpriority;
        tYdeflection = -((u16)sprite->y2 + 32) * 16;
        sprite->y2 = 0;
        CameraObjectReset2();
        object->fixedPriority = TRUE;
        sprite->oam.priority = 1;
        sprite->subpriority = 0;
        sprite->subspriteMode = SUBSPRITES_OFF;
        tState++;
    case 1:
        TeleportAnim_RotatePlayer(object, &tRotationTimer);
        tYdeflection += tDeltaY;
		
        tDeltaY -= 3;
        if (tDeltaY < 4)
            tDeltaY = 4;

        sprite->y = tYdeflection >> 4;
        if (sprite->y >= tYpos)
        {
            sprite->y = tYpos;
            tLandingDelay = 0;
            tState++;
        }
        break;
    case 2:
        TeleportAnim_RotatePlayer(object, &tRotationTimer);
		
        tLandingDelay++;
        if (tLandingDelay > 8)
            tState++;

        break;
    case 3:
        if (tFinalFacingDirection == TeleportAnim_RotatePlayer(object, &tRotationTimer))
        {
            object->fixedPriority = 0;
            sprite->oam.priority = tPriority;
            sprite->subpriority = tSubpriority;
            CameraObjectReset1();
            DestroyTask(taskId);
        }
        break;
    }
}

static u8 TeleportAnim_RotatePlayer(struct ObjectEvent *object, s16 *a1)
{
    if ((*a1 < 8 && ++(*a1) < 8) || !ObjectEventCheckHeldMovementStatus(object))
        return object->facingDirection;

    ObjectEventForceSetHeldMovement(object, GetFaceDirectionMovementAction(sTeleportFacingDirectionSequence[object->facingDirection]));
    *a1 = 0;
    return sTeleportFacingDirectionSequence[object->facingDirection];
}

#undef tLandingDelay
#undef tSubpriority
#undef tPriority
#undef tFinalFacingDirection
#undef tYpos
#undef tYdeflection
#undef tDeltaY
#undef tRotationTimer
#undef tState
