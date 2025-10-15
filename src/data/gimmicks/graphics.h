static const u8 sGimmickIndicatorGfx[] = INCBIN_U8("graphics/battle_interface/gimmicks/indicators.4bpp");
static const u16 sGimmickIndicatorPal[] = INCBIN_U16("graphics/battle_interface/gimmicks/indicators.gbapal");

static const s8 sIndicatorPositions[][2] =
{
    [B_POSITION_PLAYER_LEFT] = {49, -5},
    [B_POSITION_OPPONENT_LEFT] = {40, -5},
    [B_POSITION_PLAYER_RIGHT] = {48, -5},
    [B_POSITION_OPPONENT_RIGHT] = {40, -5},
};

const struct SpritePalette gSpritePalette_GimmickIndicator =
{
    .data = sGimmickIndicatorPal,
    .tag = TAG_GIMMICK_INDICATOR_GFX,
};

static const struct OamData sOamData_GimmickIndicator =
{
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 1,
};

static const union AnimCmd sIndicator_Mega[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sIndicator_Omega[] = {
    ANIMCMD_FRAME(1, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sIndicator_Alpha[] = {
    ANIMCMD_FRAME(2, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sIndicator_Ultra[] = {
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sIndicator_Dynamax[] = {
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sIndicator_Tera[] = {
    ANIMCMD_FRAME(5, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sIndicator_totem[] = {
    ANIMCMD_FRAME(6, 0),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sGimmickIndicators[] = {
    [GIMMICK_INDICATOR_NONE] = sIndicator_Mega, // placeholder
    [GIMMICK_INDICATOR_MEGA] = sIndicator_Mega,
    [GIMMICK_INDICATOR_OMEGA] = sIndicator_Omega,
    [GIMMICK_INDICATOR_ALPHA] = sIndicator_Alpha,
    [GIMMICK_INDICATOR_ULTRA_BURST] = sIndicator_Ultra,
    [GIMMICK_INDICATOR_DYNAMAX] = sIndicator_Dynamax,
    [GIMMICK_INDICATOR_TERA] = sIndicator_Tera,
    [GIMMICK_INDICATOR_TOTEM] = sIndicator_totem,
};

static const u8 ALIGNED(4) sMegaTriggerGfx[] = INCBIN_U8("graphics/battle_interface/gimmicks/mega_trigger.4bpp");
static const u8 ALIGNED(4) sBurstTriggerGfx[] = INCBIN_U8("graphics/battle_interface/gimmicks/ultra_burst_trigger.4bpp");
static const u8 ALIGNED(4) sTeraTriggerGfx[] = INCBIN_U8("graphics/battle_interface/gimmicks/tera_trigger.4bpp");
// static const u8 ALIGNED(4) sZMoveTriggerGfx[] = INCBIN_U8("graphics/battle_interface/gimmicks/z_move_trigger.4bpp");
// static const u8 ALIGNED(4) sDynamaxTriggerGfx[] = INCBIN_U8("graphics/battle_interface/gimmicks/dynamax_trigger.4bpp");

static const u16 sMegaTriggerPal[] = INCBIN_U16("graphics/battle_interface/gimmicks/mega_trigger.gbapal");
static const u16 sBurstTriggerPal[] = INCBIN_U16("graphics/battle_interface/gimmicks/ultra_burst_trigger.gbapal");
static const u16 sTeraTriggerPal[] = INCBIN_U16("graphics/battle_interface/gimmicks/tera_trigger.gbapal");
// static const u16 sZMoveTriggerPal[] = INCBIN_U16("graphics/battle_interface/gimmicks/z_move_trigger.gbapal");
// static const u16 sDynamaxTriggerPal[] = INCBIN_U16("graphics/battle_interface/gimmicks/dynamax_trigger.gbapal");

static const struct SpriteSheet sSpriteSheet_MegaTrigger = {sMegaTriggerGfx, sizeof(sMegaTriggerGfx), TAG_GIMMICK_TRIGGER_GFX};
static const struct SpriteSheet sSpriteSheet_BurstTrigger = {sBurstTriggerGfx, sizeof(sBurstTriggerGfx), TAG_GIMMICK_TRIGGER_GFX};
static const struct SpriteSheet sSpriteSheet_TeraTrigger = {sTeraTriggerGfx, sizeof(sTeraTriggerGfx), TAG_GIMMICK_TRIGGER_GFX};
// static const struct SpriteSheet sSpriteSheet_ZMoveTrigger = {sZMoveTriggerGfx, sizeof(sZMoveTriggerGfx), TAG_GIMMICK_TRIGGER_GFX};
// static const struct SpriteSheet sSpriteSheet_DynamaxTrigger = {sDynamaxTriggerGfx, sizeof(sDynamaxTriggerGfx), TAG_GIMMICK_TRIGGER_GFX};

static const struct OamData sOamData_GimmickTrigger =
{
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const union AffineAnimCmd sGimmickTriggerAffineAnim_Singles[] = 
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sGimmickTriggerAffineAnim_Doubles[] = 
{
    AFFINEANIMCMD_FRAME(240, 240, 0, 0), // A little smaller in doubles
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sGimmickTriggerAffineAnimTable[] = 
{
    sGimmickTriggerAffineAnim_Singles,
    sGimmickTriggerAffineAnim_Doubles
};

static const struct SpriteTemplate sSpriteTemplate_GimmickTrigger =
{
    .tileTag = TAG_GIMMICK_TRIGGER_GFX,
    .paletteTag = TAG_GIMMICK_TRIGGER_GFX,
    .oam = &sOamData_GimmickTrigger,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sGimmickTriggerAffineAnimTable,
    .callback = SpriteCB_GimmickTrigger,
};
