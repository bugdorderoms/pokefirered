#ifndef GUARD_OVERWORLD_H
#define GUARD_OVERWORLD_H

#include "global.h"
#include "main.h"

#define LINK_KEY_CODE_NULL 0x00
#define LINK_KEY_CODE_EMPTY 0x11
#define LINK_KEY_CODE_DPAD_DOWN 0x12
#define LINK_KEY_CODE_DPAD_UP 0x13
#define LINK_KEY_CODE_DPAD_LEFT 0x14
#define LINK_KEY_CODE_DPAD_RIGHT 0x15
#define LINK_KEY_CODE_UNK_2 0x16
#define LINK_KEY_CODE_EXIT_ROOM 0x17
#define LINK_KEY_CODE_START_BUTTON 0x18
#define LINK_KEY_CODE_A_BUTTON 0x19
#define LINK_KEY_CODE_UNK_4 0x1A // I'd guess this is the B button?

// These two are a hack to stop user input until link stuff can be
// resolved.
#define LINK_KEY_CODE_HANDLE_RECV_QUEUE 0x1B
#define LINK_KEY_CODE_HANDLE_SEND_QUEUE 0x1C
#define LINK_KEY_CODE_UNK_7 0x1D
#define LINK_KEY_CODE_UNK_8 0x1E

#define MOVEMENT_MODE_FREE 0
#define MOVEMENT_MODE_FROZEN 1
#define MOVEMENT_MODE_SCRIPTED 2

struct LinkPlayerObjectEvent
{
    u8 active;
    u8 linkPlayerId;
    u8 objEventId;
    u8 movementMode;
};

struct CreditsOverworldCmd
{
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
};

/* gDisableMapMusicChangeOnMapLoad */
#define MUSIC_DISABLE_OFF  0
#define MUSIC_DISABLE_STOP 1
#define MUSIC_DISABLE_KEEP 2

/* gGlobalFieldTintMode */
#define GF_TINT_NONE      0
#define GF_TINT_GRAYSCALE 1
#define GF_TINT_SEPIA     2

extern const struct UCoords32 gDirectionToVectors[];

extern struct LinkPlayerObjectEvent gLinkPlayerObjectEvents[4];
extern MainCallback gFieldCallback;

extern struct WarpData gLastUsedWarp;

extern u8 gUnknown_2031DE0;
extern u8 gFieldLinkPlayerCount;
extern u8 gLocalLinkPlayerId;

void IncrementGameStat(u32 index);

void Overworld_SetMapObjTemplateCoords(u32, s16, s16);
void Overworld_SetObjEventTemplateMovementType(u32, u32);

void SetWarpDestination(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);

