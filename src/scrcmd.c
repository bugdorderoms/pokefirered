#include "global.h"
#include "gflib.h"
#include "script.h"
#include "event_data.h"
#include "random.h"
#include "item.h"
#include "overworld.h"
#include "field_screen_effect.h"
#include "map_preview_screen.h"
#include "field_weather.h"
#include "field_tasks.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "script_movement.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "field_message_box.h"
#include "new_menu_helpers.h"
#include "script_menu.h"
#include "data.h"
#include "field_specials.h"
#include "constants/items.h"
#include "script_pokemon_util.h"
#include "pokemon_storage_system.h"
#include "party_menu.h"
#include "money.h"
#include "coins.h"
#include "battle_setup.h"
#include "shop.h"
#include "slot_machine.h"
#include "field_effect.h"
#include "fieldmap.h"
#include "field_door.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/moves.h"

extern u16 (*const gSpecials[])(void);
extern u16 (*const gSpecialsEnd[])(void);
extern const u8 *const gStdScripts[];
extern const u8 *const gStdScriptsEnd[];

static EWRAM_DATA ptrdiff_t gVScriptOffset = 0;
static EWRAM_DATA u16 sPauseCounter = 0;
static EWRAM_DATA u16 sMovingNpcId = 0;
static EWRAM_DATA u16 sMovingNpcMapBank = 0;
static EWRAM_DATA u16 sMovingNpcMapId = 0;
static EWRAM_DATA u16 sFieldEffectScriptId = 0;

u8 gSelectedObjectEvent;

// This is defined in here so the optimizer can't see its value when compiling
// script.c.
void *const gNullScriptPtr = NULL;

static const u8 sScriptConditionTable[6][3] =
{
//  <  =  >
    1, 0, 0, // <
    0, 1, 0, // =
    0, 0, 1, // >
    1, 1, 0, // <=
    0, 1, 1, // >=
    1, 0, 1, // !=
};

bool32 ScrCmd_nop(struct ScriptContext * ctx)
{
    return FALSE;
}

static bool32 WaitPlayerForFindItemAnim(void)
{
    struct ObjectEvent * playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    if (!ObjectEventIsMovementOverridden(playerObj) || ObjectEventClearHeldMovementIfFinished(playerObj))
    {
        StartPlayerAvatarSummonMonForFieldMoveAnim();
        ObjectEventSetHeldMovement(playerObj, MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
        return TRUE;
    }
    return FALSE;
}

bool32 ScrCmd_StartPlayerFindItemAnim(struct ScriptContext * ctx)
{
    ScriptContext2_Enable();
    gPlayerAvatar.preventStep = TRUE;
    SetupNativeScript(ctx, WaitPlayerForFindItemAnim);
    return TRUE;
}

static void Task_EndPlayerFindItemAnim(u32 taskId)
{
    struct ObjectEvent * playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    switch (gTasks[taskId].data[0])
    {
        case 0:
            if (ObjectEventClearHeldMovementIfFinished(playerObj))
            {
                ReturnPlayerToDefaultGraphicsIdByStateId(playerObj);
                ObjectEventForceSetHeldMovement(playerObj, GetFaceDirectionMovementAction(playerObj->facingDirection));
                ++gTasks[taskId].data[0];
            }
            break;
        case 1:
            if (ObjectEventClearHeldMovementIfFinished(playerObj))
                DestroyTask(taskId);
            break;
    }
}

static bool32 EndPlayerFindItemAnim(void)
{
    if (!FuncIsActiveTask(Task_EndPlayerFindItemAnim))
    {
        gPlayerAvatar.preventStep = FALSE;
        return TRUE;
    }
    return FALSE;
}

bool32 ScrCmd_EndPlayerFindItemAnim(struct ScriptContext * ctx)
{
    gTasks[CreateTask(Task_EndPlayerFindItemAnim, 80)].data[0] = 0;
    SetupNativeScript(ctx, EndPlayerFindItemAnim);
    return TRUE;
}

bool32 ScrCmd_end(struct ScriptContext * ctx)
{
    StopScript(ctx);
    return FALSE;
}

bool32 ScrCmd_gotonative(struct ScriptContext * ctx)
{
    bool32 (*func)(void) = (bool32 (*)(void))ScriptReadWord(ctx);
    SetupNativeScript(ctx, func);
    return TRUE;
}

bool32 ScrCmd_special(struct ScriptContext * ctx)
{
    u16 (*const *specialPtr)(void) = gSpecials + ScriptReadHalfword(ctx);
    
    if (specialPtr < gSpecialsEnd)
        (*specialPtr)();
    else
        AGB_ASSERT_EX(0, ABSPATH("scrcmd.c"), 241);
    
    return FALSE;
}

bool32 ScrCmd_specialvar(struct ScriptContext * ctx)
{
    u16 * varPtr = GetVarPointer(ScriptReadHalfword(ctx));
    u16 (*const *specialPtr)(void) = gSpecials + ScriptReadHalfword(ctx);
    
    if (specialPtr < gSpecialsEnd)
        *varPtr = (*specialPtr)();
    else
        AGB_ASSERT_EX(0, ABSPATH("scrcmd.c"), 263);
    
    return FALSE;
}

bool32 ScrCmd_callnative(struct ScriptContext * ctx)
{
    void (*func )(void) = ((void (*)(void))ScriptReadWord(ctx));
    func();
    return FALSE;
}

bool32 ScrCmd_waitstate(struct ScriptContext * ctx)
{
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_goto(struct ScriptContext * ctx)
{
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    ScriptJump(ctx, scrptr);
    return FALSE;
}

bool32 ScrCmd_return(struct ScriptContext * ctx)
{
    ScriptReturn(ctx);
    return FALSE;
}

bool32 ScrCmd_call(struct ScriptContext * ctx)
{
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    ScriptCall(ctx, scrptr);
    return FALSE;
}

bool32 ScrCmd_goto_if(struct ScriptContext * ctx)
{
    u32 condition = ScriptReadByte(ctx);
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    
    if (sScriptConditionTable[condition][ctx->comparisonResult] == 1)
        ScriptJump(ctx, scrptr);
    
    return FALSE;
}

bool32 ScrCmd_call_if(struct ScriptContext * ctx)
{
    u32 condition = ScriptReadByte(ctx);
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    
    if (sScriptConditionTable[condition][ctx->comparisonResult] == 1)
        ScriptCall(ctx, scrptr);
    
    return FALSE;
}

bool32 ScrCmd_setvaddress(struct ScriptContext * ctx)
{
    u32 addr1 = (u32)ctx->scriptPtr - 1;
    u32 addr2 = ScriptReadWord(ctx);

    gVScriptOffset = addr2 - addr1;
    return FALSE;
}

bool32 ScrCmd_vgoto(struct ScriptContext * ctx)
{
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    ScriptJump(ctx, scrptr - gVScriptOffset);
    return FALSE;
}

bool32 ScrCmd_vcall(struct ScriptContext * ctx)
{
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    ScriptCall(ctx, scrptr - gVScriptOffset);
    return FALSE;
}

bool32 ScrCmd_vgoto_if(struct ScriptContext * ctx)
{
    u32 condition = ScriptReadByte(ctx);
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx) - gVScriptOffset;
    
    if (sScriptConditionTable[condition][ctx->comparisonResult] == 1)
        ScriptJump(ctx, scrptr);
    
    return FALSE;
}

bool32 ScrCmd_vcall_if(struct ScriptContext * ctx)
{
    u32 condition = ScriptReadByte(ctx);
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx) - gVScriptOffset;
    
    if (sScriptConditionTable[condition][ctx->comparisonResult] == 1)
        ScriptCall(ctx, scrptr);
    
    return FALSE;
}

