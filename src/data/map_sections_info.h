#define CHAMBER_MAPSEC_INFO(mapSecName)                                              \
    {                                                                                \
		.mapPreview =                                                                \
		{                                                                            \
			.type = MAP_PREVIEW_TYPE_CAVE,                                           \
			.tilesptr = sMoneanChamberMapPreviewTiles,                               \
			.tilemapptr = sMoneanChamberMapPreviewTilemap,                           \
			.palptr = sMoneanChamberMapPreviewPalette,                               \
			.worldMapFlag = FLAG_WORLD_MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER, \
		},                                                                           \
		.region = REGION_KANTO,                                                      \
		.name = sMapsecName_##mapSecName,                                            \
    }

const struct MapSectionInfo gMapSectionsInfo[MAPSEC_COUNT] =
{
	// Hoenn (Unused)
	[MAPSEC_LITTLEROOT_TOWN] =
    {
		.region = REGION_HOENN,
    },
    [MAPSEC_OLDALE_TOWN] =
    {
		.region = REGION_HOENN,
    },
    [MAPSEC_DEWFORD_TOWN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_LAVARIDGE_TOWN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_FALLARBOR_TOWN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_VERDANTURF_TOWN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_PACIFIDLOG_TOWN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_PETALBURG_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SLATEPORT_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_MAUVILLE_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_RUSTBORO_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_FORTREE_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_LILYCOVE_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_MOSSDEEP_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SOOTOPOLIS_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_EVER_GRANDE_CITY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_101] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_102] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_103] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_104] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_105] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_106] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_107] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_108] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_109] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_110] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_111] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_112] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_113] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_114] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_115] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_116] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_117] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_118] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_119] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_120] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_121] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_122] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_123] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_124] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_125] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_126] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_127] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_128] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_129] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_130] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_131] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_132] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_133] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ROUTE_134] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_124] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_125] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_126] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_127] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_SOOTOPOLIS] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_GRANITE_CAVE] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_MT_CHIMNEY] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SAFARI_ZONE] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_BATTLE_FRONTIER] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_PETALBURG_WOODS] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_RUSTURF_TUNNEL] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ABANDONED_SHIP] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_NEW_MAUVILLE] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_METEOR_FALLS] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_METEOR_FALLS2] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_MT_PYRE] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_AQUA_HIDEOUT_OLD] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SHOAL_CAVE] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SEAFLOOR_CAVERN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_128] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_VICTORY_ROAD] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_MIRAGE_ISLAND] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_CAVE_OF_ORIGIN] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SOUTHERN_ISLAND] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_FIERY_PATH] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_FIERY_PATH2] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_JAGGED_PASS] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_JAGGED_PASS2] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SEALED_CHAMBER] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_UNDERWATER_SEALED_CHAMBER] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SCORCHED_SLAB] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ISLAND_CAVE] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_DESERT_RUINS] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_ANCIENT_TOMB] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_INSIDE_OF_TRUCK] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SKY_PILLAR] =
    {
        .region = REGION_HOENN,
    },
    [MAPSEC_SECRET_BASE] =
    {
        .region = REGION_HOENN,
    },
	
	// Kanto
	[MAPSEC_PALLET_TOWN] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_PALLETTOWN,
	},
    [MAPSEC_VIRIDIAN_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_VIRIDIANCITY,
	},
    [MAPSEC_PEWTER_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_PEWTERCITY,
	},
    [MAPSEC_CERULEAN_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_CERULEANCITY,
	},
    [MAPSEC_LAVENDER_TOWN] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_LAVENDERTOWN,
	},
    [MAPSEC_VERMILION_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_VERMILIONCITY,
	},
    [MAPSEC_CELADON_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_CELADONCITY,
	},
    [MAPSEC_FUCHSIA_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_FUCHSIACITY,
	},
    [MAPSEC_CINNABAR_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_CINNABARISLAND,
	},
    [MAPSEC_INDIGO_PLATEAU] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_INDIGOPLATEAU,
	},
    [MAPSEC_SAFFRON_CITY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SAFFRONCITY,
	},
    [MAPSEC_ROUTE_4_POKECENTER] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE4_Clone,
	},
    [MAPSEC_ROUTE_10_POKECENTER] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE10_Clone,
	},
    [MAPSEC_ROUTE_1] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE1,
	},
    [MAPSEC_ROUTE_2] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE2,
	},
    [MAPSEC_ROUTE_3] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE3,
	},
    [MAPSEC_ROUTE_4] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE4,
	},
    [MAPSEC_ROUTE_5] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE5,
	},
    [MAPSEC_ROUTE_6] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE6,
	},
    [MAPSEC_ROUTE_7] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE7,
	},
    [MAPSEC_ROUTE_8] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE8,
	},
    [MAPSEC_ROUTE_9] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE9,
	},
    [MAPSEC_ROUTE_10] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE10,
	},
    [MAPSEC_ROUTE_11] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE11,
	},
    [MAPSEC_ROUTE_12] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE12,
	},
    [MAPSEC_ROUTE_13] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE13,
	},
    [MAPSEC_ROUTE_14] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE14,
	},
    [MAPSEC_ROUTE_15] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE15,
	},
    [MAPSEC_ROUTE_16] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE16,
	},
    [MAPSEC_ROUTE_17] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE17,
	},
    [MAPSEC_ROUTE_18] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE18,
	},
    [MAPSEC_ROUTE_19] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE19,
	},
    [MAPSEC_ROUTE_20] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE20,
	},
    [MAPSEC_ROUTE_21] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE21,
	},
    [MAPSEC_ROUTE_22] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE22,
	},
    [MAPSEC_ROUTE_23] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE23,
	},
    [MAPSEC_ROUTE_24] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE24,
	},
    [MAPSEC_ROUTE_25] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ROUTE25,
	},
    [MAPSEC_VIRIDIAN_FOREST] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sViridianForestMapPreviewTiles,
			.tilemapptr = sViridianForestMapPreviewTilemap,
			.palptr = sViridianForestMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_VIRIDIAN_FOREST,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_VIRIDIANFOREST,
		.desc = COMPOUND_STRING("A deep and sprawling forest that\nextends around Viridian City.\nA natural maze, many people\nbecome lost inside."),
	},
    [MAPSEC_MT_MOON] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sMtMoonMapPreviewTiles,
			.tilemapptr = sMtMoonMapPreviewTilemap,
			.palptr = sMtMoonMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_MT_MOON_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_MTMOON,
		.desc = COMPOUND_STRING("A mystical mountain that is known\nfor its frequent meteor falls.\nThe shards of stars that fall\nhere are known as Moon Stones."),
	},
    [MAPSEC_S_S_ANNE] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SSANNE,
	},
    [MAPSEC_UNDERGROUND_PATH] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_UNDERGROUNDPATH,
	},
    [MAPSEC_UNDERGROUND_PATH_2] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_UNDERGROUNDPATH_Clone,
	},
    [MAPSEC_DIGLETTS_CAVE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sDiglettsCaveMapPreviewTiles,
			.tilemapptr = sDiglettsCaveMapPreviewTilemap,
			.palptr = sDiglettsCaveMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_DIGLETTS_CAVE_B1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_DIGLETTSCAVE,
		.desc = COMPOUND_STRING("A seemingly plain tunnel that was\ndug by wild Diglett.\nIt is famous for connecting\nRoutes 2 and 11."),
	},
    [MAPSEC_KANTO_VICTORY_ROAD] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sKantoVictoryRoadMapPreviewTiles,
			.tilemapptr = sKantoVictoryRoadMapPreviewTilemap,
			.palptr = sKantoVictoryRoadMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_VICTORY_ROAD_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_VICTORYROAD,
		.desc = COMPOUND_STRING("A tunnel situated on Route 23.\nIt earned its name because it\nmust be traveled by all Trainers\naiming for the top."),
	},
    [MAPSEC_ROCKET_HIDEOUT] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sRocketHideoutMapPreviewTiles,
			.tilemapptr = sRocketHideoutMapPreviewTilemap,
			.palptr = sRocketHideoutMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_ROCKET_HIDEOUT_B1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_ROCKETHIDEOUT,
	},
    [MAPSEC_SILPH_CO] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sSilphCoMapPreviewTiles,
			.tilemapptr = sSilphCoMapPreviewTilemap,
			.palptr = sSilphCoMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_SILPH_CO_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_SILPHCO,
	},
    [MAPSEC_POKEMON_MANSION] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sPokemonMansionMapPreviewTiles,
			.tilemapptr = sPokemonMansionMapPreviewTilemap,
			.palptr = sPokemonMansionMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_POKEMON_MANSION_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_POKMONMANSION,
		.desc = COMPOUND_STRING("A decrepit, burned-down mansion\non Cinnabar Island.\nIt got its name because a famous\nPokémon researcher lived there."),
	},
    [MAPSEC_KANTO_SAFARI_ZONE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sKantoSafariZoneMapPreviewTiles,
			.tilemapptr = sKantoSafariZoneMapPreviewTilemap,
			.palptr = sKantoSafariZoneMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_SAFARI_ZONE_CENTER,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_SAFARIZONE,
		.desc = COMPOUND_STRING("An amusement park outside Fuchsia\nCity where many rare Pokémon can\nbe observed in the wild.\nCatch them in a popular game!"),
	},
    [MAPSEC_POKEMON_LEAGUE] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_POKMONLEAGUE,
	},
    [MAPSEC_ROCK_TUNNEL] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sRockTunnelMapPreviewTiles,
			.tilemapptr = sRockTunnelMapPreviewTilemap,
			.palptr = sRockTunnelMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_ROCK_TUNNEL_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_ROCKTUNNEL,
		.desc = COMPOUND_STRING("A naturally formed underground\ntunnel. Because it has not been\ndeveloped, it is inky dark inside.\nA light is needed to get through."),
	},
    [MAPSEC_SEAFOAM_ISLANDS] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sSeafoamIslandsMapPreviewTiles,
			.tilemapptr = sSeafoamIslandsMapPreviewTilemap,
			.palptr = sSeafoamIslandsMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_SEAFOAM_ISLANDS_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_SEAFOAMISLANDS,
		.desc = COMPOUND_STRING("A pair of islands that is situated\non Route 20.\nThe two islands are shaped the\nsame, as if they were twins."),
	},
    [MAPSEC_POKEMON_TOWER] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sPokemonTowerMapPreviewTiles,
			.tilemapptr = sPokemonTowerMapPreviewTilemap,
			.palptr = sPokemonTowerMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_POKEMON_TOWER_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_POKMONTOWER,
		.desc = COMPOUND_STRING("A tower that houses the graves of\ncountless Pokémon.\nMany people visit it daily to pay\ntheir respects to the fallen."),
	},
    [MAPSEC_CERULEAN_CAVE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sCeruleanCaveMapPreviewTiles,
			.tilemapptr = sCeruleanCaveMapPreviewTilemap,
			.palptr = sCeruleanCaveMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_CERULEAN_CAVE_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_CERULEANCAVE,
		.desc = COMPOUND_STRING("A mysterious cave that is filled\nwith terribly tough Pokémon.\nIt is so dangerous, the Pokémon\nLeague is in charge of it."),
	},
    [MAPSEC_POWER_PLANT] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sPowerPlantMapPreviewTiles,
			.tilemapptr = sPowerPlantMapPreviewTilemap,
			.palptr = sPowerPlantMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_POWER_PLANT,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_POWERPLANT,
		.desc = COMPOUND_STRING("A power plant that was abandoned\nyears ago, though some of the\nmachines still work. It is infested\nwith electric Pokémon."),
	},
    [MAPSEC_ONE_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_ONEISLAND,
	},
    [MAPSEC_TWO_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_TWOISLAND,
	},
    [MAPSEC_THREE_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_THREEISLAND,
	},
    [MAPSEC_FOUR_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_FOURISLAND,
	},
    [MAPSEC_FIVE_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_FIVEISLAND,
	},
    [MAPSEC_SEVEN_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SEVENISLAND,
	},
    [MAPSEC_SIX_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SIXISLAND,
	},
    [MAPSEC_KINDLE_ROAD] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_KINDLEROAD,
	},
    [MAPSEC_TREASURE_BEACH] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_TREASUREBEACH,
	},
    [MAPSEC_CAPE_BRINK] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_CAPEBRINK,
	},
    [MAPSEC_BOND_BRIDGE] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_BONDBRIDGE,
	},
    [MAPSEC_THREE_ISLE_PORT] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_THREEISLEPORT,
	},
    [MAPSEC_SEVII_ISLE_6] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE6,
	},
    [MAPSEC_SEVII_ISLE_7] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE7,
	},
    [MAPSEC_SEVII_ISLE_8] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE8,
	},
    [MAPSEC_SEVII_ISLE_9] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE9,
	},
    [MAPSEC_RESORT_GORGEOUS] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_RESORTGORGEOUS,
	},
    [MAPSEC_WATER_LABYRINTH] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_WATERLABYRINTH,
    },
    [MAPSEC_FIVE_ISLE_MEADOW] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_FIVEISLEMEADOW,
    },
    [MAPSEC_MEMORIAL_PILLAR] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_MEMORIALPILLAR,
    },
    [MAPSEC_OUTCAST_ISLAND] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_OUTCASTISLAND,
    },
    [MAPSEC_GREEN_PATH] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_GREENPATH,
    },
    [MAPSEC_WATER_PATH] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_WATERPATH,
    },
    [MAPSEC_RUIN_VALLEY] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_RUINVALLEY,
    },
    [MAPSEC_TRAINER_TOWER] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_TRAINERTOWER,
    },
    [MAPSEC_CANYON_ENTRANCE] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_CANYONENTRANCE,
    },
    [MAPSEC_SEVAULT_CANYON] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_SEVAULTCANYON,
    },
    [MAPSEC_TANOBY_RUINS] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_TANOBYRUINS,
    },
    [MAPSEC_SEVII_ISLE_22] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE22,
    },
    [MAPSEC_SEVII_ISLE_23] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE23,
    },
    [MAPSEC_SEVII_ISLE_24] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_SEVIIISLE24,
    },
    [MAPSEC_NAVEL_ROCK] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_NAVELROCK,
    },
    [MAPSEC_MT_EMBER] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sMtEmberMapPreviewTiles,
			.tilemapptr = sMtEmberMapPreviewTilemap,
			.palptr = sMtEmberMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_MT_EMBER_EXTERIOR,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_MTEMBER,
		.desc = COMPOUND_STRING("Supposedly an inactive volcano.\nHowever, there are persistent\nreports that the peak blazes\nwith fire at night."),
	},
    [MAPSEC_BERRY_FOREST] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sBerryForestMapPreviewTiles,
			.tilemapptr = sBerryForestMapPreviewTilemap,
			.palptr = sBerryForestMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_THREE_ISLAND_BERRY_FOREST,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_BERRYFOREST,
		.desc = COMPOUND_STRING("A forest on a small islet off the\ncoast of Three Island. Berries\ngrow wildly in profusion, quickly\nreplenishing those that fall off."),
	},
    [MAPSEC_ICEFALL_CAVE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sIcefallCaveMapPreviewTiles,
			.tilemapptr = sIcefallCaveMapPreviewTilemap,
			.palptr = sIcefallCaveMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_FOUR_ISLAND_ICEFALL_CAVE_ENTRANCE,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_ICEFALLCAVE,
		.desc = COMPOUND_STRING("A cave which is covered by water\nand ice on Four Island.\nIt seems like the end of the cave\nis connected to the ocean."),
	},
    [MAPSEC_ROCKET_WAREHOUSE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sRocketWarehouseMapPreviewTiles,
			.tilemapptr = sRocketWarehouseMapPreviewTilemap,
			.palptr = sRocketWarehouseMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_THREE_ISLAND_BERRY_FOREST,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_ROCKETWAREHOUSE,
	},
    [MAPSEC_TRAINER_TOWER_2] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_TRAINERTOWER_Clone,
    },
    [MAPSEC_DOTTED_HOLE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sDottedHoleMapPreviewTiles,
			.tilemapptr = sDottedHoleMapPreviewTilemap,
			.palptr = sDottedHoleMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_SIX_ISLAND_DOTTED_HOLE_1F,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_DOTTEDHOLE,
		.desc = COMPOUND_STRING("A mysterious, just-discovered\nruin from an ancient time.\nIt got its name from the six dots\non its door."),
	},
    [MAPSEC_LOST_CAVE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sLostCaveMapPreviewTiles,
			.tilemapptr = sLostCaveMapPreviewTilemap,
			.palptr = sLostCaveMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_FIVE_ISLAND_LOST_CAVE_ENTRANCE,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_LOSTCAVE,
		.desc = COMPOUND_STRING("A bewildering cave off the coast\nof Resort Gorgeous.\nSome curious thrill seekers have\nnever emerged from it."),
	},
    [MAPSEC_PATTERN_BUSH] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_FOREST,
			.tilesptr = sViridianForestMapPreviewTiles,
			.tilemapptr = sViridianForestMapPreviewTilemap,
			.palptr = sViridianForestMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_SIX_ISLAND_PATTERN_BUSH,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_PATTERNBUSH,
		.desc = COMPOUND_STRING("A lush and bush-like area. \nIn it are patterns where no grass\ngrows. Some study it in the belief\nthat a secret is concealed."),
	},
    [MAPSEC_ALTERING_CAVE] =
	{
		.mapPreview =
		{
			.type = MAP_PREVIEW_TYPE_CAVE,
			.tilesptr = sAlteringCaveMapPreviewTiles,
			.tilemapptr = sAlteringCaveMapPreviewTilemap,
			.palptr = sAlteringCaveMapPreviewPalette,
			.worldMapFlag = FLAG_WORLD_MAP_SIX_ISLAND_ALTERING_CAVE,
		},
		.region = REGION_KANTO,
		.name = sMapsecName_ALTERINGCAVE,
		.desc = COMPOUND_STRING("This island has been known by this\nname since the distant past.\nNo one today knows where it got\nthis name, however."),
	},
    [MAPSEC_TANOBY_CHAMBERS] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_TANOBYCHAMBERS,
		.desc = COMPOUND_STRING("An ancient ruin that is rumored to\nbe home to a peculiar Pokémon.\nHowever, so far, the Pokémon\nremains an unconfirmed rumor."),
	},
    [MAPSEC_THREE_ISLE_PATH] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_THREEISLEPATH,
	},
    [MAPSEC_TANOBY_KEY] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_TANOBYKEY,
	},
    [MAPSEC_BIRTH_ISLAND] =
	{
		.region = REGION_KANTO,
		.name = sMapsecName_BIRTHISLAND,
	},
    [MAPSEC_MONEAN_CHAMBER] = CHAMBER_MAPSEC_INFO(MONEANCHAMBER),
	[MAPSEC_LIPTOO_CHAMBER] = CHAMBER_MAPSEC_INFO(LIPTOOCHAMBER),
	[MAPSEC_WEEPTH_CHAMBER] = CHAMBER_MAPSEC_INFO(WEEPTHCHAMBER),
	[MAPSEC_DILFORD_CHAMBER] = CHAMBER_MAPSEC_INFO(DILFORDCHAMBER),
	[MAPSEC_SCUFIB_CHAMBER] = CHAMBER_MAPSEC_INFO(SCUFIBCHAMBER),
	[MAPSEC_RIXY_CHAMBER] = CHAMBER_MAPSEC_INFO(RIXYCHAMBER),
	[MAPSEC_VIAPOIS_CHAMBER] = CHAMBER_MAPSEC_INFO(VIAPOISCHAMBER),
	
    [MAPSEC_EMBER_SPA] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_EMBERSPA,
    },
    [MAPSEC_SPECIAL_AREA] =
    {
		.region = REGION_KANTO,
		.name = sMapsecName_CELADONDEPT,
    },
};
