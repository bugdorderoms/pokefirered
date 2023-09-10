#include "global.h"
#include "battle_tower.h"
#include "random.h"
#include "text.h"
#include "event_data.h"
#include "string_util.h"
#include "battle_message.h"
#include "data.h"
#include "pokedex.h"
#include "overworld.h"
#include "battle_transition.h"
#include "easy_chat.h"
#include "battle_setup.h"
#include "battle_main.h"
#include "task.h"
#include "battle.h"
#include "party_menu.h"
#include "new_game.h"
#include "save.h"
#include "item.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/event_objects.h"

static EWRAM_DATA u16 sSpecialVar_0x8004_Copy = 0;

static u16 GetCurrentBattleTowerWinStreak(u8 levelType);
static void PrintEReaderTrainerFarewellMessage(void);

static const struct BattleTowerTrainer sBattleTowerTrainers[] =
{

};

static const u16 sBattleTowerHeldItems[] = {
    ITEM_NONE,
    ITEM_KINGS_ROCK,
    ITEM_SITRUS_BERRY,
    ITEM_ORAN_BERRY,
    ITEM_CHESTO_BERRY,
    ITEM_HARD_STONE,
    ITEM_FOCUS_BAND,
    ITEM_PERSIM_BERRY,
    ITEM_MIRACLE_SEED,
    ITEM_BERRY_JUICE,
    ITEM_MACHO_BRACE,
    ITEM_SILVER_POWDER,
    ITEM_CHERI_BERRY,
    ITEM_BLACK_GLASSES,
    ITEM_BLACK_BELT,
    ITEM_SOUL_DEW,
    ITEM_CHOICE_BAND,
    ITEM_MAGNET,
    ITEM_SILK_SCARF,
    ITEM_WHITE_HERB,
    ITEM_DEEP_SEA_SCALE,
    ITEM_DEEP_SEA_TOOTH,
    ITEM_MYSTIC_WATER,
    ITEM_SHARP_BEAK,
    ITEM_QUICK_CLAW,
    ITEM_LEFTOVERS,
    ITEM_RAWST_BERRY,
    ITEM_LIGHT_BALL,
    ITEM_POISON_BARB,
    ITEM_NEVER_MELT_ICE,
    ITEM_ASPEAR_BERRY,
    ITEM_SPELL_TAG,
    ITEM_BRIGHT_POWDER,
    ITEM_LEPPA_BERRY,
    ITEM_SCOPE_LENS,
    ITEM_TWISTED_SPOON,
    ITEM_METAL_COAT,
    ITEM_MENTAL_HERB,
    ITEM_CHARCOAL,
    ITEM_PECHA_BERRY,
    ITEM_SOFT_SAND,
    ITEM_LUM_BERRY,
    ITEM_DRAGON_SCALE,
    ITEM_DRAGON_FANG,
    ITEM_IAPAPA_BERRY,
    ITEM_WIKI_BERRY,
    ITEM_SEA_INCENSE,
    ITEM_SHELL_BELL,
    ITEM_SALAC_BERRY,
    ITEM_LANSAT_BERRY,
    ITEM_APICOT_BERRY,
    ITEM_STARF_BERRY,
    ITEM_LIECHI_BERRY,
    ITEM_LEEK,
    ITEM_LAX_INCENSE,
    ITEM_AGUAV_BERRY,
    ITEM_FIGY_BERRY,
    ITEM_THICK_CLUB,
    ITEM_MAGO_BERRY,
    ITEM_METAL_POWDER,
    ITEM_PETAYA_BERRY,
    ITEM_LUCKY_PUNCH,
    ITEM_GANLON_BERRY
};

#include "data/battle_tower/level_50_mons.h"
#include "data/battle_tower/level_100_mons.h"

static const u8 sMaleTrainerGfx[] =
{

};

static const u8 sFemaleTrainerGfx[] =
{

};

void SetEReaderTrainerGfxId(void)
{
    VarSet(VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_YOUNGSTER);
}

u8 GetBattleTowerTrainerFrontSpriteId(void)
{
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        return gFacilityClassToPicIndex[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        return gFacilityClassToPicIndex[sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].trainerClass];
    }
    else
    {
        return gFacilityClassToPicIndex[gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].trainerClass];
    }
}