bool32 ScrCmd_gotostd(struct ScriptContext * ctx)
{
    u32 stdIdx = ScriptReadByte(ctx);
    const u8 *const * script = gStdScripts + stdIdx;
    
    if (script < gStdScriptsEnd)
        ScriptJump(ctx, *script);
    
    return FALSE;
}

bool32 ScrCmd_callstd(struct ScriptContext * ctx)
{
    u32 stdIdx = ScriptReadByte(ctx);
    const u8 *const * script = gStdScripts + stdIdx;
    
    if (script < gStdScriptsEnd)
        ScriptCall(ctx, *script);
    
    return FALSE;
}

bool32 ScrCmd_gotostd_if(struct ScriptContext * ctx)
{
    u32 condition = ScriptReadByte(ctx);
    u32 stdIdx = ScriptReadByte(ctx);
    
    if (sScriptConditionTable[condition][ctx->comparisonResult] == 1)
    {
        const u8 *const * script = gStdScripts + stdIdx;
        if (script < gStdScriptsEnd)
            ScriptJump(ctx, *script);
    }
    return FALSE;
}

bool32 ScrCmd_callstd_if(struct ScriptContext * ctx)
{
    u32 condition = ScriptReadByte(ctx);
    u32 stdIdx = ScriptReadByte(ctx);
    
    if (sScriptConditionTable[condition][ctx->comparisonResult] == 1)
    {
        const u8 *const * script = gStdScripts + stdIdx;
        if (script < gStdScriptsEnd)
            ScriptCall(ctx, *script);
    }
    return FALSE;
}

bool32 ScrCmd_gotoram(struct ScriptContext * ctx)
{
    ScriptJump(ctx, gRAMScriptPtr);
    return FALSE;
}

bool32 ScrCmd_killscript(struct ScriptContext * ctx)
{
    ClearRamScript();
    StopScript(ctx);
    return TRUE;
}

bool32 ScrCmd_jumpifcantgivenick(struct ScriptContext * ctx)
{
    const u8 * scrptr = (const u8 *)ScriptReadWord(ctx);
    
    if (gSaveBlock2Ptr->optionsSkipPkmnNickname)
        ScriptJump(ctx, scrptr);
    
    return FALSE;
}

bool32 ScrCmd_execram(struct ScriptContext * ctx)
{
    return FALSE;
}

bool32 ScrCmd_loadword(struct ScriptContext * ctx)
{
    u32 which = ScriptReadByte(ctx);
    ctx->data[which] = ScriptReadWord(ctx);
    return FALSE;
}

bool32 ScrCmd_loadbytefromaddr(struct ScriptContext * ctx)
{
    u32 which = ScriptReadByte(ctx);
    ctx->data[which] = *(const u8 *)ScriptReadWord(ctx);
    return FALSE;
}

bool32 ScrCmd_writebytetoaddr(struct ScriptContext * ctx)
{
    u32 value = ScriptReadByte(ctx);
    *(u8 *)ScriptReadWord(ctx) = value;
    return FALSE;
}

bool32 ScrCmd_loadbyte(struct ScriptContext * ctx)
{
    u32 which = ScriptReadByte(ctx);
    ctx->data[which] = ScriptReadByte(ctx);
    return FALSE;
}

bool32 ScrCmd_setptrbyte(struct ScriptContext * ctx)
{
    u32 which = ScriptReadByte(ctx);
    *(u8 *)ScriptReadWord(ctx) = ctx->data[which];
    return FALSE;
}

bool32 ScrCmd_copylocal(struct ScriptContext * ctx)
{
    u32 whichDst = ScriptReadByte(ctx);
    u32 whichSrc = ScriptReadByte(ctx);
    ctx->data[whichDst] = ctx->data[whichSrc];
    return FALSE;
}

bool32 ScrCmd_copybyte(struct ScriptContext * ctx)
{
    u8 * dest = (u8 *)ScriptReadWord(ctx);
    *dest = *(const u8 *)ScriptReadWord(ctx);
    return FALSE;
}

bool32 ScrCmd_setvar(struct ScriptContext * ctx)
{
    u16 * varPtr = GetVarPointer(ScriptReadHalfword(ctx));
    *varPtr = ScriptReadHalfword(ctx);
    return FALSE;
}

bool32 ScrCmd_copyvar(struct ScriptContext * ctx)
{
    u16 * destPtr = GetVarPointer(ScriptReadHalfword(ctx));
    u16 * srcPtr = GetVarPointer(ScriptReadHalfword(ctx));
    *destPtr = *srcPtr;
    return FALSE;
}

bool32 ScrCmd_setorcopyvar(struct ScriptContext * ctx)
{
    u16 * destPtr = GetVarPointer(ScriptReadHalfword(ctx));
    *destPtr = VarGet(ScriptReadHalfword(ctx));
    return FALSE;
}

static u32 compare_012(u16 left, u16 right)
{
    if (left < right)
        return 0;
    else if (left == right)
        return 1;
    else
        return 2;
}

