#include "global.h"
#include "battle.h"
#include "cable_club.h"
#include "daycare.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "form_change.h"
#include "gflib.h"
#include "graphics.h"
#include "item.h"
#include "link.h"
#include "load_save.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon_icon.h"
#include "raid_intro.h"
#include "random.h"
#include "region_map.h"
#include "rtc.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "task.h"
#include "trainer_pokemon_sprites.h"
#include "wild_encounter.h"
#include "constants/daycare.h"
#include "constants/event_objects.h"
#include "constants/hold_effects.h"
#include "constants/map_types.h"
#include "constants/songs.h"

/*
 * Raid Intro screen and mechanics.
 * Ported from Skeli's CFRU and heavily reworked/upgraded by Blackuser
 * to accommodate the new functionality of Link Raid Battles.
 */

// Arguments for RaidIntroCreateInterfaceSprites
enum
{
    CREATE_AVAILABLE_TEAM_SPRITES,
    CREATE_PLAYER_SELECTED_TEAM_SPRITES,
    CREATE_NO_TEAM_SPRITES
};

// Arguments for SendRaidBattleConnectionState
enum
{
    RAID_CONN_STATE_START_BATTLE,
    RAID_CONN_STATE_CANCEL_BATTLE
};

enum
{
    WIN_MESSAGE,
    WIN_RECOMMENDED_LEVEL,
    WIN_CHOOSE_PARTNER,
    WIN_RULES,
    WIN_TYPES,
    WIN_INSTRUCTIONS,
    WIN_COUNT
};

#define WHITE_OUTLINE_LENGTH 2

#define TAG_RAID_INTRO_CURSOR 10002
#define CURSOR_Y_SCROLL_OFFSET 33

struct RaidInfo
{
    u16 visualSpecies; // Displays it as gigantamaxed if needed, but starts the battle with the base species stored at actualSpecies
    u16 actualSpecies;
    u16 item;
    u8 level;
    u8 numStars;
    struct ItemSlot drops[MAX_RAID_DROPS];
};

struct RaidBattleIntro
{
    u32 *tilemapPtr;
    struct RaidInfo raidInfo[2]; // For regular intro, and link intro(if slave)
    u8 raidMonSpriteId;
    u8 starSpriteIds[RAID_STAR_COUNT - 1];
    u8 cursorSelection:2;
    u8 cursorIsInvisible:1;
    u8 canCancel:1;
    u8 canConnectInLink:1;
    u8 tryLinkConnection:1;
    u8 updatingScreen:1; // Updating screen during link connection
    u8 isLinkMaster:1;
    struct RaidPartnerData
    {
        struct RaidPartner trainerData; // Ingame partners data
        struct SpriteData
        {
            u8 trainerSpriteId;
            u8 teamSpriteIds[MULTI_PARTY_SIZE];
        } spriteData;
    } partners[MAX_RAID_PARTNERS];
};

struct RaidStarsData
{
    u8 minLevel;
    u8 maxLevel;
    u8 eggMoveChance;
};

struct RaidInfoSendBuffer
{
    struct RaidInfo raidInfo;
    u16 selectedTeam[MULTI_PARTY_SIZE];
};

struct ConnectionStateSendBuffer
{
    u8 state;
};

static bool32 TryGetRaidBattleData(bool32 startRaidIntro);
static void CB2_RaidBattleIntro(void);
static void SelectRandomRaidBattleRewards(struct ItemSlot *output, const u16 *drops);
static u32 GetRaidRandomNumber(void);
static const struct RaidData *GetCurrentMapRaidData(u32 numStars);
static void RaidIntroPrintInterfaceTexts(bool32 printLinkPartnersText, bool32 useLinkInfo);
static void CreateSelectionCursorSprite(void);
static void SpriteCB_Cursor(struct Sprite *sprite);
static void RaidIntroCreateInterfaceSprites(u32 teamCreationState, bool32 useLinkInfo);
static void OutlineRaidMonSprite(void);
static void CreateRaidMon(bool32 isLink);
static void RaidIntroShowMessageInBox(const u8 *str);
static void StartRaidBattle(bool32 isLink);
static void StartLinkRaidBattleTransition(void);
static void CB2_ReturnFromRaidBattle(void);
static void CB2_ReturnFromLinkRaidBattle(void);
static void Task_RaidBattleIntroFadeIn(u32 taskId);
static void Task_RaidBattleIntroWaitForKeyPress(u32 taskId);
static void Task_RaidBattleIntroWaitTextAndStartLinkConnection(u32 taskId);
static void Task_RaidBattleIntroStartLinkConnectionAfterTimer(u32 taskId);
static void Task_RaidBattleIntroWaitForLinkPlayers(u32 taskId);
static void Task_RaidIntro_LinkConnection_Master(u32 taskId);
static void Task_RaidIntro_LinkConnection_Master_Step2(u32 taskId);
static void Task_RaidIntro_LinkConnection_Master_Step3(u32 taskId);
static void Task_RaidIntro_LinkConnection_Master_Step4(u32 taskId);
static void Task_RaidIntro_LinkConnection_Slave(u32 taskId);
static void Task_RaidBattleIntro_LinkError_WaitTextAndReturnToInput(u32 taskId);
static void Task_RaidIntro_LinkConnection_ReceiveSelectedTeamSlots(u32 taskId);
static void Task_RaidIntro_LinkConnection_UpdateInterface(u32 taskId);
static void Task_RaidIntro_LinkConnection_WaitConfirmationText(u32 taskId);
static void Task_RaidIntro_LinkConnection_WaitReceivedLinkState(u32 taskId);
static void Task_RaidIntro_LinkConnection_StartBattle(u32 taskId);
static void Task_RaidIntro_LinkConnection_StartBattle_Step2(u32 taskId);
static void Task_RaidIntro_LinkConnection_StartBattle_Step3(u32 taskId);

static EWRAM_DATA struct RaidBattleIntro *sRaidBattleIntro = NULL;
EWRAM_DATA struct ItemSlot gRaidBattleDrops[MAX_RAID_DROPS] = {0};

static const u8 sRaidCursorGfx[]   = INCBIN_U8("graphics/raid_intro/cursor.4bpp");
static const u16 sRaidCursorPal[]  = INCBIN_U16("graphics/raid_intro/cursor.gbapal");

static const struct SpriteSheet sRaidBattleCursorSpriteSheet =
{
    sRaidCursorGfx, sizeof(sRaidCursorGfx), TAG_RAID_INTRO_CURSOR
};

static const struct SpritePalette sRaidBattleCursorSpritePalette =
{
    sRaidCursorPal, TAG_RAID_INTRO_CURSOR
};

static const struct OamData sOamData_RedArrowCursor =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const struct SpriteTemplate sRaidBattleCursorSpriteTemplate =
{
    .tileTag = TAG_RAID_INTRO_CURSOR,
    .paletteTag = TAG_RAID_INTRO_CURSOR,
    .oam = &sOamData_RedArrowCursor,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Cursor,
};

static const u8 sText_PlayerWantToEnterRaidBattle[] = _("A Player want to enter the\nRaid Battle with you…\lA Button: Accept\lB Button: Reject");

static const u8 sTextColor_Normal[] = {0, 1, 2};
static const u8 sTextColor_AvailablePartners[] = {0, 2, 3};

static const struct BgTemplate sRaidBattleIntroBgTemplates[] =
{
    { // Link connection
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    { // Text
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    { // Intro Background
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    }
};

static const struct WindowTemplate sRaidBattleIntroWinTemplates[WIN_COUNT + 1] =
{
    [WIN_MESSAGE] =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 160,
    },
    [WIN_RECOMMENDED_LEVEL] =
    {
        .bg = 1,
        .tilemapLeft = 16,
        .tilemapTop = 0,
        .width = 14,
        .height = 3,
        .paletteNum = 14,
        .baseBlock = 1,
    },
    [WIN_CHOOSE_PARTNER] =
    {
        .bg = 1,
        .tilemapLeft = 16,
        .tilemapTop = 3,
        .width = 14,
        .height = 2,
        .paletteNum = 14,
        .baseBlock = 43,
    },
    [WIN_RULES] =
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 13,
        .width = 10,
        .height = 6,
        .paletteNum = 14,
        .baseBlock = 71,
    },
    [WIN_TYPES] =
    {
        .bg = 1,
        .tilemapLeft = 8,
        .tilemapTop = 0,
        .width = 8,
        .height = 2,
        .paletteNum = 12,
        .baseBlock = 131,
    },
    [WIN_INSTRUCTIONS] =
    {
        .bg = 1,
        .tilemapLeft = 8,
        .tilemapTop = 18,
        .width = 22,
        .height = 2,
        .paletteNum = 14,
        .baseBlock = 147,
    },
    [WIN_COUNT] = DUMMY_WIN_TEMPLATE
};

