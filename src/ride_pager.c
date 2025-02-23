#include "global.h"
#include "bike.h"
#include "event_data.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "fldeff.h"
#include "itemfinder.h"
#include "item_use.h"
#include "item_menu.h"
#include "link.h"
#include "list_menu.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "menu_indicators.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "region_map.h"
#include "ride_pager.h"
#include "script.h"
#include "script_menu.h"
#include "sound.h"
#include "start_menu.h"
#include "string_util.h"
#include "task.h"
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/flags.h"
#include "constants/poke_ride.h"
#include "constants/songs.h"
#include "constants/species.h"

////////////////
// RIDE PAGER //
////////////////

struct Ride
{
	struct MenuAction action;
	const u8 *desc;
	u16 species;
	u16 flagId;
	u16 playerAvatarFlag;
	u16 encounterRateMod; // > 100 increase rate, < 100 decrease rate
};

#define RIDE_PAGER_MON_PIC_X 18
#define RIDE_PAGER_MON_PIC_Y 1

static void Task_InitRidePager(u8 taskId);
static void CreateRidePagerMultichoiceWindow(u8 taskId);
static void UpdateRidePagerMonPic(u8 ride);
static void PrintRideDescInMessageWindow(u8 ride);
static void Task_RidePagerHandleInput(u8 taskId);
static void RidePager_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void DestroyRidePagerWindow(u8 taskId, bool8 useRide);
static void SharpedoPaddleCallback(u8 taskId);
static void CharizardGlideCallback(u8 taskId);
static void PlayerToPokeRideCallback(u8 taskId);
static void Task_CheckPlayerMovementAndUseRide(u8 taskId);
static void Task_SummonMonWaitPlayerAnim(u8 taskId);
static void Task_SummonMonAndSetPlayerAvatarFlag(u8 taskId);

static EWRAM_DATA struct ListMenuItem *sRidesList = NULL;
EWRAM_DATA u8 gUsingRideMon = 0;

// Extra args are player avatar flag and encounter rate mod
#define RIDE(_species, name, callback, _desc, ...)       \
    [RIDE_##_species - 1] =                   		     \
	{                                         		     \
		.action =                             		     \
		{                                     		     \
			.text = COMPOUND_STRING(name),    		     \
			{ .void_u8 = callback }           		     \
		},                                    		     \
		.desc = COMPOUND_STRING(_desc),       		     \
		.species = SPECIES_##_species,        		     \
		.flagId = FLAG_##_species##_RIDE_GET, 		     \
		.playerAvatarFlag = DEFAULT(0, __VA_ARGS__),     \
		.encounterRateMod = DEFAULT_2(100, __VA_ARGS__), \
	}

static const struct Ride sPokeRidesTable[] =
{
	RIDE(TAUROS,    "Tauros Charge",    PlayerToPokeRideCallback, "This Tauros's charge can\nbreak rocks.",          PLAYER_AVATAR_FLAG_TAUROS_RIDE,    100 - 50),
	RIDE(STOUTLAND, "Stoutland Search", PlayerToPokeRideCallback, "This Stoutland can search\nfor buried items.",    PLAYER_AVATAR_FLAG_STOUTLAND_RIDE, 100 - 45),
	RIDE(MUDSDALE,  "Mudsdale Gallop",  PlayerToPokeRideCallback, "This Mudsdale can run\nover rocky terrain.",      PLAYER_AVATAR_FLAG_MUDSDALE_RIDE,  100 - 35),
	RIDE(MACHAMP,   "Machamp Shove",    PlayerToPokeRideCallback, "This Machamp can move\nheavy boulders.",          PLAYER_AVATAR_FLAG_MACHAMP_RIDE),
	RIDE(SHARPEDO,  "Sharpedo Paddle",  SharpedoPaddleCallback,   "This Sharpedo allows you\nto surf on the water.", PLAYER_AVATAR_FLAG_SURFING,        100 - 85),
	RIDE(CHARIZARD, "Charizard Glide",  CharizardGlideCallback,   "This Charizard flies you\nto other places."),
};

u8 CountObtainedPokeRides(void)
{
	u8 i, count = 0;
	
	for (i = RIDE_NONE; i < NUM_RIDE_POKEMON; i++)
	{
		if (FlagGet(sPokeRidesTable[i].flagId))
			++count;
	}
	return count;
}

bool8 PlayerHasObtainedSharpedoPaddle(void)
{
	if (FlagGet(sPokeRidesTable[RIDE_SHARPEDO - 1].flagId) && !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
		return TRUE;
	else
		return FALSE;
}

u16 RideToSpeciesId(u8 ride)
{
	return sPokeRidesTable[ride - 1].species;
}

bool8 TryDismountPokeRide(void)
{
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_RIDE_ANY))
	{
		if (CheckPlayerInGroundRocks())
			return FALSE;
		
		TryRemoveStrengthFlag();
		SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
		Overworld_ClearSavedMusic();
		Overworld_PlaySpecialMapMusic();
	}
	return TRUE;
}

