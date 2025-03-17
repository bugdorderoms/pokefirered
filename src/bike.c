#include "global.h"
#include "bike.h"
#include "field_player_avatar.h"
#include "metatile_behavior.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "overworld.h"
#include "constants/map_types.h"
#include "constants/songs.h"

static u32 GetBikeTransitionId(u32 *, u16, u16);
static void Bike_SetBikeStill(void);
static bool32 CanBikeFaceDirectionOnRail(u32 direction, u32 metatileBehavior);
static u32 GetBikeCollisionAt(struct ObjectEvent *playerObjEvent, s16 x, s16 y, u32 direction, u32 metatileBehavior);
static void BikeTransition_FaceDirection(u32);
static void BikeTransition_TurnDirection(u32);
static void BikeTransition_MoveDirection(u32);
static void BikeTransition_Downhill(u32);
static void BikeTransition_Uphill(u32);
static u32 BikeInputHandler_Normal(u32 *, u16, u16);
static u32 BikeInputHandler_Turning(u32 *, u16, u16);
static u32 BikeInputHandler_Slope(u32 *, u16, u16);

static void (*const sBikeTransitions[])(u32) =
{
    [BIKE_TRANS_FACE_DIRECTION] = BikeTransition_FaceDirection,
    [BIKE_TRANS_TURNING]        = BikeTransition_TurnDirection,
    [BIKE_TRANS_MOVE]           = BikeTransition_MoveDirection,
    [BIKE_TRANS_DOWNHILL]       = BikeTransition_Downhill,
    [BIKE_TRANS_UPHILL]         = BikeTransition_Uphill,
};

static u32 (*const sBikeInputHandlers[])(u32 *, u16, u16) =
{
    [BIKE_STATE_NORMAL]  = BikeInputHandler_Normal,
    [BIKE_STATE_TURNING] = BikeInputHandler_Turning,
    [BIKE_STATE_SLOPE]   = BikeInputHandler_Slope,
};

void MovePlayerOnBike(u32 direction, u16 newKeys, u16 heldKeys)
{
    sBikeTransitions[GetBikeTransitionId(&direction, newKeys, heldKeys)](direction);
}

static u32 GetBikeTransitionId(u32 *direction, u16 newKeys, u16 heldKeys)
{
    return sBikeInputHandlers[gPlayerAvatar.acroBikeState](direction, newKeys, heldKeys);
}

static u32 BikeInputHandler_Normal(u32 *direction_p, u16 newKeys, u16 heldKeys)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    u32 direction = GetPlayerMovementDirection();

    gPlayerAvatar.bikeFrameCounter = 0;
    if (MetatileBehavior_IsCyclingRoadPullDownTile(playerObjEvent->currentMetatileBehavior) == TRUE)
    {
        if (!JOY_HELD(B_BUTTON))
        {
            gPlayerAvatar.acroBikeState = BIKE_STATE_SLOPE;
            gPlayerAvatar.runningState = MOVING;
			return *direction_p < DIR_NORTH ? BIKE_TRANS_DOWNHILL : BIKE_TRANS_UPHILL;
        }
        else
        {
            if (*direction_p != DIR_NONE)
            {
                gPlayerAvatar.acroBikeState = BIKE_STATE_SLOPE;
                gPlayerAvatar.runningState = MOVING;
                return BIKE_TRANS_UPHILL;
            }
        }
    }
    if (*direction_p == DIR_NONE)
    {
        *direction_p = direction;
        gPlayerAvatar.runningState = NOT_MOVING;
        return BIKE_TRANS_FACE_DIRECTION;
    }
    else
    {
        if (*direction_p != direction && gPlayerAvatar.runningState != MOVING)
        {
            gPlayerAvatar.acroBikeState = BIKE_STATE_TURNING;
            gPlayerAvatar.newDirBackup = *direction_p;
            gPlayerAvatar.runningState = NOT_MOVING;
            return GetBikeTransitionId(direction_p, newKeys, heldKeys);
        }
        else
        {
            gPlayerAvatar.runningState = MOVING;
            return BIKE_TRANS_MOVE;
        }
    }
}