static const struct RaidPartner sIngameRaidPossiblePartners[] =
{
    {
        .trainerId = PARTNER_RAID_YOUNGSTER_NOAH,
        .graphicsId = OBJ_EVENT_GFX_YOUNGSTER,
    },
    {
        .trainerId = PARTNER_RAID_PKMN_BREEDER_KIT,
        .graphicsId = OBJ_EVENT_GFX_PICNICKER,
    },
    {
        .trainerId = PARTNER_RAID_GENTLEMAN_MARTIN,
        .graphicsId = OBJ_EVENT_GFX_GENTLEMAN,
    },
    {
        .trainerId = PARTNER_RAID_BLACK_BELT_OSCAR,
        .graphicsId = OBJ_EVENT_GFX_BLACKBELT,
    },
    {
        .trainerId = PARTNER_RAID_PKMN_BREEDER_POPPY,
        .graphicsId = OBJ_EVENT_GFX_PICNICKER,
    },
    {
        .trainerId = PARTNER_RAID_GENTLEMAN_ALFIE,
        .graphicsId = OBJ_EVENT_GFX_GENTLEMAN,
    },
    {
        .trainerId = PARTNER_RAID_YOUNGSTER_AUSTIN,
        .graphicsId = OBJ_EVENT_GFX_YOUNGSTER,
    },
    {
        .trainerId = PARTNER_RAID_BLACK_BELT_ARTHUR,
        .graphicsId = OBJ_EVENT_GFX_BLACKBELT,
    },
    {
        .trainerId = PARTNER_RAID_YOUNGSTER_LOGAN,
        .graphicsId = OBJ_EVENT_GFX_YOUNGSTER,
    },
    {
        .trainerId = PARTNER_RAID_YOUNGSTER_EVAN,
        .graphicsId = OBJ_EVENT_GFX_YOUNGSTER,
    },
};

static const struct RaidStarsData sRaidStarsData[RAID_STAR_COUNT - 1] =
{
    [ONE_STAR_RAID - 1] =
    {
        .minLevel = 15,
        .maxLevel = 20,
        .eggMoveChance = 0,
    },
    [TWO_STAR_RAID - 1] =
    {
        .minLevel = 25,
        .maxLevel = 30,
        .eggMoveChance = 10,
    },
    [THREE_STAR_RAID - 1] =
    {
        .minLevel = 36,
        .maxLevel = 40,
        .eggMoveChance = 30,
    },
    [FOUR_STAR_RAID - 1] =
    {
        .minLevel = 50,
        .maxLevel = 55,
        .eggMoveChance = 50,
    },
    [FIVE_STAR_RAID - 1] =
    {
        .minLevel = 55,
        .maxLevel = 62,
        .eggMoveChance = 70,
    },
    [SIX_STAR_RAID - 1] =
    {
        .minLevel = 75,
        .maxLevel = 90,
        .eggMoveChance = 70,
    },
};

static const u8 sRaidStarsRangeByBadgeCount[NUM_BADGES + 1][2] =
{
    [0] = {ONE_STAR_RAID, ONE_STAR_RAID}, // no badges
    [1] = {ONE_STAR_RAID, ONE_STAR_RAID}, // badge 1
    [2] = {ONE_STAR_RAID, TWO_STAR_RAID}, // badge 2
    [3] = {TWO_STAR_RAID, TWO_STAR_RAID}, // badge 3
    [4] = {TWO_STAR_RAID, THREE_STAR_RAID}, // badge 4
    [5] = {THREE_STAR_RAID, THREE_STAR_RAID}, // badge 5
    [6] = {THREE_STAR_RAID, FOUR_STAR_RAID}, // badge 6
    [7] = {FOUR_STAR_RAID, FIVE_STAR_RAID}, // badge 7
    [8] = {FIVE_STAR_RAID, SIX_STAR_RAID}, // badge 8
};

// In percent
static const u8 sRaidBattleDropChances[MAX_RAID_DROPS] =
{
    100,
    80,
    80,
    50,
    50,
    30,
    30,
    25,
    25,
    5,
    4,
    1,
};

////////////////////
// INITIALIZATION //
////////////////////

void RaidDenInitIntro(void)
{
    if (!TryGetRaidBattleData(TRUE))
        gSpecialVar_0x8004 = FALSE;
}

void InitFixedRaidIntro(struct RaidIntroData *raidIntroData)
{
    u32 i, j, visualSpecies;
    
    gSpecialVar_0x8004 = FALSE;

    sRaidBattleIntro = AllocZeroed(sizeof(*sRaidBattleIntro));
    sRaidBattleIntro->tilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
    sRaidBattleIntro->canCancel = raidIntroData->canCancel;
    sRaidBattleIntro->canConnectInLink = raidIntroData->canConnectInLink;
    sRaidBattleIntro->raidInfo[FALSE].actualSpecies = raidIntroData->species;
    sRaidBattleIntro->raidInfo[FALSE].numStars = raidIntroData->numStars;
    sRaidBattleIntro->raidInfo[FALSE].level = raidIntroData->level;
    sRaidBattleIntro->raidInfo[FALSE].item = raidIntroData->item;
    
    visualSpecies = raidIntroData->species;
    if (raidIntroData->tryGigantamax)
    {
        u32 newSpecies = GetSpeciesForm(FORM_CHANGE_GIGANTAMAX, visualSpecies, 0, ABILITY_NONE, ITEM_NONE, NULL, 0);
        
        if (newSpecies)
            visualSpecies = newSpecies;
    }
    sRaidBattleIntro->raidInfo[FALSE].visualSpecies = visualSpecies;
    
    if (raidIntroData->drops != NULL)
        SelectRandomRaidBattleRewards(sRaidBattleIntro->raidInfo[FALSE].drops, raidIntroData->drops);

    for (i = 0; i < MAX_RAID_PARTNERS; i++)
    {
        sRaidBattleIntro->partners[i].trainerData.trainerId = raidIntroData->partners[i].trainerId;
        sRaidBattleIntro->partners[i].trainerData.graphicsId = raidIntroData->partners[i].graphicsId;
        
        // Init sprites
        sRaidBattleIntro->partners[i].spriteData.trainerSpriteId = MAX_SPRITES;
        for (j = 0; j < MULTI_PARTY_SIZE; j++)
            sRaidBattleIntro->partners[i].spriteData.teamSpriteIds[j] = MAX_SPRITES;
    }
    SavePlayerParty();
    ScriptContext2_Enable();
    PlayRainStoppingSoundEffect();
    CleanupOverworldWindowsAndTilemaps();
    SetMainCallback2(CB2_RaidBattleIntro);
}

static void RaidIntroFreeResources(void)
{
    FREE_AND_SET_NULL(sRaidBattleIntro->tilemapPtr);
    FREE_AND_SET_NULL(sRaidBattleIntro);
    FreeAllWindowBuffers();
}

static void VBlankCB_RaidBattleIntro(void)
{
    if (!sRaidBattleIntro->updatingScreen)
    {
        LoadOam();
        ProcessSpriteCopyRequests();
        TransferPlttBuffer();
    }
}

static void MainCB2_RaidBattleIntro(void)
{
    RunTasks();
    
    if (!sRaidBattleIntro->updatingScreen)
    {
        RunTextPrinters();
        AnimateSprites();
        BuildOamBuffer();
        UpdatePaletteFade();
    }
}

static void CB2_RaidBattleIntro(void)
{
    switch (gMain.state)
    {
        case 0:
            SetVBlankCallback(NULL);
            ResetVramOamAndBgCntRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            break;
        case 1:
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            ResetTempTileDataBuffers();
            ResetPaletteFade();
            FreeAllSpritePalettes();
            break;
        case 2:
            ResetBgsAndClearDma3BusyFlags(FALSE);
            InitBgsFromTemplates(0, sRaidBattleIntroBgTemplates, ARRAY_COUNT(sRaidBattleIntroBgTemplates));
            SetBgTilemapBuffer(2, sRaidBattleIntro->tilemapPtr);
            break;
        case 3:
            DecompressAndCopyTileDataToVram(2, &gRaidIntroBgGfx, 0, 0, 0);
            LZDecompressWram(gRaidIntroBgMap, sRaidBattleIntro->tilemapPtr);
            LoadCompressedPalette(gRaidIntroBgPal, 0, 0x20);
            break;
        case 4:
            ListMenuLoadStdPalAt(0xC0, 1);
            LoadMonIconPalettes();
            break;
        case 5:
            if (IsDma3ManagerBusyWithBgCopy())
                return;
            break;
        case 6:
            ShowBg(1);
            ShowBg(2);
            CopyBgTilemapBufferToVram(2);
            break;
        case 7:
            InitWindows(sRaidBattleIntroWinTemplates);
            DeactivateAllTextPrinters();
            break;
        case 8:
            RaidIntroPrintInterfaceTexts(sRaidBattleIntro->tryLinkConnection, FALSE);
            LoadStdWindowFrameGfx();
            DrawStdWindowFrame(WIN_MESSAGE, TRUE);
            break;
        case 9:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            break;
        case 10:
            CreateSelectionCursorSprite();
            
            if (sRaidBattleIntro->tryLinkConnection)
                sRaidBattleIntro->cursorIsInvisible = TRUE;
            break;
        case 11:
            LoadSymbolsIconGraphics();
            break;
        case 12:
            RaidIntroCreateInterfaceSprites(sRaidBattleIntro->tryLinkConnection ? CREATE_PLAYER_SELECTED_TEAM_SPRITES : CREATE_AVAILABLE_TEAM_SPRITES, FALSE);
            break;
        case 13:
            SetVBlankCallback(VBlankCB_RaidBattleIntro);
            SetMainCallback2(MainCB2_RaidBattleIntro);
            CreateTask(Task_RaidBattleIntroFadeIn, 0);
            return;
    }
    ++gMain.state;
}

