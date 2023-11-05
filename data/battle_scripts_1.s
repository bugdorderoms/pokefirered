#include "constants/moves.h"
#include "constants/battle.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_anim.h"
#include "constants/items.h"
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "constants/game_stat.h"
#include "battle_string_ids.h"
	.include "asm/macros/battle_script.inc"
	.section script_data, "aw", %progbits
	.align 2

gBattleScriptsForMoveEffects::
	.4byte BattleScript_EffectHit                   @ EFFECT_HIT
	.4byte BattleScript_EffectHit                   @ EFFECT_MULTI_HIT
	.4byte BattleScript_EffectPayDay                @ EFFECT_PAY_DAY
	.4byte BattleScript_EffectBurnHit               @ EFFECT_BURN_HIT
	.4byte BattleScript_EffectFreezeHit             @ EFFECT_FREEZE_HIT
	.4byte BattleScript_EffectParalyzeHit           @ EFFECT_PARALYZE_HIT
	.4byte BattleScript_EffectOHKO                  @ EFFECT_OHKO
	.4byte BattleScript_EffectTwoTurnMoves          @ EFFECT_TWO_TURNS_ATTACK
	.4byte BattleScript_EffectAttackUp2             @ EFFECT_ATTACK_UP_2
	.4byte BattleScript_EffectRoar                  @ EFFECT_ROAR
	.4byte BattleScript_EffectSemiInvulnerable      @ EFFECT_SEMI_INVULNERABLE
	.4byte BattleScript_EffectTrap                  @ EFFECT_TRAP
	.4byte BattleScript_EffectFlinchHit             @ EFFECT_FLINCH_HIT
	.4byte BattleScript_EffectRecoilIfMiss          @ EFFECT_RECOIL_IF_MISS
	.4byte BattleScript_EffectAccuracyDown          @ EFFECT_ACCURACY_DOWN
	.4byte BattleScript_EffectHit                   @ EFFECT_RECOIL_25
	.4byte BattleScript_EffectRampage               @ EFFECT_RAMPAGE
	.4byte BattleScript_EffectHit                   @ EFFECT_DOUBLE_EDGE
	.4byte BattleScript_EffectDefenseDown           @ EFFECT_DEFENSE_DOWN
	.4byte BattleScript_EffectPoisonHit             @ EFFECT_POISON_HIT
	.4byte BattleScript_EffectAttackDown            @ EFFECT_ATTACK_DOWN
	.4byte BattleScript_EffectSleep                 @ EFFECT_SLEEP
	.4byte BattleScript_EffectConfuse               @ EFFECT_CONFUSE
	.4byte BattleScript_EffectFixedDamage           @ EFFECT_FIXED_DAMAGE
	.4byte BattleScript_EffectDisable               @ EFFECT_DISABLE
	.4byte BattleScript_EffectSpecialDefenseDownHit @ EFFECT_SPECIAL_DEFENSE_DOWN_HIT
	.4byte BattleScript_EffectMist                  @ EFFECT_MIST
	.4byte BattleScript_EffectConfuseHit            @ EFFECT_CONFUSE_HIT
	.4byte BattleScript_EffectSpeedDownHit          @ EFFECT_SPEED_DOWN_HIT
	.4byte BattleScript_EffectAttackDownHit         @ EFFECT_ATTACK_DOWN_HIT
	.4byte BattleScript_EffectRecharge              @ EFFECT_RECHARGE
	.4byte BattleScript_EffectHit                   @ EFFECT_LOW_KICK
	.4byte BattleScript_EffectCounter               @ EFFECT_COUNTER
	.4byte BattleScript_EffectLevelDamage           @ EFFECT_LEVEL_DAMAGE
	.4byte BattleScript_EffectAbsorb                @ EFFECT_ABSORB
	.4byte BattleScript_EffectLeechSeed             @ EFFECT_LEECH_SEED
	.4byte BattleScript_EffectHit                   @ EFFECT_GROWTH
	.4byte BattleScript_EffectSolarbeam             @ EFFECT_SOLARBEAM
	.4byte BattleScript_EffectPoison                @ EFFECT_POISON
	.4byte BattleScript_EffectParalyze              @ EFFECT_PARALYZE
	.4byte BattleScript_EffectSpeedDown2            @ EFFECT_SPEED_DOWN_2
	.4byte BattleScript_EffectToxic                 @ EFFECT_TOXIC
	.4byte BattleScript_EffectAttackUp              @ EFFECT_ATTACK_UP
	.4byte BattleScript_EffectSpeedUp2              @ EFFECT_SPEED_UP_2
	.4byte BattleScript_EffectRage                  @ EFFECT_RAGE
	.4byte BattleScript_EffectTeleport              @ EFFECT_TELEPORT
	.4byte BattleScript_EffectMimic                 @ EFFECT_MIMIC
	.4byte BattleScript_EffectDefenseDown2          @ EFFECT_DEFENSE_DOWN_2
	.4byte BattleScript_EffectEvasionUp             @ EFFECT_EVASION_UP
	.4byte BattleScript_EffectRestoreHp             @ EFFECT_RESTORE_HP
	.4byte BattleScript_EffectDefenseUp             @ EFFECT_DEFENSE_UP
	.4byte BattleScript_EffectMinimize              @ EFFECT_MINIMIZE
	.4byte BattleScript_EffectDefenseCurl           @ EFFECT_DEFENSE_CURL
	.4byte BattleScript_EffectDefenseUp2            @ EFFECT_DEFENSE_UP_2
	.4byte BattleScript_EffectLightScreen           @ EFFECT_LIGHT_SCREEN
	.4byte BattleScript_EffectHaze                  @ EFFECT_HAZE
	.4byte BattleScript_EffectReflect               @ EFFECT_REFLECT
	.4byte BattleScript_EffectFocusEnergy           @ EFFECT_FOCUS_ENERGY
	.4byte BattleScript_EffectBide                  @ EFFECT_BIDE
	.4byte BattleScript_EffectMetronome             @ EFFECT_METRONOME
	.4byte BattleScript_EffectMirrorMove            @ EFFECT_MIRROR_MOVE
	.4byte BattleScript_EffectExplosion             @ EFFECT_EXPLOSION
	.4byte BattleScript_EffectSkullBash             @ EFFECT_SKULL_BASH
	.4byte BattleScript_EffectSpecialDefenseUp2     @ EFFECT_SPECIAL_DEFENSE_UP_2
	.4byte BattleScript_EffectDreamEater            @ EFFECT_DREAM_EATER
	.4byte BattleScript_EffectTransform             @ EFFECT_TRANSFORM
	.4byte BattleScript_EffectPsywave               @ EFFECT_PSYWAVE
	.4byte BattleScript_EffectDoNothing             @ EFFECT_DO_NOTHING
	.4byte BattleScript_EffectRest                  @ EFFECT_REST
	.4byte BattleScript_EffectConversion            @ EFFECT_CONVERSION
	.4byte BattleScript_EffectTriAttack             @ EFFECT_TRI_ATTACK
	.4byte BattleScript_EffectSuperFang             @ EFFECT_SUPER_FANG
	.4byte BattleScript_EffectSubstitute            @ EFFECT_SUBSTITUTE
	.4byte BattleScript_EffectHit                   @ EFFECT_RECOIL_HP_25
	.4byte BattleScript_EffectSketch                @ EFFECT_SKETCH
	.4byte BattleScript_EffectTripleKick            @ EFFECT_TRIPLE_KICK
	.4byte BattleScript_EffectHit                   @ EFFECT_THIEF
	.4byte BattleScript_EffectMeanLook              @ EFFECT_MEAN_LOOK
	.4byte BattleScript_EffectLockOn                @ EFFECT_LOCK_ON
	.4byte BattleScript_EffectNightmare             @ EFFECT_NIGHTMARE
	.4byte BattleScript_EffectSnore                 @ EFFECT_SNORE
	.4byte BattleScript_EffectHit                   @ EFFECT_CURSE
	.4byte BattleScript_EffectHit                   @ EFFECT_FLAIL
	.4byte BattleScript_EffectConversion2           @ EFFECT_CONVERSION_2
	.4byte BattleScript_EffectSpite                 @ EFFECT_SPITE
	.4byte BattleScript_EffectProtect               @ EFFECT_PROTECT
	.4byte BattleScript_EffectHit                   @ EFFECT_BELLY_DRUM
	.4byte BattleScript_EffectAccuracyDownHit       @ EFFECT_ACCURACY_DOWN_HIT
	.4byte BattleScript_EffectSpikes                @ EFFECT_SPIKES
	.4byte BattleScript_EffectForesight             @ EFFECT_FORESIGHT
	.4byte BattleScript_EffectDestinyBond           @ EFFECT_DESTINY_BOND
	.4byte BattleScript_EffectPerishSong            @ EFFECT_PERISH_SONG
	.4byte BattleScript_EffectSandstorm             @ EFFECT_SANDSTORM
	.4byte BattleScript_EffectProtect               @ EFFECT_ENDURE
	.4byte BattleScript_EffectAttackDown2           @ EFFECT_ATTACK_DOWN_2
	.4byte BattleScript_EffectRollout               @ EFFECT_ROLLOUT
	.4byte BattleScript_EffectHit                   @ EFFECT_FALSE_SWIPE
	.4byte BattleScript_EffectSwagger               @ EFFECT_SWAGGER
	.4byte BattleScript_EffectFuryCutter            @ EFFECT_FURY_CUTTER
	.4byte BattleScript_EffectDefenseUpHit          @ EFFECT_DEFENSE_UP_HIT
	.4byte BattleScript_EffectAttract               @ EFFECT_ATTRACT
	.4byte BattleScript_EffectSleepTalk             @ EFFECT_SLEEP_TALK
	.4byte BattleScript_EffectHealBell              @ EFFECT_HEAL_BELL
	.4byte BattleScript_EffectHit                   @ EFFECT_RETURN
	.4byte BattleScript_EffectPresent               @ EFFECT_PRESENT
	.4byte BattleScript_EffectHit                   @ EFFECT_FRUSTRATION
	.4byte BattleScript_EffectSafeguard             @ EFFECT_SAFEGUARD
	.4byte BattleScript_EffectPainSplit             @ EFFECT_PAIN_SPLIT
	.4byte BattleScript_EffectMagnitude             @ EFFECT_MAGNITUDE
	.4byte BattleScript_EffectBatonPass             @ EFFECT_BATON_PASS
	.4byte BattleScript_EffectEncore                @ EFFECT_ENCORE
	.4byte BattleScript_EffectHit                   @ EFFECT_PURSUIT
	.4byte BattleScript_EffectHit                   @ EFFECT_RAPID_SPIN
	.4byte BattleScript_EffectEvasionDown2          @ EFFECT_EVASION_DOWN_2
	.4byte BattleScript_EffectDefenseDownHit        @ EFFECT_DEFENSE_DOWN_HIT
	.4byte BattleScript_EffectAttackUpHit           @ EFFECT_ATTACK_UP_HIT
	.4byte BattleScript_EffectMorningSun            @ EFFECT_MORNING_SUN
	.4byte BattleScript_EffectHit                   @ EFFECT_HIDDEN_POWER
	.4byte BattleScript_EffectRainDance             @ EFFECT_RAIN_DANCE
	.4byte BattleScript_EffectSunnyDay              @ EFFECT_SUNNY_DAY
	.4byte BattleScript_EffectMirrorCoat            @ EFFECT_MIRROR_COAT
	.4byte BattleScript_EffectPsychUp               @ EFFECT_PSYCH_UP
	.4byte BattleScript_EffectAllStatsUpHit         @ EFFECT_ALL_STATS_UP_HIT
	.4byte BattleScript_EffectFutureSight           @ EFFECT_FUTURE_SIGHT
	.4byte BattleScript_EffectBeatUp                @ EFFECT_BEAT_UP
	
	.4byte BattleScript_EffectSpecialAttackUp       @ 
	.4byte BattleScript_EffectSpeedDown             @ 
	.4byte BattleScript_EffectSpecialAttackUp2      @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectSpecialDefenseDown2   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectSpecialAttackDownHit  @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectSkyAttack             @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectThief                 @ 
	.4byte BattleScript_EffectCurse                 @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectThawHit               @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectHit                   @ 
	.4byte BattleScript_EffectBellyDrum
	.4byte BattleScript_EffectTwister
	.4byte BattleScript_EffectGust
	.4byte BattleScript_EffectFlinchMinimizeHit
	.4byte BattleScript_EffectFakeOut
	.4byte BattleScript_EffectUproar
	.4byte BattleScript_EffectStockpile
	.4byte BattleScript_EffectSpitUp
	.4byte BattleScript_EffectSwallow
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectHail
	.4byte BattleScript_EffectTorment
	.4byte BattleScript_EffectFlatter
	.4byte BattleScript_EffectWillOWisp
	.4byte BattleScript_EffectMemento
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectFocusPunch
	.4byte BattleScript_EffectSmellingsalt
	.4byte BattleScript_EffectFollowMe
	.4byte BattleScript_EffectNaturePower
	.4byte BattleScript_EffectCharge
	.4byte BattleScript_EffectTaunt
	.4byte BattleScript_EffectHelpingHand
	.4byte BattleScript_EffectTrick
	.4byte BattleScript_EffectRolePlay
	.4byte BattleScript_EffectWish
	.4byte BattleScript_EffectAssist
	.4byte BattleScript_EffectIngrain
	.4byte BattleScript_EffectSuperpower
	.4byte BattleScript_EffectMagicCoat
	.4byte BattleScript_EffectRecycle
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectBrickBreak
	.4byte BattleScript_EffectYawn
	.4byte BattleScript_EffectKnockOff
	.4byte BattleScript_EffectEndeavor
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectSkillSwap
	.4byte BattleScript_EffectImprison
	.4byte BattleScript_EffectRefresh
	.4byte BattleScript_EffectGrudge
	.4byte BattleScript_EffectSnatch
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectSecretPower
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectTeeterDance
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectMudSport
	.4byte BattleScript_EffectPoisonFang
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectOverheat
	.4byte BattleScript_EffectTickle
	.4byte BattleScript_EffectCosmicPower
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectBulkUp
	.4byte BattleScript_EffectHit
	.4byte BattleScript_EffectMudSport
	.4byte BattleScript_EffectCalmMind
	.4byte BattleScript_EffectDragonDance
	.4byte BattleScript_EffectCamouflage

@@@@@@@@@@@@@@@@@@@@@@@
@ MOVE BATTLE SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_EffectHit::
	attackcanceler
BattleScript_HitFromAccCheck::
	accuracycheck BattleScript_PrintMoveMissed
BattleScript_HitFromAtkString::
	attackstring
	ppreduce
BattleScript_HitFromCritCalc::
	critcalc
	damagecalc
	typecalc
BattleScript_HitFromDamageAdjust::
	adjustdamage
BattleScript_HitFromAtkAnimation::
	attackanimation
	waitstate
	effectivenesssound
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage B_WAIT_TIME_LONG
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	seteffectwithchance
BattleScript_MoveEndFromFaint::
	tryfaintmon BS_TARGET
BattleScript_MoveEnd::
	moveendall
	end

BattleScript_EffectHit_Ret::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
BattleScript_EffectHitFromAtkString_Ret::
	attackstring
	ppreduce
BattleScript_EffectHitFromCritCalc_Ret::
	critcalc
	damagecalc
	typecalc
	adjustdamage
	attackanimation
	waitstate
BattleScript_EffectHitFromEffectiveness_Ret::
	effectivenesssound
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage B_WAIT_TIME_LONG
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_ButItFailedAtkStringPpReduce::
	attackstring
BattleScript_ButItFailedPpReduce::
	ppreduce
BattleScript_ButItFailed::
	pause B_WAIT_TIME_SHORT
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MakeMoveMissed::
	orhalfword gMoveResultFlags, MOVE_RESULT_MISSED
BattleScript_PrintMoveMissed::
	attackstring
	ppreduce
BattleScript_MoveMissedPause::
	pause B_WAIT_TIME_SHORT
BattleScript_MoveMissed::
	effectivenesssound
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MultiHitPrintStrings::
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	jumpifmovehadnoeffect BattleScript_MultiHitEnd
	copyarray gBattleTextBuff1, sMULTIHIT_STRING, 6
	printstring STRINGID_HITXTIMES
	waitmessage B_WAIT_TIME_LONG
	return
	
BattleScript_MultiHitEnd::
	seteffectwithchance
	tryfaintmon BS_TARGET
	moveendcase ATK49_SYNCHRONIZE_TARGET
	moveendfrom ATK49_STATUS_IMMUNITY_ABILITIES
	end

BattleScript_FlushMessageBox::
	flushmessagebox
	return

BattleScript_EffectPayDay::
	setmoveeffect MOVE_EFFECT_PAYDAY, FALSE, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectBurnHit::
	setmoveeffect MOVE_EFFECT_BURN
	goto BattleScript_EffectHit

BattleScript_EffectFreezeHit::
	setmoveeffect MOVE_EFFECT_FREEZE
	goto BattleScript_EffectHit

BattleScript_EffectParalyzeHit::
	setmoveeffect MOVE_EFFECT_PARALYSIS
	goto BattleScript_EffectHit

BattleScript_EffectOHKO::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	typecalc
	jumpifmovehadnoeffect BattleScript_HitFromAtkAnimation
	tryKO BattleScript_KOFail
	trysetdestinybondtohappen
	goto BattleScript_HitFromAtkAnimation

BattleScript_KOFail::
	pause B_WAIT_TIME_LONG
	printfromtable gKOFailedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectTwoTurnMoves::
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_TwoTurnMovesSecondTurn
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_TwoTurnMovesSecondTurn
	setbyte sTWOTURN_STRINGID, 0
	call BattleScriptFirstChargingTurn
	goto BattleScript_MoveEnd

BattleScript_TwoTurnMovesSecondTurn::
	attackcanceler
	clearbattlerstatus BS_ATTACKER, ID_STATUS2, STATUS2_MULTIPLETURNS
	orword gHitMarker, HITMARKER_NO_PPDEDUCT
	argumenttomoveeffect
	setbyte sB_ANIM_TURN, 1
	goto BattleScript_HitFromAccCheck

