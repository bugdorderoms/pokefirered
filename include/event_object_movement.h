#ifndef GUARD_EVENT_OBJECT_MOVEMENT_H
#define GUARD_EVENT_OBJECT_MOVEMENT_H

#include "global.h"

#define OBJ_EVENT_PAL_TAG_NONE 0x11FF

// Exported struct declarations

enum SpinnerRunnerFollowPatterns
{
    RUNFOLLOW_ANY,
    RUNFOLLOW_NORTH_SOUTH,
    RUNFOLLOW_EAST_WEST,
    RUNFOLLOW_NORTH_WEST,
    RUNFOLLOW_NORTH_EAST,
    RUNFOLLOW_SOUTH_WEST,
    RUNFOLLOW_SOUTH_EAST,
    RUNFOLLOW_NORTH_SOUTH_WEST,
    RUNFOLLOW_NORTH_SOUTH_EAST,
    RUNFOLLOW_NORTH_EAST_WEST,
    RUNFOLLOW_SOUTH_EAST_WEST
};

struct StepAnimTable
{
    const union AnimCmd *const *anims;
    u8 animPos[4];
};

typedef void (*MovementAction)(u32 direction);

#define GROUND_EFFECT_FLAG_TALL_GRASS_ON_SPAWN   Bit(0)
#define GROUND_EFFECT_FLAG_TALL_GRASS_ON_MOVE    Bit(1)
#define GROUND_EFFECT_FLAG_LONG_GRASS_ON_SPAWN   Bit(2)
#define GROUND_EFFECT_FLAG_LONG_GRASS_ON_MOVE    Bit(3)
#define GROUND_EFFECT_FLAG_ICE_REFLECTION        Bit(4)
#define GROUND_EFFECT_FLAG_REFLECTION            Bit(5)
#define GROUND_EFFECT_FLAG_SHALLOW_FLOWING_WATER Bit(6)
#define GROUND_EFFECT_FLAG_SAND                  Bit(7)
#define GROUND_EFFECT_FLAG_DEEP_SAND             Bit(8)
#define GROUND_EFFECT_FLAG_RIPPLES               Bit(9)
#define GROUND_EFFECT_FLAG_PUDDLE                Bit(10)
#define GROUND_EFFECT_FLAG_SAND_PILE             Bit(11)
#define GROUND_EFFECT_FLAG_LAND_IN_TALL_GRASS    Bit(12)
#define GROUND_EFFECT_FLAG_LAND_IN_LONG_GRASS    Bit(13)
#define GROUND_EFFECT_FLAG_LAND_IN_SHALLOW_WATER Bit(14)
#define GROUND_EFFECT_FLAG_LAND_IN_DEEP_WATER    Bit(15)
#define GROUND_EFFECT_FLAG_LAND_ON_NORMAL_GROUND Bit(16)
#define GROUND_EFFECT_FLAG_SHORT_GRASS           Bit(17)
#define GROUND_EFFECT_FLAG_HOT_SPRINGS           Bit(18)
#define GROUND_EFFECT_FLAG_SEAWEED               Bit(19)
#define GROUND_EFFECT_FLAG_SNOW                  Bit(20)

// Exported RAM declarations

// Exported ROM declarations

