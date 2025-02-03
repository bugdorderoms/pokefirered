#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_bg.h"
#include "battle_message.h"
#include "decompress.h"
#include "graphics.h"
#include "link.h"
#include "menu.h"
#include "menu_indicators.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "strings.h"
#include "text_window.h"
#include "trig.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/move_effect_bytes.h"
#include "constants/songs.h"
#include "constants/trainers.h"

#define TAG_VS_LETTERS 10000

static u8 GetBattleTerrainOverride(void);

#include "data/battle_backgrounds.h"

static const struct OamData gOamData_82482A0 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData gOamData_82482A8 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0x040,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AffineAnimCmd gUnknown_82482B0[] = {
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd gUnknown_82482C0[] = {
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(0x18, 0x18, 0x0, 0x80),
    AFFINEANIMCMD_FRAME(0x18, 0x18, 0x0, 0x80),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const gAffineAnimTable_82482E0[] = {
    gUnknown_82482B0,
    gUnknown_82482C0
};

static const struct SpriteTemplate sVsLetter_V_SpriteTemplate = {
    .tileTag = TAG_VS_LETTERS,
    .paletteTag = TAG_VS_LETTERS,
    .oam = &gOamData_82482A0,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gAffineAnimTable_82482E0,
    .callback = SpriteCB_VsLetterDummy
};

static const struct SpriteTemplate sVsLetter_S_SpriteTemplate = {
    .tileTag = TAG_VS_LETTERS,
    .paletteTag = TAG_VS_LETTERS,
    .oam = &gOamData_82482A8,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gAffineAnimTable_82482E0,
    .callback = SpriteCB_VsLetterDummy
};

static const struct CompressedSpriteSheet sVsLettersSpriteSheet = {
    gVsLettersGfx,
    0x1000,
    TAG_VS_LETTERS
};

const struct BgTemplate gBattleBgTemplates[4] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 24,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x000
    }
};

static const struct WindowTemplate gUnknown_8248330[] = {
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 0x090
    }, {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 35,
        .width = 14,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 0x1c0
    }, {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 35,
        .width = 12,
        .height = 4,
        .paletteNum = 5,
        .baseBlock = 0x190
    }, {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 55,
        .width = 8,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x300
    }, {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 55,
        .width = 8,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x310
    }, {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 57,
        .width = 8,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x320
    }, {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 57,
        .width = 8,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x330
    }, {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 55,
        .width = 3,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x290
    }, {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 57,
        .width = 8,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x296
    }, {
        .bg = 0,
        .tilemapLeft = 24,
        .tilemapTop = 55,
        .width = 5,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x2a6
    }, {
        .bg = 0,
        .tilemapLeft = 25,
        .tilemapTop = 57,
        .width = 0,
        .height = 0,
        .paletteNum = 5,
        .baseBlock = 0x2b0
    }, {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 55,
        .width = 8,
        .height = 4,
        .paletteNum = 5,
        .baseBlock = 0x2b0
    }, {
        .bg = 1,
        .tilemapLeft = 19,
        .tilemapTop = 8,
        .width = 10,
        .height = 11,
        .paletteNum = 5,
        .baseBlock = 0x100
    }, {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 0,
        .width = 12,
        .height = 3,
        .paletteNum = 6,
        .baseBlock = 0x16e
    }, {
        .bg = 0,
        .tilemapLeft = 25,
        .tilemapTop = 9,
        .width = 4,
        .height = 4,
        .paletteNum = 5,
        .baseBlock = 0x100
    }, {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 7,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x020
    }, {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 7,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x040
    }, {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 2,
        .width = 7,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x020
    }, {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 2,
        .width = 7,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x040
    }, {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 6,
        .width = 7,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x060
    }, {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 6,
        .width = 7,
        .height = 2,
        .paletteNum = 5,
        .baseBlock = 0x080
    }, {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 2,
        .width = 8,
        .height = 2,
        .paletteNum = 0,
        .baseBlock = 0x0a0
    }, {
        .bg = 0,
        .tilemapLeft = 4,
        .tilemapTop = 2,
        .width = 8,
        .height = 2,
        .paletteNum = 0,
        .baseBlock = 0x0a0
    }, {
        .bg = 0,
        .tilemapLeft = 19,
        .tilemapTop = 2,
        .width = 8,
        .height = 2,
        .paletteNum = 0,
        .baseBlock = 0x0b0
    }, {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 7,
        .baseBlock = 0x090
    }, DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sMoveInfoWindowTemplates[] =
{
	{ // Move's name
		.bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 52,
		.width = 11,
        .height = 2,
		.paletteNum = 1,
        .baseBlock = 0x340
	},
	{ // Move's description
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 54,
        .width = 20,
        .height = 6,
        .paletteNum = 1,
        .baseBlock = 0x356
    },
};

