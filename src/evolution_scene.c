#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_message.h"
#include "data.h"
#include "decompress.h"
#include "evolution_scene.h"
#include "battle_gfx_sfx_util.h"
#include "evolution_graphics.h"
#include "link.h"
#include "link_rfu.h"
#include "m4a.h"
#include "evolution.h"
#include "event_data.h"
#include "trade_scene.h"
#include "new_menu_helpers.h"
#include "menu.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon_summary_screen.h"
#include "scanline_effect.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "trig.h"
#include "constants/battle_string_ids.h"
#include "constants/songs.h"
#include "constants/pokemon.h"

struct EvoInfo
{
    u8 preEvoSpriteId;
    u8 postEvoSpriteId;
    u8 evoTaskId;
    u8 field_3;
    u16 savedPalette[48];
};

// EWRAM vars
static EWRAM_DATA struct EvoInfo *sEvoStructPtr = NULL;
static EWRAM_DATA u16 *sEvoMovingBgPtr = NULL;

// IWRAM common
void (*gCB2_AfterEvolution)(void);

#define sEvoCursorPos           gBattleCommunication[CURSOR_POSITION] // when learning a new move
#define sEvoGraphicsTaskId      gBattleCommunication[SPRITES_INIT_STATE2]

// this file's functions
static void Task_EvolutionScene(u8 taskId);
static void Task_TradeEvolutionScene(u8 taskId);
static void CB2_EvolutionSceneUpdate(void);
static void CB2_TradeEvolutionSceneUpdate(void);
static void EvoDummyFunc(void);
static void VBlankCB_EvolutionScene(void);
static void DestroyMovingBackgroundTasks(void);
static void InitMovingBackgroundTask(bool8 isLink);
static void Task_MovingBackgroundPos(u8 taskId);
static void ResetBgRegsAfterMovingBackgroundCancel(void);

// const data
static const u32 sMovingBackgroundTiles[] = INCBIN_U32("graphics/evolution_scene/bg.4bpp.lz");
static const u32 sMovingBackgroundMap1[] = INCBIN_U32("graphics/evolution_scene/bg.bin.lz");
static const u32 sMovingBackgroundMap2[] = INCBIN_U32("graphics/evolution_scene/bg2.bin.lz");
static const u16 sBlackPalette[] = INCBIN_U16("graphics/evolution_scene/gray_transition_intro.gbapal");
static const u16 sMovingBgPals[] = INCBIN_U16("graphics/evolution_scene/transition.gbapal");

// start frame, stop frame, loop count, delay
static const u8 sMovingBackgroundTimers[][4] =
{
    {  0, 12, 1, 6 },
    { 13, 36, 5, 2 },
    { 13, 24, 1, 2 },
    { 37, 49, 1, 6 },
};

static const u8 sMovingBgPalIndices[][16] = {
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  0,  0 },
    {  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0 },
    {  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,  0,  0 },
    {  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0, 11,  0,  0 },
    {  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0 },
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,  0,  0 },
    {  0,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 12,  0,  0 },
    {  0,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 12, 11,  0,  0 },
    {  0,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 12, 11, 10,  0,  0 },
    {  0,  5,  6,  7,  8,  9, 10, 11, 12, 13, 12, 11, 10,  9,  0,  0 },
    {  0,  6,  7,  8,  9, 10, 11, 12, 13, 12, 11, 10,  9,  8,  0,  0 },
    {  0,  7,  8,  9, 10, 11, 12, 13, 12, 11, 10,  9,  8,  7,  0,  0 },
    {  0,  8,  9, 10, 11, 12, 13, 12, 11, 10,  9,  8,  7,  6,  0,  0 },
    {  0,  9, 10, 11, 12, 13, 12, 11, 10,  9,  8,  7,  6,  5,  0,  0 },
    {  0, 10, 11, 12, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  0,  0 },
    {  0, 11, 12, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  0,  0 },
    {  0, 12, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  0,  0 },
    {  0, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0 },
    {  0, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  2,  0,  0 },
    {  0, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  2,  3,  0,  0 },
    {  0, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  2,  3,  4,  0,  0 },
    {  0,  9,  8,  7,  6,  5,  4,  3,  2,  1,  2,  3,  4,  5,  0,  0 },
    {  0,  8,  7,  6,  5,  4,  3,  2,  1,  2,  3,  4,  5,  6,  0,  0 },
    {  0,  7,  6,  5,  4,  3,  2,  1,  2,  3,  4,  5,  6,  7,  0,  0 },
    {  0,  6,  5,  4,  3,  2,  1,  2,  3,  4,  5,  6,  7,  8,  0,  0 },
    {  0,  5,  4,  3,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0 },
    {  0,  4,  3,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,  0,  0 },
    {  0,  3,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  0,  0 },
    {  0,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0 },
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13,  0,  0 },
    {  0, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0 },
    {  0, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0 },
    {  0, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0 },
    {  0,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0 },
    {  0,  8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

static void CB2_BeginEvolutionScene(void)
{
    UpdatePaletteFade();
    RunTasks();
}

#define tState              data[0]
#define tPreEvoSpecies      data[1]
#define tPostEvoSpecies     data[2]
#define tBits               data[3]
#define tLearnsFirstMove    data[4]
#define tLearnMoveState     data[6]
#define tData7              data[7]
#define tData8              data[8]
#define tEvoWasStopped      data[9]
#define tPartyId            data[10]

static void Task_BeginEvolutionScene(u8 taskId)
{
	struct Task *task;
    switch (gTasks[taskId].tState)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].tState++;
        break;
    case 1:
        if (!gPaletteFade.active)
        {
			task = &gTasks[taskId];
            DestroyTask(taskId);
            EvolutionScene(&gPlayerParty[task->tPartyId], task->tPostEvoSpecies, task->tBits, task->tPartyId);
        }
        break;
    }
}