BattleScriptFirstChargingTurn::
	attackcanceler
	flushmessagebox
	ppreduce
	attackanimation
	waitstate
	setchargingturn
	copybyte sMULTISTRING_CHOOSER, sTWOTURN_STRINGID
	printfromtable gFirstTurnOfTwoStringIds
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_EffectStatUp::
	attackcanceler
BattleScript_EffectStatUpAfterAtkCanceler::
	attackstring
	ppreduce
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_MoveEnd
	jumpifbyte CMP_NOT_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_StatUpAttackAnim
	pause B_WAIT_TIME_SHORT
	goto BattleScript_StatUpPrintString

BattleScript_StatUpAttackAnim::
	attackanimation
	waitstate
BattleScript_StatUpDoAnim::
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
BattleScript_StatUpPrintString::
	printfromtable gStatUpStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectAttackUp2::
	setstatchanger STAT_ATK, 2, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectRoar::
	attackcanceler
	attackstring
	ppreduce
	jumpifability BS_TARGET, ABILITY_GUARD_DOG, BattleScript_ButItFailed
	jumpifability BS_TARGET, ABILITY_SUCTION_CUPS, BattleScript_AbilityPreventsPhasingOut
	jumpifstatus3 BS_TARGET, STATUS3_ROOTED, BattleScript_PrintMonIsRooted
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	accuracycheck BattleScript_MoveMissedPause
	jumpifroarfails BattleScript_ButItFailed
	forcerandomswitch BattleScript_ButItFailed

BattleScript_PrintMonIsRooted::
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_PKMNANCHOREDITSELF
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_SuccessForceOut::
	attackanimation
	waitstate
	switchoutabilities BS_TARGET
	returntoball BS_TARGET
	waitstate
	jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_TrainerBattleForceOut
	setbyte gBattleOutcome, B_OUTCOME_PLAYER_TELEPORTED
	finishaction
BattleScript_TrainerBattleForceOut::
	getswitchedmondata BS_TARGET
	switchindataupdate BS_TARGET
	tryremoveprimalweather BS_TARGET
	switchinanim BS_TARGET, FALSE
	waitstate
	printstring STRINGID_PKMNWASDRAGGEDOUT
	switchineffects BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_EffectSemiInvulnerable::
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_SecondTurnSemiInvulnerable
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_SecondTurnSemiInvulnerable
	jumpifmove MOVE_FLY, BattleScript_FlyFirstTurn
	jumpifmove MOVE_DIVE, BattleScript_DiveFirstTurn
	jumpifmove MOVE_BOUNCE, BattleScript_BounceFirstTurn
	@ MOVE_DIG
	setbyte sTWOTURN_STRINGID, 5
	goto BattleScript_FirstTurnSemiInvulnerable

BattleScript_BounceFirstTurn::
	setbyte sTWOTURN_STRINGID, 7
	goto BattleScript_FirstTurnSemiInvulnerable

BattleScript_DiveFirstTurn::
	setbyte sTWOTURN_STRINGID, 6
	goto BattleScript_FirstTurnSemiInvulnerable

BattleScript_FlyFirstTurn::
	setbyte sTWOTURN_STRINGID, 4
BattleScript_FirstTurnSemiInvulnerable::
	call BattleScriptFirstChargingTurn
	setsemiinvulnerablebit
	goto BattleScript_MoveEnd

BattleScript_SecondTurnSemiInvulnerable::
	attackcanceler
	clearbattlerstatus BS_ATTACKER, ID_STATUS2, STATUS2_MULTIPLETURNS
	setbyte sB_ANIM_TURN, 1
	orword gHitMarker, HITMARKER_NO_PPDEDUCT
	argumenttomoveeffect
	accuracycheck BattleScript_SemiInvulnerableMiss
	clearsemiinvulnerablebit
	goto BattleScript_HitFromAtkString

BattleScript_SemiInvulnerableMiss::
	clearsemiinvulnerablebit
	goto BattleScript_PrintMoveMissed

BattleScript_EffectTrap::
	setmoveeffect MOVE_EFFECT_WRAP, FALSE, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectFlinchHit::
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_EffectHit

BattleScript_EffectRecoilIfMiss::
	attackcanceler
	accuracycheck BattleScript_MoveMissedDoDamage
	typecalc
	jumpifhalfword CMP_COMMON_BITS, gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE, BattleScript_MoveMissedDoDamage
	goto BattleScript_HitFromAtkString

BattleScript_MoveMissedDoDamage::
	attackstring
	ppreduce
	pause B_WAIT_TIME_LONG
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	jumpifability BS_ATTACKER, ABILITY_MAGIC_GUARD, BattleScript_MoveEnd
	printstring STRINGID_PKMNCRASHED
	waitmessage B_WAIT_TIME_LONG
	damagecalc
	typecalc
	adjustdamage
	manipulatedamage ATK80_DMG_HALF_USER_HP
	bichalfword gMoveResultFlags, MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER
	orhalfword gMoveResultFlags, MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE
	goto BattleScript_MoveEnd

BattleScript_EffectStatDown::
	attackcanceler
	jumpifsubstituteblocks BattleScript_ButItFailedAtkStringPpReduce
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_MoveEnd
	jumpifbyte CMP_LESS_THAN, sMULTISTRING_CHOOSER, 2, BattleScript_StatDownDoAnim
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 3, BattleScript_MoveEnd
	pause B_WAIT_TIME_SHORT
	goto BattleScript_StatDownPrintString

BattleScript_StatDownDoAnim::
	attackanimation
	waitstate
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
BattleScript_StatDownPrintString::
	printfromtable gStatDownStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectAccuracyDown::
	setstatchanger STAT_ACC, 1, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectRampage::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_EffectRampageTryConfuse
	ppreduce
BattleScript_EffectRampageTryConfuse::
	setmoveeffect MOVE_EFFECT_THRASH, TRUE, TRUE
	goto BattleScript_HitFromCritCalc

BattleScript_EffectDefenseDown::
	setstatchanger STAT_DEF, 1, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectPoisonHit::
	setmoveeffect MOVE_EFFECT_POISON
	goto BattleScript_EffectHit

BattleScript_EffectAttackDown::
	setstatchanger STAT_ATK, 1, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectSleep::
	attackcanceler
	attackstring
	ppreduce
	trysetsleep BS_TARGET, 0, BattleScript_CantMakeAsleep
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setmoveeffect MOVE_EFFECT_SLEEP
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_CantMakeAsleep::
    pause B_WAIT_TIME_SHORT
	printfromtable gUproarAwakeStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectConfuse::
	attackcanceler
	attackstring
	ppreduce
	trysetconfusion BS_TARGET
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectFixedDamage::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	typecalc2 @ no effectiveness
	argumenttomovedamage
	goto BattleScript_HitFromDamageAdjust

BattleScript_AlreadyConfused::
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_PKMNALREADYCONFUSED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_SafeguardProtected::
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_PKMNUSEDSAFEGUARD
	waitmessage B_WAIT_TIME_LONG
	end2
	
BattleScript_EffectDisable::
	attackcanceler
	attackstring
	ppreduce
	jumpifabilityonside BS_TARGET, ABILITY_AROMA_VEIL, BattleScript_AromaVeilProtects
	accuracycheck BattleScript_ButItFailed
	disablelastusedattack BS_TARGET, BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNMOVEWASDISABLED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSpecialDefenseDownHit::
	setmoveeffect MOVE_EFFECT_SP_DEF_MINUS_1
	goto BattleScript_EffectHit

BattleScript_EffectMist::
	attackcanceler
	attackstring
	ppreduce
	setmist
	attackanimation
	waitstate
	printfromtable gMistUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd
	
BattleScript_EffectConfuseHit::
	setmoveeffect MOVE_EFFECT_CONFUSION
	goto BattleScript_EffectHit

BattleScript_EffectSpeedDownHit::
	setmoveeffect MOVE_EFFECT_SPD_MINUS_1
	goto BattleScript_EffectHit

BattleScript_EffectAttackDownHit::
	setmoveeffect MOVE_EFFECT_ATK_MINUS_1
	goto BattleScript_EffectHit
	
BattleScript_EffectRecharge::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	setmoveeffect MOVE_EFFECT_RECHARGE, TRUE, TRUE
	goto BattleScript_HitFromAtkString

BattleScript_EffectCounter::
	attackcanceler
	trycounterattack SPLIT_PHYSICAL, BattleScript_ButItFailedAtkStringPpReduce
BattleScript_CounterAttack::
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	typecalc2 @ no effectiveness
	goto BattleScript_HitFromDamageAdjust

BattleScript_EffectLevelDamage::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	typecalc2 @ no effectiveness
	manipulatedamage ATK48_DMG_ATK_LEVEL
	goto BattleScript_HitFromDamageAdjust

BattleScript_EffectAbsorb::
    attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	call BattleScript_EffectHitFromAtkString_Ret
	setbyte sMULTISTRING_CHOOSER, 0
BattleScript_AbsorbDrainHp::
	manipulatedamage ATK80_DMG_DRAINED
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	jumpifability BS_TARGET, ABILITY_LIQUID_OOZE, BattleScript_AbsorbLiquidOoze
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	jumpifmovehadnoeffect BattleScript_AbsorbTryFainting
	printfromtable gDrainHpStringIds
	waitmessage B_WAIT_TIME_LONG
BattleScript_AbsorbTryFainting::
	tryfaintmon BS_ATTACKER
	goto BattleScript_MoveEndFromFaint

BattleScript_EffectLeechSeed::
	attackcanceler
	attackstring
	pause B_WAIT_TIME_SHORT
	ppreduce
	jumpifsubstituteblocks BattleScript_ButItFailed
	accuracycheck BattleScript_DoLeechSeed
BattleScript_DoLeechSeed::
	setseeded
	attackanimation
	waitstate
	printfromtable gLeechSeedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSolarbeam::
	jumpifweatheraffected BS_ATTACKER, WEATHER_SUN_ANY, BattleScript_SolarbeamOnFirstTurn
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_TwoTurnMovesSecondTurn
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_TwoTurnMovesSecondTurn
	setbyte sTWOTURN_STRINGID, 1
	call BattleScriptFirstChargingTurn
	goto BattleScript_MoveEnd

BattleScript_SolarbeamOnFirstTurn::
    setchargingturn
	ppreduce
	goto BattleScript_TwoTurnMovesSecondTurn

BattleScript_EffectPoison::
	attackcanceler
	attackstring
	ppreduce
	trysetpoison BS_TARGET
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setmoveeffect MOVE_EFFECT_POISON
	seteffectprimary
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectParalyze::
	attackcanceler
	attackstring
	ppreduce
	typecalc
	jumpifmovehadnoeffect BattleScript_ButItFailed
	trysetparalyze BS_TARGET
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setmoveeffect MOVE_EFFECT_PARALYSIS
	seteffectprimary
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSpeedDown2::
	setstatchanger STAT_SPEED, 2, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectToxic::
	attackcanceler
	attackstring
	ppreduce
	trysetpoison BS_TARGET
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setmoveeffect MOVE_EFFECT_TOXIC
	seteffectprimary
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectAttackUp::
	setstatchanger STAT_ATK, 1, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectSpeedUp2::
	setstatchanger STAT_SPEED, 2, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectRage::
	attackcanceler
	accuracycheck BattleScript_RageMiss
	call BattleScript_EffectHitFromAtkString_Ret
	jumpifmovehadnoeffect BattleScript_MoveEndFromFaint
	jumpifsubstituteblocks BattleScript_MoveEndFromFaint
	setragestatus
	goto BattleScript_MoveEndFromFaint

BattleScript_RageMiss::
    clearbattlerstatus BS_ATTACKER, ID_STATUS2, STATUS2_RAGE
	goto BattleScript_PrintMoveMissed

BattleScript_RageIsBuilding::
    setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR BattleScript_RageIsBuildingReturn
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_RageIsBuildingReturn
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	jumpifability BS_TARGET, ABILITY_CONTRARY, BattleScript_RageIsBuildingContraryMessage
BattleScript_RageIsBuildingMessage::
	printstring STRINGID_PKMNRAGEBUILDING
	waitmessage B_WAIT_TIME_LONG
BattleScript_RageIsBuildingReturn::
	return

BattleScript_EffectTeleport::
    jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_EffectBatonPass
	jumpifside BS_ATTACKER, B_SIDE_PLAYER, BattleScript_EffectBatonPass
	attackcanceler
	attackstring
	ppreduce
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_ButItFailed
	getifcantrunfrombattle BS_ATTACKER, FALSE
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, BATTLE_RUN_FORBIDDEN, BattleScript_ButItFailed
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, BATTLE_RUN_FAILURE, BattleScript_PrintAbilityMadeIneffective
	attackanimation
	waitstate
	printstring STRINGID_PKMNFLEDFROMBATTLE
	waitmessage B_WAIT_TIME_LONG
	setteleportoutcome BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectMimic::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed
	mimicattackcopy BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNLEARNEDMOVE2
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectDefenseDown2::
	setstatchanger STAT_DEF, 2, TRUE
	goto BattleScript_EffectStatDown
	
BattleScript_EffectEvasionUp::
	setstatchanger STAT_EVASION, 1, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectRestoreHp::
	attackcanceler
	attackstring
	ppreduce
	tryhealhalfhealth BS_TARGET, BattleScript_AlreadyAtFullHp
BattleScript_PresentHealTarget::
	attackanimation
	waitstate
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_AlreadyAtFullHp::
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_PKMNHPFULL
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectDefenseUp::
	setstatchanger STAT_DEF, 1, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectMinimize::
	attackcanceler
	setminimize
	setstatchanger STAT_EVASION, 1, FALSE
	goto BattleScript_EffectStatUpAfterAtkCanceler

BattleScript_EffectDefenseCurl::
	attackcanceler
	attackstring
	ppreduce
	setdefensecurlbit
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_StatUpDoAnim
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_StatUpPrintString
	attackanimation
	waitstate
	goto BattleScript_StatUpDoAnim

BattleScript_EffectDefenseUp2::
	setstatchanger STAT_DEF, 2, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectLightScreen::
	attackcanceler
	attackstring
	ppreduce
	setlightscreen
BattleScript_PrintReflectLightScreenSafeguardString::
	attackanimation
	waitstate
	printfromtable gReflectLightScreenSafeguardStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectHaze::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitstate
	normaliseallbuffs
	printstring STRINGID_STATCHANGESGONE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectReflect::
	attackcanceler
	attackstring
	ppreduce
	setreflect
	goto BattleScript_PrintReflectLightScreenSafeguardString

BattleScript_EffectFocusEnergy::
	attackcanceler
	attackstring
	ppreduce
	jumpifstatus2 BS_ATTACKER, STATUS2_FOCUS_ENERGY, BattleScript_ButItFailed
	setfocusenergy
	attackanimation
	waitstate
	printfromtable gFocusEnergyUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectBide::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitstate
	setbide
	goto BattleScript_MoveEnd

BattleScript_EffectMetronome::
	attackcanceler
	attackstring
	pause B_WAIT_TIME_SHORT
	attackanimation
	waitstate
	setbyte sB_ANIM_TURN, 0
	setbyte sB_ANIM_TARGETS_HIT, 0
	metronome

BattleScript_EffectMirrorMove::
	attackcanceler
	attackstring
	pause B_WAIT_TIME_LONG
	trymirrormove
	@ Failed
	ppreduce
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	printstring STRINGID_MIRRORMOVEFAILED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectExplosion::
	attackcanceler
	attackstring
	ppreduce
	instantfaintattacker
	waitstate
	accuracycheck BattleScript_MoveMissedPause
	call BattleScript_EffectHitFromCritCalc_Ret
	tryfaintmon BS_TARGET
	moveendall
	tryfaintmon BS_ATTACKER
	end

BattleScript_EffectSkullBash::
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_TwoTurnMovesSecondTurn
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_TwoTurnMovesSecondTurn
	setbyte sTWOTURN_STRINGID, 2
	call BattleScriptFirstChargingTurn
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_MoveEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_MoveEnd
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSpecialDefenseUp2::
	setstatchanger STAT_SPDEF, 2, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectDreamEater::
	attackcanceler
	attackstring
	ppreduce
	jumpifstatus BS_TARGET, STATUS1_SLEEP, BattleScript_DreamEaterWorked
	jumpifability BS_TARGET, ABILITY_COMATOSE, BattleScript_DreamEaterWorked
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_PKMNWASNTAFFECTED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_DreamEaterWorked::
	accuracycheck BattleScript_MoveMissedPause
	call BattleScript_EffectHitFromCritCalc_Ret
	setbyte sMULTISTRING_CHOOSER, 1
	goto BattleScript_AbsorbDrainHp

BattleScript_EffectTransform::
	attackcanceler
	attackstring
	ppreduce
	transformdataexecution
	attackanimation
	waitstate
	printfromtable gTransformUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	tryendeffectonabilitychange BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectPsywave::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	typecalc2 @ no effectiveness
	psywavedamageeffect
	goto BattleScript_HitFromDamageAdjust

BattleScript_EffectDoNothing::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitstate
	incrementgamestat GAME_STAT_USED_SPLASH
	printstring STRINGID_BUTNOTHINGHAPPENED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectRest::
	attackcanceler
	attackstring
	ppreduce
	trysetsleep BS_TARGET, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD | STATUS_CHANGE_FLAG_IGNORE_GENERAL_STATUS | STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE, BattleScript_CantMakeAsleep
	trysetrest BattleScript_AlreadyAtFullHp
	pause B_WAIT_TIME_SHORT
	printfromtable gRestUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_TARGET
	waitstate
	goto BattleScript_PresentHealTarget

BattleScript_EffectConversion::
	attackcanceler
	attackstring
	ppreduce
	tryconversiontypechange BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNCHANGEDTYPE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectTriAttack::
	setmoveeffect MOVE_EFFECT_TRI_ATTACK
	goto BattleScript_EffectHit

BattleScript_EffectSuperFang::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	typecalc2 @ no effectiveness
	manipulatedamage ATK48_DMG_HALF_TARGET_HP
	goto BattleScript_HitFromDamageAdjust

BattleScript_EffectSubstitute::
	attackcanceler
	attackstring
	ppreduce
	setsubstitute
	jumpifbyte CMP_NOT_EQUAL, sMULTISTRING_CHOOSER, 1, BattleScript_SubstituteAnim
	pause B_WAIT_TIME_SHORT
	goto BattleScript_SubstituteString