static const struct {
    u8 mapScene;
    u8 battleTerrain;
} sMapBattleSceneMapping[] = {
    {MAP_BATTLE_SCENE_GYM,      BATTLE_TERRAIN_GYM},
    {MAP_BATTLE_SCENE_INDOOR_1, BATTLE_TERRAIN_INDOOR_1},
    {MAP_BATTLE_SCENE_INDOOR_2, BATTLE_TERRAIN_INDOOR_2},
    {MAP_BATTLE_SCENE_LORELEI,  BATTLE_TERRAIN_LORELEI},
    {MAP_BATTLE_SCENE_BRUNO,    BATTLE_TERRAIN_BRUNO},
    {MAP_BATTLE_SCENE_AGATHA,   BATTLE_TERRAIN_AGATHA},
    {MAP_BATTLE_SCENE_LANCE,    BATTLE_TERRAIN_LANCE},
    {MAP_BATTLE_SCENE_LINK,     BATTLE_TERRAIN_LINK}
};

static u8 GetBattleTerrainByMapScene(u8 mapBattleScene)
{
    u32 i;
	
    for (i = 0; i < ARRAY_COUNT(sMapBattleSceneMapping); i++)
    {
        if (mapBattleScene == sMapBattleSceneMapping[i].mapScene)
            return sMapBattleSceneMapping[i].battleTerrain;
    }
    return BATTLE_TERRAIN_PLAIN;
}

void LoadBattleTerrainGfx(u8 terrain)
{
    if (terrain >= BATTLE_TERRAINS_COUNT)
        terrain = BATTLE_TERRAIN_PLAIN;
    // Copy to bg3
    LZDecompressVram(gBattleTerrainTable[terrain].tileset, (void *)BG_CHAR_ADDR(2));
    LZDecompressVram(gBattleTerrainTable[terrain].tilemap, (void *)BG_SCREEN_ADDR(26));
    LoadCompressedPalette(gBattleTerrainTable[terrain].palette, 0x20, 0x60);
}

static void LoadBattleTerrainEntryGfx(u8 terrain)
{
    if (terrain >= BATTLE_TERRAINS_COUNT)
        terrain = BATTLE_TERRAIN_PLAIN;
    // Copy to bg1
    LZDecompressVram(gBattleTerrainTable[terrain].entryTileset, (void *)BG_CHAR_ADDR(1));
    LZDecompressVram(gBattleTerrainTable[terrain].entryTilemap, (void *)BG_SCREEN_ADDR(28));
}

void BattleInitBgsAndWindows(void)
{
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, gBattleBgTemplates, ARRAY_COUNT(gBattleBgTemplates));
    InitWindows(gUnknown_8248330);
    DeactivateAllTextPrinters();
}

void InitBattleBgsVideo(void)
{
    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    BattleInitBgsAndWindows();
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_OBJWIN_ON);
}