// Handle evolving by using a evolution stone or leveled up by Rare Candy/Exp. Candies
void BeginEvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, bool8 canStopEvo, u8 partyId)
{
    u8 taskId = CreateTask(Task_BeginEvolutionScene, 0);
    gTasks[taskId].tState = 0;
    gTasks[taskId].tPostEvoSpecies = speciesToEvolve;
    gTasks[taskId].tBits = canStopEvo ? TASK_BIT_CAN_STOP : 0;
    gTasks[taskId].tPartyId = partyId;
    SetMainCallback2(CB2_BeginEvolutionScene);
}

// Do the evolution scene
void EvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, u8 bits, u8 partyId)
{
    u8 name[20];
    u16 currSpecies;
	bool8 isShiny;
    u8 id;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void*)(VRAM), VRAM_SIZE);

    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WIN1H, 0);
    SetGpuReg(REG_OFFSET_WIN1V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);

    ResetPaletteFade();

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 256;
    gBattle_BG3_Y = 0;

    gBattleTerrain = BATTLE_TERRAIN_PLAIN;

    InitBattleBgsVideo();
    LoadBattleTextboxAndBackground();
    ResetSpriteData();
    ScanlineEffect_Stop();
    ResetTasks();
    FreeAllSpritePalettes();

    gReservedSpritePaletteCount = 4;

    sEvoStructPtr = AllocZeroed(sizeof(struct EvoInfo));
    AllocateMonSpritesGfx();

    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringCopy_Nickname(gStringVar1, name);
    StringCopy(gStringVar2, gSpeciesInfo[speciesToEvolve].name);

    // preEvo sprite
    currSpecies = GetMonData(mon, MON_DATA_SPECIES);
    isShiny = GetMonData(mon, MON_DATA_IS_SHINY);
    LZDecompressWram(gSpeciesInfo[currSpecies].frontPic, gMonSpritesGfxPtr->sprites[1]);
    LoadCompressedPalette(GetMonSpritePalFromSpecies(currSpecies, isShiny), 0x110, 0x20);

    SetMultiuseSpriteTemplateToPokemon(currSpecies, 1);
    gMultiuseSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sEvoStructPtr->preEvoSpriteId = id = CreateSprite(&gMultiuseSpriteTemplate, 120, 64, 30);

    gSprites[id].callback = SpriteCallbackDummy2;
    gSprites[id].oam.paletteNum = 1;
    gSprites[id].invisible = TRUE;

    // postEvo sprite
    LZDecompressWram(gSpeciesInfo[speciesToEvolve].frontPic, gMonSpritesGfxPtr->sprites[3]);
    LoadCompressedPalette(GetMonSpritePalFromSpecies(speciesToEvolve, isShiny), 0x120, 0x20);

    SetMultiuseSpriteTemplateToPokemon(speciesToEvolve, 3);
    gMultiuseSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sEvoStructPtr->postEvoSpriteId = id = CreateSprite(&gMultiuseSpriteTemplate, 120, 64, 30);
    gSprites[id].callback = SpriteCallbackDummy2;
    gSprites[id].oam.paletteNum = 2;
    gSprites[id].invisible = TRUE;

    LoadEvoSparkleSpriteAndPal();

    sEvoStructPtr->evoTaskId = id = CreateTask(Task_EvolutionScene, 0);
    gTasks[id].tState = 0;
    gTasks[id].tPreEvoSpecies = currSpecies;
    gTasks[id].tPostEvoSpecies = speciesToEvolve;
    gTasks[id].tBits = bits;
    gTasks[id].tLearnsFirstMove = TRUE;
    gTasks[id].tEvoWasStopped = FALSE;
    gTasks[id].tPartyId = partyId;

    memcpy(&sEvoStructPtr->savedPalette, &gPlttBufferUnfaded[0x20], 0x60);

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_1D_MAP);

    SetHBlankCallback(EvoDummyFunc);
    SetVBlankCallback(VBlankCB_EvolutionScene);
    m4aMPlayAllStop();
    SetMainCallback2(CB2_EvolutionSceneUpdate);
}

static void CB2_EvolutionSceneLoadGraphics(void)
{
    u8 id;
    u16 postEvoSpecies;
    struct Pokemon* Mon;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void*)(VRAM), VRAM_SIZE);

    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WIN1H, 0);
    SetGpuReg(REG_OFFSET_WIN1V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);

    ResetPaletteFade();

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 256;
    gBattle_BG3_Y = 0;

    gBattleTerrain = BATTLE_TERRAIN_PLAIN;

    InitBattleBgsVideo();
    LoadBattleTextboxAndBackground();
    ResetSpriteData();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
	
	Mon = &gPlayerParty[gTasks[sEvoStructPtr->evoTaskId].tPartyId];
	postEvoSpecies = gTasks[sEvoStructPtr->evoTaskId].tPostEvoSpecies;
	
    LZDecompressWram(gSpeciesInfo[postEvoSpecies].frontPic, gMonSpritesGfxPtr->sprites[3]);
    LoadCompressedPalette(GetMonSpritePalFromSpecies(postEvoSpecies, GetMonData(Mon, MON_DATA_IS_SHINY)), 0x120, 0x20);

    SetMultiuseSpriteTemplateToPokemon(postEvoSpecies, 3);
    gMultiuseSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sEvoStructPtr->postEvoSpriteId = id = CreateSprite(&gMultiuseSpriteTemplate, 120, 64, 30);

    gSprites[id].callback = SpriteCallbackDummy2;
    gSprites[id].oam.paletteNum = 2;

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_1D_MAP);

    SetHBlankCallback(EvoDummyFunc);
    SetVBlankCallback(VBlankCB_EvolutionScene);
    SetMainCallback2(CB2_EvolutionSceneUpdate);

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

