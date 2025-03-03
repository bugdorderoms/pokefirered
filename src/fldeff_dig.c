#include "global.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "fldeff.h"
#include "item_use.h"
#include "field_fadetransition.h"
#include "overworld.h"
#include "party_menu.h"

static void FieldCallback_Dig(void);
static void sub_80C9AFC(void);

bool8 SetUpFieldMove_Dig(void)
{
    if (CanUseEscapeRopeOnCurrMap() == TRUE)
    {
        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
        gPostMenuFieldCallback = FieldCallback_Dig;
        return TRUE;
    }
    return FALSE;
}

static void FieldCallback_Dig(void)
{
    Overworld_ResetStateForLeavingMap();
    FieldEffectStart(FLDEFF_USE_DIG);
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
}

bool8 FldEff_UseDig(void)
{
    u8 taskId = CreateFieldEffectShowMon();

    FLDEFF_SET_FUNC_TO_DATA(sub_80C9AFC);
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
    return FALSE;
}

static void sub_80C9AFC(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_DIG);
    gTasks[CreateTask(sub_80A1C44, 8)].data[0] = 0;
}
