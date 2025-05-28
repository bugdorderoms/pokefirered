#ifndef GUARD_TRAINER_SEE_H
#define GUARD_TRAINER_SEE_H

#include "global.h"

bool32 CheckForTrainersWantingBattle(void);
void MovementAction_RevealTrainer_RunTrainerSeeFuncList(struct ObjectEvent *var);
u32 FldEff_ExclamationMarkIcon1(void);
u32 FldEff_DoubleExclMarkIcon(void);
u32 FldEff_XIcon(void);
u32 FldEff_SmileyFaceIcon(void);
u32 FldEff_QuestionMarkIcon(void);

#endif //GUARD_TRAINER_SEE_H