void LoadBattleMenuWindowGfx(void)
{
    TextWindow_SetUserSelectedFrame(2, 0x012, 0x10);
    TextWindow_SetUserSelectedFrame(2, 0x022, 0x10);
    gPlttBufferUnfaded[0x5C] = RGB( 9,  9,  9);
    gPlttBufferUnfaded[0x5D] = RGB( 9,  9,  9);
    gPlttBufferUnfaded[0x5E] = RGB(31, 31, 31);
    gPlttBufferUnfaded[0x5F] = RGB( 26,  26,  25);
    CpuCopy16(&gPlttBufferUnfaded[0x5C], &gPlttBufferFaded[0x5C], 8);
    if (gBattleTypeFlags & (BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_POKEDUDE))
    {
        Menu_LoadStdPalAt(0x70, 0x14);
        TextWindow_LoadResourcesStdFrame0(0, 0x030, 0x70);
        gPlttBufferUnfaded[0x76] = RGB( 0,  0,  0);
        CpuCopy16(&gPlttBufferUnfaded[0x76], &gPlttBufferFaded[0x76], 2);
    }
}

void DrawMainBattleBackground(void)
{
	switch (gFieldStatus & STATUS_FIELD_TERRAINS_ANY)
	{
		case STATUS_FIELD_GRASSY_TERRAIN:
			LoadMoveBg(BG_GRASSY_TERRAIN);
			break;
		case STATUS_FIELD_MISTY_TERRAIN:
			LoadMoveBg(BG_MISTY_TERRAIN);
			break;
		case STATUS_FIELD_ELECTRIC_TERRAIN:
			LoadMoveBg(BG_ELECTRIC_TERRAIN);
			break;
		case STATUS_FIELD_PSYCHIC_TERRAIN:
			LoadMoveBg(BG_PSYCHIC_TERRAIN);
			break;
		default:
			LoadBattleTerrainGfx(GetBattleTerrainOverride());
			break;
	}
}

void LoadBattleTextboxAndBackground(void)
{
    LZDecompressVram(gBattleTextboxTiles, (void *)BG_CHAR_ADDR(0));
    CopyToBgTilemapBuffer(0, gBattleTextboxTilemap, 0, 0x000);
    CopyBgTilemapBufferToVram(0);
    LoadCompressedPalette(gBattleTextboxPalette, 0x00, 0x40);
    LoadBattleMenuWindowGfx();
    DrawMainBattleBackground();
}

static void DrawLinkBattleParticipantPokeballs(u8 taskId, u8 multiplayerId, u8 bgId, u8 destX, u8 destY)
{
    s32 i;
    u16 pokeballStatuses = 0;
    u16 tiles[6];

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gTasks[taskId].data[5] != 0)
        {
            switch (multiplayerId)
            {
            case 0:
                pokeballStatuses = 0x3F & gTasks[taskId].data[3];
                break;
            case 1:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[4]) >> 6;
                break;
            case 2:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[3]) >> 6;
                break;
            case 3:
                pokeballStatuses = 0x3F & gTasks[taskId].data[4];
                break;
            }
        }
        else
        {
            switch (multiplayerId)
            {
            case 0:
                pokeballStatuses = 0x3F & gTasks[taskId].data[3];
                break;
            case 1:
                pokeballStatuses = 0x3F & gTasks[taskId].data[4];
                break;
            case 2:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[3]) >> 6;
                break;
            case 3:
                pokeballStatuses = (0xFC0 & gTasks[taskId].data[4]) >> 6;
                break;
            }
        }

        for (i = 0; i < 3; i++)
            tiles[i] = ((pokeballStatuses & (3 << (i * 2))) >> (i * 2)) + 0x6001;

        CopyToBgTilemapBufferRect_ChangePalette(bgId, tiles, destX, destY, 3, 1, 0x11);
        CopyBgTilemapBufferToVram(bgId);
    }
    else
    {
		pokeballStatuses = multiplayerId == gBattleStruct->multiplayerId ? gTasks[taskId].data[3] : gTasks[taskId].data[4];
		
        for (i = 0; i < 6; i++)
            tiles[i] = ((pokeballStatuses & (3 << (i * 2))) >> (i * 2)) + 0x6001;

        CopyToBgTilemapBufferRect_ChangePalette(bgId, tiles, destX, destY, 6, 1, 0x11);
        CopyBgTilemapBufferToVram(bgId);
    }
}

