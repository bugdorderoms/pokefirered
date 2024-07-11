enum
{
	INPUT_STATE_SET_IN_UNION_ROOM,
	INPUT_STATE_READ_KEYS,
	INPUT_STATE_RUN_OPTION_CALLBACK,
	INPUT_STATE_PREPARE_FOR_SAVE,
	INPUT_STATE_RUN_SAVE_CALLBACK,
	INPUT_STATE_CLOSE_MENU,
};

enum
{
	CALLBACK_STATE_WAIT_FADE_OUT,
	CALLBACK_STATE_CLOSE_MENU,
	CALLBACK_STATE_SAVE,
};

enum
{
    SAVECB_RETURN_CONTINUE,
    SAVECB_RETURN_OKAY,
    SAVECB_RETURN_CANCEL,
    SAVECB_RETURN_ERROR
};

enum
{
	SAVE_STAT_LOCATION,
	SAVE_STAT_NAME,
	SAVE_STAT_BADGES,
	SAVE_STAT_SEEN,
	SAVE_STAT_CAUGHT,
	SAVE_STAT_DATE,
};

enum
{
	LINK_SAVE_STATE_PRINT_SAVING_TEXT,
	LINK_SAVE_STATE_WARP_PLAYER,
	LINK_SAVE_STATE_WAIT_WRITE_SAVEBLOCK1,
	LINK_SAVE_STATE_START_FADE_OUT,
	LINK_SAVE_STATE_CREATE_LINK_SAVE_TASK,
	LINK_SAVE_STATE_WAIT_LINK_SAVE_TASK,
	LINK_SAVE_STATE_RETURN_TO_FIELD,
};

#define START_MENU_CALLBACK(callback, string) { .text = string, .func = { .u8_void = callback } }

static const struct MenuAction sStartMenuOptions[] =
{
	[START_MENU_DEXNAV]      = START_MENU_CALLBACK(StartMenuDexnavCallback, COMPOUND_STRING("Dexnav")),
	[START_MENU_POKEDEX]     = START_MENU_CALLBACK(StartMenuPokedexCallback, COMPOUND_STRING("Pokedex")),
	[START_MENU_POKEMON]     = START_MENU_CALLBACK(StartMenuPokemonCallback, COMPOUND_STRING("Pokémon")),
	[START_MENU_BAG]         = START_MENU_CALLBACK(StartMenuBagCallback, gStartMenuText_Bag),
	[START_MENU_PLAYER]      = START_MENU_CALLBACK(StartMenuPlayerCallback, NULL), // Name are handled separated
	[START_MENU_PLAYER_LINK] = START_MENU_CALLBACK(StartMenuPlayerLinkCallback, NULL), // Name are handled separated
	[START_MENU_SAVE]        = START_MENU_CALLBACK(StartMenuSaveCallback, COMPOUND_STRING("Save")),
	[START_MENU_OPTIONS]     = START_MENU_CALLBACK(StartMenuOptionCallback, gStartMenuText_Options),
	[START_MENU_EXIT]        = START_MENU_CALLBACK(StartMenuExitCallback, COMPOUND_STRING("Exit")),
	[START_MENU_RETIRE]      = START_MENU_CALLBACK(StartMenuRetireCallback, COMPOUND_STRING("Retire")),
	[START_MENU_RIDE_PAGER]  = START_MENU_CALLBACK(StartMenuRidePagerCallback, COMPOUND_STRING("Ride Pager"))
};

#undef START_MENU_CALLBACK

#define START_MENU_OPTION(option, ...)     \
	{                                      \
		.index = START_MENU_##option,      \
		.flagId = DEFAULT(0, __VA_ARGS__), \
	}

static const struct StartMenuOption sStartMenuOptionsNormal[] =
{
	START_MENU_OPTION(DEXNAV, FLAG_SYS_DEXNAV_GET),
	START_MENU_OPTION(POKEDEX, FLAG_SYS_POKEDEX_GET),
	START_MENU_OPTION(POKEMON, FLAG_SYS_POKEMON_GET),
	START_MENU_OPTION(BAG, FLAG_SYS_BAG_ENABLED),
	START_MENU_OPTION(RIDE_PAGER, FLAG_SYS_RIDE_PAGER_GET),
	START_MENU_OPTION(PLAYER),
	START_MENU_OPTION(SAVE),
	START_MENU_OPTION(OPTIONS),
	START_MENU_OPTION(EXIT),
};

static const struct StartMenuOption sStartMenuOptionsSafari[] =
{
	START_MENU_OPTION(RETIRE),
	START_MENU_OPTION(POKEDEX),
	START_MENU_OPTION(POKEMON),
	START_MENU_OPTION(BAG),
	START_MENU_OPTION(RIDE_PAGER),
	START_MENU_OPTION(PLAYER),
	START_MENU_OPTION(OPTIONS),
	START_MENU_OPTION(EXIT),
};

static const struct StartMenuOption sStartMenuOptionsLink[] =
{
	START_MENU_OPTION(POKEMON),
	START_MENU_OPTION(BAG),
	START_MENU_OPTION(PLAYER_LINK),
	START_MENU_OPTION(OPTIONS),
	START_MENU_OPTION(EXIT),
};

static const struct StartMenuOption sStartMenuOptionsUnionRoom[] =
{
	START_MENU_OPTION(POKEMON),
	START_MENU_OPTION(BAG),
	START_MENU_OPTION(PLAYER),
	START_MENU_OPTION(OPTIONS),
	START_MENU_OPTION(EXIT),
};

#undef START_MENU_OPTION

static const u8 sTextColor_StatName[] = { 1, 2, 3 };
static const u8 sTextColor_StatValue[] = { 1, 4, 5 };
static const u8 sTextColor_LocationHeader[] = { 1, 6, 7 };

static const struct WindowTemplate sSaveStatsWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 11,
    .paletteNum = 13,
    .baseBlock = 0x008
};

static const struct BgTemplate sBGTemplates_AfterLinkSaveMessage[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }
};

static const struct WindowTemplate sWindowTemplates_AfterLinkSaveMessage[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x198
    }, DUMMY_WIN_TEMPLATE
};
