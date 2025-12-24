#ifndef GUARD_BATTLE_AI_MOVE_EFFECTS_H
#define GUARD_BATTLE_AI_MOVE_EFFECTS_H

enum
{
    AI_CHECK_BAD_MOVE_EFFECT,
    AI_CHECK_GOOD_MOVE_EFFECT,
    AI_CHECK_GOOD_MOVE_EFFECT_ON_PARTNER
};

s8 BattleAIFunc_EffectOHKO(struct AIScript *data, s8 score, u32 caseId);
s8 BattleAIFunc_EffectTwoTurnsAttack(struct AIScript *data, s8 score, u32 caseId);
s8 BattleAIFunc_EffectUserAttackUp2(struct AIScript *data, s8 score, u32 caseId);
s8 BattleAIFunc_EffectRandomSwitch(struct AIScript *data, s8 score, u32 caseId);

#endif // GUARD_BATTLE_AI_MOVE_EFFECTS_H
