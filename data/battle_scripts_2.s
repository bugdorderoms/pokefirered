#include "constants/moves.h"
#include "constants/battle.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_anim.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/game_stat.h"
#include "battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
@ Define these here since misc_constants.inc conflicts with the C headers
	.set NULL, 0
	.set FALSE, 0
	.set TRUE, 1
	.section script_data, "aw", %progbits
	.align 2

gBattlescriptsForUsingItem::
	.4byte BattleScript_ItemRestoreHP                @ EFFECT_ITEM_RESTORE_HP
    .4byte BattleScript_ItemCureStatus               @ EFFECT_ITEM_CURE_STATUS
    .4byte BattleScript_ItemIncreaseStat             @ EFFECT_ITEM_INCREASE_STAT
    .4byte BattleScript_ItemSetMist                  @ EFFECT_ITEM_SET_MIST
    .4byte BattleScript_ItemSetFocusEnergy           @ EFFECT_ITEM_SET_FOCUS_ENERGY
    .4byte BattleScript_RunByUsingItem               @ EFFECT_ITEM_ESCAPE
    .4byte BattleScript_ThrowBall                    @ EFFECT_ITEM_THROW_BALL
    .4byte BattleScript_ItemRestorePP                @ EFFECT_ITEM_RESTORE_PP
    .4byte BattleScript_ItemIncreaseAllStats         @ EFFECT_ITEM_INCREASE_ALL_STATS
	.4byte BattleScript_UsePokeFlute                 @ EFFECT_ITEM_POKE_FLUTE

gBattlescriptsForSafariActions::
	.4byte BattleScript_WatchesCarefully
	.4byte BattleScript_ThrowRock
	.4byte BattleScript_ThrowBait
	.4byte BattleScript_LeftoverWallyPrepToThrow

BattleScript_ItemUseMessageEnd::
    call BattleScript_UseItemMessage

BattleScript_ItemEnd::
    end

BattleScript_UseItemMessage::
    printstring STRINGID_EMPTYSTRING3
    pause 0x30
	printstring STRINGID_TRAINERUSEDITEM
    waitmessage 0x40
	jumpifabsent BS_SCRIPTING, BattleScript_UseItemMessageReturn
	playanimation BS_SCRIPTING, B_ANIM_ITEM_THROW, NULL
	waitstate
BattleScript_UseItemMessageReturn::
    playse SE_USE_ITEM
    return

BattleScript_ItemRestoreHP::
    call BattleScript_UseItemMessage
	jumpifrevived BattleScript_ItemRestoreHP_SendOutRevivedBattler @ used revive
    bichalfword gMoveResultFlags, MOVE_RESULT_NO_EFFECT
    orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
    healthbarupdate BS_SCRIPTING
    datahpupdate BS_SCRIPTING
    updatestatusicon BS_SCRIPTING
	printstring STRINGID_ITEMRESTOREDSPECIESHEALTH
    waitmessage 0x40
    end

BattleScript_ItemRestoreHP_SendOutRevivedBattler::
    switchinanim BS_SCRIPTING, FALSE
    waitstate
    switchineffects BS_SCRIPTING
    end

BattleScript_ItemCureStatus::
    call BattleScript_UseItemMessage
	updatestatusicon BS_SCRIPTING
    printstring STRINGID_ITEMCUREDSPECIESSTATUS
    waitmessage 0x40
    end
	
BattleScript_ItemIncreaseStat::
    call BattleScript_UseItemMessage
	itemincreasestat
    statbuffchange MOVE_EFFECT_AFFECTS_USER | STAT_CHANGE_NOT_PROTECT_AFFECTED | STAT_CHANGE_BS_PTR, BattleScript_ItemEnd
    setgraphicalstatchangevalues
    playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
    printfromtable gStatUpStringIds
    waitmessage 0x40
    end

BattleScript_ItemSetMist::
    call BattleScript_UseItemMessage
    setmist
    playmoveanimation BS_ATTACKER, MOVE_MIST
	waitstate
    printstring STRINGID_PKMNSHROUDEDINMIST
    waitmessage 0x40
    end

BattleScript_ItemSetFocusEnergy::
    call BattleScript_UseItemMessage
    setfocusenergy
    playmoveanimation BS_ATTACKER, MOVE_FOCUS_ENERGY
	waitstate
    printstring STRINGID_PKMNGETTINGPUMPED
    waitmessage 0x40
    end

BattleScript_ItemRestorePP::
    call BattleScript_UseItemMessage
    printstring STRINGID_ITEMRESTOREDSPECIESPP
    waitmessage 0x40
    end

BattleScript_ItemIncreaseAllStats::
    call BattleScript_UseItemMessage
    call BattleScript_AllStatsUp
    end