static void CB2_TradeEvolutionSceneLoadGraphics(void)
{
    struct Pokemon* Mon;
    u16 postEvoSpecies = gTasks[sEvoStructPtr->evoTaskId].tPostEvoSpecies;

    switch (gMain.state)
    {
    case 0:
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetHBlankCallback(NULL);
        SetVBlankCallback(NULL);
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 4;
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 0;
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;
        gBattle_BG2_X = 0;
        gBattle_BG2_Y = 0;
        gBattle_BG3_X = 256;
        gBattle_BG3_Y = 0;
        gMain.state++;
        break;
    case 1:
        ResetPaletteFade();
        SetHBlankCallback(EvoDummyFunc);
        SetVBlankCallback(VBlankCB_EvolutionScene);
        gMain.state++;
        break;
    case 2:
        LoadTradeAnimGfx();
        gMain.state++;
        break;
    case 3:
        FillBgTilemapBufferRect(1, 0, 0, 0, 0x20, 0x20, 0x11);
        CopyBgTilemapBufferToVram(1);
        gMain.state++;
        break;
    case 4:
        {
			Mon = &gPlayerParty[gTasks[sEvoStructPtr->evoTaskId].tPartyId];
			
            LZDecompressWram(gSpeciesInfo[postEvoSpecies].frontPic, gMonSpritesGfxPtr->sprites[3]);
            LoadCompressedPalette(GetMonSpritePalFromSpecies(postEvoSpecies, GetMonData(Mon, MON_DATA_IS_SHINY)), 0x120, 0x20);
            gMain.state++;
        }
        break;
    case 5:
        {
            u8 id;

            SetMultiuseSpriteTemplateToPokemon(postEvoSpecies, 1);
            gMultiuseSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
            sEvoStructPtr->postEvoSpriteId = id = CreateSprite(&gMultiuseSpriteTemplate, 120, 64, 30);

            gSprites[id].callback = SpriteCallbackDummy2;
            gSprites[id].oam.paletteNum = 2;
            gMain.state++;
            LinkTradeDrawWindow();
        }
        break;
    case 6:
        if (gWirelessCommType)
        {
            LoadWirelessStatusIndicatorSpriteGfx();
            CreateWirelessStatusIndicatorSprite(0, 0);
        }
        BlendPalettes(PALETTES_ALL, 0x10, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        InitTradeSequenceBgGpuRegs();
        ShowBg(0);
        ShowBg(1);
        SetMainCallback2(CB2_TradeEvolutionSceneUpdate);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_OBJ_1D_MAP);
        break;
    }
}

// Handle evolving by trading it with another Pokémon
void TradeEvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, u8 preEvoSpriteId, u8 partyId)
{
    u8 name[20];
    u8 id;

    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringCopy_Nickname(gStringVar1, name);
    StringCopy(gStringVar2, gSpeciesInfo[speciesToEvolve].name);

    gAffineAnimsDisabled = TRUE;

    // preEvo sprite
    sEvoStructPtr = AllocZeroed(sizeof(struct EvoInfo));
    sEvoStructPtr->preEvoSpriteId = preEvoSpriteId;

    LZDecompressWram(gSpeciesInfo[speciesToEvolve].frontPic, gMonSpritesGfxPtr->sprites[1]);

    LoadCompressedPalette(GetMonSpritePalFromSpecies(speciesToEvolve, GetMonData(mon, MON_DATA_IS_SHINY)), 0x120, 0x20);

    SetMultiuseSpriteTemplateToPokemon(speciesToEvolve, 1);
    gMultiuseSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sEvoStructPtr->postEvoSpriteId = id = CreateSprite(&gMultiuseSpriteTemplate, 120, 64, 30);

    gSprites[id].callback = SpriteCallbackDummy2;
    gSprites[id].oam.paletteNum = 2;
    gSprites[id].invisible = TRUE;

    LoadEvoSparkleSpriteAndPal();

    sEvoStructPtr->evoTaskId = id = CreateTask(Task_TradeEvolutionScene, 0);
    gTasks[id].tState = 0;
    gTasks[id].tPreEvoSpecies = GetMonData(mon, MON_DATA_SPECIES);
    gTasks[id].tPostEvoSpecies = speciesToEvolve;
    gTasks[id].tLearnsFirstMove = TRUE;
    gTasks[id].tEvoWasStopped = FALSE;
    gTasks[id].tPartyId = partyId;

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 256;
    gBattle_BG3_Y = 0;

    gTextFlags.useAlternateDownArrow = TRUE;

    SetVBlankCallback(VBlankCB_EvolutionScene);
    SetMainCallback2(CB2_TradeEvolutionSceneUpdate);
}

static void CB2_EvolutionSceneUpdate(void)
{
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();
}

static void CB2_TradeEvolutionSceneUpdate(void)
{
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();
}

static void TryMonCreationEvolution(u16 preEvoSpecies, struct Pokemon* mon)
{
	u8 i;
	const u8 *evolutions = gSpeciesInfo[preEvoSpecies].evolutions;
	
    if (evolutions != NULL && gPlayerPartyCount < PARTY_SIZE)
    {
		while (*evolutions != EVOLUTIONS_END)
		{
			if (*evolutions != EVO_CREATE_SPECIES)
				evolutions += gEvolutionCmdArgumentsSize[*evolutions] + 1;
			else
			{
				u32 data = 0;
				u16 natDexNum, species = READ_16(evolutions + 1);
				struct Pokemon* newMon = &gPlayerParty[gPlayerPartyCount];
				
				CopyMon(newMon, mon, sizeof(struct Pokemon));
                SetMonData(newMon, MON_DATA_SPECIES, &species);
                SetMonData(newMon, MON_DATA_NICKNAME, gSpeciesInfo[species].name);
                SetMonData(newMon, MON_DATA_HELD_ITEM, &data);
                SetMonData(newMon, MON_DATA_MARKINGS, &data);
                SetMonData(newMon, MON_DATA_STATUS_ID, &data);
				SetMonData(newMon, MON_DATA_STATUS_COUNTER, &data);
                data = 0xFF;
                SetMonData(newMon, MON_DATA_MAIL, &data);
		        
                CalculateMonStats(newMon);
                CalculatePlayerPartyCount();
		        
		        natDexNum = SpeciesToNationalPokedexNum(species);
                GetSetPokedexFlag(natDexNum, FLAG_SET_SEEN);
                GetSetPokedexFlag(natDexNum, FLAG_SET_CAUGHT);
				
				break;
			}
		}
    }
}

