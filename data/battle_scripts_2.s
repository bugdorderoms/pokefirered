#include "constants/moves.h"
#include "constants/battle.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_anim.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/game_stat.h"
#include "constants/battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
	.section script_data, "aw", %progbits
	.align 2

gBattlescriptsForUsingItem::
	.4byte BattleScript_ItemRestoreHP                @ EFFECT_ITEM_RESTORE_HP
	.4byte BattleScript_ItemCurePrimaryStatus        @ EFFECT_ITEM_CURE_PRIMARY_STATUS
	.4byte BattleScript_ItemCureSecondaryStatus      @ EFFECT_ITEM_CURE_SECONDARY_STATUS
	.4byte BattleScript_ItemIncreaseStat             @ EFFECT_ITEM_INCREASE_STAT
	.4byte BattleScript_ItemSetMist                  @ EFFECT_ITEM_SET_MIST
	.4byte BattleScript_ItemSetFocusEnergy           @ EFFECT_ITEM_SET_FOCUS_ENERGY
	.4byte BattleScript_RunByUsingItem               @ EFFECT_ITEM_ESCAPE
	.4byte BattleScript_ThrowBall                    @ EFFECT_ITEM_THROW_BALL
	.4byte BattleScript_ItemRestorePP                @ EFFECT_ITEM_RESTORE_PP
	.4byte BattleScript_ItemIncreaseAllStats         @ EFFECT_ITEM_INCREASE_ALL_STATS
	.4byte BattleScript_UsePokeFlute                 @ EFFECT_ITEM_POKE_FLUTE
	.4byte BattleScript_ItemRestoreHP                @ EFFECT_ITEM_REVIVE
	.4byte BattleScript_ItemDoubleSosCallRate        @ EFFECT_ITEM_ADRENALINE_ORB

gRaidCheerEffectsTable::
	.4byte BattleScript_RaidCheerHealHP              @ RAID_CHEER_HEAL_HP
	.4byte BattleScript_RaidCheerCureStatus          @ RAID_CHEER_CURE_STATUS
	.4byte BattleScript_RaidCheerIncreaseStat        @ RAID_CHEER_BOOST_STAT
	.4byte BattleScript_RaidCheerSetReflect          @ RAID_CHEER_SET_REFLECT
	.4byte BattleScript_RaidCheerSetLightscreen      @ RAID_CHEER_SET_LIGHTSCREEN
	.4byte BattleScript_RaidCheerBreakShield         @ RAID_CHEER_BREAK_SHIELD
	.4byte BattleScript_RaidCheerNothing             @ RAID_CHEER_NOTHING

@@@@@@@@@@@@@@@@@@@@
@ ITEM USE SCRIPTS @
@@@@@@@@@@@@@@@@@@@@

BattleScript_ItemUseMessageEnd::
	call BattleScript_UseItemMessage
BattleScript_ItemEnd::
	end

UsedItemString::
	printstring STRINGID_EMPTYSTRING
	pause B_WAIT_TIME_MED
	printstring STRINGID_TRAINERUSEDITEM
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_UseItemMessage::
	call UsedItemString
	jumpifabsent BS_SCRIPTING, BattleScript_UseItemMessageReturn
	playanimation BS_SCRIPTING, B_ANIM_ITEM_THROW
	waitstate
BattleScript_UseItemMessageReturn::
	playse SE_USE_ITEM
	return

@ EFFECT_ITEM_RESTORE_HP @

BattleScript_ItemRestoreHP::
	call BattleScript_UseItemMessage
	jumpifrevived BattleScript_ItemRestoreHP_SendOutRevivedBattler @ used revive
	bichalfword gMoveResultFlags, MOVE_RESULT_NO_EFFECT
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_SCRIPTING
	datahpupdate BS_SCRIPTING
	updatestatusicon BS_SCRIPTING
	printstring STRINGID_ITEMPKMNHADHPRESTORED
	waitmessage B_WAIT_TIME_LONG
	end

BattleScript_ItemRestoreHP_SendOutRevivedBattler::
	switchinanim BS_SCRIPTING, 0
	waitstate
	switchineffects BS_SCRIPTING
	end

@ EFFECT_ITEM_CURE_PRIMARY_STATUS @

BattleScript_ItemCurePrimaryStatus::
	call BattleScript_UseItemMessage
	cureprimarystatus BS_SCRIPTING, BattleScript_ItemCureSecondaryStatus_PrintString
	updatestatusicon BS_SCRIPTING
	goto BattleScript_ItemCureSecondaryStatus_PrintString

