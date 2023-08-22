const struct FormChange *const gFormChangeTable[NUM_SPECIES] =
{
	[SPECIES_VENUSAUR] = sVenusaurFormChangeTable,
	[SPECIES_VENUSAUR_MEGA] = sVenusaurFormChangeTable,
	[SPECIES_VENUSAUR_GIGA] = sVenusaurFormChangeTable,
	[SPECIES_CHARIZARD] = sCharizardFormChangeTable,
	[SPECIES_CHARIZARD_MEGA_X] = sCharizardFormChangeTable,
	[SPECIES_CHARIZARD_MEGA_Y] = sCharizardFormChangeTable,
	[SPECIES_CHARIZARD_GIGA] = sCharizardFormChangeTable,
	[SPECIES_BLASTOISE] = sBlastoiseFormChangeTable,
	[SPECIES_BLASTOISE_MEGA] = sBlastoiseFormChangeTable,
	[SPECIES_BLASTOISE_GIGA] = sBlastoiseFormChangeTable,
	[SPECIES_BUTTERFREE] = sButterfreeFormChangeTable,
	[SPECIES_BUTTERFREE_GIGA] = sButterfreeFormChangeTable,
	[SPECIES_BEEDRILL] = sBeedrillFormChangeTable,
	[SPECIES_BEEDRILL_MEGA] = sBeedrillFormChangeTable,
	[SPECIES_PIDGEOT] = sPidgeotFormChangeTable,
	[SPECIES_PIDGEOT_MEGA] = sPidgeotFormChangeTable,
	[SPECIES_PIKACHU] = sPikachuFormChangeTable,
	[SPECIES_PIKACHU_COSPLAY] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_ROCK_STAR] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_BELLE] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_POP_STAR] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_PH_D] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_LIBRE] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_ORIGINAL_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_HOENN_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_SINNOH_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_UNOVA_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_KALOS_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_ALOLA_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_PARTNER_CAP] = sPikachuFormsFormChangeTable,
    [SPECIES_PIKACHU_WORLD_CAP] = sPikachuFormsFormChangeTable,
	[SPECIES_PIKACHU_GIGA] = sPikachuFormChangeTable,
	[SPECIES_MEOWTH] = sMeowthFormChangeTable,
	[SPECIES_MEOWTH_GIGA] = sMeowthFormChangeTable,
	[SPECIES_ALAKAZAM] = sAlakazamFormChangeTable,
	[SPECIES_ALAKAZAM_MEGA] = sAlakazamFormChangeTable,
	[SPECIES_MACHAMP] = sMachampFormChangeTable,
	[SPECIES_MACHAMP_GIGA] = sMachampFormChangeTable,
	[SPECIES_SLOWBRO] = sSlowbroFormChangeTable,
	[SPECIES_SLOWBRO_MEGA] = sSlowbroFormChangeTable,
	[SPECIES_GENGAR] = sGengarFormChangeTable,
	[SPECIES_GENGAR_MEGA] = sGengarFormChangeTable,
	[SPECIES_GENGAR_GIGA] = sGengarFormChangeTable,
	[SPECIES_KINGLER] = sKinglerFormChangeTable,
	[SPECIES_KINGLER_GIGA] = sKinglerFormChangeTable,
	[SPECIES_KANGASKHAN] = sKangaskhanFormChangeTable,
	[SPECIES_KANGASKHAN_MEGA] = sKangaskhanFormChangeTable,
	[SPECIES_PINSIR] = sPinsirFormChangeTable,
	[SPECIES_PINSIR_MEGA] = sPinsirFormChangeTable,
	[SPECIES_GYARADOS] = sGyaradosFormChangeTable,
	[SPECIES_GYARADOS_MEGA] = sGyaradosFormChangeTable,
	[SPECIES_LAPRAS] = sLaprasFormChangeTable,
	[SPECIES_LAPRAS_GIGA] = sLaprasFormChangeTable,
	[SPECIES_EEVEE] = sEeveeFormChangeTable,
	[SPECIES_EEVEE_GIGA] = sEeveeFormChangeTable,
	[SPECIES_AERODACTYL] = sAerodactylFormChangeTable,
	[SPECIES_AERODACTYL_MEGA] = sAerodactylFormChangeTable,
	[SPECIES_SNORLAX] = sSnorlaxFormChangeTable,
	[SPECIES_SNORLAX_GIGA] = sSnorlaxFormChangeTable,
	[SPECIES_MEWTWO] = sMewtwoFormChangeTable,
	[SPECIES_MEWTWO_MEGA_X] = sMewtwoFormChangeTable,
	[SPECIES_MEWTWO_MEGA_Y] = sMewtwoFormChangeTable,
	[SPECIES_AMPHAROS] = sAmpharosFormChangeTable,
	[SPECIES_AMPHAROS_MEGA] = sAmpharosFormChangeTable,
	[SPECIES_UNOWN] = sUnownFormChangeTable,
	[SPECIES_UNOWN_B] = sUnownFormChangeTable,
    [SPECIES_UNOWN_C] = sUnownFormChangeTable,
    [SPECIES_UNOWN_D] = sUnownFormChangeTable,
    [SPECIES_UNOWN_E] = sUnownFormChangeTable,
    [SPECIES_UNOWN_F] = sUnownFormChangeTable,
    [SPECIES_UNOWN_G] = sUnownFormChangeTable,
    [SPECIES_UNOWN_H] = sUnownFormChangeTable,
    [SPECIES_UNOWN_I] = sUnownFormChangeTable,
    [SPECIES_UNOWN_J] = sUnownFormChangeTable,
    [SPECIES_UNOWN_K] = sUnownFormChangeTable,
    [SPECIES_UNOWN_L] = sUnownFormChangeTable,
    [SPECIES_UNOWN_M] = sUnownFormChangeTable,
    [SPECIES_UNOWN_N] = sUnownFormChangeTable,
    [SPECIES_UNOWN_O] = sUnownFormChangeTable,
    [SPECIES_UNOWN_P] = sUnownFormChangeTable,
    [SPECIES_UNOWN_Q] = sUnownFormChangeTable,
    [SPECIES_UNOWN_R] = sUnownFormChangeTable,
    [SPECIES_UNOWN_S] = sUnownFormChangeTable,
    [SPECIES_UNOWN_T] = sUnownFormChangeTable,
    [SPECIES_UNOWN_U] = sUnownFormChangeTable,
    [SPECIES_UNOWN_V] = sUnownFormChangeTable,
    [SPECIES_UNOWN_W] = sUnownFormChangeTable,
    [SPECIES_UNOWN_X] = sUnownFormChangeTable,
    [SPECIES_UNOWN_Y] = sUnownFormChangeTable,
    [SPECIES_UNOWN_Z] = sUnownFormChangeTable,
    [SPECIES_UNOWN_EMARK] = sUnownFormChangeTable,
    [SPECIES_UNOWN_QMARK] = sUnownFormChangeTable,
	[SPECIES_STEELIX] = sSteelixFormChangeTable,
	[SPECIES_STEELIX_MEGA] = sSteelixFormChangeTable,
	[SPECIES_SCIZOR] = sScizorFormChangeTable,
	[SPECIES_SCIZOR_MEGA] = sScizorFormChangeTable,
	[SPECIES_HERACROSS] = sHeracrossFormChangeTable,
	[SPECIES_HERACROSS_MEGA] = sHeracrossFormChangeTable,
	[SPECIES_HOUNDOOM] = sHoundoomFormChangeTable,
	[SPECIES_HOUNDOOM_MEGA] = sHoundoomFormChangeTable,
	[SPECIES_TYRANITAR] = sTyranitarFormChangeTable,
	[SPECIES_TYRANITAR_MEGA] = sTyranitarFormChangeTable,
	[SPECIES_SCEPTILE] = sSceptileFormChangeTable,
	[SPECIES_SCEPTILE_MEGA] = sSceptileFormChangeTable,
	[SPECIES_BLAZIKEN] = sBlazikenFormChangeTable,
	[SPECIES_BLAZIKEN_MEGA] = sBlazikenFormChangeTable,
	[SPECIES_SWAMPERT] = sSwampertFormChangeTable,
	[SPECIES_SWAMPERT_MEGA] = sSwampertFormChangeTable,
	[SPECIES_GARDEVOIR] = sGardevoirFormChangeTable,
	[SPECIES_GARDEVOIR_MEGA] = sGardevoirFormChangeTable,
	[SPECIES_SABLEYE] = sSableyeFormChangeTable,
	[SPECIES_SABLEYE_MEGA] = sSableyeFormChangeTable,
	[SPECIES_MAWILE] = sMawileFormChangeTable,
	[SPECIES_MAWILE_MEGA] = sMawileFormChangeTable,
	[SPECIES_AGGRON] = sAggronFormChangeTable,
	[SPECIES_AGGRON_MEGA] = sAggronFormChangeTable,
	[SPECIES_MEDICHAM] = sMedichamFormChangeTable,
	[SPECIES_MEDICHAM_MEGA] = sMedichamFormChangeTable,
	[SPECIES_MANECTRIC] = sManectricFormChangeTable,
	[SPECIES_MANECTRIC_MEGA] = sManectricFormChangeTable,
	[SPECIES_SHARPEDO] = sSharpedoFormChangeTable,
	[SPECIES_SHARPEDO_MEGA] = sSharpedoFormChangeTable,
	[SPECIES_CAMERUPT] = sCameruptFormChangeTable,
	[SPECIES_CAMERUPT_MEGA] = sCameruptFormChangeTable,
	[SPECIES_ALTARIA] = sAltariaFormChangeTable,
	[SPECIES_ALTARIA_MEGA] = sAltariaFormChangeTable,
	[SPECIES_CASTFORM] = sCastformFormChangeTable,
	[SPECIES_CASTFORM_SUNNY] = sCastformFormChangeTable,
    [SPECIES_CASTFORM_RAINY] = sCastformFormChangeTable,
    [SPECIES_CASTFORM_SNOWY] = sCastformFormChangeTable,
	[SPECIES_BANETTE] = sBanetteFormChangeTable,
	[SPECIES_BANETTE_MEGA] = sBanetteFormChangeTable,
	[SPECIES_ABSOL] = sAbsolFormChangeTable,
	[SPECIES_ABSOL_MEGA] = sAbsolFormChangeTable,
	[SPECIES_GLALIE] = sGlalieFormChangeTable,
	[SPECIES_GLALIE_MEGA] = sGlalieFormChangeTable,
	[SPECIES_SALAMENCE] = sSalamenceFormChangeTable,
	[SPECIES_SALAMENCE_MEGA] = sSalamenceFormChangeTable,
	[SPECIES_METAGROSS] = sMetagrossFormChangeTable,
	[SPECIES_METAGROSS_MEGA] = sMetagrossFormChangeTable,
	[SPECIES_LATIAS] = sLatiasFormChangeTable,
	[SPECIES_LATIAS_MEGA] = sLatiasFormChangeTable,
	[SPECIES_LATIOS] = sLatiosFormChangeTable,
	[SPECIES_LATIOS_MEGA] = sLatiosFormChangeTable,
	[SPECIES_KYOGRE] = sKyogreFormChangeTable,
	[SPECIES_KYOGRE_PRIMAL] = sKyogreFormChangeTable,
	[SPECIES_GROUDON] = sGroudonFormChangeTable,
	[SPECIES_GROUDON_PRIMAL] = sGroudonFormChangeTable,
	[SPECIES_RAYQUAZA] = sRayquazaFormChangeTable,
	[SPECIES_RAYQUAZA_MEGA] = sRayquazaFormChangeTable,
	[SPECIES_DEOXYS] = sDeoxysFormChangeTable,
	[SPECIES_DEOXYS_ATTACK] = sDeoxysFormChangeTable,
	[SPECIES_DEOXYS_DEFENSE] = sDeoxysFormChangeTable,
	[SPECIES_DEOXYS_SPEED] = sDeoxysFormChangeTable,
	[SPECIES_BURMY] = sBurmyFormChangeTable,
	[SPECIES_BURMY_SANDY_CLOAK] = sBurmyFormChangeTable,
    [SPECIES_BURMY_TRASH_CLOAK] = sBurmyFormChangeTable,
	[SPECIES_CHERRIM] = sCherrimFormChangeTable,
	[SPECIES_CHERRIM_SUNSHINE] = sCherrimFormChangeTable,
	[SPECIES_SHELLOS] = sShellosFormChangeTable,
	[SPECIES_SHELLOS_EAST_SEA] = sShellosFormChangeTable,
	[SPECIES_GASTRODON] = sGastrodonFormChangeTable,
	[SPECIES_GASTRODON_EAST_SEA] = sGastrodonFormChangeTable,
	[SPECIES_LOPUNNY] = sLopunnyFormChangeTable,
	[SPECIES_LOPUNNY_MEGA] = sLopunnyFormChangeTable,
	[SPECIES_GARCHOMP] = sGarchompFormChangeTable,
	[SPECIES_GARCHOMP_MEGA] = sGarchompFormChangeTable,
	[SPECIES_LUCARIO] = sLucarioFormChangeTable,
	[SPECIES_LUCARIO_MEGA] = sLucarioFormChangeTable,
	[SPECIES_ABOMASNOW] = sAbomasnowFormChangeTable,
	[SPECIES_ABOMASNOW_MEGA] = sAbomasnowFormChangeTable,
	[SPECIES_GALLADE] = sGalladeFormChangeTable,
	[SPECIES_GALLADE_MEGA] = sGalladeFormChangeTable,
	[SPECIES_ROTOM] = sRotomFormChangeTable,
	[SPECIES_ROTOM_HEAT] = sRotomFormChangeTable,
	[SPECIES_ROTOM_WASH] = sRotomFormChangeTable,
	[SPECIES_ROTOM_FROST] = sRotomFormChangeTable,
	[SPECIES_ROTOM_FAN] = sRotomFormChangeTable,
	[SPECIES_ROTOM_MOW] = sRotomFormChangeTable,
	[SPECIES_DIALGA] = sDialgaFormChangeTable,
	[SPECIES_DIALGA_ORIGIN] = sDialgaFormChangeTable,
    [SPECIES_PALKIA] = sPalkiaFormChangeTable,
	[SPECIES_PALKIA_ORIGIN] = sPalkiaFormChangeTable,
    [SPECIES_GIRATINA] = sGiratinaFormChangeTable,
	[SPECIES_GIRATINA_ORIGIN] = sGiratinaFormChangeTable,
	[SPECIES_SHAYMIN] = sShayminFormChangeTable,
	[SPECIES_SHAYMIN_SKY] = sShayminFormChangeTable,
    [SPECIES_ARCEUS] = sArceusFormChangeTable,
	[SPECIES_ARCEUS_FIGHTING] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_FLYING] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_POISON] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_ROCK] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_GROUND] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_BUG] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_GHOST] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_STEEL] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_FIRE] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_WATER] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_GRASS] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_ELECTRIC] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_PSYCHIC] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_ICE] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_DRAGON] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_DARK] = sArceusFormChangeTable,
    [SPECIES_ARCEUS_FAIRY] = sArceusFormChangeTable,
	[SPECIES_UNFEZANT] = sUnfezantFormChangeTable,
	[SPECIES_UNFEZANT_FEMALE] = sUnfezantFormChangeTable,
	[SPECIES_AUDINO] = sAudinoFormChangeTable,
	[SPECIES_AUDINO_MEGA] = sAudinoFormChangeTable,
	[SPECIES_BASCULIN] = sBasculinFormChangeTable,
	[SPECIES_BASCULIN_BLUE_STRIPED] = sBasculinFormChangeTable,
    [SPECIES_BASCULIN_WHITE_STRIPED] = sBasculinFormChangeTable,
	[SPECIES_DARMANITAN] = sDarmanitanFormChangeTable,
	[SPECIES_DARMANITAN_ZEN_MODE] = sDarmanitanFormChangeTable,
	[SPECIES_DARMANITAN_GALARIAN] = sDarmanitanGalarianFormChangeTable,
	[SPECIES_DARMANITAN_ZEN_MODE_GALARIAN] = sDarmanitanGalarianFormChangeTable,
	[SPECIES_GARBODOR] = sGarbodorFormChangeTable,
	[SPECIES_GARBODOR_GIGA] = sGarbodorFormChangeTable,
	[SPECIES_DEERLING] = sDeerlingFormChangeTable,
	[SPECIES_DEERLING_SUMMER] = sDeerlingFormChangeTable,
    [SPECIES_DEERLING_AUTUMN] = sDeerlingFormChangeTable,
    [SPECIES_DEERLING_WINTER] = sDeerlingFormChangeTable,
	[SPECIES_SAWSBUCK] = sSawsbuckFormChangeTable,
	[SPECIES_SAWSBUCK_SUMMER] = sSawsbuckFormChangeTable,
    [SPECIES_SAWSBUCK_AUTUMN] = sSawsbuckFormChangeTable,
    [SPECIES_SAWSBUCK_WINTER] = sSawsbuckFormChangeTable,
	[SPECIES_FRILLISH] = sFrillishFormChangeTable,
	[SPECIES_FRILLISH_FEMALE] = sFrillishFormChangeTable,
	[SPECIES_JELLICENT] = sJellicentFormChangeTable,
	[SPECIES_JELLICENT_FEMALE] = sJellicentFormChangeTable,
	[SPECIES_TORNADUS] = sTornadusFormChangeTable,
	[SPECIES_TORNADUS_THERIAN] = sTornadusFormChangeTable,
	[SPECIES_THUNDURUS] = sThundurusFormChangeTable,
	[SPECIES_THUNDURUS_THERIAN] = sThundurusFormChangeTable,
	[SPECIES_RESHIRAM] = sReshiramFormChangeTable,
	[SPECIES_ZEKROM] = sZekromFormChangeTable,
	[SPECIES_LANDORUS] = sLandorusFormChangeTable,
	[SPECIES_LANDORUS_THERIAN] = sLandorusFormChangeTable,
	[SPECIES_KYUREM] = sKyuremFormChangeTable,
	[SPECIES_KYUREM_WHITE] = sKyuremFormChangeTable,
	[SPECIES_KYUREM_BLACK] = sKyuremFormChangeTable,
	[SPECIES_KELDEO] = sKeldeoFormChangeTable,
	[SPECIES_KELDEO_RESOLUTE] = sKeldeoFormChangeTable,
    [SPECIES_GENESECT] = sGenesectFormChangeTable,
	[SPECIES_GENESECT_DOUSE_DRIVE] = sGenesectFormChangeTable,
    [SPECIES_GENESECT_SHOCK_DRIVE] = sGenesectFormChangeTable,
    [SPECIES_GENESECT_BURN_DRIVE] = sGenesectFormChangeTable,
    [SPECIES_GENESECT_CHILL_DRIVE] = sGenesectFormChangeTable,
	[SPECIES_VIVILLON] = sVivillonFormChangeTable,
	[SPECIES_VIVILLON_POLAR] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_TUNDRA] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_CONTINENTAL] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_GARDEN] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_ELEGANT] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_MEADOW] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_MODERN] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_MARINE] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_ARCHIPELAGO] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_HIGH_PLAINS] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_SANDSTORM] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_RIVER] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_MONSOON] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_SAVANNA] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_SUN] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_OCEAN] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_JUNGLE] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_FANCY] = sVivillonFormChangeTable,
    [SPECIES_VIVILLON_POKE_BALL] = sVivillonFormChangeTable,
	[SPECIES_PYROAR] = sPyroarFormChangeTable,
	[SPECIES_PYROAR_FEMALE] = sPyroarFormChangeTable,
	[SPECIES_FLABEBE] = sFlabebeFormChangeTable,
	[SPECIES_FLABEBE_YELLOW_FLOWER] = sFlabebeFormChangeTable,
    [SPECIES_FLABEBE_ORANGE_FLOWER] = sFlabebeFormChangeTable,
    [SPECIES_FLABEBE_BLUE_FLOWER] = sFlabebeFormChangeTable,
    [SPECIES_FLABEBE_WHITE_FLOWER] = sFlabebeFormChangeTable,
	[SPECIES_FLOETTE] = sFloetteFormChangeTable,
	[SPECIES_FLOETTE_YELLOW_FLOWER] = sFloetteFormChangeTable,
    [SPECIES_FLOETTE_ORANGE_FLOWER] = sFloetteFormChangeTable,
    [SPECIES_FLOETTE_BLUE_FLOWER] = sFloetteFormChangeTable,
    [SPECIES_FLOETTE_WHITE_FLOWER] = sFloetteFormChangeTable,
	[SPECIES_FLORGES] = sFlorgesFormChangeTable,
	[SPECIES_FLORGES_YELLOW_FLOWER] = sFlorgesFormChangeTable,
    [SPECIES_FLORGES_ORANGE_FLOWER] = sFlorgesFormChangeTable,
    [SPECIES_FLORGES_BLUE_FLOWER] = sFlorgesFormChangeTable,
    [SPECIES_FLORGES_WHITE_FLOWER] = sFlorgesFormChangeTable,
	[SPECIES_FURFROU] = sFurfrouFormChangeTable,
	[SPECIES_FURFROU_HEART_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_STAR_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_DIAMOND_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_DEBUTANTE_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_MATRON_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_DANDY_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_LA_REINE_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_KABUKI_TRIM] = sFurfrouFormChangeTable,
    [SPECIES_FURFROU_PHARAOH_TRIM] = sFurfrouFormChangeTable,
	[SPECIES_MEOWSTIC] = sMeowsticFormChangeTable,
	[SPECIES_MEOWSTIC_FEMALE] = sMeowsticFormChangeTable,
	/*[SPECIES_AEGISLASH] = sAegislashFormChangeTable, handled separated
	[SPECIES_AEGISLASH_BLADE] = sAegislashFormChangeTable,*/
	[SPECIES_XERNEAS] = sXerneasFormChangeTable,
	[SPECIES_XERNEAS_ACTIVE] = sXerneasFormChangeTable,
	[SPECIES_DIANCIE] = sDiancieFormChangeTable,
	[SPECIES_DIANCIE_MEGA] = sDiancieFormChangeTable,
	[SPECIES_HOOPA] = sHoopaFormChangeTable,
	[SPECIES_HOOPA_UNBOUND] = sHoopaFormChangeTable,
	[SPECIES_ORICORIO] = sOricorioFormChangeTable,
	[SPECIES_ORICORIO_POM_POM] = sOricorioFormChangeTable,
    [SPECIES_ORICORIO_PAU] = sOricorioFormChangeTable,
    [SPECIES_ORICORIO_SENSU] = sOricorioFormChangeTable,
	[SPECIES_WISHIWASHI] = sWishiwashiFormChangeTable,
	[SPECIES_WISHIWASHI_SCHOOL] = sWishiwashiFormChangeTable,
    [SPECIES_SILVALLY] = sSilvallyFormChangeTable,
	[SPECIES_SILVALLY_BUG] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_DARK] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_DRAGON] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_ELECTRIC] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_FAIRY] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_FIGHTING] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_FIRE] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_FLYING] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_GHOST] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_GRASS] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_GROUND] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_ICE] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_POISON] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_PSYCHIC] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_ROCK] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_STEEL] = sSilvallyFormChangeTable,
    [SPECIES_SILVALLY_WATER] = sSilvallyFormChangeTable,
	[SPECIES_MINIOR] = sMiniorCoreFormChangeTable,
	[SPECIES_MINIOR_CORE_ORANGE] = sMiniorCoreFormChangeTable,
    [SPECIES_MINIOR_CORE_YELLOW] = sMiniorCoreFormChangeTable,
    [SPECIES_MINIOR_CORE_GREEN] = sMiniorCoreFormChangeTable,
    [SPECIES_MINIOR_CORE_BLUE] = sMiniorCoreFormChangeTable,
    [SPECIES_MINIOR_CORE_INDIGO] = sMiniorCoreFormChangeTable,
    [SPECIES_MINIOR_CORE_VIOLET] = sMiniorCoreFormChangeTable,
	[SPECIES_MINIOR_METEOR] = sMiniorMeteorFormChangeTable,
	[SPECIES_MIMIKYU] = sMimikyuFormChangeTable,
	[SPECIES_MIMIKYU_BUSTED] = sMimikyuFormChangeTable,
	[SPECIES_SOLGALEO] = sSolgaleoFormChangeTable,
	[SPECIES_LUNALA] = sLunalaFormChangeTable,
	[SPECIES_NECROZMA] = sNecrozmaFormChangeTable,
	[SPECIES_NECROZMA_DUSK_MANE] = sNecrozmaFormChangeTable,
	[SPECIES_NECROZMA_DAWN_WINGS] = sNecrozmaFormChangeTable,
	[SPECIES_MELMETAL] = sMelmetalFormChangeTable,
	[SPECIES_MELMETAL_GIGA] = sMelmetalFormChangeTable,
	[SPECIES_RILLABOOM] = sRillaboomFormChangeTable,
	[SPECIES_RILLABOOM_GIGA] = sRillaboomFormChangeTable,
	[SPECIES_CINDERACE] = sCinderaceFormChangeTable,
	[SPECIES_CINDERACE_GIGA] = sCinderaceFormChangeTable,
	[SPECIES_INTELEON] = sInteleonFormChangeTable,
	[SPECIES_INTELEON_GIGA] = sInteleonFormChangeTable,
	[SPECIES_CORVIKNIGHT] = sCorviknightFormChangeTable,
	[SPECIES_CORVIKNIGHT_GIGA] = sCorviknightFormChangeTable,
	[SPECIES_ORBEETLE] = sOrbeetleFormChangeTable,
	[SPECIES_ORBEETLE_GIGA] = sOrbeetleFormChangeTable,
	[SPECIES_DREDNAW] = sDrednawFormChangeTable,
	[SPECIES_DREDNAW_GIGA] = sDrednawFormChangeTable,
	[SPECIES_COALOSSAL] = sCoalossalFormChangeTable,
	[SPECIES_COALOSSAL_GIGA] = sCoalossalFormChangeTable,
	[SPECIES_FLAPPLE] = sFlappleFormChangeTable,
	[SPECIES_FLAPPLE_GIGA] = sFlappleFormChangeTable,
	[SPECIES_APPLETUN] = sAppletunFormChangeTable,
	[SPECIES_APPLETUN_GIGA] = sAppletunFormChangeTable,
	[SPECIES_SANDACONDA] = sSandacondaFormChangeTable,
	[SPECIES_SANDACONDA_GIGA] = sSandacondaFormChangeTable,
	[SPECIES_TOXTRICITY] = sToxtricityAmpedFormChangeTable,
	[SPECIES_TOXTRICITY_GIGA] = sToxtricityAmpedFormChangeTable,
	[SPECIES_TOXTRICITY_LOW_KEY] = sToxtricityLowKeyFormChangeTable,
	[SPECIES_TOXTRICITY_LOW_KEY_GIGA] = sToxtricityLowKeyFormChangeTable,
	[SPECIES_CENTISKORCH] = sCentiskorchFormChangeTable,
	[SPECIES_CENTISKORCH_GIGA] = sCentiskorchFormChangeTable,
	[SPECIES_HATTERENE] = sHattereneFormChangeTable,
	[SPECIES_HATTERENE_GIGA] = sHattereneFormChangeTable,
	[SPECIES_GRIMMSNARL] = sGrimmsnarlFormChangeTable,
	[SPECIES_GRIMMSNARL_GIGA] = sGrimmsnarlFormChangeTable,
	[SPECIES_ALCREMIE] = sAlcremieFormChangeTable,
	[SPECIES_ALCREMIE_RUBY_CREAM] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_MATCHA_CREAM] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_MINT_CREAM] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_LEMON_CREAM] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_SALTED_CREAM] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_RUBY_SWIRL] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_CARAMEL_SWIRL] = sAlcremieFormChangeTable,
    [SPECIES_ALCREMIE_RAINBOW_SWIRL] = sAlcremieFormChangeTable,
	[SPECIES_ALCREMIE_GIGA] = sAlcremieFormChangeTable,
	[SPECIES_EISCUE] = sEiscueFormChangeTable,
	[SPECIES_EISCUE_NOICE_FACE] = sEiscueFormChangeTable,
	[SPECIES_INDEEDEE] = sIndeedeeFormChangeTable,
	[SPECIES_INDEEDEE_FEMALE] = sIndeedeeFormChangeTable,
	/*[SPECIES_MORPEKO] = sMorpekoFormChangeTable, handled separated
	[SPECIES_MORPEKO_HANGRY] = sMorpekoFormChangeTable,*/
	[SPECIES_COPPERAJAH] = sCopperajahFormChangeTable,
	[SPECIES_COPPERAJAH_GIGA] = sCopperajahFormChangeTable,
	[SPECIES_DURALUDON] = sDuraludonFormChangeTable,
	[SPECIES_DURALUDON_GIGA] = sDuraludonFormChangeTable,
	[SPECIES_ZACIAN] = sZacianFormChangeTable,
	[SPECIES_ZACIAN_CROWNED_SWORD] = sZacianFormChangeTable,
	[SPECIES_ZAMAZENTA] = sZamazentaFormChangeTable,
	[SPECIES_ZAMAZENTA_CROWNED_SHIELD] = sZamazentaFormChangeTable,
	[SPECIES_URSHIFU] = sUrshifuSingleStrikeFormChangeTable,
	[SPECIES_URSHIFU_GIGA] = sUrshifuSingleStrikeFormChangeTable,
	[SPECIES_URSHIFU_RAPID_STRIKE_STYLE] = sUrshifuRapidStrikeFormChangeTable,
	[SPECIES_URSHIFU_RAPID_STRIKE_GIGA] = sUrshifuRapidStrikeFormChangeTable,
	[SPECIES_GLASTRIER] = sGlastrierFormChangeTable,
	[SPECIES_SPECTRIER] = sSpectrierFormChangeTable,
	[SPECIES_CALYREX] = sCalyrexFormChangeTable,
	[SPECIES_CALYREX_ICE_RIDER] = sCalyrexFormChangeTable,
	[SPECIES_CALYREX_SHADOW_RIDER] = sCalyrexFormChangeTable,
	[SPECIES_BASCULEGION] = sBasculegionFormChangeTable,
	[SPECIES_BASCULEGION_FEMALE] = sBasculegionFormChangeTable,
	[SPECIES_ENAMORUS] = sEnamorusFormChangeTable,
	[SPECIES_ENAMORUS_THERIAN] = sEnamorusFormChangeTable,
	[SPECIES_OINKOLOGNE] = sOinkologneFormChangeTable,
	[SPECIES_OINKOLOGNE_FEMALE] = sOinkologneFormChangeTable,
	[SPECIES_MAUSHOLD] = sMausholdFormChangeTable,
	[SPECIES_MAUSHOLD_FAMILY_OF_THREE] = sMausholdFormChangeTable,
	[SPECIES_SQUAWKABILLY] = sSquawkabillyFormChangeTable,
	[SPECIES_SQUAWKABILLY_BLUE] = sSquawkabillyFormChangeTable,
	[SPECIES_SQUAWKABILLY_YELLOW] = sSquawkabillyFormChangeTable,
	[SPECIES_SQUAWKABILLY_WHITE] = sSquawkabillyFormChangeTable,
	[SPECIES_PALAFIN] = sPalafinFormChangeTable,
	[SPECIES_PALAFIN_HERO] = sPalafinFormChangeTable,
	[SPECIES_TATSUGIRI] = sTatsugiriFormChangeTable,
	[SPECIES_TATSUGIRI_DROOPY] = sTatsugiriFormChangeTable,
	[SPECIES_TATSUGIRI_STRETCHY] = sTatsugiriFormChangeTable,
};