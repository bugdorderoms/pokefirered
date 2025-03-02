#include "global.h"
#include "gflib.h"
#include "bike.h"
#include "daycare.h"
#include "event_data.h"
#include "dexnav.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_control_avatar.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "field_poison.h"
#include "field_specials.h"
#include "item_menu.h"
#include "link.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "renewable_hidden_items.h"
#include "ride_pager.h"
#include "safari_zone.h"
#include "script.h"
#include "start_menu.h"
#include "trainer_see.h"
#include "vs_seeker.h"
#include "wild_encounter.h"
#include "constants/songs.h"
#include "constants/event_bg.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/metatile_behaviors.h"
#include "constants/metatile_labels.h"

#define SIGNPOST_POKECENTER 0
#define SIGNPOST_POKEMART   1
#define SIGNPOST_INDIGO_1   2
#define SIGNPOST_INDIGO_2   3
#define SIGNPOST_SCRIPTED   240
#define SIGNPOST_NA         255

static void Task_OpenStartMenu(u8 taskId);
static void GetPlayerPosition(struct MapPosition * position);
static void GetInFrontOfPlayerPosition(struct MapPosition * position);
static u16 GetPlayerCurMetatileBehavior(void);
static bool8 TryStartInteractionScript(struct MapPosition * position, u16 metatileBehavior, u8 playerDirection);
static const u8 *GetInteractionScript(struct MapPosition * position, u8 metatileBehavior, u8 playerDirection);
static const u8 *GetInteractedObjectEventScript(struct MapPosition * position, u8 metatileBehavior, u8 playerDirection);
static const u8 *GetInteractedBackgroundEventScript(struct MapPosition * position, u8 metatileBehavior, u8 playerDirection);
static const struct BgEvent *GetBackgroundEventAtPosition(struct MapHeader *, u16, u16, u8);
static const u8 *GetInteractedMetatileScript(u8 metatileBehavior, u8 playerDirection);
static const u8 *GetInteractedWaterScript(u8 metatileBehavior);
static bool8 TryStartStepBasedScript(struct MapPosition * position, u16 metatileBehavior);
static bool8 TryStartCoordEventScript(struct MapPosition * position);
static bool8 TryStartStepCountScript(u16 metatileBehavior);
static void UpdateHappinessStepCounter(void);
static bool8 UpdatePoisonStepCounter(void);
static bool8 TrySetUpWalkIntoSignpostScript(struct MapPosition * position, u16 metatileBehavior, u8 playerDirection);
static void SetUpWalkIntoSignScript(const u8 *script, u8 playerDirection);
static u8 GetFacingSignpostType(u16 metatileBehvaior, u8 direction);
static const u8 *GetSignpostScriptAtMapPosition(struct MapPosition * position);
static bool8 TryArrowWarp(struct MapPosition * position, u16 metatileBehavior, u8 playerDirection);
static bool8 TryStartWarpEventScript(struct MapPosition * position, u16 metatileBehavior);
static bool8 IsWarpMetatileBehavior(u16 metatileBehavior);
static void SetupWarp(s8 warpId, struct MapPosition * position);
static bool8 IsArrowWarpMetatileBehavior(u16 metatileBehavior, u8 playerDirection);
static s8 GetWarpEventAtMapPosition(struct MapHeader * mapHeader, struct MapPosition * mapPosition);
static bool8 TryDoorWarp(struct MapPosition * position, u16 metatileBehavior, u8 playerDirection);
static s8 GetWarpEventAtPosition(struct MapHeader * mapHeader, u16 x, u16 y, u8 z);
static const u8 *GetCoordEventScriptAtPosition(struct MapHeader * mapHeader, u16 x, u16 y, u8 z);
static bool8 DoLRButtonAction(bool8 pressedL);

void FieldClearPlayerInput(struct FieldInput *input)
{
    input->pressedAButton = FALSE;
    input->checkStandardWildEncounter = FALSE;
    input->pressedStartButton = FALSE;
    input->pressedSelectButton = FALSE;
    input->heldDirection = FALSE;
    input->heldDirection2 = FALSE;
    input->tookStep = FALSE;
    input->pressedBButton = FALSE;
    input->pressedRButton = FALSE;
    input->pressedLButton = FALSE;
    input->input_field_1_1 = FALSE;
    input->input_field_1_2 = FALSE;
    input->input_field_1_3 = FALSE;
    input->dpadDirection = 0;
}