static void Task_EvolutionScene(u8 taskId)
{
    u32 var;
    struct Pokemon* mon = &gPlayerParty[gTasks[taskId].tPartyId];

    // check if B Button was held, so the evolution gets stopped
    if (gMain.heldKeys == B_BUTTON && gTasks[taskId].tState == 8 && gTasks[sEvoGraphicsTaskId].isActive && gTasks[taskId].tBits & TASK_BIT_CAN_STOP)
    {
        gTasks[taskId].tState = 17;
        gTasks[sEvoGraphicsTaskId].EvoGraphicsTaskEvoStop = TRUE;
        DestroyMovingBackgroundTasks();
        return;
    }

    switch (gTasks[taskId].tState)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        gSprites[sEvoStructPtr->preEvoSpriteId].invisible = FALSE;
        gTasks[taskId].tState++;
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        break;
    case 1: // print 'whoa, poke is evolving!!!' msg
        if (!gPaletteFade.active)
        {
            StringExpandPlaceholders(gStringVar4, gText_PkmnIsEvolving);
            BattlePutTextOnWindow(gStringVar4, B_WIN_MSG);
            gTasks[taskId].tState++;
        }
        break;
    case 2: // wait for string, animate mon(and play its cry)
        if (!IsTextPrinterActive(0))
        {
            PlayCry_Normal(gTasks[taskId].tPreEvoSpecies, 0);
            gTasks[taskId].tState++;
        }
        break;
    case 3:
        if (IsCryFinished()) // wait for animation, play tu du SE
        {
            PlaySE(MUS_EVOLUTION_INTRO);
            gTasks[taskId].tState++;
        }
        break;
    case 4: // play evolution music and fade screen black
        if (!IsSEPlaying())
        {
            PlayNewMapMusic(MUS_EVOLUTION);
            gTasks[taskId].tState++;
            BeginNormalPaletteFade(0x1C, 4, 0, 0x10, RGB_BLACK);
        }
        break;
    case 5: // launch moving bg task, preapre evo sparkles
        if (!gPaletteFade.active)
        {
            InitMovingBackgroundTask(FALSE);
            sEvoGraphicsTaskId = LaunchTask_PreEvoSparklesSet1(17);
            gTasks[taskId].tState++;
        }
        break;
    case 6: // another set of evo sparkles
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            gTasks[taskId].tState++;
            sEvoStructPtr->field_3 = 1;
            sEvoGraphicsTaskId = LaunchTask_PreEvoSparklesSet2();
        }
        break;
    case 7: // launch task that flashes pre evo with post evo sprites
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            sEvoGraphicsTaskId = LaunchTask_PrePostEvoMonSprites(sEvoStructPtr->preEvoSpriteId, sEvoStructPtr->postEvoSpriteId);
            gTasks[taskId].tState++;
        }
        break;
    case 8: // wait for the above task to finish
        if (--sEvoStructPtr->field_3 == 0)
        {
            sEvoStructPtr->field_3 = 3;
            if (!gTasks[sEvoGraphicsTaskId].isActive)
                gTasks[taskId].tState++;
        }
        break;
    case 9: // post evo sparkles
        sEvoGraphicsTaskId = LaunchTask_PostEvoSparklesSet1();
        gTasks[taskId].tState++;
        break;
    case 10:
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            sEvoGraphicsTaskId = LaunchTask_PostEvoSparklesSet2AndFlash(gTasks[taskId].tPostEvoSpecies);
            gTasks[taskId].tState++;
        }
        break;
    case 11: // play tu du sound after evolution
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            PlaySE(SE_EXP);
            gTasks[taskId].tState++;
        }
        break;
    case 12: // stop music, return screen to pre-fade state
        if (IsSEPlaying())
        {
            m4aMPlayAllStop();
            memcpy(&gPlttBufferUnfaded[0x20], sEvoStructPtr->savedPalette, 0x60);
            ResetBgRegsAfterMovingBackgroundCancel();
            BeginNormalPaletteFade(0x1C, 0, 0x10, 0, RGB_BLACK);
            gTasks[taskId].tState++;
        }
        break;
    case 13: // animate mon
        if (!gPaletteFade.active)
        {
            PlayCry_Normal(gTasks[taskId].tPostEvoSpecies, 0);
            gTasks[taskId].tState++;
        }
        break;
    case 14: // congratulations string and rename prompt
        if (IsCryFinished())
        {
            StringExpandPlaceholders(gStringVar4, gText_CongratsPkmnEvolved);
            BattlePutTextOnWindow(gStringVar4, B_WIN_MSG);
            PlayBGM(MUS_EVOLVED);
            gTasks[taskId].tState++;
            SetMonData(mon, MON_DATA_SPECIES, (void*)(&gTasks[taskId].tPostEvoSpecies));
            CalculateMonStats(mon);
            EvolutionRenameMon(mon, gTasks[taskId].tPreEvoSpecies, gTasks[taskId].tPostEvoSpecies);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(gTasks[taskId].tPostEvoSpecies), FLAG_SET_SEEN);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(gTasks[taskId].tPostEvoSpecies), FLAG_SET_CAUGHT);
            IncrementGameStat(GAME_STAT_EVOLVED_POKEMON);
        }
        break;
    case 15: // check if it wants to learn a new move
        if (!IsTextPrinterActive(0))
        {
            var = MonTryLearningNewMoveAfterEvolution(mon, gTasks[taskId].tLearnsFirstMove);
			
            if (var != MON_DONT_FIND_MOVE_TO_LEARN && !gTasks[taskId].tEvoWasStopped)
            {
                u8 text[POKEMON_NAME_LENGTH + 1];

                StopMapMusic();
                Overworld_PlaySpecialMapMusic();
                gTasks[taskId].tBits |= TASK_BIT_LEARN_MOVE;
                gTasks[taskId].tLearnsFirstMove = FALSE;
                gTasks[taskId].tLearnMoveState = 0;
                GetMonData(mon, MON_DATA_NICKNAME, text);
                StringCopy_Nickname(gBattleTextBuff1, text);

            if (var == MON_HAS_MAX_MOVES)
                    gTasks[taskId].tState = 22;
                else if (var == MON_ALREADY_KNOWS_MOVE)
                    break;
                else
                    gTasks[taskId].tState = 20; // move has been learned
            }
            else // no move to learn
            {
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                gTasks[taskId].tState++;
            }
        }
        break;
    case 16: // task has finished, return
        if (!gPaletteFade.active)
        {
            if (!(gTasks[taskId].tBits & TASK_BIT_LEARN_MOVE))
            {
                StopMapMusic();
                Overworld_PlaySpecialMapMusic();
            }
            if (!gTasks[taskId].tEvoWasStopped)
                TryMonCreationEvolution(gTasks[taskId].tPreEvoSpecies, mon);

            DestroyTask(taskId);
            FreeMonSpritesGfx();
            Free(sEvoStructPtr);
            sEvoStructPtr = NULL;
            FreeAllWindowBuffers();
            SetMainCallback2(gCB2_AfterEvolution);
        }
        break;
    case 17: // evolution has been canceled, stop music and re-fade palette
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            m4aMPlayAllStop();
            BeginNormalPaletteFade(0x6001C, 0, 0x10, 0, RGB_WHITE);
            gTasks[taskId].tState++;
        }
        break;
    case 18: // animate pokemon trying to evolve again, evolution has been stopped
        if (!gPaletteFade.active)
        {
            PlayCry_Normal(gTasks[taskId].tPreEvoSpecies, 0);
            gTasks[taskId].tState++;
        }
        break;
    case 19: // after the animation, print the string 'WHOA IT DId NOT EVOLVE!!!'
        if (IsCryFinished())
        {
			StringExpandPlaceholders(gStringVar4, gTasks[taskId].tEvoWasStopped ? gText_EllipsisQuestionMark : gText_PkmnStoppedEvolving);
            BattlePutTextOnWindow(gStringVar4, B_WIN_MSG);
            gTasks[taskId].tEvoWasStopped = TRUE;
            gTasks[taskId].tState = 15;
        }
        break;
    case 20: // pokemon learned a new move, print string and play a fanfare
        if (!IsTextPrinterActive(0) && !IsSEPlaying())
        {
            BufferMoveToLearnIntoBattleTextBuff2();
            PlayFanfare(MUS_LEVEL_UP);
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_PKMNLEARNEDMOVE - BATTLESTRINGS_ID_ADDER]);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
            gTasks[taskId].tLearnsFirstMove = 0x40; // re-used as a counter
            gTasks[taskId].tState++;
        }
        break;
    case 21: // wait a bit and check if can learn another move
        if (!IsTextPrinterActive(0) && !IsSEPlaying() && --gTasks[taskId].tLearnsFirstMove == 0)
            gTasks[taskId].tState = 15;
        break;
    case 22: // try to learn a new move
        switch (gTasks[taskId].tLearnMoveState)
        {
        case 0:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BufferMoveToLearnIntoBattleTextBuff2();
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_TRYTOLEARNMOVE1 - BATTLESTRINGS_ID_ADDER]);
                BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 1:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_TRYTOLEARNMOVE2 - BATTLESTRINGS_ID_ADDER]);
                BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 2:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_TRYTOLEARNMOVE3 - BATTLESTRINGS_ID_ADDER]);
                BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
                gTasks[taskId].tData7 = 5;
                gTasks[taskId].tData8 = 10;
                gTasks[taskId].tLearnMoveState++;
            }
        case 3:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
                BattlePutTextOnWindow(gText_BattleYesNoChoice, B_WIN_YESNO);
                gTasks[taskId].tLearnMoveState++;
                sEvoCursorPos = 0;
                BattleCreateYesNoCursorAt();
            }
            break;
        case 4:
            if (JOY_NEW(DPAD_UP) && sEvoCursorPos != 0)
            {
                PlaySE(SE_SELECT);
                BattleDestroyYesNoCursorAt();
                sEvoCursorPos = 0;
                BattleCreateYesNoCursorAt();
            }
            if (JOY_NEW(DPAD_DOWN) && sEvoCursorPos == 0)
            {
                PlaySE(SE_SELECT);
                BattleDestroyYesNoCursorAt();
                sEvoCursorPos = 1;
                BattleCreateYesNoCursorAt();
            }
            if (JOY_NEW(A_BUTTON))
            {
                HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
                PlaySE(SE_SELECT);

                if (sEvoCursorPos != 0)
                {
                    gTasks[taskId].tLearnMoveState = gTasks[taskId].tData8;
                }
                else
                {
                    gTasks[taskId].tLearnMoveState = gTasks[taskId].tData7;
                    if (gTasks[taskId].tLearnMoveState == 5)
                        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                }
            }
            if (JOY_NEW(B_BUTTON))
            {
                HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
                PlaySE(SE_SELECT);
                gTasks[taskId].tLearnMoveState = gTasks[taskId].tData8;
            }
            break;
        case 5:
            if (!gPaletteFade.active)
            {
                FreeAllWindowBuffers();
                ShowSelectMovePokemonSummaryScreen(gPlayerParty, gTasks[taskId].tPartyId, gPlayerPartyCount - 1, CB2_EvolutionSceneLoadGraphics, gMoveToLearn);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 6:
            if (!gPaletteFade.active && gMain.callback2 == CB2_EvolutionSceneUpdate)
            {
                var = GetMoveSlotToReplace();
                if (var == MAX_MON_MOVES)
                {
                    gTasks[taskId].tLearnMoveState = 10;
                }
                else
                {
                    PrepareMoveBuffer(gBattleTextBuff2, GetMonData(mon, var + MON_DATA_MOVE1));
					RemoveMonPPBonus(mon, var);
					SetMonMoveSlot(mon, gMoveToLearn, var);
					gTasks[taskId].tLearnMoveState++;
                }
            }
            break;
        case 7:
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_123POOF - BATTLESTRINGS_ID_ADDER]);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
            gTasks[taskId].tLearnMoveState++;
            break;
        case 8:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_PKMNFORGOTMOVE - BATTLESTRINGS_ID_ADDER]);
                BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 9:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_ANDELLIPSIS - BATTLESTRINGS_ID_ADDER]);
                BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
                gTasks[taskId].tState = 20;
            }
            break;
        case 10:
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_STOPLEARNINGMOVE - BATTLESTRINGS_ID_ADDER]);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
            gTasks[taskId].tData7 = 11;
            gTasks[taskId].tData8 = 0;
            gTasks[taskId].tLearnMoveState = 3;
            break;
        case 11:
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_DIDNOTLEARNMOVE - BATTLESTRINGS_ID_ADDER]);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
            gTasks[taskId].tState = 15;
            break;
        case 12:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
                gTasks[taskId].tLearnMoveState = 5;
            break;
        }
        break;
    }
}

