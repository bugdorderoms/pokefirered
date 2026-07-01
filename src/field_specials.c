#include "global.h"
#include "gflib.h"
#include "list_menu.h"
#include "diploma.h"
#include "script.h"
#include "decompress.h"
#include "field_player_avatar.h"
#include "overworld.h"
#include "field_fadetransition.h"
#include "field_weather.h"
#include "hall_of_fame.h"
#include "field_message_box.h"
#include "event_data.h"
#include "load_save.h"
#include "learn_move.h"
#include "script_pokemon_util.h"
#include "battle.h"
#include "fieldmap.h"
#include "item.h"
#include "field_specials.h"
#include "region_map.h"
#include "task.h"
#include "berry_pouch.h"
#include "event_scripts.h"
#include "field_camera.h"
#include "field_effect.h"
#include "event_object_movement.h"
#include "menu_indicators.h"
#include "random.h"
#include "credits.h"
#include "tm_case.h"
#include "item_menu.h"
#include "mail_data.h"
#include "pokemon_storage_system.h"
#include "script_menu.h"
#include "data.h"
#include "pokedex.h"
#include "text_window.h"
#include "menu.h"
#include "naming_screen.h"
#include "party_menu.h"
#include "dynamic_placeholder_text_util.h"
#include "new_menu_helpers.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/item_menu.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/region_map_sections.h"
#include "constants/moves.h"
#include "constants/heal_locations.h"
#include "constants/menu.h"
#include "constants/event_objects.h"
#include "constants/metatile_labels.h"
#include "constants/regions.h"
#include "constants/map_event_ids.h"

struct FieldSpecialListMenu
{
    u8 count;
    u8 bgId;
    u8 x;
    u8 y;
    u8 maxShowed;
    u8 unknown; // if 0 close menu when choose, otherwise only suspend it
    u8 cursorPos;
    u8 itemsAbove;
    u16 baseBlock;
    u8 palNum;
    u8 windowTileNum;
};

struct ListMenuLabels
{
    const u8 *text;
};

struct ListMenuActions
{
    const struct ListMenuLabels * list;
};

struct FormChangeListMenuActions
{
    const struct ListMenuLabels * list;
    const u16 *forms;
    u8 count;
};

static EWRAM_DATA u8 sElevatorCurrentFloorWindowId = 0;
static EWRAM_DATA u16 sElevatorScroll = 0;
static EWRAM_DATA u16 sElevatorCursorPos = 0;
static EWRAM_DATA struct ListMenuItem * sListMenuItems = NULL;
static EWRAM_DATA u16 sListMenuLastScrollPosition = 0;
static EWRAM_DATA u8 sPCBoxToSendMon = 0;
static EWRAM_DATA u8 sBrailleTextCursorSpriteID = 0;

struct ListMenuTemplate sFieldSpecialsListMenuTemplate;
u16 sFieldSpecialsListMenuScrollBuffer;

static void Task_AnimatePcTurnOn(u32 taskId);
static void PcTurnOnUpdateMetatileId(bool32 flag);
static void Task_ShakeScreen(u32 taskId);
static u32 SampleResortGorgeousMon(void);
static u32 SampleResortGorgeousReward(void);
static void Task_ElevatorShake(u32 taskId);
static void AnimateElevatorWindowView(u32 nfloors, u32 direction);
static void Task_AnimateElevatorWindowView(u32 taskId);
static void CreateScriptListMenu(void);
static void ScriptListMenuMoveCursorFunction(s32 nothing, bool32 is, struct ListMenu * used);
static void Task_ListMenuHandleInput(u32 taskId);
static void Task_SuspendListMenu(u32 taskId);
static void Task_RedrawScrollArrowsAndWaitInput(u32 taskId);
static void Task_CreateMenuRemoveScrollIndicatorArrowPair(u32 taskId);
static void Task_ListMenuRemoveScrollIndicatorArrowPair(u32 taskId);
static void ChangeBoxPokemonNickname_CB(void);
static void ChangePokemonNickname_CB(void);
static void Task_RunPokemonLeagueLightingEffect(u32 taskId);
static void Task_CancelPokemonLeagueLightingEffect(u32 taskId);
static void Task_DoDeoxysTriangleInteraction(u32 taskId);
static void MoveDeoxysObject(u32 num);
static void Task_WaitDeoxysFieldEffect(u32 taskId);
static void Task_WingFlapSound(u32 taskId);

const u8 gText_1F[] = _("1F");
const u8 gText_2F[] = _("2F");
const u8 gText_3F[] = _("3F");
const u8 gText_4F[] = _("4F");
const u8 gText_5F[] = _("5F");
const u8 gText_6F[] = _("6F");
const u8 gText_7F[] = _("7F");
const u8 gText_8F[] = _("8F");
const u8 gText_9F[] = _("9F");
const u8 gText_10F[] = _("10F");
const u8 gText_11F[] = _("11F");
const u8 gText_B1F[] = _("B1F");
const u8 gText_B2F[] = _("B2F");
const u8 gText_B3F[] = _("B3F");
const u8 gText_B4F[] = _("B4F");
const u8 gText_Rooftop[] = _("Rooftop");
static const u8 sText_DefaultForm[] = _("Default Form");

static const u32 s8x8SymbolsGfx[] = INCBIN_U32("graphics/interface/8_8_symbols.4bpp.lz");
static const u16 s8x8SymbolsPal[] = INCBIN_U16("graphics/interface/8_8_symbols_pal.gbapal");

static const struct CompressedSpriteSheet s8x8SymbolsSpriteSheet = { .data = s8x8SymbolsGfx, .size = 0x20 * SYMBOLS_COUNT, .tag = TAG_8x8_SYMBOLS };
static const struct SpritePalette s8x8SymbolsSpritePal = { .data = s8x8SymbolsPal, .tag = TAG_8x8_SYMBOLS };

static const struct OamData s8x8SymbolOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 0,
};

static const union AnimCmd s8x8Symbol_StarIconYellow[] = {
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd s8x8Symbol_StarIconWhite[] = {
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END
};

static const union AnimCmd s8x8Symbol_PokeballIcon[] = {
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END
};

static const union AnimCmd s8x8Symbol_HeldItem[] = {
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END
};

static const union AnimCmd s8x8Symbol_HeldMail[] = {
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END
};

static const union AnimCmd * const s8x8SymbolsAnimTable[] =
{
    [SYMBOL_YELLOWSTAR] = s8x8Symbol_StarIconYellow,
    [SYMBOL_WHITESTAR]  = s8x8Symbol_StarIconWhite,
    [SYMBOL_POKEBALL]   = s8x8Symbol_PokeballIcon,
    [SYMBOL_HELDITEM]   = s8x8Symbol_HeldItem,
    [SYMBOL_HELDMAIL]   = s8x8Symbol_HeldMail
};

static const struct SpriteTemplate s8x8SymbolSpriteTemplate =
{
    .tileTag = TAG_8x8_SYMBOLS,
    .paletteTag = TAG_8x8_SYMBOLS,
    .oam = &s8x8SymbolOam,
    .anims = s8x8SymbolsAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static u8 *const sStringVarPtrs[] = {
    gStringVar1,
    gStringVar2,
    gStringVar3
};

void ShowDiploma(void)
{
    SetMainCallback2(CB2_ShowDiploma);
    ScriptContext2_Enable();
}

void ForcePlayerOntoBike(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT))
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_MACH_BIKE);
    
    Overworld_SetSavedMusic(MUS_CYCLING);
    Overworld_ChangeMusicTo(MUS_CYCLING);
}

void ShowFieldMessageStringVar4(void)
{
    ShowFieldMessage(gStringVar4);
}

void BufferBigGuyOrBigGirlString(void)
{
    StringCopy(gStringVar1, gSaveBlock2Ptr->playerGender == MALE ? COMPOUND_STRING("Big guy") : COMPOUND_STRING("Big girl"));
}

u32 GetBattleOutcome(void)
{
    return gBattleOutcome;
}

void SetHiddenItemFlag(void)
{
    FlagSet(gSpecialVar_0x8004);
}

u32 GetLeadMonFriendship(void)
{
    struct Pokemon * pokemon = &gPlayerParty[GetLeadMonIndex()];
    
    if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) == 255)
        return 6;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 200)
        return 5;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 150)
        return 4;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 100)
        return 3;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 50)
        return 2;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) > 0)
        return 1;
    else
        return 0;
}