BattleScript_SubstituteAnim::
	attackanimation
	waitstate
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
BattleScript_SubstituteString::
	printfromtable gSubsituteUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_AlreadyHasSubstitute::
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_PKMNHASSUBSTITUTE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSketch::
	attackcanceler
	attackstring
	ppreduce
	copymovepermanently BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNSKETCHEDMOVE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectTripleKick::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	critcalc
	damagecalc
	typecalc
	addbyte sTRIPLE_KICK_POWER, 10
	goto BattleScript_HitFromDamageAdjust

BattleScript_EffectMeanLook::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed
	jumpifstatus2 BS_TARGET, STATUS2_ESCAPE_PREVENTION | STATUS2_SUBSTITUTE, BattleScript_ButItFailed
	attackanimation
	waitstate
	setescapeprevention
	printstring STRINGID_TARGETCANTESCAPENOW
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectLockOn::
	attackcanceler
	attackstring
	ppreduce
	jumpifsubstituteblocks BattleScript_ButItFailed
	accuracycheck BattleScript_ButItFailed
	trysetalwayshitflag BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNTOOKAIM
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectNightmare::
	attackcanceler
	attackstring
	ppreduce
	jumpifsubstituteblocks BattleScript_ButItFailed
	jumpifstatus2 BS_TARGET, STATUS2_NIGHTMARE, BattleScript_ButItFailed
	jumpifstatus BS_TARGET, STATUS1_SLEEP, BattleScript_NightmareWorked
	jumpifability BS_TARGET, ABILITY_COMATOSE, BattleScript_NightmareWorked
	goto BattleScript_ButItFailed

BattleScript_NightmareWorked::
	attackanimation
	waitstate
	setnightmare
	printstring STRINGID_PKMNFELLINTONIGHTMARE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSnore::
	attackcanceler
	jumpifstatus BS_ATTACKER, STATUS1_SLEEP, BattleScript_SnoreIsAsleep
	jumpifability BS_ATTACKER, ABILITY_COMATOSE, BattleScript_SnoreIsAsleep
	goto BattleScript_ButItFailedAtkStringPpReduce

BattleScript_SnoreIsAsleep::
    jumpifhalfword CMP_EQUAL, gChosenMove, MOVE_SLEEP_TALK, BattleScript_DoSnore @ skip this anim if Snore is called through Sleep Talk
	printstring STRINGID_PKMNFASTASLEEP
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_SLEEP
BattleScript_DoSnore::
	attackstring
	ppreduce
	accuracycheck BattleScript_MoveMissedPause
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_HitFromCritCalc

BattleScript_EffectConversion2::
	attackcanceler
	attackstring
	ppreduce
	settypetorandomresistance BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNCHANGEDTYPE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSpite::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed
	tryspiteppreduce BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNREDUCEDPP
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectProtect::
	attackcanceler
	attackstring
	ppreduce
	setprotectlike
	attackanimation
	waitstate
	printfromtable gProtectLikeUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectAccuracyDownHit::
	setmoveeffect MOVE_EFFECT_ACC_MINUS_1
	goto BattleScript_EffectHit

BattleScript_EffectSpikes::
	attackcanceler
	trysetspikes BattleScript_ButItFailedAtkStringPpReduce
	attackstring
	ppreduce
	attackanimation
	waitstate
	printstring STRINGID_SPIKESSCATTERED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectForesight::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	jumpifstatus2 BS_TARGET, STATUS2_FORESIGHT, BattleScript_ButItFailed
	attackanimation
	waitstate
	setforesight
	printstring STRINGID_PKMNIDENTIFIED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectDestinyBond::
	attackcanceler
	attackstring
	ppreduce
	trysetdestinybond BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNTRYINGTOTAKEFOE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectPerishSong::
	attackcanceler
	attackstring
	ppreduce
	trysetperishsong BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_FAINTINTHREE
	waitmessage B_WAIT_TIME_LONG
	setbyte sBATTLER, 0
BattleScript_PerishSongLoop::
    jumpifbyteequal gBattlerAttacker, sBATTLER, BattleScript_PerishSongLoopIncrement
	jumpifability BS_SCRIPTING, ABILITY_SOUNDPROOF, BattleScript_SoundproofBlocksPerishSong
BattleScript_PerishSongLoopIncrement::
	addbyte sBATTLER, 1
	jumpifbytenotequal sBATTLER, gBattlersCount, BattleScript_PerishSongLoop
	goto BattleScript_MoveEnd

BattleScript_EffectSandstorm::
	attackcanceler
	attackstring
	ppreduce
	setsandstorm
BattleScript_MoveWeatherChange::
	attackanimation
	waitstate
	printfromtable gMoveWeatherChangeStringIds
	waitmessage B_WAIT_TIME_LONG
	call BattleScript_ActivateWeatherChangeAbilities
	goto BattleScript_MoveEnd

BattleScript_EffectAttackDown2::
	setstatchanger STAT_ATK, 2, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectRollout::
	attackcanceler
	attackstring
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_RolloutCheckAccuracy
	ppreduce
BattleScript_RolloutCheckAccuracy::
	accuracycheck BattleScript_RolloutHit
BattleScript_RolloutHit::
	typecalc2 @ no effectiveness
	handlerollout
	goto BattleScript_HitFromCritCalc

BattleScript_EffectSwagger::
	attackcanceler
	jumpifsubstituteblocks BattleScript_MakeMoveMissed
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	jumpifconfusedandstatmaxed STAT_ATK, BattleScript_ButItFailed
	attackanimation
	waitstate
	setstatchanger STAT_ATK, 2, FALSE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_SwaggerTryConfuse
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_SwaggerTryConfuse
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage B_WAIT_TIME_LONG
BattleScript_SwaggerTryConfuse::
    jumpifstatus2 BS_TARGET, STATUS2_CONFUSION, BattleScript_MoveEnd @ dont fail due to already confused
    trysetconfusion BS_TARGET
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_EffectFuryCutter::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_FuryCutterHit
BattleScript_FuryCutterHit::
	handlefurycutter
	critcalc
	damagecalc
	typecalc
	jumpifmovehadnoeffect BattleScript_FuryCutterHit
	goto BattleScript_HitFromDamageAdjust

BattleScript_EffectDefenseUpHit::
	setmoveeffect MOVE_EFFECT_DEF_PLUS_1, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectAttract::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed
	tryinfatuating BS_TARGET
	attackanimation
	waitstate
	printstring STRINGID_PKMNFELLINLOVE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectSleepTalk::
	attackcanceler
	jumpifstatus BS_ATTACKER, STATUS1_SLEEP, BattleScript_SleepTalkIsAsleep
	jumpifability BS_ATTACKER, ABILITY_COMATOSE, BattleScript_SleepTalkIsAsleep
	goto BattleScript_ButItFailedAtkStringPpReduce

BattleScript_SleepTalkIsAsleep::
	printstring STRINGID_PKMNFASTASLEEP
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_SLEEP
	attackstring
	ppreduce
	orword gHitMarker, HITMARKER_NO_PPDEDUCT | HITMARKER_ALLOW_NO_PP
	trychoosesleeptalkmove BattleScript_ButItFailed
	attackanimation
	waitstate
	setbyte sB_ANIM_TURN, 0
	setbyte sB_ANIM_TARGETS_HIT, 0
	jumptocalledmove FALSE

BattleScript_EffectHealBell::
	attackcanceler
	attackstring
	ppreduce
	attackanimation
	waitstate
	healpartystatus 0 @ Choose string to print
	printfromtable gPartyStatusHealStringIds
	waitmessage B_WAIT_TIME_LONG
	setbyte sMULTIUSE_STATE, 0 @ Loop counter
	healpartystatus 1 @ Heal all status
	waitstate
	goto BattleScript_MoveEnd
	
BattleScript_HealBellActivateOnBattler::
    cureprimarystatus BS_SCRIPTING, BattleScript_HealBellActivateOnParty
	updatestatusicon BS_SCRIPTING
	waitstate
BattleScript_HealBellActivateOnParty::
    printstring STRINGID_BUFF2CUREDOFITSBUFF1
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_EffectPresent::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	typecalc
	presentcalc

BattleScript_EffectSafeguard::
	attackcanceler
	attackstring
	ppreduce
	setsafeguard
	goto BattleScript_PrintReflectLightScreenSafeguardString

BattleScript_EffectPainSplit::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed
	painsplitdmgcalc BattleScript_ButItFailed
	attackanimation
	waitstate
	orword gHitMarker, HITMARKER_SKIP_DMG_TRACK | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	copyword gBattleMoveDamage, sPAINSPLIT_HP
	orword gHitMarker, HITMARKER_SKIP_DMG_TRACK | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_SHAREDPAIN
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectMagnitude::
	attackcanceler
	attackstring
	ppreduce
	magnitudedamagecalculation
	goto BattleScript_HitFromCritCalc
	
BattleScript_MagnitudeString::
    printstring STRINGID_MAGNITUDESTRENGTH
	waitmessage B_WAIT_TIME_LONG
	return
	
BattleScript_EffectBatonPass::
	attackcanceler
	attackstring
	ppreduce
	jumpifcantswitch BS_ATTACKER | ATK4F_DONT_CHECK_STATUSES, BattleScript_ButItFailed
	attackanimation
	waitstate
	openpartyscreen BS_ATTACKER, BattleScript_ButItFailed
	switchoutabilities BS_ATTACKER
	waitstate
	switchhandleorder BS_ATTACKER, 2
	returntoball BS_ATTACKER
	getswitchedmondata BS_ATTACKER
	switchindataupdate BS_ATTACKER
	hpthresholds BS_ATTACKER
	tryremoveprimalweather BS_ATTACKER
	printstring STRINGID_SWITCHINMON
	switchinanim BS_ATTACKER, TRUE
	waitstate
	switchineffects BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectEncore::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	jumpifabilityonside BS_TARGET, ABILITY_AROMA_VEIL, BattleScript_AromaVeilProtects
	trysetencore BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNGOTENCORE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectEvasionDown2::
	setstatchanger STAT_EVASION, 2, TRUE
	goto BattleScript_EffectStatDown

BattleScript_EffectDefenseDownHit::
	setmoveeffect MOVE_EFFECT_DEF_MINUS_1
	goto BattleScript_EffectHit

BattleScript_EffectAttackUpHit::
	setmoveeffect MOVE_EFFECT_ATK_PLUS_1, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectMorningSun::
	attackcanceler
	attackstring
	ppreduce
	recoverbasedonweather BattleScript_AlreadyAtFullHp
	goto BattleScript_PresentHealTarget

BattleScript_EffectRainDance::
	attackcanceler
	attackstring
	ppreduce
	setrain
    goto BattleScript_MoveWeatherChange

BattleScript_EffectSunnyDay::
	attackcanceler
	attackstring
	ppreduce
	setsunny
	goto BattleScript_MoveWeatherChange

BattleScript_EffectMirrorCoat::
    attackcanceler
	trycounterattack SPLIT_SPECIAL, BattleScript_ButItFailedAtkStringPpReduce
	goto BattleScript_CounterAttack

BattleScript_EffectPsychUp::
	attackcanceler
	attackstring
	ppreduce
	copyfoestats
	attackanimation
	waitstate
	printstring STRINGID_PKMNCOPIEDSTATCHANGES
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectAllStatsUpHit::
	setmoveeffect MOVE_EFFECT_ALL_STATS_UP, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectFutureSight::
	attackcanceler
	attackstring
	ppreduce
	trysetfutureattack BattleScript_ButItFailed
	attackanimation
	waitstate
	printfromtable gFutureMoveUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_EffectBeatUp::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	addbyte sMULTIUSE_STATE, 1
	goto BattleScript_HitFromAtkString

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ MOVE EFFECTS BATTLE SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_MoveEffectSleep::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_SLEEP
	printstring STRINGID_PKMNFELLASLEEP
	waitmessage B_WAIT_TIME_LONG
BattleScript_UpdateEffectStatusIconRet::
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	return

BattleScript_MoveEffectPoison::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_POISON
	printstring STRINGID_PKMNWASPOISONED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectBurn::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_BURN
	printstring STRINGID_PKMNWASBURNED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectFreeze::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_FREEZE
	printstring STRINGID_PKMNWASFROZEN
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	formchange BS_EFFECT_BATTLER, SPECIES_SHAYMIN_SKY, SPECIES_SHAYMIN, TRUE, TRUE, BattleScript_MoveEffectFreezeReturn
	copybyte sBATTLER, gEffectBattler @ for the string
    playanimation BS_EFFECT_BATTLER, B_ANIM_FORM_CHANGE
	waitstate
	printstring STRINGID_PKMNTRANSFORMED
	waitmessage B_WAIT_TIME_LONG
BattleScript_MoveEffectFreezeReturn::
    return

BattleScript_MoveEffectParalysis::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_PARALYSIS
	printstring STRINGID_PKMNWASPARALYZED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectToxic::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_TOXIC_POISON
	printstring STRINGID_PKMNBADLYPOISONED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_UpdateEffectStatusIconRet

BattleScript_MoveEffectConfusion::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS2, STATUS2_CONFUSION
	printstring STRINGID_PKMNWASCONFUSED
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_MoveEffectPayDay::
	printstring STRINGID_COINSSCATTERED
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_MoveEffectWrap::
	printfromtable gWrappedStringIds
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_MoveEffectRecoil::
	jumpifmove MOVE_STRUGGLE, BattleScript_DoRecoil
	jumpifability BS_ATTACKER, ABILITY_ROCK_HEAD, BattleScript_RecoilEnd
	jumpifability BS_ATTACKER, ABILITY_MAGIC_GUARD, BattleScript_RecoilEnd
BattleScript_DoRecoil::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNHITWITHRECOIL
	waitmessage B_WAIT_TIME_LONG
	tryfaintmon BS_ATTACKER
BattleScript_RecoilEnd::
	return

BattleScript_StatDown::
	playanimation BS_EFFECT_BATTLER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage B_WAIT_TIME_LONG
	return

@@@@@@@@@@@@@@@@@@@@@@@@@@
@ ABILITY BATTLE SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_AbilityPreventsPhasingOut::
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNANCHORSITSELFWITH
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_OwnTempoPrevents::
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNPREVENTSCONFUSIONWITH
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_AromaVeilProtects::
	pause B_WAIT_TIME_SHORT
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_AROMAVEILPROTECTED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	goto BattleScript_MoveEnd

BattleScript_AbsorbLiquidOoze::
	manipulatedamage ATK80_DMG_CHANGE_SIGN
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	jumpifmovehadnoeffect BattleScript_AbsorbTryFainting
	loadabilitypopup BS_TARGET
	printstring STRINGID_ITSUCKEDLIQUIDOOZE
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_AbsorbTryFainting

BattleScript_RageIsBuildingContraryMessage::
    printfromtable gStatDownStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_RageIsBuildingMessage

BattleScript_AbilityPreventSleep::
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printfromtable STRINGID_ITDOESNTAFFECT
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_SwitchInWeatherAbilityActivates::
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_SCRIPTING
	printfromtable gSwitchInWeatherAbilitiesStrings
	waitstate
	playanimation2 BS_BATTLER_0, sB_ANIM_ARG1
	removeabilitypopup BS_SCRIPTING
	call BattleScript_ActivateWeatherChangeAbilities
	end3

BattleScript_TraceActivates::
	pause B_WAIT_TIME_SHORT
	copyability BS_ATTACKER, BS_TARGET
	abilitycopypopup BS_ATTACKER, BS_TARGET, STRINGID_PKMNTRACED
	switchinabilities BS_ATTACKER
	return

BattleScript_TraceActivatesEnd3::
    call BattleScript_TraceActivates
	end3
	
BattleScript_RainDishActivates::
    loadabilitypopup BS_ATTACKER
	printfromtable gRainDishStringIds
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	end3

BattleScript_ShedSkinActivates::
    loadabilitypopup BS_ATTACKER
	printstring STRINGID_PKMNSXCUREDYPROBLEM
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	updatestatusicon BS_ATTACKER
	end3

BattleScript_MagicBounce::
    attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_MOVEBOUNCEDBACKBYABILITY
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	orword gHitMarker, HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_ALLOW_NO_PP
	setmagiccoattarget BS_ATTACKER
	return
	
BattleScript_IceFaceFade::
	loadabilitypopup BS_TARGET
	playanimation BS_TARGET, B_ANIM_FORM_CHANGE
	printstring STRINGID_PKMNTRANSFORMED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	return

BattleScript_DisguiseBusted::
    loadabilitypopup BS_TARGET
	printstring STRINGID_DISGUISESERVEDASDECOY
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_TARGET, B_ANIM_FORM_CHANGE
	waitstate
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_DISGUISEBUSTED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	return

BattleScript_MagicianActivates::
    loadabilitypopup BS_ATTACKER
	printstring STRINGID_PKMNSTOLEITEM
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	return

BattleScript_PickpocketActivation::
    loadabilitypopup BS_TARGET
	swapattackerwithtarget
	printstring STRINGID_PKMNSTOLEITEM
	waitmessage B_WAIT_TIME_LONG
	swapattackerwithtarget
	removeabilitypopup BS_TARGET
	return

BattleScript_EmergencyExit::
    loadabilitypopup BS_TARGET
	pause B_WAIT_TIME_LONG
	playanimation BS_TARGET, B_ANIM_SLIDE_OUT_OFFSCREEN
	waitstate
	removeabilitypopup BS_TARGET
	pause B_WAIT_TIME_LONG
	jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_EmergencyExitSwitchOut
	jumpifside BS_TARGET, B_SIDE_PLAYER, BattleScript_EmergencyExitSwitchOut
	setteleportoutcome BS_TARGET
	finishaction
	return

BattleScript_EmergencyExitSwitchOut::
	openpartyscreen BS_TARGET, BattleScript_EmergencyExitRet
	switchoutabilities BS_TARGET
	waitstate
	switchhandleorder BS_TARGET, 2
	returntoball BS_TARGET
	getswitchedmondata BS_TARGET
	switchindataupdate BS_TARGET
	hpthresholds BS_TARGET
	printstring STRINGID_SWITCHINMON
	switchinanim BS_TARGET, TRUE
	waitstate
	switchineffects BS_TARGET
BattleScript_EmergencyExitRet::
    return

