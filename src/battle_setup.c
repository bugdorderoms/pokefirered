#include "global.h"
#include "task.h"
#include "overworld.h"
#include "item.h"
#include "sound.h"
#include "pokemon.h"
#include "load_save.h"
#include "safari_zone.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "strings.h"
#include "string_util.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "metatile_behavior.h"
#include "event_scripts.h"
#include "fldeff.h"
#include "fieldmap.h"
#include "util.h"
#include "field_control_avatar.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_message_box.h"
#include "random.h"
#include "vs_seeker.h"
#include "data.h"
#include "battle.h"
#include "battle_transition.h"
#include "battle_controllers.h"
#include "wild_encounter.h"
#include "constants/battle_setup.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/songs.h"
#include "constants/pokemon.h"
#include "constants/trainers.h"
#include "constants/moves.h"

enum
{
    TRAINER_PARAM_LOAD_VAL_8BIT,
    TRAINER_PARAM_LOAD_VAL_16BIT,
    TRAINER_PARAM_LOAD_VAL_32BIT,
    TRAINER_PARAM_CLEAR_VAL_8BIT,
    TRAINER_PARAM_CLEAR_VAL_16BIT,
    TRAINER_PARAM_CLEAR_VAL_32BIT,
    TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR,
};

struct TrainerBattleParameter
{
    void *varPtr;
    u8 ptrType;
};

static void DoSafariBattle(void);
static void DoGhostBattle(void);
static void DoStandardWildBattle(bool8 isDouble);
static void CB2_EndWildBattle(void);
static void CB2_EndScriptedWildBattle(void);
static void CB2_EndMarowakBattle(void);
static void CB2_EndTrainerBattle(void);
static u8 GetWildBattleTransition(void);
static u8 GetTrainerBattleTransition(void);
static bool32 IsPlayerDefeated(u32 battleOutcome);

static EWRAM_DATA u16 sTrainerBattleMode = 0;
EWRAM_DATA u16 gTrainerBattleOpponent_A = 0;
static EWRAM_DATA u16 sTrainerObjectEventLocalId = 0;
static EWRAM_DATA u8 *sTrainerAIntroSpeech = NULL;
static EWRAM_DATA u8 *sTrainerADefeatSpeech = NULL;
static EWRAM_DATA u8 *sTrainerVictorySpeech = NULL;
static EWRAM_DATA u8 *sTrainerCannotBattleSpeech = NULL;
static EWRAM_DATA u8 *sTrainerBattleEndScript = NULL;
static EWRAM_DATA u8 *sTrainerABattleScriptRetAddr = NULL;
static EWRAM_DATA u16 sRivalBattleFlags = 0;

static const u8 sBattleTransitionTable_Wild[][2] =
{
    { B_TRANSITION_SLICED_SCREEN,        B_TRANSITION_WHITEFADE_IN_STRIPES },
    { B_TRANSITION_CLOCKWISE_BLACKFADE,  B_TRANSITION_GRID_SQUARES         },
    { B_TRANSITION_BLUR,                 B_TRANSITION_GRID_SQUARES         },
    { B_TRANSITION_BLACK_WAVE_TO_RIGHT,  B_TRANSITION_FULLSCREEN_WAVE      },
};

static const u8 sBattleTransitionTable_Trainer[][2] =
{
    { B_TRANSITION_SLIDING_POKEBALLS,    B_TRANSITION_BLACK_DOODLES        },
    { B_TRANSITION_HORIZONTAL_CORRUGATE, B_TRANSITION_BIG_POKEBALL         },
    { B_TRANSITION_BLUR,                 B_TRANSITION_GRID_SQUARES         },
    { B_TRANSITION_DISTORTED_WAVE,       B_TRANSITION_FULLSCREEN_WAVE      },
};

static const struct TrainerBattleParameter sOrdinaryBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_A,     TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerObjectEventLocalId,   TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerAIntroSpeech,         TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerADefeatSpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,        TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,   TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerABattleScriptRetAddr, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,      TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sContinueScriptBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_A,     TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerObjectEventLocalId,   TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerAIntroSpeech,         TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerADefeatSpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,        TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,   TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerABattleScriptRetAddr, TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerBattleEndScript,      TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sDoubleBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_A,     TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerObjectEventLocalId,   TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerAIntroSpeech,         TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerADefeatSpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,        TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,   TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerABattleScriptRetAddr, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,      TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sOrdinaryNoIntroBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_A,     TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerObjectEventLocalId,   TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerAIntroSpeech,         TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerADefeatSpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,        TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,   TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerABattleScriptRetAddr, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,      TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sEarlyRivalBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_A,     TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sRivalBattleFlags,            TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerAIntroSpeech,         TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerADefeatSpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,   TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerABattleScriptRetAddr, TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerBattleEndScript,      TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};