void ShowTownMap(void)
{
    InitRegionMapWithExitCB(REGIONMAP_TYPE_WALL, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

#define tState data[0]
#define tTimer data[1]

void AnimatePcTurnOn(void)
{
    u32 taskId;

    if (!FuncIsActiveTask(Task_AnimatePcTurnOn))
    {
        taskId = CreateTask(Task_AnimatePcTurnOn, 8);
        gTasks[taskId].tState = 0;
        gTasks[taskId].tTimer = 0;
    }
}

// PC flickers on and off while turning on
static void Task_AnimatePcTurnOn(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (tTimer == 6)
    {
        PcTurnOnUpdateMetatileId(tState & 1);
        DrawWholeMapView();
        tTimer = 0;

        if (++tState == 5)
            DestroyTask(taskId);
    }
    tTimer++;
}

#undef tState
#undef tTimer

static void PcTurnOnUpdateMetatileId(bool32 flickerOff)
{
    u32 metatileId = 0;
    s8 deltaX = 0;
    s8 deltaY = 0;

    switch (GetPlayerFacingDirection())
    {
    case DIR_NORTH:
        deltaX = 0;
        deltaY = -1;
        break;
    case DIR_WEST:
        deltaX = -1;
        deltaY = -1;
        break;
    case DIR_EAST:
        deltaX = 1;
        deltaY = -1;
        break;
    }
    
    if (flickerOff)
    {
        if (gSpecialVar_0x8004 == 0)
            metatileId = METATILE_Building_PCOff;
        else if (gSpecialVar_0x8004 == 1)
            metatileId = METATILE_GenericBuilding1_PlayersPCOff;
        else if (gSpecialVar_0x8004 == 2)
            metatileId = METATILE_GenericBuilding1_PlayersPCOff;
    }
    else
    {
        if (gSpecialVar_0x8004 == 0)
            metatileId = METATILE_Building_PCOn;
        else if (gSpecialVar_0x8004 == 1)
            metatileId = METATILE_GenericBuilding1_PlayersPCOn;
        else if (gSpecialVar_0x8004 == 2)
            metatileId = METATILE_GenericBuilding1_PlayersPCOn;
    }
    MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + deltaX + 7, gSaveBlock1Ptr->pos.y + deltaY + 7, metatileId | METATILE_COLLISION_MASK);
}

void AnimatePcTurnOff(void)
{
    u32 metatileId = 0;
    s8 deltaX = 0;
    s8 deltaY = 0;

    switch (GetPlayerFacingDirection())
    {
    case DIR_NORTH:
        deltaX = 0;
        deltaY = -1;
        break;
    case DIR_WEST:
        deltaX = -1;
        deltaY = -1;
        break;
    case DIR_EAST:
        deltaX = 1;
        deltaY = -1;
        break;
    }
    if (gSpecialVar_0x8004 == 0)
        metatileId = METATILE_Building_PCOff;
    else if (gSpecialVar_0x8004 == 1)
        metatileId = METATILE_GenericBuilding1_PlayersPCOff;
    else if (gSpecialVar_0x8004 == 2)
        metatileId = METATILE_GenericBuilding1_PlayersPCOff;
    
    MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + deltaX + 7, gSaveBlock1Ptr->pos.y + deltaY + 7, metatileId | METATILE_COLLISION_MASK);
    DrawWholeMapView();
}

void SpawnCameraObject(void)
{
    u32 objectEventId = SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_YOUNGSTER, 8, OBJ_EVENT_ID_CAMERA, gSaveBlock1Ptr->pos.x + 7, gSaveBlock1Ptr->pos.y + 7, 3);
    gObjectEvents[objectEventId].invisible = TRUE;
    CameraObjectSetFollowedObjectId(gObjectEvents[objectEventId].spriteId);
}

void RemoveCameraObject(void)
{
    CameraObjectSetFollowedObjectId(gPlayerAvatar.spriteId);
    RemoveObjectEventByLocalIdAndMap(127, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
}

static const u8 sSlotMachineIndices[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    2,
    2,
    2,
    3,
    3,
    3,
    4,
    4,
    5
};

u32 GetRandomSlotMachineId(void)
{
    return RandomElement(RNG_SLOT_MACHINE_INDICE, sSlotMachineIndices);
}

bool32 IsThereRoomInAnyBoxForMorePokemon(void)
{
    u32 i, j;

    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
    {
        for (j = 0; j < IN_BOX_COUNT; j++)
        {
            if (GetBoxMonDataAt(i, j, MON_DATA_SPECIES) == SPECIES_NONE)
                return TRUE;
        }
    }
    return FALSE;
}

#define tXtrans   data[0]
#define tTimer    data[1]
#define tNremain  data[2]
#define tDuration data[3]
#define tYtrans   data[4]

void ShakeScreen(void)
{
    /*
     * 0x8004 = x translation
     * 0x8005 = y translation
     * 0x8006 = num interations
     * 0x8007 = duration of an iteration
     */
    u32 taskId = CreateTask(Task_ShakeScreen, 9);
    gTasks[taskId].tXtrans = gSpecialVar_0x8005;
    gTasks[taskId].tTimer = 0;
    gTasks[taskId].tNremain = gSpecialVar_0x8006;
    gTasks[taskId].tDuration = gSpecialVar_0x8007;
    gTasks[taskId].tYtrans = gSpecialVar_0x8004;
    SetCameraPanningCallback(NULL);
    PlaySE(SE_M_STRENGTH);
}

static void Task_ShakeScreen(u32 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (++tTimer % tDuration == 0)
    {
        tTimer = 0;
        tNremain--;
        tXtrans = -tXtrans;
        tYtrans = -tYtrans;
        SetCameraPanning(tXtrans, tYtrans);
        if (tNremain == 0)
        {
            DestroyTask(taskId);
            EnableBothScriptContexts();
            InstallCameraPanAheadCallback();
        }
    }
}

#undef tYtrans
#undef tDuration
#undef tNremain
#undef tTimer
#undef tXtrans

u32 GetLeadMonIndex(void)
{
    u32 i;
    
    for (i = 0; i < CalculatePlayerPartyCount(); i++)
    {
        if (IsMonValidSpecies(&gPlayerParty[i]))
            return i;
    }
    return 0;
}

u32 GetPartyMonSpecies(void)
{
    return GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES2, NULL);
}

bool32 IsMonOTNameNotPlayers(void)
{
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_OT_NAME, gStringVar1);
    
    if (!StringCompare(gSaveBlock2Ptr->playerName, gStringVar1))
        return FALSE;
    else
        return TRUE;
}

void DoPicboxCancel(void)
{
    u8 t = EOS;
    AddTextPrinterParameterized(0, 2, &t, 0, 1, 0, NULL);
    PicboxCancel();
}

void SetVermilionTrashCans(void)
{
    u32 idx = (Random() % 15) + 1;
    
    gSpecialVar_0x8004 = idx;
    gSpecialVar_0x8005 = idx;
    
    switch (gSpecialVar_0x8004)
    {
    case 1:
        idx = Random() % 2;
        
        if (idx == 0)
            gSpecialVar_0x8005 += 1;
        else
            gSpecialVar_0x8005 += 5;
        break;
    case 2:
    case 3:
    case 4:
        idx = Random() % 3;
        
        if (idx == 0)
            gSpecialVar_0x8005 += 1;
        else if (idx == 1)
            gSpecialVar_0x8005 += 5;
        else
            gSpecialVar_0x8005 -= 1;
        break;
    case 5:
        idx = Random() % 2;
        
        if (idx == 0)
            gSpecialVar_0x8005 += 5;
        else
            gSpecialVar_0x8005 -= 1;
        break;
    case 6:
        idx = Random() % 3;
        
        if (idx == 0)
            gSpecialVar_0x8005 -= 5;
        else if (idx == 1)
            gSpecialVar_0x8005 += 1;
        else
            gSpecialVar_0x8005 += 5;
        break;
    case 7:
    case 8:
    case 9:
        idx = Random() % 4;
        
        if (idx == 0)
            gSpecialVar_0x8005 -= 5;
        else if (idx == 1)
            gSpecialVar_0x8005 += 1;
        else if (idx == 2)
            gSpecialVar_0x8005 += 5;
        else
            gSpecialVar_0x8005 -= 1;
        break;
    case 10:
        idx = Random() % 3;
        
        if (idx == 0)
            gSpecialVar_0x8005 -= 5;
        else if (idx == 1)
            gSpecialVar_0x8005 += 5;
        else
            gSpecialVar_0x8005 -= 1;
        break;
    case 11:
        idx = Random() % 2;
        
        if (idx == 0)
            gSpecialVar_0x8005 -= 5;
        else
            gSpecialVar_0x8005 += 1;
        break;
    case 12:
    case 13:
    case 14:
        idx = Random() % 3;
        
        if (idx == 0)
            gSpecialVar_0x8005 -= 5;
        else if (idx == 1)
            gSpecialVar_0x8005 += 1;
        else
            gSpecialVar_0x8005 -= 1;
        break;
    case 15:
        idx = Random() % 2;
        
        if (idx == 0)
            gSpecialVar_0x8005 -= 5;
        else
            gSpecialVar_0x8005 -= 1;
        break;
    }
    
    if (gSpecialVar_0x8005 > 15)
    {
        if (gSpecialVar_0x8004 % 5 == 1)
            gSpecialVar_0x8005 = gSpecialVar_0x8004 + 1;
        else if (gSpecialVar_0x8004 % 5 == 0)
            gSpecialVar_0x8005 = gSpecialVar_0x8004 - 1;
        else
            gSpecialVar_0x8005 = gSpecialVar_0x8004 + 1;
    }
}

static const u16 sResortGorgeousDeluxeRewards[] = {
    ITEM_BIG_PEARL,
    ITEM_PEARL,
    ITEM_STARDUST,
    ITEM_STAR_PIECE,
    ITEM_NUGGET,
    ITEM_RARE_CANDY
};

void IncrementResortGorgeousStepCounter(void)
{
    u32 var4035;
    
    if (VarGet(VAR_RESORT_GORGEOUS_REQUESTED_MON) != SPECIES_NONE)
    {
        var4035 = VarGet(VAR_RESORT_GOREGEOUS_STEP_COUNTER) + 1;
        
        if (var4035 >= 250)
        {
            VarSet(VAR_RESORT_GORGEOUS_REQUESTED_MON, 0xFFFF);
            VarSet(VAR_RESORT_GOREGEOUS_STEP_COUNTER, 0);
        }
        else
            VarSet(VAR_RESORT_GOREGEOUS_STEP_COUNTER, var4035);
    }
}

