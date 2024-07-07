SafariZone_SecretHouse_MapScripts:: @ 8162452
	.byte 0

SafariZone_SecretHouse_EventScript_Attendant:: @ 8162453
	lock
	faceplayer
	goto_if_set FLAG_SHARPEDO_RIDE_GET, SafariZone_SecretHouse_EventScript_ExplainSurf
	msgbox SafariZone_SecretHouse_Text_CongratsYouveWon
	setflag FLAG_SHARPEDO_RIDE_GET
SafariZone_SecretHouse_EventScript_ExplainSurf::
	msgbox SafariZone_SecretHouse_Text_ExplainSurf
	release
	end