static const struct TrainerBattleParameter sContinueScriptDoubleBattleParams[] =
{
    {&sTrainerBattleMode,           TRAINER_PARAM_LOAD_VAL_8BIT},
    {&gTrainerBattleOpponent_A,     TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerObjectEventLocalId,   TRAINER_PARAM_LOAD_VAL_16BIT},
    {&sTrainerAIntroSpeech,         TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerADefeatSpeech,        TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerVictorySpeech,        TRAINER_PARAM_CLEAR_VAL_32BIT},
    {&sTrainerCannotBattleSpeech,   TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerABattleScriptRetAddr, TRAINER_PARAM_LOAD_VAL_32BIT},
    {&sTrainerBattleEndScript,      TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR},
};


#define tState data[0]
#define tTransition data[1]

static void Task_BattleStart(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        if (!FldEffPoison_IsActive())
        {
            BT_StartOnField(tTransition);
            ++tState;
        }
        break;
    case 1:
        if (BT_IsDone())
        {
            CleanupOverworldWindowsAndTilemaps();
            SetMainCallback2(CB2_InitBattle);
            ResetEncounterRateModifiers();
            ClearPoisonStepCounter();
            DestroyTask(taskId);
        }
        break;
    }
}

static void CreateBattleStartTask(u8 transition, u16 song) // song == 0 means default music for current map
{
    gTasks[CreateTask(Task_BattleStart, 1)].tTransition = transition;
    PlayMapChosenOrBattleBGM(song);
}

static bool8 CheckSilphScopeInPokemonTower(u16 mapGroup, u16 mapNum)
{
    if (mapGroup == MAP_GROUP(POKEMON_TOWER_1F) && (mapNum == MAP_NUM(POKEMON_TOWER_1F) || mapNum == MAP_NUM(POKEMON_TOWER_2F) || mapNum == MAP_NUM(POKEMON_TOWER_3F)
	|| mapNum == MAP_NUM(POKEMON_TOWER_4F) || mapNum == MAP_NUM(POKEMON_TOWER_5F) || mapNum == MAP_NUM(POKEMON_TOWER_6F) || mapNum == MAP_NUM(POKEMON_TOWER_7F))
    && !(CheckBagHasItem(ITEM_SILPH_SCOPE, 1)))
        return TRUE;
    else
        return FALSE;
}

// Called when a wild encounter appears
void StartWildBattle(void)
{
    if (GetSafariZoneFlag())
        DoSafariBattle();
    else if (CheckSilphScopeInPokemonTower(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum))
        DoGhostBattle();
    else
        DoStandardWildBattle(FALSE);
}

void StartDoubleWildBattle(void)
{
	DoStandardWildBattle(TRUE);
}

static void DoSafariBattle(void)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    StopPlayerAvatar();
    gMain.savedCallback = CB2_EndSafariBattle;
    gBattleTypeFlags = BATTLE_TYPE_SAFARI;
    CreateBattleStartTask(GetWildBattleTransition(), 0);
}

static void DoGhostBattle(void)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    StopPlayerAvatar();
    gMain.savedCallback = CB2_EndWildBattle;
    gBattleTypeFlags = BATTLE_TYPE_GHOST;
    CreateBattleStartTask(GetWildBattleTransition(), 0);
    SetMonData(&gEnemyParty[0], MON_DATA_NICKNAME, gText_Ghost);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

static void DoStandardWildBattle(bool8 isDouble)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    StopPlayerAvatar();
    gMain.savedCallback = CB2_EndWildBattle;
    gBattleTypeFlags = 0;
	
	if (isDouble)
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
#if SOS_WILD_BATTLE_CHANCE != 0
	else if (RandomPercent(SOS_WILD_BATTLE_CHANCE))
		gBattleTypeFlags |= BATTLE_TYPE_SOS;
