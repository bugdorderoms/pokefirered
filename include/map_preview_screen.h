#ifndef GUARD_MAP_PREVIEW_SCREEN_H
#define GUARD_MAP_PREVIEW_SCREEN_H

enum
{
	MAP_PREVIEW_TYPE_NONE, // No map preview
	MAP_PREVIEW_TYPE_CAVE,
	MAP_PREVIEW_TYPE_FOREST,
	MAP_PREVIEW_TYPE_ANY,
};

struct MapPreviewScreen
{
    const void * tilesptr;
    const void * tilemapptr;
    const void * palptr;
	u16 worldMapFlag;
    u8 type;
};

u16 MapPreview_CreateMapNameWindow(u8 id);
void MapPreview_SetFlag(u16 a0);
u16 MapPreview_GetDuration(u8 id);
bool8 MapHasPreviewScreen(u8 mapsec, u8 type);
bool32 ForestMapPreviewScreenIsRunning(void);
const struct MapPreviewScreen *GetDungeonMapPreviewScreenInfo(u8 mapsec);
void MapPreview_InitBgs(void);
void MapPreview_LoadGfx(u8 mapsec);
bool32 MapPreview_IsGfxLoadFinished(void);
void MapPreview_Unload(s32 windowId);
void MapPreview_StartForestTransition(u8 mapsec);

#endif //GUARD_MAP_PREVIEW_SCREEN_H