// comparelocaltolocal
bool32 ScrCmd_compare_local_to_local(struct ScriptContext * ctx)
{
    const u8 value1 = ctx->data[ScriptReadByte(ctx)];
    const u8 value2 = ctx->data[ScriptReadByte(ctx)];

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

// comparelocaltoimm
bool32 ScrCmd_compare_local_to_value(struct ScriptContext * ctx)
{
    const u8 value1 = ctx->data[ScriptReadByte(ctx)];
    const u8 value2 = ScriptReadByte(ctx);

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

bool32 ScrCmd_compare_local_to_addr(struct ScriptContext * ctx)
{
    const u8 value1 = ctx->data[ScriptReadByte(ctx)];
    const u8 value2 = *(const u8 *)ScriptReadWord(ctx);

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

bool32 ScrCmd_compare_addr_to_local(struct ScriptContext * ctx)
{
    const u8 value1 = *(const u8 *)ScriptReadWord(ctx);
    const u8 value2 = ctx->data[ScriptReadByte(ctx)];

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

bool32 ScrCmd_compare_addr_to_value(struct ScriptContext * ctx)
{
    const u8 value1 = *(const u8 *)ScriptReadWord(ctx);
    const u8 value2 = ScriptReadByte(ctx);

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

bool32 ScrCmd_compare_addr_to_addr(struct ScriptContext * ctx)
{
    const u8 value1 = *(const u8 *)ScriptReadWord(ctx);
    const u8 value2 = *(const u8 *)ScriptReadWord(ctx);

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

bool32 ScrCmd_compare_var_to_value(struct ScriptContext * ctx)
{
    const u16 value1 = *GetVarPointer(ScriptReadHalfword(ctx));
    const u16 value2 = ScriptReadHalfword(ctx);

    ctx->comparisonResult = compare_012(value1, value2);
    return FALSE;
}

bool32 ScrCmd_compare_var_to_var(struct ScriptContext * ctx)
{
    const u16 *ptr1 = GetVarPointer(ScriptReadHalfword(ctx));
    const u16 *ptr2 = GetVarPointer(ScriptReadHalfword(ctx));

    ctx->comparisonResult = compare_012(*ptr1, *ptr2);
    return FALSE;
}

bool32 ScrCmd_addvar(struct ScriptContext * ctx)
{
    u16 *ptr = GetVarPointer(ScriptReadHalfword(ctx));
    *ptr += ScriptReadHalfword(ctx);
    return FALSE;
}

bool32 ScrCmd_subvar(struct ScriptContext * ctx)
{
    u16 *ptr = GetVarPointer(ScriptReadHalfword(ctx));
    *ptr -= VarGet(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_random(struct ScriptContext * ctx)
{
    u32 max = VarGet(ScriptReadHalfword(ctx));
    gSpecialVar_Result = RandomMax(max);
    return FALSE;
}

bool32 ScrCmd_additem(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = AddBagItem(itemId, (u8)quantity);
    return FALSE;
}

bool32 ScrCmd_removeitem(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = RemoveBagItem(itemId, (u8)quantity);
    return FALSE;
}

bool32 ScrCmd_checkitemspace(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = CheckBagHasSpace(itemId, (u8)quantity);
    return FALSE;
}

bool32 ScrCmd_checkitem(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = CheckBagHasItem(itemId, (u8)quantity);
    return FALSE;
}

bool32 ScrCmd_checkitemtype(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = ItemId_GetPocket(itemId);
    return FALSE;
}

bool32 ScrCmd_addpcitem(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = AddPCItem(itemId, quantity);
    return FALSE;
}

bool32 ScrCmd_checkpcitem(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    gSpecialVar_Result = CheckPCHasItem(itemId, quantity);
    return FALSE;
}

bool32 ScrCmd_givedecoration(struct ScriptContext * ctx)
{
    u32 decorId = VarGet(ScriptReadHalfword(ctx));

//    gSpecialVar_Result = DecorationAdd(decorId);
    return FALSE;
}

bool32 ScrCmd_takedecoration(struct ScriptContext * ctx)
{
    u32 decorId = VarGet(ScriptReadHalfword(ctx));

//    gSpecialVar_Result = DecorationRemove(decorId);
    return FALSE;
}

bool32 ScrCmd_checkdecorspace(struct ScriptContext * ctx)
{
    u32 decorId = VarGet(ScriptReadHalfword(ctx));

//    gSpecialVar_Result = DecorationCheckSpace(decorId);
    return FALSE;
}

bool32 ScrCmd_checkdecor(struct ScriptContext * ctx)
{
    u32 decorId = VarGet(ScriptReadHalfword(ctx));

//    gSpecialVar_Result = CheckHasDecoration(decorId);
    return FALSE;
}

bool32 ScrCmd_setflag(struct ScriptContext * ctx)
{
    FlagSet(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_clearflag(struct ScriptContext * ctx)
{
    FlagClear(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_checkflag(struct ScriptContext * ctx)
{
    ctx->comparisonResult = FlagGet(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_incrementgamestat(struct ScriptContext * ctx)
{
    IncrementGameStat(ScriptReadByte(ctx));
    return FALSE;
}

bool32 ScrCmd_comparestattoword(struct ScriptContext * ctx)
{
    u32 statIdx = ScriptReadByte(ctx);
    u32 value = ScriptReadWord(ctx);
    u32 statValue = GetGameStat(statIdx);

    if (statValue < value)
        ctx ->comparisonResult = 0;
    else if (statValue == value)
        ctx->comparisonResult = 1;
    else
        ctx->comparisonResult = 2;
    
    return FALSE;
}

bool32 ScrCmd_setworldmapflag(struct ScriptContext * ctx)
{
    MapPreview_SetFlag(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_animateflash(struct ScriptContext * ctx)
{
    AnimateFlash(ScriptReadByte(ctx));
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_setflashradius(struct ScriptContext * ctx)
{
    u32 flashLevel = VarGet(ScriptReadHalfword(ctx));

    Overworld_SetFlashLevel(flashLevel);
    return FALSE;
}

static bool32 IsPaletteNotActive(void)
{
    if (!gPaletteFade.active)
        return TRUE;
    else
        return FALSE;
}

bool32 ScrCmd_fadescreen(struct ScriptContext * ctx)
{
    FadeScreen(ScriptReadByte(ctx), 0);
    SetupNativeScript(ctx, IsPaletteNotActive);
    return TRUE;
}

bool32 ScrCmd_fadescreenspeed(struct ScriptContext * ctx)
{
    u32 mode = ScriptReadByte(ctx);
    u32 speed = ScriptReadByte(ctx);

    FadeScreen(mode, speed);
    SetupNativeScript(ctx, IsPaletteNotActive);
    return TRUE;
}

static bool32 RunPauseTimer(void)
{
    sPauseCounter--;

    if (sPauseCounter == 0)
        return TRUE;
    else
        return FALSE;
}

bool32 ScrCmd_delay(struct ScriptContext * ctx)
{
    sPauseCounter = ScriptReadHalfword(ctx);
    SetupNativeScript(ctx, RunPauseTimer);
    return TRUE;
}

bool32 ScrCmd_starttotembattle(struct ScriptContext * ctx)
{
    u32 i;
    s8 buffs[NUM_BATTLE_STATS];
    
    for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
        buffs[i] = ScriptReadByte(ctx);
    
    StartTotemBattle(buffs);
    return FALSE;
}

bool32 ScrCmd_dodailyevents(struct ScriptContext * ctx)
{
//      DoTimeBasedEvents();
    return FALSE;
}

bool32 ScrCmd_gettime(struct ScriptContext * ctx)
{
//    RtcCalcLocalTime();
//    gSpecialVar_0x8000 = gLocalTime.hours;
//    gSpecialVar_0x8001 = gLocalTime.minutes;
//    gSpecialVar_0x8002 = gLocalTime.seconds;
    gSpecialVar_0x8000 = 0;
    gSpecialVar_0x8001 = 0;
    gSpecialVar_0x8002 = 0;
    return FALSE;
}

bool32 ScrCmd_setweather(struct ScriptContext * ctx)
{
    u32 weather = VarGet(ScriptReadHalfword(ctx));

    SetSav1Weather(weather);
    return FALSE;
}

bool32 ScrCmd_resetweather(struct ScriptContext * ctx)
{
    SetSav1WeatherFromCurrMapHeader();
    return FALSE;
}

bool32 ScrCmd_doweather(struct ScriptContext * ctx)
{
    DoCurrentWeather();
    return FALSE;
}

bool32 ScrCmd_setstepcallback(struct ScriptContext * ctx)
{
    ActivatePerStepCallback(ScriptReadByte(ctx));
    return FALSE;
}

bool32 ScrCmd_setmaplayoutindex(struct ScriptContext * ctx)
{
    u32 value = VarGet(ScriptReadHalfword(ctx));

    SetCurrentMapLayout(value);
    return FALSE;
}

bool32 ScrCmd_warp(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    DoWarp();
    ResetInitialPlayerAvatarState();
    return TRUE;
}

bool32 ScrCmd_warpsilent(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    DoDiveWarp();
    ResetInitialPlayerAvatarState();
    return TRUE;
}

bool32 ScrCmd_warpdoor(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    DoDoorWarp();
    ResetInitialPlayerAvatarState();
    return TRUE;
}

bool32 ScrCmd_warphole(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u16 x, y;

    PlayerGetDestCoords(&x, &y);
    
    if (mapGroup == 0xFF && mapNum == 0xFF)
        SetWarpDestinationToFixedHoleWarp(x - 7, y - 7);
    else
        SetWarpDestination(mapGroup, mapNum, -1, x - 7, y - 7);
    
    DoFallWarp();
    ResetInitialPlayerAvatarState();
    return TRUE;
}

bool32 ScrCmd_warpteleport(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    DoTeleportWarp();
    ResetInitialPlayerAvatarState();
    return TRUE;
}

bool32 ScrCmd_warpteleport2(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    SavePlayerFacingDirectionForTeleport(GetPlayerFacingDirection());
    DoTeleport2Warp();
    ResetInitialPlayerAvatarState();
    return TRUE;
}

bool32 ScrCmd_setwarp(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetWarpDestination(mapGroup, mapNum, warpId, x, y);
    return FALSE;
}

bool32 ScrCmd_setdynamicwarp(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetDynamicWarpWithCoords(mapGroup, mapNum, warpId, x, y);
    return FALSE;
}

bool32 ScrCmd_setdivewarp(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetFixedDiveWarp(mapGroup, mapNum, warpId, x, y);
    return FALSE;
}

bool32 ScrCmd_setholewarp(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetFixedHoleWarp(mapGroup, mapNum, warpId, x, y);
    return FALSE;
}

bool32 ScrCmd_setescapewarp(struct ScriptContext * ctx)
{
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 warpId = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    SetEscapeWarp(mapGroup, mapNum, warpId, x, y);
    return FALSE;
}

bool32 ScrCmd_getplayerxy(struct ScriptContext * ctx)
{
    u16 *pX = GetVarPointer(ScriptReadHalfword(ctx));
    u16 *pY = GetVarPointer(ScriptReadHalfword(ctx));

    *pX = gSaveBlock1Ptr->pos.x;
    *pY = gSaveBlock1Ptr->pos.y;
    return FALSE;
}

bool32 ScrCmd_getpartysize(struct ScriptContext * ctx)
{
    gSpecialVar_Result = CalculatePlayerPartyCount();
    return FALSE;
}

bool32 ScrCmd_playse(struct ScriptContext * ctx)
{
    PlaySE(ScriptReadHalfword(ctx));
    return FALSE;
}

static bool32 WaitForSoundEffectFinish(void)
{
    if (!IsSEPlaying())
        return TRUE;
    else
        return FALSE;
}

bool32 ScrCmd_waitse(struct ScriptContext * ctx)
{
    SetupNativeScript(ctx, WaitForSoundEffectFinish);
    return TRUE;
}

bool32 ScrCmd_playfanfare(struct ScriptContext * ctx)
{
    PlayFanfare(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_waitfanfare(struct ScriptContext * ctx)
{
    SetupNativeScript(ctx, IsFanfareTaskInactive);
    return TRUE;
}

bool32 ScrCmd_playbgm(struct ScriptContext * ctx)
{
    u32 songId = ScriptReadHalfword(ctx);
    bool32 val = ScriptReadByte(ctx);

    if (val == TRUE)
        Overworld_SetSavedMusic(songId);
    
    PlayNewMapMusic(songId);
    return FALSE;
}

bool32 ScrCmd_savebgm(struct ScriptContext * ctx)
{
    Overworld_SetSavedMusic(ScriptReadHalfword(ctx));
    return FALSE;
}

bool32 ScrCmd_fadedefaultbgm(struct ScriptContext * ctx)
{
    Overworld_ChangeMusicToDefault();
    return FALSE;
}

bool32 ScrCmd_fadenewbgm(struct ScriptContext * ctx)
{
    u32 music = ScriptReadHalfword(ctx);
    Overworld_ChangeMusicTo(music);
    return FALSE;
}

bool32 ScrCmd_fadeoutbgm(struct ScriptContext * ctx)
{
    u32 speed = ScriptReadByte(ctx);

    if (speed != 0)
        FadeOutBGMTemporarily(4 * speed);
    else
        FadeOutBGMTemporarily(4);
    
    SetupNativeScript(ctx, IsBGMPausedOrStopped);
    return TRUE;
}

bool32 ScrCmd_fadeinbgm(struct ScriptContext * ctx)
{
    u32 speed = ScriptReadByte(ctx);

    if (speed != 0)
        FadeInBGM(4 * speed);
    else
        FadeInBGM(4);
    
    return FALSE;
}

bool32 ScrCmd_applymovement(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    const void *movementScript = (const void *)ScriptReadWord(ctx);

    ScriptMovement_StartObjectMovementScript(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, movementScript);
    sMovingNpcId = localId;
    return FALSE;
}

bool32 ScrCmd_applymovement_at(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    const void *movementScript = (const void *)ScriptReadWord(ctx);
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);

    ScriptMovement_StartObjectMovementScript(localId, mapNum, mapGroup, movementScript);
    sMovingNpcId = localId;
    return FALSE;
}

static bool32 WaitForMovementFinish(void)
{
    return ScriptMovement_IsObjectMovementFinished(sMovingNpcId, sMovingNpcMapId, sMovingNpcMapBank);
}

bool32 ScrCmd_waitmovement(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));

    if (localId != 0)
        sMovingNpcId = localId;
    
    sMovingNpcMapBank = gSaveBlock1Ptr->location.mapGroup;
    sMovingNpcMapId = gSaveBlock1Ptr->location.mapNum;
    SetupNativeScript(ctx, WaitForMovementFinish);
    return TRUE;
}

bool32 ScrCmd_waitmovement_at(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 mapBank;
    u32 mapId;

    if (localId != 0)
        sMovingNpcId = localId;
    
    mapBank = ScriptReadByte(ctx);
    mapId = ScriptReadByte(ctx);
    sMovingNpcMapBank = mapBank;
    sMovingNpcMapId = mapId;
    SetupNativeScript(ctx, WaitForMovementFinish);
    return TRUE;
}

bool32 ScrCmd_removeobject(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));

    RemoveObjectEventByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    return FALSE;
}

bool32 ScrCmd_removeobject_at(struct ScriptContext * ctx)
{
    u32 objectId = VarGet(ScriptReadHalfword(ctx));
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);

    RemoveObjectEventByLocalIdAndMap(objectId, mapNum, mapGroup);
    return FALSE;
}

bool32 ScrCmd_addobject(struct ScriptContext * ctx)
{
    u32 objectId = VarGet(ScriptReadHalfword(ctx));

    TrySpawnObjectEvent(objectId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    return FALSE;
}

bool32 ScrCmd_addobject_at(struct ScriptContext * ctx)
{
    u32 objectId = VarGet(ScriptReadHalfword(ctx));
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);

    TrySpawnObjectEvent(objectId, mapNum, mapGroup);
    return FALSE;
}

bool32 ScrCmd_setobjectxy(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    TryMoveObjectEventToMapCoords(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, x, y);
    return FALSE;
}

bool32 ScrCmd_setobjectxyperm(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    Overworld_SetMapObjTemplateCoords(localId, x, y);
    return FALSE;
}

bool32 ScrCmd_moveobjectoffscreen(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));

    TryOverrideObjectEventTemplateCoords(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    return FALSE;
}

bool32 ScrCmd_showobject_at(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);

    ShowOrHideObjectByLocalIdAndMap(localId, mapNum, mapGroup, FALSE);
    return FALSE;
}

bool32 ScrCmd_hideobject_at(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);

    ShowOrHideObjectByLocalIdAndMap(localId, mapNum, mapGroup, TRUE);
    return FALSE;
}

bool32 ScrCmd_setobjectpriority(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);
    u32 priority = ScriptReadByte(ctx);

    SetObjectPriorityByLocalIdAndMap(localId, mapNum, mapGroup, priority + 83);
    return FALSE;
}

bool32 ScrCmd_resetobjectpriority(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 mapGroup = ScriptReadByte(ctx);
    u32 mapNum = ScriptReadByte(ctx);

    UnfixObjectPriorityByLocalIdAndMap(localId, mapNum, mapGroup);
    return FALSE;
}

bool32 ScrCmd_faceplayer(struct ScriptContext * ctx)
{
    if (gObjectEvents[gSelectedObjectEvent].active)
        ObjectEventFaceOppositeDirection(&gObjectEvents[gSelectedObjectEvent], GetPlayerFacingDirection());

    return FALSE;
}

bool32 ScrCmd_turnobject(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 direction = ScriptReadByte(ctx);

    ObjectEventTurnByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, direction);
    return FALSE;
}

bool32 ScrCmd_setobjectmovementtype(struct ScriptContext * ctx)
{
    u32 localId = VarGet(ScriptReadHalfword(ctx));
    u32 movementType = ScriptReadByte(ctx);

    Overworld_SetObjEventTemplateMovementType(localId, movementType);
    return FALSE;
}

bool32 ScrCmd_createvobject(struct ScriptContext * ctx)
{
    u32 graphicsId = ScriptReadByte(ctx);
    u32 v2 = ScriptReadByte(ctx);
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));
    u32 elevation = ScriptReadByte(ctx);
    u32 direction = ScriptReadByte(ctx);

    sprite_new(graphicsId, v2, x, y, elevation, direction);
    return FALSE;
}

bool32 ScrCmd_turnvobject(struct ScriptContext * ctx)
{
    u32 v1 = ScriptReadByte(ctx);
    u32 direction = ScriptReadByte(ctx);

    TurnObjectEvent(v1, direction);
    return FALSE;
}

bool32 ScrCmd_lockall(struct ScriptContext * ctx)
{
    if (IsUpdateLinkStateCBActive())
        return FALSE;
    else
    {
        ScriptFreezeObjectEvents();
        SetupNativeScript(ctx, NativeScript_WaitPlayerStopMoving);
        return TRUE;
    }
}

bool32 ScrCmd_lock(struct ScriptContext * ctx)
{
    if (IsUpdateLinkStateCBActive())
        return FALSE;
    else
    {
        if (gObjectEvents[gSelectedObjectEvent].active)
        {
            LockSelectedObjectEvent();
            SetupNativeScript(ctx, NativeScript_WaitPlayerAndTargetNPCStopMoving);
        }
        else
        {
            ScriptFreezeObjectEvents();
            SetupNativeScript(ctx, NativeScript_WaitPlayerStopMoving);
        }
        return TRUE;
    }
}

bool32 ScrCmd_releaseall(struct ScriptContext * ctx)
{
    HideFieldMessageBox();
    ObjectEventClearHeldMovementIfFinished(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(OBJ_EVENT_ID_PLAYER, 0, 0)]);
    ScriptMovement_UnfreezeObjectEvents();
    UnfreezeObjectEvents();
    return FALSE;
}

bool32 ScrCmd_release(struct ScriptContext * ctx)
{
    HideFieldMessageBox();
    
    if (gObjectEvents[gSelectedObjectEvent].active)
        ObjectEventClearHeldMovementIfFinished(&gObjectEvents[gSelectedObjectEvent]);

    ObjectEventClearHeldMovementIfFinished(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(OBJ_EVENT_ID_PLAYER, 0, 0)]);
    ScriptMovement_UnfreezeObjectEvents();
    UnfreezeObjectEvents();
    return FALSE;
}

bool32 ScrCmd_textcolor(struct ScriptContext * ctx)
{
    gSpecialVar_PrevTextColor = gSpecialVar_TextColor;
    gSpecialVar_TextColor = ScriptReadByte(ctx);
    return FALSE;
}

bool32 ScrCmd_message(struct ScriptContext * ctx)
{
    const u8 *msg = (const u8 *)ScriptReadWord(ctx);

    if (msg == NULL)
        msg = (const u8 *)ctx->data[0];
    ShowFieldMessage(msg);
    return FALSE;
}

bool32 ScrCmd_messageautoscroll(struct ScriptContext * ctx)
{
    const u8 *msg = (const u8 *)ScriptReadWord(ctx);

    if (msg == NULL)
        msg = (const u8 *)ctx->data[0];
    ShowFieldAutoScrollMessage(msg);
    return FALSE;
}

bool32 ScrCmd_waitmessage(struct ScriptContext * ctx)
{
    SetupNativeScript(ctx, IsFieldMessageBoxHidden);
    return TRUE;
}

bool32 ScrCmd_closemessage(struct ScriptContext * ctx)
{
    HideFieldMessageBox();
    return FALSE;
}

static bool32 WaitForAorBPress(void)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
        return TRUE;
    return FALSE;
} 

bool32 ScrCmd_waitbuttonpress(struct ScriptContext * ctx)
{
    SetupNativeScript(ctx, WaitForAorBPress);
    return TRUE;
}

bool32 ScrCmd_yesnobox(struct ScriptContext * ctx)
{
    if (ScriptMenu_YesNo())
    {
        ScriptContext1_Stop();
        return TRUE;
    }
    else
        return FALSE;
}

bool32 ScrCmd_multichoice(struct ScriptContext * ctx)
{
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);
    u32 multichoiceId = ScriptReadByte(ctx);
    bool32 ignoreBPress = ScriptReadByte(ctx);
    u32 defaultOpt = ScriptReadByte(ctx);
    u32 perRowItems = ScriptReadByte(ctx);

    if (ScriptMenu_Multichoice(x, y, multichoiceId, ignoreBPress, defaultOpt, perRowItems))
    {
        ScriptContext1_Stop();
        return TRUE;
    }
    return FALSE;
}

bool32 ScrCmd_showitempic(struct ScriptContext * ctx)
{
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);

    ScriptMenu_ShowItemPic(itemId, x, y);
    return FALSE;
}

bool32 ScrCmd_drawbox(struct ScriptContext * ctx)
{
    /*u8 left = ScriptReadByte(ctx);
    u8 top = ScriptReadByte(ctx);
    u8 right = ScriptReadByte(ctx);
    u8 bottom = ScriptReadByte(ctx);

    MenuDrawTextWindow(left, top, right, bottom);*/
    return FALSE;
}

bool32 ScrCmd_erasebox(struct ScriptContext * ctx)
{
    u32 left = ScriptReadByte(ctx);
    u32 top = ScriptReadByte(ctx);
    u32 right = ScriptReadByte(ctx);
    u32 bottom = ScriptReadByte(ctx);

    // MenuZeroFillWindowRect(left, top, right, bottom);
    return FALSE;
}

bool32 ScrCmd_drawboxtext(struct ScriptContext * ctx)
{
//    u8 left = ScriptReadByte(ctx);
//    u8 top = ScriptReadByte(ctx);
//    u8 multichoiceId = ScriptReadByte(ctx);
//    u8 ignoreBPress = ScriptReadByte(ctx);

    /*if (Multichoice(left, top, multichoiceId, ignoreBPress) == TRUE)
    {
        ScriptContext1_Stop();
        return TRUE;
    }*/
    return FALSE;
}

bool32 ScrCmd_showmonpic(struct ScriptContext * ctx)
{
    u32 species = VarGet(ScriptReadHalfword(ctx));
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);

    ScriptMenu_ShowPokemonPic(species, x, y);
    PlayCry_Normal(species, 0);
    return FALSE;
}

