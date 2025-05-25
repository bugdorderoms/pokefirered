#include "global.h"
#include "gflib.h"
#include "data.h"
#include "party_menu.h"
#include "pokemon_special_anim_internal.h"
#include "item_use.h"
#include "task.h"
#include "item.h"
#include "constants/songs.h"
#include "constants/items.h"

// Functions related to the special anims Pokemon
// make when using an item on them in the field.

static EWRAM_DATA bool32 sCancelDisabled = FALSE;
static EWRAM_DATA u8 sPSATaskId = 0;
static EWRAM_DATA struct PokemonSpecialAnim * sPSAWork = NULL;

static struct PokemonSpecialAnim * AllocPSA(u32 slotId, u32 itemId, MainCallback callback);
static void SetUpUseItemAnim_Normal(struct PokemonSpecialAnim * ptr);
static void SetUpUseItemAnim_ForgetMoveAndLearnTMorHM(struct PokemonSpecialAnim * ptr);
static void Task_UseItem_Normal(u32 taskId);
static void Task_ForgetMove(u32 taskId);
static void Task_UseTM_NoForget(u32 taskId);
static void Task_MachineSet(u32 taskId);
static void Task_CleanUp(u32 taskId);
static u32 GetClosenessFromFriendship(u32 friendship);
static u32 GetAnimTypeByItemId(u32 itemId);

void StartUseItemAnim_Normal(u32 slotId, u32 itemId, MainCallback callback)
{
    struct PokemonSpecialAnim * ptr = AllocPSA(slotId, itemId, callback);
    if (ptr == NULL)
        SetMainCallback2(callback);
    else
        SetUpUseItemAnim_Normal(ptr);
}

void StartUseItemAnim_ForgetMoveAndLearnTMorHM(u32 slotId, u32 itemId, u32 moveId, MainCallback callback)
{
    struct PokemonSpecialAnim * ptr = AllocPSA(slotId, itemId, callback);
    if (ptr == NULL)
        SetMainCallback2(callback);
    else
    {
        StringCopy(ptr->nameOfMoveForgotten, gBattleMoves[moveId].name);
        SetUpUseItemAnim_ForgetMoveAndLearnTMorHM(ptr);
    }
}

static struct PokemonSpecialAnim * AllocPSA(u32 slotId, u32 itemId, MainCallback callback)
{
    struct PokemonSpecialAnim * ptr;
    struct Pokemon * pokemon;

    if (!gMain.inBattle)
        ResetTasks();
    
    ResetSpriteData();
    FreeAllSpritePalettes();
    ptr = Alloc(sizeof(struct PokemonSpecialAnim));
    if (ptr == NULL)
    {
        SetMainCallback2(callback);
        return NULL;
    }
    pokemon = &gPlayerParty[slotId];
    ptr->state = 0;
    ptr->savedCallback = callback;
    ptr->species = GetMonData(pokemon, MON_DATA_SPECIES);
    ptr->closeness = GetClosenessFromFriendship(GetMonData(pokemon, MON_DATA_FRIENDSHIP));
    ptr->personality = GetMonData(pokemon, MON_DATA_PERSONALITY);
    ptr->slotId = slotId;
    ptr->itemId = itemId;
    ptr->animType = GetAnimTypeByItemId(itemId);
    ptr->pokemon = *pokemon;
    ptr->field_00a4 = 0;
    GetMonData(pokemon, MON_DATA_NICKNAME, ptr->nickname);
    
    if (ptr->animType == 4)
        StringCopy(ptr->nameOfMoveToTeach, gBattleMoves[ItemId_GetHoldEffectParam(itemId)].name);

    return ptr;
}

static void VBlankCB_PSA(void)
{
    TransferPlttBuffer();
    LoadOam();
    ProcessSpriteCopyRequests();
}

