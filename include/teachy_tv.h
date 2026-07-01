#ifndef GUARD_TEACHY_TV_H
#define GUARD_TEACHY_TV_H

enum TeachyTvScript
{
    TTVSCR_BATTLE,
    TTVSCR_STATUS,
    TTVSCR_MATCHUPS,
    TTVSCR_CATCHING,
    TTVSCR_TMS,
    TTVSCR_REGISTER
};

void InitTeachyTvController(u32 mode, void (*cb)());
void CB2_ReturnToTeachyTV(void);
void SetTeachyTvControllerModeToResume(void);

extern const u8 gPokedudeText_TMTypes[];
extern const u8 gPokedudeText_ReadTMDescription[];

#endif //GUARD_TEACHY_TV_H