void FieldGetPlayerInput(struct FieldInput *input, u16 newKeys, u16 heldKeys)
{
    u8 tileTransitionState = gPlayerAvatar.tileTransitionState;
    bool8 forcedMove = MetatileBehavior_IsForcedMovementTile(GetPlayerCurMetatileBehavior());

    if ((tileTransitionState == T_TILE_CENTER && !forcedMove) || tileTransitionState == T_NOT_MOVING)
    {
        if (GetPlayerSpeed() != 4)
        {
            if ((newKeys & START_BUTTON) && !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_FORCED))
                input->pressedStartButton = TRUE;
            
			if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_FORCED))
			{
				if (newKeys & SELECT_BUTTON)
					input->pressedSelectButton = TRUE;
				if (newKeys & A_BUTTON)
					input->pressedAButton = TRUE;
				if (newKeys & B_BUTTON)
					input->pressedBButton = TRUE;
				if (newKeys & R_BUTTON)
					input->pressedRButton = TRUE;
				if (newKeys & L_BUTTON && !IsDexNavSearchActive())
					input->pressedLButton = TRUE;
			}
        }
		
		if (heldKeys & (DPAD_UP | DPAD_DOWN | DPAD_LEFT | DPAD_RIGHT))
		{
			input->heldDirection = TRUE;
			input->heldDirection2 = TRUE;
		}
    }

    if (!forcedMove)
    {
        if (tileTransitionState == T_TILE_CENTER)
		{
			input->checkStandardWildEncounter = TRUE;
			
			if (gPlayerAvatar.runningState == MOVING)
				input->tookStep = TRUE;
		}
    }
	
	if (heldKeys & DPAD_UP)
		input->dpadDirection = DIR_NORTH;
	else if (heldKeys & DPAD_DOWN)
		input->dpadDirection = DIR_SOUTH;
	else if (heldKeys & DPAD_LEFT)
		input->dpadDirection = DIR_WEST;
	else if (heldKeys & DPAD_RIGHT)
		input->dpadDirection = DIR_EAST;
}

int ProcessPlayerFieldInput(struct FieldInput *input)
{
    struct MapPosition position;
    u8 playerDirection;
    u16 metatileBehavior;
    u32 metatileAttributes;

    ResetFacingNpcOrSignPostVars();
    playerDirection = GetPlayerFacingDirection();
    GetPlayerPosition(&position);
    metatileAttributes = MapGridGetMetatileAttributeAt(position.x, position.y, 0xFF);
    metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);

    if (CheckForTrainersWantingBattle() || TryRunOnFrameMapScript())
        return TRUE;

    if (input->tookStep)
    {
        IncrementGameStat(GAME_STAT_STEPS);
        IncrementRenewableHiddenItemStepCounter();
        RunMassageCooldownStepCounter();
        IncrementResortGorgeousStepCounter();
        IncrementBirthIslandRockStepCount();
		
#if TAUROS_CHARGE_STAMINA != 0
		DecreaseTaurosChargeStamina();
#endif
		
        if (TryStartStepBasedScript(&position, metatileBehavior))
            return TRUE;
    }
	
    if (input->checkStandardWildEncounter)
    {
        if (input->dpadDirection == 0 || input->dpadDirection == playerDirection)
        {
            GetInFrontOfPlayerPosition(&position);
            metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
            if (TrySetUpWalkIntoSignpostScript(&position, metatileBehavior, playerDirection))
                return TRUE;

            GetPlayerPosition(&position);
            metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
        }
    }
    if (input->checkStandardWildEncounter && TryStandardWildEncounter(metatileAttributes))
        return TRUE;

    if (input->heldDirection && input->dpadDirection == playerDirection)
    {
        if (TryArrowWarp(&position, metatileBehavior, playerDirection))
            return TRUE;
    }

    GetInFrontOfPlayerPosition(&position);
    metatileBehavior = MapGridGetMetatileBehaviorAt(position.x, position.y);
    if (input->heldDirection && input->dpadDirection == playerDirection)
    {
        if (TrySetUpWalkIntoSignpostScript(&position, metatileBehavior, playerDirection))
            return TRUE;
    }

    if (input->pressedAButton && TryStartInteractionScript(&position, metatileBehavior, playerDirection))
        return TRUE;

    if (input->heldDirection2 && input->dpadDirection == playerDirection)
    {
        if (TryDoorWarp(&position, metatileBehavior, playerDirection))
            return TRUE;
    }

    if (input->pressedStartButton)
    {
        FlagSet(FLAG_OPENED_START_MENU);
        PlaySE(SE_WIN_OPEN);
        ShowStartMenu();
        return TRUE;
    }
    if (input->pressedSelectButton && UseRegisteredKeyItemOnField())
        return TRUE;

    if (input->pressedRButton && DoLRButtonAction(FALSE))
		return TRUE;
	
    if (input->pressedLButton && DoLRButtonAction(TRUE))
        return TRUE;

    return FALSE;
}