bool32 ScrCmd_hidemonpic(struct ScriptContext * ctx)
{
    bool32 (*func)(void) = ScriptMenu_GetPicboxWaitFunc();

    if (func == NULL)
        return FALSE;
    SetupNativeScript(ctx, func);
    return TRUE;
}

bool32 ScrCmd_showcontestwinner(struct ScriptContext * ctx)
{
    u32 v1 = ScriptReadByte(ctx);

    /*
    if (v1)
        sub_812FDA8(v1);
    ShowContestWinner();
    ScriptContext1_Stop();
    return TRUE;
     */

    return FALSE;
}

bool32 ScrCmd_braillemessage(struct ScriptContext * ctx)
{
    u8 *ptr = (u8 *)ScriptReadWord(ctx);
    if (ptr == NULL)
        ptr = (u8 *)ctx->data[0];

    LoadStdWindowFrameGfx();
    DrawDialogueFrame(0, 1);
    AddTextPrinterParameterized(0, 6, ptr, 0, 1, 0, NULL);
    return FALSE;
}

bool32 ScrCmd_getbraillestringwidth(struct ScriptContext * ctx)
{
    u8 *ptr = (u8 *)ScriptReadWord(ctx);
    if (ptr == NULL)
        ptr = (u8 *)ctx->data[0];

    gSpecialVar_0x8004 = GetStringWidth(6, ptr, -1);
    return FALSE;
}

