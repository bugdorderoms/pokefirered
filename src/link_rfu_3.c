#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "event_data.h"
#include "link.h"
#include "link_rfu.h"
#include "random.h"

static EWRAM_DATA u8 gWirelessStatusIndicatorSpriteId = 0;

static const u16 gWirelessLinkIconPalette[] = INCBIN_U16("graphics/interface/wireless_link_icon.gbapal");

static const u32 gWirelessLinkIconPic[] = INCBIN_U32("graphics/interface/wireless_link_icon.4bpp.lz");

static const struct OamData sWirelessStatusIndicatorOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
};

static const union AnimCmd sWirelessStatusIndicatorAnim0[] = {
    // 3 bars
    ANIMCMD_FRAME( 4,  5),
    ANIMCMD_FRAME( 8,  5),
    ANIMCMD_FRAME(12,  5),
    ANIMCMD_FRAME(16, 10),
    ANIMCMD_FRAME(12,  5),
    ANIMCMD_FRAME( 8,  5),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sWirelessStatusIndicatorAnim1[] = {
    // 2 bars
    ANIMCMD_FRAME( 4,  5),
    ANIMCMD_FRAME( 8,  5),
    ANIMCMD_FRAME(12, 10),
    ANIMCMD_FRAME( 8,  5),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sWirelessStatusIndicatorAnim2[] = {
    // 1 bar
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sWirelessStatusIndicatorAnim3[] = {
    // searching
    ANIMCMD_FRAME( 4, 10),
    ANIMCMD_FRAME(20, 10),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sWirelessStatusIndicatorAnim4[] = {
    // error
    ANIMCMD_FRAME(24, 10),
    ANIMCMD_FRAME( 4, 10),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sWirelessStatusIndicatorAnims[] = {
    sWirelessStatusIndicatorAnim0,
    sWirelessStatusIndicatorAnim1,
    sWirelessStatusIndicatorAnim2,
    sWirelessStatusIndicatorAnim3,
    sWirelessStatusIndicatorAnim4
};

static const struct CompressedSpriteSheet sWirelessStatusIndicatorSpriteSheet = {
    gWirelessLinkIconPic, 0x0380, 0xD431
};

static const struct SpritePalette sWirelessStatusIndicatorSpritePalette = {
    gWirelessLinkIconPalette, 0xD432
};

static const struct SpriteTemplate sWirelessStatusIndicatorSpriteTemplate = {
    .tileTag = 0xD431,
    .paletteTag = 0xD432,
    .oam = &sWirelessStatusIndicatorOamData,
    .anims = sWirelessStatusIndicatorAnims,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// A bunch of FIFO queues

void RfuRecvQueue_Reset(struct RfuRecvQueue *queue)
{
    s32 i;
    s32 j;

    for (i = 0; i < RECV_QUEUE_NUM_SLOTS; i++)
    {
        for (j = 0; j < RECV_QUEUE_SLOT_LENGTH; j++)
        {
            queue->slots[i][j] = 0;
        }
    }
    queue->send_slot = 0;
    queue->recv_slot = 0;
    queue->count = 0;
    queue->full = FALSE;
}

void RfuSendQueue_Reset(struct RfuSendQueue *queue)
{
    s32 i;
    s32 j;

    for (i = 0; i < SEND_QUEUE_NUM_SLOTS; i++)
    {
        for (j = 0; j < SEND_QUEUE_SLOT_LENGTH; j++)
        {
            queue->slots[i][j] = 0;
        }
    }
    queue->send_slot = 0;
    queue->recv_slot = 0;
    queue->count = 0;
    queue->full = FALSE;
}

void RfuRecvQueue_Enqueue(struct RfuRecvQueue *queue, u8 *src)
{
    s32 i;
    u16 imeBak;
    u8 count;

    if (queue->count < RECV_QUEUE_NUM_SLOTS)
    {
        imeBak = REG_IME;
        REG_IME = 0;
        count = 0;
        for (i = 0; i < RECV_QUEUE_SLOT_LENGTH; i += RECV_QUEUE_SLOT_LENGTH / MAX_RFU_PLAYERS)
        {
            if (src[i] == 0 && src[i + 1] == 0)
            {
                count++;
            }
        }
        if (count != MAX_RFU_PLAYERS)
        {
            for (i = 0; i < RECV_QUEUE_SLOT_LENGTH; i++)
            {
                queue->slots[queue->recv_slot][i] = src[i];
            }
            queue->recv_slot++;
            queue->recv_slot %= RECV_QUEUE_NUM_SLOTS;
            queue->count++;
            for (i = 0; i < RECV_QUEUE_SLOT_LENGTH; i++)
            {
                src[i] = 0;
            }
        }
        REG_IME = imeBak;
    }
    else
    {
        queue->full = TRUE;
    }
}

void RfuSendQueue_Enqueue(struct RfuSendQueue *queue, u8 *src)
{
    s32 i;
    u16 imeBak;

    if (queue->count < SEND_QUEUE_NUM_SLOTS)
    {
        imeBak = REG_IME;
        REG_IME = 0;
        for (i = 0; i < SEND_QUEUE_SLOT_LENGTH; i++)
        {
            if (src[i] != 0)
            {
                break;
            }
        }
        if (i != SEND_QUEUE_SLOT_LENGTH)
        {
            for (i = 0; i < SEND_QUEUE_SLOT_LENGTH; i++)
            {
                queue->slots[queue->recv_slot][i] = src[i];
            }
            queue->recv_slot++;
            queue->recv_slot %= SEND_QUEUE_NUM_SLOTS;
            queue->count++;
            for (i = 0; i < SEND_QUEUE_SLOT_LENGTH; i++)
            {
                src[i] = 0;
            }
        }
        REG_IME = imeBak;
    }
    else
    {
        queue->full = TRUE;
    }
}

bool8 RfuRecvQueue_Dequeue(struct RfuRecvQueue *queue, u8 *dest)
{
    u16 imeBak;
    s32 i;

    imeBak = REG_IME;
    REG_IME = 0;
    if (queue->recv_slot == queue->send_slot || queue->full)
    {
        for (i = 0; i < RECV_QUEUE_SLOT_LENGTH; i++)
        {
            dest[i] = 0;
        }
        REG_IME = imeBak;
        return FALSE;
    }
    for (i = 0; i < RECV_QUEUE_SLOT_LENGTH; i++)
    {
        dest[i] = queue->slots[queue->send_slot][i];
    }
    queue->send_slot++;
    queue->send_slot %= RECV_QUEUE_NUM_SLOTS;
    queue->count--;
    REG_IME = imeBak;
    return TRUE;
}

bool8 RfuSendQueue_Dequeue(struct RfuSendQueue *queue, u8 *dest)
{
    s32 i;
    u16 imeBak;

    if (queue->recv_slot == queue->send_slot || queue->full)
    {
        return FALSE;
    }
    imeBak = REG_IME;
    REG_IME = 0;
    for (i = 0; i < SEND_QUEUE_SLOT_LENGTH; i++)
    {
        dest[i] = queue->slots[queue->send_slot][i];
    }
    queue->send_slot++;
    queue->send_slot %= SEND_QUEUE_NUM_SLOTS;
    queue->count--;
    REG_IME = imeBak;
    return TRUE;
}

void RfuBackupQueue_Enqueue(struct RfuBackupQueue *queue, const u8 *dest)
{
    s32 i;

    if (dest[1] == 0)
    {
        RfuBackupQueue_Dequeue(queue, NULL);
    }
    else
    {
        for (i = 0; i < BACKUP_QUEUE_SLOT_LENGTH; i++)
        {
            queue->slots[queue->recv_slot][i] = dest[i];
        }
        queue->recv_slot++;
        queue->recv_slot %= BACKUP_QUEUE_NUM_SLOTS;
        if (queue->count < BACKUP_QUEUE_NUM_SLOTS)
        {
            queue->count++;
        }
        else
        {
            queue->send_slot = queue->recv_slot;
        }
    }
}

bool8 RfuBackupQueue_Dequeue(struct RfuBackupQueue *queue, u8 *dest)
{
    s32 i;

    if (queue->count == 0)
    {
        return FALSE;
    }
    if (dest != NULL)
    {
        for (i = 0; i < BACKUP_QUEUE_SLOT_LENGTH; i++)
        {
            dest[i] = queue->slots[queue->send_slot][i];
        }
    }
    queue->send_slot++;
    queue->send_slot %= BACKUP_QUEUE_NUM_SLOTS;
    queue->count--;
    return TRUE;
}

static u8 GetConnectedChildStrength(u8 maxFlags)
{
    u8 flagCount = 0;
    u8 flags = gRfuLinkStatus->connSlotFlag;
    u8 i;

    if (gRfuLinkStatus->parentChild == MODE_PARENT)
    {
        for (i = 0; i < 4; i++)
        {
            if (flags & 1)
            {
                if (maxFlags == flagCount + 1)
                {
                    return gRfuLinkStatus->strength[i];
                    break; // This break is needed to match
                }
                flagCount++;
            }
            flags >>= 1;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            if (flags & 1)
                return gRfuLinkStatus->strength[i];
            flags >>= 1;
        }
    }
    return 0;
}

void InitHostRFUtgtGname(struct GFtgtGname *data, u8 activity, bool32 started, s32 child_sprite_genders)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        data->unk_00.playerTrainerId[i] = gSaveBlock2Ptr->playerTrainerId[i];
    }
    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        data->child_sprite_gender[i] = child_sprite_genders;
        child_sprite_genders >>= 8;
    }
    data->playerGender = gSaveBlock2Ptr->playerGender;
    data->activity = activity;
    data->started = started;
    data->unk_00.language = GAME_LANGUAGE;
    data->unk_00.version = GAME_VERSION;
    data->unk_00.hasNews = FALSE;
    data->unk_00.hasCard = FALSE;
    data->unk_00.unknown = FALSE;
    data->unk_00.isChampion = FlagGet(FLAG_SYS_CAN_LINK_WITH_RS);
    data->unk_00.hasNationalDex = FlagGet(FLAG_SYS_POKEDEX_GET);
    data->unk_00.gameClear = FlagGet(FLAG_SYS_GAME_CLEAR);
}

/*
 * ==========================================================
 * Returns 1 if parent, 0 if child or neutral.
 * If partner serial number is valid, copies gname and uname.
 * Otherwise, blanks these.
 * ==========================================================
 */
bool8 LinkRfu_GetNameIfCompatible(struct GFtgtGname *gname, u8 *uname, u8 idx)
{
    bool8 retVal;

    if (lman.parent_child == MODE_PARENT)
    {
        retVal = TRUE;
        if (IsRfuSerialNumberValid(gRfuLinkStatus->partner[idx].serialNo) && ((gRfuLinkStatus->getNameFlag >> idx) & 1))
        {
            memcpy(gname, &gRfuLinkStatus->partner[idx].gname, RFU_GAME_NAME_LENGTH);
            memcpy(uname, gRfuLinkStatus->partner[idx].uname, RFU_USER_NAME_LENGTH);
        }
        else
        {
            memset(gname, 0, RFU_GAME_NAME_LENGTH);
            memset(uname, 0, RFU_USER_NAME_LENGTH);
        }
    }
    else
    {
        retVal = FALSE;
        if (IsRfuSerialNumberValid(gRfuLinkStatus->partner[idx].serialNo))
        {
            memcpy(gname, &gRfuLinkStatus->partner[idx].gname, RFU_GAME_NAME_LENGTH);
            memcpy(uname, gRfuLinkStatus->partner[idx].uname, RFU_USER_NAME_LENGTH);
        }
        else
        {
            memset(gname, 0, RFU_GAME_NAME_LENGTH);
            memset(uname, 0, RFU_USER_NAME_LENGTH);
        }
    }
    return retVal;
}

void LinkRfu3_SetGnameUnameFromStaticBuffers(struct GFtgtGname *gname, u8 *uname)
{
    memcpy(gname, &gHostRFUtgtGnameBuffer, RFU_GAME_NAME_LENGTH);
    memcpy(uname, gHostRFUtgtUnameBuffer, RFU_USER_NAME_LENGTH);
}

#define sNextAnimNum  data[0]
#define sSavedAnimNum data[1]
#define sCurrAnimNum  data[2]
#define sFrameDelay   data[3]
#define sFrameIdx     data[4]
#define sTileStart    data[6]
#define sValidator    data[7]
#define STATUS_INDICATOR_ACTIVE 0x1234 // Used to validate active indicator

void CreateWirelessStatusIndicatorSprite(u8 x, u8 y)
{
    u8 sprId;

    if (x == 0 && y == 0)
    {
        x = 231;
        y =   8;
    }
    if (gRfuLinkStatus->parentChild == MODE_PARENT)
    {
        sprId = CreateSprite(&sWirelessStatusIndicatorSpriteTemplate, x, y, 0);
        gSprites[sprId].sValidator = STATUS_INDICATOR_ACTIVE;
        gSprites[sprId].sTileStart = GetSpriteTileStartByTag(sWirelessStatusIndicatorSpriteSheet.tag);
        gSprites[sprId].invisible = TRUE;
        gWirelessStatusIndicatorSpriteId = sprId;
    }
    else
    {
        gWirelessStatusIndicatorSpriteId = CreateSprite(&sWirelessStatusIndicatorSpriteTemplate, x, y, 0);
        gSprites[gWirelessStatusIndicatorSpriteId].sValidator = STATUS_INDICATOR_ACTIVE;
        gSprites[gWirelessStatusIndicatorSpriteId].sTileStart = GetSpriteTileStartByTag(sWirelessStatusIndicatorSpriteSheet.tag);
        gSprites[gWirelessStatusIndicatorSpriteId].invisible = TRUE;
    }
}

void DestroyWirelessStatusIndicatorSprite(void)
{
    if (gSprites[gWirelessStatusIndicatorSpriteId].sValidator == STATUS_INDICATOR_ACTIVE)
    {
        gSprites[gWirelessStatusIndicatorSpriteId].sValidator = 0;
        DestroySprite(&gSprites[gWirelessStatusIndicatorSpriteId]);
        gMain.oamBuffer[125] = gDummyOamData;
        CpuCopy16(&gDummyOamData, (struct OamData *)OAM + 125, sizeof(struct OamData));
    }
}

void LoadWirelessStatusIndicatorSpriteGfx(void)
{
    if (GetSpriteTileStartByTag(sWirelessStatusIndicatorSpriteSheet.tag) == 0xFFFF)
    {
        LoadCompressedSpriteSheet(&sWirelessStatusIndicatorSpriteSheet);
    }
    LoadSpritePalette(&sWirelessStatusIndicatorSpritePalette);
    gWirelessStatusIndicatorSpriteId = 0xFF;
}

static u8 GetParentSignalStrength(void)
{
    u8 i;
    u8 flags = gRfuLinkStatus->connSlotFlag;
    for (i = 0; i < RFU_CHILD_MAX; i++)
    {
        if (flags & 1)
        {
            return gRfuLinkStatus->strength[i];
        }
        flags >>= 1;
    }
    return 0;
}

static void SetAndRestartWirelessStatusIndicatorAnim(struct Sprite *sprite, s32 signalStrengthAnimNum)
{
    if (sprite->sCurrAnimNum != signalStrengthAnimNum)
    {
        sprite->sCurrAnimNum = signalStrengthAnimNum;
        sprite->sFrameDelay = 0;
        sprite->sFrameIdx = 0;
    }
}

void UpdateWirelessStatusIndicatorSprite(void)
{
    if (gWirelessStatusIndicatorSpriteId != 0xFF && gSprites[gWirelessStatusIndicatorSpriteId].sValidator == STATUS_INDICATOR_ACTIVE)
    {
        struct Sprite *sprite = &gSprites[gWirelessStatusIndicatorSpriteId];
        u8 signalStrength = RFU_LINK_ICON_LEVEL4_MAX;
        u8 i = 0;
        if (gRfuLinkStatus->parentChild == MODE_PARENT)
        {
            for (i = 0; i < GetLinkPlayerCount() - 1; i++)
            {
                if (signalStrength >= GetConnectedChildStrength(i + 1))
                {
                    signalStrength = GetConnectedChildStrength(i + 1);
                }
            }
        }
        else
        {
            signalStrength = GetParentSignalStrength();
        }
        if (IsRfuRecoveringFromLinkLoss() == TRUE)
        {
            sprite->sNextAnimNum = 4;
        }
        else if (signalStrength <= RFU_LINK_ICON_LEVEL1_MAX)
        {
            sprite->sNextAnimNum = 3;
        }
        else if (signalStrength >= RFU_LINK_ICON_LEVEL2_MIN && signalStrength <= RFU_LINK_ICON_LEVEL2_MAX)
        {
            sprite->sNextAnimNum = 2;
        }
        else if (signalStrength >= RFU_LINK_ICON_LEVEL3_MIN && signalStrength <= RFU_LINK_ICON_LEVEL3_MAX)
        {
            sprite->sNextAnimNum = 1;
        }
        else if (signalStrength >= RFU_LINK_ICON_LEVEL4_MIN)
        {
            sprite->sNextAnimNum = 0;
        }
        if (sprite->sNextAnimNum != sprite->sSavedAnimNum)
        {
            SetAndRestartWirelessStatusIndicatorAnim(sprite, sprite->sNextAnimNum);
            sprite->sSavedAnimNum = sprite->sNextAnimNum;
        }
        if (sprite->anims[sprite->sCurrAnimNum][sprite->sFrameIdx].frame.duration < sprite->sFrameDelay)
        {
            sprite->sFrameIdx++;
            sprite->sFrameDelay = 0;
            if (sprite->anims[sprite->sCurrAnimNum][sprite->sFrameIdx].type == -2) // ANIMCMD_JUMP
            {
                sprite->sFrameIdx = 0;
            }
        }
        else
        {
            sprite->sFrameDelay++;
        }
        gMain.oamBuffer[125] = sWirelessStatusIndicatorOamData;
        gMain.oamBuffer[125].x = sprite->x + sprite->centerToCornerVecX;
        gMain.oamBuffer[125].y = sprite->y + sprite->centerToCornerVecY;
        gMain.oamBuffer[125].paletteNum = sprite->oam.paletteNum;
        gMain.oamBuffer[125].tileNum = sprite->sTileStart + sprite->anims[sprite->sCurrAnimNum][sprite->sFrameIdx].frame.imageValue;
        CpuCopy16(gMain.oamBuffer + 125, (struct OamData *)OAM + 125, sizeof(struct OamData));
        if (RfuGetStatus() == RFU_STATUS_FATAL_ERROR)
        {
            DestroyWirelessStatusIndicatorSprite();
        }
    }
}

#undef sNextAnimNum
#undef sSavedAnimNum
#undef sCurrAnimNum
#undef sFrameDelay
#undef sFrameIdx
#undef sTileStart
#undef sValidator

static void CopyTrainerRecord(struct TrainerNameRecord *dest, u32 trainerId, const u8 *name)
{
    int i;
    dest->trainerId = trainerId;
    for (i = 0; i < 7; i++)
    {
        if (name[i] == EOS)
            break;
        dest->trainerName[i] = name[i];
    }
    dest->trainerName[i] = EOS;
}

static void ZeroName(u8 *name)
{
    s32 i;

    for (i = 0; i < PLAYER_NAME_LENGTH + 1; i++)
    {
        *name++ = 0;
    }
}

static bool32 NameIsEmpty(const u8 *name)
{
    s32 i;

    for (i = 0; i < PLAYER_NAME_LENGTH + 1; i++)
    {
        if (*name++ != 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

// Save the currently connected players into the trainer records, shifting all previous records down.
void RecordMixTrainerNames(void)
{
    if (gWirelessCommType != 0)
    {
        s32 i;
        s32 j;
        s32 nextSpace;
        s32 connectedTrainerRecordIndices[5];
        struct TrainerNameRecord *newRecords = AllocZeroed(20 * sizeof(struct TrainerNameRecord));

        // Check if we already have a record saved for connected trainers.
        for (i = 0; i < GetLinkPlayerCount(); i++)
        {
            connectedTrainerRecordIndices[i] = -1;
            for (j = 0; j < 20; j++)
            {
                if ((u16)gLinkPlayers[i].trainerId ==  gSaveBlock1Ptr->trainerNameRecords[j].trainerId && StringCompare(gLinkPlayers[i].name, gSaveBlock1Ptr->trainerNameRecords[j].trainerName) == 0)
                {
                    connectedTrainerRecordIndices[i] = j;
                }
            }
        }

        // Save the connected trainers first, at the top of the list.
        nextSpace = 0;
        for (i = 0; i < GetLinkPlayerCount(); i++)
        {
            if (i != GetMultiplayerId() && gLinkPlayers[i].language != LANGUAGE_JAPANESE)
            {
                CopyTrainerRecord(&newRecords[nextSpace], (u16)gLinkPlayers[i].trainerId, gLinkPlayers[i].name);

                // If we already had a record for this trainer, wipe it so that the next step doesn't duplicate it.
                if (connectedTrainerRecordIndices[i] >= 0)
                {
                    ZeroName(gSaveBlock1Ptr->trainerNameRecords[connectedTrainerRecordIndices[i]].trainerName);
                }
                nextSpace++;
            }
        }

        // Copy all non-empty records to the new list, in the order they appear on the old list. If the list is full,
        // the last (oldest) records will be dropped.
        for (i = 0; i < 20; i++)
        {
            if (!NameIsEmpty(gSaveBlock1Ptr->trainerNameRecords[i].trainerName))
            {
                CopyTrainerRecord(&newRecords[nextSpace], gSaveBlock1Ptr->trainerNameRecords[i].trainerId, gSaveBlock1Ptr->trainerNameRecords[i].trainerName);
                if (++nextSpace >= 20)
                {
                    break;
                }
            }
        }

        // Finalize the new list, and clean up.
        memcpy(gSaveBlock1Ptr->trainerNameRecords, newRecords, 20 * sizeof(struct TrainerNameRecord));
        Free(newRecords);
    }
}

bool32 PlayerHasMetTrainerBefore(u16 id, u8 *name)
{
    s32 i;

    for (i = 0; i < 20; i++)
    {
        if (StringCompareN(gSaveBlock1Ptr->trainerNameRecords[i].trainerName, name, 7) == 0 && gSaveBlock1Ptr->trainerNameRecords[i].trainerId == id)
        {
            return TRUE;
        }
        if (NameIsEmpty(gSaveBlock1Ptr->trainerNameRecords[i].trainerName))
        {
            return FALSE;
        }
    }
    return FALSE;
}