static void DrawLinkBattleVsScreenOutcomeText(void)
{
    if (gBattleOutcome == B_OUTCOME_DREW)
        BattlePutTextOnWindow(gText_Draw, B_WIN_VS_OUTCOME_DRAW);
    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gBattleOutcome == B_OUTCOME_WON)
        {
            switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
            {
            case 0:
			case 2:
                BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_LEFT);
                BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_RIGHT);
                break;
            case 1:
			case 3:
                BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_RIGHT);
                BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_LEFT);
                break;
            }
        }
        else
        {
            switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
            {
            case 0:
			case 2:
                BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_RIGHT);
                BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_LEFT);
                break;
            case 1:
			case 3:
                BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_LEFT);
                BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_RIGHT);
                break;
            }
        }
    }
    else if (gBattleOutcome == B_OUTCOME_WON)
    {
        if (gLinkPlayers[gBattleStruct->multiplayerId].id != 0)
        {
            BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_RIGHT);
            BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_LEFT);
        }
        else
        {
            BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_LEFT);
            BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_RIGHT);
        }
    }
    else
    {
        if (gLinkPlayers[gBattleStruct->multiplayerId].id != 0)
        {
            BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_LEFT);
            BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_RIGHT);
        }
        else
        {
            BattlePutTextOnWindow(gText_Win, B_WIN_VS_OUTCOME_RIGHT);
            BattlePutTextOnWindow(gText_Loss, B_WIN_VS_OUTCOME_LEFT);
        }
    }
}