static void Task_TradeEvolutionScene(u8 taskId)
{
    u32 var;
    struct Pokemon* mon = &gPlayerParty[gTasks[taskId].tPartyId];
   
    switch (gTasks[taskId].tState)
    {
    case 0:
        StringExpandPlaceholders(gStringVar4, gText_PkmnIsEvolving);
        DrawTextOnTradeWindow(0, gStringVar4, 1);
        gTasks[taskId].tState++;
        break;
    case 1:
        if (!IsTextPrinterActive(0))
        {
            PlayCry_Normal(gTasks[taskId].tPreEvoSpecies, 0);
            gTasks[taskId].tState++;
        }
        break;
    case 2:
        if (IsCryFinished())
        {
            m4aSongNumStop(MUS_EVOLUTION);
            PlaySE(MUS_EVOLUTION_INTRO);
            gTasks[taskId].tState++;
        }
        break;
    case 3:
        if (!IsSEPlaying())
        {
            PlayBGM(MUS_EVOLUTION);
            gTasks[taskId].tState++;
            BeginNormalPaletteFade(0x1C, 4, 0, 0x10, RGB_BLACK);
        }
        break;
    case 4:
        if (!gPaletteFade.active)
        {
            InitMovingBackgroundTask(TRUE);
            sEvoGraphicsTaskId = LaunchTask_PreEvoSparklesSet1(gSprites[sEvoStructPtr->preEvoSpriteId].oam.paletteNum + 16);
            gTasks[taskId].tState++;
            SetGpuReg(REG_OFFSET_BG3CNT, BGCNT_PRIORITY(3) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(6));
        }
        break;
    case 5:
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            gTasks[taskId].tState++;
            sEvoStructPtr->field_3 = 1;
            sEvoGraphicsTaskId = LaunchTask_PreEvoSparklesSet2();
        }
        break;
    case 6:
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            sEvoGraphicsTaskId = LaunchTask_PrePostEvoMonSprites(sEvoStructPtr->preEvoSpriteId, sEvoStructPtr->postEvoSpriteId);
            gTasks[taskId].tState++;
        }
        break;
    case 7:
        if (--sEvoStructPtr->field_3 == 0)
        {
            sEvoStructPtr->field_3 = 3;
            if (!gTasks[sEvoGraphicsTaskId].isActive)
                gTasks[taskId].tState++;
        }
        break;
    case 8:
        sEvoGraphicsTaskId = LaunchTask_PostEvoSparklesSet1();
        gTasks[taskId].tState++;
        break;
    case 9:
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            sEvoGraphicsTaskId = LaunchTask_PostEvoSparklesSet2AndFlash_Trade(gTasks[taskId].tPostEvoSpecies);
            gTasks[taskId].tState++;
        }
        break;
    case 10:
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            PlaySE(SE_EXP);
            gTasks[taskId].tState++;
        }
        break;
    case 11:
        if (!IsSEPlaying())
        {
//            Free(sEvoMovingBgPtr);
            PlayCry_Normal(gTasks[taskId].tPostEvoSpecies, 0);
            memcpy(&gPlttBufferUnfaded[0x20], sEvoStructPtr->savedPalette, 0x60);
            gTasks[taskId].tState++;
        }
        break;
    case 12:
        if (IsCryFinished())
        {
            StringExpandPlaceholders(gStringVar4, gText_CongratsPkmnEvolved);
            DrawTextOnTradeWindow(0, gStringVar4, 1);
            PlayFanfare(MUS_EVOLVED);
            gTasks[taskId].tState++;
            SetMonData(mon, MON_DATA_SPECIES, (&gTasks[taskId].tPostEvoSpecies));
            CalculateMonStats(mon);
            EvolutionRenameMon(mon, gTasks[taskId].tPreEvoSpecies, gTasks[taskId].tPostEvoSpecies);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(gTasks[taskId].tPostEvoSpecies), FLAG_SET_SEEN);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(gTasks[taskId].tPostEvoSpecies), FLAG_SET_CAUGHT);
            IncrementGameStat(GAME_STAT_EVOLVED_POKEMON);
        }
        break;
    case 13:
        if (!IsTextPrinterActive(0) && IsFanfareTaskInactive() == TRUE)
        {
            var = MonTryLearningNewMoveAfterEvolution(mon, gTasks[taskId].tLearnsFirstMove);
			
            if (var != MON_DONT_FIND_MOVE_TO_LEARN && !gTasks[taskId].tEvoWasStopped)
            {
                u8 text[POKEMON_NAME_LENGTH + 1];

                gTasks[taskId].tBits |= TASK_BIT_LEARN_MOVE;
                gTasks[taskId].tLearnsFirstMove = FALSE;
                gTasks[taskId].tLearnMoveState = 0;
                GetMonData(mon, MON_DATA_NICKNAME, text);
                StringCopy_Nickname(gBattleTextBuff1, text);

                if (var == MON_HAS_MAX_MOVES)
                    gTasks[taskId].tState = 20;
                else if (var == MON_ALREADY_KNOWS_MOVE)
                    break;
                else
                    gTasks[taskId].tState = 18;
            }
            else
            {
                PlayBGM(MUS_EVOLUTION);
                DrawTextOnTradeWindow(0, gText_CommunicationStandby5, 1);
                gTasks[taskId].tState++;
            }
        }
        break;
    case 14:
        if (!IsTextPrinterActive(0))
        {
            DestroyTask(taskId);
            Free(sEvoStructPtr);
            sEvoStructPtr = NULL;
            gTextFlags.useAlternateDownArrow = FALSE;
            SetMainCallback2(gCB2_AfterEvolution);
        }
        break;
    case 15:
        if (!gTasks[sEvoGraphicsTaskId].isActive)
        {
            m4aMPlayAllStop();
            BeginNormalPaletteFade((1 << (gSprites[sEvoStructPtr->preEvoSpriteId].oam.paletteNum + 16)) | (0x4001C), 0, 0x10, 0, RGB_WHITE);
            gTasks[taskId].tState++;
        }
        break;
    case 16:
        if (!gPaletteFade.active)
        {
            PlayCry_Normal(gTasks[taskId].tPreEvoSpecies, 0);
            gTasks[taskId].tState++;
        }
        break;
    case 17:
        if (IsCryFinished())
        {
            StringExpandPlaceholders(gStringVar4, gText_EllipsisQuestionMark);
            DrawTextOnTradeWindow(0, gStringVar4, 1);
            gTasks[taskId].tEvoWasStopped = TRUE;
            gTasks[taskId].tState = 13;
        }
        break;
    case 18:
        if (!IsTextPrinterActive(0) && !IsSEPlaying())
        {
            BufferMoveToLearnIntoBattleTextBuff2();
            PlayFanfare(MUS_LEVEL_UP);
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_PKMNLEARNEDMOVE - BATTLESTRINGS_ID_ADDER]);
            DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
            gTasks[taskId].tLearnsFirstMove = 0x40; // re-used as a counter
            gTasks[taskId].tState++;
        }
        break;
    case 19:
        if (!IsTextPrinterActive(0) && IsFanfareTaskInactive() == TRUE && --gTasks[taskId].tLearnsFirstMove == 0)
            gTasks[taskId].tState = 13;
        break;
    case 20:
        switch (gTasks[taskId].tLearnMoveState)
        {
        case 0:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BufferMoveToLearnIntoBattleTextBuff2();
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_TRYTOLEARNMOVE1 - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 1:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_TRYTOLEARNMOVE2 - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 2:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_TRYTOLEARNMOVE3 - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tData7 = 5;
                gTasks[taskId].tData8 = 9;
                gTasks[taskId].tLearnMoveState++;
            }
        case 3:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                LoadUserWindowBorderGfx(0, 0xA8, 0xE0);
                CreateYesNoMenu(&gTradeEvolutionSceneYesNoWindowTemplate, 3, 0, 2, 0xA8, 0xE, 0);
                sEvoCursorPos = 0;
                gTasks[taskId].tLearnMoveState++;
                sEvoCursorPos = 0;
            }
            break;
        case 4:
            switch (Menu_ProcessInputNoWrapClearOnChoose())
            {
            case 0:
                sEvoCursorPos = 0;
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_EMPTYSTRING - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tLearnMoveState = gTasks[taskId].tData7;
                if (gTasks[taskId].tLearnMoveState == 5)
                    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                break;
            case 1:
            case -1:
                sEvoCursorPos = 1;
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_EMPTYSTRING - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tLearnMoveState = gTasks[taskId].tData8;
                break;
            }
            break;
        case 5:
            if (!gPaletteFade.active)
            {
                if (gWirelessCommType)
                    DestroyWirelessStatusIndicatorSprite();

                Free(GetBgTilemapBuffer(3));
                Free(GetBgTilemapBuffer(1));
                Free(GetBgTilemapBuffer(0));
                FreeAllWindowBuffers();

                ShowSelectMovePokemonSummaryScreen(gPlayerParty, gTasks[taskId].tPartyId, gPlayerPartyCount - 1, CB2_TradeEvolutionSceneLoadGraphics, gMoveToLearn);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 6:
            if (!gPaletteFade.active && gMain.callback2 == CB2_TradeEvolutionSceneUpdate)
            {
                var = GetMoveSlotToReplace();
                if (var == MAX_MON_MOVES)
                {
                    gTasks[taskId].tLearnMoveState = 9;
                }
                else
                {
                    PrepareMoveBuffer(gBattleTextBuff2, GetMonData(mon, var + MON_DATA_MOVE1));
					RemoveMonPPBonus(mon, var);
					SetMonMoveSlot(mon, gMoveToLearn, var);
					BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_123POOF - BATTLESTRINGS_ID_ADDER]);
					DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
					gTasks[taskId].tLearnMoveState++;
                }
            }
            break;
        case 7:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_PKMNFORGOTMOVE - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tLearnMoveState++;
            }
            break;
        case 8:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
            {
                BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_ANDELLIPSIS - BATTLESTRINGS_ID_ADDER]);
                DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
                gTasks[taskId].tState = 18;
            }
            break;
        case 9:
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_STOPLEARNINGMOVE - BATTLESTRINGS_ID_ADDER]);
            DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
            gTasks[taskId].tData7 = 10;
            gTasks[taskId].tData8 = 0;
            gTasks[taskId].tLearnMoveState = 3;
            break;
        case 10:
            BattleStringExpandPlaceholdersToDisplayedString(gBattleStringsTable[STRINGID_DIDNOTLEARNMOVE - BATTLESTRINGS_ID_ADDER]);
            DrawTextOnTradeWindow(0, gDisplayedStringBattle, 1);
            gTasks[taskId].tState = 13;
            break;
        case 11:
            if (!IsTextPrinterActive(0) && !IsSEPlaying())
                gTasks[taskId].tLearnMoveState = 5;
            break;
        }
        break;
    }
}