bool8 CheckPlayerInGroundRocks(void)
{
	s16 x, y;
	
	PlayerGetDestCoords(&x, &y);
	
	return MetatileBehavior_IsGroundRocks(MapGridGetMetatileBehaviorAt(x, y));
}

void TryRemoveStrengthFlag(void)
{
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACHAMP_RIDE))
		FlagClear(FLAG_SYS_USE_STRENGTH);
}

u32 ApplyRideEncounterRateMod(u32 encounterRate)
{
	u8 i;
	
	for (i = RIDE_NONE; i < NUM_RIDE_POKEMON; i++)
	{
		if (sPokeRidesTable[i].playerAvatarFlag && TestPlayerAvatarFlags(sPokeRidesTable[i].playerAvatarFlag))
		{
			encounterRate = encounterRate * sPokeRidesTable[i].encounterRateMod / 100;
			break;
		}
	}
	return encounterRate;
}

#define tState       data[0]
#define tWindowId    data[1]
#define tListTaskId  data[2]
#define tScrollId    data[3]
#define tCursorPos   data[4]
#define tCursorMoved data[5]
#define tSelectedId  data[6]

void InitRidePager(void)
{
	PlaySE(SE_WIN_OPEN);
	
	if (!IsUpdateLinkStateCBActive())
	{
		FreezeObjectEvents();
		HandleEnforcedLookDirectionOnPlayerStopMoving();
		StopPlayerAvatar();
	}
	ScriptContext2_Enable();
	
	CreateTask(Task_InitRidePager, 80);
}

static void Task_InitRidePager(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (tState)
	{
		case 0:
			CreateRidePagerMultichoiceWindow(taskId);
			tState++;
			break;
		case 1:
			CopyWindowToVram(tWindowId, COPYWIN_MAP);
			tState++;
			break;
		case 2:
			ScriptMenu_ShowPokemonPic(RideToSpeciesId(tSelectedId), RIDE_PAGER_MON_PIC_X, RIDE_PAGER_MON_PIC_Y);
			tState++;
			break;
		case 3:
			LoadMessageBoxAndFrameGfx(0, TRUE);
			PrintRideDescInMessageWindow(tSelectedId);
			tState++;
			break;
		case 4:
			gTasks[taskId].func = Task_RidePagerHandleInput;
			break;
	}
}

