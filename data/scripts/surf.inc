EventScript_UseSurf:: @ 81A6AC8
	goto_if_questlog EventScript_UseSurfEnd
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
EventScript_UseSurfEnd: @ 81A6B0C
	end

EventScript_CurrentTooFast:: @ 81A6B0D
	lockall
	msgbox Text_CurrentTooFast
	releaseall
	end