void SampleResortGorgeousMonAndReward(void)
{
    u32 requestedSpecies = VarGet(VAR_RESORT_GORGEOUS_REQUESTED_MON);
    
    if (requestedSpecies == SPECIES_NONE || requestedSpecies == 0xFFFF)
    {
        VarSet(VAR_RESORT_GORGEOUS_REQUESTED_MON, SampleResortGorgeousMon());
        VarSet(VAR_RESORT_GORGEOUS_REWARD, SampleResortGorgeousReward());
        VarSet(VAR_RESORT_GOREGEOUS_STEP_COUNTER, 0);
    }
    StringCopy(gStringVar1, gSpeciesInfo[requestedSpecies].name);
}

static u32 SampleResortGorgeousMon(void)
{
    u32 i, species;
    
    for (i = 0; i < 100; i++)
    {
        species = RandomUniform(RNG_SAMPLE_RESORT_SPECIES, SPECIES_BULBASAUR, NUM_SPECIES - 1);
        
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
            return species;
    }
    
    while (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
    {
        if (species == SPECIES_BULBASAUR)
            species = NUM_SPECIES - 1;
        else
            species--;
    }
    return species;
}

static u32 SampleResortGorgeousReward(void)
{
    if (RandomPercentage(RNG_SAMPLE_RESORT_DELUXE, 30))
        return RandomElement(RNG_SAMPLE_RESORT_DELUXE_REWARD, sResortGorgeousDeluxeRewards);
    else
        return ITEM_LUXURY_BALL;
}

bool32 CheckAddCoins(void)
{
    return (gSpecialVar_Result + gSpecialVar_0x8006 <= 9999);
}

static const struct WindowTemplate sElevatorCurrentFloorWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 22,
    .tilemapTop = 1,
    .width = 7,
    .height = 4,
    .paletteNum = 0xF,
    .baseBlock = 0x008
};

static const u8 *const sFloorNamePointers[] = {
    gText_B4F,
    gText_B3F,
    gText_B2F,
    gText_B1F,
    gText_1F,
    gText_2F,
    gText_3F,
    gText_4F,
    gText_5F,
    gText_6F,
    gText_7F,
    gText_8F,
    gText_9F,
    gText_10F,
    gText_11F,
    gText_Rooftop
};

static const u16 sElevatorWindowMetatilesGoingUp[][3] = {
    {
        METATILE_SilphCo_ElevatorWindow_Top0, 
        METATILE_SilphCo_ElevatorWindow_Top1, 
        METATILE_SilphCo_ElevatorWindow_Top2
    },
    {
        METATILE_SilphCo_ElevatorWindow_Mid0, 
        METATILE_SilphCo_ElevatorWindow_Mid1, 
        METATILE_SilphCo_ElevatorWindow_Mid2
    },
    {
        METATILE_SilphCo_ElevatorWindow_Bottom0, 
        METATILE_SilphCo_ElevatorWindow_Bottom1, 
        METATILE_SilphCo_ElevatorWindow_Bottom2
    }
};

static const u16 sElevatorWindowMetatilesGoingDown[][3] = {
    {
        METATILE_SilphCo_ElevatorWindow_Top0, 
        METATILE_SilphCo_ElevatorWindow_Top2, 
        METATILE_SilphCo_ElevatorWindow_Top1
    },
    {
        METATILE_SilphCo_ElevatorWindow_Mid0, 
        METATILE_SilphCo_ElevatorWindow_Mid2, 
        METATILE_SilphCo_ElevatorWindow_Mid1
    },
    {
        METATILE_SilphCo_ElevatorWindow_Bottom0, 
        METATILE_SilphCo_ElevatorWindow_Bottom2, 
        METATILE_SilphCo_ElevatorWindow_Bottom1
    }
};

static const u8 sElevatorAnimationDuration[] = {
    8,
    16,
    24,
    32,
    38,
    46,
    53,
    56,
    57
};

static const u8 sElevatorWindowAnimDuration[] = {
    3,
    6,
    9,
    12,
    15,
    18,
    21,
    24,
    27
};

void GetElevatorFloor(void)
{
    u32 floor = 4;
    
    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(ROCKET_HIDEOUT_B1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(SILPH_CO_1F):
            floor = 4;
            break;
        case MAP_NUM(SILPH_CO_2F):
            floor = 5;
            break;
        case MAP_NUM(SILPH_CO_3F):
            floor = 6;
            break;
        case MAP_NUM(SILPH_CO_4F):
            floor = 7;
            break;
        case MAP_NUM(SILPH_CO_5F):
            floor = 8;
            break;
        case MAP_NUM(SILPH_CO_6F):
            floor = 9;
            break;
        case MAP_NUM(SILPH_CO_7F):
            floor = 10;
            break;
        case MAP_NUM(SILPH_CO_8F):
            floor = 11;
            break;
        case MAP_NUM(SILPH_CO_9F):
            floor = 12;
            break;
        case MAP_NUM(SILPH_CO_10F):
            floor = 13;
            break;
        case MAP_NUM(SILPH_CO_11F):
            floor = 14;
            break;
        case MAP_NUM(ROCKET_HIDEOUT_B1F):
            floor = 3;
            break;
        case MAP_NUM(ROCKET_HIDEOUT_B2F):
            floor = 2;
            break;
        case MAP_NUM(ROCKET_HIDEOUT_B4F):
            floor = 0;
            break;
        }
    }
    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(CELADON_CITY_DEPARTMENT_STORE_1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_1F):
            floor = 4;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_2F):
            floor = 5;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_3F):
            floor = 6;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_4F):
            floor = 7;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_5F):
            floor = 8;
            break;
        }
    }
    VarSet(VAR_ELEVATOR_FLOOR, floor);
}

u32 InitElevatorFloorSelectMenuPos(void)
{
    sElevatorScroll = 0;
    sElevatorCursorPos = 0;

    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(ROCKET_HIDEOUT_B1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(SILPH_CO_11F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 0;
            break;
        case MAP_NUM(SILPH_CO_10F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 1;
            break;
        case MAP_NUM(SILPH_CO_9F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 2;
            break;
        case MAP_NUM(SILPH_CO_8F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 3;
            break;
        case MAP_NUM(SILPH_CO_7F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(SILPH_CO_6F):
            sElevatorScroll = 1;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(SILPH_CO_5F):
            sElevatorScroll = 2;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(SILPH_CO_4F):
            sElevatorScroll = 3;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(SILPH_CO_3F):
            sElevatorScroll = 4;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(SILPH_CO_2F):
            sElevatorScroll = 5;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(SILPH_CO_1F):
            sElevatorScroll = 5;
            sElevatorCursorPos = 5;
            break;
        case MAP_NUM(ROCKET_HIDEOUT_B1F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 0;
            break;
        case MAP_NUM(ROCKET_HIDEOUT_B2F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 1;
            break;
        case MAP_NUM(ROCKET_HIDEOUT_B4F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 2;
            break;
        }
    }
    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(CELADON_CITY_DEPARTMENT_STORE_1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_5F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 0;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_4F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 1;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_3F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 2;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_2F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 3;
            break;
        case MAP_NUM(CELADON_CITY_DEPARTMENT_STORE_1F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 4;
            break;
        }
    }
    return sElevatorCursorPos;
}

void AnimateElevator(void)
{
    u16 nfloors;
    s16 *data = gTasks[CreateTask(Task_ElevatorShake, 9)].data;
    
    data[1] = 0;
    data[2] = 0;
    data[4] = 1;
    
    if (gSpecialVar_0x8005 > gSpecialVar_0x8006)
    {
        nfloors = gSpecialVar_0x8005 - gSpecialVar_0x8006;
        data[6] = 1;
    }
    else
    {
        nfloors = gSpecialVar_0x8006 - gSpecialVar_0x8005;
        data[6] = 0;
    }
    if (nfloors > 8)
        nfloors = 8;
    
    data[5] = sElevatorAnimationDuration[nfloors];
    SetCameraPanningCallback(NULL);
    AnimateElevatorWindowView(nfloors, data[6]);
    PlaySE(SE_ELEVATOR);
}

static void Task_ElevatorShake(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    data[1]++;
    
    if ((data[1] % 3) == 0)
    {
        data[1] = 0;
        data[2]++;
        data[4] = -data[4];
        SetCameraPanning(0, data[4]);
        if (data[2] == data[5])
        {
            PlaySE(SE_DING_DONG);
            DestroyTask(taskId);
            EnableBothScriptContexts();
            InstallCameraPanAheadCallback();
        }
    }
}

void DrawElevatorCurrentFloorWindow(void)
{
    const u8 *floorname;
    u32 strwidth;
    
    sElevatorCurrentFloorWindowId = AddWindow(&sElevatorCurrentFloorWindowTemplate);
    TextWindow_SetStdFrame0_WithPal(sElevatorCurrentFloorWindowId, 0x21D, 0xD0);
    DrawStdFrameWithCustomTileAndPalette(sElevatorCurrentFloorWindowId, FALSE, 0x21D, 0xD);
    AddTextPrinterParameterized(sElevatorCurrentFloorWindowId, 2, COMPOUND_STRING("Now on:"), 0, 2, 0xFF, NULL);
    floorname = sFloorNamePointers[gSpecialVar_0x8005];
    strwidth = GetStringWidth(2, floorname, 0);
    AddTextPrinterParameterized(sElevatorCurrentFloorWindowId, 2, floorname, 56 - strwidth, 16, 0xFF, NULL);
    PutWindowTilemap(sElevatorCurrentFloorWindowId);
    CopyWindowToVram(sElevatorCurrentFloorWindowId, COPYWIN_BOTH);
}

void CloseElevatorCurrentFloorWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sElevatorCurrentFloorWindowId, TRUE);
    RemoveWindow(sElevatorCurrentFloorWindowId);
}

static void AnimateElevatorWindowView(u32 nfloors, u32 direction)
{
    u32 taskId;
    
    if (!FuncIsActiveTask(Task_AnimateElevatorWindowView))
    {
        taskId = CreateTask(Task_AnimateElevatorWindowView, 8);
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].data[1] = 0;
        gTasks[taskId].data[2] = direction;
        gTasks[taskId].data[3] = sElevatorWindowAnimDuration[nfloors];
    }
}

static void Task_AnimateElevatorWindowView(u32 taskId)
{
    u32 i, j;
    s16 *data = gTasks[taskId].data;
    
    if (data[1] == 6)
    {
        data[0]++;
        
        if (data[2] == 0)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                    MapGridSetMetatileIdAt(j + 8, i + 7, sElevatorWindowMetatilesGoingUp[i][data[0] % 3] | METATILE_COLLISION_MASK);
            }
        }
        else
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                    MapGridSetMetatileIdAt(j + 8, i + 7, sElevatorWindowMetatilesGoingDown[i][data[0] % 3] | METATILE_COLLISION_MASK);
            }
        }
        DrawWholeMapView();
        
        data[1] = 0;
        
        if (data[0] == data[3])
            DestroyTask(taskId);
    }
    data[1]++;
}

