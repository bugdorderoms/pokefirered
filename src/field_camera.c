#include "global.h"
#include "gflib.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "event_object_movement.h"
#include "new_menu_helpers.h"
#include "overworld.h"

EWRAM_DATA bool8 gBikeCameraAheadPanback = FALSE;

// Static type declarations
struct FieldCameraOffset
{
    u8 xPixelOffset;
    u8 yPixelOffset;
    u8 xTileOffset;
    u8 yTileOffset;
    bool8 copyBGToVRAM;
};

// static functions
static void RedrawMapSliceNorth(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout);
static void RedrawMapSliceSouth(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout);
static void RedrawMapSliceEast(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout);
static void RedrawMapSliceWest(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout);
static s32 MapPosToBgTilemapOffset(struct FieldCameraOffset *a, s32 x, s32 y);
static void DrawWholeMapViewInternal(int x, int y, const struct MapLayout *mapLayout);
static void DrawMetatileAt(const struct MapLayout *mapLayout, u16, int, int);
static void DrawMetatile(bool8 a, const u16 *b, u16 c);
static void CameraPanningCB_PanAhead(void);

// IWRAM bss vars
static struct FieldCameraOffset sFieldCameraOffset;
static s16 sHorizontalCameraPan;
static s16 sVerticalCameraPan;
static u8 gUnknown_3000E9C;
static void (*sFieldCameraPanningCallback)(void);

struct CameraObject gFieldCamera;
u16 gTotalCameraPixelOffsetY;
u16 gTotalCameraPixelOffsetX;

// text
static inline void Coords8_Add(u32 x, u32 y)
{
	sFieldCameraOffset.xPixelOffset += x;
    sFieldCameraOffset.yPixelOffset += y;
}

static void tilemap_move_something(struct FieldCameraOffset *cameraOffset, u32 b, u32 c)
{
    cameraOffset->xTileOffset += b;
    cameraOffset->xTileOffset %= 32;
    cameraOffset->yTileOffset += c;
    cameraOffset->yTileOffset %= 32;
}

void move_tilemap_camera_to_upper_left_corner(void)
{
	sFieldCameraOffset.xTileOffset = 0;
    sFieldCameraOffset.yTileOffset = 0;
    sFieldCameraOffset.xPixelOffset = 0;
    sFieldCameraOffset.yPixelOffset = 0;
    sFieldCameraOffset.copyBGToVRAM = TRUE;
}

void FieldUpdateBgTilemapScroll(void)
{
    u32 r4 = sVerticalCameraPan + sFieldCameraOffset.yPixelOffset + 8, r5 = sFieldCameraOffset.xPixelOffset + sHorizontalCameraPan;
	
    SetGpuReg(REG_OFFSET_BG1HOFS, r5);
    SetGpuReg(REG_OFFSET_BG1VOFS, r4);
    SetGpuReg(REG_OFFSET_BG2HOFS, r5);
    SetGpuReg(REG_OFFSET_BG2VOFS, r4);
    SetGpuReg(REG_OFFSET_BG3HOFS, r5);
    SetGpuReg(REG_OFFSET_BG3VOFS, r4);
}

void FieldCameraGetPixelOffsetAtGround(s16 *hofs_p, s16 *vofs_p)
{
    *hofs_p = sFieldCameraOffset.xPixelOffset + sHorizontalCameraPan;
    *vofs_p = sFieldCameraOffset.yPixelOffset + sVerticalCameraPan + 8;
}

void DrawWholeMapView(void)
{
    DrawWholeMapViewInternal(gSaveBlock1Ptr->pos.x, gSaveBlock1Ptr->pos.y, gMapHeader.mapLayout);
}

static void DrawWholeMapViewInternal(int x, int y, const struct MapLayout *mapLayout)
{
    u8 i;
    u8 j;
    u32 r6;
    u8 temp;

    for (i = 0; i < 32; i += 2)
    {
        temp = sFieldCameraOffset.yTileOffset + i;
        if (temp >= 32)
            temp -= 32;
        r6 = temp * 32;
        for (j = 0; j < 32; j += 2)
        {
            temp = sFieldCameraOffset.xTileOffset + j;
            if (temp >= 32)
                temp -= 32;
            DrawMetatileAt(mapLayout, r6 + temp, x + j / 2, y + i / 2);
        }
    }
}

static void RedrawMapSlicesForCameraUpdate(struct FieldCameraOffset *cameraOffset, int x, int y)
{
    const struct MapLayout *mapLayout = gMapHeader.mapLayout;

    if (x > 0)
        RedrawMapSliceWest(cameraOffset, mapLayout);
    if (x < 0)
        RedrawMapSliceEast(cameraOffset, mapLayout);
    if (y > 0)
        RedrawMapSliceNorth(cameraOffset, mapLayout);
    if (y < 0)
        RedrawMapSliceSouth(cameraOffset, mapLayout);
	
    cameraOffset->copyBGToVRAM = TRUE;
}