static void CB2_PSA(void)
{
    RunTextPrinters();
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void SetUseItemAnimCallback(u32 taskId, TaskFunc func)
{
    struct PokemonSpecialAnim * ptr = (void *)GetWordTaskArg(taskId, 0);
    ptr->state = 0;
    gTasks[taskId].func = func;
}

static void SetUpUseItemAnim_Normal(struct PokemonSpecialAnim * ptr)
{
    u32 taskId;
    
    switch (ptr->animType)
    {
    case 0:
    case 1:
    case 3:
        taskId = CreateTask(Task_UseItem_Normal, 0);
        break;
    case 4:
        taskId = CreateTask(Task_UseTM_NoForget, 0);
        break;
    default:
        SetMainCallback2(ptr->savedCallback);
        Free(ptr);
        return;
    }
    ptr->cancelDisabled = FALSE;
    SetWordTaskArg(taskId, 0, (uintptr_t)ptr);
    SetMainCallback2(CB2_PSA);
    sPSATaskId = taskId;
}

static void SetUpUseItemAnim_ForgetMoveAndLearnTMorHM(struct PokemonSpecialAnim * ptr)
{
    u32 taskId = CreateTask(Task_ForgetMove, 0);
    SetWordTaskArg(taskId, 0, (uintptr_t)ptr);
    SetMainCallback2(CB2_PSA);
    sPSATaskId = taskId;
    ptr->cancelDisabled = FALSE;
}

static void Task_UseItem_Normal(u32 taskId)
{
    struct PokemonSpecialAnim * ptr = (void *)GetWordTaskArg(taskId, 0);
    
    if (!ptr->cancelDisabled && JOY_HELD(A_BUTTON | B_BUTTON))
    {
        PSA_UseItem_CleanUpForCancel();
        SetUseItemAnimCallback(taskId, Task_CleanUp);
        return;
    }

    switch (ptr->state)
    {
    case 0:
        SetVBlankCallback(NULL);
        InitPokemonSpecialAnimScene(&ptr->sceneResources, ptr->animType);
        PSA_CreateMonSpriteAtCloseness(0);
        ptr->state++;
        break;
    case 1:
        if (!PokemonSpecialAnimSceneInitIsNotFinished())
        {
            BeginNormalPaletteFade(PALETTES_ALL, -1, 16, 0, RGB_BLACK);
            ptr->state++;
            SetVBlankCallback(VBlankCB_PSA);
        }
        break;
    case 2:
        if (!gPaletteFade.active)
            ptr->state++;
        break;
    case 3:
        PSA_SetUpZoomAnim(ptr->closeness);
        ptr->state++;
        break;
    case 4:
        if (!PSA_IsZoomTaskActive())
        {
            ptr->delayTimer = 0;
            ptr->state++;
        }
        break;
    case 5:
        if (!LevelUpVerticalSpritesTaskIsRunning())
            ptr->state++;
        break;
    case 6:
        PSA_SetUpItemUseOnMonAnim(ptr->itemId, ptr->closeness, TRUE);
        ptr->state++;
        break;
    case 7:
        if (!PSA_IsItemUseOnMonAnimActive())
        {
            ptr->cancelDisabled = TRUE;
            
            if (ptr->closeness == 3)
                PlayCry_Normal(ptr->species, 0);

            PSA_ShowMessageWindow();
            ptr->state++;
        }
        break;
    case 8:
        PSA_PrintMessage(PSA_TEXT_ITEM_USED);
        ptr->state++;
        break;
    case 9:
        if (!PSA_IsMessagePrintTaskActive())
            ptr->state++;
        break;
    case 10:
        PSA_SetUpZoomAnim(0);
        ptr->state++;
        break;
    case 11:
        if (!PSA_IsZoomTaskActive())
        {
            ptr->cancelDisabled = TRUE;
            ptr->state++;
        }
        break;
    case 12:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            if (GetItemUseAnimFollowUpCBIdByItemType(ptr->itemId) != ITEMUSE_FOLLOWUP_EVOLUTION_ITEM)
            {
                BeginNormalPaletteFade(PALETTES_ALL, -1, 0, 16, RGB_BLACK);
                ptr->state++;
            }
            else
                ptr->state += 2;
        }
        break;
    case 13:
        if (!gPaletteFade.active)
            ptr->state++;
        break;
    case 14:
        SetMainCallback2(ptr->savedCallback);
        FreeAllWindowBuffers();
        Free(ptr);
        DestroyTask(taskId);
        break;
    }
}