static const struct ListMenuLabels sBadgesListMenu[] = {
    { COMPOUND_STRING("Boulderbadge") },
    { COMPOUND_STRING("Cascadebadge") },
    { COMPOUND_STRING("Thunderbadge") },
    { COMPOUND_STRING("Rainbowbadge") },
    { COMPOUND_STRING("Soulbadge") },
    { COMPOUND_STRING("Marshbadge") },
    { COMPOUND_STRING("Volcanobadge") },
    { COMPOUND_STRING("Earthbadge") },
    { COMPOUND_STRING("Exit") },
};

static const struct ListMenuLabels sSilphcoFloorsListMenu[] = {
    { gText_11F },
    { gText_10F },
    { gText_9F },
    { gText_8F },
    { gText_7F },
    { gText_6F },
    { gText_5F },
    { gText_4F },
    { gText_3F },
    { gText_2F },
    { gText_1F },
    { COMPOUND_STRING("Exit") },
};

static const struct ListMenuLabels sBerryPowderListMenu[] = {
    { COMPOUND_STRING("Energypowder{CLEAR_TO 0x74}{FONT_SMALL}50") },
    { COMPOUND_STRING("Energy Root{CLEAR_TO 0x74}{FONT_SMALL}80") },
    { COMPOUND_STRING("Heal Powder{CLEAR_TO 0x74}{FONT_SMALL}50") },
    { COMPOUND_STRING("Revival Herb{CLEAR_TO 0x6F}{FONT_SMALL}300") },
    { COMPOUND_STRING("Protein{CLEAR_TO 0x65}{FONT_SMALL}1,000") },
    { COMPOUND_STRING("Iron{CLEAR_TO 0x65}{FONT_SMALL}1,000") },
    { COMPOUND_STRING("Carbos{CLEAR_TO 0x65}{FONT_SMALL}1,000") },
    { COMPOUND_STRING("Calcium{CLEAR_TO 0x65}{FONT_SMALL}1,000") },
    { COMPOUND_STRING("Zinc{CLEAR_TO 0x65}{FONT_SMALL}1,000") },
    { COMPOUND_STRING("HP Up{CLEAR_TO 0x65}{FONT_SMALL}1,000") },
    { COMPOUND_STRING("PP Up{CLEAR_TO 0x65}{FONT_SMALL}3,000") },
    { COMPOUND_STRING("Exit") },
};

static const struct ListMenuActions sListMenuLabels[] = {
    [LISTMENU_BADGES]         = {sBadgesListMenu},
    [LISTMENU_SILPHCO_FLOORS] = {sSilphcoFloorsListMenu},
    [LISTMENU_BERRY_POWDER]   = {sBerryPowderListMenu},
};

static const struct ListMenuLabels sDeoxysListMenu[] = {
    { COMPOUND_STRING("Attack Form") },
    { COMPOUND_STRING("Defense Form") },
    { COMPOUND_STRING("Speed Form") },
    { sText_DefaultForm }
};

static const u16 sDeoxysForms[] =
{
    SPECIES_DEOXYS_ATTACK,
    SPECIES_DEOXYS_DEFENSE,
    SPECIES_DEOXYS_SPEED,
    SPECIES_DEOXYS
};

static const struct ListMenuLabels sRotomListMenu[] = {
    { COMPOUND_STRING("Heat Form") },
    { COMPOUND_STRING("Wash Form") },
    { COMPOUND_STRING("Frost Form") },
    { COMPOUND_STRING("Fan Form") },
    { COMPOUND_STRING("Mow Form") },
    { sText_DefaultForm }
};

static const u16 sRotomForms[] =
{
    SPECIES_ROTOM_HEAT,
    SPECIES_ROTOM_WASH,
    SPECIES_ROTOM_FROST,
    SPECIES_ROTOM_FAN,
    SPECIES_ROTOM_MOW,
    SPECIES_ROTOM
};

static const struct ListMenuLabels sPikachuListMenu[] = {
    { COMPOUND_STRING("Rock Star") },
    { COMPOUND_STRING("Belle") },
    { COMPOUND_STRING("Pop Star") },
    { COMPOUND_STRING("Ph. D.") },
    { COMPOUND_STRING("Libre") },
    { sText_DefaultForm }
};

static const u16 sPikachuForms[] =
{
    SPECIES_PIKACHU_ROCK_STAR,
    SPECIES_PIKACHU_BELLE,
    SPECIES_PIKACHU_POP_STAR,
    SPECIES_PIKACHU_PH_D,
    SPECIES_PIKACHU_LIBRE,
    SPECIES_PIKACHU_COSPLAY
};

static const struct ListMenuLabels sFurfrouListMenu[] = {
    { COMPOUND_STRING("Heart Trim") },
    { COMPOUND_STRING("Star Trim") },
    { COMPOUND_STRING("Diamond Trim") },
    { COMPOUND_STRING("Debutante Trim") },
    { COMPOUND_STRING("Matron Trim") },
    { COMPOUND_STRING("Dandy Trim") },
    { COMPOUND_STRING("La Reine Trim") },
    { COMPOUND_STRING("Kabuki Trim") },
    { COMPOUND_STRING("Pharaoh Trim") }
    // No default form, if changed its trim it will only returns after 5 days
};

static const u16 sFurfrouForms[] =
{
    SPECIES_FURFROU_HEART_TRIM,
    SPECIES_FURFROU_STAR_TRIM,
    SPECIES_FURFROU_DIAMOND_TRIM,
    SPECIES_FURFROU_DEBUTANTE_TRIM,
    SPECIES_FURFROU_MATRON_TRIM,
    SPECIES_FURFROU_DANDY_TRIM,
    SPECIES_FURFROU_LA_REINE_TRIM,
    SPECIES_FURFROU_KABUKI_TRIM,
    SPECIES_FURFROU_PHARAOH_TRIM
};