u8 GetBattleTowerTrainerClassNameId(void)
{
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        return gFacilityClassToTrainerClass[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId >= BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        return gFacilityClassToTrainerClass[gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].trainerClass];
    }
    else
    {
        return gFacilityClassToTrainerClass[sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].trainerClass];
    }
}

void GetBattleTowerTrainerName(u8 *dest)
{
    s32 i;
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        for (i = 0; i < 7; i++)
            dest[i] = gSaveBlock2Ptr->battleTower.ereaderTrainer.name[i];
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        for (i = 0; i < 3; i++)
            dest[i] = sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].name[i];
    }
    else
    {
        for (i = 0; i < 7; i++)
            dest[i] = gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].name[i];
    }
    dest[i] = EOS;
}

static void FillBattleTowerTrainerParty(void)
{
    s32 partyIndex;
    s32 i;
    u16 chosenMonIndices[3];
    u8 friendship;
    u8 level;
    u8 fixedIV;
    u8 battleMonsOffset;
    u8 monPoolSize;
    u8 teamFlags;
    const struct BattleTowerPokemonTemplate *battleTowerMons;
	struct PokemonGenerator generator;

    battleMonsOffset = 0;
    monPoolSize = 60;
    friendship = 255;

    ZeroEnemyPartyMons();

    // Different trainers have access to different sets of pokemon to use in battle.
    // The pokemon later in gBattleTowerLevel100Mons or gBattleTowerLevel50Mons are
    // stronger. Additionally, the later trainers' pokemon are granted higher IVs.
    if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 20)
    {
        fixedIV = 6;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 30)
    {
        fixedIV = 9;
        battleMonsOffset = 30;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 40)
    {
        fixedIV = 12;
        battleMonsOffset = 60;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 50)
    {
        fixedIV = 15;
        battleMonsOffset = 90;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 60)
    {
        fixedIV = 18;
        battleMonsOffset = 120;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 70)
    {
        fixedIV = 21;
        battleMonsOffset = 150;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < 80)
    {
        fixedIV = 31;
        battleMonsOffset = 180;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId < BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID)
    {
        fixedIV = 31;
        battleMonsOffset = 200;
        monPoolSize = 100;
    }
    else if (gSaveBlock2Ptr->battleTower.battleTowerTrainerId == BATTLE_TOWER_EREADER_TRAINER_ID)
    {
        // Load E-Reader trainer's party.
        do
        {
            for (partyIndex = 0; partyIndex < 3; partyIndex++)
                CreateBattleTowerMon(&gEnemyParty[partyIndex], &gSaveBlock2Ptr->battleTower.ereaderTrainer.party[partyIndex]);
            return;
        } while (0);
    }
    else
    {
        // Load a battle tower record's party. (From record mixing)
        for (partyIndex = 0; partyIndex < 3; partyIndex++)
        {
            CreateBattleTowerMon(
                &gEnemyParty[partyIndex],
                &gSaveBlock2Ptr->battleTower.records[gSaveBlock2Ptr->battleTower.battleTowerTrainerId - BATTLE_TOWER_RECORD_MIXING_TRAINER_BASE_ID].party[partyIndex]);
        }
        return;
    }

    // Use the appropriate list of pokemon and level depending on the
    // current challenge type. (level 50 or level 100 challenge)
    if (gSaveBlock2Ptr->battleTower.battleTowerLevelType != 0)
    {
        battleTowerMons = gBattleTowerLevel100Mons;
        level = 100;
    }
    else
    {
        battleTowerMons = gBattleTowerLevel50Mons;
        level = 50;
    }

    teamFlags = sBattleTowerTrainers[gSaveBlock2Ptr->battleTower.battleTowerTrainerId].teamFlags;

    // Attempt to fill the trainer's party with random Pokemon until 3 have been
    // successfully chosen. The trainer's party may not have duplicate pokemon species
    // or duplicate held items. Each pokemon must have all of the trainer's team flags
    // set, as well.  If any of those conditions are not met, then the loop starts over
    // and another pokemon is chosen at random.
    partyIndex = 0;
    while (partyIndex != 3)
    {
        // Pick a random pokemon index based on the number of pokemon available to choose from
        // and the starting offset in the battle tower pokemon array.
        s32 battleMonIndex = ((Random() & 0xFF) * monPoolSize) / 256 + battleMonsOffset;

        // Ensure the chosen pokemon has compatible team flags with the trainer.
        if (teamFlags == 0 || (battleTowerMons[battleMonIndex].teamFlags & teamFlags) == teamFlags)
        {
            // Ensure this pokemon species isn't a duplicate.
            for (i = 0; i < partyIndex; i++)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES, NULL) == battleTowerMons[battleMonIndex].species)
                    break;
            }

            if (i != partyIndex)
                continue;

            // Ensure this pokemon's held item isn't a duplicate.
            for (i = 0; i < partyIndex; i++)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, NULL) != ITEM_NONE
                    && GetMonData(&gEnemyParty[i], MON_DATA_HELD_ITEM, NULL) == sBattleTowerHeldItems[battleTowerMons[battleMonIndex].heldItem])
                    break;
            }

            if (i != partyIndex)
                continue;

            // Ensure this exact pokemon index isn't a duplicate. This check doesn't seem necessary
            // because the species and held items were already checked directly above. Perhaps this
            // is leftover code before the logic for duplicate species and held items was added.
            //for (i = 0; i < partyIndex && chosenMonIndices[i] != battleMonIndex; i++);
            for (i = 0; i < partyIndex; i++)
            {
                if (chosenMonIndices[i] == battleMonIndex)
                    break;
            }

            if (i != partyIndex)
                continue;

            chosenMonIndices[partyIndex] = battleMonIndex;
			
			generator.species = battleTowerMons[battleMonIndex].species;
			generator.level = level;
			generator.otIdType = OT_ID_PLAYER_ID;
			generator.shinyType = GENERATE_SHINY_NORMAL;
			generator.forceGender = FALSE;
			generator.forcedGender = MON_MALE;
			generator.hasFixedPersonality = FALSE;
			generator.fixedPersonality = 0;
			generator.forceNature = FALSE;
			generator.forcedNature = NUM_NATURES;
			generator.pokemon = &gEnemyParty[partyIndex];
			
            // Place the chosen pokemon into the trainer's party.
			CreateMon(generator);
			
			for (i = 0; i < NUM_STATS; i++)
				SetMonData(&gEnemyParty[partyIndex], MON_DATA_HP_IV + i, &fixedIV);
			
			GiveMonEvSpread(&gEnemyParty[partyIndex], battleTowerMons[battleMonIndex].evSpread);
			
            // Give the chosen pokemon its specified moves.
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                SetMonMoveSlot(&gEnemyParty[partyIndex], battleTowerMons[battleMonIndex].moves[i], i);
                if (battleTowerMons[battleMonIndex].moves[i] == MOVE_FRUSTRATION)
                    friendship = 0;  // MOVE_FRUSTRATION is more powerful the lower the pokemon's friendship is.
            }

            SetMonData(&gEnemyParty[partyIndex], MON_DATA_FRIENDSHIP, &friendship);
            SetMonData(&gEnemyParty[partyIndex], MON_DATA_HELD_ITEM, &sBattleTowerHeldItems[battleTowerMons[battleMonIndex].heldItem]);

            // The pokemon was successfully added to the trainer's party, so it's safe to move on to
            // the next party slot.
            partyIndex++;
        }
    }
}