static void Task_ForgetMove(u32 taskId)
{
    struct PokemonSpecialAnim * ptr = (void *)GetWordTaskArg(taskId, 0);

    switch (ptr->state)
    {
    case 0:
        SetVBlankCallback(NULL);
        InitPokemonSpecialAnimScene(&ptr->sceneResources, ptr->animType);
        PSA_CreateMonSpriteAtCloseness(3);
        ptr->state++;
        break;
    case 1:
        if (!PokemonSpecialAnimSceneInitIsNotFinished())
        {
            BeginNormalPaletteFade(PALETTES_ALL, -1, 16, 0, RGB_BLACK);
            ptr->state++;
            SetVBlankCallback(VBlankCB_PSA);
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            ptr->delayTimer = 0;
            ptr->state++;
        }
        break;
    case 3:
        ptr->delayTimer++;
        if (ptr->delayTimer > 30)
        {
            PSA_ShowMessageWindow();
            ptr->state++;
        }
        break;
    case 4:
        PSA_PrintMessage(PSA_TEXT_FORGET_1);
        ptr->state++;
        break;
    case 5:
        if (!PSA_IsMessagePrintTaskActive())
        {
            ptr->delayTimer = 0;
            ptr->state++;
        }
        break;
    case 6:
        ptr->delayTimer++;
        if (ptr->delayTimer > 30)
        {
            PSA_PrintMessage(PSA_TEXT_FORGET_2_AND);
            ptr->state++;
        }
        break;
    case 7:
        if (!PSA_IsMessagePrintTaskActive())
        {
            ptr->delayTimer = 0;
            ptr->state++;
        }
        break;
    case 8:
        ptr->delayTimer++;
        if (ptr->delayTimer > 30)
        {
            PlaySE(SE_M_SPIT_UP);
            PSA_PrintMessage(PSA_TEXT_FORGET_POOF);
            PSA_DarkenMonSprite();
            ptr->state++;
        }
        break;
    case 9:
        if (!(PSA_RunPoofAnim() | PSA_IsMessagePrintTaskActive()))
        {
            PSA_AfterPoof_ClearMessageWindow();
            ptr->state++;
        }
        break;
    case 10:
        PSA_PrintMessage(PSA_TEXT_FORGET_FORGOT);
        ptr->state++;
        break;
    case 11:
        if (!PSA_IsMessagePrintTaskActive())
        {
            PSA_PrintMessage(PSA_TEXT_FORGET_AND);
            ptr->state++;
        }
        break;
    case 12:
        if (!PSA_IsMessagePrintTaskActive())
        {
            PSA_HideMessageWindow();
            ptr->state++;
        }
        break;
    case 13:
        SetUseItemAnimCallback(taskId, Task_MachineSet);
        break;
    }
}

static void Task_UseTM_NoForget(u32 taskId)
{
    struct PokemonSpecialAnim * ptr = (void *)GetWordTaskArg(taskId, 0);

    if (JOY_NEW(B_BUTTON))
    {
        SetUseItemAnimCallback(taskId, Task_CleanUp);
        return;
    }

    switch (ptr->state)
    {
    case 0:
        SetVBlankCallback(NULL);
        InitPokemonSpecialAnimScene(&ptr->sceneResources, ptr->animType);
        PSA_CreateMonSpriteAtCloseness(3);
        ptr->state++;
        break;
    case 1:
        if (!PokemonSpecialAnimSceneInitIsNotFinished())
        {
            BeginNormalPaletteFade(PALETTES_ALL, -1, 16, 0, RGB_BLACK);
            ptr->state++;
            SetVBlankCallback(VBlankCB_PSA);
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            ptr->delayTimer = 0;
            ptr->state++;
        }
        break;
    case 3:
        ptr->delayTimer++;
        if (ptr->delayTimer > 20)
            SetUseItemAnimCallback(taskId, Task_MachineSet);
        break;
    }
}