#define FORMS_LIST(listName)                                                          \
    { s##listName##ListMenu, s##listName##Forms, ARRAY_COUNT(s##listName##ListMenu) }

static const struct FormChangeListMenuActions sFormChangeMenuLabels[] = {
    FORMS_LIST(Deoxys),
    FORMS_LIST(Rotom),
    FORMS_LIST(Pikachu),
    FORMS_LIST(Furfrou),
};

static u32 InitFieldSpecialListMenu(const struct ListMenuLabels *list, const struct FieldSpecialListMenu *menuListTemplate)
{
    u8 width, mwidth, windowHeight;
    u32 i, taskId = CreateTask(Task_ListMenuHandleInput, 8);
    struct Task * task = &gTasks[taskId];
    struct WindowTemplate template;
    
    task->data[0] = menuListTemplate->maxShowed > menuListTemplate->count ? menuListTemplate->count : menuListTemplate->maxShowed;
    task->data[1] = menuListTemplate->count;
    task->data[2] = menuListTemplate->x;
    task->data[3] = menuListTemplate->y;
    task->data[6] = menuListTemplate->unknown;
    task->data[7] = menuListTemplate->cursorPos;
    task->data[8] = menuListTemplate->itemsAbove;
    task->data[5] = (task->data[0] * 2);
    
    windowHeight = task->data[5] - 1;
    if (task->data[5] >= 14)
    {
        task->data[5] -= 2;
        --windowHeight;
    }
    sListMenuItems = AllocZeroed(task->data[1] * sizeof(struct ListMenuItem));
    CreateScriptListMenu();
    
    for (i = 0, mwidth = 0; i < task->data[1]; i++)
    {
        sListMenuItems[i].label = list[i].text;
        sListMenuItems[i].index = i;
        
        width = GetStringWidth(2, sListMenuItems[i].label, 0);
        if (mwidth < width)
            mwidth = width;
    }
    task->data[4] = (mwidth + 9) / 8 + 1;
    if (task->data[2] + task->data[4] > 29)
        task->data[2] = 29 - task->data[4];
    
    template = SetWindowTemplateFields(menuListTemplate->bgId, task->data[2], task->data[3], task->data[4], windowHeight, menuListTemplate->palNum, menuListTemplate->baseBlock);
    task->data[13] = AddWindow(&template);
    
    if (menuListTemplate->windowTileNum == 0)
        SetStdWindowBorderStyle(task->data[13], 0);
    else
        DrawStdFrameWithCustomTileAndPalette(task->data[13], FALSE, menuListTemplate->windowTileNum, menuListTemplate->palNum);
    
    sFieldSpecialsListMenuTemplate.totalItems = task->data[1];
    sFieldSpecialsListMenuTemplate.maxShowed = task->data[0];
    sFieldSpecialsListMenuTemplate.windowId = task->data[13];
    Task_CreateMenuRemoveScrollIndicatorArrowPair(taskId);
    task->data[14] = ListMenuInit(&sFieldSpecialsListMenuTemplate, &task->data[7], &task->data[8]);
    PutWindowTilemap(task->data[13]);
    CopyWindowToVram(task->data[13], COPYWIN_BOTH);
    
    task->data[15] = taskId;
    
    return taskId;
}

void ListMenu(void)
{
    struct FieldSpecialListMenu menuList;
    u32 itemsAbove, unknown;
    
    ScriptContext2_Enable();
    
    if (gSpecialVar_0x8000 == LISTMENU_SILPHCO_FLOORS)
    {
        sListMenuLastScrollPosition = sElevatorScroll;
        itemsAbove = sElevatorCursorPos;
        unknown = 0;
    }
    else
    {
        itemsAbove = sListMenuLastScrollPosition = 0;
        unknown = 1;
    }
    menuList.count = gSpecialVar_0x8001;
    menuList.bgId = 0;
    menuList.x = gSpecialVar_0x8003;
    menuList.y = gSpecialVar_0x8004;
    menuList.maxShowed = gSpecialVar_0x8002;
    menuList.unknown = unknown;
    menuList.cursorPos = sListMenuLastScrollPosition;
    menuList.itemsAbove = itemsAbove;
    menuList.baseBlock = MULTICHOICE_DEFAULT_BASE_BLOCK;
    menuList.palNum = 15;
    menuList.windowTileNum = 0;
    InitFieldSpecialListMenu(sListMenuLabels[gSpecialVar_0x8000].list, &menuList);
}

u32 InitFormChangeListMenu(u32 listId)
{
    struct FieldSpecialListMenu menuList =
    {
        .count = sFormChangeMenuLabels[listId].count,
        .bgId = 2,
        .x = 19,
        .y = 1,
        .maxShowed = 7,
        .cursorPos = 0,
        .itemsAbove = 0,
        .baseBlock = 0x280,
        .palNum = 13,
        .windowTileNum = 0x4F,
    };
    return InitFieldSpecialListMenu(sFormChangeMenuLabels[listId].list, &menuList);
}

u32 GetFormChangeListMenuSpecies(u32 listId)
{
    return sFormChangeMenuLabels[listId].forms[gSpecialVar_Result];
}

static void CreateScriptListMenu(void)
{
    sFieldSpecialsListMenuTemplate.items = sListMenuItems;
    sFieldSpecialsListMenuTemplate.moveCursorFunc = ScriptListMenuMoveCursorFunction;
    sFieldSpecialsListMenuTemplate.itemPrintFunc = NULL;
    sFieldSpecialsListMenuTemplate.totalItems = 1;
    sFieldSpecialsListMenuTemplate.maxShowed = 1;
    sFieldSpecialsListMenuTemplate.windowId = 0;
    sFieldSpecialsListMenuTemplate.header_X = 0;
    sFieldSpecialsListMenuTemplate.item_X = 8;
    sFieldSpecialsListMenuTemplate.cursor_X = 0;
    sFieldSpecialsListMenuTemplate.upText_Y = 0;
    sFieldSpecialsListMenuTemplate.cursorPal = 2;
    sFieldSpecialsListMenuTemplate.fillValue = 1;
    sFieldSpecialsListMenuTemplate.cursorShadowPal = 3;
    sFieldSpecialsListMenuTemplate.lettersSpacing = 1;
    sFieldSpecialsListMenuTemplate.itemVerticalPadding = 0;
    sFieldSpecialsListMenuTemplate.scrollMultiple = 0;
    sFieldSpecialsListMenuTemplate.fontId = 2;
    sFieldSpecialsListMenuTemplate.cursorKind = 0;
}

static void ScriptListMenuMoveCursorFunction(s32 nothing, bool32 is, struct ListMenu * used)
{
    u32 taskId;
    struct Task * task;
    
    PlaySE(SE_SELECT);
    
    taskId = FindTaskIdByFunc(Task_ListMenuHandleInput);
    if (taskId != 0xFF)
    {
        task = &gTasks[taskId];
        ListMenuGetScrollAndRow(task->data[14], &sFieldSpecialsListMenuScrollBuffer, NULL);
        sListMenuLastScrollPosition = sFieldSpecialsListMenuScrollBuffer;
    }
}

static void Task_ListMenuHandleInput(u32 taskId)
{
    struct Task * task = &gTasks[taskId];
    s32 input = ListMenu_ProcessInput(task->data[14]);
    
    switch (input)
    {
    case -1:
        break;
    case -2:
        gSpecialVar_Result = 0x7F;
        PlaySE(SE_SELECT);
        Task_DestroyListMenu(taskId, TRUE);
        break;
    default:
        gSpecialVar_Result = input;
        PlaySE(SE_SELECT);
        
        if (task->data[6] == 0 || input == task->data[1] - 1)
            Task_DestroyListMenu(taskId, TRUE);
        else
        {
            Task_ListMenuRemoveScrollIndicatorArrowPair(taskId);
            task->func = Task_SuspendListMenu;
            EnableBothScriptContexts();
        }
        break;
    }
}

void Task_DestroyListMenu(u32 taskId, bool32 enableScripts)
{
    struct Task * task = &gTasks[taskId];
    Task_ListMenuRemoveScrollIndicatorArrowPair(taskId);
    DestroyListMenuTask(task->data[14], NULL, NULL);
    Free(sListMenuItems);
    ClearStdWindowAndFrameToTransparent(task->data[13], TRUE);
    FillWindowPixelBuffer(task->data[13], PIXEL_FILL(0));
    ClearWindowTilemap(task->data[13]);
    CopyWindowToVram(task->data[13], COPYWIN_GFX);
    RemoveWindow(task->data[13]);
    DestroyTask(taskId);
    
    if (enableScripts)
        EnableBothScriptContexts();
}

static void Task_SuspendListMenu(u32 taskId)
{
    switch (gTasks[taskId].data[6])
    {
    case 1:
        break;
    case 2:
        gTasks[taskId].data[6] = 1;
        gTasks[taskId].func = Task_RedrawScrollArrowsAndWaitInput;
        break;
    }
}

void ReturnToListMenu(void)
{
    u32 taskId = FindTaskIdByFunc(Task_SuspendListMenu);
    
    if (taskId == 0xFF)
        EnableBothScriptContexts();
    else
        gTasks[taskId].data[6]++;
}

static void Task_RedrawScrollArrowsAndWaitInput(u32 taskId)
{
    ScriptContext2_Enable();
    Task_CreateMenuRemoveScrollIndicatorArrowPair(taskId);
    gTasks[taskId].func = Task_ListMenuHandleInput;
}

static void Task_CreateMenuRemoveScrollIndicatorArrowPair(u32 taskId)
{
    struct Task * task = &gTasks[taskId];
    struct ScrollArrowsTemplate template = {
        .firstArrowType = 2,
        .secondArrowType = 3,
        .tileTag = 2000,
        .palTag = 100
    };
    
    if (task->data[0] != task->data[1])
    {
        template.firstX = 4 * task->data[4] + 8 * task->data[2];
        template.firstY = 8;
        template.secondX = 4 * task->data[4] + 8 * task->data[2];
        template.secondY = 8 * task->data[5] + 10;
        template.fullyUpThreshold = 0;
        template.fullyDownThreshold = task->data[1] - task->data[0];
        task->data[12] = AddScrollIndicatorArrowPair(&template, &sListMenuLastScrollPosition);
    }
}

static void Task_ListMenuRemoveScrollIndicatorArrowPair(u32 taskId)
{
    struct Task * task = &gTasks[taskId];
    
    if (task->data[0] != task->data[1])
        RemoveScrollIndicatorArrowPair(task->data[12]);
}

void ForcePlayerToStartSurfing(void)
{
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_SURFING);
}

static const u16 sStarterSpecies[] = {
    SPECIES_BULBASAUR,
    SPECIES_SQUIRTLE,
    SPECIES_CHARMANDER
};

static u32 GetStarterSpeciesById(u32 idx)
{
    if (idx >= ARRAY_COUNT(sStarterSpecies))
        idx = 0;
    return sStarterSpecies[idx];
}

u32 GetStarterSpecies(void)
{
    return GetStarterSpeciesById(VarGet(VAR_STARTER_MON));
}

void SetSeenMon(void)
{
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(gSpecialVar_0x8004), FLAG_SET_SEEN);
}

void ResetContextNpcTextColor(void)
{
    gSelectedObjectEvent = 0;
    gSpecialVar_TextColor = 0xFF;
}

u32 ContextNpcGetTextColor(void)
{
    u32 gfxId;
    
    if (gSpecialVar_TextColor != 0xFF)
        return gSpecialVar_TextColor;
    else if (gSelectedObjectEvent == 0)
        return 3;
    else
    {
        gfxId = gObjectEvents[gSelectedObjectEvent].graphicsId;
        if (gfxId >= OBJ_EVENT_GFX_VAR_0)
            gfxId = VarGetObjectEventGraphicsId(gfxId - OBJ_EVENT_GFX_VAR_0);
        
        return GetColorFromTextColorTable(gfxId);
    }
}

void TV_PrintIntToStringVar(u32 varidx, s32 number)
{
    s32 n = CountDigits(number);
    ConvertIntToDecimalStringN(sStringVarPtrs[varidx], number, STR_CONV_MODE_LEFT_ALIGN, n);
}