static u32 DetermineNumRaidStars(u32 random)
{
    u32 numBadges = GetNumOfBadges();
    u32 minStars = sRaidStarsRangeByBadgeCount[numBadges][0];
    u32 maxStars = sRaidStarsRangeByBadgeCount[numBadges][1];
    
    if (minStars == maxStars)
        return minStars;
    else
        return (random % ((maxStars + 1) - minStars)) + minStars;
}

static u32 DetermineRaidSpeciesLevel(u32 numStars, u32 random)
{
    u32 minLevel = sRaidStarsData[numStars - 1].minLevel;
    u32 maxLevel = sRaidStarsData[numStars - 1].maxLevel;
    
    if (minLevel == maxLevel)
        return minLevel;
    else
        return (random % (maxLevel - minLevel)) + minLevel;
}

static bool32 DetermineRaidPartners(u32 random, struct RaidPartner *partners)
{
    u32 i, numSelectedPartners = 0;

    for (i = 1; i < 0xFFFFFFFF; i += MAX_RAID_PARTNERS)
    {
        u32 index;
        
        if (!random)
            random = 0xFFFFFFFF;
        
        random *= i;
        index = random % ARRAY_COUNT(sIngameRaidPossiblePartners);
        
        if (sIngameRaidPossiblePartners[index].trainerId == partners[0].trainerId
        || sIngameRaidPossiblePartners[index].trainerId == partners[1].trainerId
        || sIngameRaidPossiblePartners[index].trainerId == partners[2].trainerId)
            continue;

        partners[numSelectedPartners].trainerId = sIngameRaidPossiblePartners[index].trainerId;
        partners[numSelectedPartners].graphicsId = sIngameRaidPossiblePartners[index].graphicsId;
        
        if (++numSelectedPartners == MAX_RAID_PARTNERS)
            break;
    }
    return (numSelectedPartners == MAX_RAID_PARTNERS);
}

static bool32 ShouldTryGigantamaxRaidMon(u32 numStars, u32 random)
{
    // 25% chance a six star raid mon turning into its gigantamax form, if any
    return (numStars == SIX_STAR_RAID && (random % 100 >= 95 || random % 100 < 20));
}

static bool32 TryGetRaidBattleData(bool32 startRaidIntro)
{
    u32 raidIndex, species;
    struct RaidPartner partners[MAX_RAID_PARTNERS];
    u32 random = GetRaidRandomNumber();
    u32 numStars = DetermineNumRaidStars(random);
    const struct RaidData *raid = GetCurrentMapRaidData(numStars);
    
    if (raid == NULL)
        return FALSE; // Failed to locate raid in this map
    
    raidIndex = random % raid->amount;
    
    species = raid->data[raidIndex].species;
    if (!species)
        return FALSE; // Failed to find a species

    memset(partners, 0, sizeof(partners));
    if (!DetermineRaidPartners(random, partners))
        return FALSE; // Failed to find partners
    
    if (startRaidIntro)
    {
        struct RaidIntroData raidIntroData =
        {
            .species = species,
            .numStars = numStars,
            .level = DetermineRaidSpeciesLevel(numStars, random),
            .item = raid->data[raidIndex].item,
            .partners = partners,
            .drops = raid->data[raidIndex].drops,
            .canCancel = TRUE,
            .canConnectInLink = TRUE,
            .tryGigantamax = ShouldTryGigantamaxRaidMon(numStars, random),
        };
        InitFixedRaidIntro(&raidIntroData);
    }
    return TRUE;
}

#define tTimer     data[0]
#define tStatus    data[1]
#define tConnected data[2]
#define tIsInfoMsg data[3]

static void Task_RaidBattleIntroFadeIn(u32 taskId)
{
    if (gTasks[taskId].tTimer < WHITE_OUTLINE_LENGTH)
    {
        gTasks[taskId].tTimer++;
        OutlineRaidMonSprite();
    }
    
    if (!gPaletteFade.active)
    {
        PlaySE(SE_CARD_OPEN);
        
        // Try start link connection after returning from party selection
        if (sRaidBattleIntro->tryLinkConnection)
        {
            sRaidBattleIntro->tryLinkConnection = FALSE;
            ShowBg(0);
            RaidIntroShowMessageInBox(CableClub_Text_PleaseWaitBCancel);
            gTasks[taskId].func = Task_RaidBattleIntroWaitTextAndStartLinkConnection;
        }
        else
            gTasks[taskId].func = Task_RaidBattleIntroWaitForKeyPress;
    }
}

static void Task_RaidBattleIntroFadeOutToField(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        LoadPlayerParty();
        RaidIntroFreeResources();
        SetMainCallback2(CB2_ReturnToFieldContinueScript);
        DestroyTask(taskId);
    }
}

///////////////////////
// INSTRUCTION TEXTS //
///////////////////////

static void ClearRaidIntroWindows(void)
{
    u32 i;
    
    for (i = WIN_RECOMMENDED_LEVEL; i < WIN_COUNT; i++)
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
}

static void ShowRaidIntroWindows(void)
{
    u32 i;
    
    for (i = WIN_RECOMMENDED_LEVEL; i < WIN_COUNT; i++)
    {
        CopyWindowToVram(i, COPYWIN_BOTH);
        PutWindowTilemap(i);
    }
}

static void PrintRaidInstructions(void)
{
    AddTextPrinterParameterized3(WIN_INSTRUCTIONS, 0, 8, 4, sTextColor_Normal, 0, COMPOUND_STRING("{DPAD_UPDOWN}Pick {A_BUTTON}Choose {START_BUTTON}Link {B_BUTTON}Cancel"));
}

static void PrintRaidRules(void)
{
    AddTextPrinterParameterized3(WIN_RULES, 0, 0, 2, sTextColor_Normal, 0, COMPOUND_STRING("Battle ends if:\n4 Pokémon faint\n10 turns pass"));
}

static void PrintRaidAvailablePartners(bool32 duringLink)
{
    if (duringLink)
        AddTextPrinterParameterized3(WIN_CHOOSE_PARTNER, 3, 16, 4, sTextColor_AvailablePartners, 0, COMPOUND_STRING("Link Partners"));
    else
        AddTextPrinterParameterized3(WIN_CHOOSE_PARTNER, 3, 1, 4, sTextColor_AvailablePartners, 0, COMPOUND_STRING("Available Partners"));
}

static void PrintRaidRecommendedLevel(bool32 duringLink)
{
    StringCopy(gStringVar1, COMPOUND_STRING("Recommended Lvl: "));
    ConvertIntToDecimalStringN(gStringVar2, GetRaidRecommendedLevel(sRaidBattleIntro->raidInfo[duringLink].numStars), STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar1, gStringVar2);
    AddTextPrinterParameterized3(WIN_RECOMMENDED_LEVEL, 0, 2, 0, sTextColor_Normal, 0, gStringVar1);
}

static void PrintRaidMonTypes(bool32 duringLink)
{
    u32 species = sRaidBattleIntro->raidInfo[duringLink].visualSpecies;
    u32 type1 = gSpeciesInfo[species].types[0];
    u32 type2 = gSpeciesInfo[species].types[1];

    BlitMoveInfoIcon(WIN_TYPES, MOVE_INFO_MOVE_TYPE_ICON + type1, 0, 2);
    
    if (type1 != type2)
        BlitMoveInfoIcon(WIN_TYPES, MOVE_INFO_MOVE_TYPE_ICON + type2, 32, 2);
}

static void RaidIntroPrintInterfaceTexts(bool32 printLinkPartnersText, bool32 useLinkInfo)
{
    ClearRaidIntroWindows();
    ShowRaidIntroWindows();
    
    PrintRaidInstructions();
    PrintRaidRules();
    
    PrintRaidAvailablePartners(printLinkPartnersText);
    
    PrintRaidRecommendedLevel(useLinkInfo);
    PrintRaidMonTypes(useLinkInfo);
    
    ShowRaidIntroWindows();
}

