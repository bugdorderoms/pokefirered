#define FUSION(master, other, itemId, target, fusionId) \
{                                                       \
	.masterSpecies = master,                            \
	.otherSpecies = other,                              \
	.item = itemId,                                     \
	.targetSpecies = target,                            \
	.id = fusionId,                                     \
}

const struct Fusion gFusionsTable[] =
{
	FUSION(SPECIES_KYUREM,   SPECIES_RESHIRAM,  ITEM_DNA_SPLICERS,   SPECIES_KYUREM_WHITE,         FUSION_RESHIRAN),
	FUSION(SPECIES_KYUREM,   SPECIES_ZEKROM,    ITEM_DNA_SPLICERS,   SPECIES_KYUREM_BLACK,         FUSION_ZEKROM),
	FUSION(SPECIES_NECROZMA, SPECIES_SOLGALEO,  ITEM_N_SOLARIZER,    SPECIES_NECROZMA_DUSK_MANE,   FUSION_SOLGALEO),
	FUSION(SPECIES_NECROZMA, SPECIES_LUNALA,    ITEM_N_LUNARIZER,    SPECIES_NECROZMA_DAWN_WINGS,  FUSION_LUNALA),
	FUSION(SPECIES_CALYREX,  SPECIES_GLASTRIER, ITEM_REINS_OF_UNITY, SPECIES_CALYREX_ICE_RIDER,    FUSION_GLASTRIER),
	FUSION(SPECIES_CALYREX,  SPECIES_SPECTRIER, ITEM_REINS_OF_UNITY, SPECIES_CALYREX_SHADOW_RIDER, FUSION_SPECTRIER),
	FUSION(SPECIES_NONE,     SPECIES_NONE,      ITEM_NONE,           SPECIES_NONE,                 FUSIONS_COUNT), // end table
};
