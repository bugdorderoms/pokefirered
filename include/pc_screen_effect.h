#ifndef GUARD_PC_SCREEN_EFFECT_H
#define GUARD_PC_SCREEN_EFFECT_H

void BeginPCScreenEffect_TurnOn(u32 speed, u32 priority);
void BeginPCScreenEffect_TurnOff(u32 speed, u32 priority);
bool32 IsPCScreenEffectRunning_TurnOff(void);
bool32 IsPCScreenEffectRunning_TurnOn(void);

#endif //GUARD_PC_SCREEN_EFFECT_H