void FieldInput_HandleCancelSignpost(struct FieldInput * input)
{
    if (ScriptContext1_IsScriptSetUp())
    {
        if (gWalkAwayFromSignInhibitTimer != 0)
            gWalkAwayFromSignInhibitTimer--;
        else if (CanWalkAwayToCancelMsgBox())
        {
            if (input->dpadDirection != 0 && GetPlayerFacingDirection() != input->dpadDirection)
            {
                if (IsMsgBoxWalkawayDisabled())
                    return;
				
                ScriptContext1_SetupScript(EventScript_CancelMessageBox);
                ScriptContext2_Enable();
            }
            else if (input->pressedStartButton)
            {
                ScriptContext1_SetupScript(EventScript_CancelMessageBox);
                ScriptContext2_Enable();
				
                if (!FuncIsActiveTask(Task_OpenStartMenu))
                    CreateTask(Task_OpenStartMenu, 8);
            }
        }
    }
}

static void Task_OpenStartMenu(u8 taskId)
{
    if (!ScriptContext2_IsEnabled())
    {
        PlaySE(SE_WIN_OPEN);
        ShowStartMenu();
        DestroyTask(taskId);
    }
}

static void GetPlayerPosition(struct MapPosition *position)
{
    PlayerGetDestCoords(&position->x, &position->y);
    position->height = PlayerGetZCoord();
}

static void GetInFrontOfPlayerPosition(struct MapPosition *position)
{
    s16 x, y;

    GetXYCoordsOneStepInFrontOfPlayer(&position->x, &position->y);
    PlayerGetDestCoords(&x, &y);
	position->height = MapGridGetZCoordAt(x, y) != 0 ? PlayerGetZCoord() : 0;
}

static u16 GetPlayerCurMetatileBehavior(void)
{
    s16 x, y;

    PlayerGetDestCoords(&x, &y);
    return MapGridGetMetatileBehaviorAt(x, y);
}

static bool8 TryStartInteractionScript(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    const u8 *script = GetInteractionScript(position, metatileBehavior, direction);
    if (script == NULL)
        return FALSE;

    // Don't play interaction sound for certain scripts.
    if (script != PalletTown_PlayersHouse_2F_EventScript_PC && script != EventScript_PC)
        PlaySE(SE_SELECT);

    ScriptContext1_SetupScript(script);
    return TRUE;
}

static const u8 *GetInteractionScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    const u8 *script = GetInteractedObjectEventScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedBackgroundEventScript(position, metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedMetatileScript(metatileBehavior, direction);
    if (script != NULL)
        return script;

    script = GetInteractedWaterScript(metatileBehavior);
    if (script != NULL)
        return script;

    return NULL;
}

const u8 *GetInteractedLinkPlayerScript(struct MapPosition *position, u8 direction)
{
    u8 objectEventId;
    s32 i;

    if (!MetatileBehavior_IsCounter(MapGridGetMetatileBehaviorAt(position->x, position->y)))
        objectEventId = GetObjectEventIdByXYZ(position->x, position->y, position->height);
    else
        objectEventId = GetObjectEventIdByXYZ(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->height);

    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
        return NULL;

    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        if (gLinkPlayerObjectEvents[i].active == TRUE && gLinkPlayerObjectEvents[i].objEventId == objectEventId)
            return NULL;
    }
    gSelectedObjectEvent = objectEventId;
    gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;
    gSpecialVar_Facing = direction;
    return GetObjectEventScriptPointerByObjectEventId(objectEventId);
}

static const u8 *GetInteractedObjectEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 objectEventId = GetObjectEventIdByXYZ(position->x, position->y, position->height);

    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
    {
        if (!MetatileBehavior_IsCounter(metatileBehavior))
            return NULL;

        // Look for an object event on the other side of the counter.
        objectEventId = GetObjectEventIdByXYZ(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->height);
        if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
            return NULL;
    }

    if (InUnionRoom() && !ObjectEventCheckHeldMovementStatus(&gObjectEvents[objectEventId]))
        return NULL;

    gSelectedObjectEvent = objectEventId;
    gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;
    gSpecialVar_Facing = direction;

    return GetRamScript(gSpecialVar_LastTalked, GetObjectEventScriptPointerByObjectEventId(objectEventId));
}