#undef tState
#undef tPreEvoSpecies
#undef tPostEvoSpecies
#undef tBits
#undef tLearnsFirstMove
#undef tLearnMoveState
#undef tData7
#undef tData8
#undef tEvoWasStopped
#undef tPartyId

static void EvoDummyFunc(void)
{
}

static void VBlankCB_EvolutionScene(void)
{
    SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_X);
    SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_Y);
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
    SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);
    SetGpuReg(REG_OFFSET_BG3HOFS, gBattle_BG3_X);
    SetGpuReg(REG_OFFSET_BG3VOFS, gBattle_BG3_Y);

    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ScanlineEffect_InitHBlankDmaTransfer();
}

static void Task_MovingBackgroundPalettes(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (data[6] != 0)
        return;
    if (data[5]++ < 20)
        return;

    if (data[0]++ > sMovingBackgroundTimers[data[2]][3])
    {
        if (sMovingBackgroundTimers[data[2]][1] == data[1])
        {
            data[3]++;
            if (data[3] == sMovingBackgroundTimers[data[2]][2])
            {
                data[3] = 0;
                data[2]++;
            }
            data[1] = sMovingBackgroundTimers[data[2]][0];
        }
        else
        {
            LoadPalette(&sEvoMovingBgPtr[data[1] * 16], 0xA0, 0x20);
            data[0] = 0;
            data[1]++;
        }
    }

    if (data[2] == 4)
        DestroyTask(taskId);
}