s32 CountDigits(s32 number)
{
    if (number / 10 == 0)
        return 1;
    else if (number / 100 == 0)
        return 2;
    else if (number / 1000 == 0)
        return 3;
    else if (number / 10000 == 0)
        return 4;
    else if (number / 100000 == 0)
        return 5;
    else if (number / 1000000 == 0)
        return 6;
    else if (number / 10000000 == 0)
        return 7;
    else if (number / 100000000 == 0)
        return 8;
    else
        return 1;
}

bool32 NameRaterWasNicknameChanged(void)
{
    struct Pokemon * pokemon = &gPlayerParty[gSpecialVar_0x8004];
    
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar1);
    
    if (StringCompare(gStringVar3, gStringVar1) == 0)
        return FALSE;
    else
        return TRUE;
}

void ChangeBoxPokemonNickname(void)
{
    struct BoxPokemon * pokemon = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);

    GetBoxMonData(pokemon, MON_DATA_NICKNAME, gStringVar3);
    GetBoxMonData(pokemon, MON_DATA_NICKNAME, gStringVar2);

    DoNamingScreen(NAMING_SCREEN_NAME_RATER, gStringVar2, GetBoxMonData(pokemon, MON_DATA_SPECIES, NULL), GetBoxMonGender(pokemon), ChangeBoxPokemonNickname_CB);
}

static void ChangeBoxPokemonNickname_CB(void)
{
    SetBoxMonNickAt(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos, gStringVar2);
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void ChangePokemonNickname(void)
{
    struct Pokemon * pokemon = &gPlayerParty[gSpecialVar_0x8004];
    
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar3);
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar2);
    
    DoNamingScreen(NAMING_SCREEN_NAME_RATER, gStringVar2, GetMonData(pokemon, MON_DATA_SPECIES, NULL), GetMonGender(pokemon), ChangePokemonNickname_CB);
}

static void ChangePokemonNickname_CB(void)
{
    SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar2);
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void BufferMonNickname(void)
{
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
}

void IsMonOTIDNotPlayers(void)
{
    if (GetPlayerTrainerId() == GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_OT_ID, NULL))
        gSpecialVar_Result = FALSE;
    else
        gSpecialVar_Result = TRUE;
}

u32 GetPlayerTrainerId(void)
{
    return (gSaveBlock2Ptr->playerTrainerId[3] << 24) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0];
}

u32 GetUnlockedSeviiAreas(void)
{
    u32 result = 0;
    
    if (FlagGet(FLAG_WORLD_MAP_ONE_ISLAND))
        result |= 1 << 0;
    if (FlagGet(FLAG_WORLD_MAP_TWO_ISLAND))
        result |= 1 << 1;
    if (FlagGet(FLAG_WORLD_MAP_THREE_ISLAND))
        result |= 1 << 2;
    if (FlagGet(FLAG_WORLD_MAP_FOUR_ISLAND))
        result |= 1 << 3;
    if (FlagGet(FLAG_WORLD_MAP_FIVE_ISLAND))
        result |= 1 << 4;
    if (FlagGet(FLAG_WORLD_MAP_SIX_ISLAND))
        result |= 1 << 5;
    if (FlagGet(FLAG_WORLD_MAP_SEVEN_ISLAND))
        result |= 1 << 6;
    
    return result;
}

void UpdateTrainerCardPhotoIcons(void)
{
    u32 i, partyCount = CalculatePlayerPartyCount();

    for (i = 0; i < partyCount; i++)
        VarSet(VAR_TRAINER_CARD_MON_ICON_1 + i, GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL));
    
    VarSet(VAR_TRAINER_CARD_MON_ICON_TINT_IDX, gSpecialVar_0x8004);
}

u32 StickerManGetBragFlags(void)
{
    u32 result = 0, numEggs = GetGameStat(GAME_STAT_HATCHED_EGGS);
    
    gSpecialVar_0x8004 = GetGameStat(GAME_STAT_ENTERED_HOF);
    gSpecialVar_0x8006 = GetGameStat(GAME_STAT_LINK_BATTLE_WINS);
    
    if (numEggs > 0xFFFF)
        gSpecialVar_0x8005 = 0xFFFF;
    else
        gSpecialVar_0x8005 = numEggs;
    
    if (gSpecialVar_0x8004 != 0)
        result |= 1 << 0;
    if (gSpecialVar_0x8005 != 0)
        result |= 1 << 1;
    if (gSpecialVar_0x8006 != 0)
        result |= 1 << 2;
    
    return result;
}

u16 GetHiddenItemAttr(u32 hiddenItem, u32 attr)
{
    switch (attr)
    {
        case HIDDEN_ITEM_ID:
            return hiddenItem & 0xFFFF;
        case HIDDEN_ITEM_FLAG:
            return ((hiddenItem >> 16) & 0xFF) + 1000;
        case HIDDEN_ITEM_QUANTITY:
            return (hiddenItem >> 24) & 0x7F;
        case HIDDEN_ITEM_UNDERFOOT:
            return (hiddenItem >> 31) & 0x01;
        default:
            return 1;
    }
}

bool32 DoesPlayerPartyContainSpecies(void)
{
    u32 i, partyCount = CalculatePlayerPartyCount();

    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL) == gSpecialVar_0x8004)
            return TRUE;
    }
    return FALSE;
}

void SetPCBoxToSendMon(u32 boxId)
{
    sPCBoxToSendMon = boxId;
}

u32 GetPCBoxToSendMon(void)
{
    return sPCBoxToSendMon;
}

bool32 ShouldShowBoxWasFullMessage(void)
{
    if (FlagGet(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE) || StorageGetCurrentBox() == VarGet(VAR_PC_BOX_TO_SEND_MON))
        return FALSE;

    FlagSet(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
    return TRUE;
}

bool32 IsDestinationBoxFull(void)
{
    s32 i, j;

    SetPCBoxToSendMon(VarGet(VAR_PC_BOX_TO_SEND_MON));
    
    i = StorageGetCurrentBox();
    
    do
    {
        for (j = 0; j < IN_BOX_COUNT; j++)
        {
            if (GetBoxMonData(GetBoxedMonPtr(i, j), MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            {
                if (GetPCBoxToSendMon() != i)
                    FlagClear(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
                
                VarSet(VAR_PC_BOX_TO_SEND_MON, i);
                
                return ShouldShowBoxWasFullMessage();
            }
        }
        i++;
        
        if (i == TOTAL_BOXES_COUNT)
            i = 0;
        
    } while (i != StorageGetCurrentBox());
    
    return FALSE;
}

const u16 sPokeCenter1FMaps[] = {
    MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F,
    MAP_PEWTER_CITY_POKEMON_CENTER_1F,
    MAP_CERULEAN_CITY_POKEMON_CENTER_1F,
    MAP_LAVENDER_TOWN_POKEMON_CENTER_1F,
    MAP_VERMILION_CITY_POKEMON_CENTER_1F,
    MAP_CELADON_CITY_POKEMON_CENTER_1F,
    MAP_FUCHSIA_CITY_POKEMON_CENTER_1F,
    MAP_CINNABAR_ISLAND_POKEMON_CENTER_1F,
    MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F,
    MAP_SAFFRON_CITY_POKEMON_CENTER_1F,
    MAP_ROUTE4_POKEMON_CENTER_1F,
    MAP_ROUTE10_POKEMON_CENTER_1F,
    MAP_ONE_ISLAND_POKEMON_CENTER_1F,
    MAP_TWO_ISLAND_POKEMON_CENTER_1F,
    MAP_THREE_ISLAND_POKEMON_CENTER_1F,
    MAP_FOUR_ISLAND_POKEMON_CENTER_1F,
    MAP_FIVE_ISLAND_POKEMON_CENTER_1F,
    MAP_SEVEN_ISLAND_POKEMON_CENTER_1F,
    MAP_SIX_ISLAND_POKEMON_CENTER_1F,
    MAP_UNION_ROOM,
    MAP_UNDEFINED
};

bool32 UsedPokemonCenterWarp(void)
{
    u32 i;
    u16 mapno = (gLastUsedWarp.mapGroup << 8) + gLastUsedWarp.mapNum;
    
    for (i = 0; sPokeCenter1FMaps[i] != MAP_UNDEFINED; i++)
    {
        if (sPokeCenter1FMaps[i] == mapno)
            return TRUE;
    }
    return FALSE;
}

bool32 BufferTMHMMoveName(void)
{
    // 8004 = item ID
    if (ItemId_GetPocket(gSpecialVar_0x8004) == POCKET_TM_CASE)
    {
        StringCopy(gStringVar1, gBattleMoves[ItemId_GetHoldEffectParam(gSpecialVar_0x8004)].name);
        return TRUE;
    }
    return FALSE;
}

void RunMassageCooldownStepCounter(void)
{
    u32 count = VarGet(VAR_MASSAGE_COOLDOWN_STEP_COUNTER);
    if (count < 500)
        VarSet(VAR_MASSAGE_COOLDOWN_STEP_COUNTER, count + 1);
}

void DaisyMassageServices(void)
{
    AdjustFriendship(&gPlayerParty[gSpecialVar_0x8004], FRIENDSHIP_EVENT_MASSAGE);
    VarSet(VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 0);
}

static const u16 sEliteFourLightingPalettes[][16] = {
    INCBIN_U16("graphics/field_specials/unk_83F5F50.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F5F70.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F5F90.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F5FB0.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F5FD0.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F5FF0.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6010.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6030.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6050.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6070.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6090.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F60B0.gbapal")
};

static const u16 sChampionRoomLightingPalettes[][16] = {
    INCBIN_U16("graphics/field_specials/unk_83F60D0.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F60F0.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6110.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6130.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6150.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6170.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6190.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F61B0.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F61D0.gbapal")
};

static const u8 sEliteFourLightingTimers[] = {
    40,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12
};

static const u8 sChampionRoomLightingTimers[] = {
    20,
     8,
     8,
     8,
     8,
     8,
     8,
     8
};

void DoPokemonLeagueLightingEffect(void)
{
    u32 taskId = CreateTask(Task_RunPokemonLeagueLightingEffect, 8);
    s16 *data = gTasks[taskId].data;
    
    if (FlagGet(FLAG_TEMP_3))
        gTasks[taskId].func = Task_CancelPokemonLeagueLightingEffect;
    else
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(POKEMON_LEAGUE_CHAMPIONS_ROOM) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(POKEMON_LEAGUE_CHAMPIONS_ROOM))
        {
            data[0] = sChampionRoomLightingTimers[0];
            data[2] = 8;
            LoadPalette(sChampionRoomLightingPalettes[0], 0x70, 0x20);
        }
        else
        {
            data[0] = sEliteFourLightingTimers[0];
            data[2] = 11;
            LoadPalette(sEliteFourLightingPalettes[0], 0x70, 0x20);
        }
        data[1] = 0;
        Fieldmap_ApplyGlobalTintToPaletteSlot(7, 1);
    }
}

static void Task_RunPokemonLeagueLightingEffect(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (!gPaletteFade.active && FlagGet(FLAG_TEMP_2) && !FlagGet(FLAG_TEMP_5) && --data[0] == 0)
    {
        if (++data[1] == data[2])
            data[1] = 0;

        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(POKEMON_LEAGUE_CHAMPIONS_ROOM) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(POKEMON_LEAGUE_CHAMPIONS_ROOM))
        {
            data[0] = sChampionRoomLightingTimers[data[1]];
            LoadPalette(sChampionRoomLightingPalettes[data[1]], 0x70, 0x20);
        }
        else
        {
            data[0] = sEliteFourLightingTimers[data[1]];
            LoadPalette(sEliteFourLightingPalettes[data[1]], 0x70, 0x20);
        }
        Fieldmap_ApplyGlobalTintToPaletteSlot(7, 1);
    }
}

static void Task_CancelPokemonLeagueLightingEffect(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (FlagGet(FLAG_TEMP_4))
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(POKEMON_LEAGUE_CHAMPIONS_ROOM) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(POKEMON_LEAGUE_CHAMPIONS_ROOM))
            LoadPalette(sChampionRoomLightingPalettes[8], 0x70, 0x20);
        else
            LoadPalette(sEliteFourLightingPalettes[11], 0x70, 0x20);

        Fieldmap_ApplyGlobalTintToPaletteSlot(7, 1);
        
        if (gPaletteFade.active)
            BlendPalettes(0x00000080, 16, RGB_BLACK);

        DestroyTask(taskId);
    }
}