static const u8 *GetInteractedBackgroundEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 signpostType;
    const struct BgEvent *bgEvent = GetBackgroundEventAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);

    if (bgEvent == NULL)
        return NULL;
    else if (bgEvent->bgUnion.script == NULL)
        return EventScript_TestSignpostMsg;

    signpostType = GetFacingSignpostType(metatileBehavior, direction);

    switch (bgEvent->kind)
    {
    default:
        break;
    case BG_EVENT_PLAYER_FACING_NORTH:
        if (direction != DIR_NORTH)
            return NULL;
        break;
    case BG_EVENT_PLAYER_FACING_SOUTH:
        if (direction != DIR_SOUTH)
            return NULL;
        break;
    case BG_EVENT_PLAYER_FACING_EAST:
        if (direction != DIR_EAST)
            return NULL;
        break;
    case BG_EVENT_PLAYER_FACING_WEST:
        if (direction != DIR_WEST)
            return NULL;
        break;
    case 5:
    case 6:
    case BG_EVENT_HIDDEN_ITEM:
        if (GetHiddenItemAttr((u32)bgEvent->bgUnion.script, HIDDEN_ITEM_UNDERFOOT) == TRUE)
            return NULL;
		
        gSpecialVar_0x8005 = GetHiddenItemAttr((u32)bgEvent->bgUnion.script, HIDDEN_ITEM_ID);
        gSpecialVar_0x8004 = GetHiddenItemAttr((u32)bgEvent->bgUnion.script, HIDDEN_ITEM_FLAG);
        gSpecialVar_0x8006 = GetHiddenItemAttr((u32)bgEvent->bgUnion.script, HIDDEN_ITEM_QUANTITY);
		
        if (FlagGet(gSpecialVar_0x8004))
            return NULL;
		
        gSpecialVar_Facing = direction;
		
        return EventScript_HiddenItemScript;
    }

    if (signpostType != SIGNPOST_NA)
        MsgSetSignPost();
	
    gSpecialVar_Facing = direction;
	
    return bgEvent->bgUnion.script;
}

static const u8 *GetInteractedMetatileScript(u8 metatileBehavior, u8 direction)
{
    gSpecialVar_Facing = direction;
	
    if (MetatileBehavior_IsPC(metatileBehavior))
        return EventScript_PC;
    else if (MetatileBehavior_IsRegionMap(metatileBehavior))
        return EventScript_WallTownMap;
    else if (MetatileBehavior_IsBookshelf(metatileBehavior))
        return EventScript_Bookshelf;
    else if (MetatileBehavior_IsPokeMartShelf(metatileBehavior))
        return EventScript_PokeMartShelf;
    else if (MetatileBehavior_IsFood(metatileBehavior))
        return EventScript_Food;
    else if (MetatileBehavior_IsImpressiveMachine(metatileBehavior))
        return EventScript_ImpressiveMachine;
    else if (MetatileBehavior_IsBlueprints(metatileBehavior))
        return EventScript_Blueprints;
    else if (MetatileBehavior_IsVideoGame(metatileBehavior))
        return EventScript_VideoGame;
    else if (MetatileBehavior_IsBurglary(metatileBehavior))
        return EventScript_Burglary;
    else if (MetatileBehavior_IsComputer(metatileBehavior))
        return EventScript_Computer;
    else if (MetatileBehavior_IsPlayerFacingTVScreen(metatileBehavior, direction))
        return EventScript_PlayerFacingTVScreen;
    else if (MetatileBehavior_IsCabinet(metatileBehavior))
        return EventScript_Cabinet;
    else if (MetatileBehavior_IsKitchen(metatileBehavior))
        return EventScript_Kitchen;
    else if (MetatileBehavior_IsDresser(metatileBehavior))
        return EventScript_Dresser;
    else if (MetatileBehavior_IsSnacks(metatileBehavior))
        return EventScript_Snacks;
    else if (MetatileBehavior_IsPainting(metatileBehavior))
        return EventScript_Painting;
    else if (MetatileBehavior_IsPowerPlantMachine(metatileBehavior))
        return EventScript_PowerPlantMachine;
    else if (MetatileBehavior_IsTelephone(metatileBehavior))
        return EventScript_Telephone;
    else if (MetatileBehavior_IsAdvertisingPoster(metatileBehavior))
        return EventScript_AdvertisingPoster;
    else if (MetatileBehavior_IsTastyFood(metatileBehavior))
        return EventScript_TastyFood;
    else if (MetatileBehavior_IsTrashBin(metatileBehavior))
        return EventScript_TrashBin;
    else if (MetatileBehavior_IsCup(metatileBehavior))
        return EventScript_Cup;
    else if (MetatileBehavior_IsPolishedWindow(metatileBehavior))
        return EventScript_PolishedWindow;
    else if (MetatileBehavior_IsBeautifulSkyWindow(metatileBehavior))
        return EventScript_BeautifulSkyWindow;
    else if (MetatileBehavior_IsBlinkingLights(metatileBehavior))
        return EventScript_BlinkingLights;
    else if (MetatileBehavior_IsNeatlyLinedUpTools(metatileBehavior))
        return EventScript_NeatlyLinedUpTools;
    else if (MetatileBehavior_IsPlayerFacingCableClubWirelessMonitor(metatileBehavior, direction))
        return CableClub_EventScript_ShowWirelessCommunicationScreen;
    else if (MetatileBehavior_IsPlayerFacingBattleRecords(metatileBehavior, direction))
        return CableClub_EventScript_ShowBattleRecords;
    else if (MetatileBehavior_IsIndigoPlateauMark(metatileBehavior))
    {
        MsgSetSignPost();
        return EventScript_Indigo_UltimateGoal;
    }
    else if (MetatileBehavior_IsIndigoPlateauMark2(metatileBehavior))
    {
        MsgSetSignPost();
        return EventScript_Indigo_HighestAuthority;
    }
    else if (MetatileBehavior_IsPlayerFacingPokeMartSign(metatileBehavior, direction))
    {
        MsgSetSignPost();
        return EventScript_PokemartSign;
    }
    else if (MetatileBehavior_IsPlayerFacingPokemonCenterSign(metatileBehavior, direction))
    {
        MsgSetSignPost();
        return EventScript_PokecenterSign;
    }
    return NULL;
}