u32 GetObjectEventIdByLocalIdAndMap(u32, u32, u32);
bool32 TryGetObjectEventIdByLocalIdAndMap(u32, u32, u32, u8 *);
u32 GetObjectEventIdByXY(s16, s16);
void SetObjectEventDirection(struct ObjectEvent *, u32);
void RemoveObjectEventByLocalIdAndMap(u32 localId, u32 mapNum, u32 mapGroup);
u32 GetObjectEventFlagByLocalIdAndMap(u32 localId, u32 mapNum, u32 mapGroup);
void LoadObjectEventPalette(u32 paletteTag);
void TryMoveObjectEventToMapCoords(u32, u32, u32, s16, s16);
void PatchObjectPalette(u32, u32);
void ReloadMapObjectsWithOffset(s16 x, s16 y);
void GetMapCoordsFromSpritePos(s16, s16, s16 *, s16 *);
u32 GetFaceDirectionAnimNum(u32);
void SetSpritePosToOffsetMapCoords(s16 *, s16 *, s16, s16);
void ObjectEventClearHeldMovement(struct ObjectEvent *);
void ObjectEventClearHeldMovementIfActive(struct ObjectEvent *);
u32 sprite_new(u32, u32, s16, s16, u32, u32);
u32 AddPseudoObjectEvent(u32, SpriteCallback, s16, s16, u32);
void BlitObjectEventToWindow(u32 windowId, u32 graphicsId, u32 frameId, u32 paletteOffset, u32 width, u32 height);
u32 TrySpawnObjectEvent(u32 localId, u32 mapNum, u32 mapGroup);
u32 SpawnSpecialObjectEventParameterized(u32, u32, u32, s16, s16, u32);
u32 SpawnSpecialObjectEvent(struct ObjectEventTemplate *);
void CameraObjectReset1(void);
void CameraObjectReset2(void);
void ReturnPlayerToDefaultGraphicsIdByStateId(struct ObjectEvent *);
void ObjectEventSetGraphicsId(struct ObjectEvent *, u32);
void ObjectEventTurn(struct ObjectEvent *, u32);
void ObjectEventTurnByLocalIdAndMap(u32, u32, u32, u32);
void ObjectEventForceSetHeldMovement(struct ObjectEvent *, u32);
const struct ObjectEventGraphicsInfo *GetObjectEventGraphicsInfo(u32);
void ShowOrHideObjectByLocalIdAndMap(u32 localId, u32 mapNum, u32 mapGroup, bool32 state);
void SetObjectPositionByLocalIdAndMap(u32 localId, u32 mapNum, u32 mapGroup, s16 x, s16 y);
void UnfixObjectPriorityByLocalIdAndMap(u32 localId, u32 mapNum, u32 mapGroup);
void SetObjectPriorityByLocalIdAndMap(u32 localId, u32 mapNum, u32 mapGroup, u32 subpriority);
void ShiftObjectEventCoords(struct ObjectEvent *, s16, s16);
void TryOverrideObjectEventTemplateCoords(u32, u32, u32);
void UpdateObjectEventCurrentMovement(struct ObjectEvent *, struct Sprite *, bool32(struct ObjectEvent *, struct Sprite *));
bool32 ObjectEventFaceOppositeDirection(struct ObjectEvent *, u32);
u32 GetOppositeDirection(u32);
u32 GetWalkInPlaceFastMovementAction(u32);
u32 GetWalkInPlaceNormalMovementAction(u32);
u32 GetWalkInPlaceSlowMovementAction(u32);
u32 GetStepInPlaceDelay32AnimId(u32);
u32 GetCollisionAtCoords(struct ObjectEvent *, s16, s16, u32);
void MoveCoords(u32, s16 *, s16 *);
bool32 ObjectEventIsHeldMovementActive(struct ObjectEvent *);
u32 ObjectEventClearHeldMovementIfFinished(struct ObjectEvent *);
u32 GetObjectEventIdByXYZ(u32 x, u32 y, u32 z);
bool32 ObjectEventSetHeldMovement(struct ObjectEvent *, u32);
void ShiftStillObjectEventCoords(struct ObjectEvent *);
void OverrideMovementTypeForObjectEvent(const struct ObjectEvent *, u32);
void SetTrainerMovementType(struct ObjectEvent *, u32);
u32 GetFishingDirectionAnimNum(u32 direction);
u32 GetFishingNoCatchDirectionAnimNum(u32 direction);
u32 sub_805EB44(u32, u32, s16, s16);
void InitObjectEventPalettes(u32 palSlot);
bool32 ObjectEventIsMovementOverridden(struct ObjectEvent *objectEvent);
u32 ObjectEventCheckHeldMovementStatus(struct ObjectEvent *objectEvent);
u32 GetWalkNormalMovementAction(u32);
u32 GetWalkFastMovementAction(u32);
u32 GetWalkFastestMovementAction(u32);
u32 GetWalkSlowestMovementAction(u32 direction);
u32 GetTrainerFacingDirectionMovementType(u32 direction);
u32 GetFaceDirectionMovementAction(u32);
u32 GetFaceDirectionFastMovementAction(u32);
void CameraObjectSetFollowedObjectId(u32 objectId);
void UnfreezeObjectEvents(void);
u32 GetSlideMovementAction(u32);
u32 GetJumpInPlaceMovementAction(u32);
u32 GetJumpMovementAction(u32);
u32 GetJump2MovementAction(u32);
bool32 NpcTakeStep(struct Sprite *sprite);
void SetJumpSpriteData(struct Sprite *, u32, u32, u32);
u8 DoJumpSpriteMovement(struct Sprite *);
u8 DoJumpSpecialSpriteMovement(struct Sprite *);
void TurnObjectEvent(u32, u32);
const u8 *GetObjectEventScriptPointerByObjectEventId(u32 objectEventId);
u32 GetFirstInactiveObjectEventId(void);
u32 GetCollisionFlagsAtCoords(struct ObjectEvent * objectEvent, s16 x, s16 y, u32 direction);
void OverrideTemplateCoordsForObjectEvent(const struct ObjectEvent *objectEvent);
void FreezeObjectEvents(void);
bool32 FreezeObjectEvent(struct ObjectEvent *);
void UnfreezeObjectEvent(struct ObjectEvent *);
void FreezeObjectEventsExceptOne(u32 objEventId);
void RfuUnionObjectSetFacingDirection(u32 objectEventId, u32 direction);
void RfuUnionObjectToggleInvisibility(u32 objectEventId, bool32 invisible);
bool32 RfuUnionObjectIsInvisible(u32 objectEventId);
void RfuUnionObjectStartWarp(u32 objectEventId, u32 animNo);
bool32 RfuUnionObjectIsWarping(u32 objectEventId);
u32 GetJumpSpecialMovementAction(u32 direction);
void EnableObjectGroundEffectsByXY(s16 x, s16 y);
void MoveObjectEventToMapCoords(struct ObjectEvent *objectEvent, s16 x, s16 y);
u32 CreateCopySpriteAt(struct Sprite * sprite, s16 x, s16 y, u32 subpriority);
void SetSpritePosToMapCoords(s16 x, s16 y, s16 *x2, s16 *y2);
void UpdateObjectEventSpriteVisibility(struct Sprite *sprite, bool32 invisible);
bool32 AreZCoordsCompatible(u32, u32);
u32 ZCoordToPriority(u32 z);
void ObjectEventUpdateZCoord(struct ObjectEvent *pObject);
void SetObjectSubpriorityByZCoord(u32 z, struct Sprite * sprite, u32 offset);
bool32 IsZCoordMismatchAt(u32, s16, s16);
void MakeObjectTemplateFromObjectEventGraphicsInfo(u32 graphicsId, void (*callback)(struct Sprite *), struct SpriteTemplate *spriteTemplate, const struct SubspriteTable **subspriteTables);
u32 AddCameraObject(u32 trackedSpriteId);
void UpdateObjectEventsForCameraUpdate(s16 x, s16 y);
void UpdateObjectEventSpriteSubpriorityAndVisibility(struct Sprite *);
void SetMovementDelay(struct Sprite *, s16);
bool32 WaitForMovementDelay(struct Sprite *);
void oamt_npc_ministep_reset(struct Sprite *, u32, u32);
bool32 obj_npc_ministep(struct Sprite *sprite);
void SetSpriteDataForNormalStep(struct Sprite *, u32);
void SetSpriteDataForNormalStep2(struct Sprite *, u32);
bool32 UpdateWalkSlowAnim(struct Sprite *sprite);
void SetSpriteDataForNormalStep3(struct Sprite *, u32);
void DoShadowFieldEffect(struct ObjectEvent *);
void SetSpriteDataForNormalStep4(struct Sprite *, u32);
void SetAndStartSpriteAnim(struct Sprite *, u32, u32);
u32 ObjectEventGetHeldMovementActionId(struct ObjectEvent *objectEvent);
u32 GetMoveDirectionAnimNum(u32 direction);
u32 StartFieldEffectForObjectEvent(u32 fieldEffectId, struct ObjectEvent * objectEvent);