@ EFFECT_ITEM_CURE_SECONDARY_STATUS @

BattleScript_ItemCureSecondaryStatus::
	call BattleScript_UseItemMessage
BattleScript_ItemCureSecondaryStatus_PrintString::
	printstring STRINGID_ITEMPKMNHADSTATUSHEALED
	waitmessage B_WAIT_TIME_LONG
	end

@ EFFECT_ITEM_INCREASE_STAT @

BattleScript_ItemIncreaseStat::
	call BattleScript_UseItemMessage
	itemincreasestat
	statbuffchange STAT_CHANGE_FLAG_SELF_INFLICT
	statchangeanimandstring
	end

@ EFFECT_ITEM_SET_MIST @

BattleScript_ItemSetMist::
	call BattleScript_UseItemMessage
	setmist
	playmoveanimation BS_ATTACKER, MOVE_MIST
	waitstate
	printstring STRINGID_ATKTEAMSHROUDEDINMIST
	waitmessage B_WAIT_TIME_LONG
	end

@ EFFECT_ITEM_SET_FOCUS_ENERGY @

BattleScript_ItemSetFocusEnergy::
	call BattleScript_UseItemMessage
	setfocusenergy
	playmoveanimation BS_ATTACKER, MOVE_FOCUS_ENERGY
	waitstate
	printstring STRINGID_ATKGETTINGPUMPED
	waitmessage B_WAIT_TIME_LONG
	end

@ EFFECT_ITEM_ESCAPE @

BattleScript_RunByUsingItem::
	playse SE_FLEE
	setbyte gBattleOutcome, B_OUTCOME_RAN
	finishturn

@ EFFECT_ITEM_THROW_BALL @

BattleScript_ThrowBall::
	printstring STRINGID_TRAINERUSEDITEM
	jumpifnotbattletype BATTLE_TYPE_SAFARI, BattleScript_DoBallThrow
	updatestatusicon BS_ATTACKER @ Update num of balls
BattleScript_DoBallThrow::
	handleballthrow

@ EFFECT_ITEM_RESTORE_PP @

BattleScript_ItemRestorePP::
	call BattleScript_UseItemMessage
	printstring STRINGID_ITEMPKMNHADPPRESTORED
	waitmessage B_WAIT_TIME_LONG
	end

@ EFFECT_ITEM_INCREASE_ALL_STATS @

BattleScript_ItemIncreaseAllStats::
	call BattleScript_UseItemMessage
	call BattleScript_AllStatsUp
	end

@ EFFECT_ITEM_POKE_FLUTE @

BattleScript_UsePokeFlute::
	checkpokeflute BS_ATTACKER
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, TRUE, BattleScript_PokeFluteWakeUp
	printstring STRINGID_ITEMCATCHYTUNE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_PokeFluteEnd

BattleScript_PokeFluteWakeUp::
	printstring STRINGID_ITEMPLAYED
	waitmessage B_WAIT_TIME_LONG
	fanfare MUS_POKE_FLUTE
	waitfanfare BS_ATTACKER
	printstring STRINGID_MONHEARINGFLUTEAWOKE
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_PLAYER2
	waitstate
BattleScript_PokeFluteEnd::
	finishaction

BattleScript_ItemDoubleSosCallRate::
	call UsedItemString
	trysetadrenalineorbeffect
	printfromtable gAdrenalineOrbUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	end

@@@@@@@@@@@@@@@@@@@@@@
@ BALL THROW SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@

BattleScript_DoRaidBattleBallThrow::
	flushmessagebox
	pause B_WAIT_TIME_MED
	goto BattleScript_DoBallThrow

BattleScript_SuccessBallThrow::
	domoncaughteffects
	printstring STRINGID_GOTCHADEFCAUGHT
	jumpifbyte CMP_EQUAL, sEXP_CATCH, FALSE, BattleScript_CaughtMonTrySetDexFlag
	setbyte sGIVEEXP_STATE, 0
	getexp BS_TARGET
	sethword gBattle_BG2_X, 0
BattleScript_CaughtMonTrySetDexFlag::
	jumpifbattletype BATTLE_TYPE_RAID, BattleScript_CaughtPokemonDone
	jumpifbattletype BATTLE_TYPE_RECORDED, BattleScript_CaughtPokemonSkipNickname
	trysetcaughtmondexflags BattleScript_CaughtPokemonSkipNewDex
	printstring STRINGID_DEFDATAADDEDTODEX
	waitstate
	setbyte sMULTIUSE_STATE, 0
	displaydexinfo
