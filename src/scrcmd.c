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
#include "rtc.h"
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

#define CMD_ARGS(...) \
    const struct __attribute__((packed)) { RECURSIVELY(R_FOR_EACH(APPEND_SEMICOLON, __VA_ARGS__)) const u8 nextInstr[0]; } *const cmd UNUSED = (const void *)ctx->scriptPtr; \
    const u8 *const __cmd_next UNUSED = (ctx->scriptPtr = cmd->nextInstr)

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
    CMD_ARGS();
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

bool32 ScrCmd_startplayerfinditemanim(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
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

bool32 ScrCmd_endplayerfinditemanim(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
    gTasks[CreateTask(Task_EndPlayerFindItemAnim, 80)].data[0] = 0;
    SetupNativeScript(ctx, EndPlayerFindItemAnim);

    return TRUE;
}

bool32 ScrCmd_end(struct ScriptContext * ctx)
{
    CMD_ARGS();
    StopScript(ctx);
    return FALSE;
}

bool32 ScrCmd_gotonative(struct ScriptContext * ctx)
{
    CMD_ARGS(bool32 (*func)(void));
    SetupNativeScript(ctx, cmd->func);
    return TRUE;
}

bool32 ScrCmd_special(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 specialId);
    
    u16 (*const *specialPtr)(void) = &gSpecials[cmd->specialId];
    
    if (specialPtr < gSpecialsEnd)
        (*specialPtr)();
    else
        AGB_ASSERT_EX(0, ABSPATH("scrcmd.c"), 241);

    return FALSE;
}

bool32 ScrCmd_specialvar(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 outputVar, u16 specialId);
    
    u16 (*const *specialPtr)(void) = &gSpecials[cmd->specialId];
    
    if (specialPtr < gSpecialsEnd)
        *GetVarPointer(cmd->outputVar) = (*specialPtr)();
    else
        AGB_ASSERT_EX(0, ABSPATH("scrcmd.c"), 263);

    return FALSE;
}

bool32 ScrCmd_callnative(struct ScriptContext * ctx)
{
    CMD_ARGS(void (*func)(void));
    void (*func)(void) = cmd->func;
    func();
    return FALSE;
}

bool32 ScrCmd_waitstate(struct ScriptContext * ctx)
{
    CMD_ARGS();
    ScriptContext1_Stop();
    return TRUE;
}

bool32 ScrCmd_goto(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *ptr);
    ScriptJump(ctx, cmd->ptr);
    return FALSE;
}

bool32 ScrCmd_return(struct ScriptContext * ctx)
{
    CMD_ARGS();
    ScriptReturn(ctx);
    return FALSE;
}

bool32 ScrCmd_call(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *ptr);
    ScriptCall(ctx, cmd->ptr);
    return FALSE;
}

bool32 ScrCmd_goto_if(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 condition, const u8 *ptr);

    if (sScriptConditionTable[cmd->condition][ctx->comparisonResult] == 1)
        ScriptJump(ctx, cmd->ptr);
    
    return FALSE;
}

bool32 ScrCmd_call_if(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 condition, const u8 *ptr);

    if (sScriptConditionTable[cmd->condition][ctx->comparisonResult] == 1)
        ScriptCall(ctx, cmd->ptr);
    
    return FALSE;
}

bool32 ScrCmd_setvaddress(struct ScriptContext * ctx)
{
    u32 addr1 = (u32)ctx->scriptPtr - 1;
    CMD_ARGS(u32 address);
    gVScriptOffset = cmd->address - addr1;
    return FALSE;
}

bool32 ScrCmd_vgoto(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *ptr);
    ScriptJump(ctx, cmd->ptr - gVScriptOffset);
    return FALSE;
}

bool32 ScrCmd_vcall(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *ptr);
    ScriptCall(ctx, cmd->ptr - gVScriptOffset);
    return FALSE;
}

bool32 ScrCmd_vgoto_if(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 condition, const u8 *ptr);
    
    if (sScriptConditionTable[cmd->condition][ctx->comparisonResult] == 1)
        ScriptJump(ctx, cmd->ptr - gVScriptOffset);
    
    return FALSE;
}

bool32 ScrCmd_vcall_if(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 condition, const u8 *ptr);

    if (sScriptConditionTable[cmd->condition][ctx->comparisonResult] == 1)
        ScriptCall(ctx, cmd->ptr - gVScriptOffset);
    
    return FALSE;
}

bool32 ScrCmd_gotostd(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 stdFuncId);

    const u8 *const * script = &gStdScripts[cmd->stdFuncId];
    
    if (script < gStdScriptsEnd)
        ScriptJump(ctx, *script);
    
    return FALSE;
}

bool32 ScrCmd_callstd(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 stdFuncId);
    
    const u8 *const * script = &gStdScripts[cmd->stdFuncId];
    
    if (script < gStdScriptsEnd)
        ScriptCall(ctx, *script);
    
    return FALSE;
}

bool32 ScrCmd_gotostd_if(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 condition, u8 stdFuncId);
    
    if (sScriptConditionTable[cmd->condition][ctx->comparisonResult] == 1)
    {
        const u8 *const * script = &gStdScripts[cmd->stdFuncId];
        if (script < gStdScriptsEnd)
            ScriptJump(ctx, *script);
    }
    return FALSE;
}

