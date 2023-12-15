.set LOCALID_DOOR_GUARD, 4

IndigoPlateau_PokemonCenter_1F_MapScripts:: @ 816EA5A
	map_script MAP_SCRIPT_ON_TRANSITION, IndigoPlateau_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

IndigoPlateau_PokemonCenter_1F_OnTransition:: @ 816EA65
	setrespawn SPAWN_INDIGO_PLATEAU
	call IndigoPlateau_PokemonCenter_1F_EventScript_CheckBlockDoor
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CheckBlockDoor:: @ 816EA79
	goto_if_set FLAG_SYS_CAN_LINK_WITH_RS, EventScript_Return
	setobjectxyperm LOCALID_DOOR_GUARD, 4, 2
	return

IndigoPlateau_PokemonCenter_1F_EventScript_DoorGuard:: @ 816EA8A
	lock
	faceplayer
	goto IndigoPlateau_PokemonCenter_1F_EventScript_CheckSeviiIslandComplete
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CheckSeviiIslandComplete:: @ 816EAA6
	goto_if_set FLAG_SYS_CAN_LINK_WITH_RS, IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete
	msgbox IndigoPlateau_PokemonCenter_1F_Text_LoreleiIsAbsentClosedForTimeBeing
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete:: @ 816EAB9
	msgbox IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_GymGuy:: @ 816EAC3
	msgbox IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Clerk:: @ 816EACC
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart 0
	msgbox Text_PleaseComeAgain
	release
	end
	
IndigoPlateau_PokemonCenter_1F_EventScript_Nurse:: @ 816EB06
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