static void RedrawMapSliceNorth(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout)
{
	u32 r7;
    u8 i, temp = cameraOffset->yTileOffset + 28;

    if (temp >= 32)
        temp -= 32;
    r7 = temp * 32;
	
    for (i = 0; i < 32; i += 2)
    {
        temp = cameraOffset->xTileOffset + i;
        if (temp >= 32)
            temp -= 32;
        DrawMetatileAt(mapLayout, r7 + temp, gSaveBlock1Ptr->pos.x + i / 2, gSaveBlock1Ptr->pos.y + 14);
    }
}

static void RedrawMapSliceSouth(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout)
{
    u8 i;
    u8 temp;
    u32 r7 = cameraOffset->yTileOffset * 32;

    for (i = 0; i < 32; i += 2)
    {
        temp = cameraOffset->xTileOffset + i;
        if (temp >= 32)
            temp -= 32;
        DrawMetatileAt(mapLayout, r7 + temp, gSaveBlock1Ptr->pos.x + i / 2, gSaveBlock1Ptr->pos.y);
    }
}

static void RedrawMapSliceEast(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout)
{
    u8 i;
    u8 temp;
    u32 r6 = cameraOffset->xTileOffset;

    for (i = 0; i < 32; i += 2)
    {
        temp = cameraOffset->yTileOffset + i;
        if (temp >= 32)
            temp -= 32;
        DrawMetatileAt(mapLayout, temp * 32 + r6, gSaveBlock1Ptr->pos.x, gSaveBlock1Ptr->pos.y + i / 2);
    }
}

static void RedrawMapSliceWest(struct FieldCameraOffset *cameraOffset, const struct MapLayout *mapLayout)
{
    u8 i;
    u8 temp;
    u8 r5 = cameraOffset->xTileOffset + 28;

    if (r5 >= 32)
        r5 -= 32;
    for (i = 0; i < 32; i += 2)
    {
        temp = cameraOffset->yTileOffset + i;
        if (temp >= 32)
            temp -= 32;
        DrawMetatileAt(mapLayout, temp * 32 + r5, gSaveBlock1Ptr->pos.x + 14, gSaveBlock1Ptr->pos.y + i / 2);
    }
}

void CurrentMapDrawMetatileAt(int x, int y)
{
    int offset = MapPosToBgTilemapOffset(&sFieldCameraOffset, x, y);

    if (offset >= 0)
        DrawMetatileAt(gMapHeader.mapLayout, offset, x, y);
}

void DrawDoorMetatileAt(int x, int y, const u16 *arr)
{
    int offset = MapPosToBgTilemapOffset(&sFieldCameraOffset, x, y);

    if (offset >= 0)
        DrawMetatile(TRUE, arr, offset);
}

static void DrawMetatileAt(const struct MapLayout *mapLayout, u16 offset, int x, int y)
{
    u16 metatileId = MapGridGetMetatileIdAt(x, y);
    u16 *metatiles;

    if (metatileId > NUM_METATILES_TOTAL)
        metatileId = 0;
    
	if (metatileId < NUM_METATILES_IN_PRIMARY)
        metatiles = mapLayout->primaryTileset->metatiles;
    else
    {
        metatiles = mapLayout->secondaryTileset->metatiles;
        metatileId -= NUM_METATILES_IN_PRIMARY;
    }
    DrawMetatile(FALSE, metatiles + metatileId * NUM_TILES_PER_METATILE, offset);
}