/////////////////
// MESSAGE BOX //
/////////////////

static void RaidIntroShowMessageInBox(const u8 *str)
{
    StringCopy(gStringVar4, str);
    FillWindowPixelBuffer(WIN_MESSAGE, PIXEL_FILL(1));
    AddTextPrinterForMessage(TRUE);
}

static inline bool32 RaidIntro_IsTextPrinterActive(void)
{
    return IsTextPrinterActive(WIN_MESSAGE);
}

//////////////////////
// PARTNERS SPRITES //
//////////////////////

static void CreateRaidPartnerObjectEventSprite(u32 graphicsId, u32 partnerIndex)
{
    u32 spriteId = AddPseudoObjectEvent(graphicsId, SpriteCallbackDummy, 126, 59 + (partnerIndex * CURSOR_Y_SCROLL_OFFSET), 0);
    sRaidBattleIntro->partners[partnerIndex].spriteData.trainerSpriteId = spriteId;
    gSprites[spriteId].oam.priority = 1;
}

static void DestroyRaidPartnerObjectEventSprite(u32 partnerIndex)
{
    if (sRaidBattleIntro->partners[partnerIndex].spriteData.trainerSpriteId != MAX_SPRITES)
    {
        DestroySprite(&gSprites[sRaidBattleIntro->partners[partnerIndex].spriteData.trainerSpriteId]);
        sRaidBattleIntro->partners[partnerIndex].spriteData.trainerSpriteId = MAX_SPRITES;
    }
}

static void CreateRaidPartnerTeamSprites(u16 *selectedTeam, u32 partnerIndex)
{
    u32 i, spriteId;
    
    for (i = 0; i < MULTI_PARTY_SIZE; i++)
    {
        if (selectedTeam[i])
            spriteId = CreateMonIcon(selectedTeam[i], SpriteCB_MonIcon, 158 + (32 * i), 59 + (partnerIndex * CURSOR_Y_SCROLL_OFFSET), 0);
        else
            spriteId = MAX_SPRITES;
        
        sRaidBattleIntro->partners[partnerIndex].spriteData.teamSpriteIds[i] = spriteId;
    }
}

static void DestroyRaidPartnerTeamSprites(u32 partnerIndex)
{
    u32 i;
    
    for (i = 0; i < MULTI_PARTY_SIZE; i++)
    {
        if (sRaidBattleIntro->partners[partnerIndex].spriteData.teamSpriteIds[i] != MAX_SPRITES)
        {
            DestroyMonIcon(&gSprites[sRaidBattleIntro->partners[partnerIndex].spriteData.teamSpriteIds[i]]);
            sRaidBattleIntro->partners[partnerIndex].spriteData.teamSpriteIds[i] = MAX_SPRITES;
        }
    }
}

static void CreateAvailablePartnersTeamSprites(void)
{
    u32 i, j, trainerId;
    u16 selectedTeam[MULTI_PARTY_SIZE];

    for (i = 0; i < MAX_RAID_PARTNERS; i++)
    {
        CreateRaidPartnerObjectEventSprite(sRaidBattleIntro->partners[i].trainerData.graphicsId, i);
        
        trainerId = sRaidBattleIntro->partners[i].trainerData.trainerId;
        
        memset(selectedTeam, 0, sizeof(selectedTeam));
        for (j = 0; j < MULTI_PARTY_SIZE && j < gBattlePartners[trainerId].partySize; j++)
            selectedTeam[j] = gBattlePartners[trainerId].party[j].species;
        
        CreateRaidPartnerTeamSprites(selectedTeam, i);
    }
}

static void CreatePlayerTeamSprites(void)
{
    u32 i;
    u16 selectedTeam[MULTI_PARTY_SIZE];
    
    // Put player's team as first team
    CreateRaidPartnerObjectEventSprite(GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_NORMAL), 0);
    
    memset(selectedTeam, 0, sizeof(selectedTeam));
    for (i = 0; i < MULTI_PARTY_SIZE; i++)
    {
        if (gSelectedOrderFromParty[i])
            selectedTeam[i] = GetMonData(&gPlayerParty[gSelectedOrderFromParty[i] - 1], MON_DATA_SPECIES);
    }
    CreateRaidPartnerTeamSprites(selectedTeam, 0);
}

static void CreateLinkPlayerTeamSprites(u32 multiplayerId, u16 *selectedTeam)
{
    u32 graphicsId;
    u32 version = gLinkPlayers[multiplayerId].version & 0xFF;
    
    if (version == VERSION_FIRE_RED || version == VERSION_LEAF_GREEN)
        graphicsId = GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, gLinkPlayers[multiplayerId].gender);
    else
        graphicsId = GetRSAvatarGraphicsIdByGender(gLinkPlayers[multiplayerId].gender);
    
    CreateRaidPartnerObjectEventSprite(graphicsId, 1);
    CreateRaidPartnerTeamSprites(selectedTeam, 1);
}

static void DestroyPartnersTeamSprites(void)
{
    u32 i;
    
    for (i = 0; i < MAX_RAID_PARTNERS; i++)
    {
        DestroyRaidPartnerObjectEventSprite(i);
        DestroyRaidPartnerTeamSprites(i);
    }
}

///////////////////
// CURSOR SPRITE //
///////////////////

static void CreateSelectionCursorSprite(void)
{
    LoadSpriteSheet(&sRaidBattleCursorSpriteSheet);
    LoadSpritePalette(&sRaidBattleCursorSpritePalette);
    CreateSprite(&sRaidBattleCursorSpriteTemplate, 95, 59, 0);
}

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    if (sprite->data[1])
    {
        if (--sprite->data[0] == 0)
            sprite->data[1] = FALSE;
    }
    else
    {
        if (++sprite->data[0] == 20)
            sprite->data[1] = TRUE;
    }
    sprite->x2 = 5 - sprite->data[0] / 4;
    sprite->y2 = sRaidBattleIntro->cursorSelection * CURSOR_Y_SCROLL_OFFSET;
    sprite->invisible = sRaidBattleIntro->cursorIsInvisible;
}

/////////////////////
// RAID MON SPRITE //
/////////////////////

static void CreateRaidMonSprite(bool32 duringLink)
{
    u32 i;
    u16 *palette;
    u32 species = sRaidBattleIntro->raidInfo[duringLink].visualSpecies;

    sRaidBattleIntro->raidMonSpriteId = CreateMonPicSprite(species, FALSE, 0xFFFFFFFF, TRUE, 45, 57, 0, species);
    
    // Create black silhouette
    palette = &gPlttBufferUnfaded[(gSprites[sRaidBattleIntro->raidMonSpriteId].oam.paletteNum + 16) * 16];
    for (i = 0; i < 16; i++)
        *palette++ = RGB(4, 4, 4);
    
    // Create white outline
    *(palette - 1) = RGB(31, 31, 31);
}

static void DestroyRaidMonSprite(void)
{
    if (sRaidBattleIntro->raidMonSpriteId != MAX_SPRITES)
    {
        FreeAndDestroyMonPicSprite(sRaidBattleIntro->raidMonSpriteId);
        sRaidBattleIntro->raidMonSpriteId = MAX_SPRITES;
    }
}