static void Task_MachineSet(u32 taskId)
{
    struct PokemonSpecialAnim * ptr = (void *)GetWordTaskArg(taskId, 0);

    if (!ptr->cancelDisabled && JOY_NEW(B_BUTTON))
    {
        PSA_UseTM_CleanUpForCancel();
        SetUseItemAnimCallback(taskId, Task_CleanUp);
        return;
    }

    switch (ptr->state)
    {
    case 0:
        CreateItemIconSpriteAtMaxCloseness(ptr->itemId);
        ptr->delayTimer = 0;
        ptr->state++;
        break;
    case 1:
        PSA_ShowMessageWindow();
        PSA_PrintMessage(PSA_TEXT_MACHINE_SET);
        ptr->state++;
        break;
    case 2:
        if (!PSA_IsMessagePrintTaskActive())
        {
            PSA_HideMessageWindow();
            ptr->state++;
        }
        break;
    case 3:
        PSA_UseTM_SetUpMachineSetWobble();
        ptr->state++;
        break;
    case 4:
        if (!PSA_UseTM_RunMachineSetWobble())
            ptr->state++;
        break;
    case 5:
        PSA_UseTM_SetUpZoomOutAnim();
        ptr->state++;
        break;
    case 6:
        if (!PSA_UseTM_RunZoomOutAnim())
        {
            ptr->delayTimer = 0;
            ptr->state++;
        }
        break;
    case 7:
        ptr->delayTimer++;
        if (ptr->delayTimer > 30)
        {
            PSA_ShowMessageWindow();
            PSA_PrintMessage(PSA_TEXT_LEARNED_MOVE);
            ptr->state++;
        }
        break;
    case 8:
        if (!PSA_IsMessagePrintTaskActive())
        {
            PlayFanfare(MUS_LEVEL_UP);
            ptr->cancelDisabled = TRUE;
            ptr->state++;
        }
        break;
    case 9:
        if (IsFanfareTaskInactive())
            SetUseItemAnimCallback(taskId, Task_CleanUp);
        break;
    }
}

static void Task_CleanUp(u32 taskId)
{
    struct PokemonSpecialAnim * ptr = (void *)GetWordTaskArg(taskId, 0);

    switch (ptr->state)
    {
    case 0:
        SetVBlankCallback(VBlankCB_PSA);
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        ptr->state++;
        break;
    case 1:
        if (!gPaletteFade.active && (ptr->field_00a4 != 1 || IsCryFinished()))
        {
            sCancelDisabled = ptr->cancelDisabled;
            SetMainCallback2(ptr->savedCallback);
            DestroyTask(taskId);
            FreeAllWindowBuffers();
            Free(ptr);
        }
        break;
    }
}

static u32 GetAnimTypeByItemId(u32 itemId)
{
    return ItemId_GetPocket(itemId) == POCKET_TM_CASE ? 4 : 0;
}

static u32 GetClosenessFromFriendship(u32 friendship)
{
    if (friendship <= 100)
        return 0;
    else if (friendship <= 150)
        return 1;
    else if (friendship <= 200)
        return 2;
    else
        return 3;
}

struct PokemonSpecialAnim * GetPSAStruct(void)
{
    return (void *)GetWordTaskArg(sPSATaskId, 0);
}

struct Pokemon * PSA_GetPokemon(void)
{
    sPSAWork = GetPSAStruct();
    return &sPSAWork->pokemon;
}

struct PokemonSpecialAnimScene * PSA_GetSceneWork(void)
{
    return &GetPSAStruct()->sceneResources;
}

u32 PSA_GetItemId(void)
{
    return GetPSAStruct()->itemId;
}

u8 *PSA_GetNameOfMoveForgotten(void)
{
    return GetPSAStruct()->nameOfMoveForgotten;
}

u8 *PSA_GetNameOfMoveToTeach(void)
{
    return GetPSAStruct()->nameOfMoveToTeach;
}

u8 *PSA_CopyMonNickname(u8 *dest)
{
    return StringCopy(dest, GetPSAStruct()->nickname);
}

u8 *PSA_GetMonNickname(void)
{
    return GetPSAStruct()->nickname;
}

u32 PSA_GetAnimType(void)
{
    return GetPSAStruct()->animType;
}

u32 PSA_GetMonSpecies(void)
{
    return GetPSAStruct()->species;
}

u32 PSA_GetMonPersonality(void)
{
    return GetPSAStruct()->personality;
}

void GetMonLevelUpWindowStats(struct Pokemon * pokemon, u16 *data)
{
    data[0] = GetMonData(pokemon, MON_DATA_MAX_HP);
    data[1] = GetMonData(pokemon, MON_DATA_ATK);
    data[2] = GetMonData(pokemon, MON_DATA_DEF);
    data[3] = GetMonData(pokemon, MON_DATA_SPEED);
    data[4] = GetMonData(pokemon, MON_DATA_SPATK);
    data[5] = GetMonData(pokemon, MON_DATA_SPDEF);
}

bool32 PSA_IsCancelDisabled(void)
{
    return sCancelDisabled;
}