bool32 ScrCmd_vmessage(struct ScriptContext * ctx)
{
    u32 v1 = ScriptReadWord(ctx);

    ShowFieldMessage((u8 *)(v1 - gVScriptOffset));
    return FALSE;
}

u8 *const sScriptStringVars[] =
{
    gStringVar1,
    gStringVar2,
    gStringVar3,
};

bool32 ScrCmd_bufferspeciesname(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 species = VarGet(ScriptReadHalfword(ctx));

    StringCopy(sScriptStringVars[stringVarIndex], gSpeciesInfo[species].name);
    return FALSE;
}

bool32 ScrCmd_bufferleadmonspeciesname(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u8 *dest = sScriptStringVars[stringVarIndex];
    u32 species = GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SPECIES, NULL);
    StringCopy(dest, gSpeciesInfo[species].name);
    return FALSE;
}

bool32 ScrCmd_bufferpartymonnick(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 partyIndex = VarGet(ScriptReadHalfword(ctx));

    GetMonData(&gPlayerParty[partyIndex], MON_DATA_NICKNAME, sScriptStringVars[stringVarIndex]);
    StringGet_Nickname(sScriptStringVars[stringVarIndex]);
    return FALSE;
}

bool32 ScrCmd_bufferitemname(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 itemId = VarGet(ScriptReadHalfword(ctx));

    CopyItemName(itemId, sScriptStringVars[stringVarIndex]);
    return FALSE;
}