static void DrawMetatile(bool8 isDoor, const u16 *metatiles, u16 offset)
{
	if (isDoor)
	{
		// A door metatile shall be drawn, we use covered behavior
		// Draw metatile's bottom layer to the bottom background layer.
        gBGTilemapBuffers3[offset] = metatiles[0];
        gBGTilemapBuffers3[offset + 1] = metatiles[1];
        gBGTilemapBuffers3[offset + 0x20] = metatiles[2];
        gBGTilemapBuffers3[offset + 0x21] = metatiles[3];

        // Draw transparent tiles to the middle background layer.
        gBGTilemapBuffers1[offset] = 0;
        gBGTilemapBuffers1[offset + 1] = 0;
        gBGTilemapBuffers1[offset + 0x20] = 0;
        gBGTilemapBuffers1[offset + 0x21] = 0;

        // Draw metatile's top layer to the top background layer.
        gBGTilemapBuffers2[offset] = metatiles[4];
        gBGTilemapBuffers2[offset + 1] = metatiles[5];
        gBGTilemapBuffers2[offset + 0x20] = metatiles[6];
        gBGTilemapBuffers2[offset + 0x21] = metatiles[7];
	}
	else
	{
		// Draw metatile's bottom layer to the bottom background layer.
        gBGTilemapBuffers3[offset] = metatiles[0];
        gBGTilemapBuffers3[offset + 1] = metatiles[1];
        gBGTilemapBuffers3[offset + 0x20] = metatiles[2];
        gBGTilemapBuffers3[offset + 0x21] = metatiles[3];
		
		// Draw metatile's middle layer to the middle background layer.
        gBGTilemapBuffers1[offset] = metatiles[4];
        gBGTilemapBuffers1[offset + 1] = metatiles[5];
        gBGTilemapBuffers1[offset + 0x20] = metatiles[6];
        gBGTilemapBuffers1[offset + 0x21] = metatiles[7];
		
		// Draw metatile's top layer to the top background layer, which covers object event sprites.
        gBGTilemapBuffers2[offset] = metatiles[8];
        gBGTilemapBuffers2[offset + 1] = metatiles[9];
        gBGTilemapBuffers2[offset + 0x20] = metatiles[10];
        gBGTilemapBuffers2[offset + 0x21] = metatiles[11];
	}
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    ScheduleBgCopyTilemapToVram(3);
}

static s32 MapPosToBgTilemapOffset(struct FieldCameraOffset *cameraOffset, s32 x, s32 y)
{
    x -= gSaveBlock1Ptr->pos.x;
    x *= 2;
    if (x >= 32 || x < 0)
        return -1;
	
    x = x + cameraOffset->xTileOffset;
    if (x >= 32)
        x -= 32;

    y = (y - gSaveBlock1Ptr->pos.y) * 2;
    if (y >= 32 || y < 0)
        return -1;
	
    y = y + cameraOffset->yTileOffset;
    if (y >= 32)
        y -= 32;

    return y * 32 + x;
}

static void CameraUpdateCallback(struct CameraObject *fieldCamera)
{
    if (fieldCamera->spriteId != 0)
    {
        fieldCamera->movementSpeedX = gSprites[fieldCamera->spriteId].data[2];
        fieldCamera->movementSpeedY = gSprites[fieldCamera->spriteId].data[3];
    }
}

void ResetCameraUpdateInfo(void)
{
    gFieldCamera.movementSpeedX = 0;
    gFieldCamera.movementSpeedY = 0;
    gFieldCamera.x = 0;
    gFieldCamera.y = 0;
    gFieldCamera.spriteId = 0;
    gFieldCamera.callback = NULL;
}

u32 InitCameraUpdateCallback(u8 trackedSpriteId)
{
    if (gFieldCamera.spriteId != 0)
        DestroySprite(&gSprites[gFieldCamera.spriteId]);
    gFieldCamera.spriteId = AddCameraObject(trackedSpriteId);
    gFieldCamera.callback = CameraUpdateCallback;
    return 0;
}

void CameraUpdate(void)
{
    int deltaX;
    int deltaY;
    int curMovementOffsetY;
    int curMovementOffsetX;
    int movementSpeedX;
    int movementSpeedY;

    if (gFieldCamera.callback != NULL)
        gFieldCamera.callback(&gFieldCamera);
    movementSpeedX = gFieldCamera.movementSpeedX;
    movementSpeedY = gFieldCamera.movementSpeedY;
    deltaX = 0;
    deltaY = 0;
    curMovementOffsetX = gFieldCamera.x;
    curMovementOffsetY = gFieldCamera.y;

    if (curMovementOffsetX == 0 && movementSpeedX != 0)
		deltaX = movementSpeedX > 0 ? 1 : -1;
	
    if (curMovementOffsetY == 0 && movementSpeedY != 0)
		deltaY = movementSpeedY > 0 ? 1 : -1;
	
    if (curMovementOffsetX != 0 && curMovementOffsetX == -movementSpeedX)
		deltaX = movementSpeedX > 0 ? 1 : -1;
	
    if (curMovementOffsetY != 0 && curMovementOffsetY == -movementSpeedY)
		deltaX = movementSpeedY > 0 ? 1 : -1;

    gFieldCamera.x += movementSpeedX;
    gFieldCamera.x = gFieldCamera.x - 16 * (gFieldCamera.x / 16);
    gFieldCamera.y += movementSpeedY;
    gFieldCamera.y = gFieldCamera.y - 16 * (gFieldCamera.y / 16);

    if (deltaX != 0 || deltaY != 0)
    {
        CameraMove(deltaX, deltaY);
        UpdateObjectEventsForCameraUpdate(deltaX, deltaY);
        // RotatingGatePuzzleCameraUpdate(deltaX, deltaY);
        // ResetBerryTreeSparkleFlags();
        tilemap_move_something(&sFieldCameraOffset, deltaX * 2, deltaY * 2);
        RedrawMapSlicesForCameraUpdate(&sFieldCameraOffset, deltaX * 2, deltaY * 2);
    }
	Coords8_Add(movementSpeedX, movementSpeedY);
    gTotalCameraPixelOffsetX -= movementSpeedX;
    gTotalCameraPixelOffsetY -= movementSpeedY;
}