void StopPokemonLeagueLightingEffectTask(void)
{
    u32 taskId = FindTaskIdByFunc(Task_RunPokemonLeagueLightingEffect);
    
    if (taskId != 0xFF)
        DestroyTask(taskId);
}

const struct CapeBrinkTutor gCapeBrinkCompatibleSpecies[3] =
{
    {SPECIES_VENUSAUR, MOVE_FRENZY_PLANT, FLAG_TUTOR_FRENZY_PLANT},
    {SPECIES_CHARIZARD, MOVE_BLAST_BURN, FLAG_TUTOR_BLAST_BURN},
    {SPECIES_BLASTOISE, MOVE_HYDRO_CANNON, FLAG_TUTOR_HYDRO_CANNON}
};

bool32 CapeBrinkGetMoveToTeachLeadPokemon(void)
{
    // Returns:
    //   8005 = Move index
    //   8006 = Num moves known by lead mon
    //   8007 = Index of lead mon
    //   to specialvar = whether a move can be taught in the first place
    u32 i, j, numMovesKnown = 0;
    u32 leadMonSlot = GetLeadMonIndex();
    
    gSpecialVar_0x8007 = leadMonSlot;
    
    if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_FRIENDSHIP) == 255)
    {
        for (i = 0; i < ARRAY_COUNT(gCapeBrinkCompatibleSpecies); i++)
        {
            if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_SPECIES2, NULL) == gCapeBrinkCompatibleSpecies[i].species)
            {
                u32 move = gCapeBrinkCompatibleSpecies[i].move;
                
                StringCopy(gStringVar2, gBattleMoves[move].name);
                gSpecialVar_0x8005 = move;
                
                if (!FlagGet(gCapeBrinkCompatibleSpecies[i].flagId))
                {
                    for (j = 0; j < MAX_MON_MOVES; j++)
                    {
                        if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_MOVE1 + j))
                            ++numMovesKnown;
                    }
                    gSpecialVar_0x8006 = numMovesKnown;
                    
                    return TRUE;
                }
                break;
            }
        }
    }
    return FALSE;
}

bool32 HasLearnedAllMovesFromCapeBrinkTutor(void)
{
    // 8005 is set by CapeBrinkGetMoveToTeachLeadPokemon
    u32 i, num = 0;
    
    for (i = 0; i < ARRAY_COUNT(gCapeBrinkCompatibleSpecies); i++)
    {
        u32 flagId = gCapeBrinkCompatibleSpecies[i].flagId;
        
        if (gSpecialVar_0x8005 == gCapeBrinkCompatibleSpecies[i].move)
            FlagSet(flagId);
        
        if (FlagGet(flagId))
            ++num;
    }
    return (num == ARRAY_COUNT(gCapeBrinkCompatibleSpecies));
}

bool32 CutMoveRuinValleyCheck(void)
{
    if (FlagGet(FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE) != TRUE
     && gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(SIX_ISLAND_RUIN_VALLEY)
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(SIX_ISLAND_RUIN_VALLEY)
     && gSaveBlock1Ptr->pos.x == 24
     && gSaveBlock1Ptr->pos.y == 25
     && GetPlayerFacingDirection() == DIR_NORTH)
        return TRUE;
    else
        return FALSE;
}

void CutMoveOpenDottedHoleDoor(void)
{
    MapGridSetMetatileIdAt(31, 31, METATILE_SeviiIslands67_DottedHoleDoor_Open);
    DrawWholeMapView();
    PlaySE(SE_BANG);
    FlagSet(FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE);
    ScriptContext2_Disable();
}

static const u16 sDeoxysObjectPals[][16] = {
    INCBIN_U16("graphics/field_specials/unk_83F6206.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6226.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6246.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6266.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6286.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F62A6.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F62C6.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F62E6.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6306.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6326.gbapal"),
    INCBIN_U16("graphics/field_specials/unk_83F6346.gbapal")
};

static const u8 sDeoxysCoords[][2] = {
    {15, 12},
    {11, 14},
    {15,  8},
    {19, 14},
    {12, 11},
    {18, 11},
    {15, 14},
    {11, 14},
    {19, 14},
    {15, 15},
    {15, 10}
};

static const u8 sDeoxysStepCaps[] = {
    4,
    8,
    8,
    8,
    4,
    4,
    4,
    6,
    3,
    3
};

void DoDeoxysTriangleInteraction(void)
{
    CreateTask(Task_DoDeoxysTriangleInteraction, 8);
}

static void Task_DoDeoxysTriangleInteraction(u32 taskId)
{
    u16 r5;
    u16 r6;
    
    if (FlagGet(FLAG_SYS_DEOXYS_AWAKENED) == TRUE)
    {
        gSpecialVar_Result = 3;
        EnableBothScriptContexts();
        DestroyTask(taskId);
    }
    else
    {
        r5 = VarGet(VAR_DEOXYS_INTERACTION_NUM);
        r6 = VarGet(VAR_DEOXYS_INTERACTION_STEP_COUNTER);
        
        VarSet(VAR_DEOXYS_INTERACTION_STEP_COUNTER, 0);
        
        if (r5 != 0 && sDeoxysStepCaps[r5 - 1] < r6)
        {
            MoveDeoxysObject(0);
            VarSet(VAR_DEOXYS_INTERACTION_NUM, 0);
            gSpecialVar_Result = 0;
            DestroyTask(taskId);
        }
        else if (r5 == 10)
        {
            FlagSet(FLAG_SYS_DEOXYS_AWAKENED);
            gSpecialVar_Result = 2;
            EnableBothScriptContexts();
            DestroyTask(taskId);
        }
        else
        {
            r5++;
            MoveDeoxysObject(r5);
            VarSet(VAR_DEOXYS_INTERACTION_NUM, r5);
            gSpecialVar_Result = 1;
            DestroyTask(taskId);
        }
    }
}

static void MoveDeoxysObject(u32 num)
{
    u8 mapObjId;
    
    LoadPalette(sDeoxysObjectPals[num], 0x1A0, 0x08);
    ApplyGlobalFieldPaletteTint(10);
    TryGetObjectEventIdByLocalIdAndMap(LOCALID_BIRTH_ISLAND_EXTERIOR_ROCK, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &mapObjId);
    
    if (num == 0)
        PlaySE(SE_M_CONFUSE_RAY);
    else
        PlaySE(SE_DEOXYS_MOVE);
    
    CreateTask(Task_WaitDeoxysFieldEffect, 8);
    gFieldEffectArguments[0] = LOCALID_BIRTH_ISLAND_EXTERIOR_ROCK;
    gFieldEffectArguments[1] = MAP_NUM(BIRTH_ISLAND_EXTERIOR);
    gFieldEffectArguments[2] = MAP_GROUP(BIRTH_ISLAND_EXTERIOR);
    gFieldEffectArguments[3] = sDeoxysCoords[num][0];
    gFieldEffectArguments[4] = sDeoxysCoords[num][1];
    
    if (num == 0)
        gFieldEffectArguments[5] = 60;
    else
        gFieldEffectArguments[5] = 5;
    
    FieldEffectStart(FLDEFF_MOVE_DEOXYS_ROCK);
    Overworld_SetMapObjTemplateCoords(LOCALID_BIRTH_ISLAND_EXTERIOR_ROCK, sDeoxysCoords[num][0], sDeoxysCoords[num][1]);
}