bool32 ScrCmd_bufferitemnameplural(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 itemId = VarGet(ScriptReadHalfword(ctx));
    u32 quantity = VarGet(ScriptReadHalfword(ctx));

    CopyItemNameHandlePlural(itemId, quantity, sScriptStringVars[stringVarIndex]);
    
    return FALSE;
}

bool32 ScrCmd_bufferdecorationname(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 decorId = VarGet(ScriptReadHalfword(ctx));

    return FALSE;
}

bool32 ScrCmd_buffermovename(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 moveId = VarGet(ScriptReadHalfword(ctx));

    StringCopy(sScriptStringVars[stringVarIndex], gBattleMoves[moveId].name);
    return FALSE;
}

bool32 ScrCmd_buffernumberstring(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 v1 = VarGet(ScriptReadHalfword(ctx));

    ConvertIntToDecimalStringN(sScriptStringVars[stringVarIndex], v1, 0, CountDigits(v1));
    return FALSE;
}

bool32 ScrCmd_bufferstdstring(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 index = VarGet(ScriptReadHalfword(ctx));

    StringCopy(sScriptStringVars[stringVarIndex], gStdStringPtrs[index]);
    return FALSE;
}

/*
bool32 ScrCmd_buffercontesttype(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 index = VarGet(ScriptReadHalfword(ctx));

    sub_818E868(sScriptStringVars[stringVarIndex], index);
    return FALSE;
}
*/

bool32 ScrCmd_bufferstring(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    const u8 *text = (u8 *)ScriptReadWord(ctx);

    StringCopy(sScriptStringVars[stringVarIndex], text);
    return FALSE;
}

bool32 ScrCmd_vloadword(struct ScriptContext * ctx)
{
    const u8 *ptr = (u8 *)(ScriptReadWord(ctx) - gVScriptOffset);

    StringExpandPlaceholders(gStringVar4, ptr);
    return FALSE;
}

bool32 ScrCmd_vbufferstring(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 addr = ScriptReadWord(ctx);
    const u8 *src = (u8 *)(addr - gVScriptOffset);
    u8 *dest = sScriptStringVars[stringVarIndex];
    StringCopy(dest, src);
    return FALSE;
}

bool32 ScrCmd_bufferboxname(struct ScriptContext * ctx)
{
    u32 stringVarIndex = ScriptReadByte(ctx);
    u32 boxId = VarGet(ScriptReadHalfword(ctx));

    StringCopy(sScriptStringVars[stringVarIndex], GetBoxNamePtr(boxId));
    return FALSE;
}

bool32 ScrCmd_givemon(struct ScriptContext * ctx)
{
    u32 species = VarGet(ScriptReadHalfword(ctx));
    u32 level = ScriptReadByte(ctx);
    u32 ball, item = VarGet(ScriptReadHalfword(ctx));
    u32 i, nature, gender;
    u8 ivs[NUM_STATS] = {0};
    bool32 isShiny, hiddenAbility;
    u16 moves[MAX_MON_MOVES];
    
    for (i = 0; i < MAX_MON_MOVES; i++)
        moves[i] = MOVE_NONE;
    
    for (i = 0; i < NUM_STATS; i++)
        ivs[i] = ScriptReadByte(ctx);
    
    isShiny = ScriptReadByte(ctx);
    
    hiddenAbility = ScriptReadByte(ctx);
    
    ball = ScriptReadHalfword(ctx);
    
    nature = ScriptReadByte(ctx);
    
    gender = ScriptReadByte(ctx);
    
    gSpecialVar_Result = ScriptGiveMon(species, level, item, ivs, ball, isShiny ? GENERATE_SHINY_FORCED : GENERATE_SHINY_NORMAL, hiddenAbility, nature, gender, moves);
    return FALSE;
}