void InitLinkBattleVsScreen(u8 taskId)
{
    struct LinkPlayer *linkPlayer;
    u8 *name;
    s32 i, palId;

    switch (gTasks[taskId].data[0])
    {
    case 0:
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            for (i = 0; i < MAX_BATTLERS_COUNT; i++)
            {
                name = gLinkPlayers[i].name;
                linkPlayer = &gLinkPlayers[i];

                switch (linkPlayer->id)
                {
                case 0:
                    BattlePutTextOnWindow(name, B_WIN_VS_MULTI_PLAYER_1);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 1, 2, 4);
                    break;
                case 1:
                    BattlePutTextOnWindow(name, B_WIN_VS_MULTI_PLAYER_2);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 2, 2, 4);
                    break;
                case 2:
                    BattlePutTextOnWindow(name, B_WIN_VS_MULTI_PLAYER_3);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 1, 2, 8);
                    break;
                case 3:
                    BattlePutTextOnWindow(name, B_WIN_VS_MULTI_PLAYER_4);
                    DrawLinkBattleParticipantPokeballs(taskId, linkPlayer->id, 2, 2, 8);
                    break;
                }
            }
        }
        else
        {
            u8 temp, playerId = gBattleStruct->multiplayerId, opponentId = BATTLE_OPPOSITE(playerId);

            if (gLinkPlayers[playerId].id != 0)
				SWAP(playerId, opponentId, temp);

            name = gLinkPlayers[playerId].name;
            BattlePutTextOnWindow(name, B_WIN_VS_PLAYER);

            name = gLinkPlayers[opponentId].name;
            BattlePutTextOnWindow(name, B_WIN_VS_OPPONENT);

            DrawLinkBattleParticipantPokeballs(taskId, playerId, 1, 2, 7);
            DrawLinkBattleParticipantPokeballs(taskId, opponentId, 2, 2, 7);
        }
        gTasks[taskId].data[0]++;
        break;
    case 1:
        palId = AllocSpritePalette(TAG_VS_LETTERS);
        gPlttBufferUnfaded[palId * 16 + 0x10F] = gPlttBufferFaded[palId * 16 + 0x10F] = RGB(31, 31, 31);
        gBattleStruct->linkBattleVsSpriteId_V = CreateSprite(&sVsLetter_V_SpriteTemplate, 108, 80, 0);
        gBattleStruct->linkBattleVsSpriteId_S = CreateSprite(&sVsLetter_S_SpriteTemplate, 132, 80, 0);
        gSprites[gBattleStruct->linkBattleVsSpriteId_V].invisible = TRUE;
        gSprites[gBattleStruct->linkBattleVsSpriteId_S].invisible = TRUE;
        gTasks[taskId].data[0]++;
        break;
    case 2:
        if (gTasks[taskId].data[5] != 0)
        {
            gBattle_BG1_X = -(20) - (Sin2(gTasks[taskId].data[1]) / 32);
            gBattle_BG2_X = -(140) - (Sin2(gTasks[taskId].data[2]) / 32);
            gBattle_BG1_Y = -36;
            gBattle_BG2_Y = -36;
        }
        else
        {
            gBattle_BG1_X = -(20) - (Sin2(gTasks[taskId].data[1]) / 32);
            gBattle_BG1_Y = (Cos2(gTasks[taskId].data[1]) / 32) - 164;
            gBattle_BG2_X = -(140) - (Sin2(gTasks[taskId].data[2]) / 32);
            gBattle_BG2_Y = (Cos2(gTasks[taskId].data[2]) / 32) - 164;
        }

        if (gTasks[taskId].data[2] != 0)
        {
            gTasks[taskId].data[2] -= 2;
            gTasks[taskId].data[1] += 2;
        }
        else
        {
            if (gTasks[taskId].data[5] != 0)
                DrawLinkBattleVsScreenOutcomeText();

            PlaySE(SE_M_HARDEN);
            DestroyTask(taskId);
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].invisible = FALSE;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].invisible = FALSE;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].oam.tileNum += 0x40;
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].data[0] = 0;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].data[0] = 1;
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].data[1] = gSprites[gBattleStruct->linkBattleVsSpriteId_V].x;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].data[1] = gSprites[gBattleStruct->linkBattleVsSpriteId_S].x;
            gSprites[gBattleStruct->linkBattleVsSpriteId_V].data[2] = 0;
            gSprites[gBattleStruct->linkBattleVsSpriteId_S].data[2] = 0;
        }
        break;
    }
}

void DrawBattleEntryBackground(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        LZDecompressVram(gFile_graphics_battle_transitions_vs_frame_sheet, (void*)(BG_CHAR_ADDR(1)));
        LZDecompressVram(gVsLettersGfx, (void*)(VRAM + 0x10000));
        LoadCompressedPalette(gFile_graphics_battle_transitions_vs_frame_palette, 0x60, 0x20);
        SetBgAttribute(1, BG_ATTR_SCREENSIZE, 1);
        SetGpuReg(REG_OFFSET_BG1CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_16COLOR | BGCNT_SCREENBASE(28) | BGCNT_TXT512x256);
        CopyToBgTilemapBuffer(1, gFile_graphics_battle_transitions_vs_frame_tilemap, 0, 0);
        CopyToBgTilemapBuffer(2, gFile_graphics_battle_transitions_vs_frame_tilemap, 0, 0);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | WININ_WIN0_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        gBattle_BG1_Y = -164;
        gBattle_BG2_Y = -164;
        LoadCompressedSpriteSheetUsingHeap(&sVsLettersSpriteSheet);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_GRASS);
    else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_LEADER)
            {
                LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
                return;
            }
            else if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
            {
                LoadBattleTerrainEntryGfx(BATTLE_TERRAIN_BUILDING);
                return;
            }
        }
		LoadBattleTerrainEntryGfx(GetCurrentMapBattleScene() == MAP_BATTLE_SCENE_NORMAL ? gBattleTerrain : BATTLE_TERRAIN_BUILDING);
    }
}