BattleScript_Pickup::
    loadabilitypopup BS_ATTACKER
	printstring STRINGID_ATKFOUNDLASTITEM
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	end3
	
BattleScript_ReceiverActivates::
    copyability BS_SCRIPTING_PARTNER, BS_SCRIPTING
	loadabilitypopup BS_SCRIPTING_PARTNER
	pause B_WAIT_TIME_SHORT
	removeabilitypopup BS_SCRIPTING_PARTNER
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_SCRIPTING_PARTNER
	printstring STRINGID_PKMNABLWASTAKENOVER
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING_PARTNER
	return

BattleScript_NeutralizingGasActivates::
    call BattleScript_NeutralizingGasActivatesRet
	end3

BattleScript_NeutralizingGasActivatesRet::
    pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNABLFILLEDTHEAREA
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	savebattlers
	setbyte gBattlerTarget, 0
BattleScript_NeutralizingGasActivatesLoop::
    copybyte sBATTLER, gBattlerTarget
	jumpifabsent BS_SCRIPTING, BattleScript_NeutralizingGasLoopIncrement
	jumpifbyteequal sBATTLER, gBattlerAttacker, BattleScript_NeutralizingGasLoopIncrement
    tryneutralizinggassuppression BS_SCRIPTING, BattleScript_NeutralizingGasLoopIncrement
	tryendeffectonabilitychange BS_SCRIPTING
BattleScript_NeutralizingGasLoopIncrement::
    addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_NeutralizingGasActivatesLoop
	restorebattlers
    return

BattleScript_NeutralizingGasExits::
	savebattlers
	printstring STRINGID_NEUTRALIZINGGASOVER
	waitmessage B_WAIT_TIME_LONG
	setbyte gBattlerTarget, 0
BattleScript_NeutralizingGasExitsLoop::
    jumpifabsent BS_TARGET, BattleScript_NeutralizingGasExitsLoopIncrement
	jumpifbyteequal gBattlerTarget, sBATTLER, BattleScript_NeutralizingGasExitsLoopIncrement
    switchinabilities BS_TARGET
BattleScript_NeutralizingGasExitsLoopIncrement::
	addbyte gBattlerTarget, 1
	jumpifbyte CMP_NOT_EQUAL, gBattlerTarget, MAX_BATTLERS_COUNT, BattleScript_NeutralizingGasExitsLoop
	restorebattlers
	return

BattleScript_PerishBodyActivates::
    loadabilitypopup BS_TARGET
	printstring STRINGID_BOTHWILLPERISHIN3TURNS
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
    return

BattleScript_WanderingSpiritActivates::
    swapabilities
	swapattackerwithtarget
	abilityswappopup BS_ATTACKER, BS_TARGET, STRINGID_PKMNSWAPPEDABILITIES
	swapattackerwithtarget
    return

BattleScript_WindPowerActivates::
    loadabilitypopup BS_TARGET
	printstring STRINGID_CURRMOVECHARGEDDEFPOWER
	waitmessage B_WAIT_TIME_LONG
	jumpifabsent BS_TARGET, BattleScript_WindPowerEnd @ dont set charge when faint, but still show the string
	setcharge
BattleScript_WindPowerEnd::
	removeabilitypopup BS_TARGET
	return

BattleScript_SandSpitActivated::
    loadabilitypopup BS_TARGET
	printstring STRINGID_ASANDSTORMKICKEDUP
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_BATTLER_0, B_ANIM_SANDSTORM_CONTINUES
	removeabilitypopup BS_TARGET
	call BattleScript_ActivateWeatherChangeAbilities
	return

BattleScript_SlowStartEnd::
    loadabilitypopup BS_ATTACKER
	printstring STRINGID_PKMNGOTITSACT
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	end3

BattleScript_PastelVeilActivates::
    pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PASTELVEILCUREDSTATUS
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	updatestatusicon BS_TARGET
	end3

BattleScript_TeamProtectedByPastelVeil::
    pause B_WAIT_TIME_SHORT
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
    loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PASTELVEILPROTECTED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	goto BattleScript_MoveEnd

BattleScript_ZenModeActivatesPause::
    pause B_WAIT_TIME_SHORT
BattleScript_ZenModeActivates::
    loadabilitypopup BS_SCRIPTING
	playanimation BS_SCRIPTING, B_ANIM_FORM_CHANGE
	waitstate
	printfromtable gFormChangeAbilitiesStrings
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	end3

BattleScript_SchoolingActivatesPause::
    pause B_WAIT_TIME_SHORT
BattleScript_SchoolingActivates::
    loadabilitypopup BS_SCRIPTING
	playanimation BS_SCRIPTING, B_ANIM_FORM_CHANGE @ assign it an apropriated animation
	waitstate
	printfromtable gStartedSchoolingStringIds
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	end3

BattleScript_TeamProtectedByFlowerVeil::
    pause B_WAIT_TIME_SHORT
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_FLOWERVEILPROTECTED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	goto BattleScript_MoveEnd

BattleScript_ProteanActivates::
    flushmessagebox
    loadabilitypopup BS_ATTACKER
    printstring STRINGID_PKMNCHANGEDTYPE
    waitmessage B_WAIT_TIME_LONG
    removeabilitypopup BS_ATTACKER
    return

BattleScript_TeamProtectedBySweetVeil::
    pause B_WAIT_TIME_SHORT
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_SWEETVEILPROTECTED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	goto BattleScript_MoveEnd

BattleScript_AttackerFormChange::
    flushmessagebox
	loadabilitypopup BS_ATTACKER
	playanimation BS_ATTACKER, B_ANIM_FORM_CHANGE
	waitstate
	printfromtable gFormChangeAbilitiesStrings
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	return

BattleScript_GooeyActivates::
    waitstate
	loadabilitypopup BS_TARGET
	seteffectprimary STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD
	removeabilitypopup BS_TARGET
	return

BattleScript_IllusionOff::
    loadabilitypopup BS_SCRIPTING
	spriteignore0hp
	playanimation BS_SCRIPTING, B_ANIM_ILLUSION_OFF
	waitstate
	updatenick BS_SCRIPTING
	waitstate
	spriteignore0hp
	printstring STRINGID_PKMNILLUSIONOFF
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	return

BattleScript_ImposterActivates::
    loadabilitypopup BS_ATTACKER
	transformdataexecution
	playmoveanimation BS_ATTACKER, MOVE_TRANSFORM
	waitstate
	printfromtable gTransformUsedStringIds
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	end3

BattleScript_MummyActivates::
	copyability BS_ATTACKER, BS_TARGET
	abilitycopypopup BS_TARGET, BS_ATTACKER, STRINGID_ATKABILITYBECAMEABL
	tryendeffectonabilitychange BS_ATTACKER
	return

BattleScript_HarvestActivates::
	tryrecycleitem BattleScript_HarvestEnd
	loadabilitypopup BS_ATTACKER
	printstring STRINGID_ATKHARVESTEDITSITEM
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
BattleScript_HarvestEnd::
	end3

BattleScript_CursedBodyActivation::
	loadabilitypopup BS_TARGET
	printstring STRINGID_DEFABLDISABLEDATKCURRMOVE
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	return

BattleScript_HealerActivates::
    loadabilitypopup BS_ATTACKER
	printstring STRINGID_ATKABLCUREDEFFPROBLEM
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	updatestatusicon BS_EFFECT_BATTLER
	end3

BattleScript_SturdyPreventsOHKO::
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNPROTECTEDBY
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_DampStopsExplosion::
    attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNPREVENTSUSAGE
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	goto BattleScript_MoveEnd

BattleScript_BadDreamsActivates::
	setbyte gBattlerTarget, 0
BattleScript_BadDreamsLoop::
    jumpifabsent BS_TARGET, BattleScript_BadDreamsNextTarget
    jumpiftargetally BattleScript_BadDreamsNextTarget
    jumpifability BS_TARGET, ABILITY_MAGIC_GUARD, BattleScript_BadDreamsNextTarget
	jumpifability BS_TARGET, ABILITY_COMATOSE, BattleScript_BadDreamsDmg
	jumpifstatus BS_TARGET, STATUS1_SLEEP, BattleScript_BadDreamsDmg
	goto BattleScript_BadDreamsNextTarget
BattleScript_BadDreamsDmg::
    loadabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, TRUE
	printstring STRINGID_DEFISTORMENTED
	waitmessage B_WAIT_TIME_LONG
	manipulatedamage ATK80_DMG_1_8_TARGET_MAX_HP
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	tryfaintmon BS_TARGET
	checkteamslot BattleScript_BadDreamsNextTarget
BattleScript_BadDreamsNextTarget::
	addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_BadDreamsLoop
    removeabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, FALSE
	end2

BattleScript_DrySkinSunActivates::
    loadabilitypopup BS_ATTACKER
	printstring STRINGID_PKMNLOSTSOMEOFHP
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_ATTACKER
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER
	end3

BattleScript_Frisk::
    pause B_WAIT_TIME_SHORT
	setbyte gBattlerTarget, 0
BattleScript_FriskLoop::
    jumpifabsent BS_TARGET, BattleScript_FriskNextTarget
	jumpiftargetally BattleScript_FriskNextTarget
	tryfrisktarget BattleScript_FriskNextTarget
	loadabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, TRUE
	printstring STRINGID_ATKFRISKEDDEFFOUNDLASTITEM
	waitmessage B_WAIT_TIME_LONG
	recordlastability BS_ATTACKER
BattleScript_FriskNextTarget::
    addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_FriskLoop
	removeabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, FALSE
	end3

BattleScript_SynchronizeActivates::
	waitstate
	loadabilitypopup BS_SCRIPTING @ Activate even if the status fails to be applyed
	seteffectprimary STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD | STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE
	removeabilitypopup BS_SCRIPTING
	return
	
BattleScript_AirLock::
    pause B_WAIT_TIME_SHORT
    loadabilitypopup BS_SCRIPTING
    printstring STRINGID_WEATHEREFFECTSDISAPPEARED
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
    call BattleScript_ActivateWeatherChangeAbilities
	end3

BattleScript_SoundproofProtected::
	attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNSXBLOCKSY
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_SoundproofBlocksPerishSong::
    loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNSXBLOCKSY2
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	goto BattleScript_PerishSongLoopIncrement

BattleScript_SoundproofBlocksHealBell::
    loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNSXBLOCKSY2
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	return

BattleScript_DazzlingProtected::
    attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_ATKCANTUSECURRMOVE
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_FlashFireBoost_PPLoss::
	ppreduce
BattleScript_FlashFireBoost::
	attackstring
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printfromtable gFlashFireStringIds
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_StickyHoldActivates::
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNSXMADEYINEFFECTIVE
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_ColorChangeActivates::
    loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNCHANGEDTYPEWITH
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	return

BattleScript_RoughSkinActivates::
    loadabilitypopup BS_TARGET
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNHURTSWITH
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	tryfaintmon BS_ATTACKER
	return

BattleScript_CuteCharmActivates::
	chosenstatusanimation BS_ATTACKER, ID_STATUS2, STATUS2_INFATUATION
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNSXINFATUATEDY
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_TARGET
	return

BattleScript_DisplaySwitchInMsg::
    call BattleScript_SwitchInAbilityMsgRet
	end3
	
BattleScript_SwitchInAbilityMsgRet::
    pause B_WAIT_TIME_SHORT
    loadabilitypopup BS_SCRIPTING
    printfromtable gSwitchInAbilitiesMsgStringIds
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_SCRIPTING
	return

BattleScript_PoisonTouchActivation::
    waitstate
	loadabilitypopup BS_ATTACKER
	call BattleScript_MoveEffectPoison
	removeabilitypopup BS_ATTACKER
	return

BattleScript_ApplySecondaryEffect::
	waitstate
	loadabilitypopup BS_TARGET
	seteffectsecondary STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD | STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE @ These are already checked
	removeabilitypopup BS_TARGET
	return

BattleScript_LimberProtected::
    pause B_WAIT_TIME_SHORT
	copybyte gEffectBattler, gBattlerTarget
	call BattleScript_LimberProtectedRet
	goto BattleScript_MoveEnd

BattleScript_LimberProtectedRet::
    pause B_WAIT_TIME_SHORT
    loadabilitypopup BS_EFFECT_BATTLER
	printstring STRINGID_PKMNPREVENTSPARALYSISWITH
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup BS_EFFECT_BATTLER
	return

@@@@@@@@@@@@@@@@@@@@@@@@@
@ OTHERS BATTLE SCRIPTS @
@@@@@@@@@@@@@@@@@@@@@@@@@

BattleScript_BideStoringEnergy::
	printstring STRINGID_PKMNSTORINGENERGY
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_BideAttack::
	attackcanceler
	clearbattlerstatus BS_ATTACKER, ID_STATUS2, STATUS2_MULTIPLETURNS
	printstring STRINGID_PKMNUNLEASHEDENERGY
	waitmessage B_WAIT_TIME_LONG
	accuracycheck BattleScript_MoveMissed
	typecalc2 @ no effectiveness
	copyword gBattleMoveDamage, sBIDE_DMG
	setbyte sB_ANIM_TURN, 1
	goto BattleScript_HitFromDamageAdjust

BattleScript_BideNoEnergyToAttack::
	attackcanceler
	clearbattlerstatus BS_ATTACKER, ID_STATUS2, STATUS2_MULTIPLETURNS
	printstring STRINGID_PKMNUNLEASHEDENERGY
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_ButItFailed

BattleScript_DarkTypePreventsPrankster::
    orhalfword gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_PrimalWeatherBlocksMove::
	attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	printfromtable gPrimalWeatherBlocksAttackStringIds
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsAsleep::
	printstring STRINGID_PKMNFASTASLEEP
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_SLEEP
	goto BattleScript_MoveEnd

BattleScript_MoveUsedWokeUp::
	printfromtable gWokeUpStringIds
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_ATTACKER
	return

BattleScript_MoveUsedIsFrozen::
	printstring STRINGID_PKMNISFROZEN
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_FREEZE
	goto BattleScript_MoveEnd
	
BattleScript_MoveUsedUnfroze::
	printfromtable gGotDefrostedStringIds
	waitmessage B_WAIT_TIME_LONG
	updatestatusicon BS_ATTACKER
	return

BattleScript_IgnoresAndUsesRandomMove::
	printstring STRINGID_PKMNIGNOREDORDERS
	waitmessage B_WAIT_TIME_LONG
	jumptocalledmove TRUE
	
BattleScript_MoveUsedLoafingAround::
	printfromtable gInobedientStringIds
	waitmessage B_WAIT_TIME_LONG
	moveendto ATK49_NEXT_TARGET
	end

BattleScript_MoveUsedMustRecharge::
	printstring STRINGID_PKMNMUSTRECHARGE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MoveUsedFlinched::
	printstring STRINGID_PKMNFLINCHED
	waitmessage B_WAIT_TIME_LONG
	jumpifability BS_ATTACKER, ABILITY_STEADFAST, BattleScript_SteadfastSpeedUp
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsDisabled::
	printstring STRINGID_PKMNMOVEISDISABLED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsTaunted::
	printstring STRINGID_PKMNCANTUSEMOVETAUNT
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsImprisoned::
	printstring STRINGID_PKMNCANTUSEMOVESEALED
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsConfused::
	printstring STRINGID_PKMNISCONFUSED
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS2, STATUS2_CONFUSION
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 1, BattleScript_DoSelfConfusionDmg
	return
	
BattleScript_DoSelfConfusionDmg::
	adjustdamage
	printstring STRINGID_ITHURTCONFUSION
	waitmessage B_WAIT_TIME_LONG
	effectivenesssound
	playanimation BS_ATTACKER, B_ANIM_MON_HIT
	waitstate
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	tryfaintmon BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsConfusedNoMore::
	printstring STRINGID_PKMNHEALEDCONFUSION
	waitmessage B_WAIT_TIME_LONG
	return

BattleScript_MoveUsedIsParalyzed::
	printstring STRINGID_PKMNISPARALYZED
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_PARALYSIS
	goto BattleScript_MoveEnd
	
BattleScript_TooScaredToMove::
	printstring STRINGID_MONTOOSCAREDTOMOVE
	waitmessage B_WAIT_TIME_LONG
	playanimation BS_ATTACKER, B_ANIM_MON_SCARED
	goto BattleScript_MoveEnd
	
BattleScript_GhostGetOutGetOut::
	printstring STRINGID_GHOSTGETOUTGETOUT
	playanimation BS_ATTACKER, B_ANIM_GHOST_GET_OUT
	goto BattleScript_MoveEnd

BattleScript_MoveUsedIsInLove::
	printstring STRINGID_PKMNINLOVE
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS2, STATUS2_INFATUATION
	return

BattleScript_MoveUsedIsInLoveCantAttack::
	printstring STRINGID_PKMNIMMOBILIZEDBYLOVE
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_MoveEnd

BattleScript_PowderMoveNoEffect::
    attackstring
	ppreduce
	pause B_WAIT_TIME_SHORT
	loadabilitypopup BS_TARGET
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage B_WAIT_TIME_LONG
	sethword gMoveResultFlags, MOVE_RESULT_FAILED
	removeabilitypopup BS_TARGET
	setbyte sBYPASS_ABILITY_POP_UP, FALSE @ clear it if set
	goto BattleScript_MoveEnd

BattleScript_MonTookFutureAttack::
	printstring STRINGID_PKMNTOOKATTACK
	waitmessage B_WAIT_TIME_LONG
	accuracycheck BattleScript_FutureAttackMiss
	critcalc
	damagecalc
	typecalc
	adjustdamage
	playanimation2 BS_ATTACKER, sB_ANIM_ARG1
	call BattleScript_EffectHitFromEffectiveness_Ret
	tryfaintmon BS_TARGET
	checkteamslot BattleScript_FutureAttackMoveEnd
BattleScript_FutureAttackMoveEnd::
    moveendfutureattack
BattleScript_FutureAttackEnd::
	sethword gMoveResultFlags, 0
	end2

BattleScript_FutureAttackMiss::
	pause B_WAIT_TIME_SHORT
	sethword gMoveResultFlags, MOVE_RESULT_FAILED
	resultmessage
	waitmessage B_WAIT_TIME_LONG
	goto BattleScript_FutureAttackEnd

BattleScript_PerishSongTakesLife::
	printstring STRINGID_PKMNPERISHCOUNTFELL
	waitmessage B_WAIT_TIME_LONG
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER
	end2