static void OutlineRaidMonSprite(void)
{
    u32 i;
    u32 nextByteColumn, nextByteRow, previousByteRow;
    u32 pixel1, pixel2, pixel3, pixel4, pixel5, pixel6, pixel7;
    bool32 isTopBorder, isBottomBorder, isLeftBorder, isRightBorder;
    u8 buffer[(64 * 64) / 2] = {0};
    u8* offset = (void*)(OBJ_VRAM0) + (gSprites[sRaidBattleIntro->raidMonSpriteId].oam.tileNum * TILE_SIZE_4BPP);
    u8* originalOffset = offset;

    for (i = 0; i < (64 * 64) / 2; ++i)
    {
        /*Pixel Map - Bits
        6    7

        1    2    3

        4    5
        */

        /*Pixel map - Bytes
        67
        12 3
        45
        */

        nextByteColumn = 1;
        if (i % 4 == 3)
            nextByteColumn = 0x1D;

        nextByteRow = 4;
        if (i % 0x20 >= 0x1C)
            nextByteRow = 0xE4;

        previousByteRow = 4;
        if (i % 0x20 < 4)
            previousByteRow = 0xE4;

        // Next column
        pixel1 = offset[i] & 0xF;
        pixel2 = (offset[i] >> 4) & 0xF;

        pixel3 = offset[i + nextByteColumn] & 0xF;

        // Next row
        pixel4 = offset[i + nextByteRow] & 0xF;
        pixel5 = (offset[i + nextByteRow] >> 4) & 0xF;

        // Row Above
        pixel6 = offset[i - previousByteRow] & 0xF;
        pixel7 = (offset[i - previousByteRow] >> 4) & 0xF;

        isTopBorder = (i < 0xE4) && ((i % 0x20) < 4);
        isBottomBorder = (i >= 0x71C) && ((i % 0x20) >= 0x1C);
        isLeftBorder = ((i % 0x100) < 0x20) && ((i % 4) == 0);
        isRightBorder = ((i % 0x100) >= 0xE0) && ((i % 4) == 3);

        if (pixel1 != 0)
        {
            if (isTopBorder || isBottomBorder || isLeftBorder)
                buffer[i + 0] |= 0xF; // Set lower bit to white
            else
                buffer[i + 0] |= 0x1; // Set lower bit to black

            if (pixel2 == 0)
                buffer[i + 0] |= 0xF0; // Set upper bit

            if (pixel4 == 0)
                buffer[i + nextByteRow] |= 0xF; // Set lower bit

            if (i >= previousByteRow && pixel6 == 0)
                buffer[i - previousByteRow] |= 0xF; // Set lower bit
        }

        if (pixel2 != 0)
        {
            if (isTopBorder || isBottomBorder || isRightBorder) // Right border
                buffer[i + 0] |= 0xF0; // Set upper bit to white
            else
                buffer[i + 0] |= 0x10; // Set upper bit to black

            if (pixel1 == 0)
                buffer[i + 0] |= 0xF; // Set lower bit

            if (pixel3 == 0 && (i % 0x100 < 0xE0 || i % 4 != 3))
                buffer[i + nextByteColumn] |= 0xF; // Set lower bit

            if (pixel5 == 0)
                buffer[i + nextByteRow] |= 0xF0; // Set upper bit

            if (i >= previousByteRow && pixel7 == 0)
                buffer[i - previousByteRow] |= 0xF0; // Set upper bit
        }

        if (pixel3 != 0 && !isRightBorder)
        {
            if (pixel2 == 0)
                buffer[i + 0] |= 0xF0; // Set upper bit
        }
    }
    CpuCopy32(buffer, originalOffset, (64 * 64) / 2);
}

//////////////////
// STAR SPRITES //
//////////////////

static void CreateRaidStarsSprites(bool32 duringLink)
{
    u32 i, numStars = sRaidBattleIntro->raidInfo[duringLink].numStars;
    
    for (i = 0; i < RAID_STAR_COUNT - 1; i++)
    {
        if (i < numStars)
        {
            sRaidBattleIntro->starSpriteIds[i] = Create8x8SymbolSprite(9 + (9 * i), 8, 0, SYMBOL_WHITESTAR);
            gSprites[sRaidBattleIntro->starSpriteIds[i]].oam.priority = 1;
        }
        else
            sRaidBattleIntro->starSpriteIds[i] = MAX_SPRITES;
    }
}

static void DestroyRaidStarsSprites(void)
{
    u32 i;

    for (i = 0; i < RAID_STAR_COUNT - 1; i++)
    {
        if (sRaidBattleIntro->starSpriteIds[i] != MAX_SPRITES)
        {
            DestroySprite(&gSprites[sRaidBattleIntro->starSpriteIds[i]]);
            sRaidBattleIntro->starSpriteIds[i] = MAX_SPRITES;
        }
    }
}

//////////////////////
// SPRITE FUNCTIONS //
//////////////////////

static void RaidIntroCreateInterfaceSprites(u32 teamCreationState, bool32 useLinkInfo)
{
    switch (teamCreationState)
    {
        case CREATE_AVAILABLE_TEAM_SPRITES:
            CreateAvailablePartnersTeamSprites();
            break;
        case CREATE_PLAYER_SELECTED_TEAM_SPRITES:
            CreatePlayerTeamSprites();
            break;
        case CREATE_NO_TEAM_SPRITES:
            break;
    }
    CreateRaidStarsSprites(useLinkInfo);
    CreateRaidMonSprite(useLinkInfo);
}

static void RaidIntroDestroyInterfaceSprites(bool32 destroyTeamSprites)
{
    if (destroyTeamSprites)
        DestroyPartnersTeamSprites();
    
    DestroyRaidStarsSprites();
    DestroyRaidMonSprite();
}

////////////////
// INPUT TASK //
////////////////

static void CB2_RaidIntroChooseHalfParty(void)
{
    if (gSelectedOrderFromParty[0] == 0)
    {
        sRaidBattleIntro->tryLinkConnection = FALSE;
        SetMainCallback2(CB2_RaidBattleIntro);
    }
    else
    {
        if (sRaidBattleIntro->tryLinkConnection)
            SetMainCallback2(CB2_RaidBattleIntro);
        else
            StartRaidBattle(FALSE);
    }
}

static void Task_RaidBattleIntroChooseParty(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        gMain.savedCallback = CB2_RaidIntroChooseHalfParty;
        InitChooseHalfPartyForBattle(CHOOSE_MONS_FOR_CABLE_CLUB_BATTLE);
        DestroyTask(taskId);
    }
}

static void Task_RaidBattleIntroWaitForKeyPress(u32 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SUCCESS);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_RaidBattleIntroChooseParty;
    }
    else if (JOY_NEW(START_BUTTON))
    {
        if (sRaidBattleIntro->canConnectInLink)
        {
            if (!IsWirelessAdapterConnected())
            {
                PlaySE(SE_SELECT);
                sRaidBattleIntro->tryLinkConnection = TRUE;
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
                gTasks[taskId].func = Task_RaidBattleIntroChooseParty;
            }
            else
            {
                PlaySE(SE_FAILURE);
                ShowBg(0);
                RaidIntroShowMessageInBox(COMPOUND_STRING("You can't start a Raid Battle while the\nWireless Adapter is connected. Please\luse the Game Link cable instead.\p"));
                gTasks[taskId].tIsInfoMsg = TRUE;
                gTasks[taskId].func = Task_RaidBattleIntro_LinkError_WaitTextAndReturnToInput;
            }
        }
        else
            PlaySE(SE_FAILURE);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        if (sRaidBattleIntro->canCancel)
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_RaidBattleIntroFadeOutToField;
        }
        else
            PlaySE(SE_FAILURE);
    }
    else if (JOY_REPT(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        
        if (sRaidBattleIntro->cursorSelection == 0)
            sRaidBattleIntro->cursorSelection = MAX_RAID_PARTNERS - 1;
        else
            sRaidBattleIntro->cursorSelection--;
    }
    else if (JOY_REPT(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        
        if (sRaidBattleIntro->cursorSelection == MAX_RAID_PARTNERS - 1)
            sRaidBattleIntro->cursorSelection = 0;
        else
            sRaidBattleIntro->cursorSelection++;
    }
}

//////////////////////////
// LINK SEND/RCV BUFFER //
//////////////////////////

static void SendRaidBattleInfo(void)
{
    u32 i;
    struct RaidInfoSendBuffer *buffer = (struct RaidInfoSendBuffer *)gBlockSendBuffer;
    
    if (sRaidBattleIntro->isLinkMaster)
        memcpy(&buffer->raidInfo, &sRaidBattleIntro->raidInfo[FALSE], sizeof(struct RaidInfo));
    else
        memset(&buffer->raidInfo, 0, sizeof(struct RaidInfo));

    for (i = 0; i < MULTI_PARTY_SIZE; i++)
    {
        if (gSelectedOrderFromParty[i])
            buffer->selectedTeam[i] = GetMonData(&gPlayerParty[gSelectedOrderFromParty[i] - 1], MON_DATA_SPECIES);
        else
            buffer->selectedTeam[i] = SPECIES_NONE;
    }
    
    if (sRaidBattleIntro->isLinkMaster)
        Link_PrepareCmd0xCCCC_Rfu0xA100(2);
}

static void ReceiveRaidBattleInfo(u32 multiplayerId)
{
    struct RaidInfoSendBuffer *buffer = (struct RaidInfoSendBuffer *)gBlockRecvBuffer[multiplayerId];
    
    memcpy(&sRaidBattleIntro->raidInfo[TRUE], sRaidBattleIntro->isLinkMaster ? &sRaidBattleIntro->raidInfo[FALSE] : &buffer->raidInfo, sizeof(struct RaidInfo));
    
    CreateLinkPlayerTeamSprites(multiplayerId, buffer->selectedTeam);
}

static void SendRaidBattleConnectionState(u32 state)
{
    ((struct ConnectionStateSendBuffer *)gBlockSendBuffer)->state = state;
    
    if (sRaidBattleIntro->isLinkMaster)
        Link_PrepareCmd0xCCCC_Rfu0xA100(2);
}

static inline u32 GetReceivedRaidBattleConnectionState(void)
{
    return ((struct ConnectionStateSendBuffer *)gBlockRecvBuffer[0])->state;
}

///////////////////////////
// LINK CANCEL FUNCTIONS //
///////////////////////////

static void Task_RaidBattleIntro_LinkCanceled_ReturnToInput(u32 taskId)
{
    gTasks[taskId].tTimer = 0;
    gTasks[taskId].tConnected = FALSE;
    gTasks[taskId].func = gTasks[taskId].tIsInfoMsg ? Task_RaidBattleIntroWaitForKeyPress : Task_RaidIntro_LinkConnection_UpdateInterface;
    gTasks[taskId].tIsInfoMsg = FALSE;
}

static void Task_RaidBattleIntro_CloseLinkAndReturnToInput(u32 taskId)
{
    gLinkType = 0;
    CloseLink();
    HideBg(0);
    gTasks[taskId].func = Task_RaidBattleIntro_LinkCanceled_ReturnToInput;
}

static void Task_RaidBattleIntro_LinkError_ReturnToInput(u32 taskId)
{
    HideBg(0);
    gTasks[taskId].func = Task_RaidBattleIntro_LinkCanceled_ReturnToInput;
}

static void Task_RaidBattleIntro_LinkError_WaitTextAndReturnToInput(u32 taskId)
{
    if (!RaidIntro_IsTextPrinterActive())
        gTasks[taskId].func = Task_RaidBattleIntro_LinkError_ReturnToInput;
}

static void Task_RaidBattleIntro_LinkErroredOut(u32 taskId)
{
    CloseLink();
    RaidIntroShowMessageInBox(COMPOUND_STRING("Sorry, we have a link error…\nPlease reset and try again.\p"));
    gTasks[taskId].func = Task_RaidBattleIntro_LinkError_WaitTextAndReturnToInput;
}

// Cancel link if pressed B button, only if connection hasn't been established
static bool32 HandleLinkCancel(u32 taskId)
{
    if (JOY_NEW(B_BUTTON) && !IsLinkConnectionEstablished())
    {
        gTasks[taskId].func = Task_RaidBattleIntro_CloseLinkAndReturnToInput;
        return TRUE;
    }
    return FALSE;
}

// Check if an SIO reg error occurred while receiving link players
static bool32 CheckSIOLinkErrorOccurred(u32 taskId)
{
    if (GetSioMultiSI())
    {
        gTasks[taskId].func = Task_RaidBattleIntro_LinkErroredOut;
        return TRUE;
    }
    return FALSE;
}

// Checks if an internal link error occurred
static bool32 CheckLinkErrorOccurred(u32 taskId)
{
    if (HasLinkErrorOccurred())
    {
        gTasks[taskId].func = Task_RaidBattleIntro_LinkErroredOut;
        return TRUE;
    }
    return FALSE;
}

// Check link connection has timed out, if so, an error occurred
static bool32 CheckLinkConnectionTimedOut(u32 taskId)
{
    if (++gTasks[taskId].tTimer > 600)
    {
        gTasks[taskId].func = Task_RaidBattleIntro_LinkErroredOut;
        return TRUE;
    }
    return FALSE;
}

// Only used for the initial link connection step
static bool32 WasLinkCanceled(u32 taskId)
{
    if (IsLinkConnectionEstablished())
        SetSuppressLinkErrorMessage(TRUE);

    if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_RaidBattleIntro_CloseLinkAndReturnToInput;
        return TRUE;
    }
    return FALSE;
}