void SetDynamicWarp(s8 mapGroup, s8 mapNum, s8 warpId);
void SetDynamicWarpWithCoords(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
void SetFixedDiveWarp(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
void SetFixedHoleWarp(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
void SetEscapeWarp(s8 mapGroup, s8 mapNum, s8 warpId, s8 x, s8 y);
void SetWarpDestinationToEscapeWarp(void);
void SetWarpDestinationToLastHealLocation(void);
void Overworld_ClearSavedMusic(void);
bool32 Overworld_MusicCanOverrideMapMusic(u32 song);

void player_avatar_init_params_reset(void);

void Overworld_SetFlashLevel(s32 a1);
u32 Overworld_GetFlashLevel(void);

void Overworld_SetSavedMusic(u32);
void Overworld_ChangeMusicToDefault(void);
void Overworld_ChangeMusicTo(u32);

bool32 IsUpdateLinkStateCBActive(void);

void ClearLinkPlayerObjectEvents(void);
const struct MapHeader *const Overworld_GetMapHeaderByGroupAndId(u32, u32);
void ObjectEventMoveDestCoords(struct ObjectEvent *, u32, s16 *, s16 *);
void CB2_ReturnToField(void);
void CB2_ReturnToFieldContinueScriptPlayMapMusic(void);
void WarpIntoMap(void);
u32 GetMapTypeByGroupAndId(s8 mapGroup, s8 mapNum);
void SetWarpDestinationToMapWarp(s8 mapGroup, s8 mapNum, s8 warpNum);
void c2_load_new_map(void);
void SetWarpDestinationToDynamicWarp(void);
void mapldr_default(void);

u32 GetGameStat(u32 statId);
void SetGameStat(u32 statId, u32 value);

void CB2_ContinueSavedGame(void);
void Overworld_SetWarpDestinationFromWarp(struct WarpData *);
void Overworld_PlaySpecialMapMusic(void);

u32 GetCurrentRegionMapSectionId(void);

void SetCurrentMapLayout(u32 mapLayoutId);
void SetWarpDestinationToFixedHoleWarp(s16 x, s16 y);

void ResetInitialPlayerAvatarState(void);
void CleanupOverworldWindowsAndTilemaps(void);
u32 ComputeWhiteOutMoneyLoss(void);

extern u8 gDisableMapMusicChangeOnMapLoad;
extern u8 gGlobalFieldTintMode;

extern bool32 (*gFieldCallback2)(void);

void SetLastHealLocationWarp(u32 healLocaionId);
void LoadMapFromCameraTransition(u32 mapGroup, u32 mapNum);
void CB2_ReturnToFieldFromDiploma(void);
void CB2_OverworldBasic(void);
void CB2_Overworld(void);
void CB2_NewGame(void);
bool32 IsMapTypeOutdoors(u32 mapType);
bool32 Overworld_MapTypeAllowsTeleportAndFly(u32 mapType);

void Overworld_FadeOutMapMusic(void);
void CB2_LoadMap(void);
bool32 IsMapTypeIndoors(u32 mapType);
bool32 Overworld_IsBikingAllowed(void);
bool32 Overworld_LinkRecvQueueLengthMoreThan2(void);
u32 GetCurrentMapType(void);

u32 GetLastUsedWarpMapType(void);
const struct MapHeader *const GetDestinationWarpMapHeader(void);
void TryFadeOutOldMapMusic(void);
void CB2_ReturnToFieldCableClub(void);
void ResetGameStats(void);

void Overworld_CreditsMainCB(void);
bool32 Overworld_DoScrollSceneForCredits(u8 *, const struct CreditsOverworldCmd *, u32);

bool32 IsSendingKeysOverCable(void);

void CB2_ReturnToFieldWithOpenMenu(void);
void CB2_WhiteOut(void);
void CB2_ReturnToFieldFromMultiplayer(void);
void ApplyNewEncryptionKeyToGameStats(u32 newKey);
void SetContinueGameWarpToDynamicWarp(void);

void SetContinueGameWarpToHealLocation(u32 loc);

void UpdateAmbientCry(s16 *state, u16 *delayCounter);
void SetWarpDestinationToHealLocation(u32 a0);
bool32 Overworld_SendKeysToLinkIsRunning(void);
bool32 Overworld_RecvKeysFromLinkIsRunning(void);
void OverworldWhiteOutGetMoneyLoss(void);
u32 GetCurrentMapBattleScene(void);
void Overworld_ResetStateForLeavingMap(void);
bool32 MetatileBehavior_IsSurfableInSeafoamIslands(u32 metatileBehavior);
void QueueExitLinkRoomKey(void);
void sub_8057F34(void);
u32 sub_8057EC0(void);
void sub_8057F70(void);
void sub_8057F48(void);
void SetMainCallback1(MainCallback cb);
void CB1_Overworld(void);
void CB2_ReturnToFieldContinueScript(void);
u32 GetLastUsedWarpMapSectionId(void);
void StoreInitialPlayerAvatarState(void);
void UpdateEscapeWarp(s16 x, s16 y);
bool32 SetDiveWarpEmerge(u16 x, u16 y);
bool32 SetDiveWarpDive(u16 x, u16 y);

extern u16 *gBGTilemapBuffers1;
extern u16 *gBGTilemapBuffers2;
extern u16 *gBGTilemapBuffers3;
extern u16 gHeldKeyCodeToSend;

#endif //GUARD_OVERWORLD_H