static u32 BikeInputHandler_Turning(u32 *direction_p, UNUSED u16 newKeys, UNUSED u16 heldKeys)
{
    *direction_p = gPlayerAvatar.newDirBackup;
    gPlayerAvatar.runningState = TURN_DIRECTION;
    gPlayerAvatar.acroBikeState = BIKE_STATE_NORMAL;
    Bike_SetBikeStill();
    return BIKE_TRANS_TURNING;
}

static u32 BikeInputHandler_Slope(u32 *direction_p, u16 newKeys, u16 heldKeys)
{
    u32 direction = GetPlayerMovementDirection();
    u32 playerObjEventId = gPlayerAvatar.objectEventId;

    if (MetatileBehavior_IsCyclingRoadPullDownTile(playerObjEventId[gObjectEvents].currentMetatileBehavior) == TRUE)
    {
        if (*direction_p != direction)
        {
            gPlayerAvatar.acroBikeState = BIKE_STATE_TURNING;
            gPlayerAvatar.newDirBackup = *direction_p;
            gPlayerAvatar.runningState = NOT_MOVING;
            return GetBikeTransitionId(direction_p, newKeys, heldKeys);
        }
        else
        {
            gPlayerAvatar.runningState = MOVING;
            gPlayerAvatar.acroBikeState = BIKE_STATE_SLOPE;
			return *direction_p < DIR_NORTH ? BIKE_TRANS_DOWNHILL : BIKE_TRANS_UPHILL;
        }
    }
    gPlayerAvatar.acroBikeState = BIKE_STATE_NORMAL;
    if (*direction_p == DIR_NONE)
    {
        *direction_p = direction;
        gPlayerAvatar.runningState = NOT_MOVING;
        return BIKE_TRANS_FACE_DIRECTION;
    }
    else
    {
        gPlayerAvatar.runningState = MOVING;
        return BIKE_TRANS_MOVE;
    }
}

static void BikeTransition_FaceDirection(u32 direction)
{
    PlayerFaceDirection(direction);
}

static void BikeTransition_TurnDirection(u32 direction)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!CanBikeFaceDirectionOnRail(direction, playerObjEvent->currentMetatileBehavior))
        direction = playerObjEvent->movementDirection;
    PlayerFaceDirection(direction);
}

static void BikeTransition_MoveDirection(u32 direction)
{
    struct ObjectEvent *playerObjEvent;
    
    playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    if (!CanBikeFaceDirectionOnRail(direction, playerObjEvent->currentMetatileBehavior))
    {
        PlayerFaceDirection(playerObjEvent->movementDirection);
    }
    else
    {
        u32 collision = GetBikeCollision(direction);

        if (collision > COLLISION_NONE && collision <= COLLISION_ISOLATED_HORIZONTAL_RAIL)
        {
            if (collision == COLLISION_LEDGE_JUMP)
                PlayerJumpLedge(direction);
            else if (collision != COLLISION_STOP_SURFING && collision != COLLISION_LEDGE_JUMP && collision != COLLISION_PUSHED_BOULDER && collision != COLLISION_ROTATING_GATE)
                PlayerOnBikeCollide(direction);
        }
        else
        {
            if (collision == COLLISION_GROUND_ROCKS)
				PlayerOnBikeCollide(direction);
            else if (collision == COLLISION_COUNT || PlayerIsMovingOnRockStairs(direction))
                PlayerGoSpeed2(direction);
            else
                PlayerRideWaterCurrent(direction);
        }
    }
}