static void Task_RaidIntro_LinkConnection_WaitAndSetFailStatus(u32 taskId)
{
    if (!gReceivedRemoteLinkPlayers)
    {
        switch (gTasks[taskId].tStatus)
        {
            case 3:
                CloseLink();
                RaidIntroShowMessageInBox(COMPOUND_STRING("The link partner appear to have\nmade a different selection.\p"));
                gTasks[taskId].func = Task_RaidBattleIntro_LinkError_WaitTextAndReturnToInput;
                break;
            case 4:
                CloseLink();
                RaidIntroShowMessageInBox(COMPOUND_STRING("There must be a partner to enter\nthe Raid Battle.\p"));
                gTasks[taskId].func = Task_RaidBattleIntro_LinkError_WaitTextAndReturnToInput;
                break;
            case 7:
            case 9:
                gTasks[taskId].func = Task_RaidBattleIntro_LinkError_ReturnToInput;
                break;
        }
    }
}

static void HandleLinkStatusCheck(u32 taskId)
{
    switch (gTasks[taskId].tStatus)
    {
        case 3:
        case 4:
            if (sRaidBattleIntro->isLinkMaster && AnyConnectedPartnersPlayingRS())
                CloseLink();
            else
                SetCloseLinkCallback();
            
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitAndSetFailStatus;
            break;
        case 7:
        case 9:
            CloseLink();
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitAndSetFailStatus;
            break;
        default:
            gFieldLinkPlayerCount = GetLinkPlayerCount_2();
            gLocalLinkPlayerId = GetMultiplayerId();
            sub_800A900(gFieldLinkPlayerCount);
            SendRaidBattleInfo();

            gTasks[taskId].tConnected = TRUE;
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_ReceiveSelectedTeamSlots;
            break;
    }
}

///////////////////////////////
// LINK CONNECTION FUNCTIONS //
///////////////////////////////

static void Task_RaidBattleIntroWaitTextAndStartLinkConnection(u32 taskId)
{
    if (!RaidIntro_IsTextPrinterActive())
    {
        gLinkType = LINKTYPE_RAID_BATTLE;
        OpenLinkTimed();
        sub_800AA24();
        ResetLinkPlayers();
        
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].func = Task_RaidBattleIntroStartLinkConnectionAfterTimer;
    }
}

static void Task_RaidBattleIntroStartLinkConnectionAfterTimer(u32 taskId)
{
    if (gTasks[taskId].tTimer > 9)
    {
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].func = Task_RaidBattleIntroWaitForLinkPlayers;
    }
    else
        gTasks[taskId].tTimer++;
}

static void Task_RaidBattleIntroWaitForLinkPlayers(u32 taskId)
{
    if (!HandleLinkCancel(taskId) && !WasLinkCanceled(taskId) && GetLinkPlayerCount_2() >= 2)
    {
        SetSuppressLinkErrorMessage(TRUE);
        
        sRaidBattleIntro->isLinkMaster = IsLinkMaster();
        
        if (sRaidBattleIntro->isLinkMaster)
        {
            PlaySE(SE_PIN);
            RaidIntroShowMessageInBox(sText_PlayerWantToEnterRaidBattle);
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_Master;
        }
        else
        {
            PlaySE(SE_BOO);
            RaidIntroShowMessageInBox(CableClub_Text_AwaitingLinkupBCancel);
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_Slave;
        }
    }
}

static void Task_RaidIntro_LinkConnection_ReceiveSelectedTeamSlots(u32 taskId)
{
    if (!CheckLinkErrorOccurred(taskId) && GetBlockReceivedStatus() == sub_800A8A4())
    {
        SetSuppressLinkErrorMessage(FALSE);
        ResetBlockReceivedFlags();
        
        if (gTasks[taskId].tStatus == 1)
        {
            gTasks[taskId].tTimer = 0;
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_UpdateInterface;
        }
        else
        {
            SetCloseLinkCallback();
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitAndSetFailStatus;
        }
    }
}

static void Task_RaidIntro_LinkConnection_UpdateInterface(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    switch (tTimer)
    {
        case 0:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tTimer++;
            break;
        case 1:
            if (!gPaletteFade.active)
            {
                RaidIntroDestroyInterfaceSprites(!tConnected);
                tTimer++;
            }
            break;
        case 2:
            sRaidBattleIntro->updatingScreen = TRUE;
            
            if (tConnected) // Fading to receive a new link player
            {
                u32 i, multiplayerId = GetMultiplayerId();
                
                for (i = 0; i < GetLinkPlayerCount(); i++)
                {
                    if (i != multiplayerId)
                        ReceiveRaidBattleInfo(i);
                }
            }
            tTimer++;
            break;
        case 3:
            RaidIntroPrintInterfaceTexts(tConnected, tConnected);
            tTimer++;
            break;
        case 4:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            tTimer++;
            break;
        case 5:
            if (!tConnected)
                sRaidBattleIntro->cursorIsInvisible = FALSE;

            RaidIntroCreateInterfaceSprites(tConnected ? CREATE_NO_TEAM_SPRITES : CREATE_AVAILABLE_TEAM_SPRITES, tConnected);
            tTimer++;
            break;
        case 6 ... 6 + WHITE_OUTLINE_LENGTH - 1:
            sRaidBattleIntro->updatingScreen = FALSE;
            OutlineRaidMonSprite();
            tTimer++;
            break;
        case 6 + WHITE_OUTLINE_LENGTH:
            if (!gPaletteFade.active)
            {
                tTimer = 0;
                
                if (tConnected) // Fading to receive a new link player
                {
                    if (sRaidBattleIntro->isLinkMaster)
                        RaidIntroShowMessageInBox(CableClub_Text_WhenAllPlayersReadyAConfirmBCancel);
                    else
                    {
                        SendRaidBattleConnectionState(RAID_CONN_STATE_START_BATTLE);
                        RaidIntroShowMessageInBox(COMPOUND_STRING("Awaiting battle start…"));
                    }
                    gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitConfirmationText;
                }
                else
                    gTasks[taskId].func = Task_RaidBattleIntroWaitForKeyPress;
            }
            break;
    }
}

