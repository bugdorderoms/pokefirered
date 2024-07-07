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

u8 *GetMapName(u8 *dest, u16 mapsec);
void InitRegionMapWithExitCB(u8 a0, void (*a1)(void));
void SetDispCnt(u8 idx, bool8 clear);

extern const struct MapSectionInfo gMapSectionsInfo[];

#endif // GUARD_REGION_MAP_H
