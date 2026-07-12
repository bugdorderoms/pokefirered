static const union AffineAnimCmd sAffineAnim_Lightbulb[] = {
    AFFINEANIMCMD_FRAME(2, 2, 0, 40),
    AFFINEANIMCMD_FRAME(0, 0, 0, 5),
    AFFINEANIMCMD_FRAME(-2, -2, 0, 40),
    AFFINEANIMCMD_FRAME(0, 0, 0, 5),
    AFFINEANIMCMD_FRAME(2, 2, 0, 20),
    AFFINEANIMCMD_FRAME(0, 0, 0, 5),
    AFFINEANIMCMD_FRAME(-2, -2, 0, 20),
    AFFINEANIMCMD_FRAME(0, 0, 0, 5),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd *const sAffineAnimTable_Lightbulb[] = {
    sAffineAnim_Lightbulb
};