static const u8 *GetInteractedWaterScript(u8 metatileBehavior)
{
    if (CheckPlayerInGroundRocks())
		return NULL;
    if (MetatileBehavior_IsSemiDeepWater(metatileBehavior) && PlayerHasObtainedSharpedoPaddle())
        return EventScript_CurrentTooFast;
    if (PlayerHasObtainedSharpedoPaddle() && IsPlayerFacingSurfableFishableWater())
        return EventScript_UseSurf;

    if (MetatileBehavior_IsWaterfall(metatileBehavior))
		return (FlagGet(FLAG_BADGE07_GET) && IsPlayerSurfingNorth()) ? EventScript_Waterfall : EventScript_CantUseWaterfall;

    return NULL;
}

static bool8 TryStartStepBasedScript(struct MapPosition *position, u16 metatileBehavior)
{
    if (TryStartCoordEventScript(position) || TryStartWarpEventScript(position, metatileBehavior) || TryStartStepCountScript(metatileBehavior)
	|| (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_FORCED) && !MetatileBehavior_IsForcedMovementTile(metatileBehavior) && UpdateRepelCounter()))
        return TRUE;
    return FALSE;
}

static bool8 TryStartCoordEventScript(struct MapPosition *position)
{
    const u8 *script = GetCoordEventScriptAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);

    if (script == NULL)
        return FALSE;
    ScriptContext1_SetupScript(script);
    return TRUE;
}

static bool8 TryStartStepCountScript(u16 metatileBehavior)
{
    if (InUnionRoom())
        return FALSE;
	
    UpdateHappinessStepCounter();

    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_FORCED) && !MetatileBehavior_IsForcedMovementTile(metatileBehavior))
    {
        if (UpdateVsSeekerStepCounter())
        {
            ScriptContext1_SetupScript(EventScript_VsSeekerChargingDone);
            return TRUE;
        }
        else if (UpdatePoisonStepCounter())
        {
            ScriptContext1_SetupScript(EventScript_FieldPoison);
            return TRUE;
        }
        else if (ShouldEggHatch())
        {
            IncrementGameStat(GAME_STAT_HATCHED_EGGS);
            ScriptContext1_SetupScript(EventScript_EggHatch);
            return TRUE;
        }
    }
    if (SafariZoneTakeStep())
        return TRUE;
	
    return FALSE;
}

static void UpdateHappinessStepCounter(void)
{
    u16 *ptr = GetVarPointer(VAR_HAPPINESS_STEP_COUNTER);
    int i;

    (*ptr)++;
    (*ptr) %= 128;
    if (*ptr == 0)
    {
        for (i = 0; i < PARTY_SIZE; i++)
            AdjustFriendship(&gPlayerParty[i], FRIENDSHIP_EVENT_WALKING);
    }
}

void ClearPoisonStepCounter(void)
{
    VarSet(VAR_POISON_STEP_COUNTER, 0);
}

static bool8 UpdatePoisonStepCounter(void)
{
    u16 *ptr;

    if (gMapHeader.mapType != MAP_TYPE_SECRET_BASE)
    {
        ptr = GetVarPointer(VAR_POISON_STEP_COUNTER);
        (*ptr)++;
        (*ptr) %= 5;
        if (*ptr == 0)
        {
            switch (DoPoisonFieldEffect())
            {
            case FLDPSN_FNT:
                return TRUE;
			default:
			    return FALSE;
            }
        }
    }
    return FALSE;
}

static bool8 TrySetUpWalkIntoSignpostScript(struct MapPosition * position, u16 metatileBehavior, u8 playerDirection)
{
    u8 signpostType;
    const u8 * script;
	
    if (JOY_HELD(DPAD_LEFT | DPAD_RIGHT))
        return FALSE;
    else if (playerDirection == DIR_EAST || playerDirection == DIR_WEST)
        return FALSE;

    signpostType = GetFacingSignpostType(metatileBehavior, playerDirection);
    if (signpostType == SIGNPOST_POKECENTER)
    {
        SetUpWalkIntoSignScript(EventScript_PokecenterSign, playerDirection);
        return TRUE;
    }
    else if (signpostType == SIGNPOST_POKEMART)
    {
        SetUpWalkIntoSignScript(EventScript_PokemartSign, playerDirection);
        return TRUE;
    }
    else if (signpostType == SIGNPOST_INDIGO_1)
    {
        SetUpWalkIntoSignScript(EventScript_Indigo_UltimateGoal, playerDirection);
        return TRUE;
    }
    else if (signpostType == SIGNPOST_INDIGO_2)
    {
        SetUpWalkIntoSignScript(EventScript_Indigo_HighestAuthority, playerDirection);
        return TRUE;
    }
    else
    {
        script = GetSignpostScriptAtMapPosition(position);
        if (script == NULL)
            return FALSE;
        if (signpostType != SIGNPOST_SCRIPTED)
            return FALSE;
        SetUpWalkIntoSignScript(script, playerDirection);
        return TRUE;
    }
}