BattleScript_CaughtPokemonSkipNewDex::
	jumpifcantgivenickname BattleScript_TryGiveCaughtNickPokemon @ Skip the string
	printstring STRINGID_GIVENICKNAMECAPTURED
	waitstate
	setbyte sMULTIUSE_STATE, 0
BattleScript_TryGiveCaughtNickPokemon::
	trygivecaughtmonnick BattleScript_CaughtPokemonSkipNickname
	givecaughtmon
	printfromtable gCaughtMonStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_CaughtPokemonDone

BattleScript_GiveNicknameYesNoBox::
	yesnobox BS_ATTACKER
	waitstate
	return

BattleScript_CaughtPokemonSkipNickname::
	givecaughtmon
BattleScript_CaughtPokemonDone::
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	finishturn

BattleScript_OldMan_Pokedude_CaughtMessage::
	printstring STRINGID_GOTCHADEFCAUGHT
	setbyte gBattleOutcome, B_OUTCOME_CAUGHT
	finishturn

BattleScript_ShakeBallThrow::
	printfromtable gBallEscapeStringIds
	waitmessage B_WAIT_TIME_LONG
	jumpifbattletype BATTLE_TYPE_RAID, BattleScript_FaintRaidBoss
	jumpifnotbattletype BATTLE_TYPE_SAFARI, BattleScript_CatchFailEnd
	jumpifbyte CMP_NOT_EQUAL, gNumSafariBalls, 0, BattleScript_CatchFailEnd
	printstring STRINGID_OUTOFSAFARIBALLS
	waitmessage B_WAIT_TIME_LONG
	setbyte gBattleOutcome, B_OUTCOME_NO_SAFARI_BALLS
BattleScript_CatchFailEnd::
	finishaction

BattleScript_TrainerBallBlock::
	waitmessage B_WAIT_TIME_LONG
	printstring STRINGID_TRAINERBLOCKEDBALL
	waitmessage B_WAIT_TIME_LONG
	printstring STRINGID_DONTBEATHIEF
	waitmessage B_WAIT_TIME_LONG
	finishaction

BattleScript_GhostBallDodge::
	waitmessage B_WAIT_TIME_LONG
	printstring STRINGID_ITDODGEDBALL
	waitmessage B_WAIT_TIME_LONG
	finishaction

@@@@@@@@@@@@@@@@@@@@@@@@@
@ SAFARI ACTION SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_WatchesCarefully::
	printfromtable gSafariPokeblockResultStringIds
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_OPPONENT1, B_ANIM_SAFARI_REACTION
	end2

BattleScript_ThrowRock::
	printstring STRINGID_THREWROCK
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_ATTACKER, B_ANIM_ROCK_THROW
	end2

BattleScript_ThrowBait::
	printstring STRINGID_THREWBAIT
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_ATTACKER, B_ANIM_BAIT_THROW
	end2

BattleScript_LeftoverWallyPrepToThrow::
	printstring STRINGID_RETURNMON
	waitmessage B_WAIT_TIME_LONG
	returnatktoball
	waitstate
	handletrainerslidecase B_POSITION_PLAYER_LEFT, ATK6B_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	printstring STRINGID_YOUTHROWABALLNOWRIGHT
	waitmessage B_WAIT_TIME_LONG
	end2

@@@@@@@@@@@@@@@@@@@@@@
@ RAID CHEER SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@

BattleScript_RaidCheerMessage::
	flushmessagebox
	playse SE_APPLAUSE
	printstring STRINGID_TRAINERCHEEREDEVERYONEON
	waitmessage B_WAIT_TIME_LONG
	return

@ RAID_CHEER_HEAL_HP @

BattleScript_RaidCheerHealHP::
	setbyte sMULTIUSE_STATE, FALSE
	setbyte gBattlerTarget, 0
BattleScript_RaidCheerHealHP_Loop::
	jumpifabsent BS_TARGET, BattleScript_RaidCheerHealHP_NextBattler
	jumpiftargetally BattleScript_RaidCheerHealHP_TryHeal
	goto BattleScript_RaidCheerHealHP_NextBattler
