Route16_House_MapScripts:: @ 817014E
	.byte 0

Route16_House_EventScript_Woman:: @ 817014F
	lock
	faceplayer
	goto_if_set FLAG_CHARIZARD_RIDE_GET, Route16_House_EventScript_AlreadyGotHM02
	msgbox Route16_House_Text_FoundMySecretRetreat
	setflag FLAG_CHARIZARD_RIDE_GET
	release
	end

Route16_House_EventScript_AlreadyGotHM02:: @ 81701A5
	msgbox Route16_House_Text_ExplainHM02
	release
	end

Route16_House_EventScript_Fearow:: @ 81701AF
	lock
	faceplayer
	waitse
	playmoncry SPECIES_FEAROW, CRY_MODE_NORMAL
	msgbox Route16_House_Text_Fearow
	waitmoncry
	release
	end
