#ifndef GUARD_FIELD_WEATHER_EFFECTS_H
#define GUARD_FIELD_WEATHER_EFFECTS_H

void Rain_InitAll(void);
void Rain_InitVars(void);
void Rain_Main(void);
bool8 Rain_Finish(void);

void Snow_InitAll(void);
void Snow_InitVars(void);
void Snow_Main(void);
bool8 Snow_Finish(void);

void Thunderstorm_InitAll(void);
void Thunderstorm_InitVars(void);
void Thunderstorm_Main(void);
bool8 Thunderstorm_Finish(void);

void FogHorizontal_InitAll(void);
void FogHorizontal_InitVars(void);
void FogHorizontal_Main(void);
bool8 FogHorizontal_Finish(void);

void Ash_InitAll(void);
void Ash_InitVars(void);
void Ash_Main(void);
bool8 Ash_Finish(void);

void Sandstorm_InitAll(void);
void Sandstorm_InitVars(void);
void Sandstorm_Main(void);
bool8 Sandstorm_Finish(void);

void FogDiagonal_InitAll(void);
void FogDiagonal_InitVars(void);
void FogDiagonal_Main(void);
bool8 FogDiagonal_Finish(void);

void Shade_InitAll(void);
void Shade_InitVars(void);
void Shade_Main(void);
bool8 Shade_Finish(void);

void Drought_InitAll(void);
void Drought_InitVars(void);
void Drought_Main(void);
bool8 Drought_Finish(void);

void Downpour_InitVars(void);
void Downpour_InitAll(void);

void Bubbles_InitAll(void);
void Bubbles_InitVars(void);
void Bubbles_Main(void);
bool8 Bubbles_Finish(void);

#endif //GUARD_FIELD_WEATHER_EFFECTS_H