BattleScript_PerishSongCountGoesDown::
	printstring STRINGID_PKMNPERISHCOUNTFELL
	waitmessage B_WAIT_TIME_LONG
	end2





BattleScript_AlreadyAsleep::
        pause 0x20
	printstring STRINGID_PKMNALREADYASLEEP
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSpecialAttackUp::
	setstatchanger STAT_SPATK, 1, FALSE
	goto BattleScript_EffectStatUp

BattleScript_StatUp::
	playanimation BS_EFFECT_BATTLER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage 0x40
	return

BattleScript_EffectSpeedDown::
	setstatchanger STAT_SPEED, 1, TRUE
	goto BattleScript_EffectStatDown

BattleScript_AlreadyPoisoned::
        pause 0x40
	printstring STRINGID_PKMNALREADYPOISONED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_ImmunityProtected::
        pause 0x40
	copybyte gEffectBattler, gBattlerTarget
	setbyte sMULTISTRING_CHOOSER, 0
	loadabilitypopup BS_TARGET
	call BattleScript_PSNPrevention
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_PSNPrevention::
        pause 0x20
	printfromtable gPSNPreventionStringIds
	waitmessage 0x40
	return

BattleScript_EffectSpecialAttackUp2::
	setstatchanger STAT_SPATK, 2, FALSE
	goto BattleScript_EffectStatUp

BattleScript_EffectSpecialDefenseDown2::
	setstatchanger STAT_SPDEF, 2, TRUE
	goto BattleScript_EffectStatDown

BattleScript_AlreadyParalyzed::
	pause 0x20
	printstring STRINGID_PKMNISALREADYPARALYZED
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_PRLZPrevention::
        pause 0x20
	printfromtable gPRLZPreventionStringIds
	waitmessage 0x40
	return

BattleScript_EffectSpecialAttackDownHit::
	setmoveeffect MOVE_EFFECT_SP_ATK_MINUS_1
	goto BattleScript_EffectHit

BattleScript_EffectSkyAttack::
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_TwoTurnMovesSecondTurn
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_NO_ATTACKSTRING, BattleScript_TwoTurnMovesSecondTurn
	setbyte sTWOTURN_STRINGID, 3
	call BattleScriptFirstChargingTurn
	goto BattleScript_MoveEnd

BattleScript_EffectThief::
	setmoveeffect MOVE_EFFECT_STEAL_ITEM
	goto BattleScript_EffectHit

BattleScript_EffectCurse::
	jumpiftype BS_ATTACKER, TYPE_GHOST, BattleScript_GhostCurse
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER, CMP_GREATER_THAN, STAT_SPEED, 0, BattleScript_CurseTrySpeed
	jumpifstat BS_ATTACKER, CMP_NOT_EQUAL, STAT_ATK, 12, BattleScript_CurseTrySpeed
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_DEF, 12, BattleScript_ButItFailed
BattleScript_CurseTrySpeed::
	copybyte gBattlerTarget, gBattlerAttacker
	setbyte sB_ANIM_TURN, 1
	attackanimation
	waitstate
	setstatchanger STAT_SPEED, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CurseTryAttack
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_CurseTryAttack::
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CurseTryDefence
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CurseTryDefence::
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CurseEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CurseEnd::
	goto BattleScript_MoveEnd

BattleScript_GhostCurse::
	jumpifbytenotequal gBattlerAttacker, gBattlerTarget, BattleScript_DoGhostCurse
	getmovetarget
BattleScript_DoGhostCurse::
	attackcanceler
	attackstring
	ppreduce
	jumpifsubstituteblocks BattleScript_ButItFailed
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	cursetarget BattleScript_ButItFailed
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE
	setbyte sB_ANIM_TURN, 0
	attackanimation
	waitstate
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_PKMNLAIDCURSE
	waitmessage 0x40
	tryfaintmon BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectThawHit::
	setmoveeffect MOVE_EFFECT_BURN
	goto BattleScript_EffectHit

BattleScript_EffectBellyDrum::
	attackcanceler
	attackstring
	ppreduce
	maxattackhalvehp BattleScript_ButItFailed
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	attackanimation
	waitstate
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
        jumpifability BS_ATTACKER, ABILITY_CONTRARY, BattleScript_BellyDrumContraryMessage
	printstring STRINGID_PKMNCUTHPMAXEDATTACK
	
BattleScript_BellyDrumPrintString::
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_BellyDrumContraryMessage::
	printstring STRINGID_ATKCUTHPMINIMIZEDATTACK
	goto BattleScript_BellyDrumPrintString

BattleScript_EffectTwister::
	jumpifnostatus3 BS_TARGET, STATUS3_ON_AIR, BattleScript_FlinchEffect
	setbyte sDMG_MULTIPLIER, 2
BattleScript_FlinchEffect::
	setmoveeffect MOVE_EFFECT_FLINCH
	goto BattleScript_EffectHit

BattleScript_EffectGust::
	goto BattleScript_EffectHit

BattleScript_EffectFlinchMinimizeHit::
	jumpifnostatus3 BS_TARGET, STATUS3_MINIMIZED, BattleScript_FlinchEffect
	setbyte sDMG_MULTIPLIER, 2
	goto BattleScript_FlinchEffect

BattleScript_EffectFakeOut::
	attackcanceler
	jumpifnotfirstturn BattleScript_ButItFailedAtkStringPpReduce
	setmoveeffect MOVE_EFFECT_FLINCH, FALSE, TRUE
	goto BattleScript_EffectHit

BattleScript_NotAffected::
	pause 0x20
	orhalfword gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	resultmessage
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectUproar::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	setmoveeffect MOVE_EFFECT_UPROAR, TRUE
	attackstring
	jumpifstatus2 BS_ATTACKER, STATUS2_MULTIPLETURNS, BattleScript_HitFromCritCalc
	ppreduce
	goto BattleScript_HitFromCritCalc

BattleScript_EffectStockpile::
	attackcanceler
	attackstring
	ppreduce
	stockpile
	attackanimation
	waitstate
	printfromtable gStockpileUsedStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSpitUp::
	attackcanceler
	jumpifbyte CMP_EQUAL, sMISS_TYPE, 1, BattleScript_SpitUpNoDamage
	attackstring
	ppreduce
	accuracycheck BattleScript_PrintMoveMissed
	stockpiletobasedamage BattleScript_SpitUpFail
	typecalc
	adjustdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_SpitUpFail::
    jumpifparentalbondcounter PARENTAL_BOND_1ST_HIT, BattleScript_MoveEnd
	pause 0x20
	printstring STRINGID_FAILEDTOSPITUP
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SpitUpNoDamage::
	attackstring
	ppreduce
	pause 0x40
	stockpiletobasedamage BattleScript_SpitUpFail
	resultmessage
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSwallow::
	attackcanceler
	attackstring
	ppreduce
	stockpiletohpheal BattleScript_SwallowFail
	goto BattleScript_PresentHealTarget

BattleScript_SwallowFail::
	pause 0x20
	printfromtable gSwallowFailStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectHail::
	attackcanceler
	attackstring
	ppreduce
	sethail
	goto BattleScript_MoveWeatherChange

BattleScript_EffectTorment::
	attackcanceler
	attackstring
	ppreduce
	jumpifabilityonside BS_TARGET, ABILITY_AROMA_VEIL, BattleScript_AromaVeilProtects
	accuracycheck BattleScript_ButItFailed
	settorment BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNSUBJECTEDTOTORMENT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectFlatter::
	attackcanceler
	jumpifsubstituteblocks BattleScript_MakeMoveMissed
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	jumpifconfusedandstatmaxed 4, BattleScript_ButItFailed
	attackanimation
	waitstate
	setstatchanger STAT_SPATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_FlatterTryConfuse
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_FlatterTryConfuse
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_FlatterTryConfuse::
	@ jumpifability BS_TARGET, ABILITY_OWN_TEMPO, BattleScript_OwnTempoPrevents
	@ jumpifsideaffecting BS_TARGET, SIDE_STATUS_SAFEGUARD, BattleScript_SafeguardProtected
	setmoveeffect MOVE_EFFECT_CONFUSION
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_EffectWillOWisp::
	attackcanceler
	attackstring
	ppreduce
	jumpifsubstituteblocks BattleScript_ButItFailed
	trysetburn BS_TARGET
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setmoveeffect MOVE_EFFECT_BURN
	seteffectprimary
	goto BattleScript_MoveEnd

BattleScript_AlreadyBurned::
	pause 0x20
	printstring STRINGID_PKMNALREADYHASBURN
	waitmessage 0x40
	goto BattleScript_MoveEnd
	
BattleScript_WaterVeilPrevents::
        pause 0x40
	copybyte gEffectBattler, gBattlerTarget
	setbyte sMULTISTRING_CHOOSER, 0
	loadabilitypopup BS_TARGET
	call BattleScript_BRNPrevention
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_BRNPrevention::
       pause 0x20
       printfromtable gBRNPreventionStringIds
       waitmessage 0x40
       return

BattleScript_EffectMemento::
	attackcanceler
	jumpifbyte CMP_EQUAL, sMISS_TYPE, 1, BattleScript_MementoNoReduceStats
	attackstring
	ppreduce
	jumpifattackandspecialattackcannotfall BattleScript_ButItFailed
	instantfaintattacker
	attackanimation
	waitstate
	jumpifsubstituteblocks BattleScript_MementoSubstituteInvulnerable
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_TARGET, BIT_ATK | BIT_SPATK, ATK48_STAT_NEGATIVE | ATK48_STAT_BY_TWO | ATK48_ONLY_MULTIPLE
	playstatchangeanimation BS_TARGET, BIT_ATK, ATK48_STAT_NEGATIVE | ATK48_STAT_BY_TWO
	setstatchanger STAT_ATK, 2, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_MementoSkipStatDown1
	jumpifbyte CMP_GREATER_THAN, sMULTISTRING_CHOOSER, 1, BattleScript_MementoSkipStatDown1
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_MementoSkipStatDown1::
	playstatchangeanimation BS_TARGET, BIT_SPATK, ATK48_STAT_NEGATIVE | ATK48_STAT_BY_TWO
	setstatchanger STAT_SPATK, 2, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_MementoSkipStatDown2
	jumpifbyte CMP_GREATER_THAN, sMULTISTRING_CHOOSER, 1, BattleScript_MementoSkipStatDown2
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_MementoSkipStatDown2::
	tryfaintmon BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_MementoSubstituteInvulnerable::
	printstring STRINGID_BUTNOEFFECT
	waitmessage 0x40
	goto BattleScript_MementoSkipStatDown2

BattleScript_MementoNoReduceStats::
	attackstring
	ppreduce
	jumpifattackandspecialattackcannotfall BattleScript_MementoNoReduceStatsEnd
BattleScript_MementoNoReduceStatsEnd::
	instantfaintattacker
	pause 0x40
	effectivenesssound
	resultmessage
	waitmessage 0x40
	tryfaintmon BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectFocusPunch::
	attackcanceler
	jumpifnodamage BattleScript_HitFromAccCheck
	ppreduce
	printstring STRINGID_PKMNLOSTFOCUS
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSmellingsalt::
	jumpifsubstituteblocks BattleScript_EffectHit
	setmoveeffect MOVE_EFFECT_REMOVE_PARALYSIS, FALSE, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectFollowMe::
	attackcanceler
	attackstring
	ppreduce
	setforcedtarget
	attackanimation
	waitstate
	printstring STRINGID_PKMNCENTERATTENTION
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectNaturePower::
	attackcanceler
	attackstring
	pause 0x20
	callterrainattack
	printstring STRINGID_NATUREPOWERTURNEDINTO
	waitmessage 0x40
	return

BattleScript_EffectCharge::
	attackcanceler
	attackstring
	ppreduce
	setcharge
	attackanimation
	waitstate
	printstring STRINGID_PKMNCHARGINGPOWER
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectTaunt::
	attackcanceler
	attackstring
	ppreduce
	jumpifability BS_TARGET, ABILITY_OBLIVIOUS, BattleScript_ObliviousPrevents
	jumpifabilityonside BS_TARGET, ABILITY_AROMA_VEIL, BattleScript_AromaVeilProtects
	accuracycheck BattleScript_ButItFailed
	settaunt BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNFELLFORTAUNT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectHelpingHand::
	attackcanceler
	attackstring
	ppreduce
	trysethelpinghand BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNREADYTOHELP
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectTrick::
	attackcanceler
	attackstring
	ppreduce
	jumpifsubstituteblocks BattleScript_ButItFailed
	accuracycheck BattleScript_ButItFailed
	tryswapitems BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNSWITCHEDITEMS
	waitmessage 0x40
	printfromtable gItemSwapStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRolePlay::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	copyability BS_ATTACKER, BS_TARGET
	attackanimation
	waitstate
	printstring STRINGID_PKMNCOPIEDFOE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectWish::
	attackcanceler
	attackstring
	ppreduce
	trywish 0, BattleScript_ButItFailed
	attackanimation
	waitstate
	goto BattleScript_MoveEnd

BattleScript_EffectAssist::
	attackcanceler
	attackstring
	assistattackselect BattleScript_ButItFailedPpReduce
	attackanimation
	waitstate
	setbyte sB_ANIM_TURN, 0
	setbyte sB_ANIM_TARGETS_HIT, 0
	jumptocalledmove FALSE
BattleScript_EffectIngrain::
	attackcanceler
	attackstring
	ppreduce
	trysetroots BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNPLANTEDROOTS
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSuperpower::
	setmoveeffect MOVE_EFFECT_ATK_DEF_DOWN, TRUE, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectMagicCoat::
	attackcanceler
	trysetmagiccoat BattleScript_ButItFailedAtkStringPpReduce
	attackstring
	ppreduce
	attackanimation
	waitstate
	printstring STRINGID_PKMNSHROUDEDITSELF
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRecycle::
	attackcanceler
	attackstring
	ppreduce
	tryrecycleitem BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_XFOUNDONEY
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectBrickBreak::
	attackcanceler
	accuracycheck BattleScript_PrintMoveMissed
	attackstring
	ppreduce
	removelightscreenreflect
	critcalc
	damagecalc
	typecalc
	adjustdamage
	jumpifbyte CMP_EQUAL, sB_ANIM_TURN, 0, BattleScript_BrickBreakAnim
	bichalfword gMoveResultFlags, MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE
BattleScript_BrickBreakAnim::
	attackanimation
	waitstate
	jumpifbyte CMP_LESS_THAN, sB_ANIM_TURN, 2, BattleScript_BrickBreakDoHit
	printstring STRINGID_THEWALLSHATTERED
	waitmessage 0x40
BattleScript_BrickBreakDoHit::
	typecalc
	effectivenesssound
	waitstate
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	critmessage
	waitmessage 0x40
	resultmessage
	waitmessage 0x40
	seteffectwithchance
	tryfaintmon BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_EffectYawn::
	attackcanceler
	attackstring
	ppreduce
	trysetsleep BS_TARGET, 0, BattleScript_ButItFailed
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	setyawn BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNWASMADEDROWSY
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_PrintAbilityMadeIneffective::
	pause 0x20
	printstring STRINGID_PKMNSXMADEITINEFFECTIVE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectKnockOff::
	setmoveeffect MOVE_EFFECT_KNOCK_OFF
	goto BattleScript_EffectHit

BattleScript_EffectEndeavor::
	attackcanceler
	attackstring
	ppreduce
	setdamagetohealthdifference BattleScript_ButItFailed
	copyword gHpDealt, gBattleMoveDamage
	accuracycheck BattleScript_MoveMissedPause
	typecalc
	jumpifmovehadnoeffect BattleScript_HitFromAtkAnimation
	bichalfword gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	copyword gBattleMoveDamage, gHpDealt
	adjustdamage
	goto BattleScript_HitFromAtkAnimation

BattleScript_EffectSkillSwap::
	attackcanceler
	attackstring
	ppreduce
	accuracycheck BattleScript_ButItFailed, NO_ACC_CALC_CHECK_LOCK_ON
	jumpifmovehadnoeffect BattleScript_ButItFailed
	swapabilities
	attackanimation
	waitstate
	printstring STRINGID_PKMNSWAPPEDABILITIES
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectImprison::
	attackcanceler
	attackstring
	ppreduce
	tryimprison BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNSEALEDOPPONENTMOVE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectRefresh::
	attackcanceler
	attackstring
	ppreduce
	jumpifstatus BS_ATTACKER, STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_POISON | STATUS1_TOXIC_POISON, BattleScript_RefreshHeal
	goto BattleScript_ButItFailed
BattleScript_RefreshHeal::
	cureprimarystatus BS_ATTACKER, BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNSTATUSNORMAL
	waitmessage 0x40
	updatestatusicon BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_EffectGrudge::
	attackcanceler
	attackstring
	ppreduce
	trysetgrudge BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNWANTSGRUDGE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSnatch::
	attackcanceler
	trysetsnatch BattleScript_ButItFailedAtkStringPpReduce
	attackstring
	ppreduce
	attackanimation
	waitstate
	pause 0x20
	printstring STRINGID_PKMNWAITSFORTARGET
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectSecretPower::
	getsecretpowereffect
	goto BattleScript_EffectHit

BattleScript_EffectTeeterDance::
	attackcanceler
	attackstring
	ppreduce
	setbyte gBattlerTarget, 0
BattleScript_TeeterDanceLoop::
	movevaluescleanup
	setmoveeffect MOVE_EFFECT_CONFUSION
	jumpifbyteequal gBattlerAttacker, gBattlerTarget, BattleScript_TeeterDanceLoopIncrement
	jumpifability BS_TARGET, ABILITY_OWN_TEMPO, BattleScript_TeeterDanceOwnTempoPrevents
	jumpifsubstituteblocks BattleScript_TeeterDanceSubstitutePrevents
	jumpifstatus2 BS_TARGET, STATUS2_CONFUSION, BattleScript_TeeterDanceAlreadyConfused
	accuracycheck BattleScript_TeeterDanceMissed
	@ jumpifsideaffecting BS_TARGET, SIDE_STATUS_SAFEGUARD, BattleScript_TeeterDanceSafeguardProtected
	attackanimation
	waitstate
	seteffectprimary
	resultmessage
	waitmessage 0x40