static void CreateRidePagerMultichoiceWindow(u8 taskId)
{
	u8 i, maxShowed, count = 0;
	s16 *data = gTasks[taskId].data;
	struct WindowTemplate template;
	struct ListMenuItem rides[NUM_RIDE_POKEMON];
	
	// Init rides list
	for (i = RIDE_NONE; i < NUM_RIDE_POKEMON; i++)
	{
		if (FlagGet(sPokeRidesTable[i].flagId))
		{
			rides[count].label = sPokeRidesTable[i].action.text;
			rides[count].index = i + 1;
			count++;
		}
	}
	sRidesList = Alloc(sizeof(struct ListMenuItem) * count);
	
	for (i = RIDE_NONE; i < count; i++)
		sRidesList[i] = rides[i];
	
	tSelectedId = rides[0].index;
	
	maxShowed = count > 6 ? 6 : count;
	
	// Create window
	LoadStdWindowFrameGfx();
	template = SetWindowTemplateFields(0, 1, 1, 13, maxShowed * 2, 15, 0xFC);
	tWindowId = AddWindow(&template);
	DrawStdWindowFrame(tWindowId, TRUE);
	
	// Create list menu
	gMultiuseListMenuTemplate.items = sRidesList;
	gMultiuseListMenuTemplate.totalItems = count;
	gMultiuseListMenuTemplate.windowId = tWindowId;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 0;
    gMultiuseListMenuTemplate.maxShowed = maxShowed;
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 1;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = RidePager_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = NULL;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
    gMultiuseListMenuTemplate.cursorKind = 0;
	
	tListTaskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);
	
	// Create scroll arrow
	tScrollId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 60, 5, 108, count - maxShowed, 110, 110, &tCursorPos);
}

static void PrintRideDescInMessageWindow(u8 ride)
{
	StringExpandPlaceholders(gStringVar4, sPokeRidesTable[ride - 1].desc);
	FillWindowPixelBuffer(0, PIXEL_FILL(1));
	AddTextPrinterWithCustomSpeedForMessage(FALSE, 0);
}

static void Task_RidePagerHandlePicboxUpdate(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (tState)
	{
		case 0:
			RemovePokemonSpeciesOnPicbox();
			++tState;
			break;
		case 1:
			UpdatePokemonSpeciesOnPicbox(RideToSpeciesId(tSelectedId), RIDE_PAGER_MON_PIC_X, RIDE_PAGER_MON_PIC_Y);
		    PrintRideDescInMessageWindow(tSelectedId);
			gTasks[taskId].func = Task_RidePagerHandleInput;
			break;
	}
}

static void Task_RidePagerHandleInput(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	s32 input = ListMenu_ProcessInput(tListTaskId);
	
	ListMenuGetScrollAndRow(tListTaskId, &tCursorPos, NULL);
	
	switch (input)
	{
		case LIST_CANCEL:
			DestroyRidePagerWindow(taskId, FALSE);
			break;
		case LIST_NOTHING_CHOSEN:
		    if (tCursorMoved)
			{
				tCursorMoved = FALSE;
				tState = 0;
				gTasks[taskId].func = Task_RidePagerHandlePicboxUpdate;
			}
			break;
		default:
			PlaySE(SE_SELECT);
			DestroyRidePagerWindow(taskId, TRUE);
			break;
	}
}

static void RidePager_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
	u8 taskId;
	
	if (!onInit)
	{
		PlaySE(SE_SELECT);
		
		taskId = FindTaskIdByFunc(Task_RidePagerHandleInput);
		gTasks[taskId].tSelectedId = itemIndex;
		gTasks[taskId].tCursorMoved = TRUE;
	}
}

static void DestroyRidePagerWindow(u8 taskId, bool8 useRide)
{
	s16 *data = gTasks[taskId].data;
	
	ClearStdWindowAndFrame(tWindowId, TRUE);
	RemoveWindow(tWindowId);
	DestroyListMenuTask(tListTaskId, NULL, NULL);
	RemoveScrollIndicatorArrowPair(tScrollId);
	FREE_AND_SET_NULL(sRidesList);
	PicboxCancel();
	ClearDialogWindowAndFrame(0, TRUE);
	ClearPlayerHeldMovementAndUnfreezeObjectEvents();
	
	if (useRide)
	{
		gPlayerAvatar.preventStep = TRUE;
		gTasks[taskId].func = sPokeRidesTable[tSelectedId - 1].action.func.void_u8;
	}
	else
	{
		PlaySE(SE_WIN_OPEN);
		ShowStartMenu();
		DestroyTask(taskId);
	}
}

