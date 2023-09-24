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
#include "map_name_popup.h"
#include "menu.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "region_map.h"
#include "ride_pager.h"
#include "script.h"
#include "script_menu.h"
#include "sound.h"
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
struct RideSpeciesAndDesc
{
	u16 species;
	const u8 *desc;
};

#define RIDE_PAGER_MON_PIC_X 18
#define RIDE_PAGER_MON_PIC_Y 1

static u8 DrawRidePagerMultichoiceWindow(s16 *windowId, s16 *cursorPos, s16 *order);
static u8 CreateRidePagerMultichoiceWindow(void);
static void UpdateRidePagerMonPic(u8 ride);
static void PrintRideDescInMessageWindow(u8 ride);
static void Task_WaitFadeAndGoToRidePagerInput(u8 taskId);
static void Task_RidePagerHandleInput(u8 taskId);
static void DestroyRidePagerWindow(u8 taskId, bool8 useRide);
static void TaurosChargeCallback(u8 taskId);
static void StoutlandSearchCallback(u8 taskId);
static void MudsdaleGallopCallback(u8 taskId);
static void MachampShoveCallback(u8 taskId);
static void SharpedoPaddleCallback(u8 taskId);
static void CharizardGlideCallback(u8 taskId);
static void Task_StartPlayerToPokeRideEffect(u8 taskId);
static void Task_CheckPlayerMovementAndUseRide(u8 taskId);
static void Task_SummonMonWaitPlayerAnim(u8 taskId);
static void Task_SummonMonAndSetPlayerAvatarFlag(u8 taskId);

EWRAM_DATA u8 gUsingRideMon = 0;

static const u8 sTaurosCharge[] = _("Tauros Charge");
static const u8 sStoutlandSearch[] = _("Stoutland Search");
static const u8 sMudsdaleGallop[] = _("Mudsdale Gallop");
static const u8 sMachampShove[] = _("Machamp Shove");
static const u8 sSharpedoPaddle[] = _("Sharpedo Paddle");
static const u8 sCharizardGlide[] = _("Charizard Glide");

static const u8 sTaurosRideDesc[] = _("This Tauros's charge can\nbreak rocks.");
static const u8 sStoutlandRideDesc[] = _("This Stoutland can search\nfor buried items.");
static const u8 sMudsdaleRideDesc[] = _("This Mudsdale can run\nover rocky terrain.");
static const u8 sMachampRideDesc[] = _("This Machamp can move\nheavy boulders.");
static const u8 sSharpedoRideDesc[] = _("This Sharpedo allows you\nto surf on the water.");
static const u8 sCharizardRideDesc[] = _("This Charizard flies you\nto other places.");

static const struct MenuAction sPokeRideActionTable[NUM_RIDE_POKEMON] = {
    [RIDE_TAUROS]  = { sTaurosCharge, TaurosChargeCallback },
    [RIDE_STOUTLAND] = { sStoutlandSearch, StoutlandSearchCallback },
    [RIDE_MUDSDALE] = { sMudsdaleGallop, MudsdaleGallopCallback },
    [RIDE_MACHAMP] = { sMachampShove, MachampShoveCallback },
    [RIDE_SHARPEDO] = { sSharpedoPaddle, SharpedoPaddleCallback },
    [RIDE_CHARIZARD] = { sCharizardGlide, CharizardGlideCallback }
};

static const struct RideSpeciesAndDesc sRideToSpeciesAndDesc[NUM_RIDE_POKEMON] = 
{
	[RIDE_TAUROS]  = {
		.species = SPECIES_TAUROS,
		.desc = sTaurosRideDesc,
	},
	[RIDE_STOUTLAND] = {
		.species = SPECIES_STOUTLAND,
		.desc = sStoutlandRideDesc,
	},
	[RIDE_MUDSDALE] = {
		.species = SPECIES_MUDSDALE,
		.desc = sMudsdaleRideDesc,
	},
	[RIDE_MACHAMP] = {
		.species = SPECIES_MACHAMP,
		.desc = sMachampRideDesc,
	},
	[RIDE_SHARPEDO] = {
		.species = SPECIES_SHARPEDO,
		.desc = sSharpedoRideDesc,
	},
	[RIDE_CHARIZARD] = {
		.species = SPECIES_CHARIZARD,
		.desc = sCharizardRideDesc,
	}
};

u8 CountObtainedPokeRides(void)
{
	u8 i, count;
	
	for (i = 0, count = 0; i < (NUM_RIDE_POKEMON - 1); i++)
	{
		if (FlagGet(FLAG_TAUROS_RIDE_GET + i))
			++count;
	}
	return count;
}