BattleScript_TeeterDanceLoopIncrement::
	moveendto ATK49_NEXT_TARGET
	addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_TeeterDanceLoop
	end

BattleScript_TeeterDanceOwnTempoPrevents::
	pause 0x20
	printstring STRINGID_PKMNPREVENTSCONFUSIONWITH
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceSafeguardProtected::
	pause 0x20
	printstring STRINGID_PKMNUSEDSAFEGUARD
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceSubstitutePrevents::
	pause 0x20
	printstring STRINGID_BUTITFAILED
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceAlreadyConfused::
	pause 0x20
	printstring STRINGID_PKMNALREADYCONFUSED
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_TeeterDanceMissed::
	resultmessage
	waitmessage 0x40
	goto BattleScript_TeeterDanceLoopIncrement

BattleScript_EffectMudSport::
	attackcanceler
	attackstring
	ppreduce
	settypebasedhalvers BattleScript_ButItFailed
	attackanimation
	waitstate
	printfromtable gSportsUsedStringIds
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectPoisonFang::
	setmoveeffect MOVE_EFFECT_TOXIC
	goto BattleScript_EffectHit

BattleScript_EffectOverheat::
	setmoveeffect MOVE_EFFECT_SP_ATK_MINUS_2, TRUE, TRUE
	goto BattleScript_EffectHit

BattleScript_EffectTickle::
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_TARGET, CMP_GREATER_THAN, STAT_ATK, 0, BattleScript_TickleDoMoveAnim
	jumpifstat BS_TARGET, CMP_EQUAL, STAT_DEF, 0, BattleScript_CantLowerMultipleStats
BattleScript_TickleDoMoveAnim::
	accuracycheck BattleScript_ButItFailed
	attackanimation
	waitstate
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_TARGET, BIT_ATK | BIT_DEF, ATK48_STAT_NEGATIVE | ATK48_ONLY_MULTIPLE
	playstatchangeanimation BS_TARGET, BIT_ATK, ATK48_STAT_NEGATIVE
	setstatchanger STAT_ATK, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_TickleTryLowerDef
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_TickleTryLowerDef
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_TickleTryLowerDef::
	playstatchangeanimation BS_TARGET, BIT_DEF, ATK48_STAT_NEGATIVE
	setstatchanger STAT_DEF, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_TickleEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_TickleEnd
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_TickleEnd::
	goto BattleScript_MoveEnd

BattleScript_CantLowerMultipleStats::
	pause 0x20
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	printstring STRINGID_STATSWONTDECREASE2
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectCosmicPower::
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_DEF, 12, BattleScript_CosmicPowerDoMoveAnim
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_SPDEF, 12, BattleScript_CantRaiseMultipleStats
BattleScript_CosmicPowerDoMoveAnim::
	attackanimation
	waitstate
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_DEF | BIT_SPDEF, 0
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CosmicPowerTrySpDef
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_CosmicPowerTrySpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CosmicPowerTrySpDef::
	setstatchanger STAT_SPDEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CosmicPowerEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_CosmicPowerEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CosmicPowerEnd::
	goto BattleScript_MoveEnd

BattleScript_EffectBulkUp::
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_ATK, 12, BattleScript_BulkUpDoMoveAnim
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_DEF, 12, BattleScript_CantRaiseMultipleStats
BattleScript_BulkUpDoMoveAnim::
	attackanimation
	waitstate
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_DEF, 0
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_BulkUpTryDef
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_BulkUpTryDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_BulkUpTryDef::
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_BulkUpEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_BulkUpEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_BulkUpEnd::
	goto BattleScript_MoveEnd

BattleScript_EffectCalmMind::
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_SPATK, 12, BattleScript_CalmMindDoMoveAnim
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_SPDEF, 12, BattleScript_CantRaiseMultipleStats
BattleScript_CalmMindDoMoveAnim::
	attackanimation
	waitstate
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_SPATK | BIT_SPDEF, 0
	setstatchanger STAT_SPATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CalmMindTrySpDef
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_CalmMindTrySpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CalmMindTrySpDef::
	setstatchanger STAT_SPDEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_CalmMindEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_CalmMindEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_CalmMindEnd::
	goto BattleScript_MoveEnd

BattleScript_CantRaiseMultipleStats::
	pause 0x20
	orhalfword gMoveResultFlags, MOVE_RESULT_FAILED
	printstring STRINGID_STATSWONTINCREASE2
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_EffectDragonDance::
	attackcanceler
	attackstring
	ppreduce
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_ATK, 12, BattleScript_DragonDanceDoMoveAnim
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_SPEED, 12, BattleScript_CantRaiseMultipleStats
BattleScript_DragonDanceDoMoveAnim::
	attackanimation
	waitstate
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_SPEED, 0
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_DragonDanceTrySpeed
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_DragonDanceTrySpeed
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_DragonDanceTrySpeed::
	setstatchanger STAT_SPEED, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_DragonDanceEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_DragonDanceEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_DragonDanceEnd::
	goto BattleScript_MoveEnd

BattleScript_EffectCamouflage::
	attackcanceler
	attackstring
	ppreduce
	settypetoterrain BattleScript_ButItFailed
	attackanimation
	waitstate
	printstring STRINGID_PKMNCHANGEDTYPE
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_FaintAttacker::
        tryremoveillusion BS_ATTACKER
	playfaintcry BS_ATTACKER
	pause 0x40
	dofaintanimation BS_ATTACKER
	cleareffectsonfaint BS_ATTACKER
	printstring STRINGID_ATTACKERFAINTED
	flushmessagebox
	tryactivatesoulheart
	tryactivatereceiver BS_ATTACKER
	tryfirstmondowntrainerslide BS_ATTACKER
	return

BattleScript_FaintTarget::
    tryremoveillusion BS_TARGET
	playfaintcry BS_TARGET
	pause 0x40
	dofaintanimation BS_TARGET
	cleareffectsonfaint BS_TARGET
	printstring STRINGID_TARGETFAINTED
	flushmessagebox
	tryactivatemoxie BS_ATTACKER
	tryactivategrimneigh BS_ATTACKER
	tryactivatebeastboost BS_ATTACKER
	tryactivatesoulheart
	tryactivatereceiver BS_TARGET
	tryfirstmondowntrainerslide BS_TARGET
	return

BattleScript_GiveExp::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_TARGET
	end2

BattleScript_HandleFaintedMon::
	checkteamslot BattleScript_LinkBattleHandleFaint
	jumpifbyte CMP_NOT_EQUAL, gBattleOutcome, 0, BattleScript_FaintedMonEnd
	jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_FaintedMonTryChooseAnother
	jumpifword CMP_NO_COMMON_BITS, gHitMarker, HITMARKER_PLAYER_FAINTED, BattleScript_FaintedMonTryChooseAnother
	printstring STRINGID_USENEXTPKMN
	setbyte sMULTIUSE_STATE, 0
	yesnobox
	jumpifbyte CMP_EQUAL, sCURSOR_POSITION, 0, BattleScript_FaintedMonTryChooseAnother
	jumpifplayerran BattleScript_FaintedMonEnd
	printstring STRINGID_CANTESCAPE2
BattleScript_FaintedMonTryChooseAnother::
	openpartyscreen BS_FAINTED, BattleScript_FaintedMonEnd
	switchhandleorder BS_FAINTED, 2
	jumpifnotbattletype BATTLE_TYPE_TRAINER, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_TYPE_LINK, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_TYPE_BATTLE_TOWER, BattleScript_FaintedMonChooseAnother
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_FaintedMonChooseAnother
	jumpifword CMP_COMMON_BITS, gHitMarker, HITMARKER_PLAYER_FAINTED, BattleScript_FaintedMonChooseAnother
	jumpifbyte CMP_EQUAL, sBATTLE_STYLE, 1, BattleScript_FaintedMonChooseAnother
	jumpifcantswitch BS_PLAYER1, BattleScript_FaintedMonChooseAnother
	setbyte sILLUSION_NICK_HACK, 1
	printstring STRINGID_ENEMYABOUTTOSWITCHPKMN
	setbyte sMULTIUSE_STATE, 0
	yesnobox
	jumpifbyte CMP_EQUAL, sCURSOR_POSITION, 1, BattleScript_FaintedMonChooseAnother
	setatktoplayer0
	openpartyscreen BS_ATTACKER | OPEN_PARTY_ALLOW_CANCEL, BattleScript_FaintedMonChooseAnother
	switchhandleorder BS_ATTACKER, 2
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, 6, BattleScript_FaintedMonChooseAnother
	atknameinbuff1
	resetintimidatetracebits BS_ATTACKER
	hpthresholds2 BS_ATTACKER
	printstring STRINGID_RETURNMON
	switchoutabilities BS_ATTACKER
	waitstate
	returnatktoball
	waitstate
	drawpartystatussummary BS_ATTACKER
	getswitchedmondata BS_ATTACKER
	switchindataupdate BS_ATTACKER
	hpthresholds BS_ATTACKER
	tryremoveprimalweather BS_ATTACKER
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_ATTACKER
	switchinanim BS_ATTACKER, FALSE
	waitstate
	switchineffects BS_ATTACKER
	resetsentmonsvalue
BattleScript_FaintedMonChooseAnother::
	drawpartystatussummary BS_FAINTED
	getswitchedmondata BS_FAINTED
	switchindataupdate BS_FAINTED
	hpthresholds BS_FAINTED
	tryremoveprimalweather BS_FAINTED
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_FAINTED
	switchinanim BS_FAINTED, FALSE
	waitstate
	resetplayerfainted
	trylastmontrainerslide BS_FAINTED
	switchineffects BS_FAINTED
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_FaintedMonEnd
	cancelallactions
BattleScript_FaintedMonEnd::
	end2

BattleScript_LinkBattleHandleFaint::
	openpartyscreen BS_UNKNOWN_5, BattleScript_LinkBattleHandleFaintStart
BattleScript_LinkBattleHandleFaintStart::
	switchhandleorder BS_FAINTED, 0
	openpartyscreen BS_UNKNOWN_6, BattleScript_LinkBattleFaintedMonEnd
	switchhandleorder BS_FAINTED, 0
BattleScript_LinkBattleFaintedMonLoop::
	switchhandleorder BS_FAINTED, 2
	drawpartystatussummary BS_FAINTED
	getswitchedmondata BS_FAINTED
	switchindataupdate BS_FAINTED
	hpthresholds BS_FAINTED
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_FAINTED
	switchinanim BS_FAINTED, FALSE
	waitstate
	switchineffects 5
	jumpifbytenotequal gBattlerFainted, gBattlersCount, BattleScript_LinkBattleFaintedMonLoop
BattleScript_LinkBattleFaintedMonEnd::
	end2

BattleScript_LocalTrainerBattleWon::
	printstring STRINGID_PLAYERDEFEATEDTRAINER1
	handletrainerslidecase B_POSITION_OPPONENT_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	printstring STRINGID_TRAINER1LOSETEXT
	getmoneyreward BattleScript_LocalTrainerBattleWonGotMoney
BattleScript_LocalTrainerBattleWonGotMoney::
	printstring STRINGID_PLAYERGOTMONEY
	waitmessage 0x40
BattleScript_PayDayMoneyAndPickUpItems::
	givepaydaymoney
	pickup
	end2

BattleScript_LocalBattleLost::
	jumpifbattletype BATTLE_TYPE_TRAINER_TOWER, BattleScript_BattleTowerLost
	jumpifbattletype BATTLE_TYPE_EREADER_TRAINER, BattleScript_EReaderTrainerEnd
	jumpifbyte CMP_NOT_EQUAL, sMULTISTRING_CHOOSER, 0, BattleScript_RivalBattleLost
BattleScript_LocalBattleLostPrintWhiteOut::
	jumpifbattletype BATTLE_TYPE_TRAINER, BattleScript_LocalBattleLostEnd
	printstring STRINGID_PLAYERWHITEOUT
	waitmessage 0x40
	getmoneyreward BattleScript_LocalBattleLostPrintTrainersWinText
	printstring STRINGID_PLAYERWHITEOUT2
	waitmessage 0x40
	goto BattleScript_EReaderTrainerEnd

BattleScript_LocalBattleLostEnd::
	printstring STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER
	waitmessage 0x40
	getmoneyreward BattleScript_LocalBattleLostPrintTrainersWinText
	printstring STRINGID_PLAYERPAIDPRIZEMONEY
	waitmessage 0x40
BattleScript_EReaderTrainerEnd::
	end2

BattleScript_LocalBattleLostPrintTrainersWinText::
	printstring STRINGID_PLAYERWHITEDOUT
	waitmessage 0x40
	end2

BattleScript_RivalBattleLost::
	jumpifabsent BS_ATTACKER, BattleScript_RivalBattleLostSkipMonRecall
	printstring STRINGID_TRAINER1MON1COMEBACK
	waitmessage 0x40
	returnatktoball
	waitstate
BattleScript_RivalBattleLostSkipMonRecall::
	handletrainerslidecase B_POSITION_OPPONENT_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	printstring STRINGID_TRAINER1WINTEXT
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_LocalBattleLostPrintWhiteOut
	end2

BattleScript_BattleTowerLost::
	getbattlersforrecall
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 0, BattleScript_BattleTowerLostLostSkipMonRecall
	printfromtable gDoubleBattleRecallStrings
	waitmessage 0x40
	returnopponentmon1toball BS_ATTACKER
	waitstate
	returnopponentmon2toball BS_ATTACKER
	waitstate
BattleScript_BattleTowerLostLostSkipMonRecall::
	handletrainerslidecase B_POSITION_OPPONENT_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	printstring STRINGID_TRAINER1WINTEXT
	jumpifnotbattletype BATTLE_TYPE_DOUBLE, BattleScript_BattleTowerLostLostSkipDouble
	printstring STRINGID_TRAINER2NAME
BattleScript_BattleTowerLostLostSkipDouble::
	end2

BattleScript_LinkBattleWonOrLost::
	printstring STRINGID_BATTLEEND
	waitmessage 0x40
	endlinkbattle
	waitmessage 0x40
	end2

BattleScript_BattleTowerTrainerBattleWon::
	printstring STRINGID_PLAYERDEFEATEDTRAINER1
	handletrainerslidecase B_POSITION_OPPONENT_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	jumpifnotbattletype BATTLE_TYPE_TRAINER_TOWER, BattleScript_BattleTowerEtcTrainerBattleWonSkipText
	printstring STRINGID_TRAINER1LOSETEXT
	jumpifnotbattletype BATTLE_TYPE_DOUBLE, BattleScript_BattleTowerEtcTrainerBattleWonSkipText
	printstring STRINGID_TRAINER2CLASS
BattleScript_BattleTowerEtcTrainerBattleWonSkipText::
	pickup
	end2

BattleScript_SmokeBallEscape::
	playanimation BS_ATTACKER, B_ANIM_SMOKEBALL_ESCAPE
	printstring STRINGID_PKMNFLEDUSINGITS
	waitmessage 0x40
	end2

BattleScript_RanAwayUsingMonAbility::
	loadabilitypopup BS_ATTACKER
	pause 0x20
	end2

BattleScript_GotAwaySafely::
	printstring STRINGID_GOTAWAYSAFELY
	waitmessage 0x40
	end2

BattleScript_WildMonFled::
	printstring STRINGID_WILDPKMNFLED
	waitmessage 0x40
	end2

BattleScript_PrintCantRunFromTrainer::
	jumpifbattletype BATTLE_TYPE_FIRST_BATTLE, BattleScript_LeftoverBirchString
	printstring STRINGID_NORUNNINGFROMTRAINERS
	end2

BattleScript_LeftoverBirchString::
	printstring STRINGID_DONTLEAVEBIRCH
	end2

BattleScript_PrintFailedToRunString::
	printfromtable gNoEscapeStringIds
	waitmessage 0x40
	end2

BattleScript_PrintCantEscapeFromBattle::
	printselectionstringfromtable gNoEscapeStringIds
	endselectionscript

BattleScript_PrintFullBox::
	printselectionstring STRINGID_BOXISFULL
	endselectionscript

BattleScript_ActionSwitch::
	hpthresholds2 BS_ATTACKER
	printstring STRINGID_RETURNMON
	setbyte sDMG_MULTIPLIER, 2
	jumpifbattletype BATTLE_TYPE_DOUBLE, BattleScript_PursuitSwitchCheckTwice
	setbyte gMultiHitCounter, 1
	goto BattleScript_PursuitSwitchLoop

BattleScript_PursuitSwitchCheckTwice::
	setbyte gMultiHitCounter, 2
BattleScript_PursuitSwitchLoop::
	jumpifnopursuitswitchdmg BattleScript_DoSwitchOut
	swapattackerwithtarget
	trysetdestinybondtohappen
@ Pursuit damage on switch out
    pause B_WAIT_TIME_SHORT
	call BattleScript_EffectHitFromAtkString_Ret
	tryfaintmon BS_TARGET
	moveendfromto ATK49_MOVE_END_ABILITIES, ATK49_CHOICE_MOVE
	getbattlerfainted BS_TARGET
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, 0, BattleScript_PursuitSwitchRivalSkip
	setbyte sGIVEEXP_STATE, 0
	getexp BS_TARGET
BattleScript_PursuitSwitchRivalSkip::
	swapattackerwithtarget
BattleScript_DoSwitchOut::
    subbyte gMultiHitCounter, 1
	jumpifbyte CMP_NOT_EQUAL, gMultiHitCounter, 0, BattleScript_PursuitSwitchLoop
	switchoutabilities BS_ATTACKER
	waitstate
	returnatktoball
	waitstate
	drawpartystatussummary BS_ATTACKER
	switchhandleorder BS_ATTACKER, 1
	getswitchedmondata BS_ATTACKER
	switchindataupdate BS_ATTACKER
	hpthresholds BS_ATTACKER
	tryremoveprimalweather BS_ATTACKER
	printstring STRINGID_SWITCHINMON
	hidepartystatussummary BS_ATTACKER
	switchinanim BS_ATTACKER, FALSE
	waitstate
	switchineffects BS_ATTACKER
	moveendcase ATK49_STATUS_IMMUNITY_ABILITIES
	moveendcase ATK49_MIRROR_MOVE
	end2

BattleScript_Pausex20::
	pause 0x20
	return