bool32 ScrCmd_callstd_if(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 condition, u8 stdFuncId);

    if (sScriptConditionTable[cmd->condition][ctx->comparisonResult] == 1)
    {
        const u8 *const * script = &gStdScripts[cmd->stdFuncId];
        if (script < gStdScriptsEnd)
            ScriptCall(ctx, *script);
    }
    return FALSE;
}

bool32 ScrCmd_jumpifcantgivenick(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *ptr);

    if (gSaveBlock2Ptr->optionsSkipPkmnNickname)
        ScriptJump(ctx, cmd->ptr);
    
    return FALSE;
}

bool32 ScrCmd_loadword(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 destination, u32 value);
    ctx->data[cmd->destination] = cmd->value;
    return FALSE;
}

bool32 ScrCmd_loadbytefromaddr(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 destination, const u8 *source);
    ctx->data[cmd->destination] = *cmd->source;
    return FALSE;
}

bool32 ScrCmd_writebytetoaddr(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 value, u8 *offset);
    *cmd->offset = cmd->value;
    return FALSE;
}

bool32 ScrCmd_loadbyte(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 destination, u8 value);
    ctx->data[cmd->destination] = cmd->value;
    return FALSE;
}

bool32 ScrCmd_setptrbyte(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 source, u8 *destination);
    *cmd->destination = ctx->data[cmd->source];
    return FALSE;
}

bool32 ScrCmd_copylocal(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 destination, u8 source);
    ctx->data[cmd->destination] = ctx->data[cmd->source];
    return FALSE;
}

bool32 ScrCmd_copybyte(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 *dest, const u8 *source);
    *cmd->dest = *cmd->source;
    return FALSE;
}

bool32 ScrCmd_setvar(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 var, u16 value);
    *GetVarPointer(cmd->var) = cmd->value;
    return FALSE;
}

bool32 ScrCmd_copyvar(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 destVar, u16 srcVar);
    *GetVarPointer(cmd->destVar) = *GetVarPointer(cmd->srcVar);
    return FALSE;
}

bool32 ScrCmd_setorcopyvar(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 destVar, u16 var);
    *GetVarPointer(cmd->destVar) = VarGet(cmd->var);
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
    CMD_ARGS(u8 byte1, u8 byte2);
    
    const u8 value1 = ctx->data[cmd->byte1];
    const u8 value2 = ctx->data[cmd->byte2];
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

// comparelocaltoimm
bool32 ScrCmd_compare_local_to_value(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 byte, u8 value);
    
    const u8 value1 = ctx->data[cmd->byte];
    const u8 value2 = cmd->value;
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

bool32 ScrCmd_compare_local_to_addr(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 byte, const u8 *pointer);
    
    const u8 value1 = ctx->data[cmd->byte];
    const u8 value2 = *cmd->pointer;
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

bool32 ScrCmd_compare_addr_to_local(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *pointer, u8 byte);
    
    const u8 value1 = *cmd->pointer;
    const u8 value2 = ctx->data[cmd->byte];
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

bool32 ScrCmd_compare_addr_to_value(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *pointer, u8 value);
    
    const u8 value1 = *cmd->pointer;
    const u8 value2 = cmd->value;
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

bool32 ScrCmd_compare_addr_to_addr(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *pointer1, const u8 *pointer2);
    
    const u8 value1 = *cmd->pointer1;
    const u8 value2 = *cmd->pointer2;
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

bool32 ScrCmd_compare_var_to_value(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 var, u16 value);
    
    const u16 value1 = *GetVarPointer(cmd->var);
    const u16 value2 = cmd->value;
    ctx->comparisonResult = compare_012(value1, value2);
    
    return FALSE;
}

bool32 ScrCmd_compare_var_to_var(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 var1, u16 var2);
    
    const u16 *ptr1 = GetVarPointer(cmd->var1);
    const u16 *ptr2 = GetVarPointer(cmd->var2);
    ctx->comparisonResult = compare_012(*ptr1, *ptr2);
    
    return FALSE;
}

bool32 ScrCmd_addvar(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 var, u16 value);
    *GetVarPointer(cmd->var) += cmd->value;
    return FALSE;
}

bool32 ScrCmd_subvar(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 var, u16 value);
    *GetVarPointer(cmd->var) -= VarGet(cmd->value);
    return FALSE;
}

bool32 ScrCmd_random(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 max);
    gSpecialVar_Result = Random() % VarGet(cmd->max);
    return FALSE;
}

bool32 ScrCmd_additem(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u16 quantity);
    gSpecialVar_Result = AddBagItem(VarGet(cmd->itemId), (u8)VarGet(cmd->quantity));
    return FALSE;
}

bool32 ScrCmd_removeitem(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u16 quantity);
    gSpecialVar_Result = RemoveBagItem(VarGet(cmd->itemId), (u8)VarGet(cmd->quantity));
    return FALSE;
}

bool32 ScrCmd_checkitemspace(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u16 quantity);
    gSpecialVar_Result = CheckBagHasSpace(VarGet(cmd->itemId), (u8)VarGet(cmd->quantity));
    return FALSE;
}

bool32 ScrCmd_checkitem(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u16 quantity);
    gSpecialVar_Result = CheckBagHasItem(VarGet(cmd->itemId), (u8)VarGet(cmd->quantity));
    return FALSE;
}

