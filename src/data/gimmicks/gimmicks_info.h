static const struct GimmickInfo sGimmicksInfo[GIMMICKS_COUNT] =
{
    [GIMMICK_NONE] = {0},
    [GIMMICK_MEGA] =
    {
        .triggerSheet = &sSpriteSheet_MegaTrigger,
        .triggerPal = sMegaTriggerPal,
        .canActivate = CanMegaEvolve,
        .activateGimmick = ActivateMegaEvolution,
        .indicatorId = GIMMICK_INDICATOR_MEGA,
    },
    [GIMMICK_ULTRA_BURST] =
    {
        .triggerSheet = &sSpriteSheet_BurstTrigger,
        .triggerPal = sBurstTriggerPal,
        .canActivate = CanUltraBurst,
        .activateGimmick = ActivateUltraBurst,
        .indicatorId = GIMMICK_INDICATOR_ULTRA_BURST,
    },
    [GIMMICK_TERA] =
    {
        .triggerSheet = &sSpriteSheet_TeraTrigger,
        .triggerPal = sTeraTriggerPal,
        .canActivate = CanTerastallize,
        .activateGimmick = ActivateTera,
        .indicatorId = GIMMICK_INDICATOR_TERA,
    }
    /*[GIMMICK_Z_MOVE] =
    {
        .triggerSheet = &sSpriteSheet_ZMoveTrigger,
        .triggerPal = sZMoveTriggerPal,
        .canActivate = CanUseZMove,
        .activateGimmick = ActivateZMove,
    },
    [GIMMICK_DYNAMAX] =
    {
        .triggerSheet = &sSpriteSheet_DynamaxTrigger,
        .triggerPal = sDynamaxTriggerPal,
        .canActivate = CanDynamax,
        .activateGimmick = ActivateDynamax,
        .indicatorId = GIMMICK_INDICATOR_DYNAMAX,
    },*/
};