BattleScript_LevelUp::
	fanfare MUS_LEVEL_UP
	printstring STRINGID_PKMNGREWTOLV
	setbyte sLVLBOX_STATE, 0
	drawlvlupbox
	handlelearnnewmove BattleScript_LearnedNewMove, BattleScript_LearnMoveReturn, 1
	goto BattleScript_AskToLearnMove

BattleScript_LearnMoveLoop::
	handlelearnnewmove BattleScript_LearnedNewMove, BattleScript_LearnMoveReturn, 0
BattleScript_AskToLearnMove::
	buffermovetolearn
	printstring STRINGID_TRYTOLEARNMOVE1
	printstring STRINGID_TRYTOLEARNMOVE2
	printstring STRINGID_TRYTOLEARNMOVE3
	waitstate
	setbyte sLEARNMOVE_STATE, 0
	yesnoboxlearnmove BattleScript_ForgotAndLearnedNewMove
	printstring STRINGID_STOPLEARNINGMOVE
	waitstate
	setbyte sLEARNMOVE_STATE, 0
	yesnoboxstoplearningmove BattleScript_AskToLearnMove
	printstring STRINGID_DIDNOTLEARNMOVE
	goto BattleScript_LearnMoveLoop

BattleScript_ForgotAndLearnedNewMove::
	printstring STRINGID_123POOF
	printstring STRINGID_PKMNFORGOTMOVE
	printstring STRINGID_ANDELLIPSIS
BattleScript_LearnedNewMove::
	buffermovetolearn
	fanfare MUS_LEVEL_UP
	printstring STRINGID_PKMNLEARNEDMOVE
	waitmessage 0x40
	updatechoicemoveonlvlup BS_ATTACKER
	goto BattleScript_LearnMoveLoop

BattleScript_LearnMoveReturn::
	return

BattleScript_RainContinuesOrEnds::
	printfromtable gRainContinuesStringIds
	waitmessage 0x40
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_RainContinuesOrEndsEnd
	playanimation BS_ATTACKER, B_ANIM_RAIN_CONTINUES
BattleScript_RainContinuesOrEndsEnd::
	end2

BattleScript_DamagingWeatherContinues::
	printfromtable gSandstormHailContinuesStringIds
	waitmessage 0x40
	playanimation2 BS_ATTACKER, sB_ANIM_ARG1
	setbyte sMULTIUSE_STATE, 0
BattleScript_DamagingWeatherLoop::
	weatherdamage
	jumpifword CMP_EQUAL, gBattleMoveDamage, NULL, BattleScript_DamagingWeatherContinuesEnd
	printfromtable gSandstormHailDmgStringIds
	waitmessage 0x40
	orword gHitMarker, HITMARKER_SKIP_DMG_TRACK | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_GRUDGE | HITMARKER_IGNORE_DISGUISE
	effectivenesssound
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER
	checkteamslot BattleScript_DamagingWeatherContinuesEnd
BattleScript_DamagingWeatherContinuesEnd::
	jumpifbyte CMP_NOT_EQUAL, gBattleOutcome, 0, BattleScript_WeatherDamageEndedBattle
	addbyte sMULTIUSE_STATE, 1
	jumpifbytenotequal sMULTIUSE_STATE, gBattlersCount, BattleScript_DamagingWeatherLoop
BattleScript_WeatherDamageEndedBattle::
	bicword gHitMarker, HITMARKER_SKIP_DMG_TRACK | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_GRUDGE | HITMARKER_IGNORE_DISGUISE
	end2

BattleScript_SandStormHailEnds::
	printfromtable gSandstormHailEndStringIds
	waitmessage 0x40
	end2

BattleScript_SunlightContinues::
	printstring STRINGID_SUNLIGHTSTRONG
	waitmessage 0x40
	playanimation BS_ATTACKER, B_ANIM_SUN_CONTINUES
	end2

BattleScript_SunlightFaded::
	printstring STRINGID_SUNLIGHTFADED
	waitmessage 0x40
	end2

BattleScript_FogContinues::
    printstring STRINGID_DEEPFOG
	waitmessage 0x40
	playanimation BS_ATTACKER, B_ANIM_FOG_CONTINUES
	end2

BattleScript_FogEnded::
    printstring STRINGID_FOGENDED
	waitmessage 0x40
	end2

BattleScript_OverworldWeatherStarts::
	printfromtable gWeatherContinuesStringIds
	waitmessage 0x40
	playanimation2 BS_ATTACKER, sB_ANIM_ARG1
	end3

BattleScript_SideStatusWoreOff::
	printstring STRINGID_PKMNSXWOREOFF
	waitmessage 0x40
	end2

BattleScript_SafeguardEnds::
	pause 0x20
	printstring STRINGID_PKMNSAFEGUARDEXPIRED
	waitmessage 0x40
	end2

BattleScript_LeechSeedTurnDrain::
	playanimation BS_ATTACKER, B_ANIM_LEECH_SEED_DRAIN, sB_ANIM_ARG1
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	copyword gBattleMoveDamage, gHpDealt
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	jumpifability BS_ATTACKER, ABILITY_LIQUID_OOZE, BattleScript_LeechSeedLiquidOoze
	manipulatedamage ATK80_DMG_CHANGE_SIGN
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNSAPPEDBYLEECHSEED
	waitmessage 0x40
	goto BattleScript_LeechSeedTurnUpdateHp

BattleScript_LeechSeedLiquidOoze::
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	loadabilitypopup BS_ATTACKER
	printstring STRINGID_ITSUCKEDLIQUIDOOZE
	waitmessage 0x40
	removeabilitypopup BS_ATTACKER
BattleScript_LeechSeedTurnUpdateHp::
	tryfaintmon BS_ATTACKER
	tryfaintmon BS_TARGET
	end2

BattleScript_MistProtected::
	pause 0x20
	printstring STRINGID_PKMNPROTECTEDBYMIST
	waitmessage 0x40
	return

BattleScript_SelectingDisabledMove::
	printselectionstring STRINGID_PKMNMOVEISDISABLED
	endselectionscript

BattleScript_DisabledNoMore::
	printstring STRINGID_PKMNMOVEDISABLEDNOMORE
	waitmessage 0x40
	end2

BattleScript_EncoredNoMore::
	printstring STRINGID_PKMNENCOREENDED
	waitmessage 0x40
	end2

BattleScript_DestinyBondTakesLife::
	printstring STRINGID_PKMNTOOKFOE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER
	return

BattleScript_SpikesOnAttacker::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	call BattleScript_PrintHurtBySpikes
	tryfaintmon BS_ATTACKER
	tryfaintonspikesdamage BS_ATTACKER, BattleScript_SpikesOnAttackerFainted
	return

BattleScript_SpikesOnAttackerFainted::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_ATTACKER
	moveendall
	goto BattleScript_HandleFaintedMon

BattleScript_SpikesOnTarget::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	call BattleScript_PrintHurtBySpikes
	tryfaintmon BS_TARGET
	tryfaintonspikesdamage BS_TARGET, BattleScript_SpikesOnTargetFainted
	return

BattleScript_SpikesOnTargetFainted::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_TARGET
	moveendall
	goto BattleScript_HandleFaintedMon

BattleScript_SpikesOnFaintedBattler::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_FAINTED
	datahpupdate BS_FAINTED
	call BattleScript_PrintHurtBySpikes
	tryfaintmon BS_FAINTED
	tryfaintonspikesdamage BS_FAINTED, BattleScript_SpikesOnFaintedBattlerFainted
	return

BattleScript_SpikesOnFaintedBattlerFainted::
	setbyte sGIVEEXP_STATE, 0
	getexp BS_FAINTED
	moveendall
	goto BattleScript_HandleFaintedMon

BattleScript_PrintHurtBySpikes::
	printstring STRINGID_PKMNHURTBYSPIKES
	waitmessage 0x40
	return

BattleScript_AllStatsUp::
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_ATK, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_DEF, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_SPEED, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_LESS_THAN, STAT_SPATK, 12, BattleScript_AllStatsUpAtk
	jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_SPDEF, 12, BattleScript_AllStatsUpRet
BattleScript_AllStatsUpAtk::
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_DEF | BIT_SPEED | BIT_SPATK | BIT_SPDEF, 0
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpDef::
	setstatchanger STAT_DEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpSpeed
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpSpeed::
	setstatchanger STAT_SPEED, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpSpAtk
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpSpAtk::
	setstatchanger STAT_SPATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpSpDef
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpSpDef::
	setstatchanger STAT_SPDEF, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_AllStatsUpRet
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_AllStatsUpRet::
	return

BattleScript_RapidSpinAway::
	rapidspinfree
	return

BattleScript_WrapFree::
	printstring STRINGID_PKMNGOTFREE
	waitmessage 0x40
	copybyte gBattlerTarget, sBATTLER
	return

BattleScript_LeechSeedFree::
	printstring STRINGID_PKMNSHEDLEECHSEED
	waitmessage 0x40
	return

BattleScript_SpikesFree::
	printstring STRINGID_PKMNBLEWAWAYSPIKES
	waitmessage 0x40
	return

BattleScript_NoMovesLeft::
	printselectionstring STRINGID_PKMNHASNOMOVESLEFT
	endselectionscript

BattleScript_SelectingMoveWithNoPP::
	printselectionstring STRINGID_NOPPLEFT
	endselectionscript

BattleScript_NoPPForMove::
	attackstring
	pause 0x20
	printstring STRINGID_BUTNOPPLEFT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SelectingTormentedMove::
	printselectionstring STRINGID_PKMNCANTUSEMOVETORMENT
	endselectionscript

	printstring STRINGID_PKMNCANTUSEMOVETORMENT
	waitmessage 0x40
	goto BattleScript_MoveEnd

BattleScript_SelectingNotAllowedMoveTaunt::
	printselectionstring STRINGID_PKMNCANTUSEMOVETAUNT
	endselectionscript

BattleScript_WishComesTrue::
	trywish 1, BattleScript_WishButFullHp
	playanimation BS_TARGET, B_ANIM_WISH_HEAL
	printstring STRINGID_PKMNWISHCAMETRUE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNREGAINEDHEALTH
	waitmessage 0x40
	end2

BattleScript_WishButFullHp::
	printstring STRINGID_PKMNWISHCAMETRUE
	waitmessage 0x40
	pause 0x20
	printstring STRINGID_PKMNHPFULL
	waitmessage 0x40
	end2

BattleScript_IngrainTurnHeal::
	playanimation BS_ATTACKER, B_ANIM_INGRAIN_HEAL
	printstring STRINGID_PKMNABSORBEDNUTRIENTS
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	end2

BattleScript_AtkDefDown::
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_ATK | BIT_DEF, ATK48_STAT_NEGATIVE | ATK48_ONLY_MULTIPLE | ATK48_DONT_CHECK_LOWER
	playstatchangeanimation BS_ATTACKER, BIT_ATK, ATK48_STAT_NEGATIVE | ATK48_DONT_CHECK_LOWER
	setstatchanger STAT_ATK, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, BattleScript_AtkDefDownAtkFail
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_AtkDefDownAtkFail
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_AtkDefDownAtkFail::
	playstatchangeanimation BS_ATTACKER, BIT_DEF, ATK48_STAT_NEGATIVE | ATK48_DONT_CHECK_LOWER
	setstatchanger STAT_DEF, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, BattleScript_AtkDefDownDefFail
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_AtkDefDownDefFail
	printfromtable gStatDownStringIds
	waitmessage 0x40
BattleScript_AtkDefDownDefFail::
	return

BattleScript_KnockedOff::
	playanimation BS_TARGET, B_ANIM_ITEM_KNOCKOFF
	printstring STRINGID_PKMNKNOCKEDOFF
	waitmessage 0x40
	return

BattleScript_SelectingImprisonedMove::
	printselectionstring STRINGID_PKMNCANTUSEMOVESEALED
	endselectionscript

BattleScript_GrudgeTakesPp::
	printstring STRINGID_PKMNLOSTPPGRUDGE
	waitmessage 0x40
	return

BattleScript_MagicCoatBounce::
	attackstring
	ppreduce
	pause 0x20
	printstring STRINGID_PKMNMOVEBOUNCED
	waitmessage 0x40
	orword gHitMarker, HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_ALLOW_NO_PP
	setmagiccoattarget BS_ATTACKER
	return

BattleScript_SnatchedMove::
	attackstring
	ppreduce
	snatchsetbattlers
	playanimation BS_TARGET, B_ANIM_SNATCH_MOVE
	printstring STRINGID_PKMNSNATCHEDMOVE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_ALLOW_NO_PP
	swapattackerwithtarget
	return

BattleScript_EnduredMsg::
	printstring STRINGID_PKMNENDUREDHIT
	waitmessage 0x40
	return

BattleScript_SturdiedMsg::
        loadabilitypopup BS_TARGET
        printstring STRINGID_PKMNENDUREDHIT
	waitmessage 0x40
	removeabilitypopup BS_TARGET
	return

BattleScript_OneHitKOMsg::
	printstring STRINGID_ONEHITKO
	waitmessage 0x40
	return

BattleScript_FocusPunchSetUp::
	flushmessagebox
	playanimation BS_ATTACKER, B_ANIM_FOCUS_PUNCH_SETUP
	printstring STRINGID_PKMNTIGHTENINGFOCUS
	waitmessage 0x40
	end2

BattleScript_MonWokeUpInUproar::
	printstring STRINGID_PKMNWOKEUPINUPROAR
	waitmessage 0x40
	updatestatusicon BS_ATTACKER
	end2

BattleScript_PoisonTurnDmg::
	printstring STRINGID_PKMNHURTBYPOISON
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_PSN_ANY
BattleScript_DoTurnDmg::
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	tryfaintmon BS_ATTACKER
	checkteamslot BattleScript_DoTurnDmgEnd
BattleScript_DoTurnDmgEnd::
	end2

BattleScript_BurnTurnDmg::
	printstring STRINGID_PKMNHURTBYBURN
	waitmessage B_WAIT_TIME_LONG
	chosenstatusanimation BS_ATTACKER, ID_STATUS1, STATUS1_BURN
	goto BattleScript_DoTurnDmg

BattleScript_DefrostedViaFireMove::
	printstring STRINGID_PKMNWASDEFROSTED
	waitmessage 0x40
	updatestatusicon BS_TARGET
	return

BattleScript_SteadfastSpeedUp::
        jumpifstat BS_ATTACKER, CMP_EQUAL, STAT_SPEED, 12, BattleScript_MoveEnd
	setbyte sSTAT_ANIM_PLAYED, 0
	playstatchangeanimation BS_ATTACKER, BIT_SPEED, 0
	setstatchanger STAT_SPEED, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_MoveEnd
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 2, BattleScript_MoveEnd
	loadabilitypopup BS_ATTACKER
	printstring STRINGID_ATKABLRAISEDITSSPEED
	waitmessage 0x40
	removeabilitypopup BS_ATTACKER
	goto BattleScript_MoveEnd

BattleScript_PrintUproarOverTurns::
	printfromtable gUproarOverTurnStringIds
	waitmessage 0x40
	end2

BattleScript_ThrashConfuses::
	chosenstatusanimation BS_ATTACKER, ID_STATUS2, STATUS2_CONFUSION
	printstring STRINGID_PKMNFATIGUECONFUSION
	waitmessage 0x40
	end2

BattleScript_PrintPayDayMoneyString::
	printstring STRINGID_PKMNPICKEDUPITEM
	waitmessage 0x40
	return

BattleScript_WrapTurnDmg::
	playanimation BS_ATTACKER, B_ANIM_TURN_TRAP, sB_ANIM_ARG1
	printstring STRINGID_PKMNHURTBY
	waitmessage 0x40
	goto BattleScript_DoTurnDmg

BattleScript_WrapEnds::
	printstring STRINGID_PKMNFREEDFROM
	waitmessage 0x40
	end2

BattleScript_NightmareTurnDmg::
	printstring STRINGID_PKMNLOCKEDINNIGHTMARE
	waitmessage 0x40
	chosenstatusanimation BS_ATTACKER, ID_STATUS2, STATUS2_NIGHTMARE
	goto BattleScript_DoTurnDmg

BattleScript_CurseTurnDmg::
	printstring STRINGID_PKMNAFFLICTEDBYCURSE
	waitmessage 0x40
	chosenstatusanimation BS_ATTACKER, ID_STATUS2, STATUS2_CURSED
	goto BattleScript_DoTurnDmg

BattleScript_TargetPRLZHeal::
	printstring STRINGID_PKMNHEALEDPARALYSIS
	waitmessage 0x40
	updatestatusicon BS_TARGET
	return

BattleScript_SilphScopeUnveiled::
	pause 0x20
	printstring STRINGID_SILPHSCOPEUNVEILED
	waitstate
	playanimation BS_OPPONENT1, B_ANIM_SILPH_SCOPED
	pause 0x20
	printstring STRINGID_GHOSTWASMAROWAK
	waitmessage 0x40
	end2

BattleScript_YawnMakesAsleep::
	chosenstatusanimation BS_EFFECT_BATTLER, ID_STATUS1, STATUS1_SLEEP
	printstring STRINGID_PKMNFELLASLEEP
	waitmessage 0x40
	updatestatusicon BS_EFFECT_BATTLER
	waitstate
	makevisible BS_EFFECT_BATTLER
	end2

BattleScript_MoveEffectUproar::
	printstring STRINGID_PKMNCAUSEDUPROAR
	waitmessage 0x40
	return

BattleScript_ItemSteal::
	playanimation BS_TARGET, B_ANIM_ITEM_STEAL
	printstring STRINGID_PKMNSTOLEITEM
	waitmessage 0x40
	return

BattleScript_SpeedBoostActivates::
    statbuffchange MOVE_EFFECT_AFFECTS_USER | STAT_CHANGE_BS_PTR, BattleScript_SpeedBoostActivatesEnd
    loadabilitypopup BS_ATTACKER
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printstring STRINGID_PKMNRAISEDSPEED
	waitmessage 0x40
	removeabilitypopup BS_ATTACKER
BattleScript_SpeedBoostActivatesEnd::
	end3

BattleScript_ActivateWeatherChangeAbilities::
	setbyte sBATTLER, 0
BattleScript_WeatherChangeAbilitiesLoop::
	activateweatherchangeabilities
	addbyte sBATTLER, 1
	jumpifbytenotequal sBATTLER, gBattlersCount, BattleScript_WeatherChangeAbilitiesLoop
	return