static u8 GetFacingSignpostType(u16 metatileBehavior, u8 playerDirection)
{
    if (MetatileBehavior_IsPlayerFacingPokemonCenterSign(metatileBehavior, playerDirection))
        return SIGNPOST_POKECENTER;

    if (MetatileBehavior_IsPlayerFacingPokeMartSign(metatileBehavior, playerDirection))
        return SIGNPOST_POKEMART;

    if (MetatileBehavior_IsIndigoPlateauMark(metatileBehavior))
        return SIGNPOST_INDIGO_1;

    if (MetatileBehavior_IsIndigoPlateauMark2(metatileBehavior))
        return SIGNPOST_INDIGO_2;

    if (MetatileBehavior_IsSignpost(metatileBehavior))
        return SIGNPOST_SCRIPTED;

    return SIGNPOST_NA;
}

static void SetUpWalkIntoSignScript(const u8 *script, u8 playerDirection)
{
    gSpecialVar_Facing = playerDirection;
    ScriptContext1_SetupScript(script);
    SetWalkingIntoSignVars();
    MsgSetSignPost();
}

static const u8 *GetSignpostScriptAtMapPosition(struct MapPosition * position)
{
    const struct BgEvent * event = GetBackgroundEventAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);
    if (event == NULL)
        return NULL;
    if (event->bgUnion.script != NULL)
        return event->bgUnion.script;
    return EventScript_TestSignpostMsg;
}

static bool8 TryArrowWarp(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    s8 warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);
    u16 delay;

    if (warpEventId != -1)
    {
        if (IsArrowWarpMetatileBehavior(metatileBehavior, direction))
        {
            StoreInitialPlayerAvatarState();
            SetupWarp(warpEventId, position);
            DoWarp();
            return TRUE;
        }
        else if (IsDirectionalStairWarpMetatileBehavior(metatileBehavior, direction))
        {
            if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_RIDE_ANY))
            {
                SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
                delay = 12;
            }
			else
				delay = 0;
			
            StoreInitialPlayerAvatarState();
            SetupWarp(warpEventId, position);
            DoStairWarp(metatileBehavior, delay);
            return TRUE;
        }
    }
    return FALSE;
}

static bool8 TryStartWarpEventScript(struct MapPosition *position, u16 metatileBehavior)
{
    s8 warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);

    if (warpEventId != -1 && IsWarpMetatileBehavior(metatileBehavior))
    {
        StoreInitialPlayerAvatarState();
        SetupWarp(warpEventId, position);
		
        if (MetatileBehavior_IsEscalator(metatileBehavior))
        {
            DoEscalatorWarp(metatileBehavior);
            return TRUE;
        }
        else if (MetatileBehavior_IsLavaridgeB1FWarp(metatileBehavior))
        {
            DoLavaridgeGymB1FWarp();
            return TRUE;
        }
        else if (MetatileBehavior_IsLavaridge1FWarp(metatileBehavior))
        {
            DoLavaridgeGym1FWarp();
            return TRUE;
        }
        else if (MetatileBehavior_IsWarpPad(metatileBehavior))
        {
            DoTeleportWarp();
            return TRUE;
        }
        else if (MetatileBehavior_IsUnionRoomWarp(metatileBehavior))
        {
            DoUnionRoomWarp();
            return TRUE;
        }
        else if (MetatileBehavior_IsFallWarp(metatileBehavior))
        {
            ResetInitialPlayerAvatarState();
            ScriptContext1_SetupScript(EventScript_1C1361);
            return TRUE;
        }
        DoWarp();
        return TRUE;
    }
    return FALSE;
}

static bool8 IsWarpMetatileBehavior(u16 metatileBehavior)
{
	if (MetatileBehavior_IsWarpDoor(metatileBehavior) || MetatileBehavior_IsLadder(metatileBehavior) || MetatileBehavior_IsEscalator(metatileBehavior)
	|| MetatileBehavior_IsNonAnimDoor(metatileBehavior) || MetatileBehavior_IsLavaridgeB1FWarp(metatileBehavior) || MetatileBehavior_IsLavaridge1FWarp(metatileBehavior)
	|| MetatileBehavior_IsWarpPad(metatileBehavior) || MetatileBehavior_IsFallWarp(metatileBehavior) || MetatileBehavior_IsUnionRoomWarp(metatileBehavior))
		return TRUE;
	
	return FALSE;
}