#endif
	
    CreateBattleStartTask(GetWildBattleTransition(), 0);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void StartRoamerBattle(void)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    StopPlayerAvatar();
    gMain.savedCallback = CB2_EndWildBattle;
    gBattleTypeFlags = BATTLE_TYPE_ROAMER;
    CreateBattleStartTask(GetWildBattleTransition(), MUS_VS_LEGEND);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

static void DoTrainerBattle(void)
{
    CreateBattleStartTask(GetTrainerBattleTransition(), 0);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_TRAINER_BATTLES);
}

void StartOldManTutorialBattle(void)
{
	struct PokemonGenerator generator =
	{
		.species = SPECIES_WEEDLE,
		.level = 5,
		.otIdType = OT_ID_RANDOM,
		.shinyType = GENERATE_SHINY_LOCKED,
		.shinyRollType = SHINY_ROLL_NORMAL,
		.forcedGender = MON_MALE,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.forcedNature = NUM_NATURES,
		.formChanges = NULL,
		.moves = {0},
		.nPerfectIvs = 0,
	};
    CreateMon(&gEnemyParty[0], generator);
    ScriptContext2_Enable();
    gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    gBattleTypeFlags = BATTLE_TYPE_OLD_MAN_TUTORIAL;
    CreateBattleStartTask(B_TRANSITION_SLICED_SCREEN, 0);
}