static void Task_RaidIntro_LinkConnection_WaitConfirmationText(u32 taskId)
{
    if (!CheckLinkErrorOccurred(taskId) && !RaidIntro_IsTextPrinterActive())
    {
        if (!sRaidBattleIntro->isLinkMaster)
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitReceivedLinkState;
        else
        {
            if (JOY_NEW(B_BUTTON))
            {
                SendRaidBattleConnectionState(RAID_CONN_STATE_CANCEL_BATTLE);
                gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitReceivedLinkState;
            }
            else if (JOY_NEW(A_BUTTON))
            {
                PlaySE(SE_SELECT);
                SendRaidBattleConnectionState(RAID_CONN_STATE_START_BATTLE);
                gTasks[taskId].func = Task_RaidIntro_LinkConnection_WaitReceivedLinkState;
            }
        }
    }
}

static void Task_RaidIntro_LinkConnection_WaitReceivedLinkState(u32 taskId)
{
    if (!CheckLinkErrorOccurred(taskId) && GetBlockReceivedStatus() == sub_800A8A4())
    {
        SetSuppressLinkErrorMessage(FALSE);
        ResetBlockReceivedFlags();

        if (GetReceivedRaidBattleConnectionState() == RAID_CONN_STATE_START_BATTLE)
        {
            sub_8057F34();
            SetLocalLinkPlayerId(sRaidBattleIntro->isLinkMaster ? 0 : 2);
            sub_8057F70();
            
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_StartBattle;
        }
        else
            gTasks[taskId].func = Task_RaidBattleIntro_CloseLinkAndReturnToInput;
    }
}

static void Task_RaidIntro_LinkConnection_StartBattle(u32 taskId)
{
    gLinkType = LINKTYPE_BATTLE;
    ClearLinkCallback_2();
    gTasks[taskId].tTimer = 0;
    gTasks[taskId].func = Task_RaidIntro_LinkConnection_StartBattle_Step2;
}

static void Task_RaidIntro_LinkConnection_StartBattle_Step2(u32 taskId)
{
    if (++gTasks[taskId].tTimer > 20)
    {
        SetCloseLinkCallback();
        gTasks[taskId].func = Task_RaidIntro_LinkConnection_StartBattle_Step3;
    }
}

static void Task_RaidIntro_LinkConnection_StartBattle_Step3(u32 taskId)
{
    if (!gReceivedRemoteLinkPlayers)
    {
        StartLinkRaidBattleTransition();
        DestroyTask(taskId);
    }
}

//////////////////////////////
// LINK CONNECTION - MASTER //
//////////////////////////////

static void Task_RaidIntro_LinkConnection_Master(u32 taskId)
{
    if (!HandleLinkCancel(taskId) && !CheckSIOLinkErrorOccurred(taskId) && !CheckLinkErrorOccurred(taskId) && !RaidIntro_IsTextPrinterActive())
    {
        s32 linkPlayerCount = GetLinkPlayerCount_2();
        
        if (JOY_NEW(A_BUTTON) && linkPlayerCount == 2)
        {
            PlaySE(SE_SELECT);
            sub_800A900(linkPlayerCount);
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_Master_Step2;
        }
    }
}

static void Task_RaidIntro_LinkConnection_Master_Step2(u32 taskId)
{
    if (!HandleLinkCancel(taskId) && !CheckSIOLinkErrorOccurred(taskId) && !CheckLinkErrorOccurred(taskId))
    {
        if (GetSavedPlayerCount() != GetLinkPlayerCount_2() || JOY_HELD(B_BUTTON))
        {
            RaidIntroShowMessageInBox(sText_PlayerWantToEnterRaidBattle);
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_Master;
        }
        else
        {
            CheckShouldAdvanceLinkState();
            gTasks[taskId].func = Task_RaidIntro_LinkConnection_Master_Step3;
        }
    }
}

static void Task_RaidIntro_LinkConnection_Master_Step3(u32 taskId)
{
    if (!CheckLinkErrorOccurred(taskId) && !CheckLinkConnectionTimedOut(taskId))
    {
        if (GetLinkPlayerCount_2() != GetSavedPlayerCount())
            gTasks[taskId].func = Task_RaidBattleIntro_LinkErroredOut;
        else
        {
            gTasks[taskId].tStatus = sub_8080844(2, 2);
            
            if (gTasks[taskId].tStatus)
                gTasks[taskId].func = Task_RaidIntro_LinkConnection_Master_Step4;
        }
    }
}

static void Task_RaidIntro_LinkConnection_Master_Step4(u32 taskId)
{
    if (!CheckLinkErrorOccurred(taskId))
        HandleLinkStatusCheck(taskId);
}

/////////////////////////////
// LINK CONNECTION - SLAVE //
/////////////////////////////

static void Task_RaidIntro_LinkConnection_Slave(u32 taskId)
{
    if (!HandleLinkCancel(taskId) && !CheckLinkErrorOccurred(taskId))
    {
        gTasks[taskId].tStatus = sub_8080844(2, 2);
        
        if (gTasks[taskId].tStatus)
            HandleLinkStatusCheck(taskId);
    }
}

#undef tTimer
#undef tStatus
#undef tConnected
#undef tIsInfoMsg

///////////////////////
// RAID MON CREATION //
///////////////////////

static void CreateRaidMon(bool32 isLink)
{
    struct RaidInfo *raidInfo = &sRaidBattleIntro->raidInfo[isLink];

    GenerateWildMon(raidInfo->actualSpecies, raidInfo->level, TRUE, min(raidInfo->numStars - 1, NUM_STATS));
    
    // Give gigantamax factor if needed
    if (raidInfo->visualSpecies != raidInfo->actualSpecies)
    {
        bool32 gmaxFactor = TRUE;
        SetMonData(&gEnemyParty[0], MON_DATA_GIGANTAMAX_FACTOR, &gmaxFactor);
    }
    
    // Give held item
    SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &raidInfo->item);
    
    // Try give egg move
    if (RandomPercentage(RNG_RAID_EGG_MOVE_CHANCE, sRaidStarsData[raidInfo->numStars - 1].eggMoveChance))
    {
        u16 eggMoveBuffer[EGG_MOVES_ARRAY_COUNT];
        u32 numEggMoves = GetEggMoves(&gEnemyParty[0], eggMoveBuffer);
        
        if (numEggMoves)
        {
            u32 move = eggMoveBuffer[RandomUniform(RNG_RAID_EGG_MOVE_ID, 0, numEggMoves - 1)];
            
            if (FindMoveSlotInMoveset(&gEnemyParty[0], move) != MAX_MON_MOVES) // Try to reroll once if mon already knows move
                move = eggMoveBuffer[RandomUniform(RNG_RAID_EGG_MOVE_ID, 0, numEggMoves - 1)];
            
            if (GiveMoveToMon(&gEnemyParty[0], move) == MON_HAS_MAX_MOVES)
                DeleteFirstMoveAndGiveMoveToMon(&gEnemyParty[0], move);
        }
    }
}

//////////////////
// RAID REWARDS //
//////////////////

static u32 GetRaidItemRewardAmount(u32 item)
{
    if (item >= ITEM_RED_SHARD && item <= ITEM_GREEN_SHARD)
        return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 10);
    
    switch (ItemId_GetPocket(item))
    {
        case POCKET_POKE_BALLS:
            return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 5);
        case POCKET_BERRY_POUCH:
            switch (ItemId_GetHoldEffect(item))
            {
                case HOLD_EFFECT_CURE_STATUS:
                case HOLD_EFFECT_RESTORE_PP:
                case HOLD_EFFECT_CONFUSE_FLAVOR:
                    return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 3);
                case HOLD_EFFECT_STAT_UP:
                case HOLD_EFFECT_RESIST_BERRY:
                    return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 2);
                case HOLD_EFFECT_CRITICAL_UP:
                case HOLD_EFFECT_RANDOM_STAT_UP:
                case HOLD_EFFECT_MICLE_BERRY:
                case HOLD_EFFECT_ENIGMA_BERRY:
                case HOLD_EFFECT_DAMAGE_ON_SPLIT:
                case HOLD_EFFECT_CUSTAP_BERRY:
                case HOLD_EFFECT_STAT_UP_PHYSICAL_HIT:
                case HOLD_EFFECT_STAT_UP_SPECIAL_HIT:
                    return 1;
                default:
                    return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 4);
            }
            break;
    }
    
    switch (ItemId_GetUsageType(item))
    {
        case ITEM_TYPE_HEALTH_RECOVERY:
        case ITEM_TYPE_STATUS_RECOVERY:
            return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 5);
        case ITEM_TYPE_PP_RECOVERY:
        case ITEM_TYPE_STAT_BOOST_DRINK:
            return RandomUniform(RNG_RAID_DROP_AMOUNT, 1, 3);
        case ITEM_TYPE_STAT_BOOST_WING:
            return RandomUniform(RNG_RAID_DROP_AMOUNT, 10, 30);
    }
    return 1;
}

