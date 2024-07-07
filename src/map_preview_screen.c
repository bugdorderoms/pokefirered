#include "global.h"
#include "gflib.h"
#include "new_menu_helpers.h"
#include "region_map.h"
#include "menu.h"
#include "field_fadetransition.h"
#include "field_weather.h"
#include "script.h"
#include "overworld.h"
#include "event_data.h"
#include "map_preview_screen.h"
#include "constants/region_map_sections.h"

static EWRAM_DATA bool8 sHasVisitedMapBefore = FALSE;
static EWRAM_DATA bool8 sAllocedBg0TilemapBuffer = FALSE;

static void Task_RunMapPreviewScreenForest(u8 taskId);

static const struct WindowTemplate sMapNameWindow = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 13,
    .height = 2,
    .paletteNum = 14,
    .baseBlock = 0x1C2
};

static const struct BgTemplate sMapPreviewBgTemplate[1] = {
    {
        .mapBaseIndex = 31
    }
};

bool8 MapHasPreviewScreen(u8 mapsec, u8 type)
{
	u8 mapPreviewType = gMapSectionsInfo[mapsec].mapPreview.type;
	
	if (mapPreviewType != MAP_PREVIEW_TYPE_NONE)
	{
		if (mapPreviewType == MAP_PREVIEW_TYPE_ANY)
			return TRUE;
		else
			return (mapPreviewType == type);
	}
	return FALSE;
}

void MapPreview_InitBgs(void)
{
    InitBgsFromTemplates(0, sMapPreviewBgTemplate, ARRAY_COUNT(sMapPreviewBgTemplate));
    ShowBg(0);
}

void MapPreview_LoadGfx(u8 mapsec)
{
    ResetTempTileDataBuffers();
    LoadPalette(gMapSectionsInfo[mapsec].mapPreview.palptr, 0xD0, 0x60);
    DecompressAndCopyTileDataToVram(0, gMapSectionsInfo[mapsec].mapPreview.tilesptr, 0, 0, 0);
    if (GetBgTilemapBuffer(0) == NULL)
    {
        SetBgTilemapBuffer(0, Alloc(BG_SCREEN_SIZE));
        sAllocedBg0TilemapBuffer = TRUE;
    }
    else
    {
        sAllocedBg0TilemapBuffer = FALSE;
    }
    CopyToBgTilemapBuffer(0, gMapSectionsInfo[mapsec].mapPreview.tilemapptr, 0, 0x000);
    CopyBgTilemapBufferToVram(0);
    
}

void MapPreview_Unload(s32 windowId)
{
    RemoveWindow(windowId);
    if (sAllocedBg0TilemapBuffer)
    {
        Free(GetBgTilemapBuffer(0));
    }
}

bool32 MapPreview_IsGfxLoadFinished(void)
{
    return FreeTempTileDataBuffersIfPossible();
}

void MapPreview_StartForestTransition(u8 mapsec)
{
    u8 taskId;

    taskId = CreateTask(Task_RunMapPreviewScreenForest, 0);
    gTasks[taskId].data[2] = GetBgAttribute(0, BG_ATTR_PRIORITY);
    gTasks[taskId].data[4] = GetGpuReg(REG_OFFSET_BLDCNT);
    gTasks[taskId].data[5] = GetGpuReg(REG_OFFSET_BLDALPHA);
    gTasks[taskId].data[3] = GetGpuReg(REG_OFFSET_DISPCNT);
    gTasks[taskId].data[6] = GetGpuReg(REG_OFFSET_WININ);
    gTasks[taskId].data[7] = GetGpuReg(REG_OFFSET_WINOUT);
    gTasks[taskId].data[10] = MapPreview_GetDuration(mapsec);
    gTasks[taskId].data[8] = 16;
    gTasks[taskId].data[9] = 0;
    SetBgAttribute(0, BG_ATTR_PRIORITY, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
    SetGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR | WININ_WIN1_CLR);
    SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WIN01_CLR);
    gTasks[taskId].data[11] = MapPreview_CreateMapNameWindow(mapsec);
    ScriptContext2_Enable();
}

