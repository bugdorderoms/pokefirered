#include "global.h"
#include "metatile_behavior.h"
#include "constants/metatile_behaviors.h"

static const u8 sTilesSurfable[] =
{
	MB_POND_WATER,
	MB_SEMI_DEEP_WATER,
	MB_DEEP_WATER,
	MB_WATERFALL,
	MB_OCEAN_WATER,
	MB_1B,
	MB_EASTWARD_CURRENT,
    MB_WESTWARD_CURRENT,
	MB_NORTHWARD_CURRENT,
    MB_SOUTHWARD_CURRENT,
};

static const u8 sTileBitAttributes[32] =
{
    [0] = 0,
    [1] = 1 << 0,
    [2] = 1 << 1,
    [3] = 1 << 2,
    [4] = 1 << 3,
};

bool32 MetatileBehavior_IsATile(u32 metatileBehavior)
{
    return TRUE;
}

bool32 MetatileBehavior_IsJumpEast(u32 metatileBehavior)
{
    if(metatileBehavior == MB_JUMP_EAST)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsJumpWest(u32 metatileBehavior)
{
    if(metatileBehavior == MB_JUMP_WEST)
            return TRUE;
        else
            return FALSE;
}

bool32 MetatileBehavior_IsJumpNorth(u32 metatileBehavior)
{
    if(metatileBehavior == MB_JUMP_NORTH)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsJumpSouth(u32 metatileBehavior)
{
    if(metatileBehavior == MB_JUMP_SOUTH)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPokeGrass(u32 metatileBehavior)
{
    if(metatileBehavior == MB_TALL_GRASS || metatileBehavior == MB_CYCLING_ROAD_PULL_DOWN_GRASS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSand(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SAND || metatileBehavior == MB_SAND_CAVE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSandOrShallowFlowingWater(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SAND || metatileBehavior == MB_SHALLOW_WATER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsDeepSand(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsReflective(u32 metatileBehavior)
{
    if(metatileBehavior == MB_POND_WATER
        || metatileBehavior == MB_PUDDLE
        || metatileBehavior == MB_1B
        || metatileBehavior == MB_ICE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsIce(u32 metatileBehavior)
{
    if(metatileBehavior == MB_ICE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWarpDoor(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WARP_DOOR)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWarpDoor_2(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WARP_DOOR)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsEscalator(u32 metatileBehavior)
{
    if(metatileBehavior >= MB_UP_ESCALATOR && metatileBehavior <= MB_DOWN_ESCALATOR)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsDirectionalUpRightStairWarp(u32 metatileBehavior)
{
    u8 result = FALSE;

    if(metatileBehavior == MB_UP_RIGHT_STAIR_WARP)
        result = TRUE;

    return result;
}

bool32 MetatileBehavior_IsDirectionalUpLeftStairWarp(u32 metatileBehavior)
{
    u8 result = FALSE;

    if(metatileBehavior == MB_UP_LEFT_STAIR_WARP)
        result = TRUE;

    return result;
}

bool32 MetatileBehavior_IsDirectionalDownRightStairWarp(u32 metatileBehavior)
{
    u8 result = FALSE;

    if(metatileBehavior == MB_DOWN_RIGHT_STAIR_WARP)
        result = TRUE;

    return result;
}

bool32 MetatileBehavior_IsDirectionalDownLeftStairWarp(u32 metatileBehavior)
{
    u8 result = FALSE;

    if(metatileBehavior == MB_DOWN_LEFT_STAIR_WARP)
        result = TRUE;

    return result;
}

bool32 MetatileBehavior_IsDirectionalStairWarp(u32 metatileBehavior)
{
    bool32 result = FALSE;

    if(metatileBehavior >= MB_UP_RIGHT_STAIR_WARP && metatileBehavior <= MB_DOWN_LEFT_STAIR_WARP)
        result = TRUE;
    else
        result = FALSE;

    return result;
}

bool32 MetatileBehavior_IsLadder(u32 metatileBehavior)
{
    if(metatileBehavior == MB_LADDER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsNonAnimDoor(u32 metatileBehavior)
{
    if(metatileBehavior == MB_CAVE_DOOR)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsDeepSouthWarp(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsSurfable(u32 metatileBehavior)
{
	u8 i;
	
	for (i = 0; i < ARRAY_COUNT(sTilesSurfable); i++)
	{
		if (sTilesSurfable[i] == metatileBehavior)
			return TRUE;
	}
	return FALSE;
}

bool32 MetatileBehavior_IsSemiDeepWater(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SEMI_DEEP_WATER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsEastArrowWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_EAST_ARROW_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWestArrowWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WEST_ARROW_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsNorthArrowWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_NORTH_ARROW_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSouthArrowWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SOUTH_ARROW_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_UnusedIsArrowWarp(u32 metatileBehavior)
{
    u8 result = FALSE;

    if(MetatileBehavior_IsEastArrowWarp(metatileBehavior)
    || MetatileBehavior_IsWestArrowWarp(metatileBehavior)
    || MetatileBehavior_IsNorthArrowWarp(metatileBehavior)
    || MetatileBehavior_IsSouthArrowWarp(metatileBehavior))
        result = TRUE;

    return result;
}

bool32 MetatileBehavior_IsForcedMovementTile(u32 metatileBehavior)
{
    if((metatileBehavior >= MB_WALK_EAST && metatileBehavior <= MB_UNKNOWN_MOVEMENT_48)
        ||(metatileBehavior >= MB_EASTWARD_CURRENT && metatileBehavior <= MB_SOUTHWARD_CURRENT)
        || metatileBehavior == MB_WATERFALL
        || metatileBehavior == MB_ICE
        || (metatileBehavior >= MB_SPIN_RIGHT && metatileBehavior <= MB_SPIN_DOWN))
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsIce_2(u32 metatileBehavior)
{
    if(metatileBehavior == MB_ICE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsUnknownMovement48(u32 metatileBehavior)
{
    if(metatileBehavior == MB_UNKNOWN_MOVEMENT_48)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWalkNorth(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WALK_NORTH)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWalkSouth(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WALK_SOUTH)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWalkWest(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WALK_WEST)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWalkEast(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WALK_EAST)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsNorthwardCurrent(u32 metatileBehavior)
{
    if(metatileBehavior == MB_NORTHWARD_CURRENT)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSouthwardCurrent(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SOUTHWARD_CURRENT)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWestwardCurrent(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WESTWARD_CURRENT)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsEastwardCurrent(u32 metatileBehavior)
{
    if(metatileBehavior == MB_EASTWARD_CURRENT)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSlideNorth(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SLIDE_NORTH)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSlideSouth(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SLIDE_SOUTH)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSlideWest(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SLIDE_WEST)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSlideEast(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SLIDE_EAST)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsCounter(u32 metatileBehavior)
{
    if(metatileBehavior == MB_COUNTER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPlayerFacingTVScreen(u8 tile, u8 playerDirection)
{
    if(playerDirection != DIR_NORTH)
        return FALSE;
    else if(tile == MB_TELEVISION)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPC(u32 metatileBehavior)
{
    if(metatileBehavior == MB_PC)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_HasRipples(u32 metatileBehavior)
{
    if(metatileBehavior == MB_POND_WATER || metatileBehavior == MB_PUDDLE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPuddle(u32 metatileBehavior)
{
    if(metatileBehavior == MB_PUDDLE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsTallGrass_2(u32 metatileBehavior)
{
    if(metatileBehavior == MB_TALL_GRASS || metatileBehavior == MB_CYCLING_ROAD_PULL_DOWN_GRASS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsLongGrass(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_ReturnFalse_4(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsFootprints(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsBridge(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_GetBridgeType(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsGroundRocks(u32 metatileBehavior)
{
    if(metatileBehavior == MB_GROUND_ROCKS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_UnusedIsTallGrass(u32 metatileBehavior)
{
    if(metatileBehavior == MB_TALL_GRASS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsIndoorEncounter(u32 metatileBehavior)
{
    if(metatileBehavior == MB_0B)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsMountain(u32 metatileBehavior)
{
    if(metatileBehavior == MB_0C)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsDiveable(u32 metatileBehavior)
{
    if(metatileBehavior >= MB_SEMI_DEEP_WATER && metatileBehavior <= MB_DEEP_WATER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsUnableToEmerge(u32 metatileBehavior)
{
    if(metatileBehavior == MB_UNDERWATER_BLOCKED_ABOVE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsShallowFlowingWater(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SHALLOW_WATER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsThinIce(u32 metatileBehavior)
{
    if(metatileBehavior == MB_THIN_ICE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsCrackedIce(u32 metatileBehavior)
{
    if(metatileBehavior == MB_CRACKED_ICE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsDeepSemiDeepOrSplashingWater(u32 metatileBehavior)
{
    if((metatileBehavior >= MB_SEMI_DEEP_WATER && metatileBehavior <= MB_DEEP_WATER)
        || metatileBehavior == MB_OCEAN_WATER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsStrengthHole(u32 metatileBehavior)
{
    if(metatileBehavior == MB_STRENGTH_HOLE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSurfableAndNotWaterfall(u32 metatileBehavior)
{
    if(MetatileBehavior_IsSurfable(metatileBehavior)
        && !MetatileBehavior_IsWaterfall(metatileBehavior))
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsEastBlocked(u32 metatileBehavior)
{
    if(metatileBehavior == MB_IMPASSABLE_EAST
        || metatileBehavior == MB_IMPASSABLE_NORTHEAST
        || metatileBehavior == MB_IMPASSABLE_SOUTHEAST)
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWestBlocked(u32 metatileBehavior)
{
    if(metatileBehavior == MB_IMPASSABLE_WEST
        || metatileBehavior == MB_IMPASSABLE_NORTHWEST
        || metatileBehavior == MB_IMPASSABLE_SOUTHWEST)
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsNorthBlocked(u32 metatileBehavior)
{
    if(metatileBehavior == MB_IMPASSABLE_NORTH
        || metatileBehavior == MB_IMPASSABLE_NORTHEAST
        || metatileBehavior == MB_IMPASSABLE_NORTHWEST)
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSouthBlocked(u32 metatileBehavior)
{
    if(metatileBehavior == MB_IMPASSABLE_SOUTH
        || metatileBehavior == MB_IMPASSABLE_SOUTHEAST
        || metatileBehavior == MB_IMPASSABLE_SOUTHWEST)
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsShortGrass(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsHotSprings(u32 metatileBehavior)
{
    if(metatileBehavior == 0x28)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWaterfall(u32 metatileBehavior)
{
    if(metatileBehavior == MB_WATERFALL)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsFortreeBridge(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse(u32 metatileBehavior){ return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_2(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_3(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_4(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsPacifidlogLog(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_ReturnFalse_11(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsRegionMap(u32 metatileBehavior)
{
    if(metatileBehavior == MB_REGION_MAP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_UnusedReturnFalse_5(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_6(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_7(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_8(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsLavaridgeB1FWarp(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsLavaridge1FWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_LAVARIDGE_1F_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWarpPad(u32 metatileBehavior)
{
    if(metatileBehavior == MB_REGULAR_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsUnionRoomWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_UNION_ROOM_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsWater(u32 metatileBehavior)
{
    if((metatileBehavior >= MB_POND_WATER && metatileBehavior <= MB_DEEP_WATER)
        || metatileBehavior == MB_OCEAN_WATER
        || (metatileBehavior >= MB_EASTWARD_CURRENT && metatileBehavior <= MB_SOUTHWARD_CURRENT))
            return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsFallWarp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_FALL_WARP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_ReturnFalse_13(u32 metatileBehavior){ return FALSE; }

bool32 MetatileBehavior_IsCyclingRoadPullDownTile(u32 metatileBehavior)
{
    if(metatileBehavior >= MB_CYCLING_ROAD_PULL_DOWN && metatileBehavior <= MB_CYCLING_ROAD_PULL_DOWN_GRASS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsCyclingRoadPullDownTileGrass(u32 metatileBehavior)
{
    return metatileBehavior == MB_CYCLING_ROAD_PULL_DOWN_GRASS;
}

bool32 MetatileBehavior_IsBumpySlope(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsIsolatedVerticalRail(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsIsolatedHorizontalRail(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsVerticalRail(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsHorizontalRail(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsSeaweed(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SEAWEED)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsRunningDisallowed(u32 metatileBehavior)
{
    if(metatileBehavior == MB_RUNNING_DISALLOWED)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_UnusedReturnFalse_9(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsBookshelf(u32 metatileBehavior)
{
    if(metatileBehavior == MB_BOOKSHELF)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPokeMartShelf(u32 metatileBehavior)
{
    if(metatileBehavior == MB_POKEMART_SHELF)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPlayerFacingPokemonCenterSign(u8 tile, u8 playerDirection)
{
    if(playerDirection != DIR_NORTH)
        return FALSE;
    else if(tile == MB_POKEMON_CENTER_SIGN)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPlayerFacingPokeMartSign(u8 tile, u8 playerDirection)
{
    if(playerDirection != DIR_NORTH)
        return FALSE;
    else if(tile == MB_POKEMART_SIGN)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_UnusedReturnFalse_10(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_11(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_12(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_UnusedReturnFalse_13(u32 metatileBehavior) { return FALSE; }

bool32 TestMetatileAttributeBit(u8 arg1, u8 arg2)
{
    if(sTileBitAttributes[arg1] & arg2)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSpinRight(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SPIN_RIGHT)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSpinLeft(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SPIN_LEFT)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSpinUp(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SPIN_UP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSpinDown(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SPIN_DOWN)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsStopSpinning(u32 metatileBehavior)
{
    if(metatileBehavior == MB_STOP_SPINNING)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSpinTile(u32 metatileBehavior)
{
    bool32 result = FALSE;

    if(metatileBehavior >= MB_SPIN_RIGHT && metatileBehavior <= MB_SPIN_DOWN)
        result = TRUE;
    else
        result = FALSE;

    return result;
}

bool32 MetatileBehavior_IsSignpost(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SIGNPOST)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsCabinet(u32 metatileBehavior)
{
    if(metatileBehavior == MB_CABINET)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsKitchen(u32 metatileBehavior)
{
    if(metatileBehavior == MB_KITCHEN)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsDresser(u32 metatileBehavior)
{
    if(metatileBehavior == MB_DRESSER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsSnacks(u32 metatileBehavior)
{
    if(metatileBehavior == MB_SNACKS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsStrengthButton(u32 metatileBehavior)
{
    if(metatileBehavior == MB_STRENGTH_BUTTON)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPlayerFacingCableClubWirelessMonitor(u8 tile, u8 playerDirection)
{
    if(playerDirection != DIR_NORTH)
        return FALSE;
    else if(tile == MB_8D)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPlayerFacingBattleRecords(u8 tile, u8 playerDirection)
{
    if(playerDirection != DIR_NORTH)
        return FALSE;
    else if(tile == MB_BATTLE_RECORDS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsIndigoPlateauMark(u32 metatileBehavior)
{
    if(metatileBehavior == MB_INDIGO_PLATEAU_MARK_DPAD)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsIndigoPlateauMark2(u32 metatileBehavior)
{
    if(metatileBehavior == MB_INDIGO_PLATEAU_MARK_2_DPAD)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsFood(u32 metatileBehavior)
{
    if(metatileBehavior == MB_FOOD)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsRockStairs(u32 metatileBehavior)
{
    bool32 result = FALSE;

    if(metatileBehavior == MB_ROCK_STAIRS)
        result = TRUE;
    else
        result = FALSE;

    return result;
}

bool32 MetatileBehavior_IsBlueprints(u32 metatileBehavior)
{
    if(metatileBehavior == MB_BLUEPRINTS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPainting(u32 metatileBehavior)
{
    if(metatileBehavior == MB_PAINTING)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPowerPlantMachine(u32 metatileBehavior)
{
    if(metatileBehavior == MB_POWER_PLANT_MACHINE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsTelephone(u32 metatileBehavior)
{
    if(metatileBehavior == MB_TELEPHONE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsComputer(u32 metatileBehavior)
{
    if(metatileBehavior == MB_COMPUTER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsAdvertisingPoster(u32 metatileBehavior)
{
    if(metatileBehavior == MB_ADVERTISING_POSTER)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsTastyFood(u32 metatileBehavior)
{
    if(metatileBehavior == MB_FOOD_SMELLS_TASTY)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsTrashBin(u32 metatileBehavior)
{
    if(metatileBehavior == MB_TRASH_BIN)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsCup(u32 metatileBehavior)
{
    if(metatileBehavior == MB_CUP)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsPolishedWindow(u32 metatileBehavior) { return FALSE; }
bool32 MetatileBehavior_IsBeautifulSkyWindow(u32 metatileBehavior) { return FALSE; }

bool32 MetatileBehavior_IsBlinkingLights(u32 metatileBehavior)
{
    if(metatileBehavior == MB_BLINKING_LIGHTS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsNeatlyLinedUpTools(u32 metatileBehavior)
{
    if(metatileBehavior == MB_NEATLY_LINED_UP_TOOLS)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsImpressiveMachine(u32 metatileBehavior)
{
    if(metatileBehavior == MB_IMPRESSIVE_MACHINE)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsVideoGame(u32 metatileBehavior)
{
    if(metatileBehavior == MB_VIDEO_GAME)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsBurglary(u32 metatileBehavior)
{
    if(metatileBehavior == MB_BURGLARY)
        return TRUE;
    else
        return FALSE;
}

bool32 MetatileBehavior_IsTrainerTowerMonitor(u32 metatileBehavior)
{
    if(metatileBehavior == MB_TRAINER_TOWER_MONITOR)
        return TRUE;
    else
        return FALSE;
}