static void SelectRandomRaidBattleRewards(struct ItemSlot *output, const u16 *drops)
{
    u32 i, numDrops = 0;
    
    for (i = 0; i < MAX_RAID_DROPS; i++)
    {
        if (!drops[i])
            break;
        
        if (RandomPercentage(RNG_RAID_DROP_ITEM, sRaidBattleDropChances[i]))
        {
            output[numDrops].itemId = drops[i];
            output[numDrops].quantity = GetRaidItemRewardAmount(drops[i]);
            numDrops++;
        }
    }
}

void GiveRaidBattleRewards(void)
{
    u32 index = (*GetVarPointer(VAR_TEMP_0))++;
    
    if (index < MAX_RAID_DROPS && gRaidBattleDrops[index].itemId)
    {
        VarSet(VAR_TEMP_1, gRaidBattleDrops[index].itemId);
        VarSet(VAR_TEMP_2, gRaidBattleDrops[index].quantity);
        
        gSpecialVar_Result = FALSE;
    }
    else
        gSpecialVar_Result = TRUE; // Done giving rewards
}

////////////////////
// UTIL FUNCTIONS //
////////////////////

void SetRaidClearedFlag(void)
{
    FlagSet(gMapSectionsInfo[GetCurrentRegionMapSectionId()].raidClearedFlagId);
}

void ClearRaidClearedFlag(void)
{
    FlagClear(gMapSectionsInfo[GetCurrentRegionMapSectionId()].raidClearedFlagId);
}

bool32 IsRaidClearedFlagSet(void)
{
    return FlagGet(gMapSectionsInfo[GetCurrentRegionMapSectionId()].raidClearedFlagId);
}

bool32 IsRaidBattleAvailable(void)
{
    return (!IsRaidClearedFlagSet() && TryGetRaidBattleData(FALSE));
}

static u32 GetRaidRandomNumber(void)
{
    u8 lastWarpId;
    u16 lastPos;
    u32 offset;
    // Make sure no values are 0
    u32 dayOfWeek = gRtcLocation.dayOfWeek == 0 ? 8 : gRtcLocation.dayOfWeek;
    u32 hour = gRtcLocation.hour == 0 ? HOURS_PER_DAY : gRtcLocation.hour;
    u32 day = gRtcLocation.day == 0 ? 32 : gRtcLocation.day;
    u32 month = gRtcLocation.month == 0 ? MONTH_COUNT + 1 : gRtcLocation.month;
    u8 lastMapGroup = gSaveBlock1Ptr->dynamicWarp.mapGroup == 0 ? 0xFF : gSaveBlock1Ptr->dynamicWarp.mapGroup;
    u8 lastMapNum = gSaveBlock1Ptr->dynamicWarp.mapNum == 0 ? 0xFF : gSaveBlock1Ptr->dynamicWarp.mapNum;
    
    if (gMapHeader.mapType == MAP_TYPE_UNDERWATER || gMapHeader.mapType == MAP_TYPE_UNDERGROUND)
    {
        // Only use position not in routes because otherwise spawns may be affected unintentionally due to multi-entrance hidden grottos
        lastWarpId = gSaveBlock1Ptr->dynamicWarp.warpId == 0 ? 0xFF : gSaveBlock1Ptr->dynamicWarp.warpId;
        lastPos = (gSaveBlock1Ptr->dynamicWarp.x + gSaveBlock1Ptr->dynamicWarp.y == 0) ? 0xFFFF : (u16)(gSaveBlock1Ptr->dynamicWarp.x + gSaveBlock1Ptr->dynamicWarp.y);
    }
    else
    {
        lastWarpId = 1;
        lastPos = 0;
    }
    offset = VarGet(VAR_WISHING_PIECE_VALUE);
    
    return ((hour * (day + month) * lastMapGroup * (lastMapNum + lastWarpId + lastPos)) + ((hour * (day + month)) ^ dayOfWeek) + offset) ^ GetPlayerTrainerId();
}

void RollWishingPieceRandomValue(void)
{
    VarSet(VAR_WISHING_PIECE_VALUE, RandomUniform(RNG_WISHING_PIECE, 0, 0xFFFF));
}

u32 GetRaidRecommendedLevel(u32 numStars)
{
    return sRaidStarsData[numStars - 1].maxLevel + 5;
}

static const struct RaidData *GetCurrentMapRaidData(u32 numStars)
{
    return &gMapSectionsInfo[GetCurrentRegionMapSectionId()].raids[numStars - 1];
}

static void StartRaidBattle(bool32 isLink)
{
    ReducePlayerPartyToThree();
    
    gNumRaidBattleStars = sRaidBattleIntro->raidInfo[isLink].numStars;
    
    if (!isLink)
    {
        gBattleTypeFlags = (BATTLE_TYPE_RAID | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_ONE_VS_TWO | BATTLE_TYPE_INGAME_PARTNER);
        gPartnerTrainerId = sRaidBattleIntro->partners[sRaidBattleIntro->cursorSelection].trainerData.trainerId;
        FillPartnerParty(gPartnerTrainerId);
    }
    else
        gBattleTypeFlags = (BATTLE_TYPE_RAID | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_ONE_VS_TWO | BATTLE_TYPE_LINK);

    CreateRaidMon(isLink);
    FlagSet(FLAG_DYNAMAX_ENABLED);
    
    PlayMapChosenOrBattleBGM(MUS_VS_LEGEND);
    gMain.savedCallback = isLink ? CB2_ReturnFromLinkRaidBattle : CB2_ReturnFromRaidBattle;
    SetMainCallback2(CB2_InitBattle);
    
    memcpy(gRaidBattleDrops, &sRaidBattleIntro->raidInfo[isLink].drops, sizeof(gRaidBattleDrops));
    
    RaidIntroFreeResources();
}

bool32 WasRaidBossDefeated(void)
{
    return (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT);
}

bool32 WasRaidBossCaught(void)
{
    return gCapturedWildMon;
}

static void CB2_ReturnFromLinkRaidBattle(void)
{
    switch (gMain.state)
    {
        case 0:
            gBattleTypeFlags &= ~(BATTLE_TYPE_LINK_ESTABLISHED);
            ClearLinkCallback_2();
            SetCloseLinkCallback();
            gMain.state++;
            break;
        case 1:
            if (!gReceivedRemoteLinkPlayers)
            {
                CloseLink();
                gMain.state++;
            }
            break;
        case 2:
            CB2_ReturnFromRaidBattle();
            break;
    }
}

static void CB2_ReturnFromRaidBattle(void)
{
    gSpecialVar_0x8004 = TRUE;
    
    LoadPlayerParty();
    
    if (WasRaidBossDefeated())
        InitRaidClearScreen(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES), gNumRaidBattleStars, WasRaidBossCaught());
    else
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

u32 GiveRaidBattleBoss(void)
{
    u32 hp;
    struct Pokemon *mon = &gEnemyParty[0];
    
    VarSet(VAR_TEMP_1, GetMonData(mon, MON_DATA_SPECIES));
    
    // Restore hp
    hp = GetMonData(mon, MON_DATA_MAX_HP);
    SetMonData(mon, MON_DATA_HP, &hp);
    
    // Give to player
    return GiveScriptCreatedMonToPlayer(mon, TRUE);
}

////////////////////////////
// RAID BATTLE TRANSITION //
////////////////////////////

#define tTransitionCurrentVal   data[0]
#define tTransitionIncrementVal data[1]

static void Task_RunLinkRaidBattleTransitionEffect(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    tTransitionCurrentVal += tTransitionIncrementVal;
    
    SetGpuReg(REG_OFFSET_WIN0V, tTransitionCurrentVal > 160 ? WIN_RANGE(0, 160) : tTransitionCurrentVal);
    
    if (tTransitionCurrentVal == 160)
    {
        StartRaidBattle(TRUE);
        DestroyTask(taskId);
    }
}

static void StartLinkRaidBattleTransition(void)
{
    u32 taskId;
    
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 0));
    
    taskId = CreateTask(Task_RunLinkRaidBattleTransitionEffect, 0);
    gTasks[taskId].tTransitionCurrentVal = -16;
    gTasks[taskId].tTransitionIncrementVal = 16;
}

#undef tTransitionCurrentVal
#undef tTransitionIncrementVal
