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

u32 MapPreview_CreateMapNameWindow(u32 id);
void MapPreview_SetFlag(u32 a0);
u32 MapPreview_GetDuration(u32 id);
bool32 MapHasPreviewScreen(u32 mapsec, u32 type);
bool32 ForestMapPreviewScreenIsRunning(void);
const struct MapPreviewScreen *GetDungeonMapPreviewScreenInfo(u32 mapsec);
void MapPreview_InitBgs(void);
void MapPreview_LoadGfx(u32 mapsec);
bool32 MapPreview_IsGfxLoadFinished(void);
void MapPreview_Unload(u32 windowId);
void MapPreview_StartForestTransition(u32 mapsec);

#endif //GUARD_MAP_PREVIEW_SCREEN_H
