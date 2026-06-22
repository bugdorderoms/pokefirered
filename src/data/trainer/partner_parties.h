const struct Trainer gBattlePartners[PARTNER_COUNT] =
{
    [PARTNER_NONE] =
    {
        .trainerName = _(""),
    },
    [PARTNER_RAID_YOUNGSTER_NOAH] =
    {
        .trainerName = _("Noah"),
        .trainerClass = TRAINER_CLASS_YOUNGSTER,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidYoungsterNoah),
    },
    [PARTNER_RAID_PKMN_BREEDER_KIT] =
    {
        .trainerName = _("Kit"),
        .trainerClass = TRAINER_CLASS_PKMN_BREEDER,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidPkmnBreederKit),
    },
    [PARTNER_RAID_GENTLEMAN_MARTIN] =
    {
        .trainerName = _("Martin"),
        .trainerClass = TRAINER_CLASS_GENTLEMAN,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidGentlemanMartin),
    },
    [PARTNER_RAID_BLACK_BELT_OSCAR] =
    {
        .trainerName = _("Oscar"),
        .trainerClass = TRAINER_CLASS_BLACK_BELT,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidBlackBeltOscar),
    },
    [PARTNER_RAID_PKMN_BREEDER_POPPY] =
    {
        .trainerName = _("Poppy"),
        .trainerClass = TRAINER_CLASS_PKMN_BREEDER,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidPkmnBreederPoppy),
    },
    [PARTNER_RAID_GENTLEMAN_ALFIE] =
    {
        .trainerName = _("Alfie"),
        .trainerClass = TRAINER_CLASS_GENTLEMAN,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidGentlemanAlfie),
    },
    [PARTNER_RAID_YOUNGSTER_AUSTIN] =
    {
        .trainerName = _("Austin"),
        .trainerClass = TRAINER_CLASS_YOUNGSTER,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidYoungsterAustin),
    },
    [PARTNER_RAID_BLACK_BELT_ARTHUR] =
    {
        .trainerName = _("Arthur"),
        .trainerClass = TRAINER_CLASS_BLACK_BELT,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidBlackBeltArthur),
    },
    [PARTNER_RAID_YOUNGSTER_LOGAN] =
    {
        .trainerName = _("Logan"),
        .trainerClass = TRAINER_CLASS_YOUNGSTER,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidYoungsterLogan),
    },
    [PARTNER_RAID_YOUNGSTER_EVAN] =
    {
        .trainerName = _("Evan"),
        .trainerClass = TRAINER_CLASS_YOUNGSTER,
        .trainerPic = TRAINER_BACK_PIC_POKEDUDE,
        .items = { ITEM_POTION, ITEM_POTION },
        TRAINER_PARTY(sParty_RaidYoungsterEvan),
    },
};
