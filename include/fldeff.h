#ifndef GUARD_FLDEFF_H
#define GUARD_FLDEFF_H

#define FLDEFF_CALL_FUNC_IN_DATA() ((void (*)(void))(((u16)gTasks[taskId].data[8] << 16) | (u16)gTasks[taskId].data[9]))();

#define FLDEFF_SET_FUNC_TO_DATA(func)                     \
    gTasks[taskId].data[8] = (u32)func >> 16;             \
    gTasks[taskId].data[9] = (u32)func;

extern struct MapPosition gPlayerFacingPosition;

u32 CreateFieldEffectShowMon(void);

// flash
bool32 MapTransitionIsExit(u32 lightLevel, u32 mapType);
bool32 MapTransitionIsEnter(u32 mapType1, u32 mapType2);
bool32 SetUpFieldMove_Flash(void);
void CB2_DoChangeMap(void);

// cut
bool32 CheckObjectGraphicsInFrontOfPlayer(u32 graphicsId);
u32 FldEff_UseCutOnGrass(void);
u32 FldEff_UseCutOnTree(void);
u32 FldEff_CutGrass(void);
bool32 SetUpFieldMove_Cut(void);

// dig
u32 FldEff_UseDig(void);
bool32 SetUpFieldMove_Dig(void);

// poison
void FldEffPoison_Start(void);
bool32 FldEffPoison_IsActive(void);

// teleport
u32 FldEff_UseTeleport(void);
bool32 SetUpFieldMove_Teleport(void);

// softboiled
bool32 SetUpFieldMove_SoftBoiled(void);
void ChooseMonForSoftboiled(u32 taskId);
void Task_TryUseSoftboiledOnPartyMon(u32 taskId);

// sweetscent
u32 FldEff_SweetScent(void);
bool32 SetUpFieldMove_SweetScent(void);
void StartSweetScentFieldEffect(void);

// defog
bool32 SetUpFieldMove_Defog(void);

#endif // GUARD_FLDEFF_H