u16 MapPreview_CreateMapNameWindow(u8 mapsec)
{
    u16 windowId;
    u32 xctr;
    u8 color[3];

    windowId = AddWindow(&sMapNameWindow);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    color[0] = TEXT_COLOR_WHITE;
    color[1] = TEXT_COLOR_RED;
    color[2] = TEXT_COLOR_LIGHT_GRAY;
    GetMapName(gStringVar4, mapsec);
    xctr = 104 - GetStringWidth(2, gStringVar4, 0);
    AddTextPrinterParameterized4(windowId, 2, xctr / 2, 2, 0, 0, color, -1, gStringVar4);
    return windowId;
}

bool32 ForestMapPreviewScreenIsRunning(void)
{
    if (FuncIsActiveTask(Task_RunMapPreviewScreenForest) == TRUE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static void Task_RunMapPreviewScreenForest(u8 taskId)
{
    s16 * data;

    data = gTasks[taskId].data;
    switch (data[0])
    {
    case 0:
        if (!MapPreview_IsGfxLoadFinished() && !IsDma3ManagerBusyWithBgCopy())
        {
            CopyWindowToVram(data[11], COPYWIN_BOTH);
            data[0]++;
        }
        break;
    case 1:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            FadeInFromBlack();
            data[0]++;
        }
        break;
    case 2:
        if (IsWeatherNotFadingIn())
        {
            Overworld_PlaySpecialMapMusic();
            data[0]++;
        }
        break;
    case 3:
        data[1]++;
        if (data[1] > data[10])
        {
            data[1] = 0;
            data[0]++;
        }
        break;
    case 4:
        switch (data[1])
        {
        case 0:
            data[9]++;
            if (data[9] > 16)
            {
                data[9] = 16;
            }
            break;
        case 1:
            data[8]--;
            if (data[8] < 0)
            {
                data[8] = 0;
            }
            break;
        }
        data[1] = (data[1] + 1) % 3;
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(data[8], data[9]));
        if (data[8] == 0 && data[9] == 16)
        {
            FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
            CopyBgTilemapBufferToVram(0);
            data[0]++;
        }
        break;
    case 5:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            MapPreview_Unload(data[11]);
            SetBgAttribute(0, BG_ATTR_PRIORITY, data[2]);
            SetGpuReg(REG_OFFSET_DISPCNT, data[3]);
            SetGpuReg(REG_OFFSET_BLDCNT, data[4]);
            SetGpuReg(REG_OFFSET_BLDALPHA, data[5]);
            SetGpuReg(REG_OFFSET_WININ, data[6]);
            SetGpuReg(REG_OFFSET_WINOUT, data[7]);
            DestroyTask(taskId);
        }
        break;
    }
}

const struct MapPreviewScreen *GetDungeonMapPreviewScreenInfo(u8 mapsec)
{
    if (gMapSectionsInfo[mapsec].mapPreview.type == MAP_PREVIEW_TYPE_NONE)
        return NULL;
    else
        return &gMapSectionsInfo[mapsec].mapPreview;
}

u16 MapPreview_GetDuration(u8 mapsec)
{
	u8 type = gMapSectionsInfo[mapsec].mapPreview.type;
    u16 flagId;

    if (type != MAP_PREVIEW_TYPE_NONE)
	{
		flagId = gMapSectionsInfo[mapsec].mapPreview.worldMapFlag;
		
		if (type == MAP_PREVIEW_TYPE_CAVE)
			return FlagGet(flagId) ? 40 : 120;
		else
			return sHasVisitedMapBefore ? 120 : 40;
	}
	return 0;
}

void MapPreview_SetFlag(u16 flagId)
{
	sHasVisitedMapBefore = FlagGet(flagId) ? FALSE : TRUE;
    FlagSet(flagId);
}