static void BikeTransition_Downhill(UNUSED u32 v)
{
    u32 collision = GetBikeCollision(DIR_SOUTH);

    if (collision == COLLISION_NONE)
        PlayerGoSpeed4(DIR_SOUTH);
    else if (collision == COLLISION_LEDGE_JUMP)
        PlayerJumpLedge(DIR_SOUTH);
}

static void BikeTransition_Uphill(u32 direction)
{
    if (GetBikeCollision(direction) == COLLISION_NONE)
        PlayerGoSpeed1(direction);
}

u32 GetBikeCollision(u32 direction)
{
    struct ObjectEvent *playerObjEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x = playerObjEvent->currentCoords.x, y = playerObjEvent->currentCoords.y;

    MoveCoords(direction, &x, &y);

    return GetBikeCollisionAt(playerObjEvent, x, y, direction, MapGridGetMetatileBehaviorAt(x, y));
}

static u32 GetBikeCollisionAt(struct ObjectEvent *playerObjEvent, s16 x, s16 y, u32 direction, u32 metatileBehavior)
{
    u32 retVal = CheckForObjectEventCollision(playerObjEvent, x, y, direction, metatileBehavior);

    if (retVal <= COLLISION_OBJECT_EVENT)
    {
        if (MetatileBehavior_IsCrackedIce(metatileBehavior))
            retVal = COLLISION_COUNT;
        if (retVal == COLLISION_NONE && MetatileBehaviorForbidsBiking(metatileBehavior))
            retVal = COLLISION_IMPASSABLE;
    }
    return retVal;
}

bool32 MetatileBehaviorForbidsBiking(u32 metatileBehavior)
{
    if (!MetatileBehavior_IsRunningDisallowed(metatileBehavior) && !MetatileBehavior_IsFortreeBridge(metatileBehavior) && (PlayerGetZCoord() & 1))
        return FALSE;
    return TRUE;
}

static bool32 CanBikeFaceDirectionOnRail(u32 direction, u32 metatileBehavior)
{
    if (direction == DIR_EAST || direction == DIR_WEST)
    {
        if (MetatileBehavior_IsIsolatedVerticalRail(metatileBehavior) || MetatileBehavior_IsVerticalRail(metatileBehavior))
            return FALSE;
    }
    else
    {
        if (MetatileBehavior_IsIsolatedHorizontalRail(metatileBehavior) || MetatileBehavior_IsHorizontalRail(metatileBehavior))
            return FALSE;
    }
    return TRUE;
}

bool32 IsBikingDisallowedByPlayer(void)
{
    s16 x, y;

    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER | PLAYER_AVATAR_FLAG_SURFING))
    {
        PlayerGetDestCoords(&x, &y);

        if (!MetatileBehaviorForbidsBiking(MapGridGetMetatileBehaviorAt(x, y)))
            return FALSE;
    }
    return TRUE;
}

bool32 IsPlayerNotUsingAcroBikeOnBumpySlope(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE) && MetatileBehavior_IsBumpySlope(gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior))
		return FALSE;
    return TRUE;
}

void GetOnOffBike(u32 flags)
{
    gBikeCameraAheadPanback = FALSE;

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
    {
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
        Overworld_ClearSavedMusic();
        Overworld_PlaySpecialMapMusic();
    }
    else
    {
        SetPlayerAvatarTransitionFlags(flags);
        
        if (Overworld_MusicCanOverrideMapMusic(MUS_CYCLING))
        {
            Overworld_SetSavedMusic(MUS_CYCLING);
            Overworld_ChangeMusicTo(MUS_CYCLING);
        }
    }
}

void BikeClearState(u32 directionHistory, u32 abStartSelectHistory)
{
    u32 i;

    gPlayerAvatar.acroBikeState = BIKE_STATE_NORMAL;
    gPlayerAvatar.newDirBackup = 0;
    gPlayerAvatar.bikeFrameCounter = 0;
    gPlayerAvatar.bikeSpeed = SPEED_STANDING;
    gPlayerAvatar.directionHistory = directionHistory;
    gPlayerAvatar.abStartSelectHistory = abStartSelectHistory;
    gPlayerAvatar.lastSpinTile = 0;

    for (i = 0; i < ARRAY_COUNT(gPlayerAvatar.dirTimerHistory); ++i)
            gPlayerAvatar.dirTimerHistory[i] = 0;
}