bool8 IsDirectionalStairWarpMetatileBehavior(u16 metatileBehavior, u8 playerDirection)
{
    switch (playerDirection)
    {
    case DIR_WEST:
        if (MetatileBehavior_IsDirectionalUpLeftStairWarp(metatileBehavior) || MetatileBehavior_IsDirectionalDownLeftStairWarp(metatileBehavior))
            return TRUE;
        break;
    case DIR_EAST:
        if (MetatileBehavior_IsDirectionalUpRightStairWarp(metatileBehavior) || MetatileBehavior_IsDirectionalDownRightStairWarp(metatileBehavior))
            return TRUE;
        break;
    }
    return FALSE;
}

static bool8 IsArrowWarpMetatileBehavior(u16 metatileBehavior, u8 direction)
{
    switch (direction)
    {
    case DIR_NORTH:
        return MetatileBehavior_IsNorthArrowWarp(metatileBehavior);
    case DIR_SOUTH:
        return MetatileBehavior_IsSouthArrowWarp(metatileBehavior);
    case DIR_WEST:
        return MetatileBehavior_IsWestArrowWarp(metatileBehavior);
    case DIR_EAST:
        return MetatileBehavior_IsEastArrowWarp(metatileBehavior);
    }
    return FALSE;
}

static s8 GetWarpEventAtMapPosition(struct MapHeader *mapHeader, struct MapPosition *position)
{
    return GetWarpEventAtPosition(mapHeader, position->x - 7, position->y - 7, position->height);
}

static void SetupWarp(s8 warpEventId, struct MapPosition *position)
{
    const struct WarpEvent *warpEvent = &gMapHeader.events->warps[warpEventId];

    if (warpEvent->mapNum == MAP_NUM(DYNAMIC))
        SetWarpDestinationToDynamicWarp();
    else
    {
        const struct MapHeader *mapHeader;

        SetWarpDestinationToMapWarp(warpEvent->mapGroup, warpEvent->mapNum, warpEvent->warpId);
        UpdateEscapeWarp(position->x, position->y);
        mapHeader = Overworld_GetMapHeaderByGroupAndId(warpEvent->mapGroup, warpEvent->mapNum);
        if (mapHeader->events->warps[warpEvent->warpId].mapNum == MAP_NUM(DYNAMIC))
            SetDynamicWarp(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, warpEventId);
    }
}

static bool8 TryDoorWarp(struct MapPosition *position, u16 metatileBehavior, u8 direction)
{
    s8 warpEventId;

    if (direction == DIR_NORTH)
    {
        if (MetatileBehavior_IsWarpDoor(metatileBehavior))
        {
            warpEventId = GetWarpEventAtMapPosition(&gMapHeader, position);
			
            if (warpEventId != -1 && IsWarpMetatileBehavior(metatileBehavior))
            {
                StoreInitialPlayerAvatarState();
                SetupWarp(warpEventId, position);
                DoDoorWarp();
                return TRUE;
            }
        }
    }
    return FALSE;
}

static s8 GetWarpEventAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    s32 i;
    struct WarpEvent *warpEvent = mapHeader->events->warps;

    for (i = 0; i < mapHeader->events->warpCount; i++, warpEvent++)
    {
        if ((u16)warpEvent->x == x && (u16)warpEvent->y == y)
        {
            if (warpEvent->elevation == elevation || warpEvent->elevation == 0)
                return i;
        }
    }
    return -1;
}

static const u8 *TryRunCoordEventScript(struct CoordEvent *coordEvent)
{
    if (coordEvent != NULL)
    {
        if (coordEvent->script == NULL)
            return NULL;

        if (coordEvent->trigger == 0)
        {
            ScriptContext2_RunNewScript(coordEvent->script);
            return NULL;
        }
        if (VarGet(coordEvent->trigger) == (u8)coordEvent->index)
            return coordEvent->script;
    }
    return NULL;
}

static const u8 *GetCoordEventScriptAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    s32 i;
    struct CoordEvent *coordEvents = mapHeader->events->coordEvents;

    for (i = 0; i < mapHeader->events->coordEventCount; i++)
    {
        if ((u16)coordEvents[i].x == x && (u16)coordEvents[i].y == y)
        {
            if (coordEvents[i].elevation == elevation || coordEvents[i].elevation == 0)
            {
                const u8 *script = TryRunCoordEventScript(&coordEvents[i]);
                if (script != NULL)
                    return script;
            }
        }
    }
    return NULL;
}

