#include "global.h"
#include "bg.h"
#include "battle.h"
#include "battle_anim.h"
#include "pokemon.h"
#include "malloc.h"

void AllocateBattleResources(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        s32 i;

        for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
            gPokedudeBattlerStates[i] = AllocZeroed(sizeof(struct PokedudeBattlerState));
    }
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    gBattleResources = AllocZeroed(sizeof(*gBattleResources));
    gBattleResources->battleScriptsStack = AllocZeroed(sizeof(*gBattleResources->battleScriptsStack));
    gBattleResources->battleCallbackStack = AllocZeroed(sizeof(*gBattleResources->battleCallbackStack));
    gBattleResources->beforeLvlUp = AllocZeroed(sizeof(*gBattleResources->beforeLvlUp));
	gBattleResources->aiData = AllocZeroed(sizeof(*gBattleResources->aiData));
	gBattleResources->aiThinking = AllocZeroed(sizeof(*gBattleResources->aiThinking));
    gLinkBattleSendBuffer = AllocZeroed(BATTLE_BUFFER_LINK_SIZE);
    gLinkBattleRecvBuffer = AllocZeroed(BATTLE_BUFFER_LINK_SIZE);
    gBattleAnimMons_BgTilesBuffer = AllocZeroed(0x2000);
    gBattleAnimMons_BgTilemapBuffer = AllocZeroed(0x1000);
    SetBgTilemapBuffer(1, gBattleAnimMons_BgTilemapBuffer);
    SetBgTilemapBuffer(2, gBattleAnimMons_BgTilemapBuffer);
}

void FreeBattleResources(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        s32 i;

        for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        {
            FREE_AND_SET_NULL(gPokedudeBattlerStates[i]);
        }
    }
    if (gBattleResources != NULL)
    {
        FREE_AND_SET_NULL(gBattleStruct);
        FREE_AND_SET_NULL(gBattleResources->battleScriptsStack);
        FREE_AND_SET_NULL(gBattleResources->battleCallbackStack);
        FREE_AND_SET_NULL(gBattleResources->beforeLvlUp);
		FREE_AND_SET_NULL(gBattleResources->aiData);
		FREE_AND_SET_NULL(gBattleResources->aiThinking);
        FREE_AND_SET_NULL(gBattleResources);
        FREE_AND_SET_NULL(gLinkBattleSendBuffer);
        FREE_AND_SET_NULL(gLinkBattleRecvBuffer);
        FREE_AND_SET_NULL(gBattleAnimMons_BgTilesBuffer);
        FREE_AND_SET_NULL(gBattleAnimMons_BgTilemapBuffer);
    }
}

void AdjustFriendshipOnBattleFaint(u8 battlerId)
{
	u8 i, friendshipEvent, level = 0;
	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (GetBattlerSide(i) == B_SIDE_OPPONENT && gBattleMons[i].level > level)
			level = gBattleMons[i].level;
	}
	
	if (level > gBattleMons[battlerId].level && level - gBattleMons[battlerId].level > 29)
		friendshipEvent = FRIENDSHIP_EVENT_FAINT_LARGE;
	else
		friendshipEvent = FRIENDSHIP_EVENT_FAINT_SMALL;
	
	AdjustFriendship(GetBattlerPartyIndexPtr(battlerId), friendshipEvent);
}