static void Task_WaitDeoxysFieldEffect(u32 taskId)
{
    if (!FieldEffectActiveListContains(FLDEFF_MOVE_DEOXYS_ROCK))
    {
        EnableBothScriptContexts();
        DestroyTask(taskId);
    }
}

void IncrementBirthIslandRockStepCount(void)
{
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(BIRTH_ISLAND_EXTERIOR) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(BIRTH_ISLAND_EXTERIOR))
    {
        u16 count = VarGet(VAR_DEOXYS_INTERACTION_STEP_COUNTER);
        count++;
        VarSet(VAR_DEOXYS_INTERACTION_STEP_COUNTER, count > 99 ? 0 : count);
    }
}

void SetDeoxysTrianglePalette(void)
{
    LoadPalette(sDeoxysObjectPals[VarGet(VAR_DEOXYS_INTERACTION_NUM)], 0x1A0, 0x08);
    ApplyGlobalFieldPaletteTint(10);
}

void BrailleCursorToggle(void)
{
    // 8004 = x - 27
    // 8005 = y
    // 8006 = action (0 = create, 1 = delete)
    u16 x = gSpecialVar_0x8004 + 27;
    
    if (gSpecialVar_0x8006 == 0)
        sBrailleTextCursorSpriteID = CreateTextCursorSpriteForOakSpeech(0, x, gSpecialVar_0x8005, 0, 0);
    else
        DestroyTextCursorSprite(sBrailleTextCursorSpriteID);
}

bool32 PlayerPartyContainsSpeciesWithPlayerID(void)
{
    // 8004 = species
    u32 i, playerCount = CalculatePlayerPartyCount();
    
    for (i = 0; i < playerCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL) == gSpecialVar_0x8004 && GetPlayerTrainerId() == GetMonData(&gPlayerParty[i], MON_DATA_OT_ID, NULL))
            return TRUE;
    }
    return FALSE;
}

/*
 * Determines which of Lorelei's doll collection to show
 * based on how many times you've entered the Hall of Fame.
 */
void UpdateLoreleiDollCollection(void)
{
    u32 numHofClears = GetGameStat(GAME_STAT_ENTERED_HOF);
    
    if (numHofClears >= 25)
    {
        FlagClear(FLAG_HIDE_LORELEI_HOUSE_MEOWTH_DOLL);
        if (numHofClears >= 50)
            FlagClear(FLAG_HIDE_LORELEI_HOUSE_CHANSEY_DOLL);
        if (numHofClears >= 75)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_NIDORAN_F_DOLL);
        if (numHofClears >= 100)
            FlagClear(FLAG_HIDE_LORELEI_HOUSE_JIGGLYPUFF_DOLL);
        if (numHofClears >= 125)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_NIDORAN_M_DOLL);
        if (numHofClears >= 150)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_FEAROW_DOLL);
        if (numHofClears >= 175)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_PIDGEOT_DOLL);
        if (numHofClears >= 200)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_LAPRAS_DOLL);
    }
}

void LoopWingFlapSound(void)
{
    // 8004 = Num flaps
    // 8005 = Frame delay between flaps
    CreateTask(Task_WingFlapSound, 8);
    PlaySE(SE_M_WING_ATTACK);
}

static void Task_WingFlapSound(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    data[1]++;
    
    if (data[1] == gSpecialVar_0x8005)
    {
        data[0]++;
        data[1] = 0;
        PlaySE(SE_M_WING_ATTACK);
    }
    if (data[0] == gSpecialVar_0x8004 - 1)
        DestroyTask(taskId);
}

void ChooseItemFromBag(void)
{
    u32 pocket = VarGet(VAR_TEMP_0);
    
    PlayRainStoppingSoundEffect();
    
    switch (pocket)
    {
        case POCKET_TM_CASE:
            InitTMCase(TMCASE_CHOOSE_ITEM, CB2_ReturnToFieldContinueScript, 0);
            break;
        case POCKET_BERRY_POUCH:
            InitBerryPouch(BERRYPOUCH_CHOOSE_ITEM, CB2_ReturnToFieldContinueScript, 0);
            break;
        default:
            GoToBagMenu(ITEMMENULOCATION_CHOOSE_ITEM, pocket - 1, CB2_ReturnToFieldContinueScript);
            break;
    }
}

void EnterHallOfFame(void)
{
    HealPlayerParty();
    
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
        gHasHallOfFameRecords = TRUE;
    else
    {
        gHasHallOfFameRecords = FALSE;
        FlagSet(FLAG_SYS_GAME_CLEAR);
    }
    if (GetGameStat(GAME_STAT_FIRST_HOF_PLAY_TIME) == 0)
        SetGameStat(GAME_STAT_FIRST_HOF_PLAY_TIME, (gSaveBlock2Ptr->playTimeHours << 16) | (gSaveBlock2Ptr->playTimeMinutes << 8) | gSaveBlock2Ptr->playTimeSeconds);

    SetContinueGameWarpStatus();
    SetContinueGameWarpToHealLocation(SPAWN_PALLET_TOWN);
    SetMainCallback2(CB2_DoHallOfFameScreen);
}

void SetCB2WhiteOut(void)
{
    SetMainCallback2(CB2_WhiteOut);
}

bool32 GetPokedexCount(void)
{
    if (gSpecialVar_0x8004 == 0)
    {
        gSpecialVar_0x8005 = GetCurrentRegionPokedexCount(FLAG_GET_SEEN);
        gSpecialVar_0x8006 = GetCurrentRegionPokedexCount(FLAG_GET_CAUGHT);
    }
    else
    {
        gSpecialVar_0x8005 = GetNationalPokedexCount(FLAG_GET_SEEN);
        gSpecialVar_0x8006 = GetNationalPokedexCount(FLAG_GET_CAUGHT);
    }
    return FlagGet(FLAG_SYS_POKEDEX_GET);
}

bool32 HasAllKantoMons(void)
{
    return HasAllRegionMons(REGION_KANTO);
}

static const u8 *GetProfOaksRatingMessageByCount(u32 count)
{
    gSpecialVar_Result = FALSE;

    if (count < 10)
        return PokedexRating_Text_LessThan10;
    else if (count < 20)
        return PokedexRating_Text_LessThan20;
    else if (count < 30)
        return PokedexRating_Text_LessThan30;
    else if (count < 40)
        return PokedexRating_Text_LessThan40;
    else if (count < 50)
        return PokedexRating_Text_LessThan50;
    else if (count < 60)
        return PokedexRating_Text_LessThan60;
    else if (count < 70)
        return PokedexRating_Text_LessThan70;
    else if (count < 80)
        return PokedexRating_Text_LessThan80;
    else if (count < 90)
        return PokedexRating_Text_LessThan90;
    else if (count < 100)
        return PokedexRating_Text_LessThan100;
    else if (count < 110)
        return PokedexRating_Text_LessThan110;
    else if (count < 120)
        return PokedexRating_Text_LessThan120;
    else if (count < 130)
        return PokedexRating_Text_LessThan130;
    else if (count < 140)
        return PokedexRating_Text_LessThan140;
    else if (count < 150)
        return PokedexRating_Text_LessThan150;
    else if (count == 150)
    {
        // Mew doesn't count for completing the pokedex
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_MEW), FLAG_GET_CAUGHT))
            return PokedexRating_Text_LessThan150;

        gSpecialVar_Result = TRUE;
        return PokedexRating_Text_Complete;
    }
    else if (count == 151)
    {
        gSpecialVar_Result = TRUE;
        return PokedexRating_Text_Complete;
    }
    return PokedexRating_Text_LessThan10;
}

void GetProfOaksRatingMessage(void)
{
    ShowFieldMessage(GetProfOaksRatingMessageByCount(gSpecialVar_0x8004));
}

void LoadSymbolsIconGraphics(void)
{
    LoadCompressedSpriteSheet(&s8x8SymbolsSpriteSheet);
    LoadSpritePalette(&s8x8SymbolsSpritePal);
}

void FreeSymbolsIconGraphics(void)
{
    FreeSpriteTilesByTag(TAG_8x8_SYMBOLS);
    FreeSpritePaletteByTag(TAG_8x8_SYMBOLS);
}

u32 Create8x8SymbolSprite(s16 x, s16 y, u32 subpriority, u32 symbolId)
{
    u32 spriteId = CreateSprite(&s8x8SymbolSpriteTemplate, x, y, subpriority);
    StartSpriteAnim(&gSprites[spriteId], symbolId);
    return spriteId;
}

static void Task_WaitFadeOutAndShowMoveTutor(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        gFieldCallback = FieldCB_ContinueScriptHandleMusic;
        ShowMoveTutorMenu(FALSE);
    }
}

void DisplayMoveTutorMenu(void)
{
    ScriptContext2_Enable();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_WaitFadeOutAndShowMoveTutor, 10);
}

void EnablePlayerBag(void)
{
    FlagSet(FLAG_SYS_BAG_ENABLED);
}

void DisablePlayerBag(void)
{
    FlagClear(FLAG_SYS_BAG_ENABLED);
}