bool32 ScrCmd_checkitemtype(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId);
    gSpecialVar_Result = ItemId_GetPocket(VarGet(cmd->itemId));
    return FALSE;
}

bool32 ScrCmd_addpcitem(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u16 quantity);
    gSpecialVar_Result = AddPCItem(VarGet(cmd->itemId), VarGet(cmd->quantity));
    return FALSE;
}

bool32 ScrCmd_checkpcitem(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u16 quantity);
    gSpecialVar_Result = CheckPCHasItem(VarGet(cmd->itemId), VarGet(cmd->quantity));
    return FALSE;
}

bool32 ScrCmd_setflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 id);
    FlagSet(cmd->id);
    return FALSE;
}

bool32 ScrCmd_clearflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 id);
    FlagClear(cmd->id);
    return FALSE;
}

bool32 ScrCmd_checkflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 id);
    ctx->comparisonResult = FlagGet(cmd->id);
    return FALSE;
}

bool32 ScrCmd_incrementgamestat(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 id);
    IncrementGameStat(cmd->id);
    return FALSE;
}

bool32 ScrCmd_comparestattoword(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 gameStatId, u32 value);
    
    u32 statValue = GetGameStat(cmd->gameStatId);

    if (statValue < cmd->value)
        ctx ->comparisonResult = 0;
    else if (statValue == cmd->value)
        ctx->comparisonResult = 1;
    else
        ctx->comparisonResult = 2;
    
    return FALSE;
}

bool32 ScrCmd_setworldmapflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 flagId);
    MapPreview_SetFlag(cmd->flagId);
    return FALSE;
}

bool32 ScrCmd_animateflash(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 level);
    
    AnimateFlash(cmd->level);
    ScriptContext1_Stop();
    
    return TRUE;
}

bool32 ScrCmd_setflashradius(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 level);
    Overworld_SetFlashLevel(VarGet(cmd->level));
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
    CMD_ARGS(u8 mode);
    
    FadeScreen(cmd->mode, 0);
    SetupNativeScript(ctx, IsPaletteNotActive);
    
    return TRUE;
}

bool32 ScrCmd_fadescreenspeed(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 mode, s8 speed);
    
    FadeScreen(cmd->mode, cmd->speed);
    SetupNativeScript(ctx, IsPaletteNotActive);
    
    return TRUE;
}

static bool32 RunPauseTimer(void)
{
    if (--sPauseCounter == 0)
        return TRUE;
    else
        return FALSE;
}

bool32 ScrCmd_delay(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 amount);
    
    sPauseCounter = cmd->amount;
    SetupNativeScript(ctx, RunPauseTimer);
    
    return TRUE;
}

bool32 ScrCmd_starttotembattle(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 buffs[NUM_BATTLE_STATS - 1]);
    
    u32 i;
    s8 buffs[NUM_BATTLE_STATS];
    
    for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
        buffs[i] = cmd->buffs[i - 1];
    
    StartTotemBattle(buffs);
    
    return FALSE;
}

bool32 ScrCmd_dodailyevents(struct ScriptContext * ctx)
{
    CMD_ARGS();
//      DoTimeBasedEvents();
    return FALSE;
}

bool32 ScrCmd_gettime(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
    gSpecialVar_0x8000 = gRtcLocation.hour;
    gSpecialVar_0x8001 = gRtcLocation.minute;
    gSpecialVar_0x8002 = gRtcLocation.second;
    
    return FALSE;
}

bool32 ScrCmd_setweather(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 weatherId);
    SetSav1Weather(VarGet(cmd->weatherId));
    return FALSE;
}

bool32 ScrCmd_resetweather(struct ScriptContext * ctx)
{
    CMD_ARGS();
    SetSav1WeatherFromCurrMapHeader();
    return FALSE;
}

bool32 ScrCmd_doweather(struct ScriptContext * ctx)
{
    CMD_ARGS();
    DoCurrentWeather();
    return FALSE;
}

bool32 ScrCmd_setstepcallback(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 stepCbId);
    ActivatePerStepCallback(cmd->stepCbId);
    return FALSE;
}

bool32 ScrCmd_setmaplayoutindex(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 index);
    SetCurrentMapLayout(VarGet(cmd->index));
    return FALSE;
}