static void PlayerToPokeRideCallback(u8 taskId)
{
	s16 x, y, *data = gTasks[taskId].data;
	u8 behavior;
	
	PlayerGetDestCoords(&x, &y);
	behavior = MapGridGetMetatileBehaviorAt(x, y);
	
	if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER | PLAYER_AVATAR_FLAG_SURFING) && !InUnionRoom() && !FlagGet(FLAG_SYS_ON_CYCLING_ROAD)
	    && !MetatileBehavior_IsVerticalRail(behavior) && !MetatileBehavior_IsHorizontalRail(behavior) && !MetatileBehavior_IsIsolatedVerticalRail(behavior)
	    && !MetatileBehavior_IsIsolatedHorizontalRail(behavior) && !MetatileBehavior_IsGroundRocks(behavior) && Overworld_IsBikingAllowed())
	{
		gBikeCameraAheadPanback = FALSE;
		
		if (TestPlayerAvatarFlags(sPokeRidesTable[tSelectedId - 1].playerAvatarFlag))
			TryDismountPokeRide();
		else
		{
			gTasks[taskId].func = Task_CheckPlayerMovementAndUseRide;
			return;
		}
	}
	else
		ScriptContext1_SetupScript(EventScript_CantUseRideHere);
	
	gPlayerAvatar.preventStep = FALSE;
	ScriptContext2_Disable();
	DestroyTask(taskId);
}

static void Task_CheckPlayerMovementAndUseRide(u8 taskId)
{
	if (!ObjectEventIsMovementOverridden(&gObjectEvents[gPlayerAvatar.objectEventId]) || ObjectEventClearHeldMovementIfFinished(&gObjectEvents[gPlayerAvatar.objectEventId]))
	{
		gUsingRideMon = gTasks[taskId].tSelectedId;
		StartPlayerAvatarSummonMonForFieldMoveAnim();
		ObjectEventSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
		gTasks[taskId].func = Task_SummonMonWaitPlayerAnim;
	}
}

static void Task_SummonMonWaitPlayerAnim(u8 taskId)
{
	if (ObjectEventCheckHeldMovementStatus(&gObjectEvents[gPlayerAvatar.objectEventId]))
	{
		gFieldEffectArguments[0] = 0 | SHOW_MON_CRY_NO_DUCKING;
		FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
		gTasks[taskId].func = Task_SummonMonAndSetPlayerAvatarFlag;
	}
}

static void Task_SummonMonAndSetPlayerAvatarFlag(u8 taskId)
{
	if (!FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
	{
		SetPlayerAvatarTransitionFlags(sPokeRidesTable[gTasks[taskId].tSelectedId - 1].playerAvatarFlag);
		
		if (gTasks[taskId].tSelectedId == RIDE_MACHAMP)
			FlagSet(FLAG_SYS_USE_STRENGTH);
		else
			FlagClear(FLAG_SYS_USE_STRENGTH);
		
		if (Overworld_MusicCanOverrideMapMusic(MUS_SURF))
		{
			Overworld_SetSavedMusic(MUS_SURF);
			Overworld_ChangeMusicTo(MUS_SURF);
		}
		FieldEffectActiveListRemove(FLDEFF_FIELD_MOVE_SHOW_MON);
		gPlayerAvatar.preventStep = FALSE;
		ScriptContext2_Disable();
		DestroyTask(taskId);
	}
}

static void SharpedoPaddleCallback(u8 taskId)
{
	s16 x, y;
	
	GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
	
	if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && !InUnionRoom() && !MetatileBehavior_IsSemiDeepWater(MapGridGetMetatileBehaviorAt(x, y)) && IsPlayerFacingSurfableFishableWater() && !CheckPlayerInGroundRocks())
	{
		TryRemoveStrengthFlag();
		gUsingRideMon = RIDE_SHARPEDO;
		gFieldEffectArguments[0] = 0;
		FieldEffectStart(FLDEFF_USE_SURF);
	}
	else
	{
		ScriptContext1_SetupScript(EventScript_CantUseRideToSurfHere);
		gPlayerAvatar.preventStep = FALSE;
		ScriptContext2_Disable();
	}
	DestroyTask(taskId);
}