static void LaunchTask_MovingBackgroundPos(bool8 isLink)
{
	gTasks[CreateTask(Task_MovingBackgroundPos, 7)].data[2] = isLink ? 1 : 0;
}

static void Task_MovingBackgroundPos(u8 taskId)
{
    u16 *outer_X, *outer_Y;

    u16 *inner_X = &gBattle_BG1_X;
    u16 *inner_Y = &gBattle_BG1_Y;

    if (!gTasks[taskId].data[2])
    {
        outer_X = &gBattle_BG2_X;
        outer_Y = &gBattle_BG2_Y;
    }
    else
    {
        outer_X = &gBattle_BG3_X;
        outer_Y = &gBattle_BG3_Y;
    }

    gTasks[taskId].data[0] = (gTasks[taskId].data[0] + 5) & 0xFF;
    gTasks[taskId].data[1] = (gTasks[taskId].data[0] + 0x80) & 0xFF;

    *inner_X = Cos(gTasks[taskId].data[0], 4) + 8;
    *inner_Y = Sin(gTasks[taskId].data[0], 4) + 16;

    *outer_X = Cos(gTasks[taskId].data[1], 4) + 8;
    *outer_Y = Sin(gTasks[taskId].data[1], 4) + 16;

    if (!FuncIsActiveTask(Task_MovingBackgroundPalettes))
    {
        DestroyTask(taskId);

        *inner_X = 0;
        *inner_Y = 0;

        *outer_X = 256;
        *outer_Y = 0;
    }
}