BattleScript_ThrowBall::
	jumpifbattletype BATTLE_TYPE_OLD_MAN_TUTORIAL, BattleScript_OldManThrowBall
	jumpifbattletype BATTLE_TYPE_POKEDUDE, BattleScript_PokedudeThrowBall
	printstring STRINGID_PLAYERUSEDITEM
	handleballthrow

BattleScript_OldManThrowBall::
	printstring STRINGID_OLDMANUSEDITEM
	handleballthrow

BattleScript_PokedudeThrowBall::
	printstring STRINGID_POKEDUDEUSED
	handleballthrow

BattleScript_ThrowSafariBall::
	printstring STRINGID_PLAYERUSEDITEM
	updatestatusicon BS_ATTACKER
	handleballthrow

BattleScript_SuccessBallThrow::
	jumpifhalfword CMP_EQUAL, gLastUsedItem, ITEM_SAFARI_BALL, BattleScript_SafariNoIncGameStat
	incrementgamestat GAME_STAT_POKEMON_CAPTURES
BattleScript_SafariNoIncGameStat::
	printstring STRINGID_GOTCHAPKMNCAUGHT
	trysetcaughtmondexflags BattleScript_CaughtPokemonSkipNewDex
	printstring STRINGID_PKMNDATAADDEDTODEX
	waitstate
	setbyte sMULTIUSE_STATE, 0
	displaydexinfo
BattleScript_CaughtPokemonSkipNewDex::
	printstring STRINGID_GIVENICKNAMECAPTURED
	waitstate
	setbyte sMULTIUSE_STATE, 0
	trygivecaughtmonnick BattleScript_CaughtPokemonSkipNickname
	givecaughtmon
	printfromtable gCaughtMonStringIds
	waitmessage 64
	goto BattleScript_CaughtPokemonDone

BattleScript_CaughtPokemonSkipNickname::
	givecaughtmon
BattleScript_CaughtPokemonDone::
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	finishturn

BattleScript_OldMan_Pokedude_CaughtMessage::
	printstring STRINGID_GOTCHAPKMNCAUGHT2
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	endlinkbattle
	finishturn

BattleScript_ShakeBallThrow::
	printfromtable gBallEscapeStringIds
	waitmessage 64
	jumpifnotbattletype BATTLE_TYPE_SAFARI, BattleScript_CatchFailEnd
	jumpifbyte CMP_NOT_EQUAL, gNumSafariBalls, 0, BattleScript_CatchFailEnd
	printstring STRINGID_OUTOFSAFARIBALLS
	waitmessage 64
	setbyte gBattleOutcome, B_OUTCOME_NO_SAFARI_BALLS
BattleScript_CatchFailEnd::
	finishaction

BattleScript_TrainerBallBlock::
	waitmessage 64
	printstring STRINGID_TRAINERBLOCKEDBALL
	waitmessage 64
	printstring STRINGID_DONTBEATHIEF
	waitmessage 64
	finishaction

BattleScript_GhostBallDodge::
	waitmessage 64
	printstring STRINGID_ITDODGEDBALL
	waitmessage 64
	finishaction

BattleScript_RunByUsingItem::
	playse SE_FLEE
	setbyte gBattleOutcome, B_OUTCOME_RAN
	finishturn

BattleScript_UsePokeFlute::
	checkpokeflute BS_ATTACKER
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 1, BattleScript_PokeFluteWakeUp
	printstring STRINGID_POKEFLUTECATCHY
	waitmessage 0x40
	goto BattleScript_PokeFluteEnd

BattleScript_PokeFluteWakeUp::
	printstring STRINGID_POKEFLUTE
	waitmessage 0x40
	fanfare MUS_POKE_FLUTE
	waitfanfare BS_ATTACKER
	printstring STRINGID_MONHEARINGFLUTEAWOKE
	waitmessage 0x40
	updatestatusicon BS_PLAYER2
	waitstate
BattleScript_PokeFluteEnd::
	finishaction

BattleScript_WatchesCarefully::
	printfromtable gSafariPokeblockResultStringIds
	waitmessage 64
	playanimation BS_OPPONENT1, B_ANIM_SAFARI_REACTION, NULL
	end2

BattleScript_ThrowRock::
	printstring STRINGID_THREWROCK
	waitmessage 64
	playanimation BS_ATTACKER, B_ANIM_ROCK_THROW, NULL
	end2

BattleScript_ThrowBait::
	printstring STRINGID_THREWBAIT
	waitmessage 64
	playanimation BS_ATTACKER, B_ANIM_BAIT_THROW, NULL
	end2

BattleScript_LeftoverWallyPrepToThrow::
	printstring STRINGID_RETURNMON
	waitmessage 64
	returnatktoball
	waitstate
	handletrainerslidecase B_POSITION_PLAYER_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	printstring STRINGID_YOUTHROWABALLNOWRIGHT
	waitmessage 64
	end2