static void Task_CharizardGlide_OpenMap(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		PlayRainStoppingSoundEffect();
		CleanupOverworldWindowsAndTilemaps();
		gUsingRideMon = RIDE_CHARIZARD;
		InitRegionMapWithExitCB(REGIONMAP_TYPE_FLY, NULL);
		gPlayerAvatar.preventStep = FALSE;
		ScriptContext2_Disable();
		DestroyTask(taskId);
	}
}

static void CharizardGlideCallback(u8 taskId)
{	
	if (!InUnionRoom() && Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) && !CheckPlayerInGroundRocks())
	{
		FadeScreen(FADE_TO_BLACK, 0);
		gTasks[taskId].func = Task_CharizardGlide_OpenMap;
	}
	else
	{
		ScriptContext1_SetupScript(EventScript_CantUseRideHere);
		gPlayerAvatar.preventStep = FALSE;
		ScriptContext2_Disable();
		DestroyTask(taskId);
	}
}

///////////////////
// RIDE MOVEMENT //
///////////////////

static u8 GetRideMovementInput(u8 *newDirection);
static u8 RideInputHandler_Normal(u8 *newDirection);
static u8 RideInputHandler_Turning(u8 *newDirection);
static void RideTransition_FaceDirection(u8 direction, u16 heldKeys);
static void RideTransition_MoveDirection(u8 direction, u16 heldKeys);
static void UpdateStoutlandSearchAndTaurosCharge(u16 heldKeys);

static void (*const sRideTransitions[])(u8, u16) =
{
    [BIKE_TRANS_FACE_DIRECTION] = RideTransition_FaceDirection,
    [BIKE_TRANS_TURNING]        = RideTransition_FaceDirection,
    [BIKE_TRANS_MOVE]           = RideTransition_MoveDirection,
};

static u8 (*const sRideInputHandlers[])(u8 *) =
{
    [BIKE_STATE_NORMAL]  = RideInputHandler_Normal,
    [BIKE_STATE_TURNING] = RideInputHandler_Turning,
};

void MovePlayerOnRide(u8 newDirection, u16 heldKeys)
{
	u8 input = GetRideMovementInput(&newDirection);
	
	UpdateStoutlandSearchAndTaurosCharge(heldKeys);
	
	sRideTransitions[input](newDirection, heldKeys);
}

static u8 GetRideMovementInput(u8 *newDirection)
{
	return sRideInputHandlers[gPlayerAvatar.acroBikeState](newDirection);
}

static u8 RideInputHandler_Normal(u8 *newDirection)
{
	u8 direction = GetPlayerMovementDirection();

	gPlayerAvatar.bikeFrameCounter = 0;
	
	if (*newDirection == DIR_NONE)
	{
		*newDirection = direction;
		gPlayerAvatar.runningState = NOT_MOVING;
		return BIKE_TRANS_FACE_DIRECTION;
	}
	else
	{
		if (*newDirection != direction && gPlayerAvatar.runningState != MOVING)
		{
			gPlayerAvatar.acroBikeState = BIKE_STATE_TURNING;
			gPlayerAvatar.newDirBackup = *newDirection;
			gPlayerAvatar.runningState = NOT_MOVING;
			return GetRideMovementInput(newDirection);
		}
		else
		{
			gPlayerAvatar.runningState = MOVING;
			return BIKE_TRANS_MOVE;
		}
	}
}