bool32 ScrCmd_warp(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    
    SetWarpDestination(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    DoWarp();
    ResetInitialPlayerAvatarState();
    
    return TRUE;
}

bool32 ScrCmd_warpsilent(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    
    SetWarpDestination(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    DoDiveWarp();
    ResetInitialPlayerAvatarState();
    
    return TRUE;
}

bool32 ScrCmd_warpdoor(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    
    SetWarpDestination(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    DoDoorWarp();
    ResetInitialPlayerAvatarState();
    
    return TRUE;
}

bool32 ScrCmd_warphole(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum);
    
    u16 x, y;

    PlayerGetDestCoords(&x, &y);
    
    if (cmd->mapGroup == -1 && cmd->mapNum == -1)
        SetWarpDestinationToFixedHoleWarp(x - 7, y - 7);
    else
        SetWarpDestination(cmd->mapGroup, cmd->mapNum, -1, x - 7, y - 7);
    
    DoFallWarp();
    ResetInitialPlayerAvatarState();
    
    return TRUE;
}

bool32 ScrCmd_warpteleport(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    
    SetWarpDestination(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    DoTeleportWarp();
    ResetInitialPlayerAvatarState();
    
    return TRUE;
}

bool32 ScrCmd_warpteleport2(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    
    SetWarpDestination(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    SavePlayerFacingDirectionForTeleport(GetPlayerFacingDirection());
    DoTeleport2Warp();
    ResetInitialPlayerAvatarState();
    
    return TRUE;
}

bool32 ScrCmd_setwarp(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    SetWarpDestination(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_setdynamicwarp(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    SetDynamicWarpWithCoords(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_setdivewarp(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    SetFixedDiveWarp(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_setholewarp(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    SetFixedHoleWarp(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_setescapewarp(struct ScriptContext * ctx)
{
    CMD_ARGS(s8 mapGroup, s8 mapNum, s8 warpId, u16 x, u16 y);
    SetEscapeWarp(cmd->mapGroup, cmd->mapNum, cmd->warpId, VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_getplayerxy(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 destXVar, u16 destYVar);

    *GetVarPointer(cmd->destXVar) = gSaveBlock1Ptr->pos.x;
    *GetVarPointer(cmd->destYVar) = gSaveBlock1Ptr->pos.y;
    
    return FALSE;
}

bool32 ScrCmd_getpartysize(struct ScriptContext * ctx)
{
    CMD_ARGS();
    gSpecialVar_Result = CalculatePlayerPartyCount();
    return FALSE;
}

bool32 ScrCmd_playse(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 seId);
    PlaySE(cmd->seId);
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
    CMD_ARGS();
    SetupNativeScript(ctx, WaitForSoundEffectFinish);
    return TRUE;
}

bool32 ScrCmd_playfanfare(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 fanfare);
    PlayFanfare(cmd->fanfare);
    return FALSE;
}

bool32 ScrCmd_waitfanfare(struct ScriptContext * ctx)
{
    CMD_ARGS();
    SetupNativeScript(ctx, IsFanfareTaskInactive);
    return TRUE;
}

bool32 ScrCmd_playbgm(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 songNum, u8 unknown);

    if (cmd->unknown == TRUE)
        Overworld_SetSavedMusic(cmd->songNum);
    
    PlayNewMapMusic(cmd->songNum);
    
    return FALSE;
}

bool32 ScrCmd_savebgm(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 songNum);
    Overworld_SetSavedMusic(cmd->songNum);
    return FALSE;
}

bool32 ScrCmd_fadedefaultbgm(struct ScriptContext * ctx)
{
    CMD_ARGS();
    Overworld_ChangeMusicToDefault();
    return FALSE;
}

bool32 ScrCmd_fadenewbgm(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 songNum);
    Overworld_ChangeMusicTo(cmd->songNum);
    return FALSE;
}

bool32 ScrCmd_fadeoutbgm(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 speed);

    if (cmd->speed != 0)
        FadeOutBGMTemporarily(4 * cmd->speed);
    else
        FadeOutBGMTemporarily(4);
    
    SetupNativeScript(ctx, IsBGMPausedOrStopped);
    
    return TRUE;
}

bool32 ScrCmd_fadeinbgm(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 speed);

    if (cmd->speed != 0)
        FadeInBGM(4 * cmd->speed);
    else
        FadeInBGM(4);
    
    return FALSE;
}

bool32 ScrCmd_applymovement(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, const void *movementScript);
    
    u32 localId = VarGet(cmd->localId);
    
    ScriptMovement_StartObjectMovementScript(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, cmd->movementScript);
    sMovingNpcId = localId;
    
    return FALSE;
}

bool32 ScrCmd_applymovement_at(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, const void *movementScript, s8 mapGroup, s8 mapNum);
    
    u32 localId = VarGet(cmd->localId);

    ScriptMovement_StartObjectMovementScript(localId, cmd->mapNum, cmd->mapGroup, cmd->movementScript);
    sMovingNpcId = localId;
    
    return FALSE;
}

static bool32 WaitForMovementFinish(void)
{
    return ScriptMovement_IsObjectMovementFinished(sMovingNpcId, sMovingNpcMapId, sMovingNpcMapBank);
}

bool32 ScrCmd_waitmovement(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId);
    
    u32 localId = VarGet(cmd->localId);

    if (localId != 0)
        sMovingNpcId = localId;
    
    sMovingNpcMapBank = gSaveBlock1Ptr->location.mapGroup;
    sMovingNpcMapId = gSaveBlock1Ptr->location.mapNum;
    SetupNativeScript(ctx, WaitForMovementFinish);
    
    return TRUE;
}

bool32 ScrCmd_waitmovement_at(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum);
    
    u32 localId = VarGet(cmd->localId);

    if (localId != 0)
        sMovingNpcId = localId;
    
    sMovingNpcMapBank = cmd->mapGroup;
    sMovingNpcMapId = cmd->mapNum;
    SetupNativeScript(ctx, WaitForMovementFinish);
    
    return TRUE;
}

bool32 ScrCmd_removeobject(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId);
    RemoveObjectEventByLocalIdAndMap(VarGet(cmd->localId), gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    return FALSE;
}

bool32 ScrCmd_removeobject_at(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum);
    RemoveObjectEventByLocalIdAndMap(VarGet(cmd->localId), cmd->mapNum, cmd->mapGroup);
    return FALSE;
}

bool32 ScrCmd_addobject(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId);
    TrySpawnObjectEvent(VarGet(cmd->localId), gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    return FALSE;
}

bool32 ScrCmd_addobject_at(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum);
    TrySpawnObjectEvent(VarGet(cmd->localId), cmd->mapNum, cmd->mapGroup);
    return FALSE;
}