// Exported data declarations

extern const struct SpriteTemplate *const gFieldEffectObjectTemplatePointers[];
extern const struct SpritePalette gSpritePalette_GeneralFieldEffect1;
extern const struct SpriteTemplate * const gFieldEffectObjectTemplatePointers[];
extern const struct OamData gObjectEventBaseOam_32x32;
extern const u16 gUnknown_8398648[];
extern const u16 gUnknown_8398688[];
u32 GetLedgeJumpDirection(s16 x, s16 y, u32 z);
u32 GetRideWaterCurrentMovementAction(u32 direction);
u32 GetPlayerRunMovementAction(u32 direction);
u32 GetPlayerRunSlowMovementAction(u32 direction);
u32 GetWalkSlowMovementAction(u32 direction);
u32 GetSpinMovementAction(u32 direction);
u32 GetAcroWheelieFaceDirectionMovementAction(u32 direction);
u32 GetAcroPopWheelieFaceDirectionMovementAction(u32 direction);
u32 GetAcroEndWheelieFaceDirectionMovementAction(u32 direction);
u32 GetAcroWheelieHopFaceDirectionMovementAction(u32 direction);
u32 GetAcroWheelieHopDirectionMovementAction(u32 direction);
u32 GetAcroWheelieJumpDirectionMovementAction(u32 direction);
u32 GetJumpInPlaceTurnAroundMovementAction(u32 direction);
u32 GetAcroWheelieInPlaceDirectionMovementAction(u32 direction);
u32 GetAcroPopWheelieMoveDirectionMovementAction(u32 direction);
u32 GetAcroWheelieMoveDirectionMovementAction(u32 direction);
u32 sub_80641EC(u32 direction);
u32 GetFishingBiteDirectionAnimNum(u32 direction);
void TrySpawnObjectEvents(s16 cameraX, s16 cameraY);
void ResetObjectEvents(void);

#endif // GUARD_EVENT_OBJECT_MOVEMENT_H