static void BufferBattleTowerTrainerMessage(const u16 *greeting)
{
    s32 i;
    if (EC_DoesEasyChatStringFitOnLine(greeting, 3, 2, 18))
    {
        ConvertEasyChatWordsToString(gStringVar4, greeting, 2, 3);
        i = 0;
        while (gStringVar4[i++] != CHAR_NEWLINE)
            ;
        while (gStringVar4[i] != CHAR_NEWLINE)
            i++;
        gStringVar4[i] = CHAR_PROMPT_SCROLL;
    }
    else
    {
        ConvertEasyChatWordsToString(gStringVar4, greeting, 3, 2);
    }
}

static void CB2_FinishEReaderBattle(void)
{
    s32 i;
    u16 heldItem;

    switch (sSpecialVar_0x8004_Copy)
    {
    case 0:
        break;
    case 1:
        for (i = 0; i < PARTY_SIZE; i++)
        {
            heldItem = GetMonData(&gSaveBlock1Ptr->playerParty[i], MON_DATA_HELD_ITEM);
            SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &heldItem);
        }
        break;
    case 2:
        PrintEReaderTrainerFarewellMessage();
        break;
    }

    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

static void Task_WaitBT(u8 taskId)
{
    if (BT_IsDone() == TRUE)
    {
        gMain.savedCallback = CB2_FinishEReaderBattle;
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_InitBattle);
        DestroyTask(taskId);
    }
}