bool32 ScrCmd_setobjectxy(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, u16 x, u16 y);
    TryMoveObjectEventToMapCoords(VarGet(cmd->localId), gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_setobjectxyperm(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, u16 x, u16 y);
    Overworld_SetMapObjTemplateCoords(VarGet(cmd->localId), VarGet(cmd->x), VarGet(cmd->y));
    return FALSE;
}

bool32 ScrCmd_moveobjectoffscreen(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId);
    TryOverrideObjectEventTemplateCoords(VarGet(cmd->localId), gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    return FALSE;
}

bool32 ScrCmd_showobject_at(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum);
    ShowOrHideObjectByLocalIdAndMap(VarGet(cmd->localId), cmd->mapNum, cmd->mapGroup, FALSE);
    return FALSE;
}

bool32 ScrCmd_hideobject_at(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum);
    ShowOrHideObjectByLocalIdAndMap(VarGet(cmd->localId), cmd->mapNum, cmd->mapGroup, TRUE);
    return FALSE;
}

bool32 ScrCmd_setobjectpriority(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum, u8 priority);
    SetObjectPriorityByLocalIdAndMap(VarGet(cmd->localId), cmd->mapNum, cmd->mapGroup, cmd->priority + 83);
    return FALSE;
}

bool32 ScrCmd_resetobjectpriority(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, s8 mapGroup, s8 mapNum);
    UnfixObjectPriorityByLocalIdAndMap(VarGet(cmd->localId), cmd->mapNum, cmd->mapGroup);
    return FALSE;
}

bool32 ScrCmd_faceplayer(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
    if (gObjectEvents[gSelectedObjectEvent].active)
        ObjectEventFaceOppositeDirection(&gObjectEvents[gSelectedObjectEvent], GetPlayerFacingDirection());

    return FALSE;
}

bool32 ScrCmd_turnobject(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, u8 direction);
    ObjectEventTurnByLocalIdAndMap(VarGet(cmd->localId), gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, cmd->direction);
    return FALSE;
}

bool32 ScrCmd_setobjectmovementtype(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 localId, u8 movementType);
    Overworld_SetObjEventTemplateMovementType(VarGet(cmd->localId), cmd->movementType);
    return FALSE;
}

bool32 ScrCmd_createvobject(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 graphicsId, u8 unknown, u16 x, u16 y, u8 elevation, u8 direction);
    sprite_new(cmd->graphicsId, cmd->unknown, VarGet(cmd->x), VarGet(cmd->y), cmd->elevation, cmd->direction);
    return FALSE;
}

bool32 ScrCmd_turnvobject(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 graphicsId, u8 direction);
    TurnObjectEvent(cmd->graphicsId, cmd->direction);
    return FALSE;
}

bool32 ScrCmd_lockall(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
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
    CMD_ARGS();
    
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
    CMD_ARGS();
    
    HideFieldMessageBox();
    ObjectEventClearHeldMovementIfFinished(&gObjectEvents[GetObjectEventIdByLocalIdAndMap(OBJ_EVENT_ID_PLAYER, 0, 0)]);
    ScriptMovement_UnfreezeObjectEvents();
    UnfreezeObjectEvents();
    
    return FALSE;
}

bool32 ScrCmd_release(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
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
    CMD_ARGS(u8 color);
    
    gSpecialVar_PrevTextColor = gSpecialVar_TextColor;
    gSpecialVar_TextColor = cmd->color;
    
    return FALSE;
}

bool32 ScrCmd_message(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *msg);
    
    const u8 *msg = cmd->msg;
    if (msg == NULL)
        msg = (const u8 *)ctx->data[0];
    
    ShowFieldMessage(msg);
    
    return FALSE;
}

bool32 ScrCmd_messageautoscroll(struct ScriptContext * ctx)
{
    CMD_ARGS(const u8 *msg);
    
    const u8 *msg = cmd->msg;
    if (msg == NULL)
        msg = (const u8 *)ctx->data[0];
    
    ShowFieldAutoScrollMessage(msg);
    
    return FALSE;
}

bool32 ScrCmd_waitmessage(struct ScriptContext * ctx)
{
    CMD_ARGS();
    SetupNativeScript(ctx, IsFieldMessageBoxHidden);
    return TRUE;
}

bool32 ScrCmd_closemessage(struct ScriptContext * ctx)
{
    CMD_ARGS();
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
    CMD_ARGS();
    SetupNativeScript(ctx, WaitForAorBPress);
    return TRUE;
}