bool8 PlayerHasObtainedSharpedoPaddle(void)
{
	if (FlagGet(FLAG_SHARPEDO_RIDE_GET) && !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
		return TRUE;
	else
		return FALSE;
}

u16 RideToSpeciesId(u8 ride)
{
	return sRideToSpeciesAndDesc[ride].species;
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

#define tWindowId  data[0]
#define tCursorPos data[1]
#define tCount     data[2]
#define tState     data[3]
#define tOrder(i)  data[i + 4]

#define tRide data[0]
#define tFlag data[1]

bool8 FieldCB_ReturnToFieldUseRidePager(void)
{
	u8 i, count;
	s16 *data, windowId, cursorPos, order[NUM_RIDE_POKEMON - 1];
	
	count = DrawRidePagerMultichoiceWindow(&windowId, &cursorPos, order);
	FadeInFromBlack();
	data = gTasks[CreateTask(Task_WaitFadeAndGoToRidePagerInput, 8)].data;
	tWindowId = windowId;
	tCursorPos = cursorPos;
	tCount = count;
	
	for (i = 0; i < count; i++)
		tOrder(i) = order[i];
	
	return TRUE;
}

void Task_InitRidePager(u8 taskId)
{
	u8 i;
	s16 *data = gTasks[taskId].data;
	
	for (i = 0; i < 16; i++)
		data[i] = 0;
	
	tCount = DrawRidePagerMultichoiceWindow(&tWindowId, &tCursorPos, &tOrder(0));
	gTasks[taskId].func = Task_RidePagerHandleInput;
}

static u8 DrawRidePagerMultichoiceWindow(s16 *windowId, s16 *cursorPos, s16 *order)
{
	u8 i, count;
	
	PlaySE(SE_WIN_OPEN);
	ScriptContext2_Enable();
	
	if (!IsUpdateLinkStateCBActive())
	{
		FreezeObjectEvents();
		HandleEnforcedLookDirectionOnPlayerStopMoving();
		StopPlayerAvatar();
	}
	LoadStdWindowFrameGfx();
	*windowId = CreateRidePagerMultichoiceWindow();
	DrawStdWindowFrame(*windowId, FALSE);
	
	for (i = 0, count = 0; i < (NUM_RIDE_POKEMON - 1); i++)
	{
		if (FlagGet(FLAG_TAUROS_RIDE_GET + i))
		{
			order[count] = i + 1;
			StringExpandPlaceholders(gStringVar4, sPokeRideActionTable[order[count]].text);
			AddTextPrinterParameterized(*windowId, 2, gStringVar4, 8, count * 16, 0xFF, NULL);
			++count;
		}
	}
	*cursorPos = Menu_InitCursor(*windowId, 2, 0, 0, 16, count, *cursorPos);
	CopyWindowToVram(*windowId, COPYWIN_MAP);
	ScriptMenu_ShowPokemonPic(RideToSpeciesId(order[*cursorPos]), RIDE_PAGER_MON_PIC_X, RIDE_PAGER_MON_PIC_Y);
	sub_80F7768(0, TRUE);
	PrintRideDescInMessageWindow(order[*cursorPos]);
	
	return count;
}

static u8 CreateRidePagerMultichoiceWindow(void)
{
	u8 windowId;
	
	struct WindowTemplate template = SetWindowTemplateFields(0, 1, 1, 13, CountObtainedPokeRides() * 2, 15, 0xFC);
	windowId = AddWindow(&template);
	PutWindowTilemap(windowId);
	
	return windowId;
}

static void PrintRideDescInMessageWindow(u8 ride)
{
	StringExpandPlaceholders(gStringVar4, sRideToSpeciesAndDesc[ride].desc);
	FillWindowPixelBuffer(0, PIXEL_FILL(1));
	AddTextPrinterWithCustomSpeedForMessage(FALSE, 0);
}

static void Task_WaitFadeAndGoToRidePagerInput(u8 taskId)
{
	if (FieldFadeTransitionBackgroundEffectIsFinished())
		gTasks[taskId].func = Task_RidePagerHandleInput;
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
			UpdatePokemonSpeciesOnPicbox(RideToSpeciesId(tOrder(tCursorPos)), RIDE_PAGER_MON_PIC_X, RIDE_PAGER_MON_PIC_Y);
		    PrintRideDescInMessageWindow(tOrder(tCursorPos));
			gTasks[taskId].func = Task_RidePagerHandleInput;
			break;
	}
}

static void Task_RidePagerHandleInput(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	if (JOY_NEW(DPAD_UP))
	{
		if (tCount > 1)
		{
			PlaySE(SE_SELECT);
			tCursorPos = Menu_MoveCursor(-1);
			tState = 0;
			gTasks[taskId].func = Task_RidePagerHandlePicboxUpdate;
			return;
		}
		PlaySE(SE_FAILURE);
	}
	else if (JOY_NEW(DPAD_DOWN))
	{
		if (tCount > 1)
		{
			PlaySE(SE_SELECT);
			tCursorPos = Menu_MoveCursor(+1);
			tState = 0;
			gTasks[taskId].func = Task_RidePagerHandlePicboxUpdate;
			return;
		}
		PlaySE(SE_FAILURE);
	}
	else if (JOY_NEW(A_BUTTON))
		DestroyRidePagerWindow(taskId, TRUE);
	else if (JOY_NEW(B_BUTTON))
		DestroyRidePagerWindow(taskId, FALSE);
}

static void DestroyRidePagerWindow(u8 taskId, bool8 useRide)
{
	s16 *data = gTasks[taskId].data;
	
	PlaySE(SE_SELECT);
	ClearStdWindowAndFrame(tWindowId, TRUE);
	RemoveWindow(tWindowId);
	PicboxCancel();
	ClearDialogWindowAndFrame(0, TRUE);
	ClearPlayerHeldMovementAndUnfreezeObjectEvents();
	
	if (useRide)
	{
		gPlayerAvatar.preventStep = TRUE;
		gTasks[taskId].func = sPokeRideActionTable[tOrder(tCursorPos)].func.void_u8;
	}
	else
	{
		ScriptContext2_Disable();
		DestroyTask(taskId);
	}
}

static void Task_StartPlayerToPokeRideEffect(u8 taskId)
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
		
		if (TestPlayerAvatarFlags(tFlag))
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
		gUsingRideMon = gTasks[taskId].tRide;
		StartPlayerAvatarSummonMonForFieldMoveAnim();
		ObjectEventSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
		gTasks[taskId].func = Task_SummonMonWaitPlayerAnim;
	}
}