void StartSpecialBattle(void)
{
    s32 i;
    u16 heldItem;
    u8 transition;

    sSpecialVar_0x8004_Copy = gSpecialVar_0x8004;
    switch (sSpecialVar_0x8004_Copy)
    {
    case 0: // battle tower battle
        gBattleTypeFlags = (BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_TRAINER);
        gTrainerBattleOpponent_A = 0;

        FillBattleTowerTrainerParty();

        CreateTask(Task_WaitBT, 1);
        PlayMapChosenOrBattleBGM(0);
        transition = BattleSetup_GetBattleTowerBattleTransition();
        BT_StartOnField(transition);
        break;
    case 1: // secret base battle
        for (i = 0; i < PARTY_SIZE; i++)
        {
            heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
            SetMonData(&gSaveBlock1Ptr->playerParty[i], MON_DATA_HELD_ITEM, &heldItem);
        }

        CreateTask(Task_WaitBT, 1);
        PlayMapChosenOrBattleBGM(0);
        transition = BattleSetup_GetBattleTowerBattleTransition();
        BT_StartOnField(transition);
        break;
    case 2: // e-reader trainer battle
        ZeroEnemyPartyMons();

        for (i = 0; i < 3; i++)
            CreateBattleTowerMon(&gEnemyParty[i], &gSaveBlock2Ptr->battleTower.ereaderTrainer.party[i]);

        gBattleTypeFlags = (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER);
        gTrainerBattleOpponent_A = 0;

        CreateTask(Task_WaitBT, 1);
        PlayMapChosenOrBattleBGM(0);
        transition = BattleSetup_GetBattleTowerBattleTransition();
        BT_StartOnField(transition);
        break;
    }
}

static void ClearEReaderTrainer(struct BattleTowerEReaderTrainer *ereaderTrainer)
{
    u32 i;

    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32); i++)
        ((u32 *)ereaderTrainer)[i] = 0;
}

static u16 GetCurrentBattleTowerWinStreak(u8 battleTowerLevelType)
{
    u16 winStreak = ((gSaveBlock2Ptr->battleTower.curStreakChallengesNum[battleTowerLevelType] - 1) * 7 - 1)
                    + gSaveBlock2Ptr->battleTower.curChallengeBattleNum[battleTowerLevelType];

    if (winStreak > 9999)
        return 9999;
    else
        return winStreak;
}

u8 GetEreaderTrainerFrontSpriteId(void)
{
    return gFacilityClassToPicIndex[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
}

u8 GetEreaderTrainerClassId(void)
{
    return gFacilityClassToTrainerClass[gSaveBlock2Ptr->battleTower.ereaderTrainer.trainerClass];
}

void CopyEReaderTrainerName5(u8 *trainerName)
{
    s32 i;

    for (i = 0; i < 5; i++)
        trainerName[i] = gSaveBlock2Ptr->battleTower.ereaderTrainer.name[i];

    trainerName[i] = EOS;
}

// Checks if the saved E-Reader trainer is valid.
void ValidateEReaderTrainer(void)
{
    u32 i;
    u32 checksum;
    struct BattleTowerEReaderTrainer *ereaderTrainer;

    gSpecialVar_Result = 0;
    ereaderTrainer = &gSaveBlock2Ptr->battleTower.ereaderTrainer;

    checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        checksum |= ((u32 *)ereaderTrainer)[i];

    if (checksum == 0)
    {
        gSpecialVar_Result = 1;
        return;
    }

    checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        checksum += ((u32 *)ereaderTrainer)[i];

    if (gSaveBlock2Ptr->battleTower.ereaderTrainer.checksum != checksum)
    {
        ClearEReaderTrainer(&gSaveBlock2Ptr->battleTower.ereaderTrainer);
        gSpecialVar_Result = 1;
    }
}

void BufferEReaderTrainerGreeting(void)
{
    BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.greeting);
}

static void PrintEReaderTrainerFarewellMessage(void)
{
    if (gBattleOutcome == B_OUTCOME_DREW)
        gStringVar4[0] = EOS;
    else if (gBattleOutcome == B_OUTCOME_WON)
        BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.farewellPlayerWon);
    else
        BufferBattleTowerTrainerMessage(gSaveBlock2Ptr->battleTower.ereaderTrainer.farewellPlayerLost);
}