bool32 ScrCmd_yesnobox(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
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
    CMD_ARGS(u8 x, u8 y, u8 multichoiceId, bool8 ignoreBPress, u8 defaultOpt, u8 perRowItems);

    if (ScriptMenu_Multichoice(cmd->x, cmd->y, cmd->multichoiceId, cmd->ignoreBPress, cmd->defaultOpt, cmd->perRowItems))
    {
        ScriptContext1_Stop();
        return TRUE;
    }
    return FALSE;
}

bool32 ScrCmd_showitempic(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 itemId, u8 x, u8 y);
    ScriptMenu_ShowItemPic(VarGet(cmd->itemId), cmd->x, cmd->y);
    return FALSE;
}

bool32 ScrCmd_showmonpic(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 species, u8 x, u8 y);
    
    u32 species = VarGet(cmd->species);

    ScriptMenu_ShowPokemonPic(species, cmd->x, cmd->y);
    PlayCry_Normal(species, 0);
    
    return FALSE;
}

bool32 ScrCmd_hidemonpic(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
    bool32 (*func)(void) = ScriptMenu_GetPicboxWaitFunc();
    if (func == NULL)
        return FALSE;
    
    SetupNativeScript(ctx, func);
    
    return TRUE;
}

bool32 ScrCmd_braillemessage(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 *ptr);
    
    u8 *ptr = cmd->ptr;
    if (ptr == NULL)
        ptr = (u8 *)ctx->data[0];

    LoadStdWindowFrameGfx();
    DrawDialogueFrame(0, 1);
    AddTextPrinterParameterized(0, 6, ptr, 0, 1, 0, NULL);
    
    return FALSE;
}

bool32 ScrCmd_getbraillestringwidth(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 *ptr);
    
    u8 *ptr = cmd->ptr;
    if (ptr == NULL)
        ptr = (u8 *)ctx->data[0];

    gSpecialVar_0x8004 = GetStringWidth(6, ptr, -1);
    
    return FALSE;
}

bool32 ScrCmd_vmessage(struct ScriptContext * ctx)
{
    CMD_ARGS(u32 offset);
    ShowFieldMessage((u8 *)(cmd->offset - gVScriptOffset));
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
    CMD_ARGS(u8 outputStringId, u16 species);
    StringCopy(sScriptStringVars[cmd->outputStringId], gSpeciesInfo[VarGet(cmd->species)].name);
    return FALSE;
}

bool32 ScrCmd_bufferleadmonspeciesname(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId);
    StringCopy(sScriptStringVars[cmd->outputStringId], gSpeciesInfo[GetMonData(&gPlayerParty[GetLeadMonIndex()], MON_DATA_SPECIES)].name);
    return FALSE;
}

bool32 ScrCmd_bufferpartymonnick(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 partyIndex);

    GetMonData(&gPlayerParty[VarGet(cmd->partyIndex)], MON_DATA_NICKNAME, sScriptStringVars[cmd->outputStringId]);
    StringGet_Nickname(sScriptStringVars[cmd->outputStringId]);
    
    return FALSE;
}

bool32 ScrCmd_bufferitemname(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 itemId);
    CopyItemName(VarGet(cmd->itemId), sScriptStringVars[cmd->outputStringId]);
    return FALSE;
}

bool32 ScrCmd_bufferitemnameplural(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 itemId, u16 quantity);
    CopyItemNameHandlePlural(VarGet(cmd->itemId), VarGet(cmd->quantity), sScriptStringVars[cmd->outputStringId]);
    return FALSE;
}

bool32 ScrCmd_buffermovename(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 move);
    StringCopy(sScriptStringVars[cmd->outputStringId], gBattleMoves[VarGet(cmd->move)].name);
    return FALSE;
}

bool32 ScrCmd_buffernumberstring(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 number);

    u32 val = VarGet(cmd->number);
    ConvertIntToDecimalStringN(sScriptStringVars[cmd->outputStringId], val, 0, CountDigits(val));
    
    return FALSE;
}

bool32 ScrCmd_bufferstdstring(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 index);
    StringCopy(sScriptStringVars[cmd->outputStringId], gStdStringPtrs[VarGet(cmd->index)]);
    return FALSE;
}

bool32 ScrCmd_bufferstring(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, const u8 *message);
    StringCopy(sScriptStringVars[cmd->outputStringId], cmd->message);
    return FALSE;
}

bool32 ScrCmd_vloadword(struct ScriptContext * ctx)
{
    CMD_ARGS(u32 offset);
    
    const u8 *ptr = (u8 *)(cmd->offset - gVScriptOffset);
    StringExpandPlaceholders(gStringVar4, ptr);
    
    return FALSE;
}

bool32 ScrCmd_vbufferstring(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u32 offset);

    const u8 *src = (u8 *)(cmd->offset - gVScriptOffset);
    StringCopy(sScriptStringVars[cmd->outputStringId], src);
    
    return FALSE;
}

bool32 ScrCmd_bufferboxname(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 outputStringId, u16 boxId);
    StringCopy(sScriptStringVars[cmd->outputStringId], GetBoxNamePtr(VarGet(cmd->boxId)));
    return FALSE;
}