BattleScript_RaidCheerHealHP_TryHeal::
	raidcheerhealfullhealth BS_TARGET, BattleScript_RaidCheerHealHP_NextBattler
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	setbyte sMULTIUSE_STATE, TRUE
BattleScript_RaidCheerHealHP_NextBattler::
	addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_RaidCheerHealHP_Loop
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, FALSE, BattleScript_RaidCheerNothing
	printstring STRINGID_EVERYONEHPWASRESTORED
	waitmessage B_WAIT_TIME_LONG
	end

@ RAID_CHEER_CURE_STATUS @

BattleScript_RaidCheerCureStatus::
	setbyte sMULTIUSE_STATE, FALSE
	setbyte gBattlerTarget, 0
BattleScript_RaidCheerCureStatus_Loop::
	jumpifabsent BS_TARGET, BattleScript_RaidCheerCureStatus_NextBattler
	jumpiftargetally BattleScript_RaidCheerCureStatus_TryStatusHeal
	goto BattleScript_RaidCheerCureStatus_NextBattler
BattleScript_RaidCheerCureStatus_TryStatusHeal::
	cureprimarystatus BS_TARGET, BattleScript_RaidCheerCureStatus_NextBattler
	updatestatusicon BS_TARGET
	setbyte sMULTIUSE_STATE, TRUE
BattleScript_RaidCheerCureStatus_NextBattler::
	addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_RaidCheerCureStatus_Loop
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, FALSE, BattleScript_RaidCheerNothing
	printstring STRINGID_EVERYONESTATUSWEREHEALED
	waitmessage B_WAIT_TIME_LONG
	end

@ RAID_CHEER_BOOST_STAT @

BattleScript_RaidCheerIncreaseStat::
	raidcheerselectstattoincrease BattleScript_RaidCheerIncreaseStat_BoostCritModifier, BattleScript_RaidCheerIncreaseStat_ByOne, BattleScript_RaidCheerNothing
	setstatchangerfrommultiusestate +2
	statbuffchange STAT_CHANGE_FLAG_SELF_INFLICT
	playstatchangeanimation
	waitstate
	settargetally BS_ATTACKER
	swapattackerwithtarget
	setstatchangerfrommultiusestate +2
	statbuffchange STAT_CHANGE_FLAG_SELF_INFLICT
	playstatchangeanimation
	waitstate
	displaystatchangestring STRINGID_BUFF1ROSEFOREVERYONE
	swapattackerwithtarget
	end

BattleScript_RaidCheerIncreaseStat_ByOne::
	setstatchangerfrommultiusestate +1
	statbuffchange STAT_CHANGE_FLAG_SELF_INFLICT
	playstatchangeanimation
	waitstate
	settargetally BS_ATTACKER
	swapattackerwithtarget
	setstatchangerfrommultiusestate +1
	statbuffchange STAT_CHANGE_FLAG_SELF_INFLICT
	playstatchangeanimation
	waitstate
	displaystatchangestring STRINGID_BUFF1ROSEFOREVERYONE
	swapattackerwithtarget
	end

BattleScript_RaidCheerIncreaseStat_BoostCritModifier::
	printstring STRINGID_EVERYONEISGETTINGPUMPED
	waitmessage B_WAIT_TIME_LONG
	end

@ RAID_CHEER_SET_REFLECT @

BattleScript_RaidCheerSetReflect::
	trysetreflect BattleScript_RaidCheerNothing
	playmoveanimation BS_ATTACKER, MOVE_REFLECT
BattleScript_RaidCheerSetReflectLightscreen_Message::
	printfromtable gReflectLightScreenSafeguardStringIds
	waitmessage B_WAIT_TIME_LONG
	end

@ RAID_CHEER_SET_LIGHTSCREEN @

BattleScript_RaidCheerSetLightscreen::
	trysetlightscreen BattleScript_RaidCheerNothing
	playmoveanimation BS_ATTACKER, MOVE_LIGHT_SCREEN
	goto BattleScript_RaidCheerSetReflectLightscreen_Message

@ RAID_CHEER_BREAK_SHIELD @

BattleScript_RaidCheerBreakShield::
	raidcheerdestroyshield BattleScript_RaidCheerNothing
	printstring STRINGID_POKEMONMYSTERIOUSBARRIERFELL
	waitmessage B_WAIT_TIME_LONG
	call BattleScript_RaidShieldBreak
	end

@ RAID_CHEER_NOTHING @

BattleScript_RaidCheerNothing::
	printstring STRINGID_BUTITECHOEDFEEBLY
	waitmessage B_WAIT_TIME_LONG
	end