static void Task_SummonMonWaitPlayerAnim(u8 taskId)
{
	if (ObjectEventCheckHeldMovementStatus(&gObjectEvents[gPlayerAvatar.objectEventId]))
	{
		gFieldEffectArguments[0] = 0 | 0x80000000;
		FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
		gTasks[taskId].func = Task_SummonMonAndSetPlayerAvatarFlag;
	}
}

static void Task_SummonMonAndSetPlayerAvatarFlag(u8 taskId)
{
	if (!FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
	{
		SetPlayerAvatarTransitionFlags(gTasks[taskId].tFlag);
		
		if (gTasks[taskId].tRide == RIDE_MACHAMP)
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

static void TaurosChargeCallback(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	tRide = RIDE_TAUROS;
	tFlag = PLAYER_AVATAR_FLAG_TAUROS_RIDE;
	gTasks[taskId].func = Task_StartPlayerToPokeRideEffect;
}

static void StoutlandSearchCallback(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	tRide = RIDE_STOUTLAND;
	tFlag = PLAYER_AVATAR_FLAG_STOUTLAND_RIDE;
	gTasks[taskId].func = Task_StartPlayerToPokeRideEffect;
}

static void MudsdaleGallopCallback(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	tRide = RIDE_MUDSDALE;
	tFlag = PLAYER_AVATAR_FLAG_MUDSDALE_RIDE;
	gTasks[taskId].func = Task_StartPlayerToPokeRideEffect;
}

static void MachampShoveCallback(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	tRide = RIDE_MACHAMP;
	tFlag = PLAYER_AVATAR_FLAG_MACHAMP_RIDE;
	gTasks[taskId].func = Task_StartPlayerToPokeRideEffect;
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
		DestroyTask(taskId);
	}
}

static void CharizardGlideCallback(u8 taskId)
{
	if (!InUnionRoom() && Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) && !CheckPlayerInGroundRocks())
	{
		FadeScreen(FADE_TO_BLACK, 0);
		gPlayerAvatar.preventStep = FALSE;
		ScriptContext2_Disable();
		gTasks[taskId].func = Task_CharizardGlide_OpenMap;
	}
	else
	{
		ScriptContext1_SetupScript(EventScript_CantUseRideHere);
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
		else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_TAUROS_RIDE) && heldKeys & B_BUTTON)
			PlayerGoSpeed4(direction);
		else if (collision == COLLISION_COUNT || PlayerIsMovingOnRockStairs(direction) || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_STOUTLAND_RIDE) && heldKeys & B_BUTTON))
			PlayerGoSpeed2(direction);
		else
			PlayerRideWaterCurrent(direction);
	}
}

static void UpdateStoutlandSearchAndTaurosCharge(u16 heldKeys)
{
	u8 taskId;
	
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_STOUTLAND_RIDE))
	{
		taskId = FindTaskIdByFunc(Task_StoutlandSearch);
		
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
		if (heldKeys & B_BUTTON && gPlayerAvatar.runningState == MOVING && CheckObjectGraphicsInFrontOfPlayer(OBJ_EVENT_GFX_ROCK_SMASH_ROCK))
			ScriptContext1_SetupScript(EventScript_UseRockSmash);
	}
}
