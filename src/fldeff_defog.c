#include "global.h"
#include "event_data.h"
#include "event_scripts.h"
#include "field_effect.h"
#include "field_weather.h"
#include "field_fadetransition.h"
#include "fldeff.h"
#include "party_menu.h"
#include "script.h"
#include "constants/weather.h"

static void FieldCallback_Defog(void);
static void FldEff_UseDefog(void);

bool8 SetUpFieldMove_Defog(void)
{
	u8 weather = GetCurrentWeather();
	
	if (weather == WEATHER_FOG_HORIZONTAL || weather == WEATHER_FOG_DIAGONAL)
	{
		gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
		gPostMenuFieldCallback = FieldCallback_Defog;
		return TRUE;
	}
	return FALSE;
}

static void FieldCallback_Defog(void)
{
	u8 taskId = CreateFieldEffectShowMon();
	gSpecialVar_Result = gFieldEffectArguments[0] = GetCursorSelectionMonId();
	FLDEFF_SET_FUNC_TO_DATA(FldEff_UseDefog);
}

static void FldEff_UseDefog(void)
{
	ScriptContext1_SetupScript(EventScript_FldEffDefog);
}
