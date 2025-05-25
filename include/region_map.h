#ifndef GUARD_REGION_MAP_H
#define GUARD_REGION_MAP_H

#include "global.h"
#include "bg.h"
#include "map_preview_screen.h"

enum
{
    REGIONMAP_TYPE_NORMAL,
    REGIONMAP_TYPE_WALL,
    REGIONMAP_TYPE_FLY,
    REGIONMAP_TYPE_COUNT
};

struct MapSectionInfo
{
    const struct MapPreviewScreen mapPreview; // Set if the Map has a preview screen
    u8 region; // Region the Map is in
    const u8 *name; // The Map name
    const u8 *desc; // For some dungeons in the Town Map
};

u8 *GetMapName(u8 *dest, u32 mapsec);
void InitRegionMapWithExitCB(u32 a0, MainCallback a1);
void SetDispCnt(u32 idx, bool32 clear);

extern const struct MapSectionInfo gMapSectionsInfo[];

#endif // GUARD_REGION_MAP_H
