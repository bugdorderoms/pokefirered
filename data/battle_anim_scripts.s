#include "constants/battle.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"
#include "constants/sound.h"
#include "constants/moves.h"
#include "constants/rgb.h"
	.include "asm/macros.inc"
	.include "asm/macros/battle_anim_script.inc"
	.include "constants/constants.inc"

	.section script_data, "aw", %progbits

	.align 2

@@@@@@@@@@@@@
@ UTILITIES @
@@@@@@@@@@@@@
	
InfatuationHearts::
	playsewithpan SE_M_CHARM, SOUND_PAN_ATTACKER
	createsprite gMagentaHeartSpriteTemplate, ANIM_ATTACKER, 3, 0, 20
	delay 15
	playsewithpan SE_M_CHARM, SOUND_PAN_ATTACKER
	createsprite gMagentaHeartSpriteTemplate, ANIM_ATTACKER, 3, -20, 20
	delay 15
	playsewithpan SE_M_CHARM, SOUND_PAN_ATTACKER
	createsprite gMagentaHeartSpriteTemplate, ANIM_ATTACKER, 3, 20, 20
	return

ElectricityEffect::
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_TARGET
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 5, 0, 5, 0
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -5, 10, 5, 1
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 15, 20, 5, 2
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -15, -10, 5, 0
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 25, 0, 5, 1
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -8, 8, 5, 2
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 2, -8, 5, 0
	delay 2
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -20, 15, 5, 1
	return

FireSpreadEffect::
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, 192, 176, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, -192, 240, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, 192, -160, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, -192, -112, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, 160, 48, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, -224, -32, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 1, 0, 10, 112, -128, 40
	return

FireSpreadEffectLong::
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 192, 76, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -192, 76, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 20, 140, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -20, -140, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 192, -160, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -192, -160, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 82, -112, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -82, -112, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 160, -52, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -160, -52, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 224, -32, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -224, -32, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, 112, -128, 40
	createsprite gFireSpreadSpriteTemplate, ANIM_TARGET, 5, 0, 0, -112, -128, 40
	return

IceCrystalEffectShort::
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, -10, -10, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 10, 20, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, -5, 10, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 17, -12, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, -15, 15, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 0, 0, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, 20, 2, FALSE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	return

IceCrystalEffectLong::
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, -10, -10, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 10, 20, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, -29, 0, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 29, -20, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, -5, 10, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 17, -12, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, -20, 0, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, -15, 15, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 26, -5, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitSmallSpriteTemplate, ANIM_TARGET, 2, 0, 0, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gIceCrystalHitLargeSpriteTemplate, ANIM_TARGET, 2, 20, 2, TRUE
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	return

ScatterFeathers::
	createsprite gScatterFeatherSpriteTemplate, ANIM_TARGET, 1, -8, -8, 160, -32
	createsprite gScatterFeatherSpriteTemplate, ANIM_TARGET, 1, -8, -8, -256, -40
	createsprite gScatterFeatherSpriteTemplate, ANIM_TARGET, 1, -8, -8, 416, -38
	createsprite gScatterFeatherSpriteTemplate, ANIM_TARGET, 1, -8, -8, -384, -31
	return

ScatterLeafs::
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_TARGET, 10, 5
	createsprite gScatterLeafSpriteTemplate, ANIM_TARGET, 1, -8, -8, 160, -32
	createsprite gScatterLeafSpriteTemplate, ANIM_TARGET, 1, -8, -8, -256, -40
	createsprite gScatterLeafSpriteTemplate, ANIM_TARGET, 1, -8, -8, 128, -16
	createsprite gScatterLeafSpriteTemplate, ANIM_TARGET, 1, -8, -8, 416, -38
	createsprite gScatterLeafSpriteTemplate, ANIM_TARGET, 1, -8, -8, -128, -22
	createsprite gScatterLeafSpriteTemplate, ANIM_TARGET, 1, -8, -8, -384, -31
	return

PoisonBubblesEffect::
	createsprite gPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 10, 10, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 6
	createsprite gPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 20, -20, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 6
	createsprite gPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -20, 15, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 6
	createsprite gPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 0, 0, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 6
	createsprite gPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -20, -20, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 6
	createsprite gPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 16, -8, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	return

RoarEffect::
	createsprite gRoarNoiseLineSpriteTemplate, ANIM_ATTACKER, 2, 24, -8, 0
	createsprite gRoarNoiseLineSpriteTemplate, ANIM_ATTACKER, 2, 24, 0, 2
	createsprite gRoarNoiseLineSpriteTemplate, ANIM_ATTACKER, 2, 24, 8, 1
	delay 15
	createsprite gRoarNoiseLineSpriteTemplate, ANIM_ATTACKER, 2, 24, -8, 0
	createsprite gRoarNoiseLineSpriteTemplate, ANIM_ATTACKER, 2, 24, 0, 2
	createsprite gRoarNoiseLineSpriteTemplate, ANIM_ATTACKER, 2, 24, 8, 1
	return

SetPsychicBackground::
	fadetobg BG_PSYCHIC
	waitbgfadeout
	createvisualtask AnimTask_SetPsychicBackground, 5
	waitbgfadein
	return

SetSkyBg::
	fadetobg BG_SKY
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -2304, 768, TRUE
	waitbgfadein
	return

SetUpDrillBackground::
	fadetobg BG_DRILL
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -2304, 768, TRUE
	waitbgfadein
	return

SetHighSpeedBackground::
	setbgbasedonside ANIM_TARGET, BG_HIGHSPEED_PLAYER, BG_HIGHSPEED_OPPONENT, TRUE
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -2304, 0, TRUE
	waitbgfadein
	return

SetFissureBackground::
	fadetobg BG_FISSURE
	waitbgfadeout
	createvisualtask AnimTask_PositionFissureBgOnBattler, 5, 1, 5
	waitbgfadein
	return

UnsetScrollingBg::
	restorebg
	waitbgfadeout
	setargret -1
	waitbgfadein
	return

WaterBubblesEffectShort::
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 10, 10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 20, -20, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, -20, 15, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, -20, -20, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 16, -8, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	return

WaterBubblesEffectLong::
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 10, 10, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, -28, -10, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 20, -20, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, -20, 15, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 27, 8, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, -20, -20, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 6
	createsprite gWaterBubbleSpriteTemplate, ANIM_ATTACKER, 2, 16, -8, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	return

HealingEffect::
	playsewithpan SE_M_ABSORB_2, SOUND_PAN_ATTACKER
	createsprite gHealingBlueStarSpriteTemplate, ANIM_ATTACKER, 2, 0, -5, ANIM_ATTACKER, TRUE
	delay 7
	createsprite gHealingBlueStarSpriteTemplate, ANIM_ATTACKER, 2, -15, 10, ANIM_ATTACKER, TRUE
	delay 7
	createsprite gHealingBlueStarSpriteTemplate, ANIM_ATTACKER, 2, -15, -15, ANIM_ATTACKER, TRUE
	delay 7
	createsprite gHealingBlueStarSpriteTemplate, ANIM_ATTACKER, 2, 10, -5, ANIM_ATTACKER, TRUE
	delay 7
	return

HealingEffectOnTarget::
	playsewithpan SE_M_ABSORB_2, SOUND_PAN_TARGET
	createsprite gHealingBlueStarSpriteTemplate, ANIM_TARGET, 2, 0, -5, ANIM_TARGET, TRUE
	delay 7
	createsprite gHealingBlueStarSpriteTemplate, ANIM_TARGET, 2, -15, 10, ANIM_TARGET, TRUE
	delay 7
	createsprite gHealingBlueStarSpriteTemplate, ANIM_TARGET, 2, -15, -15, ANIM_TARGET, TRUE
	delay 7
	createsprite gHealingBlueStarSpriteTemplate, ANIM_TARGET, 2, 10, -5, ANIM_TARGET, TRUE
	delay 7
	return

GrantingStarsEffect::
	createsprite gGrantingStarsSpriteTemplate, ANIM_ATTACKER, 2, -15, 0, ANIM_ATTACKER, 0, 32, 60
	delay 8
	createsprite gGrantingStarsSpriteTemplate, ANIM_ATTACKER, 2, 12, -5, ANIM_ATTACKER, 0, 32, 60
	delay 8
	return

SmallWaterBubblesOnAttacker::
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 10, 10, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, -15, 0, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 20, 10, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 0, -10, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, -10, 15, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 25, 20, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, -20, 20, 25, ANIM_ATTACKER
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 12, 0, 25, ANIM_ATTACKER
	delay 4
	return

WebThreadSingle::
	createsprite gWebThreadSpriteTemplate, ANIM_TARGET, 2, 20, 0, 512, 20, FALSE
	delay 1
	return

WebThreadDoubles::
	createsprite gWebThreadSpriteTemplate, ANIM_TARGET, 2, 20, 0, 512, 20, TRUE
	delay 1
	return

EyeSparkles::
	createsprite gEyeSparkleSpriteTemplate, ANIM_ATTACKER, 0, -16, -8
	createsprite gEyeSparkleSpriteTemplate, ANIM_ATTACKER, 0, 16, -8
	return

IceSpikesEffectLong::
	loopsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET, 6, 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 24, TRUE
	delay 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 8, 24, TRUE
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, -8, 24, TRUE
	delay 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 16, 24, TRUE
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, -16, 24, TRUE
	delay 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 24, 24, TRUE
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, -24, 24, TRUE
	delay 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 32, 24, TRUE
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, -32, 24, TRUE
	delay 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 40, 24, TRUE
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, -40, 24, TRUE
	delay 4
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, 48, 24, TRUE
	createsprite gIceGroundSpikeSpriteTemplate, ANIM_TARGET, 2, -48, 24, TRUE
	return

@@@@@@@@@@@@@@@@@@@@@
@ STATUS ANIMATIONS @
@@@@@@@@@@@@@@@@@@@@@

gStatusAnim_Poison::
	loopsewithpan SE_M_TOXIC, SOUND_PAN_ATTACKER, 13, 6
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 18, 2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 12, RGB(30, 0, 31) @ Purple color
	end

gStatusAnim_Confusion::
	loadspritegfx ANIM_TAG_DUCK
	loopsewithpan SE_M_DIZZY_PUNCH, SOUND_PAN_ATTACKER, 13, 6
	createsprite gConfusionDuckSpriteTemplate, ANIM_ATTACKER, 2, 0, -15, 0, 3, 90
	createsprite gConfusionDuckSpriteTemplate, ANIM_ATTACKER, 2, 0, -15, 51, 3, 90
	createsprite gConfusionDuckSpriteTemplate, ANIM_ATTACKER, 2, 0, -15, 102, 3, 90
	createsprite gConfusionDuckSpriteTemplate, ANIM_ATTACKER, 2, 0, -15, 153, 3, 90
	createsprite gConfusionDuckSpriteTemplate, ANIM_ATTACKER, 2, 0, -15, 204, 3, 90
	end

gStatusAnim_Burn::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	call BurnFlame
	call BurnFlame
	call BurnFlame
	end

BurnFlame::
	createsprite gBurnFlameSpriteTemplate, ANIM_ATTACKER, 2, 24, 24, -24, 24, 20, ANIM_ATTACKER, TRUE
	delay 4
	return

gStatusAnim_Infatuation::
	loadspritegfx ANIM_TAG_MAGENTA_HEART
	call InfatuationHearts
	end

gStatusAnim_Sleep::
	loadspritegfx ANIM_TAG_LETTER_Z
	playsewithpan SE_M_SNORE, SOUND_PAN_ATTACKER
	createsprite gSleepLetterZSpriteTemplate, ANIM_ATTACKER, 2, 4, -10
	delay 30
	createsprite gSleepLetterZSpriteTemplate, ANIM_ATTACKER, 2, 4, -10
	end

gStatusAnim_Paralysis::
	loadspritegfx ANIM_TAG_SPARK_2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 10, 1
	call ElectricityEffect
	end

gStatusAnim_Freeze::
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_ABOVE
	loadspritegfx ANIM_TAG_ICE_CUBE
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	waitplaysewithpan SE_M_HAIL, SOUND_PAN_ATTACKER, 17
	createvisualtask AnimTask_FrozenIceCube, 2, ANIM_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	end

@ Credits: Blackuser
gStatusAnim_Curse::
	loadspritegfx ANIM_TAG_NAIL
	monbg ANIM_ATK_SIDE
	createsprite gCurseNailSpriteTemplate, ANIM_ATTACKER, 2, 0, 0
	delay 60
	call CurseGhostShakeFromNail
	delay 41
	call CurseGhostShakeFromNail
	delay 41
	call CurseGhostShakeFromNail
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	end

gStatusAnim_Nightmare::
	loadspritegfx ANIM_TAG_DEVIL
	monbg ANIM_ATK_SIDE
	playsewithpan SE_M_NIGHTMARE, SOUND_PAN_ATTACKER
	createsprite gNightmareDevilSpriteTemplate, ANIM_ATTACKER, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 2, 0, 14, 1
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	end

@@@@@@@@@@@@@@@@@@@@@@
@ GENERAL ANIMATIONS @
@@@@@@@@@@@@@@@@@@@@@@

gGeneralAnim_FormChange::
	monbg ANIM_ATTACKER
	playsewithpan SE_M_TELEPORT, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_MINIMIZE, SOUND_PAN_ATTACKER, 48
	createvisualtask AnimTask_TransformMon, 2, ANIM_ATTACKER, SPECIESGFX_FLAG_NO_TRANSFORM_PAL_FADE
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

gGeneralAnim_StatsChange::
	createvisualtask AnimTask_StatsChange, 5
	end

gGeneralAnim_SubstituteFade::
	monbg ANIM_ATTACKER
	createvisualtask AnimTask_SubstituteFadeToInvisible, 5
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 0, 0, 16, RGB_WHITE
	waitforvisualfinish
	delay 1
	clearmonbg ANIM_ATTACKER
	delay 2
	blendoff
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 0, 0, 0, RGB_WHITE
	goto gSpecialAnim_SubstituteToMon

gGeneralAnim_SubstituteAppear::
	createvisualtask AnimTask_MonToSubstitute, 2
	end

gGeneralAnim_BaitThrow::
	createvisualtask AnimTask_SafariOrGhost_DecideBattlers, 2, 0 @ Attacker = Player left, target = Opponent left
	createvisualtask AnimTask_LoadOrFreeBaitGfx, 2, TRUE @ Load bait gfx
	waitplaysewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER, 22
	createsprite gSafariBaitSpriteTemplate, ANIM_TARGET, 3, -18, 12, 0, 32
	delay 50
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_TARGET, 19, 2
	createvisualtask AnimTask_SwayMon, 5, 1, 8, 1536, 2, ANIM_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_LoadOrFreeBaitGfx, 2, FALSE @ Free bait gfx
	end

@ Credits: Blackuser
gGeneralAnim_ItemKnockOff::
	createsprite gKnockOffItemSpriteTemplate, ANIM_TARGET, 2
	end

gGeneralAnim_TurnTrap::
	createvisualtask AnimTask_GetTrappedMoveAnimId, 5
	jumpreteq TRAP_ID_FIRE_SPIN, General_FireSpin
	jumpreteq TRAP_ID_WHIRLPOOL, General_Whirlpool
	jumpreteq TRAP_ID_CLAMP,     gMoveAnim_CLAMP
	jumpreteq TRAP_ID_SAND_TOMB, General_SandTomb

General_BindWrap::
	loadspritegfx ANIM_TAG_TENDRILS
	loopsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET, 6, 2
	createsprite gConstrictBindingSpriteTemplate, ANIM_TARGET, 4, 0, 16, 0, 1
	delay 7
	createsprite gConstrictBindingSpriteTemplate, ANIM_TARGET, 2, 0, 8, 1, 1
	delay 3
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	delay 20
	setargret -1 @ Signal given to advance the anim to the next step
	playsewithpan SE_M_BIND, SOUND_PAN_TARGET
	end

General_FireSpin::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	playsewithpan SE_M_SACRED_FIRE2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 2, 30, 1
	call FireSpinEffect
	call FireSpinEffect
	waitforvisualfinish
	stopsound
	end

General_Whirlpool::
	loadspritegfx ANIM_TAG_WATER_ORB
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 0, 7, RGB(0, 13, 23) @ Blue color
	playsewithpan SE_M_WHIRLPOOL, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 2, 30, 1
	call WhirlpoolEffect
	call WhirlpoolEffect
	delay 12
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 7, 0, RGB(0, 13, 23) @ Blue color
	waitforvisualfinish
	stopsound
	clearmonbg ANIM_DEF_SIDE
	end

General_SandTomb::
	loadspritegfx ANIM_TAG_MUD_SAND
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 0, 7, RGB(19, 17, 0) @ Brown color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 2, 30, 1
	playsewithpan SE_M_SAND_TOMB, SOUND_PAN_TARGET
	call SandTombSwirlingDirt
	call SandTombSwirlingDirt
	delay 22
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 7, 0, RGB(19, 17, 0) @ Brown color
	waitforvisualfinish
	stopsound
	end

@ Credits: Skeli
gGeneralAnim_ItemEffect::
	loadspritegfx ANIM_TAG_THIN_RING
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 2, 0, 9, RGB_WHITE
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 16, 128, ANIM_ATTACKER, 2
	waitforvisualfinish
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 16, 128, ANIM_ATTACKER, 2
	waitforvisualfinish
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 3, 7, 0, RGB(17, 31, 25) @ Some fade color
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE
	end

gGeneralAnim_SmokeballEscape::
	loadspritegfx ANIM_TAG_PINK_CLOUD
	monbg ANIM_ATTACKER
	setalpha 12, 4
	playsewithpan SE_BALL_OPEN, SOUND_PAN_ATTACKER
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_TARGET, 0, 0, 32, 28, 30
	delay 4
	playsewithpan SE_BALL_OPEN, SOUND_PAN_ATTACKER
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 127, 2, 12, 20, 30
	delay 12
	playsewithpan SE_BALL_OPEN, SOUND_PAN_ATTACKER
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 126, 2, -28, 4, 30
	delay 12
	playsewithpan SE_BALL_OPEN, SOUND_PAN_ATTACKER
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 124, 2, 14, -20, 30
	delay 4
	playsewithpan SE_BALL_OPEN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_AttackerFadeToInvisible, 2, 2
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 123, 3, 4, 4, 30
	delay 14
	playsewithpan SE_BALL_OPEN, SOUND_PAN_ATTACKER
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 122, 3, -14, 18, 46
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 121, 3, 14, -14, 46
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 120, 3, -12, -10, 46
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 119, 3, 14, 14, 46
	createsprite gSmokeBallEscapeCloudSpriteTemplate, ANIM_ATTACKER, 118, 3, 0, 0, 46
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	invisible ANIM_ATTACKER
	blendoff
	end

gGeneralAnim_HangedOn::
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_ATTACKER, 7, 0, 9, RGB_RED
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_SlideMonForFocusBand, 5, 30, 128, 0, TRUE, 2, 0, 1
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_ATTACKER, 4, 9, 0, RGB_RED
	waitforvisualfinish
	delay 6
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 0, ANIM_ATTACKER, 0, 15
	end

gGeneralAnim_Rain::
	loadspritegfx ANIM_TAG_RAIN_DROPS
	playsewithpan SE_M_RAIN_DANCE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 13, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_CreateRaindrops, 2, 0, 3, 60
	createvisualtask AnimTask_CreateRaindrops, 2, 0, 3, 60
	delay 50
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 13, 0, RGB_BLACK
	end

gGeneralAnim_LeechSeedDrain::
	createvisualtask AnimTask_GetBattlersFromArg, 5
	goto gMoveAnim_ABSORB

@ Credits: Blackuser
gGeneralAnim_ItemSteal::
	createvisualtask AnimTask_GetBattlersFromArg, 5
	createsprite gItemStealSpriteTemplate, ANIM_ATTACKER, 2, 0, -5
	end

gGeneralAnim_SnatchMove::
	createvisualtask AnimTask_SetAnimAttackerAndTargetForEffectTgt, 2
	call SnatchMoveTrySwapFromSubstitute
	delay 1
	createvisualtask AnimTask_SwayMon, 2, 0, 5, 5120, 4, ANIM_TARGET
	waitforvisualfinish
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_SnatchMonMove, 2
	waitforvisualfinish
	call SnatchMoveTrySwapToSubstitute
	end

SnatchMoveTrySwapFromSubstitute::
	createvisualtask AnimTask_IsAttackerBehindSubstitute, 2
	jumprettrue SnatchMoveSwapSubstituteForMon
SnatchMoveTrySwapFromSubstituteEnd::
	return

SnatchMoveSwapSubstituteForMon::
	createvisualtask AnimTask_SwapMonSpriteToFromSubstitute, 2, TRUE
	waitforvisualfinish
	goto SnatchMoveTrySwapFromSubstituteEnd

SnatchMoveTrySwapToSubstitute::
	createvisualtask AnimTask_IsAttackerBehindSubstitute, 2
	jumprettrue SnatchMoveSwapMonForSubstitute
SnatchMoveTrySwapToSubstituteEnd::
	return

SnatchMoveSwapMonForSubstitute::
	createvisualtask AnimTask_SwapMonSpriteToFromSubstitute, 2, FALSE
	waitforvisualfinish
	goto SnatchMoveTrySwapToSubstituteEnd

gGeneralAnim_FutureSightHit::
	createvisualtask AnimTask_SetAnimTargetFromArg, 2
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	call SetPsychicBackground
	setalpha 8, 8
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_TARGET
	waitplaysewithpan SE_M_SUPERSONIC, SOUND_PAN_TARGET, 8
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 4, 0, 15, 1
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -5, -5, 15, ANIM_TARGET, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 4, 0, 24, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

gGeneralAnim_DoomDesireHit::
	loadspritegfx ANIM_TAG_EXPLOSION
	createvisualtask AnimTask_SetAnimTargetFromArg, 2
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 0, 16, RGB_WHITE
	waitforvisualfinish
	delay 10
	createvisualtask AnimTask_DoomDesireLightBeam, 5
	delay 9
	playsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_ATTACKER
	delay 9
	playsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_ABOVE
	delay 9
	playsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_TARGET
	delay 25
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 10, 0, 20, 1
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 24, -24, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -16, 16, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -24, -12, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 16, 16, ANIM_TARGET, FALSE
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 16, 0, RGB_WHITE
	end

gGeneralAnim_WishHeal::
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_SPARKLE_2
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 0, 10, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER
	call GrantingStarsEffect
	waitforvisualfinish
	call HealingEffect
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 10, 0, RGB_BLACK
	end

gGeneralAnim_MonScared::
	loadspritegfx ANIM_TAG_SWEAT_BEAD
	createvisualtask AnimTask_SafariOrGhost_DecideBattlers, 2, 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 0, 10, RGB(0, 23, 25) @ Deep blue color
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 10, 1
	delay 20
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, FALSE, ANIM_TARGET
	playsewithpan SE_M_SKETCH, SOUND_PAN_TARGET
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, TRUE, ANIM_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 5, 1
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_TARGET
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 10, 0, RGB(0, 23, 25) @ Deep blue color
	end

gGeneralAnim_GhostGetOut::
	loadspritegfx ANIM_TAG_SWEAT_BEAD
	createvisualtask AnimTask_SafariOrGhost_DecideBattlers, 2, 1
	fadetobg BG_GHOST
	waitbgfadeout
	monbg_22 ANIM_ATTACKER
	createvisualtask AnimTask_GhostGetOutAttackerEffect, 2
	waitbgfadein
	loopsewithpan SE_M_PSYBEAM, SOUND_PAN_TARGET, 20, 3
	waitforvisualfinish
	clearmonbg_23 ANIM_ATTACKER
	delay 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, -1, 0, 6, RGB(21, 22, 26) @ White color
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, FALSE, ANIM_TARGET
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, TRUE, ANIM_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 5, 1
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_TARGET
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, -1, 6, 0, RGB(21, 22, 26) @ White color
	waitforvisualfinish
	restorebg
	waitbgfadein
	end

gGeneralAnim_SilphScoped::
	monbg ANIM_ATTACKER
	playsewithpan SE_M_TELEPORT, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_MINIMIZE, SOUND_PAN_ATTACKER, 48
	createvisualtask AnimTask_TransformMon, 2, ANIM_ATTACKER, SPECIESGFX_FLAG_IS_GHOST
	waitsound
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

gGeneralAnim_SafariRockThrow::
	loadspritegfx ANIM_TAG_ROCKS
	loadspritegfx ANIM_TAG_IMPACT
	createvisualtask AnimTask_SafariOrGhost_DecideBattlers, 2, 0
	waitplaysewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER, 22
	createsprite gSafariRockTemplate, ANIM_TARGET, 3, -17, 14, 8, 0
	delay 50
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -4, -20, ANIM_TARGET, 2
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gGeneralAnim_SafariReaction::
	createvisualtask AnimTask_SafariGetReaction, 2
	jumpreteq 0, SafariReaction_WatchingCarefully
	jumpreteq 1, SafariReaction_Angry
	jumpreteq 2, SafariReaction_Eating
	end

SafariReaction_WatchingCarefully::
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_TARGET
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 16, 96, ANIM_ATTACKER, 2
	waitforvisualfinish
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_TARGET
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 16, -96, ANIM_ATTACKER, 2
	end

SafariReaction_Angry::
	loadspritegfx ANIM_TAG_ANGER
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 20, -20
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	waitforvisualfinish
	delay 12
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -20, -20
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	end

SafariReaction_Eating::
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_TARGET
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 8, 136, ANIM_ATTACKER, 2
	waitforvisualfinish
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_TARGET
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 8, 136, ANIM_ATTACKER, 2
	end

@ Credits: pokeemerald-expansion
gGeneralAnim_LoadAbilityPopUp::
	monbg ANIM_ATK_SIDE
    createvisualtask AnimTask_CreateAbilityPopUp, 5
	waitforvisualfinish
	delay 32
	clearmonbg ANIM_ATK_SIDE
	end

@ Credits: pokeemerald-expansion and Blackuser
gGeneralAnim_RemoveAbilityPopUp::
    createvisualtask AnimTask_HideAbilityPopUp, 5, TRUE
	delay 32
	end

@ Credits: pokeemerald-expansion and Blackuser
gGeneralAnim_SlideOutOffscreen::
	createvisualtask AnimTask_SlideOffScreen, 2, ANIM_ATTACKER, 3
    waitforvisualfinish
	invisible ANIM_ATTACKER
	end

@ Credits: Blackuser
gGeneralAnim_ItemThrow::
    loadspritegfx ANIM_TAG_THIN_RING
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
    createvisualtask AnimTask_ItemThrow, 2
	waitforvisualfinish
	delay 20
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, 3, 7, 0, RGB(17, 31, 25) @ Some fade color
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE
	end

@ Credits: Blackuser
gGeneralAnim_Commander::
	loadspritegfx ANIM_TAG_SHARP_TEETH
	createvisualtask AnimTask_SetAnimTargetFromArg, 2
	playsewithpan SE_M_SAND_ATTACK, SOUND_PAN_ATTACKER
	createsprite gSlideMonToPartnerOffsetAndBackSpriteTemplate, ANIM_ATTACKER, 2, 2, -5, TRUE
	waitforvisualfinish
	delay 8
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gSharpTeethSpriteTemplate, ANIM_TARGET, 5, 0, -32, 0, 0, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_TARGET, 5, 0, 32, 4, 0, -819, 10
	waitforvisualfinish
	invisible ANIM_TARGET
	delay 5
	createsprite gSlideMonToPartnerOffsetAndBackSpriteTemplate, ANIM_ATTACKER, 2, -2, -5, FALSE
	end

@ Credits: Blackuser
gGeneralAnim_UpdateAbilityPopUp::
	monbg ANIM_ATK_SIDE
	createvisualtask AnimTask_HideAbilityPopUp, 5, FALSE
	delay 64
	createvisualtask AnimTask_UpdateAbilityPopUp, 5
	delay 32
	clearmonbg ANIM_ATK_SIDE
	end

@ Credits: Blackuser
gGeneralAnim_HealingWishHeal::
	loadspritegfx ANIM_TAG_GREEN_SPARKLE
	loadspritegfx ANIM_TAG_BLUE_STAR
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 16, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_MOONLIGHT, SOUND_PAN_ABOVE
	createsprite gHealingWishSparkleSpriteTemplate, ANIM_ATTACKER, 40, FALSE
	waitforvisualfinish
	delay 32
	call HealingEffect
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 16, 0, RGB_BLACK
	end

@ Credits: Skeli and Blackuser
gGeneralAnim_MegaEvolution::
	loadspritegfx ANIM_TAG_MEGA_EVOLUTION_ORBS
	loadspritegfx ANIM_TAG_MEGA_EVOLUTION_STONE
	loadspritegfx ANIM_TAG_MEGA_SYMBOL
	loadspritegfx ANIM_TAG_ELECTRICITY
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_ELECTRICITY, 0, 12, 12, RGB(27, 6, 29) @ Reddish Purple color
	loopsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER, 13, 3
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 6, 0, 11, RGB(31, 31, 11) @ Yellow color
	call MegaEvolutionOrbs
	call MegaEvolutionOrbs
	call MegaEvolutionOrbs
	waitforvisualfinish
	playsewithpan SE_M_SOLAR_BEAM, SOUND_PAN_ATTACKER
	createsprite gMegaEvolutionStoneSpriteTemplate, ANIM_ATTACKER, 41, 0, 0, ANIM_ATTACKER, TRUE
	delay 20
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_NO_EXCEPTIONS, 2, 0, 16, RGB_WHITE
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_MEGA_EVOLUTION_STONE, 4, 0, 16, RGB_WHITE
	waitforvisualfinish
	call MegaEvolutionFadeIn
	blendoff
	createsprite gMegaEvolutionSymbolSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE
	delay 2
	createsprite gMegaEvolutionSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 12, 528, 30, 13, 50, ANIM_ATTACKER
	delay 2
	createsprite gMegaEvolutionSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 480, 20, 16, -46, ANIM_ATTACKER
	delay 2
	createsprite gMegaEvolutionSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 1, 576, 20, 8, 42, ANIM_ATTACKER
	delay 2
	createsprite gMegaEvolutionSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 15, 400, 25, 11, -42, ANIM_ATTACKER
	delay 2
	createsprite gMegaEvolutionSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 12, 512, 25, 16, 46, ANIM_ATTACKER
	delay 2
	createsprite gMegaEvolutionSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 1, 464, 30, 15, -50, ANIM_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BATTLERS_2, 2, 0, 0, RGB_WHITE
	end

MegaEvolutionOrbs::
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, 40, -10, 13, 0
	delay 3
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, -35, -10, 13, 1
	delay 3
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, 15, -40, 13, 2
	delay 3
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, -10, -32, 13, 3
	delay 3
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, 25, -20, 13, 4
	delay 3
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, -40, -20, 13, 5
	delay 3
	createsprite gMegaEvolutionOrbsSpriteTemplate, ANIM_ATTACKER, 2, 5, -40, 13, 6
	delay 3
	return

MegaEvolutionFadeIn::
	clearmonbg ANIM_ATK_SIDE
	createvisualtask AnimTask_MegaEvolutionUpdateAttackerSprite, 5
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_NO_EXCEPTIONS, 2, 16, 16, RGB_WHITE
	delay 1
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_NO_EXCEPTIONS, 2, 16, 0, RGB_WHITE
	createvisualtask AnimTask_HorizontalShake, 5, ANIM_ATTACKER, 5, 14
	createvisualtask SoundTask_PlayCryHighPitch, 1, ANIM_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	return

@ Credits: Blackuser
gGeneralAnim_BattleBond::
	loadspritegfx ANIM_TAG_WATER_ORB
	loadspritegfx ANIM_TAG_WHITE_CIRCLE_OF_LIGHT
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WHITE_CIRCLE_OF_LIGHT, 0, 12, 12, RGB(0, 13, 23) @ Blue color
	playsewithpan SE_M_SOLAR_BEAM, SOUND_PAN_ATTACKER
	call WhirlpoolEffect
	createsprite gLusterPurgeCircleSpriteTemplate, ANIM_ATTACKER, 41, 0, 0, ANIM_ATTACKER, TRUE
	call WhirlpoolEffect
	call WhirlpoolEffect
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_NO_EXCEPTIONS, 2, 0, 16, RGB(0, 13, 23) @ Blue color
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WHITE_CIRCLE_OF_LIGHT, 2, 12, 16, RGB(0, 13, 23) @ Blue color
	call WhirlpoolEffect
	createvisualtask AnimTask_TransformMon, 2, ANIM_ATTACKER, SPECIESGFX_FLAG_NO_TRANSFORM_PAL_FADE
	call WhirlpoolEffect
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_NO_EXCEPTIONS, 2, 16, 0, RGB(0, 13, 23) @ Blue color
	waitforvisualfinish
	createvisualtask AnimTask_HorizontalShake, 5, ANIM_ATTACKER, 5, 14
	createvisualtask SoundTask_PlayCryHighPitch, 1, ANIM_ATTACKER
	createvisualtask SoundTask_WaitForCry, 1
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

@ Credits: Skeli
gGeneralAnim_ZygardeCellSwirl::
	loadspritegfx ANIM_TAG_ZYGARDE_HEXES
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 15, RGB(0, 6, 0) @ Green color
	delay 10
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 120, 70, 5, 70, 30
	delay 1
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 115, 55, 6, 60, 25
	delay 1
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 115, 60, 7, 60, 30
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 115, 55, 10, 60, 30
	delay 3
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 100, 50, 4, 50, 26
	delay 1
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 105, 25, 8, 60, 20
	delay 1
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 115, 40, 10, 48, 30
	delay 3
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_TransformMon, 2, ANIM_ATTACKER, SPECIESGFX_FLAG_NO_TRANSFORM_PAL_FADE
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 120, 30, 6, 45, 25
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 115, 35, 10, 60, 30
	delay 3
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 105, 20, 8, 40, 0
	delay 3
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ATTACKER
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 20, 255, 15, 32, 0
	createsprite gZygardeHexSpriteTemplate, ANIM_ATTACKER, 2, 110, 10, 8, 32, 20
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 15, 0, RGB(0, 6, 0) @ Green color
	waitforvisualfinish
	createvisualtask AnimTask_HorizontalShake, 5, ANIM_ATTACKER, 5, 14
	createvisualtask SoundTask_PlayCryHighPitch, 1, ANIM_ATTACKER
	createvisualtask SoundTask_WaitForCry, 1
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

@ Credits: Skeli
gGeneralAnim_Schooling::
	loadspritegfx ANIM_TAG_SMALL_FISH
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	loopsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER, 3, 16
	call SchoolingFishes
	createvisualtask AnimTask_TransformMon, 2, ANIM_ATTACKER, SPECIESGFX_FLAG_NO_TRANSFORM_PAL_FADE
	call SchoolingFishes
	call SchoolingFishes
	waitforvisualfinish
	createvisualtask AnimTask_HorizontalShake, 5, ANIM_ATTACKER, 5, 14
	createvisualtask SoundTask_PlayCryHighPitch, 1, ANIM_ATTACKER
	createvisualtask SoundTask_WaitForCry, 1
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

SchoolingFishes::
	createsprite gSchoolingFishSpriteTemplate, ANIM_ATTACKER, 2, 0, 28, 528, 30, 13, 50, ANIM_ATTACKER
	delay 2
	createsprite gSchoolingSmallBubblesSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 480, 20, 16, -46, ANIM_ATTACKER
	delay 2
	createsprite gSchoolingFishSpriteTemplate, ANIM_ATTACKER, 2, 0, 33, 576, 20, 8, 42, ANIM_ATTACKER
	delay 2
	createsprite gSchoolingSmallBubblesSpriteTemplate, ANIM_ATTACKER, 2, 0, 31, 400, 25, 11, -42, ANIM_ATTACKER
	delay 2
	createsprite gSchoolingFishSpriteTemplate, ANIM_ATTACKER, 2, 0, 28, 512, 25, 16, 46, ANIM_ATTACKER
	delay 2
	createsprite gSchoolingSmallBubblesSpriteTemplate, ANIM_ATTACKER, 2, 0, 33, 464, 30, 15, -50, ANIM_ATTACKER
	delay 2
	return

@ Credits: Blackuser
gGeneralAnim_SpitOutPrey::
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_IMPACT
	createvisualtask AnimTask_SetAnimTargetFromArg, 2
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gPreySpitOutSpriteTemplate, ANIM_TARGET, 2, 0, 0, 21
	waitforvisualfinish
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	delay 1
	goto gGeneralAnim_FormChange

@ Credits: Blackuser
gGeneralAnim_SilentFormChange::
	monbg ANIM_ATTACKER
	createvisualtask AnimTask_TransformMon, 2, ANIM_ATTACKER, SPECIESGFX_FLAG_NO_TRANSFORM_PAL_FADE
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

@ Credits: Skeli
gGeneralAnim_TotemBoost::
	loadspritegfx ANIM_TAG_FOCUS_ENERGY
	loadspritegfx ANIM_TAG_WHIP_HIT @ Green color
	loadspritegfx ANIM_TAG_SWEAT_BEAD @ Blue color
	loadspritegfx ANIM_TAG_PAW_PRINT @ Yellow color
	monbg ANIM_ATTACKER
	setalpha 12, 8
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 6, 0, 11, RGB_RED
	call RainbowBuffEffect
	call RainbowBuffEffect
	call RainbowBuffEffect
	call RainbowBuffEffect
	call RainbowBuffEffect
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

RainbowBuffEffect::
	createsprite gEndureBlueEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, 26, 2
	delay 3
	createsprite gEndureEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 14, 28, 1
	delay 3
	createsprite gEndureGreenEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -5, 10, 2
	delay 3
	createsprite gEndureYellowEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 28, 26, 3
	delay 3
	return

@ Credits: Skeli
gGeneralAnim_AquaRingHeal::
	loadspritegfx ANIM_TAG_GUARD_RING
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_WATER_ORB @ Blue color
	monbg ANIM_ATTACKER
	setalpha 8, 8
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 10, RGB(14, 27, 31) @ Blue color
	call AquaRingRings
	waitforvisualfinish
	call HealingEffect
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

@@@@@@@@@@@@@@@@@@@@@@
@ SPECIAL ANIMATIONS @
@@@@@@@@@@@@@@@@@@@@@@

gSpecialAnim_LvlUp::
	playsewithpan SE_EXP_MAX, SOUND_PAN_ABOVE
	createvisualtask AnimTask_LoadHealthboxPalsForLevelUp, 2
	createvisualtask AnimTask_FlashHealthboxOnLevelUp, 5, 0, 0
	waitforvisualfinish
	createvisualtask AnimTask_FreeHealthboxPalsForLevelUp, 2
	end

gSpecialAnim_SwitchOutMon::
	createvisualtask AnimTask_SwitchOutBallEffect, 2
	delay 10
	createvisualtask AnimTask_SwitchOutShrinkMon, 2
	end

gSpecialAnim_BallThrow::
	createvisualtask AnimTask_LoadOrFreeBallGfx, 2, TRUE @ Load ball gfx
	playsewithpan SE_BALL_THROW, SOUND_PAN_ABOVE
NormalBallThrowAnim::
	createvisualtask AnimTask_ThrowBall, 2
	createvisualtask AnimTask_IsBallBlockedByTrainerOrDodged, 2
	jumpreteq -1, BallThrowTrainerBlock @ Trainer blocked
	jumpreteq -2, BallThrowGhostDodged @ Ghost dodged
BallThrowEnd::
	waitforvisualfinish
	createvisualtask AnimTask_LoadOrFreeBallGfx, 2, FALSE @ Free ball gfx
	end

BallThrowTrainerBlock::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	delay 25
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -4, -20, ANIM_TARGET, 2
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	goto BallThrowEnd

BallThrowGhostDodged::
	delay 16
	createvisualtask AnimTask_WindUpLunge, 2, ANIM_TARGET, 48, 6, 16, 48, -48, 16
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_TARGET
	waitplaysewithpan SE_M_TAKE_DOWN, SOUND_PAN_TARGET, 48
	goto BallThrowEnd

gSpecialAnim_SafariBallThrow::
	createvisualtask AnimTask_LoadOrFreeBallGfx, 2, TRUE @ Load ball gfx
	createvisualtask AnimTask_ThrowBallSpecial, 2
	goto BallThrowEnd

gSpecialAnim_SubstituteToMon::
	createvisualtask AnimTask_SwapMonSpriteToFromSubstitute, 2, TRUE
	end

gSpecialAnim_MonToSubstitute::
	createvisualtask AnimTask_SwapMonSpriteToFromSubstitute, 2, FALSE
	end

gSpecialAnim_CriticalCaptureBallThrow::
	createvisualtask AnimTask_LoadOrFreeBallGfx, 2, TRUE @ Load ball gfx
	playsewithpan SE_FALL, SOUND_PAN_ABOVE
	goto NormalBallThrowAnim

@@@@@@@@@@@@@@@@@@@
@ MOVE ANIMATIONS @
@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@ GEN I @@@@@@@@@@@@@@@@@@@@@@@

gMoveAnim_NONE::
gMoveAnim_POUND::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_KARATE_CHOP::
	loadspritegfx ANIM_TAG_QUICK_GUARD
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	createsprite gKarateHandSpriteTemplate, ANIM_ATTACKER, 3, 0, -32, 15, 16
	delay 19
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 4, 0, 6, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli and Blackuser
gMoveAnim_DOUBLE_SLAP::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_QUICK_GUARD
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gDoubleSlapSpriteTemplate, ANIM_TARGET, 3, -64, 6, 1, 0
	delay 8
	call SlapHitAnim
	delay 20
	call SlapHitAnim
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

SlapHitAnim::
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 2, 0, 6, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 1, 0, 0, ANIM_TARGET, 2
	return

gMoveAnim_COMET_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_TARGET
	setalpha 12, 8
	choosetwoturnanim CometPunchLeft, CometPunchRight
CometPunchContinue::
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

CometPunchLeft::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -8, -8, ANIM_TARGET, 2
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 3, -8, 0, 8, ANIM_TARGET, 0
	goto CometPunchContinue

CometPunchRight::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 8, -8, ANIM_TARGET, 2
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 3, 8, 0, 8, ANIM_TARGET, 0
	goto CometPunchContinue

gMoveAnim_MEGA_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_TARGET
	delay 2
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 0, 16, RGB_BLACK
	setalpha 12, 8
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_TARGET
	createsprite gMegaPunchKickSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 0, 50
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 7, RGB_WHITE
	delay 52
	setbgbasedonside ANIM_TARGET, BG_IMPACT_PLAYER, BG_IMPACT_OPPONENT
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 22, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 0, RGB_WHITE
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_BLACK, 8, RGB_BLACK, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	delay 2
	restorebg
	waitbgfadein
	end

gMoveAnim_PAY_DAY::
	loadspritegfx ANIM_TAG_COIN
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gCoinThrowSpriteTemplate, ANIM_ATTACKER, 2, 20, 0, 0, 0, 1152, 256
	waitforvisualfinish
	playsewithpan SE_M_PAY_DAY, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 1, 0, 0, ANIM_TARGET, 2
	createsprite gFallingCoinSpriteTemplate, ANIM_ATTACKER, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_FIRE_PUNCH::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_SMALL_EMBER
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 9, RGB_RED
	createsprite gFireSpiralInwardSpriteTemplate, ANIM_TARGET, 1, 0
	createsprite gFireSpiralInwardSpriteTemplate, ANIM_TARGET, 1, 64
	createsprite gFireSpiralInwardSpriteTemplate, ANIM_TARGET, 1, 128
	createsprite gFireSpiralInwardSpriteTemplate, ANIM_TARGET, 1, 196
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 3, 0, 0, 8, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 15, 1
	call FireSpreadEffect
	delay 4
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 0, 9, 0, RGB_RED
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_ICE_PUNCH::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 7, RGB_BLACK
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 9, RGB(12, 26, 31) @ Crystal blue color
	delay 20
	playsewithpan SE_M_STRING_SHOT, SOUND_PAN_TARGET
	createsprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 0
	createsprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 64
	createsprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 128
	createsprite gIceCrystalSpiralInwardSmall, ANIM_ATTACKER, 2, 192
	delay 5
	createsprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 32
	createsprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 96
	createsprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 160
	createsprite gIceCrystalSpiralInwardLarge, ANIM_ATTACKER, 2, 224
	delay 17
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 4, 0, -10, 8, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -10, ANIM_TARGET, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 3, 1
	waitforvisualfinish
	delay 15
	call IceCrystalEffectShort
	delay 5
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 9, 0, RGB(12, 26, 31) @ Crystal blue color
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 7, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Skeli
gMoveAnim_THUNDER_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_SPARK_2
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 14, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 0, 0, 8, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 15, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 9, RGB(30, 31, 1) @ Yellow color
	delay 5
	call ElectricityEffect
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 9, 0, RGB(30, 31, 1) @ Yellow color
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 14, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_SCRATCH::
	loadspritegfx ANIM_TAG_SCRATCH
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	createsprite gScratchSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, TRUE
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_VISE_GRIP::
	loadspritegfx ANIM_TAG_CUT
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_VICEGRIP, SOUND_PAN_TARGET
	createsprite gViseGripSpriteTemplate, ANIM_ATTACKER, 2, FALSE
	createsprite gViseGripSpriteTemplate, ANIM_ATTACKER, 2, TRUE
	delay 9
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 1, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 2, 0, 5, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_GUILLOTINE::
	loadspritegfx ANIM_TAG_CUT
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	setbgbasedonside ANIM_TARGET, BG_GUILLOTINE_PLAYER, BG_GUILLOTINE_OPPONENT, TRUE
	waitbgfadein
	playsewithpan SE_M_VICEGRIP, SOUND_PAN_TARGET
	createsprite gGuillotineSpriteTemplate, ANIM_ATTACKER, 2, FALSE
	createsprite gGuillotineSpriteTemplate, ANIM_ATTACKER, 2, TRUE
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 16, RGB_BLACK
	delay 9
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 2, 0, 23, 1
	delay 46
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 4, 0, 8, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 0
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_BLACK, 8, RGB_BLACK, 0
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	restorebg
	waitbgfadein
	end

gMoveAnim_RAZOR_WIND::
	choosetwoturnanim RazorWindSetUp, RazorWindUnleash
RazorWindEnd::
	end

RazorWindSetUp::
	loadspritegfx ANIM_TAG_GUST
	playsewithpan SE_M_GUST, SOUND_PAN_ATTACKER
	createsprite gRazorWindTornadoSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, 16, 0, 7, 40
	createsprite gRazorWindTornadoSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, 16, 85, 7, 40
	createsprite gRazorWindTornadoSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, 16, 170, 7, 40
	waitforvisualfinish
	playsewithpan SE_M_GUST2, SOUND_PAN_ATTACKER
	goto RazorWindEnd

RazorWindUnleash::
	loadspritegfx ANIM_TAG_AIR_WAVE_2
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, 8, 0, 0, 22, 2, TRUE
	delay 2
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, -8, 16, 14, 22, 1, TRUE
	delay 2
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, 12, -16, -14, 22, 0, TRUE
	delay 17
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 10, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 2, 0, 10, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	goto RazorWindEnd

@ Credits: Blackuser
gMoveAnim_SWORDS_DANCE::
	loadspritegfx ANIM_TAG_SWORD
	loadspritegfx ANIM_TAG_LEER
	monbg ANIM_ATTACKER
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_LEER, 0, 12, 12, RGB_CYAN @ Blue cyan color
	waitforvisualfinish
	playsewithpan SE_M_SWORDS_DANCE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 16, 6, 1, 4
	createsprite gSwordsDanceBladeSpriteTemplate, ANIM_ATTACKER, 2, 0, -4, 1, 0
	createsprite gSwordsDanceBladeSpriteTemplate, ANIM_ATTACKER, 2, -32, -12, 2, 0
	createsprite gSwordsDanceBladeSpriteTemplate, ANIM_ATTACKER, 2, 32, -12, 3, 0
	createsprite gSwordsDanceBladeSpriteTemplate, ANIM_ATTACKER, 2, -16, -24, 4, 1
	createsprite gSwordsDanceBladeSpriteTemplate, ANIM_ATTACKER, 2, 16, -24, 5, 1
	delay 22
	createvisualtask AnimTask_FlashAnimTagWithColor, 2, ANIM_TAG_SWORD, 2, 2, RGB(18, 31, 31), 16, 0, 0 @ White blue color
	delay 36
	playsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER
	createsprite gLeerSpriteTemplate, ANIM_ATTACKER, 2, 4, -80
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_CUT::
	loadspritegfx ANIM_TAG_CUT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 0
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 10, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_GUST::
	loadspritegfx ANIM_TAG_GUST
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_GUST, SOUND_PAN_TARGET
	createsprite gEllipticalGustSpriteTemplate, ANIM_ATTACKER, 2, 0, -16
	createvisualtask AnimTask_AnimateGustTornadoPalette, 5, 1, 70
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 1, 0, 7, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	playsewithpan SE_M_GUST2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Skeli
gMoveAnim_WING_ATTACK::
	loadspritegfx ANIM_TAG_WHITE_FEATHER
	loadspritegfx ANIM_TAG_IMPACT
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WHITE_FEATHER, 2, 10, 10, RGB(28, 28, 12) @ Yellow color
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 10, 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 10, ANIM_TARGET, 2
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 2
	call ScatterFeathers
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, -10, ANIM_TARGET, 2
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 5, 0, -20, ANIM_TARGET, 2
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 6, 0, -30, ANIM_TARGET, 2
	waitforvisualfinish
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_WHIRLWIND::
	loadspritegfx ANIM_TAG_WHIRLWIND_LINES
	createsprite gWhirlwindLineSpriteTemplate, ANIM_ATTACKER, 2, 0, -8, ANIM_TARGET, 60, 0
	createsprite gWhirlwindLineSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 60, 1
	createsprite gWhirlwindLineSpriteTemplate, ANIM_ATTACKER, 2, 0, 8, ANIM_TARGET, 60, 2
	createsprite gWhirlwindLineSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, ANIM_TARGET, 60, 3
	createsprite gWhirlwindLineSpriteTemplate, ANIM_ATTACKER, 2, 0, 24, ANIM_TARGET, 60, 4
	createsprite gWhirlwindLineSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, ANIM_TARGET, 60, 0
	delay 5
	loopsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET, 10, 4
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 15, 1
	delay 29
	createvisualtask AnimTask_WhirlwindStretchAndRotateTarget, 3
	delay 19
	playsewithpan SE_M_STRING_SHOT, SOUND_PAN_TARGET
	createvisualtask AnimTask_SlideOffScreen, 5, ANIM_TARGET, 8
	end

gMoveAnim_FLY::
	loadspritegfx ANIM_TAG_ROUND_SHADOW
	choosetwoturnanim FlySetUp, FlyUnleash
FlyEnd::
	end

FlySetUp::
	playsewithpan SE_M_FLY, SOUND_PAN_ATTACKER
	createsprite gFlyBallUpSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 13, 336
	waitforvisualfinish
	goto FlyEnd

FlyUnleash::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	createsprite gFlyBallAttackSpriteTemplate, ANIM_ATTACKER, 2, 20
	delay 20
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	goto FlyEnd

gMoveAnim_BIND::
	createvisualtask AnimTask_SwayMon, 5, 0, 6, 3328, 4, ANIM_ATTACKER
BindWrap::
	playsewithpan SE_M_BIND, SOUND_PAN_TARGET
	call BindWrapSqueezeTarget
	call BindWrapSqueezeTarget
	end

BindWrapSqueezeTarget::
	createvisualtask AnimTask_ScaleMonAndRestore, 5, 10, -5, 5, ANIM_TARGET, ST_OAM_OBJ_NORMAL
	delay 16
	return

gMoveAnim_SLAM::
	loadspritegfx ANIM_TAG_SLAM_HIT
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_ATTACKER
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 3, FALSE, 4
	delay 1
	createsprite gSlamHitSpriteTemplate, ANIM_ATTACKER, 2, 0, 0
	delay 3
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -12, 10, FALSE, 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	delay 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 0, 3, 6, 1
	waitforvisualfinish
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_VINE_WHIP::
	loadspritegfx ANIM_TAG_WHIP_HIT
	loadspritegfx ANIM_TAG_LEAF
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 6
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	createsprite gVineWhipSpriteTemplate, ANIM_TARGET, 2, 0, 0
	delay 6
	call ScatterLeafs
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 6, 1
	end

gMoveAnim_STOMP::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	createsprite gStompFootSpriteTemplate, ANIM_ATTACKER, 3, 0, -32, 15, 0, 1, 0
	delay 19
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, -8, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 4, 9, 1
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_DOUBLE_KICK::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT @ Sprite created in gFistFootRandomPosSpriteTemplate
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gFistFootRandomPosSpriteTemplate, ANIM_ATTACKER, 3, ANIM_TARGET, 20, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 4, 0, 6, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_MEGA_KICK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_TARGET
	delay 2
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 0, 16, RGB_BLACK
	setalpha 12, 8
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_TARGET
	createsprite gMegaPunchKickSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 1, 50
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 7, RGB_WHITE
	delay 50
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	setbgbasedonside ANIM_TARGET, BG_IMPACT_PLAYER, BG_IMPACT_OPPONENT
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 22, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 0, RGB_WHITE
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_BLACK, 8, RGB_BLACK, 0
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	delay 2
	restorebg
	waitbgfadein
	end

gMoveAnim_JUMP_KICK::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 3
	createsprite gJumpKickSpriteTemplate, ANIM_ATTACKER, 2, -16, 8, 0, 0, 10, ANIM_TARGET, 1, TRUE
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 1, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 5, 0, 7, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_ROLLING_KICK::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 18, 6, 1, 4
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 7, 3
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	delay 6
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 0, FALSE, 4
	createsprite gCircularPunchOrKickSpriteTemplate, ANIM_TARGET, 2, 0, 0, 1, 0, 25, -10
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	delay 3
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -8, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 8
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_SAND_ATTACK::
	loadspritegfx ANIM_TAG_MUD_SAND
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	setalpha 12, 8
	playsewithpan SE_M_SAND_ATTACK, SOUND_PAN_ATTACKER
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -10, 0, FALSE, 3
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 2
	call SandAttackDirt
	call SandAttackDirt
	call SandAttackDirt
	call SandAttackDirt
	call SandAttackDirt
	call SandAttackDirt
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

SandAttackDirt::
	createsprite gSandAttackDirtSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 0, 0
	createsprite gSandAttackDirtSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 10, 10
	createsprite gSandAttackDirtSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, -10, -10
	createsprite gSandAttackDirtSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 20, 5
	createsprite gSandAttackDirtSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, -20, -5
	delay 2
	return

gMoveAnim_HEADBUTT::
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 0
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 2
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 1
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 4, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 2
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	end

gMoveAnim_HORN_ATTACK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HORN_HIT
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 0
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 2
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 1
	createsprite gHornHitSpriteTemplate, ANIM_TARGET, 4, 0, 0, 10
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 4, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 2
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	end

gMoveAnim_FURY_ATTACK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HORN_HIT
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 4, 256, ANIM_ATTACKER, 2
	choosetwoturnanim FuryAttackRight, FuryAttackLeft
FuryAttackContinue::
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	end

FuryAttackRight::
	createsprite gHornHitSpriteTemplate, ANIM_TARGET, 4, 8, 8, 10
	waitforvisualfinish
	goto FuryAttackContinue

FuryAttackLeft::
	createsprite gHornHitSpriteTemplate, ANIM_TARGET, 4, -8, -8, 10
	waitforvisualfinish
	goto FuryAttackContinue

gMoveAnim_HORN_DRILL::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HORN_HIT
	call SetUpDrillBackground
	setalpha 12, 8
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 0
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 2
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 1
	createsprite gHornHitSpriteTemplate, ANIM_TARGET, 4, 0, 0, 12
	waitforvisualfinish
	playse SE_BANG
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 40, 1
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 10, 0, 40, 1
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 2, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, -4, 3, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, -8, -5, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 4, -12, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 16, 0, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 5, 18, ANIM_TARGET, 3
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, -17, 12, ANIM_TARGET, 2
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, -21, -15, ANIM_TARGET, 2
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 8, -27, ANIM_TARGET, 2
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 32, 0, ANIM_TARGET, 2
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	delay 4
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 2
	waitforvisualfinish
	call UnsetScrollingBg
	end

gMoveAnim_TACKLE::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_BODY_SLAM::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	waitforvisualfinish
	delay 11
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 26, 0, FALSE, 5
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 4, -10, 0, ANIM_TARGET, 0
	loopsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET, 10, 2
	delay 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -28, 0, FALSE, 3
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 4, 0, 12, 1
	waitforvisualfinish
	delay 10
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 6
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_WRAP::
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 6, 4, 2, 4
	goto BindWrap

gMoveAnim_TAKE_DOWN::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_WindUpLunge, 5, ANIM_ATTACKER, -24, 8, 23, 10, 40, 10
	delay 35
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_BLACK, 10, RGB_BLACK, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 4, -10, 0, ANIM_TARGET, 0
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	delay 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -16, 0, FALSE, 4
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 4, 0, 12, 1
	waitforvisualfinish
	delay 2
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	delay 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 7
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_THRASH::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT @ Sprite created in gFistFootRandomPosSpriteTemplate
	createvisualtask AnimTask_ThrashMoveMonHorizontal, 2
	createvisualtask AnimTask_ThrashMoveMonVertical, 2
	createsprite gFistFootRandomPosSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 10, 0
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 4, 0, 7, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 28
	createsprite gFistFootRandomPosSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 10, 1
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 4, 0, 7, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 28
	createsprite gFistFootRandomPosSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 10, 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 8, 0, 16, 1
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	end

@ Credits: Skeli
gMoveAnim_DOUBLE_EDGE::
	loadspritegfx ANIM_TAG_IMPACT
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER, 8
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 18, 6, 2, 4
	delay 10
	setbgbasedonside ANIM_TARGET, BG_IMPACT_PLAYER, BG_IMPACT_OPPONENT, TRUE
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 0, FALSE, 4
	delay 3
	waitforvisualfinish
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, -10, 0, ANIM_TARGET, 0
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -32, 0, FALSE, 3
	waitforvisualfinish
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 8, -256, ANIM_ATTACKER, 0
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 8, -256, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 4, 0, 12, 1
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 4, 0, 12, 1
	waitforvisualfinish
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 8, -256, ANIM_ATTACKER, 1
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 8, -256, ANIM_TARGET, 1
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	delay 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 7
	waitforvisualfinish
	restorebg
	waitbgfadeout
	end

gMoveAnim_TAIL_WHIP::
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 24, 3
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 12, 4, 2, 3
	end

@ Credits: Blackuser
gMoveAnim_POISON_STING::
	loadspritegfx ANIM_TAG_NEEDLE
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_NEEDLE, 0, 4, 4, RGB(30, 0, 31) @ Purple color
	waitforvisualfinish
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gLinearStingerSpriteTemplate, ANIM_TARGET, 2, 20, 0, -8, 0, 20
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 5, 1
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	waitforvisualfinish
	call PoisonBubblesEffect
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_TWINEEDLE::
	loadspritegfx ANIM_TAG_NEEDLE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_NEEDLE, 0, 4, 4, RGB(30, 0, 31) @ Purple color
	waitforvisualfinish
	loopsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER, 6, 2
	createsprite gLinearStingerSpriteTemplate, ANIM_TARGET, 2, 10, -4, 0, -4, 20
	createsprite gLinearStingerSpriteTemplate, ANIM_TARGET, 2, 20, 12, 10, 12, 20
	delay 20
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 5, 1
	createsprite gHandleInvertHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -4, ANIM_TARGET, 3
	loopsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 5, 2
	delay 1
	createsprite gHandleInvertHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 10, 12, ANIM_TARGET, 3
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_PIN_MISSILE::
	loadspritegfx ANIM_TAG_NEEDLE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gPinMissileSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, -8, -8, 20, -32
	delay 15
	createsprite gPinMissileSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, 8, 8, 20, -40
	delay 4
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -8, -8, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	delay 9
	createsprite gPinMissileSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, 0, 0, 20, -32
	delay 4
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 8, 8, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	delay 14
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_LEER::
	loadspritegfx ANIM_TAG_LEER
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	setalpha 8, 8
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	createsprite gLeerSpriteTemplate, ANIM_ATTACKER, 2, 24, -12
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -5, -5, 10, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	delay 10
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 9, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 1, 0, 9, 1
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_BITE::
	loadspritegfx ANIM_TAG_SHARP_TEETH
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, -32, 0, 0, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 4, 0, -819, 10
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 7, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_GROWL::
	loadspritegfx ANIM_TAG_NOISE_LINE
	createvisualtask SoundTask_PlayDoubleCry, 2, ANIM_ATTACKER, CRY_MODE_GROWL_1, CRY_MODE_GROWL_2
	call RoarEffect
	delay 10
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 9, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 1, 0, 9, 1
	waitforvisualfinish
	createvisualtask SoundTask_WaitForCry, 5
	end

@ Credits: Blackuser
gMoveAnim_ROAR::
	loadspritegfx ANIM_TAG_NOISE_LINE
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	setalpha 8, 8
	createvisualtask SoundTask_PlayDoubleCry, 2, ANIM_ATTACKER, CRY_MODE_ROAR_1, CRY_MODE_ROAR_2
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -5, -5, 10, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	call RoarEffect
	delay 15
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 9, 1
	delay 15
	createvisualtask AnimTask_SlideOffScreen, 5, ANIM_TARGET, 2
	waitforvisualfinish
	createvisualtask SoundTask_WaitForCry, 5
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_SING::
	loadspritegfx ANIM_TAG_MUSIC_NOTES
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_MusicNotesRainbowBlend, 2
	panse_1B SE_M_SING, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 7, 0, 12
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 6, 1, 12
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 1, 2, 12
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 3, 12
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 3, 0, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 1, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 5, 2, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 6, 3, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 0, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 1, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 1, 2, 12
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 5, 3, 12
	waitforvisualfinish
	createvisualtask AnimTask_MusicNotesClearRainbowBlend, 2
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_SUPERSONIC::
	loadspritegfx ANIM_TAG_GOLD_RING
	monbg ANIM_ATK_SIDE
	monbgprio_2A ANIM_ATTACKER
	setalpha 12, 8
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 2, 0, 8, 1
	call SupersonicRing
	call SupersonicRing
	call SupersonicRing
	call SupersonicRing
	call SupersonicRing
	call SupersonicRing
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

SupersonicRing::
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	createsprite gSupersonicWaveSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 30, 0
	delay 2
	return

gMoveAnim_SONIC_BOOM::
	loadspritegfx ANIM_TAG_AIR_WAVE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	call SonicBoomProjectile
	call SonicBoomProjectile
	call SonicBoomProjectile
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 10, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 2
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

SonicBoomProjectile::
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gSonicBoomSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 15
	delay 4
	return

gMoveAnim_DISABLE::
	loadspritegfx ANIM_TAG_SPARKLE_4
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 8, 8
	playsewithpan SE_M_DETECT, SOUND_PAN_ATTACKER
	createsprite gDisableSparkleSpriteTemplate, ANIM_ATTACKER, 13, 24, -16
	waitforvisualfinish
	createvisualtask AnimTask_GrowAndGreyscale, 5, ANIM_TARGET, 80
	loopsewithpan SE_M_BIND, SOUND_PAN_TARGET, 15, 4
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_ACID::
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_POISON_BUBBLE, 0, 12, 12, RGB(29, 16, 1) @ Orange color
	waitforvisualfinish
	createsprite gAcidPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 20, 0, 40, TRUE, 0, 0, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 5
	createsprite gAcidPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 20, 0, 40, TRUE, 24, 0, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 5
	createsprite gAcidPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 20, 0, 40, TRUE, -24, 0, TRUE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 15
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 2, 0, 10, 1
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_DEF_PARTNER, 2, 0, 10, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_DEF_SIDE, 2, 2, 0, 12, RGB(30, 0, 31) @ Purple color
	createsprite gAcidPoisonDropletSpriteTemplate, ANIM_TARGET, 2, 0, -22, 0, 55, TRUE
	playsewithpan SE_M_BUBBLE, SOUND_PAN_TARGET
	delay 10
	createsprite gAcidPoisonDropletSpriteTemplate, ANIM_TARGET, 2, -26, -24, 0, 55, TRUE
	playsewithpan SE_M_BUBBLE, SOUND_PAN_TARGET
	delay 10
	createsprite gAcidPoisonDropletSpriteTemplate, ANIM_TARGET, 2, 15, -27, 0, 50, TRUE
	playsewithpan SE_M_BUBBLE, SOUND_PAN_TARGET
	delay 10
	createsprite gAcidPoisonDropletSpriteTemplate, ANIM_TARGET, 2, -15, -17, 0, 45, TRUE
	playsewithpan SE_M_BUBBLE, SOUND_PAN_TARGET
	delay 10
	createsprite gAcidPoisonDropletSpriteTemplate, ANIM_TARGET, 2, 27, -22, 0, 50, TRUE
	playsewithpan SE_M_BUBBLE, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_EMBER::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	loopsewithpan SE_M_EMBER, SOUND_PAN_ATTACKER, 5, 2
	createsprite gEmberSpriteTemplate, ANIM_TARGET, 2, 20, 0, -16, 24, 20, 1
	delay 4
	createsprite gEmberSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 24, 20, 1
	delay 4
	createsprite gEmberSpriteTemplate, ANIM_TARGET, 2, 20, 0, 16, 24, 20, 1
	delay 16
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
	call EmberFireHit
	call EmberFireHit
	call EmberFireHit
	end

EmberFireHit::
	createsprite gEmberFlareSpriteTemplate, ANIM_TARGET, 2, -24, 24, 24, 24, 20, ANIM_TARGET, TRUE
	delay 4
	return

@ Credits: Skeli
gMoveAnim_FLAMETHROWER::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 8, RGB(29, 1, 1) @ Red color
	delay 5
	loopsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER, 7, 10
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 1, 0, 8, RGB(29, 1, 1) @ Red color
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 2, 0, 42, 1
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	call FlamethrowerCreateFlames
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_TARGET, 1, 8, 0, RGB(29, 1, 1) @ Red color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

FlamethrowerCreateFlames::
	createsprite gFlamethrowerFireSpriteTemplate, ANIM_TARGET, 2, 0, 0, 0, 0, 20
	delay 2
	return

gMoveAnim_MIST::
	loadspritegfx ANIM_TAG_MIST_CLOUD
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	loopsewithpan SE_M_MIST, SOUND_PAN_ATTACKER, 20, 15
	call MistCloud
	call MistCloud
	call MistCloud
	call MistCloud
	call MistCloud
	call MistCloud
	call MistCloud
	delay 32
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATK_SIDE, 8, 2, 0, 14, RGB_WHITE
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

MistCloud::
	createsprite gMistCloudSpriteTemplate, ANIM_ATTACKER, 2, 0, -24, 48, 240, ANIM_ATTACKER, TRUE
	delay 7
	return

@ Credits: Skeli
gMoveAnim_WATER_GUN::
	loadspritegfx ANIM_TAG_WATER_ORB
	panse_1B SE_M_HYDRO_PUMP, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 30, 1
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	call LaunchWaterGunOrb
	end

LaunchWaterGunOrb::
	createsprite gWaterGunOrbSpriteTemplate, ANIM_TARGET, 2, 20, 0, MUS_DUMMY
	delay 5
	return

@ Credits: Skeli
gMoveAnim_HYDRO_PUMP::
	loadspritegfx ANIM_TAG_WATER_ORB
	loadspritegfx ANIM_TAG_WATER_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	fadetobg BG_WATER
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -1024, 0, TRUE
	waitbgfadein
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 40, 1
	delay 6
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WATER_ORB, 2, 10, 10, RGB(10, 21, 28) @ Darken a little the blue
	panse_1B SE_M_HYDRO_PUMP, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createvisualtask AnimTask_StartSinAnimTimer, 5, 100
	call HydroPumpBeams
	call HydroPumpBeams
	call HydroPumpBeams
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 3, 0, 37, 1
	call HydroPumpHitSplats
	call HydroPumpBeams
	call HydroPumpBeams
	call HydroPumpHitSplats
	call HydroPumpBeams
	call HydroPumpBeams
	call HydroPumpHitSplats
	call HydroPumpBeams
	call HydroPumpBeams
	call HydroPumpHitSplats
	call HydroPumpBeams
	call HydroPumpBeams
	call HydroPumpHitSplats
	delay 2
	call HydroPumpHitSplats
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

HydroPumpBeams::
	createsprite gHydroPumpOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, 10, 16
	createsprite gHydroPumpOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, 10, -16
	delay 1
	createsprite gHydroPumpOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, 10, 16
	createsprite gHydroPumpOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, 10, -16
	delay 1
	return

HydroPumpHitSplats::
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 4, 0, 15, ANIM_TARGET, 1
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 4, 0, -15, ANIM_TARGET, 1
	return

gMoveAnim_SURF::
	createvisualtask AnimTask_CreateSurfWave, 2, 0
	delay 24
	panse_1B SE_M_SURF, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	end

@ Credits: Blackuser
gMoveAnim_ICE_BEAM::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 7, RGB_BLUE @ Blue instead of black
	waitforvisualfinish
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_BUBBLE_BEAM2, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 4, 4, 0, 10
	createsprite gIceBeamOuterCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, 12, 0, 12, 20
	createsprite gIceBeamOuterCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, -12, 0, -12, 20
	delay 1
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_TARGET, -31, 0, 7, RGB(0, 20, 31) @ Blue color
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 25, 1
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	call IceBeamCreateCrystals
	createsprite gIceBeamInnerCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, 0, 0, 0, 11
	delay 1
	createsprite gIceBeamInnerCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, 0, 0, 0, 11
	waitforvisualfinish
	delay 20
	call IceCrystalEffectShort
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_TARGET, 5, 7, 0, RGB(0, 20, 31) @ Blue color
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 7, 0, RGB_BLUE @ Blue instead of black
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

IceBeamCreateCrystals::
	createsprite gIceBeamOuterCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, 12, 0, 12, 20
	createsprite gIceBeamOuterCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, -12, 0, -12, 20
	createsprite gIceBeamInnerCrystalSpriteTemplate, ANIM_ATTACKER, 2, 20, 0, 0, 0, 11
	delay 1
	return

gMoveAnim_BLIZZARD::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	monbg ANIM_DEF_SIDE
	call SetHighSpeedBackground
	panse_1B SE_M_BLIZZARD, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	call BlizzardIceCrystals
	call BlizzardIceCrystals
	playsewithpan SE_M_BLIZZARD2, SOUND_PAN_TARGET
	waitforvisualfinish
	call IceCrystalEffectLong
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	delay 20
	call UnsetScrollingBg
	end

BlizzardIceCrystals::
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -10, 0, -10, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, 0, 0, 0, 80, 0, 0, TRUE
	delay 3
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -15, 0, -15, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, -10, 0, -10, 80, 0, 0, TRUE
	delay 3
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -5, 0, -5, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, 10, 0, 10, 80, 0, 0, TRUE
	delay 3
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -10, 0, -10, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, -20, 0, -20, 80, 0, 0, TRUE
	delay 3
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -20, 0, -20, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, 15, 0, 15, 80, 0, 0, TRUE
	delay 3
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -15, 0, -15, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, -20, 0, -20, 80, 0, 0, TRUE
	delay 3
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -25, 0, -25, 72, TRUE
	createsprite gBlizzardIceCrystalSpriteTemplate, ANIM_ATTACKER, 40, 0, 20, 0, 20, 80, 0, 0, TRUE
	delay 3
	return

gMoveAnim_PSYBEAM::
	loadspritegfx ANIM_TAG_GOLD_RING
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	call SetPsychicBackground
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_PSYBEAM2, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 3, 4, 0, 15
	call PsybeamRings
	call PsybeamRings
	createvisualtask AnimTask_SwayMon, 5, 0, 6, 2048, 4, ANIM_TARGET
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 12, RGB(31, 18, 31) @ Purple color
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	call PsybeamRings
	waitforvisualfinish
	call UnsetScrollingBg
	end

PsybeamRings::
	createsprite gGoldRingSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 13, 0
	delay 4
	return

gMoveAnim_BUBBLE_BEAM::
	loadspritegfx ANIM_TAG_BUBBLE
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	call BulbblebeamCreateBubbles
	createvisualtask AnimTask_SwayMon, 5, 0, 3, 3072, 8, ANIM_TARGET
	call BulbblebeamCreateBubbles
	call BulbblebeamCreateBubbles
	waitforvisualfinish
	call WaterBubblesEffectShort
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

BulbblebeamCreateBubbles::
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 35, 70, 0, 256, 50
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	delay 3
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 20, 40, -10, 256, 50
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	delay 3
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 10, -60, 0, 256, 50
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	delay 3
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 15, -15, 10, 256, 50
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	delay 3
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 30, 10, -10, 256, 50
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	delay 3
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 25, -30, 10, 256, 50
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	delay 3
	return

gMoveAnim_AURORA_BEAM::
	loadspritegfx ANIM_TAG_RAINBOW_RINGS
	fadetobg BG_AURORA
	waitbgfadein
	playsewithpan SE_M_BUBBLE_BEAM, SOUND_PAN_ATTACKER
	setarg ARG_RET_ID, 0
	createvisualtask AnimTask_RotateAuroraRingColors, 10, 130
	call AuroraBeamCreateRings
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 1, 0, 17, 1
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	setargret -1 @ Signal to change sprite size
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_BUBBLE_BEAM2, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 3, 6, 0, 10
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 2, 0, 40, 1
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	call AuroraBeamCreateRings
	waitforvisualfinish
	restorebg
	waitbgfadein
	end

AuroraBeamCreateRings::
	createsprite gAuroraBeamRingSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 17
	delay 1
	createsprite gAuroraBeamRingSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 17
	delay 1
	createsprite gAuroraBeamRingSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 17
	delay 1
	createsprite gAuroraBeamRingSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 17
	delay 1
	return

gMoveAnim_HYPER_BEAM::
	loadspritegfx ANIM_TAG_ORBS
	fadetobg BG_HYPER_BEAM
	waitbgfadein
	delay 10
	playsewithpan SE_M_HYPER_BEAM, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 4, 1
	waitforvisualfinish
	delay 30
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_HYPER_BEAM2, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 1, 15, 0, 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_ATTACKER, 0, 4, 50, 1
	createvisualtask AnimTask_FlashAnimTagWithColor, 2, ANIM_TAG_ORBS, 1, 12, RGB_RED, 16, 0, 0
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 50, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 11, RGB(25, 25, 25) @ White color
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	call HyperBeamOrbs
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 11, 0, RGB(25, 25, 25) @ White color
	waitforvisualfinish
	restorebg
	waitbgfadein
	end

HyperBeamOrbs::
	createsprite gHyperBeamOrbSpriteTemplate, ANIM_TARGET, 2
	createsprite gHyperBeamOrbSpriteTemplate, ANIM_TARGET, 2
	delay 1
	return

gMoveAnim_PECK::
	loadspritegfx ANIM_TAG_IMPACT
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createvisualtask AnimTask_RotateMonToSideAndRestore, 2, 3, -768, ANIM_TARGET, 2
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, -12, 0, ANIM_TARGET, 3
	end

gMoveAnim_DRILL_PECK::
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 0
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 2
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 1
	delay 2
	loopsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 4, 8
	createvisualtask AnimTask_DrillPeckHitSplats, 5
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 18, 1
	waitforvisualfinish
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 2
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 6
	end

gMoveAnim_SUBMISSION::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 10
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER, 20
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 30
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER, 40
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 50
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER, 60
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 70
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER, 80
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 90
	createvisualtask AnimTask_TranslateMonElliptical, 2, ANIM_ATTACKER, -18, 6, 6, 4
	createvisualtask AnimTask_TranslateMonElliptical, 2, ANIM_TARGET, 18, 6, 6, 4
	call SubmissionHit
	call SubmissionHit
	call SubmissionHit
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

SubmissionHit::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -12, ANIM_TARGET, 1
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -12, 8, ANIM_TARGET, 1
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 12, 0, ANIM_TARGET, 1
	delay 8
	return

gMoveAnim_LOW_KICK::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 0, FALSE, 4
	createsprite gSlidingKickSpriteTemplate, ANIM_TARGET, 2, -24, 28, 40, 8, 160, 0
	delay 4
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -8, 8, ANIM_TARGET, 2
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 6, 384, ANIM_TARGET, 2
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 4
	end

gMoveAnim_COUNTER::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 18, 6, 1, 4
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 0, FALSE, 4
	delay 4
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -15, 18, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 25, 1
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 3, -15, 18, 8, ANIM_TARGET, 0
	delay 3
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, -4, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 1
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 3, 0, -4, 8, ANIM_TARGET, 0
	delay 3
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 15, 9, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 1
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 3, 15, 9, 8, ANIM_TARGET, 0
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_SEISMIC_TOSS::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_ROCKS
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	fadetobg BG_IN_AIR
	waitbgfadeout
	createvisualtask AnimTask_MoveSeismicTossBg, 3
	playsewithpan SE_M_SKY_UPPERCUT, SOUND_PAN_ABOVE
	waitbgfadein
	waitforvisualfinish
	createvisualtask AnimTask_GetSeismicTossDamageLevel, 3
	createvisualtask AnimTask_SeismicTossBgAccelerateDownAtEnd, 3
	jumpreteq 0, SeismicTossWeak
	jumpreteq 1, SeismicTossMedium
	jumpreteq 2, SeismicTossStrong
SeismicTossContinue::
	call UnsetScrollingBg
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

SeismicTossWeak::
	call SeismicTossRockScatter1
	delay 16
	call SeismicTossRockScatter2
	goto SeismicTossContinue

SeismicTossMedium::
	call SeismicTossRockScatter1
	delay 14
	call SeismicTossRockScatter2
	delay 14
	call SeismicTossRockScatter1
	goto SeismicTossContinue

SeismicTossStrong::
	call SeismicTossRockScatter2
	delay 10
	call SeismicTossRockScatter1
	delay 10
	call SeismicTossRockScatter2
	delay 10
	call SeismicTossRockScatter1
	goto SeismicTossContinue

SeismicTossRockScatter1::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -10, -8, ANIM_TARGET, 1
	playsewithpan SE_M_STRENGTH, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 5, 1
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, -12, 27, 2, 3
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, 8, 28, 3, 4
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, -4, 30, 2, 3
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, 12, 25, 4, 4
	return

SeismicTossRockScatter2::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 10, -8, ANIM_TARGET, 1
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 5, 1
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, -12, 32, 3, 4
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, 8, 31, 2, 2
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, -4, 28, 2, 3
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, 12, 30, 4, 3
	return

@ Credits: Blackuser
gMoveAnim_STRENGTH::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeAndSinkMon, 5, ANIM_ATTACKER, 2, 0, 96, 30
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 12, RGB_RED
	waitforvisualfinish
	delay 10
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 4
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_TARGET, 18, 6, 2, 4
	delay 4
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 16, 12, ANIM_TARGET, 1
	delay 4
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -16, -12, ANIM_TARGET, 1
	delay 4
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 3, 4, ANIM_TARGET, 1
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_ABSORB::
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 4, RGB(13, 31, 12) @ Green color
	waitforvisualfinish
	playsewithpan SE_M_ABSORB, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	delay 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 5, 1
	waitforvisualfinish
	delay 3
	call AbsorbEffect
	waitforvisualfinish
	delay 15
	call HealingEffect
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 4, 0, RGB(13, 31, 12) @ Green color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

AbsorbEffect::
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	delay 4
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -20, 35
	delay 4
	return

gMoveAnim_MEGA_DRAIN::
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 8, RGB(13, 31, 12) @ Green color
	waitforvisualfinish
	playsewithpan SE_M_ABSORB, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 1
	delay 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 5, 1
	waitforvisualfinish
	delay 3
	call MegaDrainAbsorbEffect
	waitforvisualfinish
	delay 15
	call HealingEffect
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 8, 0, RGB(9, 25, 4) @ Green color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

MegaDrainAbsorbEffect::
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -20, 35
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	delay 4
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -20, 35
	delay 4
	return

gMoveAnim_LEECH_SEED::
	loadspritegfx ANIM_TAG_SEED
	call LaunchLeechSeeds
	end

LaunchLeechSeeds::
	playsewithpan SE_M_POISON_POWDER, SOUND_PAN_ATTACKER
	createsprite gLeechSeedSpriteTemplate, ANIM_TARGET, 2, 15, 0, 0, 24, 35, -32
	delay 8
	playsewithpan SE_M_POISON_POWDER, SOUND_PAN_ATTACKER
	createsprite gLeechSeedSpriteTemplate, ANIM_TARGET, 2, 15, 0, -16, 24, 35, -40
	delay 8
	playsewithpan SE_M_POISON_POWDER, SOUND_PAN_ATTACKER
	createsprite gLeechSeedSpriteTemplate, ANIM_TARGET, 2, 15, 0, 16, 24, 35, -37
	delay 12
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_TARGET, 10, 8
	return

@ Credits: Blackuser
gMoveAnim_GROWTH::
	createvisualtask AnimTask_GetWeather, 2
	jumpreteq ENUM_WEATHER_SUN, DoGrowthEffectLarger
	jumpreteq ENUM_WEATHER_SUN_PRIMAL, DoGrowthEffectLarger
	call GrowthEffect
	call GrowthEffect
EndGrowthAnim::
	end

DoGrowthEffectLarger::
	call GrowthEffectLarger
	call GrowthEffectLarger
	goto EndGrowthAnim

GrowthEffect::
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 8, RGB_WHITE
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -3, -3, 16, ANIM_ATTACKER, ST_OAM_OBJ_NORMAL
	waitforvisualfinish
	return

GrowthEffectLarger::
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 8, RGB_WHITE
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -6, -6, 16, ANIM_ATTACKER, ST_OAM_OBJ_NORMAL
	waitforvisualfinish
	return

gMoveAnim_RAZOR_LEAF::
	loadspritegfx ANIM_TAG_LEAF
	loadspritegfx ANIM_TAG_RAZOR_LEAF
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	delay 1
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_ATTACKER, 10, 5
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -3, -2, 10
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -1, -1, 15
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -4, -4, 7
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 3, -3, 11
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -1, -6, 8
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 2, -1, 12
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -3, -4, 13
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 4, -5, 7
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 2, -6, 11
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -3, -5, 8
	delay 60
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gRazorLeafCutterSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 22, 20, TRUE
	createsprite gRazorLeafCutterSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 22, -20, TRUE
	delay 20
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 2, 0, 8, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_SOLAR_BEAM::
	loadspritegfx ANIM_TAG_ORBS
	createvisualtask AnimTask_GetWeather, 2
	@ If sunny weather set, play full anim
	callreteq ENUM_WEATHER_SUN, SolarBeamSetUpAnim
	callreteq ENUM_WEATHER_SUN_PRIMAL, SolarBeamSetUpAnim
	choosetwoturnanim SolarBeamSetUp, SolarBeamUnleash
SolarBeamEnd::
	end

SolarBeamSetUp:
	call SolarBeamSetUpAnim
	goto SolarBeamEnd

SolarBeamSetUpAnim::
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 1, 4, 0, 11, RGB(31, 31, 11) @ Yellow color
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, 40, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, -40, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, 40, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, -40, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, -20, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, 20, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, -20, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, 20, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -20, 30, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 20, -30, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -20, -30, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 20, 30, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, 0, 16
	delay 2
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, 0, 16
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	return

SolarBeamUnleash::
	setbgbasedonside ANIM_TARGET, BG_SOLARBEAM_PLAYER, BG_SOLARBEAM_OPPONENT, TRUE
	waitbgfadein
	panse_1B SE_M_SOLAR_BEAM, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createvisualtask AnimTask_CreateSmallSolarbeamOrbs, 5
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 0
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 1
	delay 4
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 1, 0, 10, RGB(25, 31, 0) @ Yellow color
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 2
	delay 4
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 2, 0, 65, 1
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 3
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 4
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 5
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 6
	delay 4
	call SolarBeamUnleashOrbs
	call SolarBeamUnleashOrbs
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 1, 10, 0, RGB(25, 31, 0) @ Yellow color
	waitforvisualfinish
	restorebg
	waitbgfadein
	goto SolarBeamEnd

SolarBeamUnleashOrbs::
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 0
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 1
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 2
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 3
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 4
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 5
	delay 4
	createsprite gSolarbeamBigOrbSpriteTemplate, ANIM_TARGET, 3, 15, 0, 20, 6
	delay 4
	return

gMoveAnim_POISON_POWDER::
	loadspritegfx ANIM_TAG_POISON_POWDER
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_TARGET, 10, 6
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -30, -22, 117, 80, 5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 10, -22, 117, 80, -5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -25, -22, 117, 112, 5, 3
	delay 15
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -5, -22, 117, 80, -5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 5, -22, 117, 96, 5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 0, -22, 117, 69, -5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -15, -22, 117, 112, 5, 2
	delay 30
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -15, -22, 117, 112, 5, 2
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 15, -22, 117, 80, -5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -10, -22, 117, 96, 7, 2
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -5, -22, 117, 90, -8, 0
	delay 20
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, -10, -22, 117, 80, -5, 1
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 0, -22, 117, 89, 5, 2
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 20, -22, 117, 112, -8, 2
	createsprite gPoisonPowderParticleSpriteTemplate, ANIM_TARGET, 2, 5, -22, 117, 80, 5, 1
	end

gMoveAnim_STUN_SPORE::
	loadspritegfx ANIM_TAG_STUN_SPORE
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_TARGET, 10, 6
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -30, -22, 117, 80, 5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 10, -22, 117, 80, -5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -25, -22, 117, 112, 5, 3
	delay 15
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -5, -22, 117, 80, -5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 5, -22, 117, 96, 5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 0, -22, 117, 69, -5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -15, -22, 117, 112, 5, 2
	delay 30
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -15, -22, 117, 112, 5, 2
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 15, -22, 117, 80, -5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -10, -22, 117, 96, 7, 2
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -5, -22, 117, 90, -8, 0
	delay 20
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, -10, -22, 117, 80, -5, 1
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 0, -22, 117, 89, 5, 2
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 20, -22, 117, 112, -8, 2
	createsprite gStunSporeParticleSpriteTemplate, ANIM_TARGET, 2, 5, -22, 117, 80, 5, 1
	end

gMoveAnim_SLEEP_POWDER::
	loadspritegfx ANIM_TAG_SLEEP_POWDER
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_TARGET, 10, 6
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -30, -22, 117, 80, 5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 10, -22, 117, 80, -5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -25, -22, 117, 112, 5, 3
	delay 15
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -5, -22, 117, 80, -5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 5, -22, 117, 96, 5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 0, -22, 117, 69, -5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -15, -22, 117, 112, 5, 2
	delay 30
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -15, -22, 117, 112, 5, 2
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 15, -22, 117, 80, -5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -10, -22, 117, 96, 7, 2
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -5, -22, 117, 90, -8, 0
	delay 20
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, -10, -22, 117, 80, -5, 1
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 0, -22, 117, 89, 5, 2
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 20, -22, 117, 112, -8, 2
	createsprite gSleepPowderParticleSpriteTemplate, ANIM_TARGET, 2, 5, -22, 117, 80, 5, 1
	end

gMoveAnim_PETAL_DANCE::
	loadspritegfx ANIM_TAG_FLOWER
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_PETAL_DANCE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 12, 6, 6, 3
	createsprite gPetalDanceBigFlowerSpriteTemplate, ANIM_ATTACKER, 2, 0, -24, 8, 140, TRUE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, 16, -24, 8, 100, FALSE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, -16, -24, 8, 100, FALSE
	delay 15
	createsprite gPetalDanceBigFlowerSpriteTemplate, ANIM_ATTACKER, 2, 0, -24, 8, 140, TRUE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, 32, -24, 8, 100, FALSE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, -32, -24, 8, 100, FALSE
	delay 15
	createsprite gPetalDanceBigFlowerSpriteTemplate, ANIM_ATTACKER, 2, 0, -24, 8, 140, TRUE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, 24, -24, 8, 100, FALSE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, -24, -24, 8, 100, FALSE
	delay 30
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, 16, -24, 0, 100, FALSE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, -16, -24, 0, 100, FALSE
	delay 30
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, 20, -16, 14, 80, FALSE
	createsprite gPetalDanceSmallFlowerSpriteTemplate, ANIM_ATTACKER, 2, -20, -14, 16, 80, FALSE
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, 0, FALSE, 5
	delay 3
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 6, 0, 8, 1
	waitforvisualfinish
	delay 8
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 7
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_STRING_SHOT::
	loadspritegfx ANIM_TAG_STRING
	loadspritegfx ANIM_TAG_WEB_THREAD
	monbg ANIM_DEF_SIDE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 5, F_PAL_BG, 2, 0, 9, RGB_BLACK
	waitforvisualfinish
	loopsewithpan SE_M_STRING_SHOT, SOUND_PAN_ATTACKER, 9, 6
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	call WebThreadDoubles
	waitforvisualfinish
	playsewithpan SE_M_STRING_SHOT2, SOUND_PAN_TARGET
	createsprite gStringWrapSpriteTemplate, ANIM_TARGET, 2, 0, 10, TRUE
	delay 4
	createsprite gStringWrapSpriteTemplate, ANIM_TARGET, 2, 0, -2, TRUE
	delay 4
	createsprite gStringWrapSpriteTemplate, ANIM_TARGET, 2, 0, 22, TRUE
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 5, F_PAL_BG, 2, 9, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

@ Credits: Blackuser
gMoveAnim_DRAGON_RAGE::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	loadspritegfx ANIM_TAG_FIRE_PLUME
	loadspritegfx ANIM_TAG_PURPLE_FLAME @ Purple color
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 40, 1
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_TARGET, 2, ANIM_ATTACKER, 15, 0, FALSE, 4
	waitforvisualfinish
	createsprite gDragonRageFireSpitSpriteTemplate, ANIM_TARGET, 2, 30, 15, 0, 10, 10
	waitforvisualfinish
	loopsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET, 11, 3
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 3, 25, 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, 5, 0
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, -10, -15
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 25
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, 15, 5
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, -25, 0
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 30, 30
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -27, 25
	delay 1
	createsprite gDragonRageFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, 0, 8
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_TARGET, 66, ANIM_ATTACKER, 0, 4
	end

gMoveAnim_FIRE_SPIN::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	playsewithpan SE_M_SACRED_FIRE2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 2, 47, 1
	call FireSpinEffect
	call FireSpinEffect
	call FireSpinEffect
	end

FireSpinEffect::
	createsprite gFireSpinSpriteTemplate, ANIM_TARGET, 2, 0, 28, 528, 30, 13, 50, ANIM_TARGET
	delay 2
	createsprite gFireSpinSpriteTemplate, ANIM_TARGET, 2, 0, 32, 480, 20, 16, -46, ANIM_TARGET
	delay 2
	createsprite gFireSpinSpriteTemplate, ANIM_TARGET, 2, 0, 33, 576, 20, 8, 42, ANIM_TARGET
	delay 2
	createsprite gFireSpinSpriteTemplate, ANIM_TARGET, 2, 0, 31, 400, 25, 11, -42, ANIM_TARGET
	delay 2
	createsprite gFireSpinSpriteTemplate, ANIM_TARGET, 2, 0, 28, 512, 25, 16, 46, ANIM_TARGET
	delay 2
	createsprite gFireSpinSpriteTemplate, ANIM_TARGET, 2, 0, 33, 464, 30, 15, -50, ANIM_TARGET
	delay 2
	return

@ Credits: Blackuser
gMoveAnim_THUNDER_SHOCK::
	loadspritegfx ANIM_TAG_SPARK
	loadspritegfx ANIM_TAG_SPARK_2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 6, RGB_BLACK
	waitforvisualfinish
	delay 10
	createvisualtask AnimTask_ElectricBolt, 5, 0, -44, FALSE
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_TARGET
	delay 9
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 0, 0, 13, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 0, 13, 0, RGB_BLACK
	waitforvisualfinish
	delay 20
	call ElectricityEffect
	waitforvisualfinish
	delay 20
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 6, 0, RGB_BLACK
	end

@ Credits: Blackuser
gMoveAnim_THUNDERBOLT::
	loadspritegfx ANIM_TAG_SPARK
	loadspritegfx ANIM_TAG_SPARK_2
	loadspritegfx ANIM_TAG_SHOCK_3
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 6, RGB_BLACK
	waitforvisualfinish
	delay 10
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 16, 1
	createvisualtask AnimTask_ElectricBolt, 5, 24, -52, FALSE
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_TARGET
	delay 7
	createvisualtask AnimTask_ElectricBolt, 5, -24, -52, FALSE
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_TARGET
	delay 7
	createvisualtask AnimTask_ElectricBolt, 5, 0, -60, TRUE
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_TARGET
	delay 9
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 0, 0, 13, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 0, 13, 0, RGB_BLACK
	waitforvisualfinish
	delay 20
	createsprite gThunderboltOrbSpriteTemplate, ANIM_TARGET, 3, 44, 0, 0, 3
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 32, 44, 0, 40, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 32, 44, 64, 40, 1
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 32, 44, 128, 40, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 32, 44, 192, 40, 2
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 16, 44, 32, 40, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 16, 44, 96, 40, 1
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 16, 44, 160, 40, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, -32764, 16, 44, 224, 40, 2
	playsewithpan SE_M_HYPER_BEAM, SOUND_PAN_TARGET
	delay 0
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 2, 2, RGB_BLACK
	delay 6
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 6, 6, RGB_BLACK
	delay 6
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 2, 2, RGB_BLACK
	delay 6
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 6, 6, RGB_BLACK
	waitforvisualfinish
	delay 20
	waitplaysewithpan SE_M_THUNDERBOLT2, SOUND_PAN_TARGET, 19
	call ElectricityEffect
	waitforvisualfinish
	delay 20
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 6, 0, RGB_BLACK
	end

gMoveAnim_THUNDER_WAVE::
	loadspritegfx ANIM_TAG_SPARK
	loadspritegfx ANIM_TAG_SPARK_2
	loadspritegfx ANIM_TAG_SPARK_H
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 6, RGB_BLACK
	waitforvisualfinish
	delay 10
	createvisualtask AnimTask_ElectricBolt, 5, 0, -48, FALSE
	playsewithpan SE_M_THUNDER_WAVE, SOUND_PAN_TARGET
	delay 20
	loopsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_TARGET, 10, 4
	createsprite gThunderWaveSpriteTemplate, ANIM_TARGET, 2, 0, 10, FALSE
	delay 4
	createsprite gThunderWaveSpriteTemplate, ANIM_TARGET, 2, 0, -2, FALSE
	delay 4
	createsprite gThunderWaveSpriteTemplate, ANIM_TARGET, 2, 0, 22, FALSE
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 6, 0, RGB_BLACK
	end

gMoveAnim_THUNDER::
	loadspritegfx ANIM_TAG_LIGHTNING
	fadetobg BG_THUNDER
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -256, 0, TRUE
	waitbgfadein
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 16, RGB_BLACK
	delay 16
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	playsewithpan SE_M_THUNDER_WAVE, SOUND_PAN_TARGET
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 16, -36
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 16, -20
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 16, 12
	delay 20
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 6, -16, -32
	playsewithpan SE_M_THUNDER_WAVE, SOUND_PAN_TARGET
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 6, -16, -16
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 6, -16, 16
	playsewithpan SE_M_THUNDER_WAVE, SOUND_PAN_TARGET
	delay 5
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 24, -32
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 24, -16
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 24, 16
	delay 30
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 5
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 0, -32
	playsewithpan SE_M_TRI_ATTACK2, SOUND_PAN_TARGET
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 0, -16
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 0, 16
	delay 10
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	createvisualtask AnimTask_ShakeTargetInPattern, 2, 30, 3, 1, 0
	delay 2
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_TARGET, 2, F_PAL_BG, 2, 16, 0, RGB_BLACK
	waitforvisualfinish
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_ROCK_THROW::
	loadspritegfx ANIM_TAG_SMALL_ROCK
	loadspritegfx ANIM_TAG_ROCKS
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 0, 1, 0, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 12
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 5, 20, 1
	createsprite gScatterSmallRockSpriteTemplate, ANIM_TARGET, 2, -12, 17, 2, 0
	createsprite gScatterSmallRockSpriteTemplate, ANIM_TARGET, 2, 8, 18, 3, 0
	createsprite gScatterSmallRockSpriteTemplate, ANIM_TARGET, 2, -4, 20, 2, 0
	createsprite gScatterSmallRockSpriteTemplate, ANIM_TARGET, 2, 12, 15, 4, 0
	end

gMoveAnim_EARTHQUAKE::
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT + 1, 10, 50
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT, 10, 50
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_ABOVE
	delay 10
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	delay 16
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	end

gMoveAnim_FISSURE::
	loadspritegfx ANIM_TAG_MUD_SAND
	createvisualtask AnimTask_HorizontalShake, 3, MAX_BATTLERS_COUNT + 1, 10, 50
	createvisualtask AnimTask_HorizontalShake, 3, ANIM_TARGET, 10, 50
	playsewithpan SE_M_EARTHQUAKE, SOUND_PAN_TARGET
	delay 8
	call FissureDirtPlumeFar
	delay 15
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	delay 15
	call FissureDirtPlumeClose
	delay 15
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	delay 15
	call FissureDirtPlumeFar
	delay 50
	call SetFissureBackground
	delay 40
	call UnsetScrollingBg
	end

FissureDirtPlumeFar::
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 12, -48, -16, 24
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 16, -16, -10, 24
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 1, 14, -52, -18, 24
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 1, 12, -32, -16, 24
	playsewithpan SE_M_DIG, SOUND_PAN_TARGET
	return

FissureDirtPlumeClose::
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 12, -24, -16, 24
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 16, -38, -10, 24
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 1, 14, -20, -18, 24
	createsprite gDirtPlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 1, 12, -36, -16, 24
	playsewithpan SE_M_DIG, SOUND_PAN_TARGET
	return

gMoveAnim_DIG::
	loadspritegfx ANIM_TAG_DIRT_MOUND
	choosetwoturnanim DigSetUp, DigUnleash
DigEnd::
	end

DigSetUp::
	loadspritegfx ANIM_TAG_MUD_SAND
	createsprite gDirtMoundSpriteTemplate, ANIM_ATTACKER, 1, ANIM_ATTACKER, 0, 180
	createsprite gDirtMoundSpriteTemplate, ANIM_ATTACKER, 1, ANIM_ATTACKER, 1, 180
	monbg_22 ANIM_ATTACKER
	delay 1
	createvisualtask AnimTask_DigDownMovement, 2, FALSE
	delay 6
	call DigThrowDirt
	call DigThrowDirt
	call DigThrowDirt
	call DigThrowDirt
	call DigThrowDirt
	waitforvisualfinish
	clearmonbg_23 ANIM_ATTACKER
	delay 1
	createvisualtask AnimTask_DigDownMovement, 2, TRUE
	goto DigEnd

DigThrowDirt::
	createsprite gDirtPlumeSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 12, 4, -16, 18
	createsprite gDirtPlumeSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 16, 4, -10, 18
	createsprite gDirtPlumeSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 14, 4, -18, 18
	createsprite gDirtPlumeSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 12, 4, -16, 18
	playsewithpan SE_M_DIG, SOUND_PAN_ATTACKER
	delay 32
	return

DigUnleash::
	loadspritegfx ANIM_TAG_IMPACT
	createvisualtask AnimTask_DigUpMovement, 2, FALSE
	waitforvisualfinish
	monbg ANIM_ATTACKER
	createsprite gDirtMoundSpriteTemplate, ANIM_ATTACKER, 1, ANIM_ATTACKER, 0, 48
	createsprite gDirtMoundSpriteTemplate, ANIM_ATTACKER, 1, ANIM_ATTACKER, 1, 48
	delay 1
	createvisualtask AnimTask_DigUpMovement, 2, TRUE
	delay 16
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -8, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_ATTACKER
	clearmonbg ANIM_ATTACKER
	goto DigEnd

gMoveAnim_TOXIC::
	loadspritegfx ANIM_TAG_TOXIC_BUBBLE
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	call ToxicBubbles
	call ToxicBubbles
	waitforvisualfinish
	delay 15
	call PoisonBubblesEffect
	end

ToxicBubbles::
	createsprite gToxicBubbleSpriteTemplate, ANIM_TARGET, 2, -24, 16, ANIM_TARGET, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 15
	createsprite gToxicBubbleSpriteTemplate, ANIM_TARGET, 2, 8, 16, ANIM_TARGET, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 15
	createsprite gToxicBubbleSpriteTemplate, ANIM_TARGET, 2, -8, 16, ANIM_TARGET, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 15
	createsprite gToxicBubbleSpriteTemplate, ANIM_TARGET, 2, 24, 16, ANIM_TARGET, FALSE
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	delay 15
	return

gMoveAnim_CONFUSION::
	monbg ANIM_DEF_SIDE
	call SetPsychicBackground
	setalpha 8, 8
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 10, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 8, RGB_WHITE
	waitforvisualfinish
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 15, 1
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -4, -4, 15, ANIM_TARGET, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

gMoveAnim_PSYCHIC::
	monbg ANIM_DEF_SIDE
	call SetPsychicBackground
	setalpha 8, 8
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 10, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 8, RGB(31, 23, 0) @ Yellow color
	waitforvisualfinish
	loopsewithpan SE_M_SUPERSONIC, SOUND_PAN_TARGET, 10, 3
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 15, 1
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -6, -6, 15, ANIM_TARGET, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

gMoveAnim_HYPNOSIS::
	loadspritegfx ANIM_TAG_GOLD_RING
	call SetPsychicBackground
	call HypnosisRings
	call HypnosisRings
	call HypnosisRings
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 12, RGB(31, 18, 31) @ Purple color
	waitforvisualfinish
	call UnsetScrollingBg
	end

HypnosisRings::
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	createsprite gGoldRingSpriteTemplate, ANIM_TARGET, 2, 0, 8, 0, 8, 27, 0
	createsprite gGoldRingSpriteTemplate, ANIM_TARGET, 2, 16, -8, 0, -8, 27, 0
	delay 6
	return

gMoveAnim_MEDITATE::
	call SetPsychicBackground
	createvisualtask AnimTask_MeditateStretchAttacker, 2
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	delay 16
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	waitforvisualfinish
	call UnsetScrollingBg
	end

gMoveAnim_AGILITY::
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 24, 6, 4, 4
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 7, 10
	call AgilitySoundEffect
	call AgilitySoundEffect
	call AgilitySoundEffect
	call AgilitySoundEffect
	call AgilitySoundEffect
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

AgilitySoundEffect::
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	delay 12
	return

gMoveAnim_QUICK_ATTACK::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 24, 6, 1, 5
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 7, 3
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	delay 4
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_RAGE::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_ANGER
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendMonInAndOut, 3, ANIM_ATTACKER, RGB_RED, 10, 0, 2
	createsprite gAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	delay 20
	createsprite gAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 4
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, TRUE, 1, 10, TRUE, FALSE
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

gMoveAnim_TELEPORT::
	call SetPsychicBackground
	createvisualtask AnimTask_Teleport, 2 
	playsewithpan SE_M_TELEPORT, SOUND_PAN_ATTACKER
	delay 15
	call UnsetScrollingBg
	waitforvisualfinish
	end

gMoveAnim_NIGHT_SHADE::
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	fadetobg BG_GHOST
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	waitbgfadein
	delay 10
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_NightShadeClone, 5, 85
	delay 70
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 12, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 0, 2, 0, 13, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	restorebg
	waitbgfadein
	end

gMoveAnim_MIMIC::
gMoveAnim_ME_FIRST::
	loadspritegfx ANIM_TAG_ORBS
	monbg_22 ANIM_DEF_SIDE
	setalpha 11, 5
	panse_1B SE_M_MINIMIZE, SOUND_PAN_TARGET, SOUND_PAN_ATTACKER, 253, 0
	createvisualtask AnimTask_ShrinkBattlerCopy, 5, 128, 24, ANIM_TARGET
	delay 15
	createsprite gMimicOrbSpriteTemplate, ANIM_TARGET, 2, -12, 24
	delay 10
	setargret -1 @ Signal to end shink anim
	waitforvisualfinish
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 11, RGB_WHITE
	waitforvisualfinish
	clearmonbg_23 ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_SCREECH::
	loadspritegfx ANIM_TAG_PURPLE_RING
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 3, 0, 2, 1
	call ScreechRing
	call ScreechRing
	delay 16
	createvisualtask AnimTask_SwayMon, 5, 0, 6, 2048, 2, ANIM_TARGET
	end

ScreechRing::
	playsewithpan SE_M_SCREECH, SOUND_PAN_ATTACKER
	createsprite gScreechWaveSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 30, 0
	delay 2
	return

gMoveAnim_DOUBLE_TEAM::
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_DoubleTeam, 2
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	delay 32
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	delay 24
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	delay 16
	call DoubleTeamSoundEffect
	call DoubleTeamSoundEffect
	call DoubleTeamSoundEffect
	call DoubleTeamSoundEffect
	call DoubleTeamSoundEffect
	call DoubleTeamSoundEffect
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

DoubleTeamSoundEffect::
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	delay 8
	return

gMoveAnim_RECOVER::
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	loopsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER, 13, 3
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 6, 0, 11, RGB(31, 31, 11) @ Yellow color
	call RecoverAbsorbEffect
	call RecoverAbsorbEffect
	call RecoverAbsorbEffect
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	call HealingEffect
	end

RecoverAbsorbEffect::
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, -10, 13
	delay 3
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -35, -10, 13
	delay 3
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 15, -40, 13
	delay 3
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -10, -32, 13
	delay 3
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 25, -20, 13
	delay 3
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, -20, 13
	delay 3
	createsprite gPowerAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 5, -40, 13
	delay 3
	return

gMoveAnim_HARDEN::
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	end

gMoveAnim_MINIMIZE::
	setalpha 10, 8
	createvisualtask AnimTask_Minimize, 2
	loopsewithpan SE_M_MINIMIZE, SOUND_PAN_ATTACKER, 34, 3
	waitforvisualfinish
	blendoff
	end

gMoveAnim_SMOKESCREEN::
	loadspritegfx ANIM_TAG_BLACK_SMOKE
	loadspritegfx ANIM_TAG_BLACK_BALL
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	createsprite gBlackBallSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 35, -25
	waitforvisualfinish
	createvisualtask AnimTask_SmokescreenImpact, 2
	delay 2
	call SmokescreenBlackSmoke
	end

SmokescreenBlackSmoke::
	playsewithpan SE_M_SAND_ATTACK, SOUND_PAN_TARGET
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -12, 104, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -12, -72, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -6, -56, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -6, 88, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 0, 56, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 0, -88, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 6, 72, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 6, -104, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 12, 72, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 12, -56, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 18, 80, 75
	createsprite gBlackSmokeSpriteTemplate, ANIM_TARGET, 4, 0, 18, -72, 75
	return

gMoveAnim_CONFUSE_RAY::
	loadspritegfx ANIM_TAG_YELLOW_BALL
	monbg ANIM_DEF_SIDE
	fadetobg BG_GHOST
	waitbgfadein
	createvisualtask SoundTask_AdjustPanningVar, 2, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createvisualtask AnimTask_BlendColorCycleByTag, 2, ANIM_TAG_YELLOW_BALL, 0, 6, 0, 14, RGB(31, 10, 0) @ Red color
	createsprite gConfuseRayBallBounceSpriteTemplate, ANIM_TARGET, 2, 28, 0, 288
	waitforvisualfinish
	setalpha 8, 8
	playsewithpan SE_M_STRING_SHOT2, SOUND_PAN_TARGET
	createsprite gConfuseRayBallSpiralSpriteTemplate, ANIM_TARGET, 2, 0, -16
	waitforvisualfinish
	blendoff
	clearmonbg ANIM_DEF_SIDE
	restorebg
	waitbgfadein
	end

@ Credits: Skeli and Blackuser
gMoveAnim_WITHDRAW::
	loadspritegfx ANIM_TAG_SHELL
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShellSmashShrinkAttacker, 2
	createsprite gLeftRightShellSpriteTemplate, ANIM_ATTACKER, 2, -40, -6, 16, 0
	createsprite gLeftRightShellSpriteTemplate, ANIM_ATTACKER, 2, 40, 6, 16, 1
	delay 64
	createvisualtask AnimTask_FadeOutParticles, 2, 0
	end

gMoveAnim_DEFENSE_CURL::
	loadspritegfx ANIM_TAG_ECLIPSING_ORB
	loopsewithpan SE_M_TRI_ATTACK, SOUND_PAN_ATTACKER, 18, 3
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_ATTACKER, FALSE
	createvisualtask AnimTask_DefenseCurlDeformMon, 5
	waitforvisualfinish
	createsprite gEclipsingOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, 6, ANIM_ATTACKER, FALSE
	waitforvisualfinish
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_ATTACKER, TRUE
	end

gMoveAnim_BARRIER::
	loadspritegfx ANIM_TAG_GRAY_LIGHT_WALL
	setalpha 0, 16
	waitplaysewithpan SE_M_BARRIER, SOUND_PAN_ATTACKER, 15
	createsprite gBarrierWallSpriteTemplate, ANIM_ATTACKER, 3, 40, 0, ANIM_TAG_GRAY_LIGHT_WALL
	waitforvisualfinish
	blendoff
	end

gMoveAnim_LIGHT_SCREEN::
	loadspritegfx ANIM_TAG_GREEN_LIGHT_WALL
	loadspritegfx ANIM_TAG_SPARKLE_3
	setalpha 0, 16
	waitplaysewithpan SE_M_REFLECT, SOUND_PAN_ATTACKER, 15
	createsprite gLightScreenWallSpriteTemplate, ANIM_ATTACKER, 1, 40, 0, ANIM_TAG_GREEN_LIGHT_WALL
	delay 10
	call SpecialScreenSparkle
	waitforvisualfinish
	blendoff
	end

SpecialScreenSparkle::
	createsprite gSpecialScreenSparkleSpriteTemplate, ANIM_ATTACKER, 2, 23, 0, ANIM_ATTACKER
	delay 6
	createsprite gSpecialScreenSparkleSpriteTemplate, ANIM_ATTACKER, 2, 31, -8, ANIM_ATTACKER
	delay 5
	createsprite gSpecialScreenSparkleSpriteTemplate, ANIM_ATTACKER, 2, 30, 20, ANIM_ATTACKER
	delay 7
	createsprite gSpecialScreenSparkleSpriteTemplate, ANIM_ATTACKER, 2, 10, -15, ANIM_ATTACKER
	delay 6
	createsprite gSpecialScreenSparkleSpriteTemplate, ANIM_ATTACKER, 2, 20, 10, ANIM_ATTACKER
	delay 6
	createsprite gSpecialScreenSparkleSpriteTemplate, ANIM_ATTACKER, 2, 10, 18, ANIM_ATTACKER
	return

gMoveAnim_HAZE::
	playsewithpan SE_M_HAZE, SOUND_PAN_ABOVE
	createvisualtask AnimTask_Haze1, 5
	delay 30
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BATTLERS_2, 2, 0, 16, RGB_BLACK
	delay 90
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BATTLERS_2, 1, 16, 0, RGB_BLACK
	end

gMoveAnim_REFLECT::
	loadspritegfx ANIM_TAG_BLUE_LIGHT_WALL
	loadspritegfx ANIM_TAG_SPARKLE_4
	setalpha 0, 16
	waitplaysewithpan SE_M_REFLECT, SOUND_PAN_ATTACKER, 15
	createsprite gReflectWallSpriteTemplate, ANIM_ATTACKER, 1, 40, 0, ANIM_TAG_BLUE_LIGHT_WALL
	delay 20
	createsprite gReflectSparkleSpriteTemplate, ANIM_ATTACKER, 2, 30, 0, ANIM_ATTACKER
	delay 7
	createsprite gReflectSparkleSpriteTemplate, ANIM_ATTACKER, 2, 19, -12, ANIM_ATTACKER
	delay 7
	createsprite gReflectSparkleSpriteTemplate, ANIM_ATTACKER, 2, 10, 20, ANIM_ATTACKER
	waitforvisualfinish
	blendoff
	end

gMoveAnim_FOCUS_ENERGY::
	loadspritegfx ANIM_TAG_FOCUS_ENERGY
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	call EndureEffect
	delay 8
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 11, RGB_WHITE
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 32, 1
	call EndureEffect
	delay 8
	call EndureEffect
	end

EndureEffect::
	createsprite gEndureEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, 26, 2
	delay 4
	createsprite gEndureEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 14, 28, 1
	delay 4
	createsprite gEndureEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -5, 10, 2
	delay 4
	createsprite gEndureEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 28, 26, 3
	delay 4
	createsprite gEndureEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -12, 0, 1
	return

gMoveAnim_BIDE::
	choosetwoturnanim BideSetUp, BideRelease
BideEndAnim::
	end

BideSetUp::
	loopsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER, 9, 2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 11, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 32, 1
	goto BideEndAnim

BideRelease::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	loopsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER, 9, 2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 2, 0, 11, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 32, 1
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, 0, FALSE, 4
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 12, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 16, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 1, 18, -8, ANIM_TARGET, 1
	delay 5
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 1, -18, 8, ANIM_TARGET, 1
	delay 5
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 1, -8, -5, ANIM_TARGET, 1
	waitforvisualfinish
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 7
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 2, 11, 0, RGB_RED
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	goto BideEndAnim

gMoveAnim_METRONOME::
	loadspritegfx ANIM_TAG_FINGER
	loadspritegfx ANIM_TAG_THOUGHT_BUBBLE
	createsprite gThoughtBubbleSpriteTemplate, ANIM_ATTACKER, 11, ANIM_ATTACKER, 100
	playsewithpan SE_M_METRONOME, SOUND_PAN_ATTACKER
	delay 6
	createsprite gMetronomeFingerSpriteTemplate, ANIM_ATTACKER, 12, ANIM_ATTACKER
	delay 24
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 22, 3
	end

gMoveAnim_SELF_DESTRUCT::
	loadspritegfx ANIM_TAG_EXPLOSION
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 1, 0, 9, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT, 6, 0, 38, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 1, 6, 0, 38, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 2, 6, 0, 38, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 3, 6, 0, 38, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 4, 6, 0, 38, 1
	call SelfDestructExplode
	call SelfDestructExplode
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 1, 9, 0, RGB_RED
	end

SelfDestructExplode::
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 24, -24, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -16, 16, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -24, -12, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 16, 16, ANIM_ATTACKER, FALSE
	delay 6
	return

gMoveAnim_EGG_BOMB::
	loadspritegfx ANIM_TAG_EXPLOSION
	loadspritegfx ANIM_TAG_LARGE_FRESH_EGG
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	createsprite gEggThrowSpriteTemplate, ANIM_TARGET, 2, 10, 0, 0, 0, 25, -32
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 16, 1
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 4, 6, 5, ANIM_TARGET, TRUE
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	delay 3
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 4, -16, -15, ANIM_TARGET, TRUE
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	delay 3
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 4, 16, -5, ANIM_TARGET, TRUE
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	delay 3
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 4, -12, 18, ANIM_TARGET, TRUE
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	delay 3
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 4, 0, 5, ANIM_TARGET, TRUE
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	end

gMoveAnim_LICK::
	loadspritegfx ANIM_TAG_LICK
	delay 15
	playsewithpan SE_M_LICK, SOUND_PAN_TARGET
	createsprite gLickSpriteTemplate, ANIM_TARGET, 2, 0, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 16, 1
	end

@ Credits: Skeli
gMoveAnim_SMOG::
	loadspritegfx ANIM_TAG_BLACK_SMOKE
	call SmokescreenBlackSmoke
	delay 20
	loopsewithpan SE_M_TOXIC, SOUND_PAN_TARGET, 18, 2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 12, RGB(26, 0, 26) @ Purple color
	delay 10
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 15, 1
	end

gMoveAnim_SLUDGE::
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	createsprite gSludgeProjectileSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 40, -30, FALSE
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 3, 0, 5, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 1, 2, 0, 12, RGB(30, 0, 31) @ Purple color
	call PoisonBubblesEffect
	end

gMoveAnim_BONE_CLUB::
	loadspritegfx ANIM_TAG_BONE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_BONEMERANG, SOUND_PAN_TARGET
	createsprite gSpinningBoneSpriteTemplate, ANIM_ATTACKER, 2, -42, -25, 0, 0, 15
	delay 12
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 5, 1
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_ATTACKER | F_PAL_TARGET, 5, 1, RGB_BLACK, 10, RGB_BLACK, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_FIRE_BLAST::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	createsoundtask SoundTask_FireBlast, SE_M_FLAME_WHEEL, SE_M_FLAME_WHEEL2
	call FireBlastRing
	call FireBlastRing
	call FireBlastRing
	delay 24
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 3, 0, 8, RGB_BLACK
	waitforvisualfinish
	delay 19
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 20, 1
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	call FireBlastCross
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 2, 8, 0, RGB_BLACK
	end

FireBlastRing::
	createsprite gFireBlastRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 0
	createsprite gFireBlastRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 51
	createsprite gFireBlastRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 102
	createsprite gFireBlastRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 153
	createsprite gFireBlastRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 204
	delay 5
	return

FireBlastCross::
	createsprite gFireBlastCrossSpriteTemplate, ANIM_TARGET, 2, 0, 0, 10, 0, -2
	createsprite gFireBlastCrossSpriteTemplate, ANIM_TARGET, 2, 0, 0, 13, -2, 0
	createsprite gFireBlastCrossSpriteTemplate, ANIM_TARGET, 2, 0, 0, 13, 2, 0
	createsprite gFireBlastCrossSpriteTemplate, ANIM_TARGET, 2, 0, 0, 15, -2, 2
	createsprite gFireBlastCrossSpriteTemplate, ANIM_TARGET, 2, 0, 0, 15, 2, 2
	delay 3
	return

gMoveAnim_WATERFALL::
	loadspritegfx ANIM_TAG_WATER_IMPACT
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	fadetobg BG_WATERFALL
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, 0, 61440, FALSE
	waitbgfadein
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 23, 1
	delay 5
	call SmallWaterBubblesOnAttacker
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 5
	delay 6
	call RisingWaterHitEffect
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

RisingWaterHitEffect::
	playsewithpan SE_M_WATERFALL, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 4, 0, 17, 1
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 20, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 20
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 20
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 15, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 15
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 15
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 10, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 10
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 10
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 5
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 5
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 0
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, 0
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -5, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -5
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -5
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -10, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -10
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -10
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -15
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -15
	delay 2
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, -20, ANIM_TARGET, 1
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -20
	createsprite gSmallDriftingBubblesSpriteTemplate, ANIM_ATTACKER, 4, 0, -20
	return

@ Credits: Skeli
gMoveAnim_CLAMP::
	loadspritegfx ANIM_TAG_SHELL
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	playsewithpan SE_M_VICEGRIP, SOUND_PAN_TARGET
	createsprite gClampShellSpriteTemplate, ANIM_TARGET, 3, -32, 0, 2, 819, 0, 10, 0
	createsprite gClampShellSpriteTemplate, ANIM_TARGET, 3, 32, 0, 6, -819, 0, 10, 1
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 3, 0, 5, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

gMoveAnim_SWIFT::
	loadspritegfx ANIM_TAG_YELLOW_STAR
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gSwiftStarSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 22, 20, TRUE
	delay 5
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gSwiftStarSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 5, 22, -18, TRUE
	delay 5
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gSwiftStarSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, -10, 22, 15, TRUE
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 18, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 2, 0, 18, 1
	delay 5
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gSwiftStarSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 22, -20, TRUE
	delay 5
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gSwiftStarSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 22, 12, TRUE
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_SKULL_BASH::
	choosetwoturnanim SkullBashSetUp, SkullBashAttack
SkullBashEnd::
	end

SkullBashSetUp::
	call SkullBashSetUpHeadDown
	call SkullBashSetUpHeadDown
	goto SkullBashEnd

SkullBashSetUpHeadDown::
	createsprite gSlideMonToOffsetAndBackSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, 0, FALSE, 10, FALSE
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_RotateMonSpriteToSide, 2, 16, 96, ANIM_ATTACKER, 2
	waitforvisualfinish
	createsprite gSlideMonToOffsetAndBackSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, 0, FALSE, 10, TRUE
	waitforvisualfinish
	return

SkullBashAttack::
	loadspritegfx ANIM_TAG_IMPACT
	createvisualtask AnimTask_SkullBashPosition, 2, 0
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	waitforvisualfinish
	playse SE_BANG
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 40, 1
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 10, 0, 40, 1
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, 0, ANIM_TARGET, 0
	loopsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET, 8, 3
	waitforvisualfinish
	createvisualtask AnimTask_SkullBashPosition, 2, 1
	goto SkullBashEnd

gMoveAnim_SPIKE_CANNON::
	loadspritegfx ANIM_TAG_NEEDLE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_WindUpLunge, 5, ANIM_ATTACKER, -4, 0, 4, 6, 8, 4
	waitforvisualfinish
	loopsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER, 5, 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	createsprite gLinearStingerSpriteTemplate, ANIM_ATTACKER, 2, 10, -8, -8, -8, 20
	createsprite gLinearStingerSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 0, 0, 20
	createsprite gLinearStingerSpriteTemplate, ANIM_ATTACKER, 2, 26, 8, 8, 8, 20
	waitforvisualfinish
	createsprite gHandleInvertHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -8, -8, ANIM_TARGET, 2
	createsprite gHandleInvertHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createsprite gHandleInvertHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 8, 8, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 7, 1
	loopsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 5, 3
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_CONSTRICT::
	loadspritegfx ANIM_TAG_TENDRILS
	loopsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET, 6, 4
	createsprite gConstrictBindingSpriteTemplate, ANIM_TARGET, 4, 0, 16, 0, 2
	delay 7
	createsprite gConstrictBindingSpriteTemplate, ANIM_TARGET, 3, 0, 0, 0, 2
	createsprite gConstrictBindingSpriteTemplate, ANIM_TARGET, 2, 0, 8, 1, 2
	delay 7
	createsprite gConstrictBindingSpriteTemplate, ANIM_TARGET, 3, 0, -8, 1, 2
	delay 8
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 6, 1
	delay 20
	playsewithpan SE_M_BIND, SOUND_PAN_TARGET
	setargret -1 @ Signal given to advance the anim to the next step
	end

gMoveAnim_AMNESIA::
	loadspritegfx ANIM_TAG_AMNESIA
	call SetPsychicBackground
AmnesiaAnim::
	delay 8
	createsprite gQuestionMarkSpriteTemplate, ANIM_ATTACKER, 20
	playsewithpan SE_M_METRONOME, SOUND_PAN_ATTACKER
	delay 54
	loopsewithpan SE_M_METRONOME, SOUND_PAN_ATTACKER, 16, 3
	waitforvisualfinish
	call UnsetScrollingBg
	end

gMoveAnim_KINESIS::
	loadspritegfx ANIM_TAG_ALERT
	loadspritegfx ANIM_TAG_BENT_SPOON
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	call SetPsychicBackground
	createsprite gBentSpoonSpriteTemplate, ANIM_ATTACKER, 20
	createsprite gKinesisZapEnergySpriteTemplate, ANIM_ATTACKER, 19, 32, -8, FALSE
	createsprite gKinesisZapEnergySpriteTemplate, ANIM_ATTACKER, 19, 32, 16, TRUE
	loopsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_ATTACKER, 21, 2
	delay 60
	playsewithpan SE_M_DIZZY_PUNCH, SOUND_PAN_ATTACKER
	delay 30
	loopsewithpan SE_M_DIZZY_PUNCH, SOUND_PAN_ATTACKER, 20, 2
	delay 70
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	waitforvisualfinish
	call UnsetScrollingBg
	end

gMoveAnim_SOFT_BOILED::
	loadspritegfx ANIM_TAG_BREAKING_EGG
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_BLUE_STAR
	monbg ANIM_ATK_SIDE
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon, 2, ANIM_ATTACKER, 0, 2, 6, 1
	createsprite gSoftBoiledEggSpriteTemplate, ANIM_ATTACKER, 4, 0, 16, 0
	createsprite gSoftBoiledEggSpriteTemplate, ANIM_ATTACKER, 4, 0, 16, 1
	delay 127
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_ATTACKER
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 10, 0, RGB(12, 24, 30) @ Gray color
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 31, 16, ANIM_ATTACKER, FALSE
	delay 8
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 31, 16, ANIM_ATTACKER, FALSE
	delay 60
	setargret -1 @ Signal to end anim
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	call HealingEffect
	end

gMoveAnim_HIGH_JUMP_KICK::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, 0, FALSE, 8
	waitforvisualfinish
	delay 10
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 3
	delay 2
	createsprite gJumpKickSpriteTemplate, ANIM_ATTACKER, 2, -16, 8, 0, 0, 10, ANIM_TARGET, 1, TRUE
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -28, 0, FALSE, 3
	delay 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 3, 0, 11, 1
	waitforvisualfinish
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_GLARE::
	loadspritegfx ANIM_TAG_SMALL_RED_EYE
	loadspritegfx ANIM_TAG_EYE_SPARKLE
	createvisualtask AnimTask_GlareEyeDots, 5
	playsewithpan SE_M_PSYBEAM2, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 5, F_PAL_BG, 0, 0, 16, RGB_BLACK
	waitforvisualfinish
	call EyeSparkles
	createvisualtask AnimTask_ScaryFace, 5
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	delay 2
	createvisualtask AnimTask_ShakeTargetInPattern, 3, 20, 1, 0
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 5, F_PAL_BG, 0, 16, 0, RGB_BLACK
	end

gMoveAnim_DREAM_EATER::
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 8, 8
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	call SetPsychicBackground
	playsewithpan SE_M_MINIMIZE, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 15, 1
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -6, -6, 15, ANIM_TARGET, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	setalpha 12, 8
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 2, 25, 1
	call DreamEaterAbsorb
	waitforvisualfinish
	delay 15
	call HealingEffect
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

DreamEaterAbsorb::
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -40, 35
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 28, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 40, 39
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -32, 26
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -40, 26
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 36, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -20, 35
	delay 4
	return

@ Credits: Blackuser
gMoveAnim_POISON_GAS::
	loadspritegfx ANIM_TAG_PURPLE_GAS_CLOUD
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	loopsewithpan SE_M_MIST, SOUND_PAN_TARGET, 20, 15
	call PoisonGasCloud
	call PoisonGasCloud
	call PoisonGasCloud
	call PoisonGasCloud
	call PoisonGasCloud
	call PoisonGasCloud
	call PoisonGasCloud
	delay 32
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_DEF_SIDE, 6, 2, 0, 12, RGB(26, 0, 26) @ Purple color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end
	
PoisonGasCloud::
	createsprite gPoisonGasCloudSpriteTemplate, ANIM_TARGET, 2, 0, -24, 48, 240, ANIM_TARGET, TRUE
	delay 7
	return

gMoveAnim_BARRAGE::
	loadspritegfx ANIM_TAG_RED_BALL
	createvisualtask AnimTask_BarrageBall, 3
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	delay 24
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, 8, 1, 40, 1, ANIM_ATTACKER
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 0, 4, 20, 1
	createvisualtask AnimTask_ShakeMon, 3, ANIM_DEF_PARTNER, 0, 4, 20, 1
	loopsewithpan SE_M_STRENGTH, SOUND_PAN_TARGET, 8, 2
	end

@ Credits: Blackuser
gMoveAnim_LEECH_LIFE::
	loadspritegfx ANIM_TAG_FANG_ATTACK
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gFangSpriteTemplate, ANIM_TARGET, 2
	delay 10
	playsewithpan SE_M_ABSORB, SOUND_PAN_TARGET
	delay 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 5, 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 7, RGB_BLACK
	waitforvisualfinish
	call GigaDrainAbsorbEffect
	waitforvisualfinish
	delay 15
	call HealingEffect
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 7, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_LOVELY_KISS::
	loadspritegfx ANIM_TAG_PINK_HEART
	loadspritegfx ANIM_TAG_DEVIL
	createsprite gDevilSpriteTemplate, ANIM_TARGET, 2, 0, -24
	playsewithpan SE_M_PSYBEAM2, SOUND_PAN_TARGET
	waitforvisualfinish
	playsewithpan SE_M_ATTRACT, SOUND_PAN_TARGET
	createsprite gPinkHeartSpriteTemplate, ANIM_TARGET, 3, -256, -42
	createsprite gPinkHeartSpriteTemplate, ANIM_TARGET, 3, 128, -14
	createsprite gPinkHeartSpriteTemplate, ANIM_TARGET, 3, 416, -38
	createsprite gPinkHeartSpriteTemplate, ANIM_TARGET, 3, -128, -22
	end

@ Credits: Blackuser
gMoveAnim_SKY_ATTACK::
	choosetwoturnanim SkyAttackSetUp, SkyAttackUnleash
SkyAttackEnd::
	end

SkyAttackSetUp::
	loadspritegfx ANIM_TAG_FOCUS_ENERGY
	loadspritegfx ANIM_TAG_WATER_ORB @ Blue color
	createvisualtask AnimTask_BlendExcept, 10, F_PAL_EXCEPT_TARGET, 0, 0, 8, RGB_BLACK
	waitforvisualfinish
	delay 12
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	call SkyAttackCloakEffect
	delay 8
	call SkyAttackCloakEffect
	delay 8
	call SkyAttackCloakEffect
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 11, RGB_WHITE
	waitforvisualfinish
	delay 5
	createvisualtask AnimTask_BlendExcept, 10, F_PAL_EXCEPT_TARGET, 0, 8, 0, RGB_BLACK
	goto SkyAttackEnd
	
SkyAttackCloakEffect::
	createsprite gSkyAttackCloakEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, 26, 2
	delay 4
	createsprite gSkyAttackCloakEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 14, 28, 1
	delay 4
	createsprite gSkyAttackCloakEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -5, 10, 2
	delay 4
	createsprite gSkyAttackCloakEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 28, 26, 3
	delay 4
	createsprite gSkyAttackCloakEnergySpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -12, 0, 1
	return

SkyAttackUnleash::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_BIRD
	monbg ANIM_ATTACKER
	call SetSkyBg
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 0, 0, 16, RGB_WHITE
	delay 4
	createvisualtask AnimTask_AttackerFadeToInvisible, 5, 0
	waitforvisualfinish
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_SKY_UPPERCUT, SOUND_PAN_ATTACKER
	createsprite gSkyAttackBirdSpriteTemplate, ANIM_TARGET, 2
	delay 14
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 10, 0, 18, 1
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	delay 20
	createvisualtask AnimTask_AttackerFadeFromInvisible, 5, 1
	delay 2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 0, 15, 0, RGB_WHITE
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	call UnsetScrollingBg
	goto SkyAttackEnd

gMoveAnim_TRANSFORM::
	monbg ANIM_ATTACKER
	playsewithpan SE_M_TELEPORT, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_MINIMIZE, SOUND_PAN_ATTACKER, 48
	createvisualtask AnimTask_TransformMon, 2, ANIM_TARGET, 0
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

gMoveAnim_BUBBLE::
	loadspritegfx ANIM_TAG_BUBBLE
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	monbg ANIM_TARGET
	setalpha 12, 8
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 15, -15, 10, 128, 100
	call BubbleSound
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 35, 37, 40, 128, 100
	call BubbleSound
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 10, -37, 30, 128, 100
	call BubbleSound
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 30, 10, 15, 128, 100
	call BubbleSound
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 20, 33, 20, 128, 100
	call BubbleSound
	createsprite gWaterBubbleProjectileSpriteTemplate, ANIM_ATTACKER, 2, 18, 0, 25, -30, 10, 128, 100
	call BubbleSound
	waitforvisualfinish
	call WaterBubblesEffectLong
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

BubbleSound::
	playsewithpan SE_M_BUBBLE, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_TARGET, 100
	delay 6
	return

gMoveAnim_DIZZY_PUNCH::
	loadspritegfx ANIM_TAG_DUCK
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	call DizzyPunchLunge
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 5, 16, 8, 20, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 16, 0, ANIM_TARGET, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, 16, 8, 160, -32
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, 16, 8, -256, -40
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, 16, 8, 128, -16
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, 16, 8, 416, -38
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, 16, 8, -128, -22
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, 16, 8, -384, -31
	delay 10
	call DizzyPunchLunge
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 5, -16, -8, 20, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, -16, -16, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, -16, -8, 160, -32
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, -16, -8, -256, -40
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, -16, -8, 128, -16
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, -16, -8, 416, -38
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, -16, -8, -128, -22
	createsprite gDizzyPunchDuckSpriteTemplate, ANIM_TARGET, 3, -16, -8, -384, -31
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

DizzyPunchLunge::
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 6
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 7, 1
	return

gMoveAnim_SPORE::
	loadspritegfx ANIM_TAG_SPORE
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_SporeDoubleBattle, 2
	setalpha 12, 8
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_TARGET, 16, 11
	call CreateSpore
	call CreateSpore
	call CreateSpore
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

CreateSpore::
	createsprite gSporeParticleSpriteTemplate, ANIM_TARGET, 2, 0, -20, 85, 80, TRUE, ANIM_TARGET
	delay 12
	createsprite gSporeParticleSpriteTemplate, ANIM_TARGET, 2, 0, -10, 170, 80, TRUE, ANIM_TARGET
	delay 12
	createsprite gSporeParticleSpriteTemplate, ANIM_TARGET, 2, 0, -15, 0, 80, TRUE, ANIM_TARGET
	delay 12
	return

gMoveAnim_FLASH::
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_Flash, 2
	end

gMoveAnim_PSYWAVE::
	loadspritegfx ANIM_TAG_BLUE_RING
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	call SetPsychicBackground
	createvisualtask AnimTask_StartSinAnimTimer, 5, 100
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_TELEPORT, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 9, 0, 10
	call PsywaveRings
	call PsywaveRings
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 1, 4, 0, 12, RGB(31, 18, 31) @ Purple color
	call PsywaveRings
	call PsywaveRings
	call PsywaveRings
	call PsywaveRings
	waitforvisualfinish
	call UnsetScrollingBg
	end

PsywaveRings::
	createsprite gPsywaveRingSpriteTemplate, ANIM_TARGET, 3, 10, 10, 16
	delay 4
	createsprite gPsywaveRingSpriteTemplate, ANIM_TARGET, 3, 10, 10, 16
	delay 4
	return

gMoveAnim_SPLASH::
	createvisualtask AnimTask_Splash, 2, ANIM_ATTACKER, 3
	delay 8
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 38, 3
	end

gMoveAnim_ACID_ARMOR::
	monbg ANIM_ATTACKER
	setalpha 15, 0
	createvisualtask AnimTask_AcidArmor, 2, ANIM_ATTACKER
	playsewithpan SE_M_ACID_ARMOR, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_CRABHAMMER::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	loadspritegfx ANIM_TAG_WATER_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gWaterHitSplatSpriteTemplate, ANIM_ATTACKER, 4, 0, 0, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 1
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB(13, 21, 31), 10, RGB_BLACK, 0 @ Blue color then black
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -24, 0, FALSE, 4
	waitforvisualfinish
	delay 8
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 4
	waitforvisualfinish
	loopsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET, 20, 3
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 8, 1
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 10, 10, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 20, -20, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, -15, 15, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, -10, -20, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 16, -8, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, 5, 8, 20, ANIM_TARGET
	delay 4
	createsprite gSmallBubblePairSpriteTemplate, ANIM_ATTACKER, 2, -16, 0, 20, ANIM_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_EXPLOSION::
	loadspritegfx ANIM_TAG_EXPLOSION
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 8, 9, RGB(26, 8, 8), 8, RGB_BLACK, 8 @ Red color
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT, 8, 0, 40, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 1, 8, 0, 40, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 2, 8, 0, 40, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 3, 8, 0, 40, 1
	createvisualtask AnimTask_ShakeMon2, 5, MAX_BATTLERS_COUNT + 4, 8, 0, 40, 1
	call Explosion
	call Explosion
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 16, 16, RGB_WHITE
	delay 50
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 3, 16, 0, RGB_WHITE
	end

Explosion::
	playsewithpan SE_M_EXPLOSION, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_EXPLOSION, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 24, -24, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_EXPLOSION, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -16, 16, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_EXPLOSION, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -24, -12, ANIM_ATTACKER, FALSE
	delay 6
	playsewithpan SE_M_EXPLOSION, SOUND_PAN_ATTACKER
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 16, 16, ANIM_ATTACKER, FALSE
	delay 6
	return

gMoveAnim_FURY_SWIPES::
	loadspritegfx ANIM_TAG_SWIPE
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 5, 5
	delay 4
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	createsprite gFurySwipesSpriteTemplate, ANIM_TARGET, 2, 16, 0, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 5, 1
	delay 10
	createsprite gHorizontalLungeSpriteTemplate, ANIM_TARGET, 2, 5, 5
	delay 4
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	createsprite gFurySwipesSpriteTemplate, ANIM_TARGET, 2, -16, 0, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 7, 1
	end

gMoveAnim_BONEMERANG::
	loadspritegfx ANIM_TAG_BONE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_BONEMERANG, SOUND_PAN_ATTACKER
	createsprite gBonemerangSpriteTemplate, ANIM_ATTACKER, 2
	delay 20
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 5, 0, 5, 1
	delay 17
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_ATTACKER
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, -4
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_REST::
	loadspritegfx ANIM_TAG_LETTER_Z
	playsewithpan SE_M_SNORE, SOUND_PAN_ATTACKER
	call RestZLetter
	call RestZLetter
	call RestZLetter
	end

RestZLetter::
	createsprite gSleepLetterZSpriteTemplate, ANIM_ATTACKER, 2, 4, -10
	delay 20
	return

@ Credits: Skeli
gMoveAnim_ROCK_SLIDE::
	loadspritegfx ANIM_TAG_ROCKS
	monbg ANIM_DEF_SIDE
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, -5, 1, -5, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 5, 0, 6, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 19, 1, 10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, -23, 2, -10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 5, 50, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_DEF_PARTNER, 0, 5, 50, 1
	delay 2
	call RockSlideRocks
	call RockSlideRocks
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

RockSlideRocks::
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 28, 1, 10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, -10, 1, -5, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 10, 0, 6, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 24, 1, 10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, -32, 2, -10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingRockSpriteTemplate, ANIM_TARGET, 2, 30, 2, 10, TRUE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	return

gMoveAnim_HYPER_FANG::
	loadspritegfx ANIM_TAG_FANG_ATTACK
	setbgbasedonside ANIM_TARGET, BG_IMPACT_PLAYER, BG_IMPACT_OPPONENT, TRUE
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	waitbgfadeout
	createsprite gFangSpriteTemplate, ANIM_TARGET, 2
	waitbgfadein
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 0, 10, 10, 1
	playsewithpan SE_M_LEER, SOUND_PAN_TARGET
	delay 20
	restorebg
	waitbgfadein
	end

gMoveAnim_SHARPEN::
	loadspritegfx ANIM_TAG_SPHERE_TO_CUBE
	createsprite gSharpenSphereSpriteTemplate, ANIM_ATTACKER, 2
	end

gMoveAnim_CONVERSION::
	loadspritegfx ANIM_TAG_CONVERSION
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	setalpha 16, 0
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -24, -24
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -8, -24
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 8, -24
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 24, -24
	delay 3
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -24, -8
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -8, -8
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 8, -8
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 24, -8
	delay 3
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -24, 8
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -8, 8
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 8, 8
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 24, 8
	delay 3
	playsewithpan SE_M_SWIFT, SOUND_PAN_ATTACKER
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -24, 24
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, -8, 24
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 8, 24
	delay 3
	createsprite gConversionSpriteTemplate, ANIM_ATTACKER, 2, 24, 24
	delay 20
	playsewithpan SE_M_BARRIER, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_FlashAnimTagWithColor, 2, ANIM_TAG_CONVERSION, 1, 1, RGB(31, 31, 13), 12, 0, 0 @ Yellow color
	delay 6
	createvisualtask AnimTask_ConversionAlphaBlend, 5
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_TRI_ATTACK::
	loadspritegfx ANIM_TAG_TRI_FORCE_TRIANGLE
	loadspritegfx ANIM_TAG_FIRE
	loadspritegfx ANIM_TAG_LIGHTNING
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	createsprite gTriAttackTriangleSpriteTemplate, ANIM_TARGET, 2, 16, 0
	playsewithpan SE_M_TRI_ATTACK, SOUND_PAN_ATTACKER
	delay 20
	playsewithpan SE_M_TRI_ATTACK, SOUND_PAN_ATTACKER
	delay 20
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_TRI_ATTACK, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 5, 6, 0, 7
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 16, RGB_BLACK
	delay 16
	call SacredFireLargeFlames
	delay 2
	createvisualtask AnimTask_ShakeTargetInPattern, 2, 20, 3, 1, 1
	waitforvisualfinish
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	playsewithpan SE_M_TRI_ATTACK2, SOUND_PAN_TARGET
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 0, -48
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 0, -16
	delay 1
	createsprite gLightningSpriteTemplate, ANIM_TARGET, 2, 0, 16
	delay 20
	createvisualtask AnimTask_ShakeTargetInPattern, 2, 20, 3, 1, 0
	delay 2
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	waitforvisualfinish
	call IceCrystalEffectShort
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 16, 0, RGB_BLACK
	end

gMoveAnim_SUPER_FANG::
	loadspritegfx ANIM_TAG_FANG_ATTACK
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 1, 0, 20, 1
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 3, 0, 48, 1
	createvisualtask AnimTask_BlendMonInAndOut, 2, ANIM_ATTACKER, RGB(31, 6, 1), 12, 4, 1 @ Red color
	waitforvisualfinish
	delay 20
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 4
	createsprite gSuperFangSpriteTemplate, ANIM_TARGET, 2
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	delay 8
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB(31, 2, 2), 14, RGB_WHITE, 14 @ Red color
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 7, 12, 1
	waitforvisualfinish
	blendoff
	end

gMoveAnim_SLASH::
	loadspritegfx ANIM_TAG_SLASH
	createsprite gSlashSliceSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -8, 0
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	delay 4
	createsprite gSlashSliceSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 8, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 18, 1
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	end

gMoveAnim_SUBSTITUTE::
	playsewithpan SE_M_ATTRACT, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_MonToSubstitute, 2
	end

gMoveAnim_STRUGGLE::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_MOVEMENT_WAVES
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 3, 0, 12, 4
	createsprite gMovementWavesSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, FALSE, 2
	createsprite gMovementWavesSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, TRUE, 2
	loopsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER, 12, 4
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 3, 0, 6, 1
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@@@@@@@@@@@@@@@@@@@@@@@ GEN II @@@@@@@@@@@@@@@@@@@@@@@

gMoveAnim_SKETCH::
	loadspritegfx ANIM_TAG_PENCIL
	monbg ANIM_TARGET
	createvisualtask AnimTask_SketchDrawMon, 2
	createsprite gPencilSpriteTemplate, ANIM_TARGET, 2
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

gMoveAnim_TRIPLE_KICK::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	jumpifmoveturn 0, TripleKickLeft
	jumpifmoveturn 1, TripleKickRight
	goto TripleKickCenter
TripleKickContinue::
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

TripleKickLeft::
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, -16, -8, 20, ANIM_TARGET, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -16, -16, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 4, 0, 6, 1
	goto TripleKickContinue

TripleKickRight::
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 8, 8, 20, ANIM_TARGET, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 8, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 4, 0, 6, 1
	goto TripleKickContinue

TripleKickCenter::
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 0, 0, 20, ANIM_TARGET, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, -8, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	goto TripleKickContinue

gMoveAnim_THIEF::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	fadetobg BG_DARK
	waitbgfadein
	setalpha 12, 8
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 6
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 8, 1
	waitforvisualfinish
	delay 20
	clearmonbg ANIM_TARGET
	blendoff
	restorebg
	waitbgfadein
	end

gMoveAnim_SPIDER_WEB::
	loadspritegfx ANIM_TAG_SPIDER_WEB
	loadspritegfx ANIM_TAG_WEB_THREAD
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 5, F_PAL_BG, 2, 0, 9, RGB_BLACK
	waitforvisualfinish
	loopsewithpan SE_M_STRING_SHOT, SOUND_PAN_ATTACKER, 9, 6
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	call WebThreadSingle
	waitforvisualfinish
	playsewithpan SE_M_STRING_SHOT2, SOUND_PAN_TARGET
	createsprite gSpiderWebSpriteTemplate, ANIM_ATTACKER, 2
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 5, F_PAL_BG, 2, 9, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_MIND_READER::
	loadspritegfx ANIM_TAG_TEAL_ALERT
	loadspritegfx ANIM_TAG_OPENING_EYE
	loadspritegfx ANIM_TAG_ROUND_WHITE_HALO
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_TARGET
	createsprite gOpeningEyeSpriteTemplate, ANIM_ATTACKER, 5, 0, 0, ANIM_TARGET, TRUE
	createsprite gWhiteHaloSpriteTemplate, ANIM_ATTACKER, 5, 0, 0, ANIM_TARGET
	delay 40
	playsewithpan SE_M_LEER, SOUND_PAN_TARGET
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG, 1, 2, 0, 10, RGB_BLACK
	call MindReaderEyeSpikeEffect
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

MindReaderEyeSpikeEffect::
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 70, 0, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 40, 40, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 10, -60, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -50, -40, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -40, 40, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 50, -50, 6, ANIM_TARGET
	delay 2
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 50, -30, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 60, 10, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 0, 60, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 0, -40, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -60, 20, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -60, -30, 6, ANIM_TARGET
	delay 2
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -50, 50, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -60, 20, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -40, -40, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 20, -60, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 50, -50, 6, ANIM_TARGET
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 35, 40, 6, ANIM_TARGET
	delay 2
	return

@ Credits: Blackuser
gMoveAnim_NIGHTMARE::
	monbg ANIM_DEF_SIDE
	fadetobg BG_NIGHTMARE
	waitbgfadeout
	createvisualtask AnimTask_NightmareBgWaveEffect, 5
	waitbgfadein
	createvisualtask AnimTask_NightmareClone, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 40, 1
	playsewithpan SE_M_NIGHTMARE, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	call UnsetScrollingBg
	end

gMoveAnim_FLAME_WHEEL::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 0
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	delay 2
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 4
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	delay 2
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 8
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	delay 2
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 12
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	delay 2
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 16
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	delay 2
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 20
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	delay 2
	createsprite gFireSpiralOutwardSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 24
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, 0, FALSE, 6
	delay 4
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 8, 1
	createvisualtask AnimTask_BlendMonInAndOut, 3, ANIM_TARGET, RGB_RED, 12, 1, 1
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET
	call FireSpreadEffect
	delay 7
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 9
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_SNORE::
	loadspritegfx ANIM_TAG_SNORE_Z
	monbg ANIM_ATK_SIDE
	setalpha 8, 8
	call SnoreEffect
	delay 30
	call SnoreEffect
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

SnoreEffect::
	playsewithpan SE_M_SNORE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -7, -7, 7, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 7, 1
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, 14, 0, ANIM_ATTACKER
	createsprite gSnoreZSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, -42, -38, 24, ANIM_ATTACKER, FALSE
	createsprite gSnoreZSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 0, -42, 24, ANIM_ATTACKER, FALSE
	createsprite gSnoreZSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 42, -38, 24, ANIM_ATTACKER, FALSE
	return

gMoveAnim_CURSE::
	choosetwoturnanim CurseGhost, CurseStats

CurseGhost::
	loadspritegfx ANIM_TAG_NAIL
	loadspritegfx ANIM_TAG_GHOSTLY_SPIRIT
	monbg ANIM_ATK_SIDE
	createvisualtask AnimTask_CurseStretchingBlackBg, 5
	waitforvisualfinish
	delay 20
	createsprite gCurseNailSpriteTemplate, ANIM_ATTACKER, 2, 0, 0
	delay 60
	call CurseGhostShakeFromNail
	delay 41
	call CurseGhostShakeFromNail
	delay 41
	call CurseGhostShakeFromNail
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	delay 1
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_NIGHTMARE, SOUND_PAN_TARGET
	createsprite gCurseGhostSpriteTemplate, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 14, 1
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 16, 0, RGB_BLACK @ Blend Curse bg back to normal
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

CurseGhostShakeFromNail::
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 4, 0, 10, 0
	playsewithpan SE_M_BIND, SOUND_PAN_ATTACKER
	return

CurseStats::
	createvisualtask AnimTask_SwayMon, 5, 0, 10, 1536, 3, ANIM_ATTACKER
	waitforvisualfinish
	delay 10
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_SetUpCurseBackground, 5
	createvisualtask AnimTask_BlendColorCycle, 5, F_PAL_ATTACKER, 4, 2, 0, 10, RGB_RED
	end

gMoveAnim_FLAIL::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_FlailMovement, 2, ANIM_ATTACKER
	loopsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER, 8, 2
	waitforvisualfinish
	createsprite gRandomPosHitSplatSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 3
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 30, TRUE, FALSE
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Blackuser
gMoveAnim_CONVERSION_2::
	loadspritegfx ANIM_TAG_CONVERSION
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 0, 16
	playsewithpan SE_M_BARRIER, SOUND_PAN_TARGET
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -24, -24, 60
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -8, -24, 65
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 8, -24, 70
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 24, -24, 75
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -24, -8, 80
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -8, -8, 85
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 8, -8, 90
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 24, -8, 95
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -24, 8, 100
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -8, 8, 105
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 8, 8, 110
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 24, 8, 115
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -24, 24, 120
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -8, 24, 125
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 8, 24, 130
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 24, 24, 135
	createvisualtask AnimTask_Conversion2AlphaBlend, 5
	delay 60
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

Conversion2Sound::
	playsewithpan SE_M_SWIFT, SOUND_PAN_TARGET
	delay 10
	return

gMoveAnim_AEROBLAST::
	loadspritegfx ANIM_TAG_AIR_WAVE_2
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	call SetSkyBg
	setalpha 12, 8
	call AeroblastBeam
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 5, 0, 50, 1
	call AeroblastBeam
	call AeroblastBeam
	call AeroblastBeam
	call AeroblastBeam
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

AeroblastBeam::
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, -12, 0, -12, 15, 0, FALSE
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 26, 8, 12, 8, 15, 0, FALSE
	delay 3
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, -12, 0, -12, 15, 1, FALSE
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 26, 8, 12, 8, 15, 1, FALSE
	delay 3
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, -12, 0, -12, 15, 2, FALSE
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 26, 8, 12, 8, 15, 2, FALSE
	delay 3
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 14, -12, 0, -12, 15, 3, FALSE
	createsprite gAirWaveCrescentSpriteTemplate, ANIM_ATTACKER, 2, 26, 8, 12, 8, 15, 3, FALSE
	delay 3
	return

@ Credits: Blackuser
gMoveAnim_COTTON_SPORE::
	loadspritegfx ANIM_TAG_SPORE
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_TARGET, 18, 10
	createvisualtask AnimTask_CreateCottonSporeSpores, 5, -20, 85, -10, 170, -15, 0
	end

gMoveAnim_REVERSAL::
	loadspritegfx ANIM_TAG_BLUE_ORB
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	playsewithpan SE_M_DETECT, SOUND_PAN_ATTACKER
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 3, RGB_WHITE, 8, RGB_BLACK, 0
	waitforvisualfinish
	delay 30
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 2, 0, 10, RGB_WHITE
	delay 10
	playsewithpan SE_M_REVERSAL, SOUND_PAN_ATTACKER
	createsprite gReversalOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 0
	createsprite gReversalOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 42
	createsprite gReversalOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 84
	createsprite gReversalOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 126
	createsprite gReversalOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 168
	createsprite gReversalOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 210
	waitforvisualfinish
	delay 20
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 8
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_WHITE, 8, RGB_BLACK, 0
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 0, 0, 10, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 5, FALSE, 1, 8, TRUE, FALSE
	end

gMoveAnim_SPITE::
	monbg ANIM_DEF_SIDE
	fadetobg BG_GHOST
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	waitbgfadein
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 6, 0, 8, RGB_WHITE
	createvisualtask AnimTask_SpiteTargetShadow, 2
	loopsewithpan SE_M_PSYBEAM, SOUND_PAN_TARGET, 20, 3
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	restorebg
	waitbgfadein
	end

gMoveAnim_POWDER_SNOW::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	monbg ANIM_DEF_SIDE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 1, 0, 3, RGB_BLACK
	waitforvisualfinish
	panse_1B SE_M_GUST, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	call PowderSnowSnowballs
	call PowderSnowSnowballs
	playsewithpan SE_M_GUST2, SOUND_PAN_TARGET
	waitforvisualfinish
	waitsound
	call IceCrystalEffectLong
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	delay 20
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 1, 3, 0, RGB_BLACK
	end

PowderSnowSnowballs::
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, 0, 0, 0, 56, 4, 4, TRUE
	delay 3
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -10, 0, -10, 56, 4, 4, TRUE
	delay 3
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, 10, 0, 10, 56, -4, 3, TRUE
	delay 3
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -20, 0, -20, 56, -4, 5, TRUE
	delay 3
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, 15, 0, 15, 56, 4, 4, TRUE
	delay 3
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, -20, 0, -20, 56, 4, 4, TRUE
	delay 3
	createsprite gPowderSnowSnowballSpriteTemplate, ANIM_ATTACKER, 40, 0, 20, 0, 20, 56, 4, 4, TRUE
	delay 3
	return

gMoveAnim_PROTECT::
	loadspritegfx ANIM_TAG_PROTECT
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	waitplaysewithpan SE_M_REFLECT, SOUND_PAN_ATTACKER, 16
	createsprite gProtectWallSpriteTemplate, ANIM_ATTACKER, 2, 24, 0, 90
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	end

@ Credits: Skeli
gMoveAnim_MACH_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_ATK_SIDE
	setalpha 9, 8
	createvisualtask AnimTask_AttackerPunchWithTrace, 2, RGB(8, 9, 28), 10 @ Blue color
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	delay 2
	createsprite gMachPunchFistSpriteTemplate, ANIM_TARGET, 4, ANIM_TARGET, 0, 0, 12, 0, TRUE, 0
	delay 6
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_SCARY_FACE::
	loadspritegfx ANIM_TAG_EYE_SPARKLE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_ATK_SIDE | F_PAL_DEF_PARTNER, 3, 0, 16, RGB_BLACK
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 10
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaryFace, 5
	delay 13
	call EyeSparkles
	waitforvisualfinish
	createvisualtask AnimTask_ShakeTargetInPattern, 3, 20, 1, 0
	playsewithpan SE_M_STRING_SHOT2, SOUND_PAN_TARGET
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_ATK_SIDE | F_PAL_DEF_PARTNER, 3, 16, 0, RGB_BLACK
	end

gMoveAnim_FAINT_ATTACK::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_ATTACKER
	fadetobg BG_DARK
	waitbgfadein
	playsewithpan SE_M_FAINT_ATTACK, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 18, 6, 1, 3
	createvisualtask AnimTask_AttackerFadeToInvisible, 2, 1
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	invisible ANIM_ATTACKER
	delay 1
	createvisualtask AnimTask_SetAttackerInvisibleWaitForSignal, 2
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 9, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	setargret -2 @ Signal to restore battler sprite
	delay 32
	createvisualtask AnimTask_InitAttackerFadeFromInvisible, 2
	monbg ANIM_ATTACKER
	createvisualtask AnimTask_AttackerFadeFromInvisible, 2, 1
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	restorebg
	waitbgfadein
	end

gMoveAnim_SWEET_KISS::
	loadspritegfx ANIM_TAG_RED_HEART
	loadspritegfx ANIM_TAG_ANGEL
	createsprite gAngelSpriteTemplate, ANIM_TARGET, 2, 16, -48
	call SweetKissAngelSound
	call SweetKissAngelSound
	call SweetKissAngelSound
	waitforvisualfinish
	playsewithpan SE_M_ATTRACT, SOUND_PAN_TARGET
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, 160, -30
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, -256, -42
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, 128, -14
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, 416, -38
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, -128, -22
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, -384, -31
	end

SweetKissAngelSound::
	playsewithpan SE_M_HEAL_BELL, SOUND_PAN_TARGET
	delay 23
	return

gMoveAnim_BELLY_DRUM::
	loadspritegfx ANIM_TAG_MUSIC_NOTES
	loadspritegfx ANIM_TAG_PURPLE_HAND_OUTLINE
	createvisualtask AnimTask_MusicNotesRainbowBlend, 2
	call BellyDrumRight
	createsprite gSlowFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 0, 0
	playsewithpan SE_M_BELLY_DRUM, SOUND_PAN_ATTACKER
	delay 15
	call BellyDrumLeft
	createsprite gSlowFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 1, 1, 1, 0
	playsewithpan SE_M_BELLY_DRUM, SOUND_PAN_ATTACKER
	delay 15
	call BellyDrumRight
	createsprite gSlowFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 0, 3, 3, 128
	playsewithpan SE_M_BELLY_DRUM, SOUND_PAN_ATTACKER
	delay 7
	call BellyDrumLeft
	createsprite gSlowFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 1, 2, 0, 128
	playsewithpan SE_M_BELLY_DRUM, SOUND_PAN_ATTACKER
	delay 7
	call BellyDrumRight
	createsprite gSlowFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 0, 1, 1, 0
	playsewithpan SE_M_BELLY_DRUM, SOUND_PAN_ATTACKER
	delay 7
	call BellyDrumLeft
	createsprite gSlowFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 1, 0, 3, 0
	playsewithpan SE_M_BELLY_DRUM, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_MusicNotesClearRainbowBlend, 2
	end

BellyDrumLeft::
	createsprite gBellyDrumHandSpriteTemplate, ANIM_ATTACKER, 3, FALSE
	createvisualtask AnimTask_ShakeMon, 2, ANIM_ATTACKER, 0, 8, 2, 1
	return

BellyDrumRight::
	createsprite gBellyDrumHandSpriteTemplate, ANIM_ATTACKER, 3, TRUE
	createvisualtask AnimTask_ShakeMon, 2, ANIM_ATTACKER, 0, 8, 2, 1
	return

gMoveAnim_SLUDGE_BOMB::
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	call SludgeBombProjectile
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 3, 0, 15, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 1, 2, 0, 12, RGB(30, 0, 31) @ Purple color
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, 42, 27, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, -27, 44, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, 39, -28, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, -42, -42, 20
	playsewithpan SE_M_DIG, SOUND_PAN_TARGET
	delay 5
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, 0, 40, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, -8, -44, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, -46, -28, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, 46, 9, 20
	playsewithpan SE_M_DIG, SOUND_PAN_TARGET
	delay 5
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, 42, 0, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, -43, -12, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, 16, -46, 20
	createsprite gSludgeBombHitParticleSpriteTemplate, ANIM_TARGET, 2, -16, 44, 20
	playsewithpan SE_M_DIG, SOUND_PAN_TARGET
	waitsound
	waitforvisualfinish
	call PoisonBubblesEffect
	end

SludgeBombProjectile::
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	createsprite gSludgeProjectileSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 40, -30, FALSE
	delay 3
	return

gMoveAnim_MUD_SLAP::
	loadspritegfx ANIM_TAG_MUD_SAND
	playsewithpan SE_M_SAND_ATTACK, SOUND_PAN_ATTACKER
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -10, 0, FALSE, 3
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 2
	call MudSlapMud
	call MudSlapMud
	call MudSlapMud
	call MudSlapMud
	call MudSlapMud
	call MudSlapMud
	end

MudSlapMud::
	createsprite gMudSlapMudSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 0, 0
	createsprite gMudSlapMudSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 10, 5
	createsprite gMudSlapMudSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, -10, -5
	createsprite gMudSlapMudSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 20, 10
	createsprite gMudSlapMudSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, -20, -10
	delay 2
	return

gMoveAnim_OCTAZOOKA::
	loadspritegfx ANIM_TAG_GRAY_SMOKE
	loadspritegfx ANIM_TAG_BLACK_BALL
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_ATTACKER
	createsprite gOctazookaBallSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 0, 20, 0
	waitforvisualfinish
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gOctazookaSmokeSpriteTemplate, ANIM_TARGET, 2, 8, 8, ANIM_TARGET, TRUE
	delay 2
	createsprite gOctazookaSmokeSpriteTemplate, ANIM_TARGET, 2, -8, -8, ANIM_TARGET, TRUE
	delay 2
	createsprite gOctazookaSmokeSpriteTemplate, ANIM_TARGET, 2, 8, -8, ANIM_TARGET, TRUE
	delay 2
	createsprite gOctazookaSmokeSpriteTemplate, ANIM_TARGET, 2, -8, 8, ANIM_TARGET, TRUE
	end

gMoveAnim_SPIKES::
	loadspritegfx ANIM_TAG_SPIKES
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 28
	createsprite gSpikesSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 24, 30
	delay 10
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 28
	createsprite gSpikesSpriteTemplate, ANIM_TARGET, 2, 20, 0, -24, 24, 30
	delay 10
	waitplaysewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 28
	createsprite gSpikesSpriteTemplate, ANIM_TARGET, 2, 20, 0, 24, 24, 30
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_ZAP_CANNON::
	loadspritegfx ANIM_TAG_BLACK_BALL_2
	loadspritegfx ANIM_TAG_SPARK_2
	playsewithpan SE_M_THUNDER_WAVE, SOUND_PAN_ATTACKER
	createsprite gZapCannonBallSpriteTemplate, ANIM_TARGET, 3, 10, 0, 0, 0, 30, 0
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 16, 30, 0, 40, 0
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 16, 30, 64, 40, 1
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 16, 30, 128, 40, 0
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 16, 30, 192, 40, 2
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 8, 30, 32, 40, 0
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 8, 30, 96, 40, 1
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 8, 30, 160, 40, 0
	createsprite gZapCannonSparkSpriteTemplate, ANIM_TARGET, 4, 10, 0, 8, 30, 224, 40, 2
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 5, 1
	delay 15
	waitplaysewithpan SE_M_THUNDERBOLT2, SOUND_PAN_TARGET, 19
	call ElectricityEffect
	end

gMoveAnim_FORESIGHT::
	loadspritegfx ANIM_TAG_MAGNIFYING_GLASS
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 16, 0
	createsprite gForesightMagnifyingGlassSpriteTemplate, ANIM_TARGET, 2
	delay 17
	loopsewithpan SE_M_SKETCH, SOUND_PAN_TARGET, 16, 4
	delay 72
	playsewithpan SE_M_SKETCH, SOUND_PAN_TARGET
	delay 10
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_TARGET, RGB_WHITE, 12, 2, 1
	playsewithpan SE_M_DETECT, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_DESTINY_BOND::
	loadspritegfx ANIM_TAG_WHITE_SHADOW
	fadetobg BG_GHOST
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	waitbgfadein
	createvisualtask AnimTask_DestinyBondWhiteShadow, 5, 48, TRUE, FALSE
	playsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_ATTACKER
	delay 48
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 24, 1
	createvisualtask AnimTask_BlendExcept, 2, F_PAL_EXCEPT_BG | F_PAL_EXCEPT_ATK_PARTNER, 1, 0, 12, RGB(29, 29, 29) @ White color
	delay 24
	createvisualtask AnimTask_BlendExcept, 2, F_PAL_EXCEPT_BG | F_PAL_EXCEPT_ATK_PARTNER, 1, 12, 0, RGB(29, 29, 29) @ White color
	playsewithpan SE_M_NIGHTMARE, SOUND_PAN_TARGET
	waitforvisualfinish
	restorebg
	waitbgfadein
	clearmonbg MAX_BATTLERS_COUNT + 1
	blendoff
	end

gMoveAnim_PERISH_SONG::
	loadspritegfx ANIM_TAG_MUSIC_NOTES_2
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 0, 0, 0
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 1, 1, 16
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 2, 1, 32
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 3, 2, 48
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 4, 2, 64
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 5, 0, 80
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 6, 0, 96
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 7, 1, 112
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 8, 2, 128
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 9, 0, 144
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 10, 2, 160
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 11, 0, 176
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 12, 1, 192
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 13, 3, 208
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 14, 3, 224
	createsprite gPerishSongMusicNoteSpriteTemplate, ANIM_ATTACKER, 4, 15, 0, 240
	createsprite gPerishSongMusicNote2SpriteTemplate, ANIM_ATTACKER, 4, 15
	delay 20
	panse_1B SE_M_PERISH_SONG, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	delay 80
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 0, 16, RGB_BLACK
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT, FALSE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT + 1, FALSE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT + 2, FALSE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT + 3, FALSE
	delay 100
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 16, 0, RGB_BLACK
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT, TRUE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT + 1, TRUE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT + 2, TRUE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, MAX_BATTLERS_COUNT + 3, TRUE
	end

gMoveAnim_ICY_WIND::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	loadspritegfx ANIM_TAG_ICE_SPIKES
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_ATK_SIDE, 4, 0, 4, RGB_BLACK
	fadetobg BG_ICE
	waitbgfadeout
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_ABOVE
	waitbgfadein
	waitforvisualfinish
	panse_1B SE_M_GUST, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	call IcyWindSwirlingSnowballs
	delay 5
	call IcyWindSwirlingSnowballs
	playsewithpan SE_M_GUST2, SOUND_PAN_TARGET
	delay 55
	call IceSpikesEffectLong
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	restorebg
	waitbgfadeout
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_ATK_SIDE, 4, 4, 0, RGB_BLACK
	waitbgfadein
	end

IcyWindSwirlingSnowballs::
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_TARGET, 40, 0, 0, 0, 0, 72, TRUE
	delay 5
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_TARGET, 40, 0, 10, 0, 10, 72, TRUE
	delay 5
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_TARGET, 40, 0, -10, 0, -10, 72, TRUE
	delay 5
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_TARGET, 40, 0, 15, 0, 15, 72, TRUE
	delay 5
	createsprite gSwirlingSnowballSpriteTemplate, ANIM_TARGET, 40, 0, -5, 0, -5, 72, TRUE
	return

gMoveAnim_DETECT::
	loadspritegfx ANIM_TAG_SPARKLE_4
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 9, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 1, 0, 9, RGB_WHITE
	delay 18
	playsewithpan SE_M_DETECT, SOUND_PAN_ATTACKER
	createsprite gDisableSparkleSpriteTemplate, ANIM_ATTACKER, 13, 20, -20
	waitforvisualfinish
	delay 10
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 9, 0, RGB_BLACK
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 2, 9, 0, RGB_WHITE
	end

gMoveAnim_BONE_RUSH::
	loadspritegfx ANIM_TAG_BONE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_BONEMERANG, SOUND_PAN_TARGET
	createsprite gSpinningBoneSpriteTemplate, ANIM_ATTACKER, 2, -42, -25, 0, 0, 15
	delay 12
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 3, 5, 1
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_LOCK_ON::
	loadspritegfx ANIM_TAG_LOCK_ON
	createsprite gLockOnTargetSpriteTemplate, ANIM_ATTACKER, 40 
	createsprite gLockOnMoveTargetSpriteTemplate, ANIM_ATTACKER, 40, 1
	createsprite gLockOnMoveTargetSpriteTemplate, ANIM_ATTACKER, 40, 2
	createsprite gLockOnMoveTargetSpriteTemplate, ANIM_ATTACKER, 40, 3
	createsprite gLockOnMoveTargetSpriteTemplate, ANIM_ATTACKER, 40, 4
	delay 120
	setargret -1 @ Signal to start the blink effect
	end

gMoveAnim_OUTRAGE::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	loopsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER, 8, 3
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER | F_PAL_TARGET, 2, 5, 3, 8, RGB(14, 13, 0) @ Green color
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 12, 6, 5, 4
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, 0, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, 0, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 0, 1280, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 0, -1280, 3
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 40, 1
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, 768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, 768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, -768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, -768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, 0, 3
	call OutrageFlames
	call OutrageFlames
	end

OutrageFlames::
	delay 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, 0, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 0, 1280, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 0, -1280, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, 768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, 768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, -768, 3
	createsprite gOutrageFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, -768, 3
	return

gMoveAnim_SANDSTORM::
	loadspritegfx ANIM_TAG_FLYING_DIRT
	playsewithpan SE_M_SANDSTORM, SOUND_PAN_ABOVE
	createvisualtask AnimTask_LoadSandstormBackground, 5, FALSE
	delay 16
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 10, 2304, 96, FALSE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 90, 2048, 96, FALSE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 50, 2560, 96, FALSE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 20, 2304, 96, FALSE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 70, 1984, 96, FALSE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 0, 2816, 96, FALSE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 60, 2560, 96, FALSE
	end

gMoveAnim_GIGA_DRAIN::
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 12, RGB(13, 31, 12) @ Green color
	waitforvisualfinish
	playsewithpan SE_M_ABSORB, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	delay 2
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 5, 1
	waitforvisualfinish
	delay 3
	call GigaDrainAbsorbEffect
	waitforvisualfinish
	delay 15
	call HealingEffect
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 12, 0, RGB(13, 31, 12) @ Green color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

GigaDrainAbsorbEffect::
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -40, 35
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 28, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 40, 39
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -32, 26
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 16, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -40, 26
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -5, 15, 36, 33
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, -5, -8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, -10, 20, 20, 39
	delay 4
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 5, 8, 26
	createsprite gAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 3, 5, -18, -20, 35
	delay 4
	return

gMoveAnim_ENDURE::
	loadspritegfx ANIM_TAG_FOCUS_ENERGY
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	call EndureEffect
	delay 8
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 11, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 32, 1
	call EndureEffect
	delay 8
	call EndureEffect
	end

gMoveAnim_CHARM::
	loadspritegfx ANIM_TAG_MAGENTA_HEART
	createvisualtask AnimTask_RockMonBackAndForth, 5, ANIM_ATTACKER, 2, 0
	call InfatuationHearts
	end

gMoveAnim_ROLLOUT::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_MUD_SAND
	loadspritegfx ANIM_TAG_ROCKS
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_SetRolloutAnimTimer, 2
	createvisualtask AnimTask_Rollout, 2
	waitforvisualfinish
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 30, TRUE, FALSE
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 4, 0, 0, ANIM_TARGET, 2
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_FALSE_SWIPE::
	loadspritegfx ANIM_TAG_SLASH_2
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gFalseSwipeSliceSpriteTemplate, ANIM_TARGET, 2
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_TARGET
	delay 16
	createsprite gFalseSwipePositionedSliceSpriteTemplate, ANIM_TARGET, 2, 0
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	delay 2
	createsprite gFalseSwipePositionedSliceSpriteTemplate, ANIM_TARGET, 2, 16
	delay 2
	createsprite gFalseSwipePositionedSliceSpriteTemplate, ANIM_TARGET, 2, 32
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	delay 2
	createsprite gFalseSwipePositionedSliceSpriteTemplate, ANIM_TARGET, 2, 48
	delay 2
	createsprite gFalseSwipePositionedSliceSpriteTemplate, ANIM_TARGET, 2, 64
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	delay 2
	createsprite gFalseSwipePositionedSliceSpriteTemplate, ANIM_TARGET, 2, 80
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, 0, ANIM_TARGET, 3
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	end

@ Credits: Skeli
gMoveAnim_SWAGGER::
	loadspritegfx ANIM_TAG_BREATH
	loadspritegfx ANIM_TAG_ANGER
	createvisualtask AnimTask_GrowAndShrink, 2
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 11, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 32, 1
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	delay 12
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	end

gMoveAnim_MILK_DRINK::
	loadspritegfx ANIM_TAG_MILK_BOTTLE
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_BLUE_STAR
	monbg ANIM_ATTACKER
	createsprite gMilkBottleSpriteTemplate, ANIM_ATTACKER, 2
	delay 40
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	delay 12
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	delay 20
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE
	playsewithpan SE_M_MILK_DRINK, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	call HealingEffect
	end

gMoveAnim_SPARK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SPARK_2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 5, 5, RGB(31, 31, 22) @ Yellow color
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	call SparkElectricity1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 0, 0, RGB(31, 31, 22) @ Yellow color
	delay 10
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 5, 5, RGB(31, 31, 22) @ Yellow color
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	call SparkElectricity2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 0, 0, RGB(31, 31, 22) @ Yellow color
	delay 20
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 7, 7, RGB(31, 31, 22) @ Yellow color
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 32, 12, 0, 20, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 32, 12, 64, 20, 1
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 32, 12, 128, 20, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 32, 12, 192, 20, 2
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 16, 12, 32, 20, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 16, 12, 96, 20, 1
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 16, 12, 160, 20, 0
	createsprite gSparkElectricityFlashingSpriteTemplate, ANIM_ATTACKER, 4, ANIM_TARGET, 0, 16, 12, 224, 20, 2
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 0, 0, RGB(31, 31, 22) @ Yellow color
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 4
	playsewithpan SE_M_HYPER_BEAM, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, -31, 2, 0, 6, RGB(31, 31, 22) @ Yellow color
	call ElectricityEffect
	end

SparkElectricity1:
	createsprite gSparkElectricitySpriteTemplate, ANIM_ATTACKER, 0, 32, 24, 190, 12, ANIM_ATTACKER, TRUE, FALSE
	createsprite gSparkElectricitySpriteTemplate, ANIM_ATTACKER, 0, 80, 24, 22, 12, ANIM_ATTACKER, TRUE, FALSE
	createsprite gSparkElectricitySpriteTemplate, ANIM_ATTACKER, 0, 156, 24, 121, 13, ANIM_ATTACKER, TRUE, TRUE
	return

SparkElectricity2:
	createsprite gSparkElectricitySpriteTemplate, ANIM_ATTACKER, 0, 100, 24, 60, 10, ANIM_ATTACKER, TRUE, FALSE
	createsprite gSparkElectricitySpriteTemplate, ANIM_ATTACKER, 0, 170, 24, 42, 11, ANIM_ATTACKER, TRUE, TRUE
	createsprite gSparkElectricitySpriteTemplate, ANIM_ATTACKER, 0, 238, 24, 165, 10, ANIM_ATTACKER, TRUE, TRUE
	return

gMoveAnim_FURY_CUTTER::
	loadspritegfx ANIM_TAG_CUT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createvisualtask AnimTask_IsFuryCutterHitRight, 2
	jumpretfalse FuryCutterLeft
	goto FuryCutterRight
FuryCutterContinue::
	createvisualtask AnimTask_GetFuryCutterHitCount, 2
	jumpreteq 1, FuryCutterContinue2
	jumpreteq 2, FuryCutterMedium
	goto FuryCutterStrong
FuryCutterContinue2::
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 10, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

FuryCutterLeft::
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 0
	goto FuryCutterContinue

FuryCutterRight::
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 1, 0
	goto FuryCutterContinue

FuryCutterMedium::
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB(9, 8, 10), 4, RGB_BLACK, 0 @ Some black color
	goto FuryCutterContinue2

FuryCutterStrong::
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 3, RGB(9, 8, 10), 4, RGB_BLACK, 0 @ Some black color
	goto FuryCutterContinue2

@ Credits: Skeli
gMoveAnim_STEEL_WING::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_WHITE_FEATHER
	createvisualtask AnimTask_GrayscaleParticle, 5, ANIM_TAG_IMPACT
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WHITE_FEATHER, 2, 14, 14, RGB(18, 19, 21) @ Silver color
	waitforvisualfinish
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 10, 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -30, 10, ANIM_TARGET, 1
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -15, 0, ANIM_TARGET, 1
	call ScatterFeathers
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, -10, ANIM_TARGET, 1
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 5, 15, -20, ANIM_TARGET, 1
	delay 1
	playsewithpan SE_M_WING_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 6, 30, -30, ANIM_TARGET, 1
	end

gMoveAnim_MEAN_LOOK::
	loadspritegfx ANIM_TAG_EYE
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 16, RGB_BLACK
	loopsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_TARGET, 15, 4
	waitplaysewithpan SE_M_LEER, SOUND_PAN_TARGET, 85
	createsprite gMeanLookEyeSpriteTemplate, ANIM_ATTACKER, 2
	delay 120
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 16, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_ATTRACT::
	loadspritegfx ANIM_TAG_RED_HEART
	loopsewithpan SE_M_CHARM, SOUND_PAN_ATTACKER, 12, 3
	createvisualtask AnimTask_SwayMon, 5, 0, 12, 4096, 4, ANIM_ATTACKER
	delay 15
	createsprite gRedHeartProjectileSpriteTemplate, ANIM_TARGET, 3, 20, -8
	waitforvisualfinish
	playsewithpan SE_M_ATTRACT, SOUND_PAN_TARGET
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, 160, -32
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, -256, -40
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, 128, -16
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, 416, -38
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, -128, -22
	createsprite gRedHeartBurstSpriteTemplate, ANIM_TARGET, 3, -384, -31
	waitforvisualfinish
	waitplaysewithpan SE_M_ATTRACT2, SOUND_PAN_ABOVE, 15
	createvisualtask AnimTask_HeartsBackground, 5
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 16, 256, 0
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 224, 240, 15
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 126, 272, 30
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 80, 224, 45
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 170, 272, 60
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 40, 256, 75
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 112, 256, 90
	createsprite gRedHeartRisingSpriteTemplate, ANIM_ATTACKER, 40, 200, 272, 90
	delay 75
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 4, 4, 0, 10, RGB(31, 25, 27) @ Pink color
	end

gMoveAnim_SLEEP_TALK::
	loadspritegfx ANIM_TAG_LETTER_Z
	createvisualtask AnimTask_SwayMon, 5, 0, 4, 4096, 2, ANIM_ATTACKER
	call SleepTalkLetterSound
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -1
	delay 6
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -1
	delay 6
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -1
	call SleepTalkLetterSound
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -5
	delay 6
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -5
	delay 6
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -5
	call SleepTalkLetterSound
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -3
	delay 6
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -3
	delay 6
	createsprite gLetterZSpriteTemplate, ANIM_TARGET, 2, 0, 5, -3
	end

SleepTalkLetterSound::
	delay 20
	playsewithpan SE_M_SNORE, SOUND_PAN_ATTACKER
	return

gMoveAnim_HEAL_BELL::
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_BELL
	loadspritegfx ANIM_TAG_MUSIC_NOTES_2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATK_SIDE, 0, 0, 10, RGB_WHITE
	createvisualtask AnimTask_LoadMusicNotesPals, 5
	waitforvisualfinish
	createsprite gBellSpriteTemplate, ANIM_ATTACKER, 2, 0, -24, ANIM_ATTACKER, FALSE
	delay 12
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, 48, -18, 35, 0, 0
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, -48, 20, 30, 1, 1
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, -38, -29, 30, 2, 2
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, 36, 18, 30, 3, 3
	call HealBellRing
	delay 33
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, 19, 26, 35, 4, 4
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, -34, -12, 30, 5, 5
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, 41, -20, 34, 6, 2
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, -15, 26, 32, 7, 0
	call HealBellRing
	delay 33
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, -48, 18, 31, 0, 2
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, 48, -20, 30, 2, 5
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, 38, 29, 33, 4, 3
	createsprite gHealBellMusicNoteSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, -36, -18, 30, 6, 1
	call HealBellRing
	waitforvisualfinish
	createvisualtask AnimTask_FreeMusicNotesPals, 5
	unloadspritegfx ANIM_TAG_BELL
	unloadspritegfx ANIM_TAG_MUSIC_NOTES_2
	loadspritegfx ANIM_TAG_SPARKLE_2
	call HealBellSparkles
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendExcept, 10, F_PAL_EXCEPT_ATTACKER_AND_TARGET, 3, 10, 0, RGB(12, 24, 30) @ White color
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATK_SIDE, 3, 10, 0, RGB_WHITE
	createsprite gBlendThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 16, 0, 0, ANIM_ATTACKER, FALSE
	end

HealBellRing::
	createvisualtask AnimTask_BlendExcept, 10, F_PAL_EXCEPT_ATTACKER_AND_TARGET, 3, 8, 0, RGB(12, 24, 30) @ White color
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATK_SIDE, 3, 2, 10, RGB_WHITE
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 40, 0, -24, ANIM_ATTACKER, FALSE
	playsewithpan SE_M_HEAL_BELL, SOUND_PAN_ATTACKER
	return

HealBellSparkles::
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_ATTACKER
	createsprite gSparklingStarsSpriteTemplate, ANIM_ATTACKER, 16, -15, 0, ANIM_ATTACKER, 0, 32, 60, TRUE
	delay 8
	createsprite gSparklingStarsSpriteTemplate, ANIM_ATTACKER, 16, 12, -5, ANIM_ATTACKER, 0, 32, 60, TRUE
	waitforvisualfinish
	return

gMoveAnim_RETURN::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	delay 2
	createvisualtask AnimTask_GetReturnPowerLevel, 2
	jumpreteq 0, ReturnWeak
	jumpreteq 1, ReturnMedium
	jumpreteq 2, ReturnStrong
	jumpreteq 3, ReturnStrongest
ReturnContinue::
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

ReturnWeak::
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 16, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 16, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	goto ReturnContinue

ReturnMedium::
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 11
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 5, 4
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	goto ReturnContinue

ReturnStrong::
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, 10, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 3, -5, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -5, 3, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	goto ReturnContinue

ReturnStrongest::
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 0, 6, RGB_BLACK
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 16, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 3, -5, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 12, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	waitforvisualfinish
	delay 4
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 8, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	waitforvisualfinish
	delay 2
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 5, 1
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 4, 1, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 5, 1
	waitforvisualfinish
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 4, 2, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 5, 1
	waitforvisualfinish
	call ReturnStrongestHit
	call ReturnStrongestHit
	call ReturnStrongestHit
	call ReturnStrongestHit
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 0
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 8, 0, 24, 1
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, 10, ANIM_TARGET, 0
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 3, -5, ANIM_TARGET, 0
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -5, 3, ANIM_TARGET, 0
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 6, 0, RGB_BLACK
	goto ReturnContinue

ReturnStrongestHit::
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 4, 3, ANIM_ATTACKER
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 5, 1
	waitforvisualfinish
	return

gMoveAnim_PRESENT::
	loadspritegfx ANIM_TAG_ITEM_BAG_2
	createsprite gPresentSpriteTemplate, ANIM_TARGET, 2, 0, -5
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 14
	playsewithpan SE_M_BUBBLE2, SOUND_PAN_ATTACKER
	delay 14
	playsewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE
	delay 20
	playsewithpan SE_M_BUBBLE2, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_IsHealingMove, 2
	jumprettrue PresentHeal
PresentDamage::
	loadspritegfx ANIM_TAG_EXPLOSION
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, 24, -24, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, -16, 16, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, -24, -12, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, 16, 16, ANIM_TARGET, FALSE
	end

PresentHeal::
	loadspritegfx ANIM_TAG_GREEN_SPARKLE
	loadspritegfx ANIM_TAG_BLUE_STAR
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_TARGET
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, -16, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, 16, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, 32, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, -32, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, 0, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, -8, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, -8, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, 24, 32, -3
	delay 3
	createsprite gPresentHealParticleSpriteTemplate, ANIM_TARGET, 4, -24, 32, -3
	waitforvisualfinish
	waitsound
	call HealingEffectOnTarget
	end

gMoveAnim_FRUSTRATION::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_ANGER
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_GetFrustrationPowerLevel, 1
	jumpreteq 0, Frustration_Strongest
	jumpreteq 1, Frustration_Strong
	jumpreteq 2, Frustration_Medium
	goto Frustration_Weak
Frustration_Continue::
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

Frustration_Strongest::
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_ATTACKER, 1, 0, 15, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 3, 0, 9, RGB_RED
	waitforvisualfinish
	delay 20
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	createsprite gAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, -28
	waitforvisualfinish
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	createsprite gAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, -28
	waitforvisualfinish
	delay 10
	createvisualtask AnimTask_SwayMon, 5, 0, 16, 6144, 8, ANIM_ATTACKER
	delay 5
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 4, 0, 30, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 24, 8, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -24, -16, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 8, 4, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -16, 19, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 18, -18, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 3, 9, 0, RGB_RED
	goto Frustration_Continue

Frustration_Strong::
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_ATTACKER, 1, 0, 15, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 3, 0, 9, RGB_RED
	waitforvisualfinish
	delay 20
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	createsprite gAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, -28
	waitforvisualfinish
	delay 5
	createvisualtask AnimTask_StrongFrustrationGrowAndShrink, 5
	delay 7
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 8, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 4, 0, 6, 1
	delay 14
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 12, -6, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 4, 0, 6, 1
	delay 14
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -12, -6, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 4, 0, 6, 1
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 3, 9, 0, RGB_RED
	goto Frustration_Continue

Frustration_Medium::
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ATTACKER
	createsprite gAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, -28
	waitforvisualfinish
	delay 5
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 6
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, 4, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 6
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -4, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 6, 1
	goto Frustration_Continue

Frustration_Weak::
	createsprite gWeakFrustrationAngerMarkSpriteTemplate, ANIM_ATTACKER, 2, 20, -28
	waitforvisualfinish
	delay 10
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 10, 2
	delay 12
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 6, 1
	goto Frustration_Continue

gMoveAnim_SAFEGUARD::
	loadspritegfx ANIM_TAG_GUARD_RING
	monbg ANIM_ATK_SIDE
	setalpha 8, 8
	playsewithpan SE_M_MILK_DRINK, SOUND_PAN_ATTACKER
	createsprite gGuardRingSpriteTemplate, ANIM_ATTACKER, 2, TRUE
	delay 4
	createsprite gGuardRingSpriteTemplate, ANIM_ATTACKER, 2, TRUE
	delay 4
	createsprite gGuardRingSpriteTemplate, ANIM_ATTACKER, 2, TRUE
	waitforvisualfinish
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATK_SIDE, 0, 2, 0, 10, RGB_WHITE
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_PAIN_SPLIT::
	loadspritegfx ANIM_TAG_PAIN_SPLIT
	createsprite gPainSplitProjectileSpriteTemplate, ANIM_ATTACKER, 2, -8, -42, ANIM_ATTACKER
	createsprite gPainSplitProjectileSpriteTemplate, ANIM_TARGET, 2, -8, -42, ANIM_TARGET
	delay 10
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ABOVE
	createvisualtask AnimTask_PainSplitMovement, 2, ANIM_ATTACKER, 0
	createvisualtask AnimTask_PainSplitMovement, 2, ANIM_TARGET, 0
	waitforvisualfinish
	createsprite gPainSplitProjectileSpriteTemplate, ANIM_ATTACKER, 2, -24, -42, ANIM_ATTACKER
	createsprite gPainSplitProjectileSpriteTemplate, ANIM_TARGET, 2, -24, -42, ANIM_TARGET
	delay 10
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ABOVE
	createvisualtask AnimTask_PainSplitMovement, 2, ANIM_ATTACKER, 1
	createvisualtask AnimTask_PainSplitMovement, 2, ANIM_TARGET, 1
	waitforvisualfinish
	createsprite gPainSplitProjectileSpriteTemplate, ANIM_ATTACKER, 2, 8, -42, ANIM_ATTACKER
	createsprite gPainSplitProjectileSpriteTemplate, ANIM_TARGET, 2, 8, -42, 1
	delay 10
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_ABOVE
	createvisualtask AnimTask_PainSplitMovement, 2, ANIM_ATTACKER, 2
	createvisualtask AnimTask_PainSplitMovement, 2, ANIM_TARGET, 2
	end

gMoveAnim_SACRED_FIRE::
	loadspritegfx ANIM_TAG_FIRE
	loadspritegfx ANIM_TAG_FIRE_PLUME
	loopsewithpan SE_M_SACRED_FIRE, SOUND_PAN_ATTACKER, 7, 5
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -32, 0, 50, 5, -2, 0
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, -20, -10, 50, 5, -1, -1
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 0, -16, 50, 5, 0, -1
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 20, -10, 50, 5, 1, -1
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, 32, 0, 50, 5, 2, 0
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, 20, 10, 50, 5, 1, 1
	delay 1
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, 50, 5, 0, 1
	delay 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -20, 10, 50, 5, -1, 1
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	waitforvisualfinish
	playsewithpan SE_M_SACRED_FIRE2, SOUND_PAN_TARGET
	createsprite gLargeFlameSpriteTemplate, ANIM_TARGET, 2, -16, 0, 70, 16, 0, 1
	delay 10
	playsewithpan SE_M_SACRED_FIRE2, SOUND_PAN_TARGET
	createsprite gLargeFlameSpriteTemplate, ANIM_TARGET, 2, 0, 0, 70, 16, 0, 1
	delay 10
	playsewithpan SE_M_SACRED_FIRE2, SOUND_PAN_TARGET
	createsprite gLargeFlameSpriteTemplate, ANIM_TARGET, 2, 16, 0, 80, 16, 0, 1
	delay 1
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	waitforvisualfinish
	createvisualtask AnimTask_InvertScreenColor, 2, 257, 257, 257
	delay 1
	call SacredFireLargeFlames
	end

SacredFireLargeFlames::
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, -1, 0
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, 0, 1
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, -1, -1
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, 2, 1
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, 1, -1
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, -1, 1
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, 1, -2
	delay 1
	createsprite gLargeFlameScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 30, 3, 1
	return

gMoveAnim_MAGNITUDE::
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT + 1, 0, 50
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT, 0, 50
	loopsewithpan SE_M_STRENGTH, SOUND_PAN_TARGET, 8, 10
	createvisualtask AnimTask_IsPowerOver99, 2
	callrettrue MagnitudeIntense
	end

MagnitudeIntense::
	delay 10
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	delay 16
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_BLACK, 14, RGB_WHITE, 14
	return

gMoveAnim_DYNAMIC_PUNCH::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_EXPLOSION
	loadspritegfx ANIM_TAG_EXPLOSION_6
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 3, 0, 0, 20, ANIM_TARGET, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 5, 0, 7, 1
	delay 1
	waitsound
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 5, ANIM_TARGET, 5, 0, 28, 1
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 24, -24, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -16, 16, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, -24, -12, ANIM_TARGET, FALSE
	delay 6
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_ATTACKER, 3, 16, 16, ANIM_TARGET, FALSE
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_MEGAHORN::
	loadspritegfx ANIM_TAG_HORN_HIT_2
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	call SetUpDrillBackground
	setalpha 12, 8
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 2, 0, 15, 1
	waitforvisualfinish
	delay 10
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, 0, FALSE, 6
	delay 3
	createsprite gMegahornHornSpriteTemplate, ANIM_ATTACKER, 3, -42, 25, 0, 0, 6
	delay 4
	playsewithpan SE_M_VICEGRIP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -16, 4, TRUE, 4
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, -4, 1, 12, 1
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_ATTACKER | F_PAL_TARGET, 5, 1, RGB_WHITE, 10, RGB_BLACK, 0
	delay 10
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 11
	delay 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 7
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_DRAGON_BREATH::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	loadspritegfx ANIM_TAG_WATER_ORB @ Blue color
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WATER_ORB, 2, 10, 10, RGB(5, 16, 23) @ Darken a little the blue
	waitforvisualfinish
	loopsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_ATTACKER, 7, 7
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 1, 0, 9, RGB_RED
	call DragonBreathCreateFlames
	delay 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 21, 1
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	call DragonBreathCreateFlames
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 1, 9, 0, RGB_RED
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

DragonBreathCreateFlames::
	createsprite gDragonBreathFireSpriteTemplate, ANIM_TARGET, 2, 0, 0, 0, 0, 20
	delay 2
	return

gMoveAnim_BATON_PASS::
	loadspritegfx ANIM_TAG_POKEBALL
	playsewithpan SE_M_BATON_PASS, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_BATTLERS, 1, 2, 0, 11, RGB(31, 22, 30) @ Pink color
	createsprite gBatonPassPokeballSpriteTemplate, ANIM_ATTACKER, 2
	end

gMoveAnim_ENCORE::
	loadspritegfx ANIM_TAG_SPOTLIGHT
	loadspritegfx ANIM_TAG_TAG_HAND
	createvisualtask AnimTask_CreateSpotlight, 2
	createvisualtask AnimTask_HardwarePaletteFade, 2, BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN, 3, 0, 10, FALSE
	waitforvisualfinish
	createsprite gSpotlightSpriteTemplate, ANIM_TARGET, 2, 0, -8, ANIM_TARGET
	createsprite gClappingHandSpriteTemplate, ANIM_ATTACKER, 2, -2, 0, FALSE, 9
	createsprite gClappingHandSpriteTemplate, ANIM_ATTACKER, 2, 2, 0, TRUE, 9
	createsprite gClappingHand2SpriteTemplate, ANIM_ATTACKER, 3, -2, 0, FALSE, 9
	createsprite gClappingHand2SpriteTemplate, ANIM_ATTACKER, 3, 2, 0, TRUE, 9
	delay 16
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_ENCORE2, SOUND_PAN_TARGET
	createvisualtask AnimTask_SwayMon, 5, 1, 8, 1536, 5, ANIM_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_HardwarePaletteFade, 2, BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN, 3, 10, 0, TRUE
	waitforvisualfinish
	createvisualtask AnimTask_RemoveSpotlight, 2
	end

gMoveAnim_PURSUIT::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	fadetobg BG_DARK
	waitbgfadein
	setalpha 12, 8
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	choosetwoturnanim PursuitNormal, PursuitOnSwitchout
PursuitContinue::
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 5, FALSE, 1, 6, TRUE, FALSE
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	restorebg
	waitbgfadein
	end

PursuitNormal::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	goto PursuitContinue

PursuitOnSwitchout::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 1
	goto PursuitContinue

gMoveAnim_RAPID_SPIN::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_RAPID_SPIN
	monbg ANIM_ATTACKER
	createsprite gRapidSpinSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 32, -32, 40, -2
	createvisualtask AnimTask_RapinSpinMonElevation, 2, ANIM_ATTACKER, 2, FALSE
	loopsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER, 8, 4
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 10, TRUE, FALSE
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	waitforvisualfinish
	delay 8
	createvisualtask AnimTask_RapinSpinMonElevation, 2, ANIM_ATTACKER, 2, TRUE
	loopsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER, 8, 4
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

gMoveAnim_SWEET_SCENT::
	loadspritegfx ANIM_TAG_PINK_PETAL
	playsewithpan SE_M_SWEET_SCENT, SOUND_PAN_ATTACKER
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 100, 0
	delay 25
	setpan SOUND_PAN_ABOVE
	call SweetScentEffect
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 55, 0
	setpan SOUND_PAN_TARGET
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_DEF_SIDE, 1, 5, 5, 13, RGB(31, 21, 21) @ Pink color
	call SweetScentEffect
	end

SweetScentEffect::
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 70, 1
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 60, 0
	delay 5
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 80, 1
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 58, 0
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 100, 0
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 90, 0
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 48, 0
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 95, 1
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 100, 0
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 75, 1
	delay 2
	createsprite gSweetScentPetalSpriteTemplate, ANIM_ATTACKER, 2, 85, 0
	delay 2
	return

@ Credits: Skeli
gMoveAnim_IRON_TAIL::
	loadspritegfx ANIM_TAG_ROCKS
	loadspritegfx ANIM_TAG_SLAM_HIT_2
	loadspritegfx ANIM_TAG_IMPACT
	setalpha 12, 8
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SLAM_HIT_2, 2, 14, 14, RGB(16, 17, 17) @ Gray color
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	waitforvisualfinish
	monbg ANIM_TARGET
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_ATTACKER
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 3, FALSE, 4
	delay 1
	createsprite gKnockOffStrikeSpriteTemplate, ANIM_TARGET, 2, -16, -16
	delay 3
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -12, 10, FALSE, 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	call IronTailRockScatter
	delay 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 0, 3, 6, 1
	waitforvisualfinish
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BATTLERS_2, 1, 0, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

IronTailRockScatter::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -10, -8, ANIM_TARGET, 1
IronTailRockScatterNoHit::
	playsewithpan SE_M_STRENGTH, SOUND_PAN_TARGET
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, -12, 27, 2, 3
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, 8, 28, 3, 4
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, -4, 30, 2, 3
	createsprite gRockScatterSpriteTemplate, ANIM_TARGET, 2, 12, 25, 4, 4
	return

gMoveAnim_METAL_CLAW::
	loadspritegfx ANIM_TAG_CLAW_SLASH
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	waitforvisualfinish
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 2
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, -10, -10, 0
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, -10, 10, 0
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, -4, 1, 10, 3, ANIM_TARGET
	delay 8
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 2
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, 10, -10, 1
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, 10, 10, 1
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, -4, 1, 10, 3, ANIM_TARGET
	end

gMoveAnim_VITAL_THROW::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 12, 4, 1, 2
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 0, FALSE, 4
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -24, 0, FALSE, 4
	waitforvisualfinish
	delay 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 7
	delay 11
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 10
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_MORNING_SUN::
	loadspritegfx ANIM_TAG_GREEN_STAR
	loadspritegfx ANIM_TAG_BLUE_STAR
	createvisualtask AnimTask_MorningSunLightBeam, 5
	delay 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_BATTLERS_2, 8, 0, 12, RGB_WHITE
	delay 14
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	call MorningSunStar
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_BATTLERS_2, 3, 12, 0, RGB_WHITE
	waitforvisualfinish
	waitsound
	call HealingEffect
	end

MorningSunStar::
	createsprite gGreenStarSpriteTemplate, ANIM_ATTACKER, 2, 30, 640
	delay 5
	return

gMoveAnim_SYNTHESIS::
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_SPARKLE_2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 16, RGB(27, 31, 18) @ Yellow color
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER
	call GrantingStarsEffect
	waitforvisualfinish
	call HealingEffect
	end

gMoveAnim_MOONLIGHT::
	loadspritegfx ANIM_TAG_MOON
	loadspritegfx ANIM_TAG_GREEN_SPARKLE
	loadspritegfx ANIM_TAG_BLUE_STAR
	setalpha 0, 16
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 16, RGB_BLACK
	waitforvisualfinish
	createsprite gMoonSpriteTemplate, ANIM_ATTACKER, 2, 120, 56
	createvisualtask AnimTask_AlphaFadeIn, 3, 0, 16, 16, 0, 1
	playsewithpan SE_M_MOONLIGHT, SOUND_PAN_ABOVE
	delay 30
	createsprite gMoonlightSparkleSpriteTemplate, ANIM_ATTACKER, 40, -12, 0
	delay 30
	createsprite gMoonlightSparkleSpriteTemplate, ANIM_ATTACKER, 40, -24, 0
	delay 30
	createsprite gMoonlightSparkleSpriteTemplate, ANIM_ATTACKER, 40, 21, 0
	delay 30
	createsprite gMoonlightSparkleSpriteTemplate, ANIM_ATTACKER, 40, 0, 0
	delay 30
	createsprite gMoonlightSparkleSpriteTemplate, ANIM_ATTACKER, 40, 10, 0
	delay 20
	createvisualtask AnimTask_FadeScreenBlue, 2
	waitforvisualfinish
	call HealingEffect
	end

gMoveAnim_HIDDEN_POWER::
	loadspritegfx ANIM_TAG_RED_ORB
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -7, -7, 11, ANIM_ATTACKER, ST_OAM_OBJ_NORMAL
	waitforvisualfinish
	delay 30
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB(31, 31, 19), 12, 5, 1 @ White color
	delay 4
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -7, -7, 11, ANIM_ATTACKER, ST_OAM_OBJ_NORMAL
	playsewithpan SE_M_REVERSAL, SOUND_PAN_ATTACKER
	createsprite gHiddenPowerOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 0
	createsprite gHiddenPowerOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 42
	createsprite gHiddenPowerOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 84
	createsprite gHiddenPowerOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 126
	createsprite gHiddenPowerOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 168
	createsprite gHiddenPowerOrbSpriteTemplate, ANIM_ATTACKER, 2, 26, 210
	delay 52
	setargret -1 @ Signal to end orbit orbs anim
	playsewithpan SE_M_REFLECT, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -7, -7, 11, ANIM_ATTACKER, ST_OAM_OBJ_NORMAL
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 0, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 32, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 64, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 96, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 128, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 160, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 192, 0
	createsprite gHiddenPowerOrbScatterSpriteTemplate, ANIM_TARGET, 2, 224, 0
	end

gMoveAnim_CROSS_CHOP::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_CROSS_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_TARGET
	createsprite gCrossChopHandSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, FALSE
	createsprite gCrossChopHandSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, TRUE
	delay 40
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_WHITE, 10, RGB_BLACK, 10
	createsprite gCrossImpactSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 20
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 7, 0, 9, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_TWISTER::
	loadspritegfx ANIM_TAG_LEAF
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_ROCKS
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	playsewithpan SE_M_TWISTER, SOUND_PAN_TARGET
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 120, 70, 5, 70, 30
	delay 1
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 115, 55, 6, 60, 25
	delay 1
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 115, 60, 7, 60, 30
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 115, 55, 10, 60, 30
	delay 3
	createsprite gTwisterRockSpriteTemplate, ANIM_TARGET, 2, 100, 50, 4, 50, 26
	delay 1
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 105, 25, 8, 60, 20
	delay 1
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 115, 40, 10, 48, 30
	delay 3
	createsprite gTwisterRockSpriteTemplate, ANIM_TARGET, 2, 120, 30, 6, 45, 25
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 115, 35, 10, 60, 30
	delay 3
	createsprite gTwisterRockSpriteTemplate, ANIM_TARGET, 2, 105, 20, 8, 40, 0
	delay 3
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 20, 255, 15, 32, 0
	createsprite gTwisterLeafParticleSpriteTemplate, ANIM_TARGET, 2, 110, 10, 8, 32, 20
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -32, -16, ANIM_TARGET, 3
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 3, 0, 12, 1
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_DEF_PARTNER, 3, 0, 12, 1
	delay 4
	call TwisterRandomHit
	call TwisterRandomHit
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 32, 20, ANIM_TARGET, 3
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

TwisterRandomHit::
	createsprite gRandomPosHitSplatSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 3
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 4
	return

@ Credits: Skeli
gMoveAnim_RAIN_DANCE::
	loadspritegfx ANIM_TAG_RAIN_DROPS
	playsewithpan SE_M_RAIN_DANCE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 13, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_CreateRaindrops, 2, 0, 3, 120
	createvisualtask AnimTask_CreateRaindrops, 2, 0, 3, 120
	delay 150
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 13, 0, RGB_BLACK
	end

gMoveAnim_SUNNY_DAY::
	loadspritegfx ANIM_TAG_SUNLIGHT
	monbg ANIM_ATK_SIDE
	setalpha 13, 3
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_BATTLERS_2, 1, 0, 6, RGB_WHITE
	waitforvisualfinish
	panse_26 SE_M_PETAL_DANCE, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 1, 0
	call SunnyDayLightRay
	call SunnyDayLightRay
	call SunnyDayLightRay
	call SunnyDayLightRay
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG | F_PAL_BATTLERS_2, 1, 6, 0, RGB_WHITE
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

SunnyDayLightRay::
	createsprite gSunlightRaySpriteTemplate, ANIM_ATTACKER, 40
	delay 6
	return

gMoveAnim_CRUNCH::
	loadspritegfx ANIM_TAG_SHARP_TEETH
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	fadetobg BG_DARK
	waitbgfadein
	setalpha 12, 8
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, -32, -32, 1, 819, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 32, 32, 5, -819, -819, 10
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -8, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 7, 5, 2
	waitforvisualfinish
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 32, -32, 7, -819, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, -32, 32, 3, 819, -819, 10
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 8, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 8, 4, 2
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	restorebg
	waitbgfadein
	end

gMoveAnim_MIRROR_COAT::
	loadspritegfx ANIM_TAG_SPARKLE_3
	loadspritegfx ANIM_TAG_RED_LIGHT_WALL
	setalpha 0, 16
	createsprite gMirrorCoatWallSpriteTemplate, ANIM_ATTACKER, 1, 40, 0, ANIM_TAG_RED_LIGHT_WALL
	delay 10
	playsewithpan SE_M_REFLECT, SOUND_PAN_ATTACKER
	call SpecialScreenSparkle
	waitforvisualfinish
	blendoff
	end

gMoveAnim_PSYCH_UP::
	loadspritegfx ANIM_TAG_SPIRAL
	monbg ANIM_ATK_SIDE
	createvisualtask AnimTask_BlendColorCycleExclude, 2, F_PAL_BG, 2, 6, 1, 11, RGB_BLACK
	setalpha 12, 8
	loopsewithpan SE_M_PSYBEAM2, SOUND_PAN_ATTACKER, 5, 10
	createsprite gPsychUpSpiralSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_ATTACKER, TRUE
	createvisualtask AnimTask_SwayMon, 5, 0, 5, 2560, 8, ANIM_ATTACKER
	delay 131
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -5, -5, 10, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	createvisualtask AnimTask_BlendSelected, 9, F_PAL_ATTACKER, 2, 10, 0, RGB_YELLOW
	delay 30
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_EXTREME_SPEED::
	loadspritegfx ANIM_TAG_SPEED_DUST
	loadspritegfx ANIM_TAG_IMPACT
	call SetHighSpeedBackground
	createvisualtask AnimTask_StretchAttacker, 2
	loopsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER, 8, 3
	waitforvisualfinish
	delay 1
	createvisualtask AnimTask_SetAttackerInvisibleWaitForSignal, 2
	monbg ANIM_TARGET
	setalpha 12, 8
	delay 18
	createvisualtask AnimTask_ExtremeSpeedImpact, 2
	delay 2
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gMonEdgeHitSplatSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, -12, 3
	delay 10
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gMonEdgeHitSplatSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 12, 3
	delay 10
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gMonEdgeHitSplatSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 0, 3
	waitforvisualfinish
	createvisualtask AnimTask_SpeedDust, 2
	delay 10
	createvisualtask AnimTask_ExtremeSpeedMonReappear, 2
	loopsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER, 8, 4
	waitforvisualfinish
	call UnsetScrollingBg
	clearmonbg ANIM_TARGET
	blendoff
	delay 1
	setargret -2 @ Signal to restore battler sprite
	delay 1
	end

gMoveAnim_ANCIENT_POWER::
	loadspritegfx ANIM_TAG_ROCKS
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, 4, 1, 10, ANIM_TARGET
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 20, 32, -48, 50, 2
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, -38, 25, 5
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 32, 32, -28, 40, 3
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, -20, 32, -48, 50, 2
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 20, 32, -28, 60, 1
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, -28, 30, 4
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 30, 1
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	delay 10
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 15, 32, -48, 25, 5
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, -10, 32, -42, 30, 4
	delay 10
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, -42, 25, 5
	createsprite gAncientPowerRockSpriteTemplate, ANIM_ATTACKER, 2, -25, 32, -48, 30, 4
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 16, 0, FALSE, 4
	delay 3
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 7
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_SHADOW_BALL::
	loadspritegfx ANIM_TAG_SHADOW_BALL
	fadetobg BG_GHOST
	waitbgfadein
	delay 15
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_MIST, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 5, 5, 0, 5
	createsprite gShadowBallSpriteTemplate, ANIM_TARGET, 2, 16, 16, 8
	waitforvisualfinish
	playsewithpan SE_M_SAND_ATTACK, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 8, 1
	waitforvisualfinish
	restorebg
	waitbgfadein
	end

gMoveAnim_FUTURE_SIGHT::
	monbg ANIM_ATK_SIDE
	setalpha 8, 8
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	call SetPsychicBackground
	delay 1
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 8, RGB_WHITE
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -4, -4, 15, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_ROCK_SMASH::
	loadspritegfx ANIM_TAG_ROCKS
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gRockSmashRockSpriteTemplate, ANIM_ATTACKER, 1, ANIM_TARGET, 0, 0, 32, 10, TRUE, 0
	delay 26
	createsprite gFistFootSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 8, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 6
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 20, 24, 14, 2
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, 5, 0, -20, 24, 14, 1
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, 0, 5, 20, -24, 14, 2
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, -5, 0, -20, -24, 14, 2
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, 0, -5, 30, 18, 8, 2
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 30, -18, 8, 2
	createsprite gRockFragmentSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, -30, 18, 8, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 7, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_WHIRLPOOL::
	loadspritegfx ANIM_TAG_WATER_ORB
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 0, 7, RGB(0, 13, 23) @ Blue color
	playsewithpan SE_M_WHIRLPOOL, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 2, 50, 1
	call WhirlpoolEffect
	call WhirlpoolEffect
	call WhirlpoolEffect
	delay 12
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 7, 0, RGB(0, 13, 23) @ Blue color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

WhirlpoolEffect::
	createsprite gWhirlpoolSpriteTemplate, ANIM_TARGET, 2, 0, 28, 384, 50, 8, 50, ANIM_TARGET
	delay 2
	createsprite gWhirlpoolSpriteTemplate, ANIM_TARGET, 2, 0, 32, 240, 40, 11, -46, ANIM_TARGET
	delay 2
	createsprite gWhirlpoolSpriteTemplate, ANIM_TARGET, 2, 0, 33, 416, 40, 4, 42, ANIM_TARGET
	delay 2
	createsprite gWhirlpoolSpriteTemplate, ANIM_TARGET, 2, 0, 31, 288, 45, 6, -42, ANIM_TARGET
	delay 2
	createsprite gWhirlpoolSpriteTemplate, ANIM_TARGET, 2, 0, 28, 448, 45, 11, 46, ANIM_TARGET
	delay 2
	createsprite gWhirlpoolSpriteTemplate, ANIM_TARGET, 2, 0, 33, 464, 50, 10, -50, ANIM_TARGET
	delay 2
	return

gMoveAnim_BEAT_UP::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_TARGET
	setalpha 12, 8
	choosetwoturnanim BeatUpLeft, BeatUpRight
BeatUpContinue::
	call BeatUpShakeTarget
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

BeatUpLeft::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -20, -20, ANIM_TARGET, 2
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 3, -20, -12, 8, ANIM_TARGET, 0
	call BeatUpShakeTarget
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 8, 0, ANIM_TARGET, 2
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 3, 8, 8, 8, ANIM_TARGET, 0
	goto BeatUpContinue

BeatUpRight::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 12, -20, ANIM_TARGET, 2
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 3, 12, -12, 8, ANIM_TARGET, 0
	call BeatUpShakeTarget
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -12, 0, ANIM_TARGET, 2
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 3, -12, 8, 8, ANIM_TARGET, 0
	goto BeatUpContinue

BeatUpShakeTarget::
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	return

@@@@@@@@@@@@@@@@@@@@@@@ GEN III @@@@@@@@@@@@@@@@@@@@@@@

gMoveAnim_FAKE_OUT::
	playsewithpan SE_M_FLATTER, SOUND_PAN_ABOVE
	createvisualtask AnimTask_FakeOut, 5
	waitforvisualfinish
	playsewithpan SE_M_SKETCH, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 5, 1
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_TARGET
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 16, 0, RGB_WHITE
	end

gMoveAnim_UPROAR::
	loadspritegfx ANIM_TAG_JAGGED_MUSIC_NOTE
	loadspritegfx ANIM_TAG_THIN_RING
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	playsewithpan SE_M_UPROAR, SOUND_PAN_ATTACKER
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 29, -12, 0
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -12, -29, 1
	delay 16
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	playsewithpan SE_M_UPROAR, SOUND_PAN_ATTACKER
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 12, -29, 1
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -29, -12, 0
	delay 16
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	playsewithpan SE_M_UPROAR, SOUND_PAN_ATTACKER
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, -24, 1
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, -24, 0
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_STOCKPILE::
	loadspritegfx ANIM_TAG_GRAY_ORB
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 8, 1, 0, 12, RGB_WHITE
	createvisualtask AnimTask_StockpileDeformMon, 5
	call StockpileAbsorb
	call StockpileAbsorb
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, 0, 12, 0, RGB_WHITE
	end

StockpileAbsorb::
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 55, 55, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -55, -55, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, 55, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, -55, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 55, -34, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 55, 34, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -55, -34, 13
	delay 1
	createsprite gStockpileAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -55, 34, 13
	delay 1
	return

gMoveAnim_SPIT_UP::
	loadspritegfx ANIM_TAG_RED_ORB_2
	loadspritegfx ANIM_TAG_IMPACT
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_SpitUpDeformMon, 5
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 8, 2
	delay 45
	playsewithpan SE_M_SPIT_UP, SOUND_PAN_ATTACKER
	delay 3
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 32, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 64, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 96, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 128, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 160, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 192, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 224, 12
	delay 5
	jumpifmoveturn 2, SpitUpStrong
	jumpifmoveturn 3, SpitUpStrongest
SpitUpContinue::
	delay 5
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 8, TRUE, FALSE
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, -12, 10, ANIM_TARGET, 1
	delay 5
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 12, -10, ANIM_TARGET, 1
	end

SpitUpStrong::
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 16, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 80, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 144, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 208, 12
	goto SpitUpContinue

SpitUpStrongest::
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 16, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 48, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 80, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 112, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 144, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 176, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 208, 12
	createsprite gSpitUpOrbSpriteTemplate, ANIM_ATTACKER, 2, 240, 12
	goto SpitUpContinue

gMoveAnim_SWALLOW::
	loadspritegfx ANIM_TAG_BLUE_ORB
	loadspritegfx ANIM_TAG_BLUE_STAR
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_SwallowDeformMon, 5
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 8, 2
	delay 38
	playsewithpan SE_M_SPIT_UP, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 2, 0, 12, 1
	call SwallowEffect
	jumpifmoveturn 2, SwallowGood
	jumpifmoveturn 3, SwallowBest
SwallowContinue::
	waitforvisualfinish
	call HealingEffect
	end

SwallowEffect::
	createsprite gSwallowBlueOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, -8
	delay 1
	createsprite gSwallowBlueOrbSpriteTemplate, ANIM_ATTACKER, 2, -24, -8
	delay 1
	createsprite gSwallowBlueOrbSpriteTemplate, ANIM_ATTACKER, 2, 16, -8
	delay 1
	createsprite gSwallowBlueOrbSpriteTemplate, ANIM_ATTACKER, 2, -16, -8
	delay 1
	createsprite gSwallowBlueOrbSpriteTemplate, ANIM_ATTACKER, 2, 24, -8
	delay 1
	return

SwallowGood::
	call SwallowEffect
	goto SwallowContinue

SwallowBest::
	call SwallowEffect
	call SwallowEffect
	goto SwallowContinue

gMoveAnim_HEAT_WAVE::
	loadspritegfx ANIM_TAG_FLYING_DIRT
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_FLYING_DIRT, 0, 6, 6, RGB_RED
	createvisualtask AnimTask_LoadSandstormBackground, 5, TRUE
	createvisualtask AnimTask_BlendBackground, 6, 6, RGB_RED
	panse_1B SE_M_HEAT_WAVE, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	delay 4
	createvisualtask AnimTask_MoveHeatWaveTargets, 5
	delay 12
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 10, 2304, 96, TRUE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 90, 2048, 96, TRUE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 50, 2560, 96, TRUE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 20, 2304, 96, TRUE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 70, 1984, 96, TRUE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 0, 2816, 96, TRUE
	delay 10
	createsprite gFlyingSandCrescentSpriteTemplate, ANIM_ATTACKER, 40, 60, 2560, 96, TRUE
	end

gMoveAnim_HAIL::
	loadspritegfx ANIM_TAG_HAIL
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 3, 0, 6, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_Hail1, 5
	loopsewithpan SE_M_HAIL, SOUND_PAN_ABOVE, 8, 10
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 3, 6, 0, RGB_BLACK
	end

gMoveAnim_TORMENT::
	loadspritegfx ANIM_TAG_ANGER
	loadspritegfx ANIM_TAG_THOUGHT_BUBBLE
	createvisualtask AnimTask_TormentAttacker, 2
	waitforvisualfinish
	createvisualtask AnimTask_BlendMonInAndOut, 2, ANIM_TARGET, RGB_RED, 10, 1, 1
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	delay 20
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	end

gMoveAnim_FLATTER::
	loadspritegfx ANIM_TAG_SPOTLIGHT
	loadspritegfx ANIM_TAG_CONFETTI
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_ENCORE2, SOUND_PAN_TARGET
	createvisualtask AnimTask_CreateSpotlight, 2
	createvisualtask AnimTask_HardwarePaletteFade, 2, BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN, 3, 0, 10, FALSE
	waitforvisualfinish
	createsprite gFlatterSpotlightSpriteTemplate, ANIM_TARGET, 2, 0, -8, 80
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 5, 2, ANIM_TARGET
	delay 10
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 5, 2, ANIM_TARGET
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_FLATTER, SOUND_PAN_ATTACKER
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	call CreateFlatterConfetti
	delay 5
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_FLATTER, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_HardwarePaletteFade, 2, BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN, 3, 10, 0, TRUE
	waitforvisualfinish
	createvisualtask AnimTask_RemoveSpotlight, 2
	end

CreateFlatterConfetti::
	createsprite gFlatterConfettiSpriteTemplate, ANIM_ATTACKER, 40, ANIM_ATTACKER
	createsprite gFlatterConfettiSpriteTemplate, ANIM_ATTACKER, 40, ANIM_TARGET
	return

gMoveAnim_WILL_O_WISP::
	loadspritegfx ANIM_TAG_WISP_FIRE
	loadspritegfx ANIM_TAG_WISP_ORB
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	playsewithpan SE_M_EMBER, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_EMBER, SOUND_PAN_ATTACKER, 10
	createvisualtask SoundTask_AdjustPanningVar, 2, SOUND_PAN_ATTACKER, SOUND_PAN_ATTACKER, 1, 0
	createsprite gWillOWispOrbSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 0
	delay 3
	createsprite gWillOWispOrbSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 1
	delay 3
	createsprite gWillOWispOrbSpriteTemplate, ANIM_ATTACKER, 4, 0, 0, 2
	delay 3
	createsprite gWillOWispOrbSpriteTemplate, ANIM_ATTACKER, 4, 0, 0, 3
	delay 40
	createvisualtask SoundTask_AdjustPanningVar, 2, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	waitforvisualfinish
	monbgprio_29
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 13, 1
	createsprite gWillOWispFireSpriteTemplate, ANIM_ATTACKER, 2, 0
	createsprite gWillOWispFireSpriteTemplate, ANIM_ATTACKER, 2, 42
	createsprite gWillOWispFireSpriteTemplate, ANIM_ATTACKER, 2, 84
	createsprite gWillOWispFireSpriteTemplate, ANIM_ATTACKER, 2, 126
	createsprite gWillOWispFireSpriteTemplate, ANIM_ATTACKER, 2, 168
	createsprite gWillOWispFireSpriteTemplate, ANIM_ATTACKER, 2, 210
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_MEMENTO::
	setalpha 0, 16
	createvisualtask AnimTask_InitMementoShadow, 2
	createvisualtask AnimTask_MoveAttackerMementoShadow, 5
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	delay 48
	playsewithpan SE_M_PSYBEAM2, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_EndMementoShadow, 2
	delay 12
	setalpha 0, 16
	monbg_22 ANIM_TARGET
	createvisualtask AnimTask_MoveTargetMementoShadow, 5
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg_23 ANIM_TARGET
	blendoff
	end

gMoveAnim_FACADE::
	loadspritegfx ANIM_TAG_SWEAT_DROP
	createvisualtask AnimTask_SquishAndSweatDroplets, 2, ANIM_ATTACKER, 3
	createvisualtask AnimTask_FacadeColorBlend, 2, ANIM_ATTACKER, 72
	loopsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER, 24, 3
	end

gMoveAnim_FOCUS_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	setbgbasedonside ANIM_TARGET, BG_IMPACT_PLAYER, BG_IMPACT_OPPONENT, TRUE
	waitbgfadein
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gFocusPunchFistSpriteTemplate, ANIM_TARGET, 2
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -8, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 8, 0, 24, 1
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, 2, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, -6, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 8, ANIM_TARGET, 0
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	restorebg
	waitbgfadein
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_SMELLING_SALT::
	loadspritegfx ANIM_TAG_TAG_HAND
	loadspritegfx ANIM_TAG_SMELLINGSALT_EFFECT
	createsprite gSmellingSaltsHandSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 2
	createsprite gSmellingSaltsHandSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, 2
	delay 32
	createvisualtask AnimTask_SmellingSaltsSquish, 3, 2
	loopsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET, 12, 2
	waitforvisualfinish
	delay 4
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 6, 2
	createsprite gSmellingSaltExclamationSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 8, 3
	loopsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET, 16, 3
	end

gMoveAnim_FOLLOW_ME::
	loadspritegfx ANIM_TAG_FINGER
	createsprite gFollowMeFingerSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	delay 18
	playsewithpan SE_M_ATTRACT, SOUND_PAN_ATTACKER
	delay 71
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 22, 3
	end

gMoveAnim_CHARGE::
	loadspritegfx ANIM_TAG_ELECTRIC_ORBS
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_ELECTRICITY
	monbg ANIM_ATTACKER
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 4, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_ElectricChargingParticles, 2, ANIM_ATTACKER, 60, 2, 12
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	delay 30
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	delay 30
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	createsprite gGrowingChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 0
	delay 25
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	delay 20
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	delay 15
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	delay 16
	loopsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER, 6, 5
	waitforvisualfinish
	createsprite gElectricPuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 16, 16
	delay 2
	createsprite gElectricPuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -16, -16
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 4, 4, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_TAUNT::
	loadspritegfx ANIM_TAG_FINGER_2
	loadspritegfx ANIM_TAG_THOUGHT_BUBBLE
	loadspritegfx ANIM_TAG_ANGER
	createsprite gThoughtBubbleSpriteTemplate, ANIM_ATTACKER, 11, ANIM_ATTACKER, 45
	playsewithpan SE_M_METRONOME, SOUND_PAN_ATTACKER
	delay 6
	createsprite gTauntFingerSpriteTemplate, ANIM_ATTACKER, 12, ANIM_ATTACKER
	delay 4
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 16, 2
	waitforvisualfinish
	delay 8
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	waitforvisualfinish
	delay 12
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	end

gMoveAnim_HELPING_HAND::
	loadspritegfx ANIM_TAG_TAG_HAND
	createvisualtask AnimTask_HelpingHandAttackerMovement, 5
	createsprite gHelpingHandClapSpriteTemplate, ANIM_ATTACKER, 40, FALSE
	createsprite gHelpingHandClapSpriteTemplate, ANIM_ATTACKER, 40, TRUE
	delay 19
	playsewithpan SE_M_ENCORE, SOUND_PAN_ABOVE
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATK_PARTNER, 2, 0, 5, 1
	delay 14
	playsewithpan SE_M_ENCORE, SOUND_PAN_ABOVE
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATK_PARTNER, 2, 0, 5, 1
	delay 20
	playsewithpan SE_M_ENCORE, SOUND_PAN_ABOVE
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATK_PARTNER, 3, 0, 10, 1
	createvisualtask AnimTask_BlendMonInAndOut, 2, ANIM_ATK_PARTNER, RGB_YELLOW, 12, 1, 1
	end

gMoveAnim_TRICK::
	loadspritegfx ANIM_TAG_ITEM_BAG_2
	loadspritegfx ANIM_TAG_SPEED_DUST
	createsprite gTrickBagSpriteTemplate, ANIM_ATTACKER, 2, -40, 80
	createsprite gTrickBagSpriteTemplate, ANIM_ATTACKER, 2, -40, 208
	delay 16
	playsewithpan SE_M_SKETCH, SOUND_PAN_ABOVE
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_TARGET
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_ATTACKER
	delay 30
	call TrickBagSoundEffect
	delay 8
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	playsewithpan SE_M_ATTRACT, SOUND_PAN_ABOVE
	createvisualtask AnimTask_ShakeMon, 3, ANIM_ATTACKER, 5, 0, 7, 2
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 5, 0, 7, 2
	end

TrickBagSoundEffect::
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ABOVE
	delay 16
	return

gMoveAnim_ROLE_PLAY::
	monbg ANIM_ATK_SIDE
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 16, RGB_WHITE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 10, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_TRI_ATTACK, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_DETECT, SOUND_PAN_ATTACKER, 30
	createvisualtask AnimTask_RolePlaySilhouette, 2
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 16, 0, RGB_WHITE
	delay 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 10, 0, RGB_BLACK
	end

gMoveAnim_WISH::
	loadspritegfx ANIM_TAG_GOLD_STARS
	loadspritegfx ANIM_TAG_SPARKLE_2
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 0, 10, RGB_BLACK
	waitforvisualfinish
	panse_27 SE_M_REFLECT, SOUND_PAN_TARGET, SOUND_PAN_ATTACKER, 253, 0
	createsprite gWishStarSpriteTemplate, ANIM_ATTACKER, 40
	waitforvisualfinish
	delay 60
	loopsewithpan SE_M_HEAL_BELL, SOUND_PAN_ATTACKER, 16, 3
	call GrantingStarsEffect
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 10, 0, RGB_BLACK
	end

gMoveAnim_ASSIST::
	loadspritegfx ANIM_TAG_PAW_PRINT
	createsprite gAssistPawprintSpriteTemplate, ANIM_ATTACKER, 50, 112, -16, 140, 128, 36
	delay 2
	createsprite gAssistPawprintSpriteTemplate, ANIM_ATTACKER, 50, 208, 128, -16, 48, 36
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ABOVE
	delay 2
	createsprite gAssistPawprintSpriteTemplate, ANIM_ATTACKER, 50, -16, 112, 256, -16, 36
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ABOVE
	delay 2
	createsprite gAssistPawprintSpriteTemplate, ANIM_ATTACKER, 50, 108, 128, 84, -16, 36
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ABOVE
	delay 2
	createsprite gAssistPawprintSpriteTemplate, ANIM_ATTACKER, 50, -16, 56, 256, 56, 36
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ABOVE
	end

gMoveAnim_INGRAIN::
	loadspritegfx ANIM_TAG_ROOTS
	loadspritegfx ANIM_TAG_ORBS
	createsprite gIngrainRootSpriteTemplate, ANIM_ATTACKER, 2, 16, 26, -1, 2, 150
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ATTACKER
	delay 10
	createsprite gIngrainRootSpriteTemplate, ANIM_ATTACKER, 2, -32, 20, 1, 1, 140
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ATTACKER
	delay 10
	createsprite gIngrainRootSpriteTemplate, ANIM_ATTACKER, 2, 32, 22, 1, 0, 130
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ATTACKER
	delay 10
	createsprite gIngrainRootSpriteTemplate, ANIM_ATTACKER, 2, -16, 25, -1, 3, 120
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ATTACKER
	delay 40
	createsprite gIngrainOrbSpriteTemplate, ANIM_ATTACKER, 3, 32, 26, -1, 3, 30
	delay 5
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 5
	createsprite gIngrainOrbSpriteTemplate, ANIM_ATTACKER, 3, -48, 20, 1, 2, 30
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 5
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 5
	createsprite gIngrainOrbSpriteTemplate, ANIM_ATTACKER, 3, 48, 26, -2, 3, 18
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	delay 10
	end

gMoveAnim_SUPERPOWER::
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_METEOR
	loadspritegfx ANIM_TAG_FLAT_ROCK
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	setalpha 12, 8
	createsprite gSuperpowerOrbSpriteTemplate, ANIM_TARGET, 2, ANIM_ATTACKER, TRUE
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER
	delay 20
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, 4, 1, 180, ANIM_TARGET
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_EARTHQUAKE, SOUND_PAN_ABOVE
	delay 40
	createsprite gSuperpowerRockSpriteTemplate, ANIM_ATTACKER, 41, 200, 96, 1, 120
	delay 8
	createsprite gSuperpowerRockSpriteTemplate, ANIM_ATTACKER, 41, 20, 248, 4, 112
	delay 8
	createsprite gSuperpowerRockSpriteTemplate, ANIM_ATTACKER, 41, 130, 160, 2, 104
	delay 8
	createsprite gSuperpowerRockSpriteTemplate, ANIM_ATTACKER, 41, 160, 192, 0, 96
	delay 8
	createsprite gSuperpowerRockSpriteTemplate, ANIM_ATTACKER, 41, 60, 288, 3, 88
	delay 74
	createsprite gSuperpowerFireballSpriteTemplate, ANIM_TARGET, 3, ANIM_ATTACKER
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	delay 16
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 8, 0, 16, 1
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_MAGIC_COAT::
	loadspritegfx ANIM_TAG_ORANGE_LIGHT_WALL
	setalpha 0, 16
	waitplaysewithpan SE_M_BARRIER, SOUND_PAN_ATTACKER, 15
	createsprite gMagicCoatWallSpriteTemplate, ANIM_ATTACKER, 3, 40, 0, ANIM_TAG_ORANGE_LIGHT_WALL
	waitforvisualfinish
	blendoff
	end

gMoveAnim_RECYCLE::
	loadspritegfx ANIM_TAG_RECYCLE
	monbg ANIM_ATTACKER
	setalpha 0, 16
	createsprite gRecycleSpriteTemplate, ANIM_ATTACKER, 2
	loopsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER, 24, 3
	waitforvisualfinish
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB_WHITE, 12, 2, 1
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_REVENGE::
	loadspritegfx ANIM_TAG_PURPLE_SCRATCH
	loadspritegfx ANIM_TAG_PURPLE_SWIPE
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createsprite gRevengeSmallScratchSpriteTemplate, ANIM_ATTACKER, 2, 10, -10, ANIM_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 4, 2, 8, RGB_RED
	waitforvisualfinish
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gRevengeBigScratchSpriteTemplate, ANIM_TARGET, 2, 10, -10, ANIM_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 10, 1
	createsprite gPersistHitSplatSpriteTemplate, ANIM_TARGET, 3, -10, -8, ANIM_TARGET, 1, 8
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 8
	createsprite gPersistHitSplatSpriteTemplate, ANIM_TARGET, 3, 10, 8, ANIM_TARGET, 1, 8
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli and Blackuser
gMoveAnim_BRICK_BREAK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_QUICK_GUARD
	loadspritegfx ANIM_TAG_BLUE_LIGHT_WALL
	loadspritegfx ANIM_TAG_TORN_METAL
	monbg ANIM_TARGET
	setalpha 12, 8
	choosetwoturnanim BrickBreakNormal, BrickBreakShatteredWall
BrickBreakEnd::
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff @ Fix Brick Break + Soft-Boiled glitch
	end

BrickBreakNormal::
	call BrickBreakKarateHand
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 4, 9, 1
	goto BrickBreakEnd

BrickBreakKarateHand::
	createsprite gKarateHandSpriteTemplate, ANIM_ATTACKER, 3, 0, -32, 15, 16
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	delay 19
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	return

BrickBreakShatteredWall::
	createsprite gBrickBreakWallSpriteTemplate, ANIM_ATTACKER, 3, ANIM_TARGET, 0, 0, 50, 10, FALSE, 0
	delay 31
	call BrickBreakKarateHand
	delay 3
	createsprite gBrickBreakWallShardSpriteTemplate, ANIM_ATTACKER, 2, 1, 0, -8, -12
	createsprite gBrickBreakWallShardSpriteTemplate, ANIM_ATTACKER, 2, 1, 1, 8, -12
	createsprite gBrickBreakWallShardSpriteTemplate, ANIM_ATTACKER, 2, 1, 2, -8, 12
	createsprite gBrickBreakWallShardSpriteTemplate, ANIM_ATTACKER, 2, 1, 3, 8, 12
	playsewithpan SE_M_BRICK_BREAK, SOUND_PAN_TARGET
	goto BrickBreakEnd

gMoveAnim_YAWN::
	loadspritegfx ANIM_TAG_PINK_CLOUD
	createvisualtask AnimTask_DeepInhale, 2, ANIM_ATTACKER
	playsewithpan SE_M_YAWN, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gYawnCloudSpriteTemplate, ANIM_TARGET, 5, 2
	playsewithpan SE_M_SPIT_UP, SOUND_PAN_ATTACKER
	delay 4
	createsprite gYawnCloudSpriteTemplate, ANIM_TARGET, 5, 1
	delay 4
	createsprite gYawnCloudSpriteTemplate, ANIM_TARGET, 5, 0
	waitforvisualfinish
	createvisualtask AnimTask_DeepInhale, 2, ANIM_TARGET
	playsewithpan SE_M_YAWN, SOUND_PAN_TARGET
	end

gMoveAnim_KNOCK_OFF::
	loadspritegfx ANIM_TAG_SLAM_HIT_2
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 4
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_TARGET
	createsprite gKnockOffStrikeSpriteTemplate, ANIM_TARGET, 2, -16, -16
	delay 8
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 5, 1, RGB_WHITE, 10, RGB_BLACK, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 2
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -12, 10, FALSE, 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	delay 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 0, 3, 6, 1
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	end

gMoveAnim_ENDEAVOR::
	loadspritegfx ANIM_TAG_SWEAT_DROP
	loadspritegfx ANIM_TAG_IMPACT
	createvisualtask AnimTask_SquishAndSweatDroplets, 2, ANIM_ATTACKER, 2
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 24, 2
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB(31, 21, 0), 12, 1, 2 @ Yellow color
	delay 6
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 5, FALSE, 1, 8, TRUE, FALSE
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 12, -12, ANIM_TARGET, 2
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	delay 24
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 5, FALSE, 1, 8, TRUE, FALSE
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -12, 12, ANIM_TARGET, 2
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	end

gMoveAnim_ERUPTION::
	loadspritegfx ANIM_TAG_WARM_ROCK
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 2, 0, 4, RGB_RED
	waitforvisualfinish
	call LaunchEruptionRocks
	call LaunchEruptionRocks
	delay 30
	createsprite gEruptionFallingRockSpriteTemplate, ANIM_ATTACKER, 40, 200, -32, 0, 100, 0
	createsprite gEruptionFallingRockSpriteTemplate, ANIM_ATTACKER, 40, 30, -32, 16, 90, 1
	createsprite gEruptionFallingRockSpriteTemplate, ANIM_ATTACKER, 40, 150, -32, 32, 60, 2
	createsprite gEruptionFallingRockSpriteTemplate, ANIM_ATTACKER, 40, 90, -32, 48, 80, 3
	createsprite gEruptionFallingRockSpriteTemplate, ANIM_ATTACKER, 40, 110, -32, 64, 50, 0
	createsprite gEruptionFallingRockSpriteTemplate, ANIM_ATTACKER, 40, 60, -32, 80, 70, 1
	delay 22
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT + 1, 8, 60
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT, 8, 60
	loopsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET, 16, 12
	delay 80
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 40, F_PAL_BG | F_PAL_BATTLERS, 4, 4, 0, RGB_RED
	end

LaunchEruptionRocks::
	createvisualtask AnimTask_EruptionLaunchRocks, 2
	waitplaysewithpan SE_M_EXPLOSION, SOUND_PAN_ATTACKER, 60
	waitforvisualfinish
	return

@ Credits: Skeli
gMoveAnim_SKILL_SWAP::
	loadspritegfx ANIM_TAG_BLUEGREEN_ORB
	loadspritegfx ANIM_TAG_ELECTRIC_ORBS @ Yellow color
	call SetPsychicBackground
	createvisualtask AnimTask_SkillSwap, 3, TRUE, 0
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_TARGET, RGB_WHITE, 12, 3, 1
	loopsewithpan SE_M_REVERSAL, SOUND_PAN_ATTACKER, 24, 3
	delay 16
	createvisualtask AnimTask_SkillSwap, 3, FALSE, 0
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB_WHITE, 12, 3, 1
	waitforvisualfinish
	call UnsetScrollingBg
	end

gMoveAnim_IMPRISON::
	loadspritegfx ANIM_TAG_HOLLOW_ORB
	loadspritegfx ANIM_TAG_X_SIGN
	call SetPsychicBackground
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_ImprisonOrbs, 5
	delay 8
	loopsewithpan SE_M_HORN_ATTACK, SOUND_PAN_ATTACKER, 8, 5
	waitforvisualfinish
	delay 4
	createsprite gRedXSpriteTemplate, ANIM_ATTACKER, 5, ANIM_ATTACKER, 40
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT, 1, 10
	playsewithpan SE_M_HYPER_BEAM, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	call UnsetScrollingBg
	end

gMoveAnim_REFRESH::
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_SPARKLE_2
	playsewithpan SE_M_STAT_INCREASE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_StatusClearedEffect, 2, FALSE
	waitforvisualfinish
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_ATTACKER
	call GrantingStarsEffect
	waitforvisualfinish
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 10, 0, RGB(12, 24, 30) @ Blue color
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE
	end

gMoveAnim_GRUDGE::
	loadspritegfx ANIM_TAG_PURPLE_FLAME
	monbg ANIM_ATTACKER
	monbgprio_29
	fadetobg BG_GHOST
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	waitbgfadein
	createvisualtask AnimTask_GrudgeFlames, 3, ANIM_ATTACKER
	loopsewithpan SE_M_EMBER, SOUND_PAN_ATTACKER, 16, 4
	delay 90
	playsewithpan SE_M_NIGHTMARE, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	restorebg
	waitbgfadein
	end

gMoveAnim_SNATCH::
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_WindUpLunge, 5, ANIM_ATTACKER, -12, 4, 10, 10, 12, 6
	end

gMoveAnim_SECRET_POWER::
	createvisualtask AnimTask_GetBattleTerrain, 5
	jumpreteq BATTLE_TERRAIN_GRASS, gMoveAnim_NEEDLE_ARM
	jumpreteq BATTLE_TERRAIN_LONG_GRASS, gMoveAnim_MAGICAL_LEAF
	jumpreteq BATTLE_TERRAIN_SAND, gMoveAnim_MUD_SHOT
	jumpreteq BATTLE_TERRAIN_UNDERWATER, gMoveAnim_WATERFALL
	jumpreteq BATTLE_TERRAIN_WATER, gMoveAnim_SURF
	jumpreteq BATTLE_TERRAIN_POND, gMoveAnim_BUBBLE_BEAM
	jumpreteq BATTLE_TERRAIN_MOUNTAIN, gMoveAnim_ROCK_THROW
	jumpreteq BATTLE_TERRAIN_CAVE, gMoveAnim_BITE
	jumpreteq BATTLE_TERRAIN_BUILDING, gMoveAnim_STRENGTH
	goto gMoveAnim_SLAM

gMoveAnim_DIVE::
	loadspritegfx ANIM_TAG_SPLASH
	loadspritegfx ANIM_TAG_SWEAT_BEAD
	choosetwoturnanim DiveSetUp, DiveAttack

DiveSetUp::
	loadspritegfx ANIM_TAG_ROUND_SHADOW
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	createsprite gDiveBallSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, 13, 336
	waitforvisualfinish
	playsewithpan SE_M_DIVE, SOUND_PAN_ATTACKER
	createsprite gDiveWaterSplashSpriteTemplate, ANIM_ATTACKER, 3, ANIM_ATTACKER
	call DiveSetUpWaterDroplets
	call DiveSetUpWaterDroplets
	call DiveSetUpWaterDroplets
	call DiveSetUpWaterDroplets
	call DiveSetUpWaterDroplets
	end

DiveSetUpWaterDroplets::
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_ATTACKER, 5, FALSE, ANIM_ATTACKER
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_ATTACKER, 5, TRUE, ANIM_ATTACKER
	return

DiveAttack::
	loadspritegfx ANIM_TAG_WATER_IMPACT
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_EXPLOSION, SOUND_PAN_TARGET
	createsprite gDiveWaterSplashSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET
	call DiveAttackWaterDroplets
	call DiveAttackWaterDroplets
	call DiveAttackWaterDroplets
	call DiveAttackWaterDroplets
	call DiveAttackWaterDroplets
	delay 12
	call RisingWaterHitEffect
	waitforvisualfinish
	visible ANIM_ATTACKER
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

DiveAttackWaterDroplets::
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, FALSE, ANIM_TARGET
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, TRUE, ANIM_TARGET
	return

gMoveAnim_ARM_THRUST::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_IMPACT
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_RotateMonSpriteToSide, 5, 8, 5, ANIM_ATTACKER, 0
	delay 6
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 3
	delay 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gArmThrustHandSpriteTemplate, ANIM_TARGET, 2, 10, -8, 14, 3
	waitforvisualfinish
	createvisualtask AnimTask_RotateMonSpriteToSide, 5, 8, 5, ANIM_ATTACKER, 1
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	choosetwoturnanim ArmThrustRight, ArmThrustLeft
ArmThrustContinue::
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 4, 0, 6, 1
	waitforvisualfinish
	blendoff
	end

ArmThrustRight::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 8, 0, ANIM_TARGET, 2
	goto ArmThrustContinue

ArmThrustLeft::
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -8, 0, ANIM_TARGET, 2
	goto ArmThrustContinue

gMoveAnim_CAMOUFLAGE::
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	setalpha 16, 0
	createvisualtask AnimTask_SetCamouflageBlend, 5, F_PAL_ATTACKER, 3, 0, 14
	delay 16
	createvisualtask AnimTask_AttackerFadeToInvisible, 2, 4
	playsewithpan SE_M_FAINT_ATTACK, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 8
	createvisualtask AnimTask_SetCamouflageBlend, 5, F_PAL_ATTACKER, 0, 0, 0
	waitforvisualfinish
	createvisualtask AnimTask_AttackerFadeFromInvisible, 2, 1
	waitforvisualfinish
	blendoff
	clearmonbg ANIM_ATK_SIDE
	end

gMoveAnim_TAIL_GLOW::
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	monbg ANIM_ATTACKER
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 4, RGB_BLACK
	waitforvisualfinish
	createsprite gGrowingChargeOrbSpriteTemplate, ANIM_ATTACKER, 66, ANIM_ATTACKER, 3, 18
	delay 18
	loopsewithpan SE_M_MORNING_SUN, SOUND_PAN_ATTACKER, 16, 6
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 4, 4, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_LUSTER_PURGE::
	loadspritegfx ANIM_TAG_WHITE_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_IMPACT
	call LusterPurgeLightBgEffect
	monbg ANIM_ATTACKER
	setalpha 12, 8
	playsewithpan SE_M_SOLAR_BEAM, SOUND_PAN_ATTACKER
	createsprite gLusterPurgeCircleSpriteTemplate, ANIM_ATTACKER, 41, 0, 0, ANIM_ATTACKER, TRUE
	delay 20
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_EXCEPT_ATTACKER, 2, 0, 16, RGB_WHITE
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WHITE_CIRCLE_OF_LIGHT, 2, 0, 16, RGB_WHITE
	waitforvisualfinish
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_IMPACT, 0, 12, 12, RGB(0, 0, 23) @ Blue color
	waitforvisualfinish
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	waitforvisualfinish
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_EXCEPT_ATTACKER, 2, 16, 0, RGB_WHITE
	createvisualtask AnimTask_HorizontalShake, 5, ANIM_TARGET, 5, 14
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	call UnsetScrollingBg
	end

LusterPurgeLightBgEffect::
	fadetobg BG_PSYCHIC
	waitbgfadeout
	createvisualtask AnimTask_FadeScreenToWhite, 5
	waitbgfadein
	return

LusterPurgeRandomHit::
	createsprite gRandomPosHitSplatSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 2
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_HYPER_BEAM, SOUND_PAN_TARGET
	delay 3
	return

gMoveAnim_MIST_BALL::
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	playsewithpan SE_M_STRING_SHOT, SOUND_PAN_ATTACKER
	createsprite gMistBallSpriteTemplate, ANIM_TARGET, 0, 0, 0, 0, 0, 30, 0
	waitforvisualfinish
	playsewithpan SE_M_SAND_ATTACK, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 10, 0
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_BG, 1, 1, RGB(23, 16, 31), 16, RGB_WHITE, 16 @ Blue color
	delay 0
	playsewithpan SE_M_HAZE, SOUND_PAN_ABOVE
	createvisualtask AnimTask_LoadMistTiles, 5
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 3, 0, 16, RGB_WHITE
	delay 8
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 4, 0, 70, 0
	delay 70
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 16, 0, RGB_WHITE
	end

gMoveAnim_FEATHER_DANCE::
	loadspritegfx ANIM_TAG_WHITE_FEATHER
	monbg ANIM_DEF_SIDE
	monbgprio_29
	playsewithpan SE_M_PETAL_DANCE, SOUND_PAN_TARGET
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 64, 2, 104, 11304, 32, ANIM_TARGET
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 32, 2, 104, 11304, 32, ANIM_TARGET
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 0, 2, 104, 11304, 32, ANIM_TARGET
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 224, 2, 104, 11304, 32, ANIM_TARGET
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 128, 2, 104, 11304, 32, ANIM_TARGET
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 192, 2, 104, 11304, 32, ANIM_TARGET
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 160, 2, 104, 11304, 32, ANIM_TARGET
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_TARGET, 0, 0, -16, 96, 2, 104, 11304, 32, ANIM_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_TEETER_DANCE::
	loadspritegfx ANIM_TAG_MUSIC_NOTES
	createvisualtask AnimTask_TeeterDanceMovement, 5
	createsprite gFastFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, -2
	playsewithpan SE_M_TEETER_DANCE, SOUND_PAN_ATTACKER
	delay 24
	createsprite gFastFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, -2
	playsewithpan SE_M_TEETER_DANCE, SOUND_PAN_ATTACKER
	delay 24
	createsprite gFastFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 0, -16, -2
	playsewithpan SE_M_TEETER_DANCE, SOUND_PAN_ATTACKER
	delay 24
	createsprite gFastFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 1, -8, -2
	playsewithpan SE_M_TEETER_DANCE, SOUND_PAN_ATTACKER
	delay 24
	createsprite gFastFlyingMusicNotesSpriteTemplate, ANIM_ATTACKER, 2, 2, 8, -2
	playsewithpan SE_M_TEETER_DANCE, SOUND_PAN_ATTACKER
	end

gMoveAnim_BLAZE_KICK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_SMALL_EMBER
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
	createsprite gSpinningHandOrFootSpriteTemplate, ANIM_TARGET, 3, 0, 0, 1, 30
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 7, RGB_WHITE
	delay 30
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 14, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 0, RGB_WHITE
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_BLACK, 8, RGB_BLACK, 0
	call FireSpreadEffect
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_MUD_SPORT::
	loadspritegfx ANIM_TAG_MUD_SAND
	createvisualtask AnimTask_Splash, 2, ANIM_ATTACKER, 6
	delay 24
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, FALSE, -4, -16
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, FALSE, 4, -12
	playsewithpan SE_M_DIG, SOUND_PAN_ATTACKER
	delay 32
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, FALSE, -3, -12
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, FALSE, 5, -14
	playsewithpan SE_M_DIG, SOUND_PAN_ATTACKER
	delay 32
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, FALSE, -5, -18
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, FALSE, 3, -14
	playsewithpan SE_M_DIG, SOUND_PAN_ATTACKER
	delay 16
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 220, 60
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 15
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 60, 100
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 25
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 140, 55
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 14
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 180, 50
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 10
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 20, 90
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 22
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 90, 90
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 22
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 160, 60
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 15
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 30, 90
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 22
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 120, 60
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 15
	delay 2
	createsprite gMudsportMudSpriteTemplate, ANIM_TARGET, 2, TRUE, 200, 40
	waitplaysewithpan SE_M_BUBBLE2, SOUND_PAN_ABOVE, 10
	end

gMoveAnim_ICE_BALL::
	loadspritegfx ANIM_TAG_ICE_CHUNK
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	createvisualtask AnimTask_GetRolloutCounter, 5
	callreteq 4, IceBallSetIceBg
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_ATTACKER
	createsprite gIceBallChunkSpriteTemplate, ANIM_TARGET, 2, 15, 0, -12, -16, 30, -40
	delay 28
	playsewithpan SE_M_BRICK_BREAK, SOUND_PAN_TARGET
	jumpreteq 0, IceBallWeakest
	jumpreteq 1, IceBallWeak
	jumpreteq 2, IceBallMediun
	jumpreteq 3, IceBallStrong
	jumpreteq 4, IceBallStrongest
IceBallEnd::
	end

IceBallSetIceBg::
	fadetobg BG_ICE
	return

IceBallWeakest::
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 8, TRUE, FALSE
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	goto IceBallEnd

IceBallWeak::
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 10, TRUE, FALSE
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	goto IceBallEnd

IceBallMediun::
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 14, TRUE, FALSE
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	goto IceBallEnd

IceBallStrong::
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 18, TRUE, FALSE
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	goto IceBallEnd

IceBallStrongest::
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 30, TRUE, FALSE
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	call IceBallImpactShard
	waitbgfadein
	delay 45
	restorebg
	waitbgfadein
	goto IceBallEnd

IceBallImpactShard::
	createsprite gIceBallImpactShardSpriteTemplate, ANIM_TARGET, 4, -12, -16
	return

gMoveAnim_NEEDLE_ARM::
	loadspritegfx ANIM_TAG_GREEN_SPIKE
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loopsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 2, 16
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 0, -32, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 22, -22, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 30, 0, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 20, 20, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 0, 28, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, -19, 19, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, -27, 0, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, -18, -18, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 0, -25, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 17, -17, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 23, 0, 16
	delay 2
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 16, 16, 16
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 18, 1
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 0, 0, 8, ANIM_TARGET, 0
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, 0, -24, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, 17, -17, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, 24, 0, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, 17, 17, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, 0, 24, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, -17, 17, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, -24, 0, 10
	createsprite gNeedleArmSpikeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, TRUE, -17, -17, 10
	end

gMoveAnim_SLACK_OFF::
	loadspritegfx ANIM_TAG_BLUE_STAR
	createvisualtask AnimTask_SlackOffSquish, 2, ANIM_ATTACKER
	playsewithpan SE_M_YAWN, SOUND_PAN_ATTACKER
	waitforvisualfinish
	call HealingEffect
	end

gMoveAnim_HYPER_VOICE::
	loadspritegfx ANIM_TAG_THIN_RING
	call HyperVoiceEffect
	waitforvisualfinish
	delay 8
	call HyperVoiceEffect
	waitforvisualfinish
	end

HyperVoiceEffect::
	createvisualtask SoundTask_PlayCryWithEcho, 5
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 8, 0, RGB_YELLOW
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -5, -5, 5, ANIM_ATTACKER, ST_OAM_OBJ_NORMAL
	createsprite gHyperVoiceRingSpriteTemplate, ANIM_ATTACKER, 0, 45, 0, TRUE, 0, 0
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 1, 0, 6, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 1, 0, 6, 1
	createvisualtask AnimTask_ShakeBattleTerrain, 2, 1, 0, 6, 1
	createvisualtask SoundTask_WaitForCry, 5
	return

gMoveAnim_POISON_FANG::
	loadspritegfx ANIM_TAG_FANG_ATTACK
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gFangSpriteTemplate, ANIM_TARGET, 2
	delay 10
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 3, 0, 10, 1
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 0, 4, 0, 12, RGB(26, 0, 26) @ Purple color
	call PoisonBubblesEffect
	end

gMoveAnim_CRUSH_CLAW::
	loadspritegfx ANIM_TAG_BLUE_LIGHT_WALL
	loadspritegfx ANIM_TAG_CLAW_SLASH
	loadspritegfx ANIM_TAG_TORN_METAL
	monbg ANIM_TARGET
	setalpha 12, 8
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 4
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 18, 1
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, -10, -10, 0
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, -10, 10, 0
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	delay 12
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, 10, -10, 1
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, 10, 10, 1
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_BLAST_BURN::
	loadspritegfx ANIM_TAG_FIRE_PLUME
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SMALL_EMBER
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_SACRED_FIRE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 10, RGB(29, 1, 1) @ Red color
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -32, 0, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -20, -10, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 0, -16, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 20, -10, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 32, 0, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 20, 10, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, 0, 16, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -20, 10, 24, 0, 0, 0
	delay 25
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_ATTACKER
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -64, 0, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 6, -40, -20, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 70, 0, -32, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 70, 40, -20, 24, 0, 0, 0
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 6, 0, 8, 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 64, 0, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 40, 20, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -40, 20, 24, 0, 0, 0
	delay 25
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_ATTACKER
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -96, 0, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 6, -60, -30, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 70, 0, -48, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 70, 60, -30, 24, 0, 0, 0
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -4, 3, 1, 0
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 12, 0, 20, 1
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 96, 0, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 66, 60, 30, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, 0, 48, 24, 0, 0, 0
	createsprite gFirePlumeSpriteTemplate, ANIM_ATTACKER, 2, -60, 30, 24, 0, 0, 0
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 11, RGB_RED
	call FireSpreadEffect
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 10, 0, RGB(29, 1, 1) @ Red color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Skeli and Blackuser
gMoveAnim_HYDRO_CANNON::
	loadspritegfx ANIM_TAG_WATER_ORB
	fadetobg BG_HYDRO_CANNON
	waitbgfadein
	monbg ANIM_DEF_SIDE
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_WATER_ORB, 2, 10, 10, RGB(5, 16, 23) @ Darken a little the blue
	delay 16
	panse_1B SE_M_HYDRO_PUMP, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createvisualtask AnimTask_HorizontalShake, 5, MAX_BATTLERS_COUNT + 1, 1, 67
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 69, 1
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	panse_1B SE_M_HYDRO_PUMP, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	call HydroCannonBeam
	waitforvisualfinish
	delay 5
	clearmonbg ANIM_DEF_SIDE
	restorebg
	waitbgfadein
	end

HydroCannonBeam::
	createsprite gHydroCannonBeamSpriteTemplate, ANIM_TARGET, 0, 15, 0, 20, 0, 0
	createsprite gHydroCannonBeamSpriteTemplate, ANIM_TARGET, 0, 15, 0, 20, 10, 5
	createsprite gHydroCannonBeamSpriteTemplate, ANIM_TARGET, 0, 15, 0, 20, -10, -5
	delay 2
	createsprite gHydroCannonBeamSpriteTemplate, ANIM_TARGET, 0, 15, 0, 20, 20, 10
	createsprite gHydroCannonBeamSpriteTemplate, ANIM_TARGET, 0, 15, 0, 20, -20, -10
	delay 2
	return

gMoveAnim_METEOR_MASH::
	loadspritegfx ANIM_TAG_GOLD_STARS
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	panse_1B SE_M_BARRIER, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 3, 0
	fadetobg BG_COSMIC
	waitbgfadein
	waitforvisualfinish
	createsprite gMeteorMashStarSpriteTemplate, ANIM_TARGET, 3, -48, -64, 72, 32, 30
	delay 10
	createsprite gMeteorMashStarSpriteTemplate, ANIM_TARGET, 3, -112, -64, 8, 32, 30
	delay 40
	createsprite gSpinningHandOrFootSpriteTemplate, ANIM_TARGET, 3, 0, 0, 0, 30
	createsprite gMeteorMashStarSpriteTemplate, ANIM_TARGET, 3, -80, -64, 40, 32, 30
	delay 20
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 5, 0, 20, 1
	waitforvisualfinish
	delay 10
	restorebg
	waitbgfadein
	end

gMoveAnim_ASTONISH::
	loadspritegfx ANIM_TAG_SWEAT_BEAD
	playsewithpan SE_M_ENCORE, SOUND_PAN_ATTACKER
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 25
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, FALSE, ANIM_TARGET
	playsewithpan SE_M_SKETCH, SOUND_PAN_TARGET
	createsprite gSprayWaterDropletSpriteTemplate, ANIM_TARGET, 5, TRUE, ANIM_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 5, 1
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_TARGET
	end

gMoveAnim_WEATHER_BALL::
	loadspritegfx ANIM_TAG_WEATHER_BALL
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 8, 1, ANIM_ATTACKER
	delay 8
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	createsprite gWeatherBallUpSpriteTemplate, ANIM_ATTACKER, 2
	waitforvisualfinish
	delay 15
	playsewithpan SE_M_DETECT, SOUND_PAN_ABOVE
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 5, 1, RGB_WHITE, 10, RGB_BLACK, 0
	waitforvisualfinish
	createvisualtask AnimTask_GetWeather, 2
	jumpreteq ENUM_WEATHER_SUN, WeatherBallFire
	jumpreteq ENUM_WEATHER_SUN_PRIMAL, WeatherBallFire
	jumpreteq ENUM_WEATHER_RAIN, WeatherBallWater
	jumpreteq ENUM_WEATHER_RAIN_PRIMAL, WeatherBallWater
	jumpreteq ENUM_WEATHER_SANDSTORM, WeatherBallSandstorm
	jumpreteq ENUM_WEATHER_HAIL, WeatherBallIce
WeatherBallNormal::
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gWeatherBallNormalDownSpriteTemplate, ANIM_TARGET, 2, 25, 0, 0
	waitforvisualfinish
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, -10, 0, ANIM_TARGET, 2
WeatherBallShakeAndEnd::
	call WeatherBallTargetShake
WeatherBallEnd::
	end

WeatherBallTargetShake::
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 8, 1
	return

WeatherBallFire::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	createsprite gWeatherBallFireDownSpriteTemplate, ANIM_TARGET, 2, 25, 40, 10
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
	delay 10
	createsprite gWeatherBallFireDownSpriteTemplate, ANIM_TARGET, 2, 25, -40, 20
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
	delay 10
	createsprite gWeatherBallFireDownSpriteTemplate, ANIM_TARGET, 2, 25, 0, 0
	playsewithpan SE_M_FLAME_WHEEL, SOUND_PAN_TARGET
	waitforvisualfinish
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET
	goto WeatherBallShakeAndEnd

WeatherBallWater::
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	createsprite gWeatherBallWaterDownSpriteTemplate, ANIM_TARGET, 2, 25, 50, 10
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	delay 8
	createsprite gWeatherBallWaterDownSpriteTemplate, ANIM_TARGET, 2, 25, -20, 20
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	delay 13
	createsprite gWeatherBallWaterDownSpriteTemplate, ANIM_TARGET, 2, 25, 0, 0
	playsewithpan SE_M_CRABHAMMER, SOUND_PAN_TARGET
	waitforvisualfinish
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_TARGET
	goto WeatherBallShakeAndEnd

WeatherBallSandstorm::
	loadspritegfx ANIM_TAG_ROCKS
	createsprite gWeatherBallRockDownSpriteTemplate, ANIM_TARGET, 2, 25, 30, 0
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 5
	createsprite gWeatherBallRockDownSpriteTemplate, ANIM_TARGET, 2, 25, -40, 20
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 14
	createsprite gWeatherBallRockDownSpriteTemplate, ANIM_TARGET, 2, 25, 0, 0
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	waitforvisualfinish
	playsewithpan SE_M_STRENGTH, SOUND_PAN_TARGET
	call IronTailRockScatterNoHit
	goto WeatherBallShakeAndEnd

WeatherBallIce::
	loadspritegfx ANIM_TAG_HAIL
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	createsprite gWeatherBallIceDownSpriteTemplate, ANIM_TARGET, 2, 25, -40, 20
	playsewithpan SE_M_HAIL, SOUND_PAN_TARGET
	delay 10
	createsprite gWeatherBallIceDownSpriteTemplate, ANIM_TARGET, 2, 25, 40, 0
	playsewithpan SE_M_HAIL, SOUND_PAN_TARGET
	delay 10
	createsprite gWeatherBallIceDownSpriteTemplate, ANIM_TARGET, 2, 25, 0, 0
	playsewithpan SE_M_HAIL, SOUND_PAN_TARGET
	waitforvisualfinish
	call WeatherBallTargetShake
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_TARGET
	call IceCrystalEffectShort
	goto WeatherBallEnd

gMoveAnim_AROMATHERAPY::
	loadspritegfx ANIM_TAG_FLOWER
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_SPARKLE_2
	monbg ANIM_ATTACKER
	playsewithpan SE_M_PETAL_DANCE, SOUND_PAN_ABOVE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_BG, 0, 0, 7, RGB(13, 31, 12) @ Green color
	delay 2
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 0, 24, 16, 0, 2, 2, 0, ANIM_ATTACKER
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 66, 64, 24, 0, 3, 1, 1, ANIM_ATTACKER
	createsprite gAromatherapyBigFlowerSpriteTemplate, ANIM_ATTACKER, 0, 16, 24, 0, 2, 1, 0, ANIM_ATTACKER
	delay 20
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 66, 48, 12, 0, 4, 3, 1, ANIM_ATTACKER
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 0, 100, 16, 0, 3, 2, 0, ANIM_ATTACKER
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 0, 74, 24, 180, 3, 2, 0, ANIM_ATTACKER
	delay 10
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 66, 80, 30, 0, 4, 1, 1, ANIM_ATTACKER
	createsprite gAromatherapySmallFlowerSpriteTemplate, ANIM_ATTACKER, 0, 128, 12, 0, 3, 3, 0, ANIM_ATTACKER
	createsprite gAromatherapyBigFlowerSpriteTemplate, ANIM_ATTACKER, 0, 90, 16, 0, 2, 1, 0, ANIM_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	delay 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_BG, 0, 7, 0, RGB(13, 31, 12) @ Green color
	delay 1
	playsewithpan SE_M_STAT_INCREASE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_StatusClearedEffect, 2, TRUE
	waitforvisualfinish
	call HealBellSparkles
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_BG | F_PAL_ATK_SIDE | F_PAL_ANIM_1, 3, 10, 0, RGB(13, 31, 12) @ Green color
	createsprite gBlendThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 16, 0, 0, ANIM_ATTACKER, FALSE
	end

gMoveAnim_FAKE_TEARS::
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SMALL_BUBBLES, 0, 4, 4, RGB(12, 11, 31) @ Blue color
	waitforvisualfinish
	createvisualtask AnimTask_RockMonBackAndForth, 5, ANIM_ATTACKER, 2, 1
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 12, 4
	call FakeTearsTears
	call FakeTearsTears
	end

FakeTearsTears::
	delay 8
	createsprite gTearDropSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 1
	createsprite gTearDropSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 1
	delay 8
	createsprite gTearDropSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 2, 1
	createsprite gTearDropSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 3, 1
	return

gMoveAnim_AIR_CUTTER::
	loadspritegfx ANIM_TAG_AIR_WAVE
	loadspritegfx ANIM_TAG_CUT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_AirCutterProjectile, 2, 32, -24, 1536, 2, 128
	waitforvisualfinish
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 2
	delay 5
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_DEF_PARTNER, 2, 0, 8, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_OVERHEAT::
	loadspritegfx ANIM_TAG_SMALL_EMBER
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 18
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 5, RGB(28, 0, 0) @ Red color
	waitforvisualfinish
	createvisualtask AnimTask_AllocBackupPalBuffer, 5
	createvisualtask AnimTask_CopyPalUnfadedToBackup, 5, 0, 1
	createvisualtask AnimTask_CopyPalFadedToUnfaded, 5, 0
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_CopyPalUnfadedToBackup, 5, 1, 0
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, 1, 0, 13, RGB(28, 0, 0) @ Red color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 2, 0, 15, 1
	waitforvisualfinish
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_ATTACKER
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 0, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 32, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 64, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 96, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 128, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 160, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 192, 30, 25, -20
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 224, 30, 25, -20
	delay 5
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 0, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 32, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 64, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 96, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 128, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 160, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 192, 30, 25, 0
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 224, 30, 25, 0
	delay 5
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 0, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 32, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 64, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 96, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 2, 1, 128, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 160, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 192, 30, 25, 10
	createsprite gOverheatFlameSpriteTemplate, ANIM_ATTACKER, 66, 1, 224, 30, 25, 10
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -5, 3, ANIM_TARGET, 0
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 10, 0, 25, 1
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 8, -5, ANIM_TARGET, 0
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 10, 10, ANIM_TARGET, 0
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	delay 8
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 0
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	createvisualtask AnimTask_CopyPalFadedToUnfaded, 5, 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, -1, 0, 13, RGB(18, 18, 18) @ Gray color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 3, 0, 15, 1
	waitforvisualfinish
	createvisualtask AnimTask_CopyPalUnfadedFromBackup, 5, 0, 1
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 5, 0, RGB(28, 0, 0) @ Red color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	delay 15
	createvisualtask AnimTask_CopyPalUnfadedFromBackup, 5, 1, 0
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, 0, 13, 0, RGB(18, 18, 18) @ Gray color
	waitforvisualfinish
	createvisualtask AnimTask_FreeBackupPalBuffer, 5
	end

gMoveAnim_ODOR_SLEUTH::
	monbg ANIM_TARGET
	createvisualtask AnimTask_OdorSleuthMovement, 5
	delay 24
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 3, 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	delay 6
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 3, 4
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	delay 1
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 3, 1, RGB_WHITE, 16, RGB_WHITE, 0
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	end

gMoveAnim_ROCK_TOMB::
	loadspritegfx ANIM_TAG_X_SIGN
	loadspritegfx ANIM_TAG_ROCKS
	createvisualtask AnimTask_ShakeBattleTerrain, 2, 2, 0, 10, 1
	waitforvisualfinish
	createsprite gRockTombRockSpriteTemplate, ANIM_TARGET, 2, 20, 12, 64, 114, 0
	delay 8
	call RockTombTerrainShake
	createsprite gRockTombRockSpriteTemplate, ANIM_TARGET, 2, -20, 12, 64, 98, 0
	delay 8
	call RockTombTerrainShake
	createsprite gRockTombRockSpriteTemplate, ANIM_TARGET, 66, 3, 6, 64, 82, 0
	delay 8
	call RockTombTerrainShake
	createsprite gRockTombRockSpriteTemplate, ANIM_TARGET, 2, -3, 13, 64, 66, 0
	delay 8
	call RockTombTerrainShake
	delay 16
	playsewithpan SE_M_HYPER_BEAM, SOUND_PAN_TARGET
	createsprite gRedXSpriteTemplate, ANIM_TARGET, 5, ANIM_TARGET, 50
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 3, 0, 20, 1
	createvisualtask AnimTask_ShakeBattleTerrain, 2, 2, 0, 10, 1
	end

RockTombTerrainShake::
	createvisualtask AnimTask_ShakeBattleTerrain, 2, 0, 2, 3, 1
	playsewithpan SE_M_STRENGTH, SOUND_PAN_TARGET
	delay 8
	return

gMoveAnim_SILVER_WIND::
	loadspritegfx ANIM_TAG_SPARKLE_6
	panse_1B SE_M_GUST, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_ABOVE
	monbg ANIM_DEF_SIDE
	monbgprio_29
	createvisualtask AnimTask_BlendExcept, 10, F_PAL_EXCEPT_TARGET, 0, 0, 4, RGB_BLACK
	setbgbasedonside ANIM_ATTACKER, BG_BUG_PLAYER, BG_BUG_OPPONENT, TRUE
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -1536, 0, TRUE
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 4, 4, RGB_BLACK
	waitbgfadein
	createsprite gSilverWindBigSparkSpriteTemplate, ANIM_TARGET, 66, -32, 16, 0, 6, 2, 3, ANIM_TARGET
	createsprite gSilverWindBigSparkSpriteTemplate, ANIM_TARGET, 66, -8, 18, 64, 3, 2, 2, ANIM_TARGET
	createsprite gSilverWindBigSparkSpriteTemplate, ANIM_ATTACKER, 120, -24, 18, 90, 5, 1, 2, ANIM_TARGET
	createsprite gSilverWindBigSparkSpriteTemplate, ANIM_ATTACKER, 120, -40, 14, 128, 4, 1, 2, ANIM_TARGET
	createsprite gSilverWindMediumSparkSpriteTemplate, ANIM_TARGET, 66, -32, 16, 0, 6, 2, 3, ANIM_TARGET
	createsprite gSilverWindMediumSparkSpriteTemplate, ANIM_TARGET, 66, -8, 18, 64, 3, 2, 2, ANIM_TARGET
	createsprite gSilverWindMediumSparkSpriteTemplate, ANIM_ATTACKER, 120, -24, 18, 90, 5, 1, 2, ANIM_TARGET
	createsprite gSilverWindMediumSparkSpriteTemplate, ANIM_ATTACKER, 120, -40, 14, 128, 4, 1, 2, ANIM_TARGET
	createsprite gSilverWindSmallSparkSpriteTemplate, ANIM_TARGET, 66, -32, 16, 0, 6, 2, 3, ANIM_TARGET
	createsprite gSilverWindSmallSparkSpriteTemplate, ANIM_TARGET, 66, -8, 18, 64, 3, 2, 2, ANIM_TARGET
	createsprite gSilverWindSmallSparkSpriteTemplate, ANIM_ATTACKER, 120, -24, 18, 90, 5, 1, 2, ANIM_TARGET
	createsprite gSilverWindSmallSparkSpriteTemplate, ANIM_ATTACKER, 120, -40, 14, 128, 4, 1, 2, ANIM_TARGET
	delay 6
	createsprite gSilverWindBigSparkSpriteTemplate, ANIM_TARGET, 66, -4, 16, 0, 6, 1, 2, ANIM_TARGET
	createsprite gSilverWindBigSparkSpriteTemplate, ANIM_TARGET, 66, -16, 12, 192, 5, 2, 3, ANIM_TARGET
	createsprite gSilverWindMediumSparkSpriteTemplate, ANIM_TARGET, 66, -4, 16, 0, 6, 1, 2, ANIM_TARGET
	createsprite gSilverWindMediumSparkSpriteTemplate, ANIM_TARGET, 66, -16, 12, 192, 5, 2, 3, ANIM_TARGET
	createsprite gSilverWindSmallSparkSpriteTemplate, ANIM_TARGET, 66, -4, 16, 0, 6, 1, 2, ANIM_TARGET
	createsprite gSilverWindSmallSparkSpriteTemplate, ANIM_TARGET, 66, -16, 12, 192, 5, 2, 3, ANIM_TARGET
	waitforvisualfinish
	playsewithpan SE_M_GUST2, SOUND_PAN_TARGET
	clearmonbg ANIM_DEF_SIDE
	restorebg
	waitbgfadeout
	createvisualtask AnimTask_BlendExcept, 10, F_PAL_EXCEPT_TARGET, 0, 4, 0, RGB_BLACK
	setargret -1 @ Signal to end bg slide
	waitbgfadein
	end

gMoveAnim_METAL_SOUND::
	loadspritegfx ANIM_TAG_METAL_SOUND_WAVES
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 2, 0, 8, 1
	call MetalSoundRings
	call MetalSoundRings
	call MetalSoundRings
	call MetalSoundRings
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

MetalSoundRings::
	panse_1B SE_M_SCREECH, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createsprite gMetalSoundSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 30, 0
	delay 2
	return

gMoveAnim_GRASS_WHISTLE::
	loadspritegfx ANIM_TAG_MUSIC_NOTES
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 4, RGB(18, 31, 12) @ Green color
	waitforvisualfinish
	createvisualtask AnimTask_MusicNotesRainbowBlend, 2
	panse_1B SE_M_GRASSWHISTLE, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 7, 1, 0
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 6, 1, 0
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 1, 1, 0
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 1, 0
	delay 5
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 3, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 5, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 6, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 2, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 1, 1, 0
	delay 4
	createsprite gWavyMusicNotesSpriteTemplate, ANIM_TARGET, 2, 5, 1, 0
	waitforvisualfinish
	createvisualtask AnimTask_MusicNotesClearRainbowBlend, 2
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 4, 4, 0, RGB(18, 31, 12) @ Green color
	end

gMoveAnim_TICKLE::
	loadspritegfx ANIM_TAG_EYE_SPARKLE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, 0, 0, 16, RGB_BLACK
	waitforvisualfinish
	call EyeSparkles
	playsewithpan SE_M_DETECT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_ATTACKER, 0, 16, 0, RGB_BLACK
	waitforvisualfinish
	delay 20
	createvisualtask AnimTask_SwayMon, 3, 0, 6, 1280, 3, ANIM_ATTACKER
	delay 12
	createvisualtask AnimTask_RockMonBackAndForth, 3, ANIM_TARGET, 6, 2
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_TARGET, 8, 8
	end

gMoveAnim_COSMIC_POWER::
	loadspritegfx ANIM_TAG_SPARKLE_2
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_COSMIC_POWER, SOUND_PAN_ABOVE
	playsewithpan SE_M_COSMIC_POWER, SOUND_PAN_ABOVE
	createvisualtask AnimTask_BlendNonAttackerPalettes, 2, 0, 0, 15, RGB_BLACK
	waitforvisualfinish
	fadetobg BG_COSMIC
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 2, 0, 128, FALSE
	waitbgfadein
	delay 70
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MORNING_SUN, SOUND_PAN_ATTACKER
	call GrantingStarsEffect
	delay 32
	createvisualtask AnimTask_BlendNonAttackerPalettes, 2, 0, 15, 0, RGB_BLACK
	waitforvisualfinish
	call UnsetScrollingBg
	end

gMoveAnim_WATER_SPOUT::
	loadspritegfx ANIM_TAG_GLOWY_BLUE_ORB
	loadspritegfx ANIM_TAG_WATER_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_WaterSpoutLaunch, 5
	playsewithpan SE_M_HEADBUTT, SOUND_PAN_ATTACKER
	delay 44
	playsewithpan SE_M_DIVE, SOUND_PAN_ATTACKER
	waitforvisualfinish
	delay 16
	createvisualtask AnimTask_WaterSpoutRain, 5
	playsewithpan SE_M_SURF, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_SIGNAL_BEAM::
	loadspritegfx ANIM_TAG_GLOWY_RED_ORB
	loadspritegfx ANIM_TAG_GLOWY_GREEN_ORB
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 25, 1
	delay 6
	panse_1B SE_M_BUBBLE_BEAM, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 1, 0
	createvisualtask AnimTask_StartSinAnimTimer, 5, 100
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 3, 0, 25, 1
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_TARGET, 8, 5, RGB_RED, 8, RGB(1, 30, 0), 8 @ Green color
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	call SignalBeamOrbs
	end

SignalBeamOrbs::
	createsprite gSignalBeamRedOrbSpriteTemplate, ANIM_TARGET, 3, 10, 10, 16
	createsprite gSignalBeamGreenOrbSpriteTemplate, ANIM_TARGET, 3, 10, 10, -16
	delay 1
	return

gMoveAnim_SHADOW_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	fadetobg BG_GHOST
	waitbgfadein
	monbg ANIM_ATK_SIDE
	setalpha 9, 8
	createvisualtask AnimTask_AttackerPunchWithTrace, 2, RGB_BLACK, 13
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 0, 0, 8, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	restorebg
	waitbgfadein
	end

gMoveAnim_EXTRASENSORY::
	call SetPsychicBackground
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB(27, 27, 0), 12, 1, 1 @ Yellow color
	createvisualtask AnimTask_ExtrasensoryDistortion, 5, 0
	playsewithpan SE_M_BIND, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB(27, 27, 0), 12, 1, 1 @ Yellow color
	createvisualtask AnimTask_ExtrasensoryDistortion, 5, 1
	playsewithpan SE_M_BIND, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_TransparentCloneGrowAndShrink, 5, ANIM_ATTACKER
	createvisualtask AnimTask_ExtrasensoryDistortion, 5, 2
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

gMoveAnim_SKY_UPPERCUT::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	fadetobg BG_IN_AIR
	waitbgfadeout
	playsewithpan SE_M_SKY_UPPERCUT, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_MoveSkyUppercutBg, 5, 55
	waitbgfadein
	setalpha 12, 8
	delay 38
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 28, 0, FALSE, 5
	delay 4
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 6, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -28, 28, ANIM_TARGET, 1
	delay 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -15, 8, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	delay 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -5, -12, ANIM_TARGET, 1
	delay 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, -32, ANIM_TARGET, 1
	delay 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 5, -52, ANIM_TARGET, 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -26, 16, TRUE, 4
	delay 4
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 0, 3, 6, 1
	delay 30
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 6
	delay 4
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	call UnsetScrollingBg
	end

gMoveAnim_SAND_TOMB::
	loadspritegfx ANIM_TAG_MUD_SAND
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 0, 7, RGB(19, 17, 0) @ Brown color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 2, 43, 1
	playsewithpan SE_M_SAND_TOMB, SOUND_PAN_TARGET
	call SandTombSwirlingDirt
	call SandTombSwirlingDirt
	call SandTombSwirlingDirt
	delay 22
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 0, F_PAL_TARGET, 2, 7, 0, RGB(19, 17, 0) @ Brown color
	end

SandTombSwirlingDirt::
	createsprite gSwirlingDirtSpriteTemplate, ANIM_TARGET, 2, 0, 32, 528, 30, 10, 50, ANIM_TARGET
	delay 2
	createsprite gSwirlingDirtSpriteTemplate, ANIM_TARGET, 2, 0, 36, 480, 20, 13, -46, ANIM_TARGET
	delay 2
	createsprite gSwirlingDirtSpriteTemplate, ANIM_TARGET, 2, 0, 37, 576, 20, 5, 42, ANIM_TARGET
	delay 2
	createsprite gSwirlingDirtSpriteTemplate, ANIM_TARGET, 2, 0, 35, 400, 25, 8, -42, ANIM_TARGET
	delay 2
	createsprite gSwirlingDirtSpriteTemplate, ANIM_TARGET, 2, 0, 32, 512, 25, 13, 46, ANIM_TARGET
	delay 2
	createsprite gSwirlingDirtSpriteTemplate, ANIM_TARGET, 2, 0, 37, 464, 30, 12, -50, ANIM_TARGET
	delay 2
	return

gMoveAnim_SHEER_COLD::
	loadspritegfx ANIM_TAG_ICE_CUBE
	fadetobg BG_ICE
	waitbgfadeout
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_ABOVE
	waitbgfadein
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_FrozenIceCube, 2, ANIM_TARGET
	waitplaysewithpan SE_M_HAIL, SOUND_PAN_TARGET, 17
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	restorebg
	waitbgfadein
	end

gMoveAnim_MUDDY_WATER::
	panse_1B SE_M_WHIRLPOOL, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createvisualtask AnimTask_CreateSurfWave, 2, 1
	end

gMoveAnim_BULLET_SEED::
	loadspritegfx ANIM_TAG_SEED
	call GunBulletSeed
	call GunBulletSeed
	call GunBulletSeed
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 30, 1
	call GunBulletSeed
	call GunBulletSeed
	call GunBulletSeed
	call GunBulletSeed
	call GunBulletSeed
	call GunBulletSeed
	call GunBulletSeed
	end

GunBulletSeed::
	createsprite gBulletSeedSpriteTemplate, ANIM_TARGET, 2, 20, 0, SE_M_HORN_ATTACK
	delay 5
	return

gMoveAnim_AERIAL_ACE::
	loadspritegfx ANIM_TAG_CUT
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 24, 6, 1, 5
	createvisualtask AnimTask_CloneBattlerSpriteWithBlend, 2, ANIM_ATTACKER, 4, 7, 3
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 0
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 10, 1
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 3, 1, RGB_BLACK, 10, RGB_BLACK, 0
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_ICICLE_SPEAR::
	loadspritegfx ANIM_TAG_ICICLE_SPEAR
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_ICY_WIND, SOUND_PAN_ATTACKER
	createsprite gIcicleSpearSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, -8, -8, 20, -32
	delay 15
	createsprite gIcicleSpearSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, 8, 8, 20, -40
	delay 4
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -8, -8, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	delay 9
	createsprite gIcicleSpearSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, 0, 0, 20, -32
	delay 4
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 8, 8, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	delay 14
	playsewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

gMoveAnim_IRON_DEFENSE::
	loopsewithpan SE_SHINY, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 8, 2, RGB_WHITE, 14, RGB_WHITE, 0
	end

gMoveAnim_BLOCK::
	loadspritegfx ANIM_TAG_X_SIGN
	createsprite gBlockXSpriteTemplate, ANIM_TARGET, 66
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	end

gMoveAnim_HOWL::
	loadspritegfx ANIM_TAG_NOISE_LINE
	createvisualtask AnimTask_DeepInhale, 2, ANIM_ATTACKER
	delay 12
	call RoarEffect
	createvisualtask SoundTask_PlayCryHighPitch, 2, ANIM_ATTACKER
	waitforvisualfinish
	delay 30
	end

@ Credits: Skeli
gMoveAnim_DRAGON_CLAW::
	loadspritegfx ANIM_TAG_FOCUS_ENERGY
	loadspritegfx ANIM_TAG_CLAW_SLASH
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 42, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 8, RGB_RED
	call EndureEffect
	delay 8
	call EndureEffect
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 8, RGB_RED
	delay 4
	call EndureEffect
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 2
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 18, 1
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, -10, -10, 0
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, -10, 10, 0
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, -4, 1, 10, 3, ANIM_TARGET
	delay 10
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 6, 4
	delay 2
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, 10, -10, 1
	createsprite gClawSlashSpriteTemplate, ANIM_TARGET, 2, 10, 10, 1
	createsprite gShakeMonOrTerrainSpriteTemplate, ANIM_ATTACKER, 2, -4, 1, 10, 3, ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_FRENZY_PLANT::
	loadspritegfx ANIM_TAG_ROOTS
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	fadetobg BG_LEAF_STORM
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, 61440, 0, FALSE
	waitbgfadein
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 10, 8, 2, 0, 0, 100
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ATTACKER
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, -2, 0, 1, 95
	playsewithpan SE_M_SCRATCH, -43
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 30, 8, -4, 0, 0, 90
	playsewithpan SE_M_SCRATCH, -22
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 40, -8, 4, 0, 1, 85
	playsewithpan SE_M_SCRATCH, SOUND_PAN_ABOVE
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 50, 8, 0, 0, 0, 85
	playsewithpan SE_M_SCRATCH, 21
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 60, -8, -2, 0, 1, 85
	playsewithpan SE_M_SCRATCH, 42
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 75, 8, 0, 0, 0, 85
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 85, 16, 6, 0, 3, 80
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	delay 5
	createsprite gFrenzyPlantRootSpriteTemplate, ANIM_ATTACKER, 2, 85, -16, -6, 0, 2, 75
	playsewithpan SE_M_SCRATCH, SOUND_PAN_TARGET
	delay 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, -10, ANIM_TARGET, 3
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 8, 0, 20, 1
	delay 3
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, 8, ANIM_TARGET, 3
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	delay 3
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 10, -3, ANIM_TARGET, 2
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	delay 3
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -3, 1, ANIM_TARGET, 2
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -10, 1, ANIM_TARGET, 1
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 10, ANIM_TARGET, 1
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	call UnsetScrollingBg
	end

gMoveAnim_BULK_UP::
	loadspritegfx ANIM_TAG_BREATH
	createvisualtask AnimTask_GrowAndShrink, 2
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createsprite gBreathPuffSpriteTemplate, ANIM_ATTACKER, 2
	loopsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER, 4, 2
	end

gMoveAnim_BOUNCE::
	loadspritegfx ANIM_TAG_ROUND_SHADOW
	loadspritegfx ANIM_TAG_IMPACT
	choosetwoturnanim BounceSetUp, BounceUnleash
BounceEnd::
	end

BounceSetUp::
	playsewithpan SE_M_TELEPORT, SOUND_PAN_ATTACKER
	createsprite gBounceBallShrinkSpriteTemplate, ANIM_ATTACKER, 2, 0, 0
	goto BounceEnd

BounceUnleash::
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	createsprite gBounceBallLandSpriteTemplate, ANIM_TARGET, 3
	delay 7
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 11, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	goto BounceEnd

@ Credits: Blackuser
gMoveAnim_MUD_SHOT::
	loadspritegfx ANIM_TAG_WATER_ORB
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 2
	createvisualtask AnimTask_BlendParticle 5, ANIM_TAG_WATER_ORB, 0, 12, 12, RGB(25, 8, 0) @ Brown color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 46, 1
	delay 6
	createvisualtask AnimTask_StartSinAnimTimer, 5, 100
	panse_1B SE_M_WHIRLPOOL, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 1, 0
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 3, 0, 43, 1
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	call MudShotOrbs
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

MudShotOrbs::
	createsprite gMudShotOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, 10, 16
	delay 2
	createsprite gMudShotOrbSpriteTemplate, ANIM_ATTACKER, 3, 10, 10, 16
	delay 2
	return

gMoveAnim_POISON_TAIL::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, TRUE, TRUE, RGB(22, 27, 8) @ Purple color
	waitforvisualfinish
	monbg ANIM_TARGET
	setalpha 12, 8
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 4
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_ATTACKER, TRUE
	clearmonbg ANIM_TARGET
	blendoff
	call PoisonBubblesEffect
	end

gMoveAnim_COVET::
	loadspritegfx ANIM_TAG_MAGENTA_HEART
	createvisualtask AnimTask_RockMonBackAndForth, 5, ANIM_ATTACKER, 2, 0
	call InfatuationHearts
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	loopsewithpan SE_M_DIZZY_PUNCH, SOUND_PAN_TARGET, 4, 3
	end

gMoveAnim_VOLT_TACKLE::
	loadspritegfx ANIM_TAG_SPARK
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_ELECTRICITY
	monbg ANIM_ATTACKER
	setalpha 12, 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 8, RGB_BLACK
	waitforvisualfinish
	createsprite gVoltTackleOrbSlideSpriteTemplate, ANIM_ATTACKER, 1
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	delay 8
	createvisualtask AnimTask_VoltTackleBolt, 5, 0
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_VoltTackleBolt, 5, 1
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_VoltTackleBolt, 5, 2
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_VoltTackleBolt, 5, 3
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_VoltTackleBolt, 5, 4
	playsewithpan SE_M_THUNDERBOLT, SOUND_PAN_ATTACKER
	delay 8
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 10, 0, 18, 1
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gElectricPuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 16, 16
	delay 2
	createsprite gElectricPuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -16, -16
	delay 8
	createvisualtask AnimTask_VoltTackleAttackerReappear, 5
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 3, 0, 9, 1
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	createsprite gElectricPuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 16, 16
	delay 2
	createsprite gElectricPuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -16, -16
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 8, 0, RGB_BLACK
	end

gMoveAnim_MAGICAL_LEAF::
	loadspritegfx ANIM_TAG_LEAF
	loadspritegfx ANIM_TAG_RAZOR_LEAF
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	delay 1
	loopsewithpan SE_M_POISON_POWDER, SOUND_PAN_ATTACKER, 10, 5
	createvisualtask AnimTask_CycleMagicalLeafPal, 5
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -3, -2, 10
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -1, -1, 15
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -4, -4, 7
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 3, -3, 11
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -1, -6, 8
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 2, -1, 12
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -3, -4, 13
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 4, -5, 7
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, 2, -6, 11
	delay 2
	createsprite gRazorLeafParticleSpriteTemplate, ANIM_ATTACKER, 2, -3, -5, 8
	delay 60
	playsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER
	createsprite gRazorLeafCutterSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 32, 20, FALSE
	createsprite gRazorLeafCutterSpriteTemplate, ANIM_TARGET, 3, 20, -10, 20, 0, 32, -20, FALSE
	delay 30
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, -10, -4, ANIM_TARGET, 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 10, 4, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	delay 20
	setargret -1 @ Signal to end palette cycle effect
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

gMoveAnim_WATER_SPORT::
	loadspritegfx ANIM_TAG_GLOWY_BLUE_ORB
	createvisualtask AnimTask_WaterSport, 5
	delay 8
	playsewithpan SE_M_SURF, SOUND_PAN_ATTACKER
	delay 44
	playsewithpan SE_M_SURF, SOUND_PAN_ATTACKER
	delay 44
	playsewithpan SE_M_SURF, SOUND_PAN_ATTACKER
	delay 44
	panse_1B SE_M_SURF, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	end

gMoveAnim_CALM_MIND::
	loadspritegfx ANIM_TAG_THIN_RING
	monbg ANIM_ATK_SIDE
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_EXCEPT_ATTACKER, 0, 0, 16, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_SetAllNonAttackersInvisiblity, 5, TRUE
	createsprite gThinRingShrinkingSpriteTemplate, ANIM_ATTACKER, 40, 0, 0, ANIM_ATTACKER, TRUE
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	delay 14
	createsprite gThinRingShrinkingSpriteTemplate, ANIM_ATTACKER, 40, 0, 0, ANIM_ATTACKER, TRUE
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	delay 14
	createsprite gThinRingShrinkingSpriteTemplate, ANIM_ATTACKER, 40, 0, 0, ANIM_ATTACKER, TRUE
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_SetAllNonAttackersInvisiblity, 5, FALSE
	visible ANIM_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_BlendExcept, 5, F_PAL_EXCEPT_ATTACKER, 0, 16, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	end

gMoveAnim_LEAF_BLADE::
	loadspritegfx ANIM_TAG_LEAF
	loadspritegfx ANIM_TAG_CROSS_IMPACT
	createvisualtask AnimTask_LeafBlade, 5
	delay 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	delay 50
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	delay 50
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 8, 1
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	waitforvisualfinish
	monbg ANIM_TARGET
	setalpha 12, 8
	delay 12
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 8, 0, 18, 1
	createsprite gCrossImpactSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 36
	playsewithpan SE_M_LEER, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_DRAGON_DANCE::
	loadspritegfx ANIM_TAG_PINK_CLOUD
	loadspritegfx ANIM_TAG_HANDS_AND_FEET @ Black color
	monbg ANIM_ATTACKER
	splitbgprio ANIM_ATTACKER
	setalpha 12, 8
	loopsewithpan SE_M_SACRED_FIRE2, SOUND_PAN_ATTACKER, 32, 2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 4, 0, 10, RGB(27, 0, 0) @ Red color
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 2, 0, 56, 1
	call DragonDanceBlackSmoke
	call DragonDanceBlackSmoke
	call DragonDanceBlackSmoke
	call DragonDanceBlackSmoke
	delay 6
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 4, 10, 0, RGB(27, 0, 0) @ Red color
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

DragonDanceBlackSmoke::
	createsprite gDragonDanceBlackSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 28, 528, 30, 13, 50, ANIM_ATTACKER
	delay 2
	createsprite gDragonDanceBlackSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 480, 20, 16, -46, ANIM_ATTACKER
	delay 2
	createsprite gDragonDanceBlackSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 33, 576, 20, 8, 42, ANIM_ATTACKER
	delay 2
	createsprite gDragonDanceBlackSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 31, 400, 25, 11, -42, ANIM_ATTACKER
	delay 2
	createsprite gDragonDanceBlackSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 28, 512, 25, 16, 46, ANIM_ATTACKER
	delay 2
	createsprite gDragonDanceBlackSmokeSpriteTemplate, ANIM_ATTACKER, 2, 0, 33, 464, 30, 15, -50, ANIM_ATTACKER
	delay 2
	return

gMoveAnim_ROCK_BLAST::
	loadspritegfx ANIM_TAG_ROCKS
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 3
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	createsprite gRockBlastRockSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 25, 257
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	createsprite gRockFragmentSpriteTemplate, ANIM_TARGET, 2, 0, 0, 20, 24, 14, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 5, 1
	createsprite gRockFragmentSpriteTemplate, ANIM_TARGET, 2, 5, 0, -20, 24, 14, 1
	createsprite gRockFragmentSpriteTemplate, ANIM_TARGET, 2, 0, 5, 20, -24, 14, 2
	createsprite gRockFragmentSpriteTemplate, ANIM_TARGET, 2, -5, 0, -20, -24, 14, 2
	end

gMoveAnim_SHOCK_WAVE::
	loadspritegfx ANIM_TAG_ELECTRIC_ORBS
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_SPARK
	loadspritegfx ANIM_TAG_LIGHTNING
	monbg ANIM_ATTACKER
	setalpha 12, 8
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 2, 0, 4, RGB_BLACK
	waitforvisualfinish
	createvisualtask AnimTask_ElectricChargingParticles, 2, ANIM_ATTACKER, 20, 0, 2
	playsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER
	delay 12
	createsprite gGrowingChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 2, 0
	delay 30
	createvisualtask AnimTask_ShockWaveProgressingBolt, 5
	delay 12
	waitforvisualfinish
	createvisualtask AnimTask_ShockWaveLightning, 5
	playsewithpan SE_M_TRI_ATTACK2, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 6, 18, 1
	createvisualtask AnimTask_BlendSelected, 5, F_PAL_BG, 3, 16, 0, RGB_WHITE
	createvisualtask AnimTask_BlendSelected, 5, F_PAL_TARGET, 0, 16, 16, RGB_BLACK
	delay 4
	createvisualtask AnimTask_BlendSelected, 5, F_PAL_TARGET, 0, 0, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	blendoff
	end

gMoveAnim_WATER_PULSE::
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	loadspritegfx ANIM_TAG_BLUE_RING_2
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_ATTACKER
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 0, 0, 7, RGB(0, 25, 28) @ Cyan color
	delay 10
	createsprite gWaterPulseBubbleSpriteTemplate, ANIM_ATTACKER, 66, 100, 100, 8, 1, 20, 40, 0
	createsprite gWaterPulseBubbleSpriteTemplate, ANIM_ATTACKER, 66, 20, 100, 16, 2, 10, 35, 1
	createsprite gWaterPulseBubbleSpriteTemplate, ANIM_ATTACKER, 66, 200, 80, 8, 1, 40, 20, 0
	createsprite gWaterPulseBubbleSpriteTemplate, ANIM_ATTACKER, 66, 80, 60, 10, 3, 20, 50, 0
	createsprite gWaterPulseBubbleSpriteTemplate, ANIM_ATTACKER, 66, 140, 100, 16, 1, 20, 30, 1
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	waitforvisualfinish
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_ATTACKER
	createsprite gWaterPulseRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 40, 15
	delay 5
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_ATTACKER
	createsprite gWaterPulseRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 40, 15
	delay 5
	playsewithpan SE_M_GIGA_DRAIN, SOUND_PAN_ATTACKER
	createsprite gWaterPulseRingSpriteTemplate, ANIM_TARGET, 2, 0, 0, 40, 15
	delay 13
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 8, 18, 1
	waitforvisualfinish
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 7, 0, RGB(0, 25, 28) @ Cyan color
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

gMoveAnim_DOOM_DESIRE::
	monbg ANIM_ATK_SIDE
	setalpha 8, 8
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_TARGET, FALSE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 0, 4, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -4, -4, 15, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	waitforvisualfinish
	delay 20
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_TARGET, TRUE
	createsprite gSimplePaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG, 1, 4, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

gMoveAnim_PSYCHO_BOOST::
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	monbg ANIM_ATK_SIDE
	call LusterPurgeLightBgEffect
	delay 6
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG, 2, 8, 0, 10, RGB_BLACK
	splitbgprio ANIM_ATTACKER
	setalpha 8, 8
	delay 10
	createvisualtask AnimTask_ShakeMon, 2, ANIM_ATTACKER, 3, 0, 240, 0
	loopsewithpan SE_M_PSYBEAM2, SOUND_PAN_ATTACKER, 14, 10
	createsprite gPsychoBoostOrbSpriteTemplate, ANIM_ATTACKER, 2
	delay 110
	loopsewithpan SE_M_PSYBEAM2, SOUND_PAN_ATTACKER, 7, 10
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, -8, 1, 24, 1
	playsewithpan SE_M_LEER, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	call UnsetScrollingBg
	end

@@@@@@@@@@@@@@@@@@@@@@@ GEN IV @@@@@@@@@@@@@@@@@@@@@@@

@ Credits: Skeli and Blackuser
gMoveAnim_ROOST::
	loadspritegfx ANIM_TAG_WHITE_FEATHER
	loadspritegfx ANIM_TAG_BLUE_STAR
	monbg ANIM_ATTACKER
	monbgprio_29
	playsewithpan SE_M_PETAL_DANCE, SOUND_PAN_ATTACKER
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 64, 2, 104, 11304, 32, ANIM_ATTACKER
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 32, 2, 104, 11304, 32, ANIM_ATTACKER
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 0, 2, 104, 11304, 32, ANIM_ATTACKER
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 224, 2, 104, 11304, 32, ANIM_ATTACKER
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 128, 2, 104, 11304, 32, ANIM_ATTACKER
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 192, 2, 104, 11304, 32, ANIM_ATTACKER
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 160, 2, 104, 11304, 32, ANIM_ATTACKER
	delay 6
	createsprite gFallingFeatherSpriteTemplate, ANIM_ATTACKER, 0, 0, -16, 96, 2, 104, 11304, 32, ANIM_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	call HealingEffect
	end

@ Credits: Lixdel and Blackuser
gMoveAnim_GRAVITY::
	loadspritegfx ANIM_TAG_SHADOW_BALL
	fadetobg BG_COSMIC
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 2, 0, -1024, FALSE
	waitbgfadein
	playsewithpan SE_M_NIGHTMARE, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ShakeAndSinkMon, 5, MAX_BATTLERS_COUNT, 2, 0, 96, 30
	createvisualtask AnimTask_ShakeAndSinkMon, 5, MAX_BATTLERS_COUNT + 1, 2, 0, 96, 30
	createvisualtask AnimTask_ShakeAndSinkMon, 5, MAX_BATTLERS_COUNT + 2, 2, 0, 96, 30
	createvisualtask AnimTask_ShakeAndSinkMon, 5, MAX_BATTLERS_COUNT + 3, 2, 0, 96, 30
	createsprite gGravityBlackHoleSpriteTemplate, ANIM_ATTACKER, 1, ANIM_ATTACKER, 0, 0, 69, 10, FALSE, 0
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, MAX_BATTLERS_COUNT, 0, 4
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, MAX_BATTLERS_COUNT + 1, 0, 4
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, MAX_BATTLERS_COUNT + 2, 0, 4
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, MAX_BATTLERS_COUNT + 3, 0, 4
	waitforvisualfinish
	call UnsetScrollingBg
	end

@ Credits: Blackuser
gMoveAnim_MIRACLE_EYE::
	loadspritegfx ANIM_TAG_TEAL_ALERT
	loadspritegfx ANIM_TAG_OPENING_EYE
	loadspritegfx ANIM_TAG_ROUND_WHITE_HALO
	monbg ANIM_ATK_SIDE
	playsewithpan SE_M_CONFUSE_RAY, SOUND_PAN_ATTACKER
	createsprite gOpeningEyeSpriteTemplate, ANIM_ATTACKER, 5, 0, 0, ANIM_ATTACKER, TRUE
	createsprite gWhiteHaloSpriteTemplate, ANIM_ATTACKER, 5, 0, 0, ANIM_ATTACKER
	delay 40
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG, 1, 2, 0, 10, RGB_BLACK
	call MiracleEyeEyeSpikeEffect
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	end

MiracleEyeEyeSpikeEffect::
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 70, 0, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 40, 40, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 10, -60, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -50, -40, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -40, 40, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 50, -50, 6, ANIM_ATTACKER
	delay 2
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 50, -30, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 60, 10, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 0, 60, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 0, -40, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -60, 20, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -60, -30, 6, ANIM_ATTACKER
	delay 2
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -50, 50, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -60, 20, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, -40, -40, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 20, -60, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 50, -50, 6, ANIM_ATTACKER
	createsprite gTealAlertSpriteTemplate, ANIM_ATTACKER, 4, 35, 40, 6, ANIM_ATTACKER
	delay 2
	return

@ Credits: Skeli
gMoveAnim_WAKE_UP_SLAP::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_QUICK_GUARD
	loadspritegfx ANIM_TAG_PAIN_SPLIT
	loadspritegfx ANIM_TAG_DUCK @ White color
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createsprite gDoubleSlapSpriteTemplate, ANIM_TARGET, 3, -64, 8, 2, 1
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 4, 0, 4, 16
	call WakeUpSlapHit
	call WakeUpSlapHit
	call WakeUpSlapHit
	call WakeUpSlapHit
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

WakeUpSlapHit::
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 1, 0, 0, ANIM_TARGET, 1
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 2, -16, -8, 160, -32
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 2, -16, -8, -256, -40
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 2, -16, -8, 416, -38
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 2, -16, -8, -384, -31
	delay 14
	return

@ Credits: Skeli and Blackuser
gMoveAnim_HAMMER_ARM::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET
	createsprite gStompFootSpriteTemplate, ANIM_TARGET, 4, 0, -32, 15, 0, 0, 1
	delay 19
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createvisualtask AnimTask_SquishTarget, 2
	end

@ Credits: Ledxil
gMoveAnim_GYRO_BALL::
	loadspritegfx ANIM_TAG_WISP_ORB
	loadspritegfx ANIM_TAG_YELLOW_BALL
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_GUST @ Silver color
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 0
	delay 2
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 4
	delay 2
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 8
	delay 2
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 12
	delay 2
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 16
	delay 2
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 20
	delay 2
	createsprite gGyroBallSteelBallSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 24
	waitforvisualfinish
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, 0, FALSE, 6
	delay 4
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	createsprite gGyroBallCircleHitSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	createsprite gSilverThinRingSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, TRUE, RGB_RED, 8
	delay 2
	createsprite gSilverThinRingSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, TRUE, RGB_RED, 8
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 9
	delay 2
	createsprite gSilverThinRingSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, TRUE, RGB_RED, 8
	end

@ Credits: Blackuser
gMoveAnim_HEALING_WISH::
	loadspritegfx ANIM_TAG_GREEN_SPARKLE
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 16, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_MOONLIGHT, SOUND_PAN_ATTACKER
	createsprite gHealingWishSparkleSpriteTemplate, ANIM_ATTACKER, 40, TRUE
	waitforvisualfinish
	delay 60
	waitsound
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 16, 0, RGB_BLACK
	end

@ Credits: Skeli and Blackuser
gMoveAnim_BRINE::
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	monbg ANIM_TARGET
	panse_1B SE_M_HYDRO_PUMP, SOUND_PAN_TARGET, SOUND_PAN_TARGET, 2, 0
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, -5, 0, -5, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, 5, 0, 6, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, 19, 0, 10, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, -23, 0, -10, FALSE
	delay 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 5, 50, 1
	call BrineWaterDropFall
	call BrineWaterDropFall
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

BrineWaterDropFall::
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, 28, 0, 10, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, -10, 0, -5, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, 10, 0, 6, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, 24, 0, 10, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, -32, 0, -10, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, FALSE
	delay 2
	createsprite gFallingWaterDropSpriteTemplate, ANIM_TARGET, 2, 30, 0, 10, FALSE
	delay 2
	return

@ Credits: Skeli and Blackuser
gMoveAnim_NATURAL_GIFT::
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_SPARKLE_2
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SPARKLE_2, 0, 4, 4, RGB_GREEN
	waitforvisualfinish
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_ATTACKER
	call GrantingStarsEffect
	waitforvisualfinish
	call NaturalGiftGreenRing
	call NaturalGiftGreenRing
	call NaturalGiftGreenRing
	end

NaturalGiftGreenRing::
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 16, 0, 0, ANIM_ATTACKER, FALSE, RGB_GREEN, 6
	delay 10
	return

@ Credits: Skeli
gMoveAnim_FEINT::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	monbg ANIM_TARGET
	setalpha 12, 8
	loopsewithpan SE_M_JUMP_KICK, SOUND_PAN_TARGET, 10, 2
	createsprite gLeftRightMovingFistSpriteTemplate, ANIM_TARGET, 0, 50, 6
	waitforvisualfinish
	createsprite gMachPunchFistSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 0, 0, 20, 0, TRUE, 1
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_PLUCK::
	loadspritegfx ANIM_TAG_IMPACT
AnimPluckBite::
	loadspritegfx ANIM_TAG_SMALL_ROCK
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 2, 0, 28, 1
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 1, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 2, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 3, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 1, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 2, 0
	call PluckRandomBite
	createsprite gDropRockSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 3, 0
	end

PluckRandomBite::
	createsprite gRandomPosHitSplatSpriteTemplate, ANIM_TARGET, 3, ANIM_TARGET, 2
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	delay 4
	return

@ Credits: Skeli and Blackuser
gMoveAnim_TAILWIND::
	createvisualtask AnimTask_LoadSandstormBackground, 5, TRUE
	createvisualtask AnimTask_BlendBackground, 6, 6, RGB_WHITE
	playsewithpan SE_M_GUST, SOUND_PAN_ABOVE
	delay 68
	playsewithpan SE_M_GUST2, SOUND_PAN_ABOVE
	delay 56
	end

@ Credits: Skeli and Blackuser
gMoveAnim_ACUPRESSURE::
	loadspritegfx ANIM_TAG_ACUPRESSURE_FINGER
	loadspritegfx ANIM_TAG_SPARK_2
	monbg ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_M_METRONOME, SOUND_PAN_ATTACKER
	createsprite gAcupressureFingerSpriteTemplate, ANIM_ATTACKER, 3, 0, -32, 15, 0, 0, 0
	delay 19
	call ElectricityEffect
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: ghoulslash and Blackuser
gMoveAnim_METAL_BURST::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SPIKES
	loadspritegfx ANIM_TAG_SPARKLE_2
	createvisualtask AnimTask_GrayscaleParticle, 5, ANIM_TAG_IMPACT
	createvisualtask AnimTask_GrayscaleParticle, 5, ANIM_TAG_SPARKLE_2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 12, RGB_BLACK
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	call GrantingStarsEffect
	waitforvisualfinish
	delay 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, 0, 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 0, 1280, 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 0, -1280, 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, 768, 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, 768, 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, 1280, -768, 3
	createsprite gMetalBurstSpikeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, -1280, -768, 3
	monbg ANIM_TARGET
	loopsewithpan SE_M_DIG, SOUND_PAN_ATTACKER, 7, 3
	createvisualtask AnimTask_FlailMovement, 2, ANIM_ATTACKER
	waitforvisualfinish
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, TRUE, 1, 20, TRUE, FALSE
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	call LusterPurgeRandomHit
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 12, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_U_TURN::
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_RAZOR_LEAF @ Green color
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	invisible ANIM_ATTACKER
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	createsprite gUTurnBallInSpriteTemplate, ANIM_TARGET, 2, 0, 0, 21
	waitforvisualfinish
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	createsprite gUTurnBallOutSpriteTemplate, ANIM_ATTACKER, 3, 0, -15, -16, 36
	waitforvisualfinish
	visible ANIM_ATTACKER
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_CLOSE_COMBAT::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_RED_CIRCLE_OF_LIGHT @ Red color
	monbg ANIM_TARGET
	setalpha 12, 8
	call SetHighSpeedBackground
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 42, 1
	call CloseCombatBeatDown
	call CloseCombatBeatDown
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	call UnsetScrollingBg
	end

CloseCombatBeatDown::
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gRedFistFootSpriteTemplate, ANIM_ATTACKER, 3, 21, 0, 8, ANIM_TARGET, 0
	delay 4
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 21, 0, ANIM_TARGET, 0
	createsprite gRedFistFootSpriteTemplate, ANIM_ATTACKER, 3, 0, 21, 8, ANIM_TARGET, 0
	delay 4
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 21, ANIM_TARGET, 0
	createsprite gRedFistFootSpriteTemplate, ANIM_ATTACKER, 3, -6, -16, 8, ANIM_TARGET, 0
	delay 4
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -6, -16, ANIM_TARGET, 0
	createsprite gRedFistFootSpriteTemplate, ANIM_ATTACKER, 3, -16, 16, 8, ANIM_TARGET, 0
	delay 4
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -16, 16, ANIM_TARGET, 0
	createsprite gRedFistFootSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 8, ANIM_TARGET, 0
	delay 4
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 0
	return

@ Credits: Skeli
gMoveAnim_PAYBACK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_POISON_JAB @ Purple color
	loadspritegfx ANIM_TAG_POISON_BUBBLE @ Purple color
	monbg ANIM_TARGET
	setalpha 12, 8
	loopsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER, 13, 3
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 6, 0, 11, RGB(9, 7, 11) @ Black color
	call PaybackPowerAbsorption
	call PaybackPowerAbsorption
	call PaybackPowerAbsorption
	waitforvisualfinish
	createsprite gPaybackPurpleHitSpriteTemplate, ANIM_TARGET, 2, -10, -10, FALSE
	call PaybackShakeTarget
	createsprite gPaybackPurpleHitSpriteTemplate, ANIM_TARGET, 2, 10, 20, FALSE
	call PaybackShakeTarget
	createsprite gPaybackPurpleHitSpriteTemplate, ANIM_TARGET, 2, -5, 10, FALSE
	call PaybackShakeTarget
	createsprite gPaybackPurpleHitSpriteTemplate, ANIM_TARGET, 2, 17, -12, FALSE
	call PaybackShakeTarget
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end
	
PaybackPowerAbsorption::
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, -10, 13
	delay 3
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -35, -10, 13
	delay 3
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 15, -40, 13
	delay 3
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -10, -32, 13
	delay 3
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 25, -20, 13
	delay 3
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, -20, 13
	delay 3
	createsprite gPaybackAbsorptionOrbSpriteTemplate, ANIM_ATTACKER, 2, 5, -40, 13
	delay 3
	return

PaybackShakeTarget::
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	delay 4
	return

@ Credits: Skeli
gMoveAnim_ASSURANCE::
	loadspritegfx ANIM_TAG_ASSURANCE_HAND
	monbg ANIM_TARGET
	setalpha 12, 8
	fadetobg BG_DARK
	waitbgfadein
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gAssuranceHandSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 5, FALSE, 1, 6, TRUE, FALSE
	waitforvisualfinish
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gAssuranceHandSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, 0
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 5, FALSE, 1, 6, TRUE, FALSE
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	restorebg
	waitbgfadein
	end

@ Credits: Blackuser
gMoveAnim_EMBARGO::
	loadspritegfx ANIM_TAG_CHAIN_LINK
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_CHAIN_LINK, 0, 4, 4, RGB(30, 0, 31) @ Purple color
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 0, 6, RGB_BLACK
	waitforvisualfinish
	call ChainLinkEffect
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 0, 6, 0, RGB_BLACK
	end

ChainLinkEffect::
	loopsewithpan SE_SHINY, SOUND_PAN_TARGET, 28, 2
	createsprite gChainLinkSpriteTemplate, ANIM_TARGET, 2, 0, 10, FALSE
	delay 4
	createsprite gChainLinkSpriteTemplate, ANIM_TARGET, 2, 0, -2, FALSE
	delay 4
	createsprite gChainLinkSpriteTemplate, ANIM_TARGET, 2, 0, 22, FALSE
	return

@ Credits: Skeli and Blackuser
gMoveAnim_FLING::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	playsewithpan SE_FALL, SOUND_PAN_TARGET
	createsprite gFlingItemSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, -8, -8, 20, -32
	delay 19
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 3, -8, -8, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 2, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_PSYCHO_SHIFT::
	loadspritegfx ANIM_TAG_SPARKLE_4
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 8, 8
	call SetPsychicBackground
	createvisualtask AnimTask_MeditateStretchAttacker, 2
	playsewithpan SE_M_DETECT, SOUND_PAN_ATTACKER
	createsprite gDisableSparkleSpriteTemplate, ANIM_ATTACKER, 13, 20, -20
	waitforvisualfinish
	createvisualtask AnimTask_GrowAndGreyscale, 5, ANIM_TARGET, 80
	loopsewithpan SE_M_BIND, SOUND_PAN_TARGET, 15, 4
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_TRUMP_CARD::
	loadspritegfx ANIM_TAG_CONVERSION
	loadspritegfx ANIM_TAG_CUT
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	setalpha 0, 16
	playsewithpan SE_M_BARRIER, SOUND_PAN_TARGET
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -24, 8, 100
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 8, 8, 110
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, -8, 24, 125
	createsprite gConversion2SpriteTemplate, ANIM_ATTACKER, 2, 24, 24, 135
	createvisualtask AnimTask_Conversion2AlphaBlend, 5
	delay 60
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	call Conversion2Sound
	waitforvisualfinish
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 0
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 10, 1
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Skeli and Blackuser
gMoveAnim_HEAL_BLOCK::
	loadspritegfx ANIM_TAG_SPARKLE_2
	monbg ANIM_TARGET
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SPARKLE_2, 0, 10, 10, RGB_RED
	waitforvisualfinish
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_TARGET
	createsprite gSparklingStarsSpriteTemplate, ANIM_ATTACKER, 16, -15, 0, ANIM_TARGET, 0, 32, 60, TRUE
	delay 8
	createsprite gSparklingStarsSpriteTemplate, ANIM_ATTACKER, 16, 12, -5, ANIM_TARGET, 0, 32, 60, TRUE
	waitsound
	createvisualtask AnimTask_GreyscaleBackgound, 5, FALSE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_TARGET, FALSE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_DEF_PARTNER, FALSE
	playsewithpan SE_M_SWAGGER, SOUND_PAN_TARGET
	delay 40
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_TARGET, TRUE
	createvisualtask AnimTask_SetGreyscaleOrOriginalPal, 5, ANIM_DEF_PARTNER, TRUE
	createvisualtask AnimTask_GreyscaleBackgound, 5, TRUE
	clearmonbg ANIM_TARGET
	end

@ Credits: ghoulslash
gMoveAnim_WRING_OUT::
	loadspritegfx ANIM_TAG_ASSURANCE_HAND
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 10
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET, 20
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 30
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET, 40
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 50
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET, 60
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 70
	waitplaysewithpan SE_M_DOUBLE_TEAM, SOUND_PAN_TARGET, 80
	waitplaysewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET, 90
	createvisualtask AnimTask_TranslateMonElliptical, 2, ANIM_ATTACKER, -18, 6, 6, 4
	createvisualtask AnimTask_TranslateMonElliptical, 2, ANIM_TARGET, 18, 6, 6, 4
	call WringOutHand
	call WringOutHand
	call WringOutHand
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

WringOutHand::
	createsprite gAssuranceHandSpriteTemplate, ANIM_ATTACKER, 3, 0, -12, ANIM_TARGET, 1
	delay 8
	createsprite gAssuranceHandSpriteTemplate, ANIM_ATTACKER, 3, -12, 8, ANIM_TARGET, 1
	delay 8
	createsprite gAssuranceHandSpriteTemplate, ANIM_ATTACKER, 3, 12, 0, ANIM_TARGET, 1
	delay 8
	return

@ Credits: Skeli and Blackuser
gMoveAnim_POWER_TRICK::
	loadspritegfx ANIM_TAG_POWER_TRICK
	monbg ANIM_ATK_SIDE
	splitbgprio ANIM_ATTACKER
	setalpha 16, 2
	loopsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER, 10, 3
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gPowerTrickSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 8, 63, 0, FALSE, 0
	delay 16
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

@ Credits: Skeli
gMoveAnim_GASTRO_ACID::
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	loadspritegfx ANIM_TAG_RAZOR_LEAF @ Green color
	monbg ANIM_DEF_SIDE
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -5, 1, -5, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 5, 0, 6, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 19, 1, 10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -23, 2, -10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 4, 0, 8, RGB(21, 25, 14) @ Green color
	call GreenAcidFall
	call GreenAcidFall
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

GreenAcidFall::
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 28, 1, 10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -10, 1, -5, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 10, 0, 6, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 24, 1, 10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -32, 2, -10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	createsprite gGastroAcidGreenPoisonBubbleSpriteTemplate, ANIM_TARGET, 2, 30, 2, 10, FALSE
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	delay 2
	return

@ Credits: Lixdel
gMoveAnim_LUCKY_CHANT::
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_SPARKLE_2
	loadspritegfx ANIM_TAG_PINK_PETAL @ Pink color
	playsewithpan SE_M_PETAL_DANCE, SOUND_PAN_ATTACKER
	call LuckyChantStars
	call LuckyChantStars
	call LuckyChantStars
	call LuckyChantStars
	call LuckyChantStars
	call LuckyChantStars
	delay 29
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 2, 2, 0, 11, RGB_WHITE
	delay 69
	playsewithpan SE_M_HEAL_BELL, SOUND_PAN_ATTACKER
	createsprite gPinkThinRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	delay 3
	createsprite gPinkThinRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	end

LuckyChantStars::
	createsprite gLuckyChantPinkStarsSpriteTemplate, ANIM_ATTACKER, 2, 0, -24, 8, 140, TRUE
	delay 3
	return

@ Credits: Skeli
gMoveAnim_COPYCAT::
	loadspritegfx ANIM_TAG_ORBS
	monbg ANIM_ATK_SIDE
	setalpha 12, 8
	loopsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER, 13, 3
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 6, 0, 11, RGB_WHITE
	call RecoverAbsorbEffect
	call RecoverAbsorbEffect
	call RecoverAbsorbEffect
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

@ Credits: Skeli and Blackuser
gMoveAnim_POWER_SWAP::
	loadspritegfx ANIM_TAG_BLUEGREEN_ORB
	loadspritegfx ANIM_TAG_RED_HEART @ Red color
	call SetPsychicBackground
	call PowerSwapEffect
	waitforvisualfinish
	call UnsetScrollingBg
	end

PowerSwapEffect::
	createvisualtask AnimTask_SkillSwap, 3, TRUE, 1
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_TARGET, RGB_WHITE, 12, 3, 1
	loopsewithpan SE_M_REVERSAL, SOUND_PAN_ATTACKER, 24, 3
	delay 16
	createvisualtask AnimTask_SkillSwap, 3, FALSE, 1
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB_WHITE, 12, 3, 1
	return

@ Credits: Skeli
gMoveAnim_GUARD_SWAP::
	loadspritegfx ANIM_TAG_BLUEGREEN_ORB
	call SetPsychicBackground
	createvisualtask AnimTask_SkillSwap, 3, TRUE, 2
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_TARGET, RGB_WHITE, 12, 3, 1
	loopsewithpan SE_M_REVERSAL, SOUND_PAN_ATTACKER, 24, 3
	delay 16
	createvisualtask AnimTask_SkillSwap, 3, FALSE, 2
	createvisualtask AnimTask_BlendMonInAndOut, 5, ANIM_ATTACKER, RGB_WHITE, 12, 3, 1
	waitforvisualfinish
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_PUNISHMENT::
	loadspritegfx ANIM_TAG_PUNISHMENT_BLADES
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_POISON_BUBBLE @ Purple color
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 4
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_TARGET
	createsprite gPunishmentBladeSpriteTemplate, ANIM_TARGET, 2, -16, -16
	delay 8
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 5, 1, RGB_WHITE, 10, RGB_BLACK, 0
	createsprite gPunishmentPurpleHitSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 2
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -12, 10, 0, 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	delay 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 0, 3, 6, 1
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	end

@ Credits: ghoulslash
gMoveAnim_LAST_RESORT::
	loadspritegfx ANIM_TAG_THIN_RING
	loadspritegfx ANIM_TAG_SPARKLE_2
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_PAIN_SPLIT @ White color
	loadspritegfx ANIM_TAG_DUCK @ White color
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	createsprite gThinRingExpandingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_TARGET, TRUE
	delay 10
	createsprite gLastResortSparklesSpriteTemplate, ANIM_ATTACKER, 2, -15, 0, ANIM_TARGET, 0, 32, 60
	delay 8
	createsprite gLastResortSparklesSpriteTemplate, ANIM_ATTACKER, 2, 12, -5, ANIM_TARGET, 0, 32, 60
	waitforvisualfinish
	createsprite gThinRingShrinkingSpriteTemplate, ANIM_ATTACKER, 40, 0, 0, ANIM_TARGET, TRUE
	waitforvisualfinish
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 3, -16, -8, 160, -32
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 3, -16, -8, -256, -40
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 3, -16, -8, 128, -16
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 3, -16, -8, 416, -38
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 3, -16, -8, -128, -22
	createsprite gWakeUpSlapStarSpriteTemplate, ANIM_TARGET, 3, -16, -8, -384, -31
	playsewithpan SE_M_MORNING_SUN, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

@ Credits: Skeli and Blackuser
gMoveAnim_WORRY_SEED::
	loadspritegfx ANIM_TAG_SEED
	loadspritegfx ANIM_TAG_ANGER
	call LaunchLeechSeeds
	waitforvisualfinish
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	waitforvisualfinish
	delay 12
	createsprite gAngerMarkSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 20, -28
	playsewithpan SE_M_SWAGGER2, SOUND_PAN_TARGET
	end

@ Credits: Skeli
gMoveAnim_SUCKER_PUNCH::
	loadspritegfx ANIM_TAG_POISON_JAB
	loadspritegfx ANIM_TAG_IMPACT
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 20, 0, 0, 4
	createsprite gSuckerPunchSpriteTemplate, ANIM_TARGET, 2, -24, 5, 40, 8, 160, 0
	delay 4
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 0, 0, ANIM_TARGET, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 9, 1
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 4
	end

@ Credits: Skeli
gMoveAnim_TOXIC_SPIKES::
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	loadspritegfx ANIM_TAG_SPIKES
	loadspritegfx ANIM_TAG_SHADOW_BALL @ Purple color
	monbg ANIM_DEF_SIDE
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 28
	createsprite gToxicSpikesSpriteTemplate, ANIM_TARGET, 2, 20, 0, 0, 24, 30
	delay 10
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	waitplaysewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 28
	createsprite gToxicSpikesSpriteTemplate, ANIM_TARGET, 2, 20, 0, -24, 24, 30
	delay 10
	waitplaysewithpan SE_M_HORN_ATTACK, SOUND_PAN_TARGET, 28
	createsprite gToxicSpikesSpriteTemplate, ANIM_TARGET, 2, 20, 0, 24, 24, 30
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_DEF_SIDE, 0, 4, 0, 12, RGB(26, 0, 26) @ Purple color
	call PoisonBubblesEffect
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

@ Credits: WesleyFG and Blackuser
gMoveAnim_HEART_SWAP::
	loadspritegfx ANIM_TAG_BLUEGREEN_ORB
	loadspritegfx ANIM_TAG_RED_HEART @ Red color
	createvisualtask AnimTask_HeartsBackground, 5
	delay 75
	call PowerSwapEffect
	end

@ Credits: Lixdel
gMoveAnim_AQUA_RING::
	loadspritegfx ANIM_TAG_GUARD_RING
	loadspritegfx ANIM_TAG_SMALL_BUBBLES
	loadspritegfx ANIM_TAG_WATER_ORB @ Blue color
	monbg ANIM_ATK_SIDE
	setalpha 8, 8
	call AquaRingRings
	waitforvisualfinish
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 0, 2, 0, 10, RGB(14, 27, 31) @ Blue color
	waitforvisualfinish
	clearmonbg ANIM_ATK_SIDE
	blendoff
	end

AquaRingRings::
	playsewithpan SE_M_MILK_DRINK, SOUND_PAN_ATTACKER
	createsprite gBlueGuardRingSpriteTemplate, ANIM_ATTACKER, 2, FALSE
	createsprite gAquaRingBubblesSpriteTemplate, ANIM_ATTACKER, 2, 0, -10, 25, ANIM_ATTACKER
	delay 4
	createsprite gBlueGuardRingSpriteTemplate, ANIM_ATTACKER, 2, FALSE
	createsprite gAquaRingBubblesSpriteTemplate, ANIM_ATTACKER, 2, -15, 0, 25, ANIM_ATTACKER
	delay 4
	createsprite gBlueGuardRingSpriteTemplate, ANIM_ATTACKER, 2, FALSE
	createsprite gAquaRingBubblesSpriteTemplate, ANIM_ATTACKER, 2, 20, 10, 25, ANIM_ATTACKER
	return

@ Credits: WesleyFG
gMoveAnim_MAGNET_RISE::
	loadspritegfx ANIM_TAG_SPARK_2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 5, 5, RGB(31, 31, 22) @ Yellow color
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_WindUpLunge, 5, ANIM_ATTACKER, -12, 4, 10, 10, 12, 6
	call SparkElectricity1
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_ATTACKER
	delay 4
	call SparkElectricity2
	call SparkElectricity1
	call SparkElectricity2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 0, 0, RGB(31, 31, 22) @ Yellow color
	delay 20
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 7, 7, RGB(31, 31, 22) @ Yellow color
	waitforvisualfinish
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_BG | F_PAL_ATTACKER, -31, 1, 0, 0, RGB(31, 31, 22) @ Yellow color
	end

@ Credits: Skeli
gMoveAnim_FLARE_BLITZ::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SMALL_EMBER
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_IMPACT, 0, 8, 8, RGB_RED
	playsewithpan SE_M_DRAGON_RAGE, SOUND_PAN_ATTACKER
	call FlareBlitzFlameBuffEffect
	delay 4
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_ATTACKER, 1, 2, 0, 11, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_ATTACKER, 1, 0, 21, 1
	call FlareBlitzFlameBuffEffect
	delay 4
	call FlareBlitzFlameBuffEffect
	fadetobg BG_FIRE
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, 4096, 0, FALSE
	waitbgfadein
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	delay 25
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 42, 0, FALSE, 5
	delay 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 32, 1
	loopsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET, 2, 5
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 0, 21, 0, ANIM_TARGET, 0
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 1, 0, 21, ANIM_TARGET, 0
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -6, -16, ANIM_TARGET, 0
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 3, -16, 16, ANIM_TARGET, 0
	delay 2
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, 0, ANIM_TARGET, 0
	delay 2
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	call FireSpreadEffectLong
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	call UnsetScrollingBg
	end

FlareBlitzFlameBuffEffect::
	createsprite gFlameBuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, 26, 2, 22
	delay 4
	createsprite gFlameBuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 14, 28, 1, 22
	delay 4
	createsprite gFlameBuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -5, 10, 2, 22
	delay 4
	createsprite gFlameBuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 28, 26, 3, 22
	delay 4
	createsprite gFlameBuffSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -12, 0, 1, 22
	return

@ Credits: Skeli and Blackuser
gMoveAnim_FORCE_PALM::
	loadspritegfx ANIM_TAG_FORCE_PALM
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SHADOW_BALL @ Purple color
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createsprite gForcePalmHandSpriteTemplate, ANIM_TARGET, 2, 0, 0, 40, 0, 8, 32
	delay 32
	createsprite gPurpleHitSplatSpriteTemplate, ANIM_ATTACKER, 2, -11, 0, ANIM_TARGET, 2
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 4, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_AURA_SPHERE::
	loadspritegfx ANIM_TAG_AURA_SPHERE
	loadspritegfx ANIM_TAG_IMPACT_2
	fadetobg BG_AURA_SPHERE
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, 63232, 0, TRUE
	waitbgfadein
	playsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER
	createsprite gAuraSphereBallSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 16, 53, 0, FALSE, 0
	delay 48
	panse_1B SE_M_SWAGGER, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 2, 0
	createsprite gShotHitBallSpriteTemplate, ANIM_TARGET, 2, 0, 0, 16
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 8, 1
	waitforvisualfinish
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_ROCK_POLISH::
	loadspritegfx ANIM_TAG_CUT
	loadspritegfx ANIM_TAG_SPARKLE_2
	monbg ANIM_ATTACKER
	setalpha 12, 8
	playsewithpan SE_M_CUT, SOUND_PAN_ATTACKER
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 0
	delay 5
	playsewithpan SE_M_CUT, SOUND_PAN_ATTACKER
	createsprite gCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 1, 0
	waitforvisualfinish
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	call GrantingStarsEffect
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	end

@ Credits: Skeli
gMoveAnim_POISON_JAB::
	loadspritegfx ANIM_TAG_POISON_JAB
	loopsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET, 2, 16
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 32, 1
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 0, -32, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 22, -22, 16
	delay 2
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 12, RGB(26, 0, 26) @ Purple color
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 30, 0, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 20, 20, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 0, 28, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, -19, 19, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, -27, 0, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, -18, -18, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 0, -25, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 17, -17, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 23, 0, 16
	delay 2
	createsprite gPoisonJabSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, FALSE, 16, 16, 16
	end

@ Credits: Skeli
gMoveAnim_DARK_PULSE::
	loadspritegfx ANIM_TAG_PURPLE_RING
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 14, RGB_BLACK
	delay 4
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_REFLECT, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 3, 4, 0, 15
	call DarkPulseBeam
	call DarkPulseBeam
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 4, 0, 15, RGB(6, 0, 12) @ Purple color
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 50, 1
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	call DarkPulseBeam
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 9, 0, RGB(6, 0, 12) @ Purple color
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 14, 0, RGB_BLACK
	end

DarkPulseBeam::
	createsprite gDarkPulseRingSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 22, 3
	delay 2
	return

@ Credits: Skeli
gMoveAnim_NIGHT_SLASH::
	loadspritegfx ANIM_TAG_CUT
	monbg ANIM_TARGET
	fadetobg BG_DARK
	waitbgfadein
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_CUT, 0, 13, 13, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	createsprite gNightSlashSliceSpriteTemplate, ANIM_TARGET, 2, 50, -10, 100, 4, TRUE, 0
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 4, 10, 1
	delay 21
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	createsprite gNightSlashSliceSpriteTemplate, ANIM_TARGET, 2, -50, 10, 100, 4, FALSE, 1
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 4, 10, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	restorebg
	waitbgfadein
	end

@ Credits: Skeli
gMoveAnim_AQUA_TAIL::
	loadspritegfx ANIM_TAG_WATER_IMPACT
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	loadspritegfx ANIM_TAG_SLAM_HIT_2
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	createvisualtask AnimTask_ShakeMon, 5, ANIM_ATTACKER, 0, 2, 23, 1
	delay 5
	call SmallWaterBubblesOnAttacker
	waitforvisualfinish
	createsprite gHorizontalLungeSpriteTemplate, ANIM_ATTACKER, 2, 4, 6
	delay 4
	playsewithpan SE_M_VITAL_THROW, SOUND_PAN_TARGET
	createsprite gKnockOffStrikeSpriteTemplate, ANIM_TARGET, 2, -16, -16
	delay 8
	createsprite gComplexPaletteBlendSpriteTemplate, ANIM_ATTACKER, 2, F_PAL_BG | F_PAL_BATTLERS, 5, 1, RGB_WHITE, 10, RGB_BLACK, 0
	createsprite gWaterHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 0
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -12, 10, FALSE, 3
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 5
	delay 3
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 0, 3, 6, 1
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

@ Credits: Skeli
gMoveAnim_SEED_BOMB::
	loadspritegfx ANIM_TAG_EXPLOSION
	loadspritegfx ANIM_TAG_SEED
	monbg ANIM_TARGET
	call SeedBombSeed
	call SeedBombSeed
	call SeedBombSeed
	call SeedBombSeed
	call SeedBombSeed
	delay 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 16, 1
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, FALSE
	delay 3
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, 24, -24, ANIM_TARGET, FALSE
	delay 3
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, -16, 16, ANIM_TARGET, FALSE
	delay 3
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, -24, -12, ANIM_TARGET, FALSE
	delay 3
	playsewithpan SE_M_SELF_DESTRUCT, SOUND_PAN_TARGET
	createsprite gExplosionSpriteTemplate, ANIM_TARGET, 3, 16, 16, ANIM_TARGET, FALSE
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

SeedBombSeed::
	playsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER
	createsprite gSeedBombSeedSpriteTemplate, ANIM_ATTACKER, 2, 20, -8, -8, -8, 20, -32
	delay 3
	return

@ Credits: Skeli and Blackuser
gMoveAnim_AIR_SLASH::
	loadspritegfx ANIM_TAG_PUNISHMENT_BLADES
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	call SetSkyBg
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_PUNISHMENT_BLADES, 0, 12, 12, RGB_WHITE
	waitforvisualfinish
	loopsewithpan SE_M_RAZOR_WIND2, SOUND_PAN_ATTACKER, 5, 3
	call AirSlashBlade
	delay 5
	call AirSlashBlade
	delay 5
	call AirSlashBlade
	delay 9
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 3, 0, 20, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 1, 2, 0, 11, RGB_WHITE
	loopsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET, 5, 3
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	call UnsetScrollingBg
	stopsound
	end

AirSlashBlade::
	createsprite gAirSlashBladeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 24
	return

@ Credits: Skeli
gMoveAnim_X_SCISSOR::
	loadspritegfx ANIM_TAG_CUT
	loadspritegfx ANIM_TAG_JAGGED_MUSIC_NOTE @ Red color
	monbg ANIM_TARGET
	playsewithpan SE_M_CUT, SOUND_PAN_TARGET
	createsprite gRedCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 0, 0
	createsprite gRedCuttingSliceSpriteTemplate, ANIM_ATTACKER, 2, 40, -32, 1, 0
	delay 5
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 10, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_BUG_BUZZ::
	loadspritegfx ANIM_TAG_JAGGED_MUSIC_NOTE
	loadspritegfx ANIM_TAG_THIN_RING
	monbg ANIM_DEF_SIDE
	fadetobg BG_BUG_PLAYER
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -2304, 0, TRUE
	waitbgfadein
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	playsewithpan SE_ELEVATOR, SOUND_PAN_ATTACKER
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 29, -12, 0
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -12, -29, 1
	delay 16
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	playsewithpan SE_ELEVATOR, SOUND_PAN_ATTACKER
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 12, -29, 1
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -29, -12, 0
	delay 16
	createvisualtask AnimTask_UproarDistortion, 2, ANIM_ATTACKER
	createsprite gUproarRingSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, ANIM_ATTACKER, TRUE, RGB_RED, 8
	playsewithpan SE_ELEVATOR, SOUND_PAN_ATTACKER
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 24, -24, 1
	createsprite gJaggedMusicNoteSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, -24, -24, 0
	waitforvisualfinish
	stopsound
	clearmonbg ANIM_DEF_SIDE
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_DRAGON_PULSE::
	loadspritegfx ANIM_TAG_DRAGON_PULSE_RING
	loadspritegfx ANIM_TAG_FLASH_CANNON_BALL
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 4, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_PSYBEAM, SOUND_PAN_ATTACKER
	loopsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET, 11, 9
	call DragonPulseBeam
	call DragonPulseBeam
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 2, 0, 37, 2
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	call DragonPulseBeam
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 4, 0, RGB_BLACK
	end

DragonPulseBeam::
	createsprite gDragonPulseRingSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 22, 0
	delay 1
	createsprite gDragonPulseBallSpriteTemplate, ANIM_TARGET, 2, 16, 0, 0, 0, 22, 0
	delay 1
	return

@ Credits: Skeli
gMoveAnim_DRAGON_RUSH::
	loadspritegfx ANIM_TAG_PURPLE_DRAKE
	loadspritegfx ANIM_TAG_ROCKS
	fadetobg BG_COSMIC
	monbg ANIM_ATTACKER
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -2304, 0, TRUE
	waitbgfadein
	createvisualtask AnimTask_AttackerFadeToInvisible, 5, 0
	waitforvisualfinish
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_SKY_UPPERCUT, SOUND_PAN_ATTACKER
	createsprite gPurpleDrakeSpriteTemplate, ANIM_TARGET, 2
	delay 14
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 11, RGB(15, 0, 26) @ Purple color
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 10, 0, 18, 1
	call IronTailRockScatterNoHit
	delay 20
	createvisualtask AnimTask_AttackerFadeFromInvisible, 5, 1
	delay 2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 0, 15, 0, RGB_WHITE
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_POWER_GEM::
	loadspritegfx ANIM_TAG_PUNISHMENT_BLADES
	loadspritegfx ANIM_TAG_POWER_GEM
	loadspritegfx ANIM_TAG_AIR_WAVE_2 @ Gray color
	monbg ANIM_DEF_SIDE
	monbgprio_2A ANIM_TARGET
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 13, RGB_BLACK
	waitforvisualfinish
	createsprite gPowerGemWheelSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 0
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	delay 2
	createsprite gPowerGemWheelSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 4
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	delay 2
	createsprite gPowerGemWheelSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 8
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	delay 2
	createsprite gPowerGemWheelSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 12
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	delay 2
	createsprite gPowerGemWheelSpriteTemplate, ANIM_ATTACKER, 3, 0, 0, 56, 12
	playsewithpan SE_SHINY, SOUND_PAN_ATTACKER
	delay 48
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_ICY_WIND, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 5, 5, 0, 5
	call PowerGemGemAttack
	call PowerGemGemAttack
	call PowerGemGemAttack
	call PowerGemGemAttack
	call PowerGemGemAttack
	waitforvisualfinish
	loopsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET, 3, 5
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, -64, 176, 40
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, -192, 240, 40
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, -64, -160, 40
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, -192, -112, 40
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, 160, 48, 40
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, -224, -32, 40
	createsprite gPowerGemBladeSpriteTemplate, ANIM_TARGET, 1, 0, 10, 112, -128, 40
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 15, 1
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 13, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	end

PowerGemGemAttack::
	createsprite gPowerGemGemSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 0, 0
	createsprite gPowerGemGemSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 10, 5
	createsprite gPowerGemGemSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, -10, -5
	createsprite gPowerGemGemSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, 20, 10
	createsprite gPowerGemGemSpriteTemplate, ANIM_TARGET, 2, 15, 15, 20, -20, -10
	delay 2
	return

@ Credits: Skeli
gMoveAnim_DRAIN_PUNCH::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_ORBS
	loadspritegfx ANIM_TAG_BLUE_STAR
	loadspritegfx ANIM_TAG_VERTICAL_HEX @ Orange color
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 10, RGB(29, 24, 5) @ Orange color
	createsprite gDrainPunchFistSpriteTemplate, ANIM_TARGET, 1, ANIM_TARGET, 0, 0, 16, 44, TRUE, 0
	delay 2
	playsewithpan SE_M_COMET_PUNCH, SOUND_PAN_TARGET
	delay 6
	playsewithpan SE_M_ABSORB, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 5, 26, 1
	call GigaDrainAbsorbEffect
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 10, 0, RGB(29, 24, 5) @ Orange color
	call HealingEffect
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_VACUUM_WAVE::
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	loadspritegfx ANIM_TAG_BLACK_SMOKE
	loadspritegfx ANIM_TAG_ICE_CHUNK @ White color
	monbg ANIM_TARGET
	setalpha 9, 8
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_TARGET
	createsprite gFistFootSpriteTemplate, ANIM_TARGET, 4, 0, 0, 8, ANIM_TARGET, 0
	createsprite gVacuumWaveWhiteSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -12, 104, 75
	createsprite gVacuumWaveWhiteSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -12, -72, 75
	createsprite gVacuumWaveWhiteSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -6, -56, 75
	createsprite gVacuumWaveWhiteSmokeSpriteTemplate, ANIM_TARGET, 4, 0, -6, 88, 75
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli and Blackuser
gMoveAnim_FOCUS_BLAST::
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_THIN_RING
	monbg ANIM_TARGET
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	fadetobg BG_FOCUS_BLAST
	waitbgfadeout
	createvisualtask AnimTask_StartSlidingBg, 5, -4096, 0, FALSE
	waitbgfadein
	createsprite gSuperpowerOrbSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, FALSE
	loopsewithpan SE_M_MEGA_KICK, SOUND_PAN_ATTACKER, 32, 4
	delay 100
	delay 89
	playsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 16, 1
	createsprite gUproarRingSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, TRUE, RGB_YELLOW, 8
	delay 1
	createsprite gUproarRingSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, TRUE, RGB_YELLOW, 8
	delay 1
	createsprite gUproarRingSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, TRUE, RGB_YELLOW, 8
	waitforvisualfinish
	delay 8
	clearmonbg ANIM_TARGET
	blendoff
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_ENERGY_BALL::
	loadspritegfx ANIM_TAG_ENERGY_BALL
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 13, RGB_BLACK
	waitforvisualfinish
	createsoundtask SoundTask_LoopSEAdjustPanning, SE_M_MIST, SOUND_PAN_ATTACKER, SOUND_PAN_TARGET, 5, 5, 0, 5
	createsprite gEnergyBallSpriteTemplate, ANIM_TARGET, 2, 0, 0, 37
	waitforvisualfinish
	playsewithpan SE_M_TOXIC, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 8, 1
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 1, 2, 0, 11, RGB(1, 31, 7) @ Green color
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 13, 0, RGB_BLACK
	end

@ Credits: Skeli
gMoveAnim_BRAVE_BIRD::
	loadspritegfx ANIM_TAG_BIRD
	loadspritegfx ANIM_TAG_JAGGED_MUSIC_NOTE @ Red color
	monbg ANIM_ATTACKER
	call SetHighSpeedBackground
	createvisualtask AnimTask_AttackerFadeToInvisible, 5, 0
	waitforvisualfinish
	createvisualtask SoundTask_PlaySE2WithPanning, 5, SE_M_SKY_UPPERCUT, SOUND_PAN_ATTACKER
	createsprite gBraveBirdBirdSpriteTemplate, ANIM_TARGET, 2
	delay 14
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 11, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 10, 0, 18, 1
	createvisualtask SoundTask_PlaySE1WithPanning, 5, SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	delay 20
	createvisualtask AnimTask_AttackerFadeFromInvisible, 5, 1
	delay 2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_ATTACKER, 0, 15, 0, RGB_WHITE
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	call UnsetScrollingBg
	end

@ Credits: Skeli
gMoveAnim_EARTH_POWER::
	loadspritegfx ANIM_TAG_FIRE_PLUME
	call SetFissureBackground
	loopsewithpan SE_M_FLAME_WHEEL2, SOUND_PAN_TARGET, 11, 3
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 3, 25, 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, 5, 0
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, -10, -15
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 0, 25
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, 15, 5
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, -25, 0
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, 30, 30
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 2, ANIM_TARGET, -27, 25
	delay 1
	createsprite gEarthPowerFirePlumeSpriteTemplate, ANIM_TARGET, 66, ANIM_TARGET, 0, 8
	waitforvisualfinish
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_TARGET, 66, ANIM_ATTACKER, 0, 4
	waitforvisualfinish
	call UnsetScrollingBg
	end

@ Credits: MrDollSteak
gMoveAnim_SWITCHEROO::
	loadspritegfx ANIM_TAG_ITEM_BAG_2
	loadspritegfx ANIM_TAG_SPEED_DUST
	fadetobg BG_DARK
	waitbgfadein
	createsprite gTrickBagSpriteTemplate, ANIM_ATTACKER, 2, -40, 80
	createsprite gTrickBagSpriteTemplate, ANIM_ATTACKER, 2, -40, 208
	delay 16
	playsewithpan SE_M_SKETCH, SOUND_PAN_ABOVE
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_TARGET
	createvisualtask AnimTask_StretchBattlerUp, 3, ANIM_ATTACKER
	delay 30
	call TrickBagSoundEffect
	delay 8
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	call TrickBagSoundEffect
	playsewithpan SE_M_ATTRACT, SOUND_PAN_ABOVE
	createvisualtask AnimTask_ShakeMon, 3, ANIM_ATTACKER, 5, 0, 7, 2
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 5, 0, 7, 2
	waitforvisualfinish
	restorebg
	waitbgfadein
	end

@ Credits: Skeli
gMoveAnim_GIGA_IMPACT::
	loadspritegfx ANIM_TAG_IMPACT
	monbg ANIM_DEF_SIDE
	setalpha 12, 8
	playsewithpan SE_M_TAKE_DOWN, SOUND_PAN_ATTACKER
	createsprite gVerticalDipSpriteTemplate, ANIM_ATTACKER, 2, 6, 1, ANIM_ATTACKER
	waitforvisualfinish
	setbgbasedonside ANIM_TARGET, BG_GIGA_IMPACT_PLAYER, BG_GIGA_IMPACT_OPPONENT
	delay 11
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 26, 0, FALSE, 5
	delay 6
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 4, -10, 0, ANIM_TARGET, 0
	loopsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET, 10, 2
	delay 1
	createsprite gSlideMonToOffsetSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, -28, 0, FALSE, 3
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_TARGET, 4, 0, 12, 1
	waitforvisualfinish
	delay 10
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 6
	delay 5
	createsprite gSlideMonToOriginalPosSpriteTemplate, ANIM_ATTACKER, 2, ANIM_TARGET, 0, 6
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	restorebg
	waitbgfadein
	end

@ Credits: Skeli
gMoveAnim_NASTY_PLOT::
	loadspritegfx ANIM_TAG_AMNESIA
	fadetobg BG_DARK
	waitbgfadeout
	createvisualtask AnimTask_SetPsychicBackground, 5
	waitbgfadein
	goto AmnesiaAnim

@ Credits: Skeli
gMoveAnim_BULLET_PUNCH::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET
	setalpha 12, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_IMPACT, 2, 14, 14, RGB_BLACK
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_HANDS_AND_FEET, 2, 12, 12, RGB(28, 28, 26) @ Beige color
	playsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	delay 28
	createvisualtask AnimTask_AttackerPunchWithTrace, 2, RGB(24, 23, 24), 14 @ Gray color
	playsewithpan SE_M_JUMP_KICK, SOUND_PAN_ATTACKER
	delay 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 21, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 10, 0, ANIM_TARGET, 1
	delay 1
	createsprite gBulletPunchFistSpriteTemplate, ANIM_TARGET, 3, 10, 0, 8, ANIM_TARGET, 0
	delay 2
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -16, 16, ANIM_TARGET, 1
	delay 1
	createsprite gBulletPunchFistSpriteTemplate, ANIM_TARGET, 3, -16, 16, 8, ANIM_TARGET, 0
	delay 2
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 5, 16, ANIM_TARGET, 1
	delay 1
	createsprite gBulletPunchFistSpriteTemplate, ANIM_TARGET, 3, 5, 16, 8, ANIM_TARGET, 0
	delay 2
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, 5, -11, ANIM_TARGET, 1
	delay 1
	createsprite gBulletPunchFistSpriteTemplate, ANIM_TARGET, 3, 5, -11, 8, ANIM_TARGET, 0
	delay 2
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 2, -11, -9, ANIM_TARGET, 1
	delay 1
	createsprite gBulletPunchFistSpriteTemplate, ANIM_TARGET, 3, -11, -9, 8, ANIM_TARGET, 0
	waitforvisualfinish
	blendoff
	end

@ Credits: Skeli
gMoveAnim_AVALANCHE::
	loadspritegfx ANIM_TAG_AVALANCHE_ROCKS
	monbg ANIM_DEF_SIDE
	fadetobg BG_ICE
	waitbgfadein
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, -5, 1, -5, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, 5, 0, 6, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, 19, 1, 10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, -23, 2, -10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 5, 50, 1
	delay 2
	call AvalancheRocks
	call AvalancheRocks
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	restorebg
	waitbgfadein
	end

AvalancheRocks::
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, 28, 1, 10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, -10, 1, -5, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, 10, 0, 6, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, 24, 1, 10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, -32, 2, -10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, -20, 0, -10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	createsprite gFallingIceRockSpriteTemplate, ANIM_TARGET, 2, 30, 2, 10, FALSE
	playsewithpan SE_M_ROCK_THROW, SOUND_PAN_TARGET
	delay 2
	return

@ Credits: Skeli
gMoveAnim_ICE_SHARD::
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	createsprite gIceShardSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, SE_M_SAND_ATTACK
	delay 1
	createsprite gIceShardSpriteTemplate, ANIM_TARGET, 2, 0, 0, -20, SE_M_SAND_ATTACK
	delay 1
	createsprite gIceShardSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, SE_M_SAND_ATTACK
	delay 1
	createsprite gIceShardSpriteTemplate, ANIM_TARGET, 2, 0, 0, -20, SE_M_SAND_ATTACK
	delay 1
	createsprite gIceShardSpriteTemplate, ANIM_TARGET, 2, 0, 0, 30, SE_M_SAND_ATTACK
	delay 4
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 3, 2, 0, 11, RGB_WHITE
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 2, 0, 32, 1
	call IceCrystalEffectShort
	end

@ Credits: Skeli
gMoveAnim_SHADOW_CLAW::
	loadspritegfx ANIM_TAG_CLAW_SLASH
	loadspritegfx ANIM_TAG_HANDS_AND_FEET @ Black color
	monbg ANIM_TARGET
	setalpha 12, 8
	fadetobg BG_GHOST
	waitbgfadein
	createsprite gShadowClawSpriteTemplate, ANIM_TARGET, 2, 0, -22, 1
	createsprite gShadowClawSpriteTemplate, ANIM_TARGET, 2, 0, -6, 1
	createsprite gShadowClawSpriteTemplate, ANIM_TARGET, 2, 0, 10, 1
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 16, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	restorebg
	waitbgfadein
	end

@ Credits: Skeli
gMoveAnim_THUNDER_FANG::
	loadspritegfx ANIM_TAG_SHARP_TEETH
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SPARK_2
	monbg ANIM_TARGET
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 9, RGB(30, 31, 1) @ Yellow color
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, -32, 0, 0, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 4, 0, -819, 10
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SHARP_TEETH, 1, 0, 10, RGB(30, 31, 1) @ Yellow color
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 5, 0, 5, 0
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -5, 10, 5, 1
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	playsewithpan SE_M_THUNDERBOLT2, SOUND_PAN_TARGET
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 15, 20, 5, 2
	delay 1
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -15, -10, 5, 0
	delay 1
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 25, 0, 5, 1
	delay 1
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -8, 8, 5, 2
	delay 1
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, 2, -8, 5, 0
	delay 1
	createsprite gElectricitySpriteTemplate, ANIM_TARGET, 2, -20, 15, 5, 1
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 9, 0, RGB(30, 31, 1) @ Yellow color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 7, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_ICE_FANG::
	loadspritegfx ANIM_TAG_SHARP_TEETH
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_ICE_CRYSTALS
	monbg ANIM_TARGET
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 0, 9, RGB(12, 26, 31) @ Blue color
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, -32, 0, 0, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 4, 0, -819, 10
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SHARP_TEETH, 1, 0, 10, RGB(12, 26, 31) @ Blue color
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	call IceCrystalEffectShort
	delay 4
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 9, 0, RGB(12, 26, 31) @ Blue color
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 7, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_FIRE_FANG::
	loadspritegfx ANIM_TAG_SHARP_TEETH
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_SMALL_EMBER
	monbg ANIM_TARGET
	playsewithpan SE_M_BITE, SOUND_PAN_TARGET
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, -32, 0, 0, 819, 10
	createsprite gSharpTeethSpriteTemplate, ANIM_ATTACKER, 2, 0, 32, 4, 0, -819, 10
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_SHARP_TEETH, 1, 0, 10, RGB_RED
	delay 10
	createsprite gBasicHitSplatSpriteTemplate, ANIM_ATTACKER, 2, 0, 0, ANIM_TARGET, 2
	call FireSpreadEffect
	delay 4
	playsewithpan SE_M_FIRE_PUNCH, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 5, ANIM_TARGET, 0, 4, 7, 1
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_SHADOW_SNEAK::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_WHITE_SHADOW
	loadspritegfx ANIM_TAG_HANDS_AND_FEET @ Black color
	loadspritegfx ANIM_TAG_QUICK_GUARD @ Black color
	monbg ANIM_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 7, RGB_BLACK
	waitforvisualfinish
	playsewithpan SE_M_FAINT_ATTACK, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_TranslateMonEllipticalRespectSide, 2, ANIM_ATTACKER, 18, 6, 1, 3
	createvisualtask AnimTask_AttackerFadeToInvisible, 2, 1
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	invisible ANIM_ATTACKER
	delay 1
	createvisualtask AnimTask_DestinyBondWhiteShadow, 5, 48, FALSE, TRUE
	delay 48
	createvisualtask AnimTask_BlendColorCycle, 2, F_PAL_TARGET, 2, 2, 0, 15, RGB_BLACK
	createsprite gShadowSneakHitSpriteTemplate, ANIM_TARGET, 2, -10, -10, FALSE
	call ShadowSneakHitTargetShake
	createsprite gShadowSneakHitSpriteTemplate, ANIM_TARGET, 2, 10, 20, FALSE
	call ShadowSneakHitTargetShake
	createsprite gShadowSneakHitSpriteTemplate, ANIM_TARGET, 2, -5, 10, FALSE
	call ShadowSneakHitTargetShake
	createsprite gShadowSneakHitSpriteTemplate, ANIM_TARGET, 2, 17, -12, FALSE
	call ShadowSneakHitTargetShake
	waitforvisualfinish
	visible ANIM_ATTACKER
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 7, 0, RGB_BLACK
	end

ShadowSneakHitTargetShake::
	playsewithpan SE_M_DOUBLE_SLAP, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 3, 0, 6, 1
	delay 4
	return

@ Credits: Skeli
gMoveAnim_MUD_BOMB::
	loadspritegfx ANIM_TAG_MUD_BOMB
	loadspritegfx ANIM_TAG_POISON_BUBBLE
	loadspritegfx ANIM_TAG_BROWN_ORB @ Brown color
	createsprite gMudBombBallSpriteTemplate, ANIM_TARGET, 2, 0, 0, 56, TRUE, 0, 0, FALSE
	playsewithpan SE_M_SWAGGER, SOUND_PAN_ATTACKER
	delay 56
	playsewithpan SE_M_BUBBLE3, SOUND_PAN_TARGET
	createvisualtask AnimTask_ShakeMon, 3, ANIM_TARGET, 0, 4, 20, 1
	createsprite gMudBombDropletSpriteTemplate, ANIM_TARGET, 2, 0, -8, 0, 55, FALSE
	createsprite gMudBombDropletSpriteTemplate, ANIM_TARGET, 2, 15, -11, 0, 50, FALSE
	createsprite gMudBombSludgeSpriteTemplate, ANIM_TARGET, 2, 0, 5, 20, -24, 14, 2
	createsprite gMudBombSludgeSpriteTemplate, ANIM_TARGET, 2, -5, 0, -20, -24, 14, 2
	end

@ Credits: Skeli
gMoveAnim_PSYCHO_CUT::
	loadspritegfx ANIM_TAG_PUNISHMENT_BLADES
	monbg ANIM_TARGET
	loopsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER, 10, 3
	createsprite gPsychoCutBladeWheelSpriteTemplate, ANIM_ATTACKER, 3, ANIM_ATTACKER, 0, 0, 69, 0, FALSE, 0
	delay 37
	createsprite gAirSlashBladeSpriteTemplate, ANIM_TARGET, 2, 0, 0, 37
	delay 37
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 4, 0, 8, 1
	playsewithpan SE_M_RAZOR_WIND, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	end

@ Credits: Skeli
gMoveAnim_ZEN_HEADBUTT::
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_WATER_IMPACT @ Blue color
	monbg ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 0, 16, RGB_BLACK
	waitforvisualfinish
	loopsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER, 10, 3
	createsprite gZenHeadbuttCircleSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 2, 0
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 0
	waitsound
	createsprite gZenHeadbuttLaunchBallSpriteTemplate, ANIM_TARGET, 3, ANIM_ATTACKER
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 1
	waitforvisualfinish
	createvisualtask AnimTask_ShakeMonInPlace, 2, ANIM_ATTACKER, 2, 0, 4, 1
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 5, 0, 6, 1
	createsprite gBowMonSpriteTemplate, ANIM_ATTACKER, 2, 2
	createsprite gFlashingHitSplatSpriteTemplate, ANIM_TARGET, 3, 0, 0, ANIM_TARGET, 1
	playsewithpan SE_M_VITAL_THROW2, SOUND_PAN_TARGET
	waitforvisualfinish
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_BG, 1, 16, 0, RGB_BLACK
	waitforvisualfinish
	clearmonbg ANIM_TARGET
	blendoff
	end

@ Credits: Skeli
gMoveAnim_MIRROR_SHOT::
	loopsewithpan SE_M_HARDEN, SOUND_PAN_ATTACKER, 28, 2
	createvisualtask AnimTask_MetallicShine, 5, FALSE, FALSE, 0
	waitforvisualfinish
	playsewithpan SE_M_LEER, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_Flash, 2
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 0, 3, 16, 1
	end

@ Credits: Lixdel and Skeli
gMoveAnim_FLASH_CANNON::
	loadspritegfx ANIM_TAG_FLASH_CANNON_BALL
	loadspritegfx ANIM_TAG_ELECTRIC_ORBS
	loadspritegfx ANIM_TAG_CIRCLE_OF_LIGHT
	loadspritegfx ANIM_TAG_HANDS_AND_FEET @ Black color
	monbg ANIM_ATTACKER
	setalpha 13, 8
	createvisualtask AnimTask_BlendParticle, 5, ANIM_TAG_ELECTRIC_ORBS, 0, 16, 16, RGB(29, 29, 28) @ Light gray color
	loopsewithpan SE_M_CHARGE, SOUND_PAN_ATTACKER, 7, 20
	createsprite gFlashCannonChargeSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 0
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopBottomChargeOrb
	call FlashCannonTopBottomChargeOrb
	call FlashCannonLeftRightChargeOrb
	call FlashCannonLeftRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopBottomChargeOrb
	call FlashCannonTopBottomChargeOrb
	call FlashCannonLeftRightChargeOrb
	call FlashCannonLeftRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopBottomChargeOrb
	call FlashCannonTopBottomChargeOrb
	call FlashCannonLeftRightChargeOrb
	call FlashCannonLeftRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopLeftBottomRightChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopRightBottomLeftChargeOrb
	call FlashCannonTopBottomChargeOrb
	waitforvisualfinish
	stopsound
	playsewithpan SE_M_PAY_DAY, SOUND_PAN_ATTACKER
	createsprite gFlashCannonBallSpriteTemplate, ANIM_TARGET, 2, 0, 0, 21
	waitforvisualfinish
	clearmonbg ANIM_ATTACKER
	playsewithpan SE_M_LEER, SOUND_PAN_TARGET
	createvisualtask AnimTask_Flash, 2
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 1, 16, 0, RGB_RED
	createvisualtask AnimTask_ShakeMon2, 2, ANIM_TARGET, 2, 0, 15, 1
	end

FlashCannonTopRightBottomLeftChargeOrb::
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, 25, -40, 0, 0, 30, 20
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, -25, 40, 0, 0, 30, -20
	delay 3
	return

FlashCannonTopBottomChargeOrb::
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, 5, -40, 0, 0, 30, 10
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, -5, 40, 0, 0, 30, 10
	delay 3
	return

FlashCannonLeftRightChargeOrb::
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, -40, 5, 0, 0, 30, -10
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, 40, -5, 0, 0, 30, 10
	delay 3
	return

FlashCannonTopLeftBottomRightChargeOrb::
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, -25, -40, 0, 0, 30, -20
	createsprite gFlashCannonChargeOrbSpriteTemplate, ANIM_ATTACKER, 2, 25, 20, 0, 0, 30, 20
	delay 3
	return

@ Credits: Skeli
gMoveAnim_ROCK_CLIMB::
	loadspritegfx ANIM_TAG_IMPACT
	loadspritegfx ANIM_TAG_MUD_SAND
	loadspritegfx ANIM_TAG_SMALL_ROCK
	monbg ANIM_DEF_SIDE
	splitbgprio ANIM_TARGET
	setalpha 12, 8
	createvisualtask AnimTask_Splash, 2, ANIM_ATTACKER, 3
	delay 8
	loopsewithpan SE_M_TAIL_WHIP, SOUND_PAN_ATTACKER, 38, 3
	call RockClimbRock
	delay 16
	call RockClimbRock
	delay 16
	call RockClimbRock
	delay 16
	call RockClimbRock
	waitforvisualfinish
	setargret 1
	createvisualtask AnimTask_Rollout, 2
	waitforvisualfinish
	createvisualtask AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 2, FALSE, 1, 30, TRUE, FALSE
	createsprite gBasicHitSplatSpriteTemplate, ANIM_TARGET, 4, 0, 0, ANIM_TARGET, 2
	playsewithpan SE_M_MEGA_KICK2, SOUND_PAN_TARGET
	waitforvisualfinish
	clearmonbg ANIM_DEF_SIDE
	blendoff
	end

RockClimbRock::
	createsprite gDropRockSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 0, 0
	createsprite gDropRockSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 1, 0
	createsprite gDropRockSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 2, 0
	createsprite gDropRockSpriteTemplate, ANIM_ATTACKER, 2, ANIM_ATTACKER, 3, 0
	return

@ Credits: Lixdel
gMoveAnim_DEFOG::
	playsewithpan SE_M_HAZE, SOUND_PAN_ABOVE
	createvisualtask AnimTask_LoadMistTiles, 5
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 3, 0, 16, RGB_WHITE
	delay 8
	createvisualtask AnimTask_ShakeMon, 2, ANIM_TARGET, 4, 0, 70, 0
	delay 70
	createvisualtask AnimTask_BlendSelected, 10, F_PAL_TARGET, 2, 16, 0, RGB_WHITE
	end

@ Credits: Skeli
gMoveAnim_TRICK_ROOM::
	call SummonRoomAttackerScaleAnim
	fadetobg BG_TRICK_ROOM
	waitbgfadein
	delay 64
	blendoff
	restorebg
	waitbgfadein
	end

SummonRoomAttackerScaleAnim::
	setalpha 8, 8
	playsewithpan SE_M_SUPERSONIC, SOUND_PAN_ATTACKER
	createvisualtask AnimTask_ScaleMonAndRestore, 5, -5, -5, 15, ANIM_ATTACKER, ST_OAM_OBJ_BLEND
	return