bool32 ScrCmd_givemon(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 species, u8 level, u16 item, u8 ivs[NUM_STATS], bool8 isShiny, bool8 hiddenAbility, u16 ballId, u8 nature, u8 gender);
    
    u16 moves[MAX_MON_MOVES] = {0};
    u8 ivs[NUM_STATS];
    
    memcpy(ivs, cmd->ivs, NUM_STATS);
    
    gSpecialVar_Result = ScriptGiveMon(VarGet(cmd->species), cmd->level, VarGet(cmd->item), ivs, cmd->ballId, cmd->isShiny ? GENERATE_SHINY_FORCED : GENERATE_SHINY_NORMAL,
                                       cmd->hiddenAbility, cmd->nature, cmd->gender, moves);
    
    return FALSE;
}

bool32 ScrCmd_giveegg(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 species, u8 ivs[NUM_STATS], bool8 isShiny, bool8 hiddenAbility, u8 nature);
    
    u16 moves[MAX_MON_MOVES] = {0};
    u8 ivs[NUM_STATS];
    
    memcpy(ivs, cmd->ivs, NUM_STATS);
    
    gSpecialVar_Result = ScriptGiveEgg(VarGet(cmd->species), ivs, cmd->isShiny ? GENERATE_SHINY_FORCED : GENERATE_SHINY_NORMAL, cmd->hiddenAbility, cmd->nature, moves);
    
    return FALSE;
}

bool32 ScrCmd_setmonmove(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 partyIndex, u8 moveSlot, u16 move);
    ScriptSetMonMoveSlot(cmd->partyIndex, cmd->move, cmd->moveSlot);
    return FALSE;
}

bool32 ScrCmd_checkpartymove(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 move);
    
    u32 i;

    gSpecialVar_Result = PARTY_SIZE;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u32 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL);
        if (!species)
            break;
        
        if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG) && FindMoveSlotInMoveset(&gPlayerParty[i], cmd->move) != MAX_MON_MOVES)
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
    CMD_ARGS(u32 amount);
    AddMoney(&gSaveBlock1Ptr->money, cmd->amount);
    return FALSE;
}

bool32 ScrCmd_removemoney(struct ScriptContext * ctx)
{
    CMD_ARGS(u32 amount);
    RemoveMoney(&gSaveBlock1Ptr->money, cmd->amount);
    return FALSE;
}

bool32 ScrCmd_checkmoney(struct ScriptContext * ctx)
{
    CMD_ARGS(u32 amount);
    gSpecialVar_Result = IsEnoughMoney(&gSaveBlock1Ptr->money, cmd->amount);
    return FALSE;
}

bool32 ScrCmd_showmoneybox(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 x, u8 y);
    DrawMoneyBox(GetMoney(&gSaveBlock1Ptr->money), cmd->x, cmd->y);
    return FALSE;
}

bool32 ScrCmd_hidemoneybox(struct ScriptContext * ctx)
{
    CMD_ARGS();
    HideMoneyBox();
    return FALSE;
}

bool32 ScrCmd_updatemoneybox(struct ScriptContext * ctx)
{
    CMD_ARGS();
    ChangeAmountInMoneyBox(GetMoney(&gSaveBlock1Ptr->money));
    return FALSE;
}

bool32 ScrCmd_showcoinsbox(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 x, u8 y);
    ShowCoinsWindow(GetCoins(), cmd->x, cmd->y);
    return FALSE;
}

bool32 ScrCmd_hidecoinsbox(struct ScriptContext * ctx)
{
    CMD_ARGS();
    HideCoinsWindow();
    return FALSE;
}

bool32 ScrCmd_updatecoinsbox(struct ScriptContext * ctx)
{
    CMD_ARGS();
    PrintCoinsString(GetCoins());
    return FALSE;
}

bool32 ScrCmd_trainerbattle(struct ScriptContext * ctx)
{
    // Don't need call CMD_ARGS here
    ctx->scriptPtr = BattleSetup_ConfigureTrainerBattle(ctx->scriptPtr);
    return FALSE;
}

bool32 ScrCmd_dotrainerbattle(struct ScriptContext * ctx)
{
    CMD_ARGS();
    StartTrainerBattle();
    return TRUE;
}

bool32 ScrCmd_gotopostbattlescript(struct ScriptContext * ctx)
{
    CMD_ARGS();
    ctx->scriptPtr = BattleSetup_GetScriptAddrAfterBattle();
    return FALSE;
}

bool32 ScrCmd_gotobeatenscript(struct ScriptContext * ctx)
{
    CMD_ARGS();
    ctx->scriptPtr = BattleSetup_GetTrainerPostBattleScript();
    return FALSE;
}

bool32 ScrCmd_checktrainerflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 trainerId);
    ctx->comparisonResult = HasTrainerBeenFought(VarGet(cmd->trainerId));
    return FALSE;
}

bool32 ScrCmd_settrainerflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 trainerId);
    SetTrainerFlag(VarGet(cmd->trainerId));
    return FALSE;
}

bool32 ScrCmd_cleartrainerflag(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 trainerId);
    ClearTrainerFlag(VarGet(cmd->trainerId));
    return FALSE;
}

bool32 ScrCmd_setwildbattle(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 species1, u8 level1, u16 item1, u16 species2, u8 level2, u16 item2);
    CreateScriptedWildMon(cmd->species1, cmd->level1, cmd->item1, cmd->species2, cmd->level2, cmd->item2);
    return FALSE;
}