bool32 ScrCmd_giveegg(struct ScriptContext * ctx)
{
    u32 i, species = VarGet(ScriptReadHalfword(ctx));
    u8 ivs[NUM_STATS] = {0};
    bool32 isShiny, hiddenAbility;
    u32 nature;
    u16 moves[MAX_MON_MOVES];
    
    for (i = 0; i < MAX_MON_MOVES; i++)
        moves[i] = MOVE_NONE;
    
    for (i = 0; i < NUM_STATS; i++)
        ivs[i] = ScriptReadByte(ctx);
    
    isShiny = ScriptReadByte(ctx);
    
    hiddenAbility = ScriptReadByte(ctx);
    
    nature = ScriptReadByte(ctx);
    
    gSpecialVar_Result = ScriptGiveEgg(species, ivs, isShiny ? GENERATE_SHINY_FORCED : GENERATE_SHINY_NORMAL, hiddenAbility, nature, moves);
    return FALSE;
}

bool32 ScrCmd_setmonmove(struct ScriptContext * ctx)
{
    u32 partyIndex = ScriptReadByte(ctx);
    u32 slot = ScriptReadByte(ctx);
    u32 move = ScriptReadHalfword(ctx);

    ScriptSetMonMoveSlot(partyIndex, move, slot);
    return FALSE;
}

bool32 ScrCmd_checkpartymove(struct ScriptContext * ctx)
{
    u32 i, moveId = ScriptReadHalfword(ctx);

    gSpecialVar_Result = PARTY_SIZE;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u32 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL);
        if (!species)
            break;
        
        if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG) && FindMoveSlotInMoveset(&gPlayerParty[i], moveId) != MAX_MON_MOVES)
        {
            gSpecialVar_Result = i;
            gSpecialVar_0x8004 = species;
            break;
        }
    }
    return FALSE;
}

bool32 ScrCmd_addmoney(struct ScriptContext * ctx)
{
    u32 amount = ScriptReadWord(ctx);
    u32 ignore = ScriptReadByte(ctx);

    if (!ignore)
        AddMoney(&gSaveBlock1Ptr->money, amount);
    
    return FALSE;
}

bool32 ScrCmd_removemoney(struct ScriptContext * ctx)
{
    u32 amount = ScriptReadWord(ctx);
    u32 ignore = ScriptReadByte(ctx);

    if (!ignore)
        RemoveMoney(&gSaveBlock1Ptr->money, amount);
    
    return FALSE;
}

bool32 ScrCmd_checkmoney(struct ScriptContext * ctx)
{
    u32 amount = ScriptReadWord(ctx);
    u32 ignore = ScriptReadByte(ctx);

    if (!ignore)
        gSpecialVar_Result = IsEnoughMoney(&gSaveBlock1Ptr->money, amount);
    
    return FALSE;
}

bool32 ScrCmd_showmoneybox(struct ScriptContext * ctx)
{
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);
    u32 ignore = ScriptReadByte(ctx);
    
    DrawMoneyBox(GetMoney(&gSaveBlock1Ptr->money), x, y);
    return FALSE;
}

bool32 ScrCmd_hidemoneybox(struct ScriptContext * ctx)
{
    /*u8 x = ScriptReadByte(ctx);
    u8 y = ScriptReadByte(ctx);*/

    HideMoneyBox();
    return FALSE;
}

bool32 ScrCmd_updatemoneybox(struct ScriptContext * ctx)
{
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);
    u32 ignore = ScriptReadByte(ctx);

    if (!ignore)
        ChangeAmountInMoneyBox(GetMoney(&gSaveBlock1Ptr->money));
    
    return FALSE;
}

bool32 ScrCmd_showcoinsbox(struct ScriptContext * ctx)
{
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);
    
    ShowCoinsWindow(GetCoins(), x, y);
    return FALSE;
}

bool32 ScrCmd_hidecoinsbox(struct ScriptContext * ctx)
{
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);

    HideCoinsWindow();
    return FALSE;
}

bool32 ScrCmd_updatecoinsbox(struct ScriptContext * ctx)
{
    u32 x = ScriptReadByte(ctx);
    u32 y = ScriptReadByte(ctx);

    PrintCoinsString(GetCoins());
    return FALSE;
}

bool32 ScrCmd_trainerbattle(struct ScriptContext * ctx)
{
    ctx->scriptPtr = BattleSetup_ConfigureTrainerBattle(ctx->scriptPtr);
    return FALSE;
}

bool32 ScrCmd_dotrainerbattle(struct ScriptContext * ctx)
{
    StartTrainerBattle();
    return TRUE;
}

bool32 ScrCmd_gotopostbattlescript(struct ScriptContext * ctx)
{
    ctx->scriptPtr = BattleSetup_GetScriptAddrAfterBattle();
    return FALSE;
}

bool32 ScrCmd_gotobeatenscript(struct ScriptContext * ctx)
{
    ctx->scriptPtr = BattleSetup_GetTrainerPostBattleScript();
    return FALSE;
}

bool32 ScrCmd_checktrainerflag(struct ScriptContext * ctx)
{
    u32 index = VarGet(ScriptReadHalfword(ctx));

    ctx->comparisonResult = HasTrainerBeenFought(index);
    return FALSE;
}

bool32 ScrCmd_settrainerflag(struct ScriptContext * ctx)
{
    u32 index = VarGet(ScriptReadHalfword(ctx));

    SetTrainerFlag(index);
    return FALSE;
}

bool32 ScrCmd_cleartrainerflag(struct ScriptContext * ctx)
{
    u32 index = VarGet(ScriptReadHalfword(ctx));

    ClearTrainerFlag(index);
    return FALSE;
}

bool32 ScrCmd_setwildbattle(struct ScriptContext * ctx)
{
    u32 species = ScriptReadHalfword(ctx);
    u32 level = ScriptReadByte(ctx);
    u32 item = ScriptReadHalfword(ctx);
    u32 species2 = ScriptReadHalfword(ctx);
    u32 level2 = ScriptReadByte(ctx);
    u32 item2 = ScriptReadHalfword(ctx);

    CreateScriptedWildMon(species, level, item, species2, level2, item2);
    return FALSE;
}

