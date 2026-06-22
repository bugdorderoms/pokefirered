static const struct GimmickInfo sGimmicksInfo[GIMMICKS_COUNT] =
{
    [GIMMICK_NONE] = {0},
    [GIMMICK_MEGA] =
    {
        .triggerSheet = &sSpriteSheet_MegaTrigger,
        .triggerPal = sMegaTriggerPal,
        .keyItemCheck = HasMegaBracelet,
        .canActivate = CanMegaEvolve,
        .activateGimmick = ActivateMegaEvolution,
        .indicatorId = GIMMICK_INDICATOR_MEGA,
        .activationScript = BattleScript_MegaEvolution,
        .removesOnFaint = TRUE,
    },
    [GIMMICK_ULTRA_BURST] =
    {
        .triggerSheet = &sSpriteSheet_BurstTrigger,
        .triggerPal = sBurstTriggerPal,
        .keyItemCheck = HasZRing,
        .canActivate = CanUltraBurst,
        .activateGimmick = ActivateUltraBurst,
        .indicatorId = GIMMICK_INDICATOR_ULTRA_BURST,
        .activationScript = BattleScript_UltraBurst,
        .removesOnFaint = TRUE,
    },
    [GIMMICK_DYNAMAX] =
    {
        .triggerSheet = &sSpriteSheet_DynamaxTrigger,
        .triggerPal = sDynamaxTriggerPal,
        .keyItemCheck = HasDynamaxBand,
        .canActivate = CanDynamax,
        .activateGimmick = ActivateDynamax,
        .indicatorId = GIMMICK_INDICATOR_DYNAMAX,
        .activationScript = BattleScript_Dynamax,
        .removesOnFaint = TRUE,
        .removesOnSwitchOut = TRUE,
    },
    [GIMMICK_TERA] =
    {
        .triggerSheet = &sSpriteSheet_TeraTrigger,
        .triggerPal = sTeraTriggerPal,
        .keyItemCheck = HasTeraOrb,
        .canActivate = CanTerastallize,
        .activateGimmick = ActivateTera,
        .indicatorId = GIMMICK_INDICATOR_TERA,
        .activationScript = BattleScript_Terastallization,
        .removesOnFaint = TRUE,
    }
    /*[GIMMICK_Z_MOVE] =
    {
        .triggerSheet = &sSpriteSheet_ZMoveTrigger,
        .triggerPal = sZMoveTriggerPal,
        .keyItemCheck = HasZRing,
        .canActivate = CanUseZMove,
        .activateGimmick = ActivateZMove,
    },*/
};
