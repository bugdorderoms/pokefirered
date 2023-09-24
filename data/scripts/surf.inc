EventScript_UseSurf:: @ 81A6AC8
	lockall
	msgbox Text_WantToSurf, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_UseSurfRelease
	msgbox Text_UsedSurf
	callnative TryRemoveStrengthFlag
	setfieldeffectarg 0, 0
	writebytetoaddr RIDE_SHARPEDO, gUsingRideMon
	dofieldeffect FLDEFF_USE_SURF
EventScript_UseSurfRelease:@ 81A6B0B
	releaseall
	end

EventScript_CurrentTooFast:: @ 81A6B0D
	lockall
	msgbox Text_CurrentTooFast
	releaseall
	end