static u8 GetBattleTerrainOverride(void)
{
    u8 battleScene;
	
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        return BATTLE_TERRAIN_LINK;
    else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        gBattleTerrain = BATTLE_TERRAIN_GRASS;
        return BATTLE_TERRAIN_GRASS;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_LEADER)
            return BATTLE_TERRAIN_LEADER;
        else if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
            return BATTLE_TERRAIN_CHAMPION;
    }
    battleScene = GetCurrentMapBattleScene();
	
    return battleScene == MAP_BATTLE_SCENE_NORMAL ? gBattleTerrain : GetBattleTerrainByMapScene(battleScene);
}

#define NUM_TOPBAR_REMOVE_LINES 5 // Num lines to remove on top bar

static void DrawMoveInfoWindowBorder(u8 windowId)
{
	u8 i, pixelsCount;
	
	// Remove some pixels on top
	FillWindowPixelRect(windowId, PIXEL_FILL(0), 0, 0, WindowWidthPx(windowId), NUM_TOPBAR_REMOVE_LINES);
	
	// Draw left circle border
	for (i = 0, pixelsCount = 1; i < 2; i++, pixelsCount++)
		FillWindowPixelRect(windowId, PIXEL_FILL(0), 0, (NUM_TOPBAR_REMOVE_LINES + 1) - i, pixelsCount, 1);
	
	// Draw right retangle border
	for (i = 0, pixelsCount = NUM_TOPBAR_REMOVE_LINES; i < 16; i++, pixelsCount++)
		FillWindowPixelRect(windowId, PIXEL_FILL(0), 73 + pixelsCount, NUM_TOPBAR_REMOVE_LINES + i, 16 - pixelsCount, 1);
}

void CreateBattleMoveInfoWindowsAndArrows(u16 move)
{
	u8 i, colors[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
	bool8 isNameWindow;
	
	// Create windows
	for (i = 0; i < 2; i++)
	{
		gBattleStruct->moveInfo.windowIds[i] = AddWindow(&sMoveInfoWindowTemplates[i]);
		FillWindowPixelBuffer(gBattleStruct->moveInfo.windowIds[i], PIXEL_FILL(15));
		
		isNameWindow = (i == 0);
		
		if (isNameWindow)
			DrawMoveInfoWindowBorder(gBattleStruct->moveInfo.windowIds[i]);
		
		AddTextPrinterParameterized3(gBattleStruct->moveInfo.windowIds[i], 0, 4, 3, colors, 0xFF, isNameWindow ? gBattleMoves[move].name : gStringVar4);
		PutWindowTilemap(gBattleStruct->moveInfo.windowIds[i]);
		CopyWindowToVram(gBattleStruct->moveInfo.windowIds[i], COPYWIN_BOTH);
	}
	
	// Create arrows
	gBattleStruct->moveInfo.arrowTaskId = AddScrollIndicatorArrowPairParameterized(0, 137, 165, 235, NUM_MOVEINFO_SUBMENUS - 1, 110, 110, &gBattleStruct->moveInfo.submenuState);
}

void DestroyBattleMoveInfoWindows(void)
{
	u8 i;
	
	// Destroy windows
	for (i = 0; i < 2; i++)
	{
		FillWindowPixelBuffer(gBattleStruct->moveInfo.windowIds[i], PIXEL_FILL(15));
		PutWindowTilemap(gBattleStruct->moveInfo.windowIds[i]);
		CopyWindowToVram(gBattleStruct->moveInfo.windowIds[i], COPYWIN_BOTH);
		RemoveWindow(gBattleStruct->moveInfo.windowIds[i]);
	}
	
	// Load moves box
	CopyToBgTilemapBuffer(0, gBattleTextboxTilemap, 0, 0x000);
    CopyBgTilemapBufferToVram(0);
	BattleInterfaceSetWindowPals();
}