bool32 ScrCmd_dowildbattle(struct ScriptContext * ctx)
{
    StartScriptedWildBattle();
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_pokemart(struct ScriptContext * ctx)
{
    const void *ptr = (void *)ScriptReadWord(ctx);

    CreatePokemartMenu(ptr);
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_pokemartdecoration(struct ScriptContext * ctx)
{
    const void *ptr = (void *)ScriptReadWord(ctx);

    CreateDecorationShop1Menu(ptr);
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_pokemartdecoration2(struct ScriptContext * ctx)
{
    const void *ptr = (void *)ScriptReadWord(ctx);

    CreateDecorationShop2Menu(ptr);
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_playslotmachine(struct ScriptContext * ctx)
{
    u32 slotMachineIndex = VarGet(ScriptReadHalfword(ctx));

    PlaySlotMachine(slotMachineIndex, CB2_ReturnToFieldContinueScriptPlayMapMusic);
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_setberrytree(struct ScriptContext * ctx)
{
//    u8 treeId = ScriptReadByte(ctx);
//    u8 berry = ScriptReadByte(ctx);
//    u8 growthStage = ScriptReadByte(ctx);
//
//    if (berry == 0)
//        PlantBerryTree(treeId, 0, growthStage, FALSE);
//    else
//        PlantBerryTree(treeId, berry, growthStage, FALSE);
    return FALSE;
}

bool32 ScrCmd_getpricereduction(struct ScriptContext * ctx)
{
//    u16 value = VarGet(ScriptReadHalfword(ctx));
//
//    gSpecialVar_Result = GetPriceReduction(value);
    return FALSE;
}

bool32 ScrCmd_choosecontestmon(struct ScriptContext * ctx)
{
//    sub_81B9404();
    ScriptContext1_Stop();
    return TRUE;
}


bool32 ScrCmd_startcontest(struct ScriptContext * ctx)
{
//    sub_80F840C();
//    ScriptContext1_Stop();
//    return TRUE;
    return FALSE;
}

bool32 ScrCmd_showcontestresults(struct ScriptContext * ctx)
{
//    sub_80F8484();
//    ScriptContext1_Stop();
//    return TRUE;
    return FALSE;
}

bool32 ScrCmd_contestlinktransfer(struct ScriptContext * ctx)
{
//    sub_80F84C4(gSpecialVar_ContestCategory);
//    ScriptContext1_Stop();
//    return TRUE;
    return FALSE;
}

bool32 ScrCmd_dofieldeffect(struct ScriptContext * ctx)
{
    u32 effectId = VarGet(ScriptReadHalfword(ctx));

    sFieldEffectScriptId = effectId;
    FieldEffectStart(sFieldEffectScriptId);
    return FALSE;
}

bool32 ScrCmd_setfieldeffectarg(struct ScriptContext * ctx)
{
    u32 argNum = ScriptReadByte(ctx);

    gFieldEffectArguments[argNum] = (s16)VarGet(ScriptReadHalfword(ctx));
    return FALSE;
}

static bool32 WaitForFieldEffectFinish(void)
{
    if (!FieldEffectActiveListContains(sFieldEffectScriptId))
        return TRUE;
    else
        return FALSE;
}

bool32 ScrCmd_waitfieldeffect(struct ScriptContext * ctx)
{
    sFieldEffectScriptId = VarGet(ScriptReadHalfword(ctx));
    SetupNativeScript(ctx, WaitForFieldEffectFinish);
    return TRUE;
}

bool32 ScrCmd_setrespawn(struct ScriptContext * ctx)
{
    u32 healLocationId = VarGet(ScriptReadHalfword(ctx));

    SetLastHealLocationWarp(healLocationId);
    return FALSE;
}

bool32 ScrCmd_checkplayergender(struct ScriptContext * ctx)
{
    gSpecialVar_Result = gSaveBlock2Ptr->playerGender;
    return FALSE;
}

bool32 ScrCmd_playmoncry(struct ScriptContext * ctx)
{
    u32 species = VarGet(ScriptReadHalfword(ctx));
    u32 mode = VarGet(ScriptReadHalfword(ctx));
    PlayCry_ByMode(species, 0, mode);
    return FALSE;
}

bool32 ScrCmd_waitmoncry(struct ScriptContext * ctx)
{
    SetupNativeScript(ctx, IsCryFinished);
    return TRUE;
}

bool32 ScrCmd_setmetatile(struct ScriptContext * ctx)
{
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));
    u32 tileId = VarGet(ScriptReadHalfword(ctx));
    u32 v8 = VarGet(ScriptReadHalfword(ctx));

    x += 7;
    y += 7;
    
    if (!v8)
        MapGridSetMetatileIdAt(x, y, tileId);
    else
        MapGridSetMetatileIdAt(x, y, tileId | METATILE_COLLISION_MASK);
    
    return FALSE;
}

bool32 ScrCmd_opendoor(struct ScriptContext * ctx)
{
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    x += 7;
    y += 7;
    
    PlaySE(GetDoorSoundEffect(x, y));
    FieldAnimateDoorOpen(x, y);
    return FALSE;
}

bool32 ScrCmd_closedoor(struct ScriptContext * ctx)
{
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    x += 7;
    y += 7;
    
    FieldAnimateDoorClose(x, y);
    return FALSE;
}

static bool32 IsDoorAnimationStopped(void)
{
    if (!FieldIsDoorAnimationRunning())
        return TRUE;
    else
        return FALSE;
}

bool32 ScrCmd_waitdooranim(struct ScriptContext * ctx)
{
    SetupNativeScript(ctx, IsDoorAnimationStopped);
    return TRUE;
}

bool32 ScrCmd_setdooropen(struct ScriptContext * ctx)
{
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    x += 7;
    y += 7;
    FieldSetDoorOpened(x, y);
    return FALSE;
}

bool32 ScrCmd_setdoorclosed(struct ScriptContext * ctx)
{
    u32 x = VarGet(ScriptReadHalfword(ctx));
    u32 y = VarGet(ScriptReadHalfword(ctx));

    x += 7;
    y += 7;
    FieldSetDoorClosed(x, y);
    return FALSE;
}

bool32 ScrCmd_addelevmenuitem(struct ScriptContext * ctx)
{
//    u8 v3 = ScriptReadByte(ctx);
//    u16 v5 = VarGet(ScriptReadHalfword(ctx));
//    u16 v7 = VarGet(ScriptReadHalfword(ctx));
//    u16 v9 = VarGet(ScriptReadHalfword(ctx));

    //ScriptAddElevatorMenuItem(v3, v5, v7, v9);
    return FALSE;
}

bool32 ScrCmd_showelevmenu(struct ScriptContext * ctx)
{
    /*ScriptShowElevatorMenu();
    ScriptContext1_Stop();
    return TRUE;*/
    return FALSE;
}

bool32 ScrCmd_checkcoins(struct ScriptContext * ctx)
{
    u16 *ptr = GetVarPointer(ScriptReadHalfword(ctx));
    *ptr = GetCoins();
    return FALSE;
}

bool32 ScrCmd_addcoins(struct ScriptContext * ctx)
{
    u32 coins = VarGet(ScriptReadHalfword(ctx));

    if (AddCoins(coins))
        gSpecialVar_Result = 0;
    else
        gSpecialVar_Result = 1;
    return FALSE;
}

bool32 ScrCmd_removecoins(struct ScriptContext * ctx)
{
    u32 coins = VarGet(ScriptReadHalfword(ctx));

    if (RemoveCoins(coins))
        gSpecialVar_Result = 0;
    else
        gSpecialVar_Result = 1;
    return FALSE;
}

bool32 ScrCmd_signmsg(struct ScriptContext * ctx)
{
    MsgSetSignPost();
    return FALSE;
}

bool32 ScrCmd_normalmsg(struct ScriptContext * ctx)
{
    MsgSetNotSignPost();
    return FALSE;
}
