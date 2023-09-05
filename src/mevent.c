#include "global.h"
#include "gflib.h"
#include "constants/songs.h"
#include "easy_chat.h"
#include "task.h"
#include "decompress.h"
#include "link.h"
#include "link_rfu.h"
#include "util.h"
#include "script.h"
#include "event_data.h"
#include "battle_tower.h"
#include "new_game.h"
#include "mystery_gift_menu.h"
#include "mevent.h"
#include "strings.h"

static bool32 IsReceivedWonderCardHeaderValid(const struct WonderCard * src);
static void RecordIdOfWonderCardSender(u32 eventId, u32 trainerId, u32 *idsList, s32 count);

static EWRAM_DATA bool32 sReceivedWonderCardIsValid = FALSE;

u16 * GetMEventProfileECWordsMaybe(void)
{
    return gSaveBlock1Ptr->mysteryGift.questionnaireWords;
}

static bool32 ValidateReceivedWonderCard(void)
{
    if (gSaveBlock1Ptr->mysteryGift.cardCrc != CalcCRC16WithTable((void *)&gSaveBlock1Ptr->mysteryGift.card, sizeof(struct WonderCard)))
        return FALSE;
    if (!IsReceivedWonderCardHeaderValid(&gSaveBlock1Ptr->mysteryGift.card))
        return FALSE;
    if (!ValidateRamScript())
        return FALSE;
    return TRUE;
}

static bool32 IsReceivedWonderCardHeaderValid(const struct WonderCard * data)
{
    if (data->flagId == 0)
        return FALSE;
    if (data->type > 2)
        return FALSE;
    if (!(data->sendType == 0 || data->sendType == 1 || data->sendType == 2))
        return FALSE;
    if (data->bgType > 7)
        return FALSE;
    if (data->maxStamps > 7)
        return FALSE;
    return TRUE;
}

u16 GetWonderCardFlagId(void)
{
    if (ValidateReceivedWonderCard())
        return gSaveBlock1Ptr->mysteryGift.card.flagId;
    return 0;
}

// Increments an interaction count in the save block
static void IncrementBattleCardCount(u32 command)
{
    struct WonderCard * data = &gSaveBlock1Ptr->mysteryGift.card;
    if (data->type == 2)
    {
        u16 * dest = NULL;
        switch (command)
        {
            case 0:
                dest = &gSaveBlock1Ptr->mysteryGift.cardMetadata.battlesWon;
                break;
            case 1:
                dest = &gSaveBlock1Ptr->mysteryGift.cardMetadata.battlesLost;
                break;
            case 2:
                dest = &gSaveBlock1Ptr->mysteryGift.cardMetadata.numTrades;
                break;
            case 3:
                break;
            case 4:
                break;
        }
        if (dest == NULL)
        {
             AGB_ASSERT_EX(0, ABSPATH("mevent.c"), 868);
        }
        else if (++(*dest) > 999)
        {
            *dest = 999;
        }
    }
}

void ResetReceivedWonderCardFlag(void)
{
    sReceivedWonderCardIsValid = FALSE;
}

bool32 MEventHandleReceivedWonderCard(u16 flagId)
{
    sReceivedWonderCardIsValid = FALSE;
    if (flagId == 0)
        return FALSE;
    if (!ValidateReceivedWonderCard())
        return FALSE;
    if (gSaveBlock1Ptr->mysteryGift.card.flagId != flagId)
        return FALSE;
    sReceivedWonderCardIsValid = TRUE;
    return TRUE;
}

void MEvent_RecordIdOfWonderCardSenderByEventType(u32 eventId, u32 trainerId)
{
    if (sReceivedWonderCardIsValid)
    {
        switch (eventId)
        {
            case 2: // trade
                RecordIdOfWonderCardSender(2, trainerId, gSaveBlock1Ptr->mysteryGift.trainerIds[1], 5);
                break;
            case 0: // link win
                RecordIdOfWonderCardSender(0, trainerId, gSaveBlock1Ptr->mysteryGift.trainerIds[0], 5);
                break;
            case 1: // link loss
                RecordIdOfWonderCardSender(1, trainerId, gSaveBlock1Ptr->mysteryGift.trainerIds[0], 5);
                break;
            default:
                 AGB_ASSERT_EX(0, ABSPATH("mevent.c"), 988);
        }
    }
}

// Looks up trainerId in an array idsList with count elements.
// If trainerId is found, rearranges idsList to put it in the front.
// Otherwise, drops the last element of the list and inserts
// trainerId at the front.
// Returns TRUE in the latter case.
static bool32 PlaceTrainerIdAtFrontOfList(u32 trainerId, u32 * idsList, s32 count)
{
    s32 i;
    s32 j;

    for (i = 0; i < count; i++)
    {
        if (idsList[i] == trainerId)
            break;
    }
    if (i == count)
    {
        for (j = count - 1; j > 0; j--)
        {
            idsList[j] = idsList[j - 1];
        }
        idsList[0] = trainerId;
        return TRUE;
    }
    else
    {
        for (j = i; j > 0; j--)
        {
            idsList[j] = idsList[j - 1];
        }
        idsList[0] = trainerId;
        return FALSE;
    }
}

static void RecordIdOfWonderCardSender(u32 eventId, u32 trainerId, u32 * idsList, s32 count)
{
    if (PlaceTrainerIdAtFrontOfList(trainerId, idsList, count))
        IncrementBattleCardCount(eventId);
}