static u8 RideInputHandler_Turning(u8 *newDirection)
{
	*newDirection = gPlayerAvatar.newDirBackup;
	gPlayerAvatar.runningState = TURN_DIRECTION;
	gPlayerAvatar.acroBikeState = BIKE_STATE_NORMAL;
	gPlayerAvatar.bikeFrameCounter = 0;
	gPlayerAvatar.bikeSpeed = SPEED_STANDING;
	return BIKE_TRANS_TURNING;
}

static void RideTransition_FaceDirection(u8 direction, u16 heldKeys)
{
	PlayerFaceDirection(direction);
}

static void RideTransition_MoveDirection(u8 direction, u16 heldKeys)
{
	u8 collision = GetBikeCollision(direction);
	
	if (collision > COLLISION_NONE && collision <= COLLISION_WHEELIE_HOP)
	{
		if (collision == COLLISION_LEDGE_JUMP)
			PlayerJumpLedge(direction);
		else if (collision != COLLISION_STOP_SURFING && collision != COLLISION_LEDGE_JUMP && collision != COLLISION_PUSHED_BOULDER && collision != COLLISION_ROTATING_GATE)
			PlayerOnBikeCollide(direction);
	}
	else
	{
		if (collision == COLLISION_GROUND_ROCKS)
		{
			if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MUDSDALE_RIDE))
				PlayerGoSpeed2(direction);
			else
				PlayerOnBikeCollide(direction);
		}
		else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_TAUROS_RIDE) && !gSaveBlock2Ptr->waitingTaurosChargeStamina && (heldKeys & B_BUTTON))
			PlayerGoSpeed4(direction);
		else if (collision == COLLISION_COUNT || PlayerIsMovingOnRockStairs(direction) || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_STOUTLAND_RIDE) && heldKeys & B_BUTTON))
			PlayerGoSpeed2(direction);
		else
			PlayerRideWaterCurrent(direction);
	}
}

static void UpdateStoutlandSearchAndTaurosCharge(u16 heldKeys)
{
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_STOUTLAND_RIDE))
	{
		u8 taskId = FindTaskIdByFunc(Task_StoutlandSearch);
		
		if (heldKeys & B_BUTTON)
		{
			if (taskId == 0xFF && !IsMapNamePopupTaskActive())
				gTasks[CreateTask(Task_StoutlandSearch, 80)].data[7] = MAX_SPRITES;
		}
		else
		{
			if (taskId != 0xFF)
				gTasks[taskId].data[8] = TRUE;
		}
	}
	else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_TAUROS_RIDE))
	{
		if (!gSaveBlock2Ptr->waitingTaurosChargeStamina && (heldKeys & B_BUTTON) && gPlayerAvatar.runningState == MOVING
		&& CheckObjectGraphicsInFrontOfPlayer(OBJ_EVENT_GFX_ROCK_SMASH_ROCK))
			ScriptContext1_SetupScript(EventScript_UseRockSmash);
	}
}

void DecreaseTaurosChargeStamina(void)
{
	if (!gSaveBlock2Ptr->waitingTaurosChargeStamina && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_TAUROS_RIDE) && JOY_HELD(B_BUTTON))
	{
		if (--gSaveBlock2Ptr->taurosChargeStamina == 0)
			gSaveBlock2Ptr->waitingTaurosChargeStamina = TRUE;
	}
	else
	{
		gSaveBlock2Ptr->taurosChargeStamina += 2; // Takes half of steps to recharge
		if (gSaveBlock2Ptr->taurosChargeStamina > TAUROS_CHARGE_STAMINA)
			gSaveBlock2Ptr->taurosChargeStamina = TAUROS_CHARGE_STAMINA;
		
		if (gSaveBlock2Ptr->taurosChargeStamina == TAUROS_CHARGE_STAMINA)
			gSaveBlock2Ptr->waitingTaurosChargeStamina = FALSE;
	}
}