void Bike_UpdateBikeCounterSpeed(u8 counter)
{
    gPlayerAvatar.bikeFrameCounter = counter;
    gPlayerAvatar.bikeSpeed = counter + (gPlayerAvatar.bikeFrameCounter >> 1);
}

static void Bike_SetBikeStill(void)
{
    gPlayerAvatar.bikeFrameCounter = 0;
    gPlayerAvatar.bikeSpeed = SPEED_STANDING;
}

s16 GetPlayerSpeed(void)
{
    s16 machBikeSpeeds[] = { SPEED_NORMAL, SPEED_FAST, SPEED_FASTEST };

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE))
        return machBikeSpeeds[gPlayerAvatar.bikeFrameCounter];
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_RIDE_ANY))
        return SPEED_FASTER;
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_DASH))
        return SPEED_FAST;
    else
        return SPEED_NORMAL;
}

void Bike_HandleBumpySlopeJump(void)
{
    s16 x, y;

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE))
    {
        PlayerGetDestCoords(&x, &y);

        if (MetatileBehavior_IsBumpySlope(MapGridGetMetatileBehaviorAt(x, y)))
        {
            gPlayerAvatar.acroBikeState = BIKE_STATE_SLOPE;
            PlayerUseAcroBikeOnBumpySlope(GetPlayerMovementDirection());
        }
    }
}

// R/S leftover

// the struct below is used for checking button combinations of the last input so that the acro can potentially perform a side/turn jump.
// its possible that at some point Game Freak intended for the acro bike to have more complex tricks: but only the acro jump combinations can be seen in the final ROM.
struct BikeHistoryInputInfo
{
    u32 dirHistoryMatch; // the direction you need to press
    u32 abStartSelectHistoryMatch; // the button you need to press
    u32 dirHistoryMask; // mask applied so that way only the recent nybble (the recent input) is checked
    u32 abStartSelectHistoryMask; // mask applied so that way only the recent nybble (the recent input) is checked
    const u8 *dirTimerHistoryList; // list of timers to check for direction before the button+dir combination can be verified.
    const u8 *abStartSelectHistoryList; // list of timers to check for buttons before the button+dir combination can be verified.
    u32 direction; // direction to jump
};

// this is a list of timers to compare against later, terminated with 0. the only timer being compared against is 4 frames in this list.
static const u8 sAcroBikeJumpTimerList[] = {4, 0};

// this is a list of history inputs to do in order to do the check to retrieve a jump direction for acro bike. it seems to be an extensible list, so its possible that Game Freak may have intended for the Acro Bike to have more complex tricks at some point. The final list only has the acro jump.
static const struct BikeHistoryInputInfo sAcroBikeTricksList[] =
{
    // the 0xF is a mask performed with each byte of the array in order to perform the check on only the last entry of the history list, otherwise the check wouldn't work as there can be 0xF0 as opposed to 0x0F.
    {DIR_SOUTH, B_BUTTON, 0xF, 0xF, sAcroBikeJumpTimerList, sAcroBikeJumpTimerList, DIR_SOUTH},
    {DIR_NORTH, B_BUTTON, 0xF, 0xF, sAcroBikeJumpTimerList, sAcroBikeJumpTimerList, DIR_NORTH},
    {DIR_WEST, B_BUTTON, 0xF, 0xF, sAcroBikeJumpTimerList, sAcroBikeJumpTimerList, DIR_WEST},
    {DIR_EAST, B_BUTTON, 0xF, 0xF, sAcroBikeJumpTimerList, sAcroBikeJumpTimerList, DIR_EAST},
};