BattleScript_CastformChange::
	call BattleScript_DoCastformChangeAnim
	end3

BattleScript_DoCastformChangeAnim::
	loadabilitypopup BS_SCRIPTING
	playanimation BS_SCRIPTING, B_ANIM_FORM_CHANGE
	waitstate
	printstring STRINGID_PKMNTRANSFORMED
	waitmessage 0x40
	removeabilitypopup BS_SCRIPTING
	return

BattleScript_IntimidateActivates::
    pause 0x20
	setbyte gBattlerTarget, 0
BattleScript_IntimidateActivationLoop::
    jumpifabsent BS_TARGET, BattleScript_IntimidateIncrement
	jumpiftargetally BattleScript_IntimidateIncrement
    loadabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, TRUE
	jumpifstatus2 BS_TARGET, STATUS2_SUBSTITUTE, BattleScript_IntimidateIncrement
	jumpifability BS_TARGET, ABILITY_CLEAR_BODY, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_HYPER_CUTTER, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_WHITE_SMOKE, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_OBLIVIOUS, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_OWN_TEMPO, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_INNER_FOCUS, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_SCRAPPY, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_FULL_METAL_BODY, BattleScript_IntimidateAbilityFail
	jumpifability BS_TARGET, ABILITY_GUARD_DOG, BattleScript_GuardDogOnIntimidate
	setstatchanger STAT_ATK, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR | STAT_CHANGE_NOT_PROTECT_AFFECTED, BattleScript_IntimidateIncrement
	jumpifbyte CMP_GREATER_THAN, sMULTISTRING_CHOOSER, 1, BattleScript_IntimidateIncrement
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage 0x40
	jumpifstat BS_TARGET, CMP_EQUAL, STAT_SPEED, 12, BattleScript_IntimidateIncrement
	jumpifability BS_TARGET, ABILITY_RATTLED, BattleScript_RattledOnIntimidateActivates
BattleScript_IntimidateIncrement::
	addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_IntimidateActivationLoop
	removeabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, FALSE
	end3

BattleScript_RattledOnIntimidateActivates::
    setstatchanger STAT_SPEED, 1, FALSE
    call BattleScript_DefiantCompetitive
	goto BattleScript_IntimidateIncrement

BattleScript_IntimidateAbilityFail::
	pause 0x20
	loadabilitypopup BS_TARGET
	printstring STRINGID_PREVENTEDFROMWORKING
	waitmessage 0x40
	removeabilitypopup BS_TARGET
	recordlastability BS_TARGET
	goto BattleScript_IntimidateIncrement

BattleScript_GuardDogOnIntimidate::
    pause 0x20
	loadabilitypopup BS_TARGET
	setstatchanger STAT_ATK, 1, FALSE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_GuardDogOnIntimidateEnd
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_GuardDogOnIntimidateEnd::
	removeabilitypopup BS_TARGET
	recordlastability BS_TARGET
	goto BattleScript_IntimidateIncrement

BattleScript_TookAttack::
	attackstring
	pause 0x20
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNSXTOOKATTACK
	waitmessage 0x40
	removeabilitypopup BS_TARGET
	orword gHitMarker, HITMARKER_ATTACKSTRING_PRINTED
	return

BattleScript_MoveHPDrain_PPLoss::
	ppreduce
BattleScript_MoveHPDrain::
	attackstring
	pause 0x20
	loadabilitypopup BS_TARGET
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_TARGET
	datahpupdate BS_TARGET
	printstring STRINGID_PKMNRESTOREDHPUSING
	waitmessage 0x40
	removeabilitypopup BS_TARGET
	orhalfword gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	goto BattleScript_MoveEnd

BattleScript_MonMadeMoveUseless_PPLoss::
	ppreduce
BattleScript_MonMadeMoveUseless::
	attackstring
	pause 0x20
	loadabilitypopup BS_TARGET
	printstring STRINGID_PKMNSXMADEYUSELESS
	waitmessage 0x40
	removeabilitypopup BS_TARGET
	orhalfword gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	goto BattleScript_MoveEnd

BattleScript_AbilityNoStatLoss::
	pause 0x20
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNPREVENTSSTATLOSSWITH
	waitmessage 0x40
	removeabilitypopup BS_SCRIPTING
	return

BattleScript_ObliviousPrevents::
	pause 0x20
	loadabilitypopup BS_TARGET
	orhalfword gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	printstring STRINGID_ITDOESNTAFFECT
	waitmessage 0x40
	removeabilitypopup BS_TARGET
	goto BattleScript_MoveEnd

BattleScript_FlinchPrevention::
	pause 0x20
	loadabilitypopup BS_EFFECT_BATTLER
	printstring STRINGID_PKMNSXPREVENTSFLINCHING
	waitmessage 0x40
	removeabilitypopup BS_EFFECT_BATTLER
	goto BattleScript_MoveEnd

BattleScript_AbilityNoSpecificStatLoss::
	pause 0x20
	loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNSXPREVENTSYLOSS
	waitmessage 0x40
	removeabilitypopup BS_SCRIPTING
	setbyte sMULTISTRING_CHOOSER, 3
	return

BattleScript_AbilityCuredStatus::
    loadabilitypopup BS_SCRIPTING
	printstring STRINGID_PKMNSXCUREDITSYPROBLEM
	waitmessage 0x40
	removeabilitypopup BS_SCRIPTING
	updatestatusicon BS_SCRIPTING
	return

BattleScript_IgnoresWhileAsleep::
	printstring STRINGID_PKMNIGNORESASLEEP
	waitmessage 0x40
	moveendto ATK49_NEXT_TARGET
	end

BattleScript_IgnoresAndFallsAsleep::
	printstring STRINGID_PKMNBEGANTONAP
	waitmessage 0x40
	setmoveeffect MOVE_EFFECT_SLEEP, TRUE
	seteffectprimary
	moveendto ATK49_NEXT_TARGET
	end

BattleScript_IgnoresAndHitsItself::
	printstring STRINGID_PKMNWONTOBEY
	waitmessage 0x40
	goto BattleScript_DoSelfConfusionDmg

BattleScript_SubstituteFade::
	playanimation BS_TARGET, B_ANIM_SUBSTITUTE_FADE
	printstring STRINGID_PKMNSUBSTITUTEFADED
	return

BattleScript_BerryCurePrlzEnd2::
	call BattleScript_BerryCureParRet
	end2

BattleScript_BerryCureParRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMCUREDPARALYSIS
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	removeitem BS_SCRIPTING
	return

BattleScript_BerryCurePsnEnd2::
	call BattleScript_BerryCurePsnRet
	end2

BattleScript_BerryCurePsnRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMCUREDPOISON
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	removeitem BS_SCRIPTING
	return

BattleScript_BerryCureBrnEnd2::
	call BattleScript_BerryCureBrnRet
	end2

BattleScript_BerryCureBrnRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMHEALEDBURN
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	removeitem BS_SCRIPTING
	return

BattleScript_BerryCureFrzEnd2::
	call BattleScript_BerryCureFrzRet
	end2

BattleScript_BerryCureFrzRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMDEFROSTEDIT
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	removeitem BS_SCRIPTING
	return

BattleScript_BerryCureSlpEnd2::
	call BattleScript_BerryCureSlpRet
	end2

BattleScript_BerryCureSlpRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMWOKEIT
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	removeitem BS_SCRIPTING
	return

BattleScript_BerryCureConfusionEnd2::
	call BattleScript_BerryCureConfusionRet
	end2

BattleScript_BerryCureConfusionRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMSNAPPEDOUT
	waitmessage 0x40
	removeitem BS_SCRIPTING
	return

BattleScript_BerryCureChosenStatusEnd2::
	call BattleScript_BerryCureChosenStatusRet
	end2

BattleScript_BerryCureChosenStatusRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printfromtable gBerryEffectStringIds
	waitmessage 0x40
	updatestatusicon BS_SCRIPTING
	removeitem BS_SCRIPTING
	return

BattleScript_WhiteHerbEnd2::
	call BattleScript_WhiteHerbRet
	end2

BattleScript_WhiteHerbRet::
	playanimation BS_SCRIPTING, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMRESTOREDSTATUS
	waitmessage 0x40
	removeitem BS_SCRIPTING
	return

BattleScript_ItemHealHP_RemoveItem::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMRESTOREDHEALTH
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	removeitem BS_ATTACKER
	end2

BattleScript_BerryPPHealEnd2::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMRESTOREDPP
	waitmessage 0x40
	removeitem BS_ATTACKER
	end2

BattleScript_ItemHealHP_End2::
	call BattleScript_ItemHealHP_Ret
	end2

BattleScript_ItemHealHP_Ret::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMRESTOREDHPALITTLE
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	return

BattleScript_SelectingNotAllowedMoveChoiceItem::
	printselectionstring STRINGID_ITEMALLOWSONLYYMOVE
	endselectionscript

BattleScript_HangedOnMsg::
	playanimation BS_TARGET, B_ANIM_HANGED_ON
	printstring STRINGID_PKMNHUNGONWITHX
	waitmessage 0x40
	return

BattleScript_BerryConfuseHealEnd2::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNSITEMRESTOREDHEALTH
	waitmessage 0x40
	orword gHitMarker, HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE
	healthbarupdate BS_ATTACKER
	datahpupdate BS_ATTACKER
	printstring STRINGID_FORXCOMMAYZ
	waitmessage 0x40
	setmoveeffect MOVE_EFFECT_CONFUSION, TRUE
	seteffectprimary
	removeitem BS_ATTACKER
	end2

BattleScript_BerryStatRaiseEnd2::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT
	statbuffchange STAT_CHANGE_BS_PTR | MOVE_EFFECT_AFFECTS_USER, BattleScript_BerryStatRaiseDoStatUp
BattleScript_BerryStatRaiseDoStatUp::
	setbyte sMULTISTRING_CHOOSER, 4
	call BattleScript_StatUp
	removeitem BS_ATTACKER
	end2

BattleScript_BerryFocusEnergyEnd2::
	playanimation BS_ATTACKER, B_ANIM_ITEM_EFFECT
	printstring STRINGID_PKMNUSEDXTOGETPUMPED
	waitmessage 0x40
	removeitem BS_ATTACKER
	end2

BattleScript_ActionSelectionItemsCantBeUsed::
	printselectionstring STRINGID_ITEMSCANTBEUSEDNOW
	endselectionscript

BattleScript_TrainerSlideMsgEnd2::
       call BattleScript_TrainerSlideMsg
       end2

BattleScript_TrainerSlideMsg::
    handletrainerslidecase BS_SCRIPTING, ATK83_TRAINER_SLIDE_CASE_SAVE_SPRITES
	handletrainerslidecase B_POSITION_OPPONENT_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_IN
	waitstate
	handletrainerslidecase BS_SCRIPTING, ATK83_TRAINER_SLIDE_CASE_PRINT_STRING
	waitmessage 0x40
	handletrainerslidecase B_POSITION_OPPONENT_LEFT, ATK83_TRAINER_SLIDE_CASE_SLIDE_OUT
	waitstate
	handletrainerslidecase BS_SCRIPTING, ATK83_TRAINER_SLIDE_CASE_RESTORE_SPRITES
	return

BattleScript_AngerPointActivation::
        setstatchanger STAT_ATK, 1, FALSE
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	loadabilitypopup BS_TARGET
        printstring STRINGID_DEFMAXEDATTACK
        waitmessage 0x40
	removeabilitypopup BS_TARGET
        return
	
BattleScript_MoveStatRaise_PPLoss::
    ppreduce
BattleScript_MoveStatRaise::
    attackstring
	pause 0x20
	loadabilitypopup BS_TARGET
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	waitstate
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_MoveStatRaiseEnd
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_MoveStatRaiseEnd::
	removeabilitypopup BS_TARGET
	orhalfword gMoveResultFlags, MOVE_RESULT_DOESNT_AFFECT_FOE
	goto BattleScript_MoveEnd

BattleScript_DefiantCompetitive::
    pause 0x20
	loadabilitypopup BS_TARGET
	statbuffchange 0, NULL
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printstring STRINGID_PKMNSSTATCHANGED2
    waitmessage 0x40
	removeabilitypopup BS_TARGET
	return

BattleScript_WeakArmorActivation::
        loadabilitypopup BS_TARGET
	setstatchanger STAT_DEF, 1, TRUE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_WeakArmorRaisesSpeed
	jumpifbyte CMP_LESS_THAN, sMULTISTRING_CHOOSER, 2, BattleScript_WeakArmorDefAnim
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 3, BattleScript_WeakArmorRaisesSpeed
	pause 0x20
	goto BattleScript_WeakArmorDefDownPrintString
	
BattleScript_WeakArmorDefAnim::
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
BattleScript_WeakArmorDefDownPrintString::
	printfromtable gStatDownStringIds
	waitmessage 0x40
	
BattleScript_WeakArmorRaisesSpeed::
	setstatchanger STAT_SPEED, 2, FALSE
	statbuffchange STAT_CHANGE_BS_PTR, BattleScript_WeakArmorReturn
	jumpifbyte CMP_LESS_THAN, sMULTISTRING_CHOOSER, 2, BattleScript_WeakArmorSpeedAnim
	jumpifbyte CMP_EQUAL, sMULTISTRING_CHOOSER, 3, BattleScript_WeakArmorReturn
	pause 0x20
	goto BattleScript_WeakArmorSpeedUpPrintString
	
BattleScript_WeakArmorSpeedAnim::
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
BattleScript_WeakArmorSpeedUpPrintString::
	printfromtable gStatUpStringIds
	waitmessage 0x40
BattleScript_WeakArmorReturn::
	removeabilitypopup BS_TARGET
	return
	
BattleScript_MoodyActivates::
        loadabilitypopup BS_ATTACKER
	jumpifbyte CMP_EQUAL, sSTATCHANGER, 0, BattleScript_MoodyLower
	statbuffchange MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN | STAT_CHANGE_NOT_PROTECT_AFFECTED, BattleScript_MoodyLower
	jumpifbyte CMP_GREATER_THAN, sMULTISTRING_CHOOSER, 1, BattleScript_MoodyLower
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatUpStringIds
	waitmessage 0x40
	
BattleScript_MoodyLower::
        jumpifbyte CMP_EQUAL, sGIVEEXP_STATE, 0, BattleScript_MoodyEnd
	copybyte sSTATCHANGER, sGIVEEXP_STATE
	statbuffchange MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN | STAT_CHANGE_NOT_PROTECT_AFFECTED, BattleScript_MoodyEnd
	jumpifbyte CMP_GREATER_THAN, sMULTISTRING_CHOOSER, 1, BattleScript_MoodyEnd
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage 0x40
	
BattleScript_MoodyEnd::
        removeabilitypopup BS_ATTACKER
        end3

BattleScript_RaiseStatOnFaintingTarget::
    copybyte sSAVED_DMG, gBattlerAttacker
	copybyte gBattlerAttacker, sBATTLER
    loadabilitypopup BS_ATTACKER
    statbuffchange MOVE_EFFECT_AFFECTS_USER | STAT_CHANGE_BS_PTR, BattleScript_RaiseStatOnFaintingTarget_End
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	waitstate
	printstring STRINGID_ATTACKERABILITYSTATRAISE
	waitmessage 0x40
	removeabilitypopup BS_ATTACKER
BattleScript_RaiseStatOnFaintingTarget_End::
    copybyte gBattlerAttacker, sSAVED_DMG
	return

BattleScript_TargetAbilityStatRaiseRet::
        copybyte gBattlerAttacker, gBattlerTarget
	loadabilitypopup BS_TARGET
	statbuffchange MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, BattleScript_TargetAbilityStatRaiseRet_End
	setgraphicalstatchangevalues
	call BattleScript_StatUp
BattleScript_TargetAbilityStatRaiseRet_End::
        removeabilitypopup BS_TARGET
	return

BattleScript_AttackerAbilityStatRaiseEnd3::
    pause 0x20
	statbuffchange MOVE_EFFECT_AFFECTS_USER | STAT_CHANGE_BS_PTR, BattleScript_AttackerAbilityStatRaise_End
    loadabilitypopup BS_ATTACKER
	setgraphicalstatchangevalues
	playanimation BS_ATTACKER, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	waitstate
	printstring STRINGID_ATTACKERABILITYSTATRAISE
	waitmessage 0x40
	removeabilitypopup BS_ATTACKER
BattleScript_AttackerAbilityStatRaise_End::
	end3

BattleScript_CottonDownActivates::
	copybyte sBATTLER, gBattlerTarget @save target in somewere
	setbyte gBattlerTarget, 0
	setstatchanger STAT_SPEED, 1, TRUE
BattleScript_CottonDownLoop::
    loadabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, TRUE
    jumpifabsent BS_TARGET, BattleScript_CottonDownIncrement
	jumpifbyteequal gBattlerTarget, gBattlerAttacker, BattleScript_CottonDownIncrement
	statbuffchange STAT_CHANGE_NOT_PROTECT_AFFECTED, BattleScript_CottonDownTargetSpeedCantGoLower
	setgraphicalstatchangevalues
	playanimation BS_TARGET, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	printfromtable gStatDownStringIds
	waitmessage 0x40
	goto BattleScript_CottonDownIncrement
BattleScript_CottonDownTargetSpeedCantGoLower::
    printstring STRINGID_STATSWONTDECREASE
	waitmessage 0x40
BattleScript_CottonDownIncrement::
    addbyte gBattlerTarget, 1
	jumpifbytenotequal gBattlerTarget, gBattlersCount, BattleScript_CottonDownLoop
	removeabilitypopup BS_ATTACKER
	setbyte sFIXED_ABILITY_POPUP, FALSE
	copybyte gBattlerTarget, sBATTLER
	return

BattleScript_BattlerAbilityStatRaiseOnSwitchIn::
    pause 0x20
	loadabilitypopup BS_SCRIPTING
	statbuffchange MOVE_EFFECT_AFFECTS_USER | STAT_CHANGE_NOT_PROTECT_AFFECTED | MOVE_EFFECT_CERTAIN, NULL
	setgraphicalstatchangevalues
	playanimation BS_SCRIPTING, B_ANIM_STATS_CHANGE, sB_ANIM_ARG1
	waitstate
	printstring STRINGID_ATTACKERABILITYSTATRAISE
	waitmessage 0x40
	removeabilitypopup BS_SCRIPTING
	end3
