Route12_MapScripts:: @ 8168000
	map_script MAP_SCRIPT_ON_RESUME, Route12_OnResume
	.byte 0

Route12_OnResume:: @ 8168006
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, Route12_EventScript_TryRemoveSnorlax
	end

Route12_EventScript_TryRemoveSnorlax:: @ 8168010
	removeobject VAR_LAST_TALKED
	return

Route12_EventScript_Snorlax:: @ 8168014
	lock
	faceplayer
	goto_if_unset FLAG_GOT_POKE_FLUTE, Route12_EventScript_SnorlaxNoPokeFlute
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Route12_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
	setwildbattle SPECIES_SNORLAX, 30, ITEM_NONE
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_12_SNORLAX
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	setflag FLAG_WOKE_UP_ROUTE_12_SNORLAX
	dowildbattle
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Route12_EventScript_FoughtSnorlax
	compare VAR_RESULT, B_OUTCOME_RAN
	goto_if_eq Route12_EventScript_FoughtSnorlax
	compare VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED
	goto_if_eq Route12_EventScript_FoughtSnorlax
	release
	end

Route12_EventScript_DontUsePokeFlute:: @ 816808D
	release
	end

Route12_EventScript_FoughtSnorlax:: @ 816808F
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route12_EventScript_SnorlaxNoPokeFlute:: @ 8168099
	msgbox Route12_Text_MonSprawledOutInSlumber
	release
	end

Route12_EventScript_RouteSign:: @ 81680A3
	msgbox Route12_Text_RouteSign, MSGBOX_SIGN
	end

Route12_EventScript_FishingSign:: @ 81680AC
	msgbox Route12_Text_SportfishingArea, MSGBOX_SIGN
	end