bool32 ScrCmd_dowildbattle(struct ScriptContext * ctx)
{
    CMD_ARGS();
    
    StartScriptedWildBattle();
    ScriptContext1_Stop();
    
    return TRUE;
}

bool32 ScrCmd_pokemart(struct ScriptContext * ctx)
{
    CMD_ARGS(const void *list);

    CreatePokemartMenu(cmd->list);
    ScriptContext1_Stop();
    
    return TRUE;
}

bool32 ScrCmd_pokemartdecoration(struct ScriptContext * ctx)
{
    CMD_ARGS(const void *list);

    CreateDecorationShop1Menu(cmd->list);
    ScriptContext1_Stop();
    
    return TRUE;
}

bool32 ScrCmd_pokemartdecoration2(struct ScriptContext * ctx)
{
    CMD_ARGS(const void *list);

    CreateDecorationShop2Menu(cmd->list);
    ScriptContext1_Stop();
    
    return TRUE;
}

bool32 ScrCmd_playslotmachine(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 index);
    
    PlaySlotMachine(VarGet(cmd->index), CB2_ReturnToFieldContinueScriptPlayMapMusic);
    ScriptContext1_Stop();
    
    return TRUE;
}

bool32 ScrCmd_dofieldeffect(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 fieldEffectId);
    
    sFieldEffectScriptId = VarGet(cmd->fieldEffectId);
    FieldEffectStart(sFieldEffectScriptId);
    
    return FALSE;
}

bool32 ScrCmd_setfieldeffectarg(struct ScriptContext * ctx)
{
    CMD_ARGS(u8 argNum, u16 value);
    gFieldEffectArguments[cmd->argNum] = (s16)VarGet(cmd->value);
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
    CMD_ARGS(u16 fieldEffectId);
    
    sFieldEffectScriptId = VarGet(cmd->fieldEffectId);
    SetupNativeScript(ctx, WaitForFieldEffectFinish);
    
    return TRUE;
}

bool32 ScrCmd_setrespawn(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 healLocation);
    SetLastHealLocationWarp(VarGet(cmd->healLocation));
    return FALSE;
}

bool32 ScrCmd_checkplayergender(struct ScriptContext * ctx)
{
    CMD_ARGS();
    gSpecialVar_Result = gSaveBlock2Ptr->playerGender;
    return FALSE;
}

bool32 ScrCmd_playmoncry(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 species, u16 mode);
    PlayCry_ByMode(VarGet(cmd->species), 0, VarGet(cmd->mode));
    return FALSE;
}

bool32 ScrCmd_waitmoncry(struct ScriptContext * ctx)
{
    CMD_ARGS();
    SetupNativeScript(ctx, IsCryFinished);
    return TRUE;
}

bool32 ScrCmd_setmetatile(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 x, u16 y, u16 tileId, bool16 isImpassable);
    
    u16 x = VarGet(cmd->x);
    u16 y = VarGet(cmd->y);
    u32 tileId = VarGet(cmd->tileId);

    x += 7;
    y += 7;
    
    if (!cmd->isImpassable)
        MapGridSetMetatileIdAt(x, y, tileId);
    else
        MapGridSetMetatileIdAt(x, y, tileId | METATILE_COLLISION_MASK);
    
    return FALSE;
}

bool32 ScrCmd_opendoor(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 x, u16 y);
    
    u16 x = VarGet(cmd->x);
    u16 y = VarGet(cmd->y);

    x += 7;
    y += 7;
    
    PlaySE(GetDoorSoundEffect(x, y));
    FieldAnimateDoorOpen(x, y);
    
    return FALSE;
}

bool32 ScrCmd_closedoor(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 x, u16 y);
    
    u16 x = VarGet(cmd->x);
    u16 y = VarGet(cmd->y);

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
    CMD_ARGS();
    SetupNativeScript(ctx, IsDoorAnimationStopped);
    return TRUE;
}

bool32 ScrCmd_setdooropen(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 x, u16 y);
    
    u16 x = VarGet(cmd->x);
    u16 y = VarGet(cmd->y);

    x += 7;
    y += 7;
    
    FieldSetDoorOpened(x, y);
    
    return FALSE;
}

bool32 ScrCmd_setdoorclosed(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 x, u16 y);
    
    u16 x = VarGet(cmd->x);
    u16 y = VarGet(cmd->y);

    x += 7;
    y += 7;
    
    FieldSetDoorClosed(x, y);
    
    return FALSE;
}

bool32 ScrCmd_checkcoins(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 outputVar);
    *GetVarPointer(cmd->outputVar) = GetCoins();
    return FALSE;
}

bool32 ScrCmd_addcoins(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 amount);
    gSpecialVar_Result = !AddCoins(VarGet(cmd->amount));
    return FALSE;
}

bool32 ScrCmd_removecoins(struct ScriptContext * ctx)
{
    CMD_ARGS(u16 amount);
    gSpecialVar_Result = !RemoveCoins(VarGet(cmd->amount));
    return FALSE;
}

bool32 ScrCmd_signmsg(struct ScriptContext * ctx)
{
    CMD_ARGS();
    MsgSetSignPost();
    return FALSE;
}

bool32 ScrCmd_normalmsg(struct ScriptContext * ctx)
{
    CMD_ARGS();
    MsgSetNotSignPost();
    return FALSE;
}