void StartScriptedWildBattle(void)
{
    ScriptContext2_Enable();
    gMain.savedCallback = CB2_EndScriptedWildBattle;
	
	gBattleTypeFlags = BATTLE_TYPE_WILD_SCRIPTED;
	if (GetMonData(&gEnemyParty[1], MON_DATA_SPECIES)) // if have a second mon turn into double battle
		gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
		
    CreateBattleStartTask(GetWildBattleTransition(), 0);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void StartTotemBattle(s8 *buffs)
{
	u8 i;
	
	ScriptContext2_Enable();
    gMain.savedCallback = CB2_EndScriptedWildBattle;
	gBattleTypeFlags = (BATTLE_TYPE_TOTEM | BATTLE_TYPE_SOS);
	
	for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
	{
		if (buffs[i] != 0)
		{
			gQueuedStatBoosts[B_POSITION_OPPONENT_LEFT].stats |= gBitTable[i];
			gQueuedStatBoosts[B_POSITION_OPPONENT_LEFT].statChanges[i] = buffs[i];
		}
	}
	CreateBattleStartTask(GetWildBattleTransition(), 0);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void StartMarowakBattle(void)
{
    ScriptContext2_Enable();
    gMain.savedCallback = CB2_EndMarowakBattle;
	
    if (CheckBagHasItem(ITEM_SILPH_SCOPE, 1))
    {
		struct PokemonGenerator generator =
		{
			.species = SPECIES_MAROWAK,
			.level = 30,
			.otIdType = OT_ID_PLAYER_ID,
			.shinyType = GENERATE_SHINY_LOCKED,
			.shinyRollType = SHINY_ROLL_NORMAL,
			.forcedGender = MON_FEMALE,
			.hasFixedPersonality = FALSE,
			.fixedPersonality = 0,
			.forcedNature = NATURE_SERIOUS,
			.formChanges = NULL,
			.moves = {0},
			.nPerfectIvs = 0,
		};
        CreateMon(&gEnemyParty[0], generator);
		
		gBattleTypeFlags = (BATTLE_TYPE_GHOST | BATTLE_TYPE_GHOST_UNVEILED);
    }
    else
        gBattleTypeFlags = BATTLE_TYPE_GHOST;
	
	SetMonData(&gEnemyParty[0], MON_DATA_NICKNAME, gText_Ghost);
	
    CreateBattleStartTask(GetWildBattleTransition(), 0);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

void StartLegendaryBattle(void)
{
	u8 transition = B_TRANSITION_BLUR;
    u16 mus;
    
    ScriptContext2_Enable();
    gMain.savedCallback = CB2_EndScriptedWildBattle;
    gBattleTypeFlags = BATTLE_TYPE_WILD_SCRIPTED;
	
    switch (GetMonData(&gEnemyParty[0], MON_DATA_SPECIES))
    {
		case SPECIES_MEWTWO:
			mus = MUS_VS_MEWTWO;
			break;
		case SPECIES_DEOXYS:
			mus = MUS_VS_DEOXYS;
			break;
		case SPECIES_MOLTRES:
		case SPECIES_ARTICUNO:
		case SPECIES_ZAPDOS:
		case SPECIES_HO_OH:
		case SPECIES_LUGIA:
			mus = MUS_VS_LEGEND;
			break;
		case SPECIES_GROUDON:
		    transition = B_TRANSITION_BLACK_DOODLES;
			mus = MUS_RS_VS_TRAINER;
			break;
		default:
            mus = MUS_RS_VS_TRAINER;
			break;
    }
	CreateBattleStartTask(transition, mus);
    IncrementGameStat(GAME_STAT_TOTAL_BATTLES);
    IncrementGameStat(GAME_STAT_WILD_BATTLES);
}

static void CB2_EndWildBattle(void)
{
    CpuFill16(0, (void *)BG_PLTT, BG_PLTT_SIZE);
    ResetOamRange(0, 128);
	
    if (IsPlayerDefeated(gBattleOutcome))
        SetMainCallback2(CB2_WhiteOut);
    else
    {
        SetMainCallback2(CB2_ReturnToField);
        gFieldCallback = FieldCB_SafariZoneRanOutOfBalls;
    }
}

static void CB2_EndScriptedWildBattle(void)
{
    CpuFill16(0, (void *)BG_PLTT, BG_PLTT_SIZE);
    ResetOamRange(0, 128);
	
    if (IsPlayerDefeated(gBattleOutcome))
        SetMainCallback2(CB2_WhiteOut);
    else
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

static void CB2_EndMarowakBattle(void)
{
    CpuFill16(0, (void *)BG_PLTT, BG_PLTT_SIZE);
    ResetOamRange(0, 128);
	
    if (IsPlayerDefeated(gBattleOutcome))
        SetMainCallback2(CB2_WhiteOut);
    else
    {
        // If result is TRUE player didnt defeat Marowak, force player back from stairs
		gSpecialVar_Result = (gBattleOutcome != B_OUTCOME_WON);
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
    }
}

u8 BattleSetup_GetTerrainId(void)
{
    u16 tileBehavior;
    s16 x, y;
	
	if (gIsFishingEncounter)
		return BATTLE_TERRAIN_WATER;

    PlayerGetDestCoords(&x, &y);
    tileBehavior = MapGridGetMetatileBehaviorAt(x, y);
	
    if (MetatileBehavior_IsTallGrass_2(tileBehavior))
        return BATTLE_TERRAIN_GRASS;
    else if (MetatileBehavior_IsLongGrass(tileBehavior))
        return BATTLE_TERRAIN_LONG_GRASS;
    else if (MetatileBehavior_IsSandOrShallowFlowingWater(tileBehavior))
        return BATTLE_TERRAIN_SAND;
	
    switch (gMapHeader.mapType)
    {
		case MAP_TYPE_TOWN:
		case MAP_TYPE_CITY:
		case MAP_TYPE_ROUTE:
			break;
		case MAP_TYPE_UNDERGROUND:
			if (MetatileBehavior_IsIndoorEncounter(tileBehavior))
				return BATTLE_TERRAIN_BUILDING;
			else if (MetatileBehavior_IsSurfable(tileBehavior))
				return BATTLE_TERRAIN_POND;
			
			return BATTLE_TERRAIN_CAVE;
		case MAP_TYPE_INDOOR:
		case MAP_TYPE_SECRET_BASE:
			return BATTLE_TERRAIN_BUILDING;
		case MAP_TYPE_UNDERWATER:
			return BATTLE_TERRAIN_UNDERWATER;
		case MAP_TYPE_OCEAN_ROUTE:
			if (MetatileBehavior_IsSurfable(tileBehavior))
				return BATTLE_TERRAIN_WATER;
			
			return BATTLE_TERRAIN_PLAIN;
    }
	
    if (MetatileBehavior_IsDeepSemiDeepOrSplashingWater(tileBehavior))
        return BATTLE_TERRAIN_WATER;
    else if (MetatileBehavior_IsSurfable(tileBehavior))
        return BATTLE_TERRAIN_POND;
    else if (MetatileBehavior_IsMountain(tileBehavior))
        return BATTLE_TERRAIN_MOUNTAIN;
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
    {
        if (MetatileBehavior_GetBridgeType(tileBehavior))
            return BATTLE_TERRAIN_POND;
        else if (MetatileBehavior_IsBridge(tileBehavior))
            return BATTLE_TERRAIN_WATER;
    }
    return BATTLE_TERRAIN_PLAIN;
}

static u8 GetBattleTransitionTypeByMap(void)
{
    u16 tileBehavior;
    s16 x, y;
	
    if (Overworld_GetFlashLevel())
        return B_TRANSITION_HORIZONTAL_CORRUGATE;
	
	PlayerGetDestCoords(&x, &y);
    tileBehavior = MapGridGetMetatileBehaviorAt(x, y);
	
    if (!MetatileBehavior_IsSurfable(tileBehavior) && !gIsFishingEncounter)
    {
        switch (gMapHeader.mapType)
        {
        case MAP_TYPE_UNDERGROUND:
            return B_TRANSITION_DISTORTED_WAVE;
        case MAP_TYPE_UNDERWATER:
            return B_TRANSITION_BIG_POKEBALL;
        default:
            return B_TRANSITION_BLUR;
        }
    }
    return B_TRANSITION_BIG_POKEBALL;
}

static u32 GetSumOfPlayerPartyLevel(void)
{
	u8 i;
    u32 sum;

    for (i = 0, sum = 0; i < PARTY_SIZE; ++i)
    {
        if (MonCanBattle(&gPlayerParty[i]))
            sum += GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
    }
    return sum;
}

u8 GetTrainerPartyMonLevel(const struct TrainerMon partyIdx)
{
#if DYNAMIC_LEVEL
	return GetPlayerPartyHighestLevel();
#endif
	return partyIdx.lvl;
}

static u32 GetSumOfEnemyPartyLevel(u16 opponentId)
{
	const struct TrainerMon *party = gTrainers[opponentId].party;
    u8 i, numMons = gTrainers[opponentId].partySize;
    u32 sum;
	
	for (i = 0, sum = 0; i < numMons; i++)
		sum += GetTrainerPartyMonLevel(party[i]);

    return sum;
}

static u8 GetWildBattleTransition(void)
{
	return sBattleTransitionTable_Wild[GetBattleTransitionTypeByMap()][(GetMonData(&gEnemyParty[0], MON_DATA_LEVEL) < GetSumOfPlayerPartyLevel()) ? 0 : 1];
}

static u8 GetTrainerBattleTransition(void)
{
    if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_ELITE_FOUR)
    {
        if (gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_LORELEI || gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_LORELEI_2)
            return B_TRANSITION_LORELEI;
        if (gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_BRUNO || gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_BRUNO_2)
            return B_TRANSITION_BRUNO;
        if (gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_AGATHA || gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_AGATHA_2)
            return B_TRANSITION_AGATHA;
        if (gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_LANCE || gTrainerBattleOpponent_A == TRAINER_ELITE_FOUR_LANCE_2)
            return B_TRANSITION_LANCE;
        return B_TRANSITION_BLUE;
    }
    if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
        return B_TRANSITION_BLUE;
	
	return sBattleTransitionTable_Trainer[GetBattleTransitionTypeByMap()][(GetSumOfEnemyPartyLevel(gTrainerBattleOpponent_A) < GetSumOfPlayerPartyLevel()) ? 0 : 1];
}

static u16 GetTrainerAFlag(void)
{
    return TRAINER_FLAGS_START + gTrainerBattleOpponent_A;
}

static bool32 IsPlayerDefeated(u32 battleOutcome)
{
    switch (battleOutcome)
    {
    case B_OUTCOME_LOST:
    case B_OUTCOME_DREW:
        return TRUE;
    default:
        return FALSE;
    }
}

static void InitTrainerBattleVariables(void)
{
    sTrainerBattleMode = 0;
    gTrainerBattleOpponent_A = 0;
    sTrainerObjectEventLocalId = 0;
    sTrainerAIntroSpeech = NULL;
    sTrainerADefeatSpeech = NULL;
    sTrainerVictorySpeech = NULL;
    sTrainerCannotBattleSpeech = NULL;
    sTrainerBattleEndScript = NULL;
    sTrainerABattleScriptRetAddr = NULL;
    sRivalBattleFlags = 0;
}

static inline void SetU8(void *ptr, u8 value)
{
    *(u8 *)(ptr) = value;
}

static inline void SetU16(void *ptr, u16 value)
{
    *(u16 *)(ptr) = value;
}

static inline void SetU32(void *ptr, u32 value)
{
    *(u32 *)(ptr) = value;
}

static inline void SetPtr(const void *ptr, const void *value)
{
    *(const void **)(ptr) = value;
}

static void TrainerBattleLoadArgs(const struct TrainerBattleParameter *specs, const u8 *data)
{
    while (1)
    {
        switch (specs->ptrType)
        {
        case TRAINER_PARAM_LOAD_VAL_8BIT:
            SetU8(specs->varPtr, data[0]);
            data += 1;
            break;
        case TRAINER_PARAM_LOAD_VAL_16BIT:
            SetU16(specs->varPtr, READ_16(data));
            data += 2;
            break;
        case TRAINER_PARAM_LOAD_VAL_32BIT:
            SetU32(specs->varPtr, READ_32(data));
            data += 4;
            break;
        case TRAINER_PARAM_CLEAR_VAL_8BIT:
            SetU8(specs->varPtr, 0);
            break;
        case TRAINER_PARAM_CLEAR_VAL_16BIT:
            SetU16(specs->varPtr, 0);
            break;
        case TRAINER_PARAM_CLEAR_VAL_32BIT:
            SetU32(specs->varPtr, 0);
            break;
        case TRAINER_PARAM_LOAD_SCRIPT_RET_ADDR:
            SetPtr(specs->varPtr, data);
            return;
        }
        ++specs;
    }
}

static void SetMapVarsToTrainer(void)
{
    if (sTrainerObjectEventLocalId != 0)
    {
        gSpecialVar_LastTalked = sTrainerObjectEventLocalId;
        gSelectedObjectEvent = GetObjectEventIdByLocalIdAndMap(sTrainerObjectEventLocalId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    }
}

const u8 *BattleSetup_ConfigureTrainerBattle(const u8 *data)
{
    InitTrainerBattleVariables();
	
    sTrainerBattleMode = data[0];
	
    switch (sTrainerBattleMode)
    {
    case TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT:
        TrainerBattleLoadArgs(sOrdinaryNoIntroBattleParams, data);
        return EventScript_DoNoIntroTrainerBattle;
    case TRAINER_BATTLE_DOUBLE:
        TrainerBattleLoadArgs(sDoubleBattleParams, data);
        SetMapVarsToTrainer();
        return EventScript_TryDoDoubleTrainerBattle;
    case TRAINER_BATTLE_CONTINUE_SCRIPT:
    case TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC:
        TrainerBattleLoadArgs(sContinueScriptBattleParams, data);
        SetMapVarsToTrainer();
        return EventScript_TryDoNormalTrainerBattle;
    case TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE:
    case TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE_NO_MUSIC:
        TrainerBattleLoadArgs(sContinueScriptDoubleBattleParams, data);
        SetMapVarsToTrainer();
        return EventScript_TryDoDoubleTrainerBattle;
    case TRAINER_BATTLE_REMATCH_DOUBLE:
        TrainerBattleLoadArgs(sDoubleBattleParams, data);
        SetMapVarsToTrainer();
        gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
        return EventScript_TryDoDoubleRematchBattle;
    case TRAINER_BATTLE_REMATCH:
        TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
        SetMapVarsToTrainer();
        gTrainerBattleOpponent_A = GetRematchTrainerId(gTrainerBattleOpponent_A);
        return EventScript_TryDoRematchBattle;
    case TRAINER_BATTLE_EARLY_RIVAL:
        TrainerBattleLoadArgs(sEarlyRivalBattleParams, data);
        return EventScript_DoNoIntroTrainerBattle;
    default:
        TrainerBattleLoadArgs(sOrdinaryBattleParams, data);
        SetMapVarsToTrainer();
        return EventScript_TryDoNormalTrainerBattle;
    }
}

void ConfigureAndSetUpOneTrainerBattle(u8 trainerEventObjId, const u8 *trainerScript)
{
    gSelectedObjectEvent = trainerEventObjId;
    gSpecialVar_LastTalked = gObjectEvents[trainerEventObjId].localId;
    BattleSetup_ConfigureTrainerBattle(trainerScript + 1);
    ScriptContext1_SetupScript(EventScript_DoTrainerBattleFromApproach);
    ScriptContext2_Enable();
}

bool32 GetTrainerFlagFromScriptPointer(const u8 *data)
{
    return FlagGet(TRAINER_FLAGS_START + READ_16(data + 2));
}

void SetUpTrainerMovement(void)
{
    struct ObjectEvent *objectEvent = &gObjectEvents[gSelectedObjectEvent];
    SetTrainerMovementType(objectEvent, GetTrainerFacingDirectionMovementType(objectEvent->facingDirection));
}

u8 GetTrainerBattleMode(void)
{
    return sTrainerBattleMode;
}

u16 GetRivalBattleFlags(void)
{
    return sRivalBattleFlags;
}

u16 Script_HasTrainerBeenFought(void)
{
    return FlagGet(GetTrainerAFlag());
}

void SetBattledTrainerFlag(void)
{
    FlagSet(GetTrainerAFlag());
}

bool8 HasTrainerBeenFought(u16 trainerId)
{
    return FlagGet(TRAINER_FLAGS_START + trainerId);
}

void SetTrainerFlag(u16 trainerId)
{
    FlagSet(TRAINER_FLAGS_START + trainerId);
}

void ClearTrainerFlag(u16 trainerId)
{
    FlagClear(TRAINER_FLAGS_START + trainerId);
}

void StartTrainerBattle(void)
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
	
    if (GetTrainerBattleMode() == TRAINER_BATTLE_EARLY_RIVAL && GetRivalBattleFlags() & RIVAL_BATTLE_TUTORIAL)
        gBattleTypeFlags |= BATTLE_TYPE_FIRST_BATTLE;
	
    gMain.savedCallback = CB2_EndTrainerBattle;
    DoTrainerBattle();
    ScriptContext1_Stop();
}

static void CB2_EndTrainerBattle(void)
{
    if (sTrainerBattleMode == TRAINER_BATTLE_EARLY_RIVAL)
    {
        if (IsPlayerDefeated(gBattleOutcome))
        {
            gSpecialVar_Result = TRUE;
			
            if (sRivalBattleFlags & RIVAL_BATTLE_HEAL_AFTER)
                HealPlayerParty();
            else
            {
                SetMainCallback2(CB2_WhiteOut);
                return;
            }
        }
        else
            gSpecialVar_Result = FALSE;
    }
    else
    {
        if (IsPlayerDefeated(gBattleOutcome))
		{
            SetMainCallback2(CB2_WhiteOut);
			return;
        }
    }
	SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
	SetBattledTrainerFlag();
}

static void CB2_EndRematchBattle(void)
{
    if (IsPlayerDefeated(gBattleOutcome))
        SetMainCallback2(CB2_WhiteOut);
    else
    {
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        SetBattledTrainerFlag();
        ClearRematchStateOfLastTalked();
    }
}

void StartRematchBattle(void)
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    gMain.savedCallback = CB2_EndRematchBattle;
    DoTrainerBattle();
    ScriptContext1_Stop();
}

static const u8 *ReturnEmptyStringIfNull(const u8 *string)
{
	return string == NULL ? gString_Dummy : string;
}

void ShowTrainerIntroSpeech(void)
{
    ShowFieldMessage(ReturnEmptyStringIfNull(sTrainerAIntroSpeech));
}

const u8 *BattleSetup_GetScriptAddrAfterBattle(void)
{
	return sTrainerBattleEndScript != NULL ? sTrainerBattleEndScript : EventScript_TestSignpostMsg;
}

const u8 *BattleSetup_GetTrainerPostBattleScript(void)
{
	return sTrainerABattleScriptRetAddr != NULL ? sTrainerABattleScriptRetAddr : EventScript_TestSignpostMsg;
}

void ShowTrainerCantBattleSpeech(void)
{
    ShowFieldMessage(ReturnEmptyStringIfNull(sTrainerCannotBattleSpeech));
}

void PlayTrainerEncounterMusic(void)
{
    if (sTrainerBattleMode != TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC && sTrainerBattleMode != TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE_NO_MUSIC)
		PlayNewMapMusic(gTrainerEncounterMusicIdsTable[gTrainers[gTrainerBattleOpponent_A].encounterMusic]);
}

const u8 *GetTrainerALoseText(void)
{
    StringExpandPlaceholders(gStringVar4, ReturnEmptyStringIfNull(sTrainerADefeatSpeech));
    return gStringVar4;
}

const u8 *GetTrainerWonSpeech(void)
{
    StringExpandPlaceholders(gStringVar4, ReturnEmptyStringIfNull(sTrainerVictorySpeech));
    return gStringVar4;
}
