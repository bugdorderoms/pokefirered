#include "global.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_player_avatar.h"
#include "field_fadetransition.h"
#include "fldeff.h"
#include "party_menu.h"
#include "overworld.h"

static void FieldCallback_Teleport(void);
static void StartTeleportFieldEffect(void);

bool32 SetUpFieldMove_Teleport(void)
{
    if (Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType))
    {
        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
        gPostMenuFieldCallback = FieldCallback_Teleport;
        return TRUE;
    }
    return FALSE;
}

static void FieldCallback_Teleport(void)
{
    Overworld_ResetStateForLeavingMap();
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    FieldEffectStart(FLDEFF_USE_TELEPORT);
}

u32 FldEff_UseTeleport(void)
{
    u32 taskId = CreateFieldEffectShowMon();
    FLDEFF_SET_FUNC_TO_DATA(StartTeleportFieldEffect);
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
    return 0;
}

static void StartTeleportFieldEffect(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_TELEPORT);
    CreateTeleportFieldEffectTask();
}