void CameraUpdateNoObjectRefresh(void)
{
    int deltaX;
    int deltaY;
    int curMovementOffsetY;
    int curMovementOffsetX;
    int movementSpeedX;
    int movementSpeedY;

    if (gFieldCamera.callback != NULL)
        gFieldCamera.callback(&gFieldCamera);
    movementSpeedX = gFieldCamera.movementSpeedX;
    movementSpeedY = gFieldCamera.movementSpeedY;
    deltaX = 0;
    deltaY = 0;
    curMovementOffsetX = gFieldCamera.x;
    curMovementOffsetY = gFieldCamera.y;

	if (curMovementOffsetX == 0 && movementSpeedX != 0)
		deltaX = movementSpeedX > 0 ? 1 : -1;
	
    if (curMovementOffsetY == 0 && movementSpeedY != 0)
		deltaY = movementSpeedY > 0 ? 1 : -1;
	
    if (curMovementOffsetX != 0 && curMovementOffsetX == -movementSpeedX)
		deltaX = movementSpeedX > 0 ? 1 : -1;
	
    if (curMovementOffsetY != 0 && curMovementOffsetY == -movementSpeedY)
		deltaX = movementSpeedY > 0 ? 1 : -1;

    gFieldCamera.x += movementSpeedX;
    gFieldCamera.x = gFieldCamera.x - 16 * (gFieldCamera.x / 16);
    gFieldCamera.y += movementSpeedY;
    gFieldCamera.y = gFieldCamera.y - 16 * (gFieldCamera.y / 16);

    if (deltaX != 0 || deltaY != 0)
    {
        CameraMove(deltaX, deltaY);
        // UpdateObjectEventsForCameraUpdate(deltaX, deltaY);
        // RotatingGatePuzzleCameraUpdate(deltaX, deltaY);
        // ResetBerryTreeSparkleFlags();
        tilemap_move_something(&sFieldCameraOffset, deltaX * 2, deltaY * 2);
        RedrawMapSlicesForCameraUpdate(&sFieldCameraOffset, deltaX * 2, deltaY * 2);
    }
	Coords8_Add(movementSpeedX, movementSpeedY);
    // gTotalCameraPixelOffsetX -= movementSpeedX;
    // gTotalCameraPixelOffsetY -= movementSpeedY;
}

void SetCameraPanningCallback(void (*a)(void))
{
    sFieldCameraPanningCallback = a;
}

void SetCameraPanning(s16 a, s16 b)
{
    sHorizontalCameraPan = a;
    sVerticalCameraPan = b + 32;
}

void InstallCameraPanAheadCallback(void)
{
    sFieldCameraPanningCallback = CameraPanningCB_PanAhead;
    gUnknown_3000E9C = 0;
    sHorizontalCameraPan = 0;
    sVerticalCameraPan = 32;
}

void UpdateCameraPanning(void)
{
    if (sFieldCameraPanningCallback != NULL)
        sFieldCameraPanningCallback();
    // Update sprite offset of overworld objects
    gSpriteCoordOffsetX = gTotalCameraPixelOffsetX - sHorizontalCameraPan;
    gSpriteCoordOffsetY = gTotalCameraPixelOffsetY - sVerticalCameraPan - 8;
}

static void CameraPanningCB_PanAhead(void)
{
    u8 var;

    if (gBikeCameraAheadPanback == FALSE)
    {
        InstallCameraPanAheadCallback();
    }
    else
    {
        // this code is never reached.
        if (gPlayerAvatar.tileTransitionState == 1)
        {
            gUnknown_3000E9C ^= 1;
            if (gUnknown_3000E9C == 0)
                return;
        }
        else
        {
            gUnknown_3000E9C = 0;
        }

        var = GetPlayerMovementDirection();
        if (var == 2)
        {
            if (sVerticalCameraPan > -8)
                sVerticalCameraPan -= 2;
        }
        else if (var == 1)
        {
            if (sVerticalCameraPan < 72)
                sVerticalCameraPan += 2;
        }
        else if (sVerticalCameraPan < 32)
        {
            sVerticalCameraPan += 2;
        }
        else if (sVerticalCameraPan > 32)
        {
            sVerticalCameraPan -= 2;
        }
    }
}