void HandleBoulderFallThroughOrCoverHole(struct ObjectEvent * object)
{
	s16 x = object->currentCoords.x, y = object->currentCoords.y;
	u8 metatileBehavior = MapGridGetMetatileBehaviorAt(x, y);
	
    if (metatileBehavior == MB_FALL_WARP)
    {
        PlaySE(SE_FALL);
        RemoveObjectEventByLocalIdAndMap(object->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
        FlagClear(GetObjectEventFlagByLocalIdAndMap(object->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup));
    }
	else if (metatileBehavior == MB_STRENGTH_HOLE)
	{
		PlaySE(SE_M_ROCK_THROW);
		RemoveObjectEventByLocalIdAndMap(object->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
		FlagClear(GetObjectEventFlagByLocalIdAndMap(object->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup));
		MapGridSetMetatileIdAt(x, y, METATILE_Cave_Covered_Hole);
		CurrentMapDrawMetatileAt(x, y);
	}
}

void HandleBoulderActivateVictoryRoadSwitch(u16 x, u16 y)
{
    int i;
    const struct CoordEvent * events = gMapHeader.events->coordEvents;
    int n = gMapHeader.events->coordEventCount;

    if (MapGridGetMetatileBehaviorAt(x, y) == MB_STRENGTH_BUTTON)
    {
        for (i = 0; i < n; i++)
        {
            if (events[i].x + 7 == x && events[i].y + 7 == y)
            {
                ScriptContext1_SetupScript(events[i].script);
                ScriptContext2_Enable();
            }
        }
    }
}

const u8 *GetCoordEventScriptAtMapPosition(struct MapPosition *position)
{
    return GetCoordEventScriptAtPosition(&gMapHeader, position->x - 7, position->y - 7, position->height);
}

static const struct BgEvent *GetBackgroundEventAtPosition(struct MapHeader *mapHeader, u16 x, u16 y, u8 elevation)
{
    u8 i;
    struct BgEvent *bgEvents = mapHeader->events->bgEvents;

    for (i = 0; i < mapHeader->events->bgEventCount; i++)
    {
        if ((u16)bgEvents[i].x == x && (u16)bgEvents[i].y == y)
        {
            if (bgEvents[i].elevation == elevation || bgEvents[i].elevation == 0)
                return &bgEvents[i];
        }
    }
    return NULL;
}

bool8 dive_warp(struct MapPosition *position, u16 metatileBehavior)
{
    if (gMapHeader.mapType == MAP_TYPE_UNDERWATER && !MetatileBehavior_IsUnableToEmerge(metatileBehavior))
    {
        if (SetDiveWarpEmerge(position->x - 7, position->y - 7))
        {
            StoreInitialPlayerAvatarState();
            DoDiveWarp();
            PlaySE(SE_M_DIVE);
            return TRUE;
        }
    }
    else if (MetatileBehavior_IsDiveable(metatileBehavior))
    {
        if (SetDiveWarpDive(position->x - 7, position->y - 7))
        {
            StoreInitialPlayerAvatarState();
            DoDiveWarp();
            PlaySE(SE_M_DIVE);
            return TRUE;
        }
    }
    return FALSE;
}

static u8 TrySetDiveWarp(void)
{
    s16 x, y;
    u8 metatileBehavior;

    PlayerGetDestCoords(&x, &y);
    metatileBehavior = MapGridGetMetatileBehaviorAt(x, y);
	
    if (gMapHeader.mapType == MAP_TYPE_UNDERWATER && !MetatileBehavior_IsUnableToEmerge(metatileBehavior))
    {
        if (SetDiveWarpEmerge(x - 7, y - 7))
            return 1;
    }
    else if (MetatileBehavior_IsDiveable(metatileBehavior))
    {
        if (SetDiveWarpDive(x - 7, y - 7))
            return 2;
    }
    return 0;
}

static const u8 *GetObjectEventScriptPointerPlayerFacing(void)
{
    struct MapPosition position;
    GetInFrontOfPlayerPosition(&position);
    return GetInteractedObjectEventScript(&position, MapGridGetMetatileBehaviorAt(position.x, position.y), GetPlayerMovementDirection());
}

int SetCableClubWarp(void)
{
    struct MapPosition position;
    GetPlayerPosition(&position);
    SetupWarp(GetWarpEventAtMapPosition(&gMapHeader, &position), &position);
    return 0;
}

static bool8 ToggleAutoRun(void)
{
	if (!FlagGet(FLAG_SYS_B_DASH))
		return FALSE;

	PlaySE(SE_SELECT);
	gSaveBlock2Ptr->autoRun ^= TRUE;
	ScriptContext1_SetupScript(gSaveBlock2Ptr->autoRun ? EventScript_EnableAutoRun : EventScript_DisableAutoRun);

	return TRUE;
}

static bool8 DoLRButtonAction(bool8 pressedL)
{
	if (pressedL)
		return gSaveBlock2Ptr->optionsDexnavSearchOnR ? ToggleAutoRun() : TryStartDexnavSearch();
	else
		return gSaveBlock2Ptr->optionsDexnavSearchOnR ? TryStartDexnavSearch() : ToggleAutoRun();
}
