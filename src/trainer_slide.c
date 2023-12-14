#include "global.h"
#include "battle.h"
#include "pokemon.h"
#include "battle_anim.h"
#include "battle_util.h"
#include "trainer_slide.h"
#include "constants/pokemon.h"

static u8 GetEnemyTeamCount(bool8 onlyAlive)
{
	u8 i, count;
	
	for (i = 0, count = 0; i < PARTY_SIZE; i++)
	{
		if (IsMonValidSpecies(&gEnemyParty[i]) && (!onlyAlive || GetMonData(&gEnemyParty[i], MON_DATA_HP, NULL)))
			++count;
	}
	return count;
}

bool8 ShouldDoTrainerSlide(u8 battlerId, u8 caseId)
{
	bool8 doSlide = FALSE;
	u16 trainerId = gTrainerBattleOpponent_A;
	
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
	{
		gBattleScripting.battler = battlerId;
		
		switch (caseId)
		{
			case TRAINER_SLIDE_FIRST_MON_SEND_OUT:
			    if (!gBattleStruct->firstMonSendOutMsgDone && gTrainerSlides[trainerId].firstMonMsg != NULL)
				{
					gBattleStruct->firstMonSendOutMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].firstMonMsg;
					doSlide = TRUE;
				}
			    break;
			case TRAINER_SLIDE_FIRST_MON_DOWN:
			    if (!gBattleStruct->firstMonDownMsgDone && gTrainerSlides[trainerId].firstMonDownMsg != NULL
				&& GetEnemyTeamCount(TRUE) == GetEnemyTeamCount(FALSE) - 1)
				{
					gBattleStruct->firstMonDownMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].firstMonDownMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_LAST_MON_SEND_OUT:
			    if (!gBattleStruct->lastMonSendOutMsgDone && gTrainerSlides[trainerId].lastMonMsg != NULL && GetEnemyTeamCount(TRUE) == 1)
				{
					gBattleStruct->lastMonSendOutMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].lastMonMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_LAST_MON_LOW_HP:
				if (!gBattleStruct->lastMonLowHPMsgDone && gTrainerSlides[trainerId].lastMonLowHPMsg != NULL && GetEnemyTeamCount(TRUE) == 1
				&& gBattleMons[battlerId].hp < gBattleMons[battlerId].maxHP / 4) // Below 25% of hp
				{
					gBattleStruct->lastMonLowHPMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].lastMonLowHPMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_FIRST_SUPER_EFFECTIVE_HIT_TAKEN:
			    if (gBattleStruct->firstSuperEffectiveHitTakenMsgState == 1 && gTrainerSlides[trainerId].firstSuperEffectiveHitTakenMsg != NULL)
				{
					++gBattleStruct->firstSuperEffectiveHitTakenMsgState;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].firstSuperEffectiveHitTakenMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_FIRST_CRITICAL_TAKEN:
			    if (gBattleStruct->firstCritcalHitTakenMsgState == 1 && gTrainerSlides[trainerId].firstCriticalTakenMsg != NULL)
				{
					++gBattleStruct->firstCritcalHitTakenMsgState;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].firstCriticalTakenMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_MEGA_EVOLUTION:
			    if (!gBattleStruct->megaEvolutionMsgDone && gTrainerSlides[trainerId].megaEvoMsg != NULL)
				{
					gBattleStruct->megaEvolutionMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].megaEvoMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_Z_MOVE:
			    if (!gBattleStruct->zMoveMsgDone && gTrainerSlides[trainerId].zMoveMsg != NULL)
				{
					gBattleStruct->zMoveMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].zMoveMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_DYNAMAX:
			    if (!gBattleStruct->dynamaxMsgDone && gTrainerSlides[trainerId].dynamaxMsg != NULL)
				{
					gBattleStruct->dynamaxMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].dynamaxMsg;
					doSlide = TRUE;
				}
				break;
			case TRAINER_SLIDE_TERASTALIZATION:
			    if (!gBattleStruct->terastalMsgDone && gTrainerSlides[trainerId].terastalMsg != NULL)
				{
					gBattleStruct->terastalMsgDone = TRUE;
					gBattleStruct->trainerSlideMsg = gTrainerSlides[trainerId].terastalMsg;
					doSlide = TRUE;
				}
				break;
		}
	}
	return doSlide;
}