static void InitMovingBgValues(u16 *movingBgs)
{
    s32 i, j;

    for (i = 0; i < 50; i++)
    {
        for (j = 0; j < 16; j++)
        {
            movingBgs[i * 16 + j] = sMovingBgPals[sMovingBgPalIndices[i][j]];
        }
    }
}

static void InitMovingBackgroundTask(bool8 isLink)
{
    u8 innerBgId = 1, outerBgId = isLink ? 3 : 2;

    sEvoMovingBgPtr = AllocZeroed(0x640);
    InitMovingBgValues(sEvoMovingBgPtr);

    LoadPalette(sBlackPalette, 0xA0, 0x20);

    DecompressAndLoadBgGfxUsingHeap(1, sMovingBackgroundTiles, FALSE, 0, 0);
    CopyToBgTilemapBuffer(1, sMovingBackgroundMap1, 0, 0);
    CopyToBgTilemapBuffer(outerBgId, sMovingBackgroundMap2, 0, 0);
    CopyBgTilemapBufferToVram(1);
    CopyBgTilemapBufferToVram(outerBgId);

    if (!isLink)
    {
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 8));
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP);

        SetBgAttribute(innerBgId, BG_ATTR_PRIORITY, 2);
        SetBgAttribute(outerBgId, BG_ATTR_PRIORITY, 2);

        ShowBg(1);
        ShowBg(2);
    }
    else
    {
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 8));
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG3_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP);
    }

    CreateTask(Task_MovingBackgroundPalettes, 5);
    LaunchTask_MovingBackgroundPos(isLink);
}

void IsMovingBackgroundTaskRunning(void)
{
    u8 taskId = FindTaskIdByFunc(Task_MovingBackgroundPalettes);

    if (taskId != 0xFF)
        gTasks[taskId].data[6] = 1;

    FillPalette(RGB_BLACK, 0xA0, 0x20);
}

static void DestroyMovingBackgroundTasks(void)
{
    u8 taskId = FindTaskIdByFunc(Task_MovingBackgroundPalettes);

    if (taskId  != 0xFF)
        DestroyTask(taskId);
	
	taskId = FindTaskIdByFunc(Task_MovingBackgroundPos);
	
    if (taskId != 0xFF)
        DestroyTask(taskId);

    FillPalette(RGB_BLACK, 0xA0, 0x20);
    ResetBgRegsAfterMovingBackgroundCancel();
}

static void ResetBgRegsAfterMovingBackgroundCancel(void)
{
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    SetBgAttribute(1, BG_ATTR_PRIORITY, GetBattleBgAttribute(1, 5));
    SetBgAttribute(2, BG_ATTR_PRIORITY, GetBattleBgAttribute(2, 5));
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG3_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP);
    Free(sEvoMovingBgPtr);
}
