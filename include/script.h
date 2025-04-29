#ifndef GUARD_SCRIPT_H
#define GUARD_SCRIPT_H

#include "global.h"

struct ScriptContext;

typedef bool32 (*ScrCmdFunc)(struct ScriptContext *);
typedef u8 Script[];

struct ScriptContext
{
    u8 stackDepth;
    u8 mode;
    u8 comparisonResult;
    bool32 (*nativePtr)(void);
    const u8 *scriptPtr;
    const u8 *stack[20];
    ScrCmdFunc *cmdTable;
    ScrCmdFunc *cmdTableEnd;
    u32 data[4];
};

#define ScriptReadByte(ctx) (*(ctx->scriptPtr++))

void InitScriptContext(struct ScriptContext *ctx, void *cmdTable, void *cmdTableEnd);
void SetupNativeScript(struct ScriptContext *ctx, bool32 (*ptr)(void));
void StopScript(struct ScriptContext *ctx);
bool32 RunScriptCommand(struct ScriptContext *ctx);
bool32 ScriptPush(struct ScriptContext *ctx, const u8 *ptr);
const u8 *ScriptPop(struct ScriptContext *ctx);
void ScriptJump(struct ScriptContext *ctx, const u8 *ptr);
void ScriptCall(struct ScriptContext *ctx, const u8 *ptr);
void ScriptReturn(struct ScriptContext *ctx);
u16 ScriptReadHalfword(struct ScriptContext *ctx);
u32 ScriptReadWord(struct ScriptContext *ctx);
void ScriptContext2_Enable(void);
void ScriptContext2_Disable(void);
bool32 ScriptContext2_IsEnabled(void);
void ScriptContext1_Init(void);
bool32 ScriptContext1_IsScriptSetUp(void);
bool32 ScriptContext2_RunScript(void);
void ScriptContext1_SetupScript(const u8 *ptr);
void ScriptContext1_Stop(void);
void EnableBothScriptContexts(void);
void ScriptContext2_RunNewScript(const u8 *ptr);
u8 *mapheader_get_tagged_pointer(u8 tag);
void mapheader_run_script_by_tag(u8 tag);
u8 *mapheader_get_first_match_from_tagged_ptr_list(u8 tag);
void RunOnLoadMapScript(void);
void RunOnTransitionMapScript(void);
void RunOnResumeMapScript(void);
void RunOnReturnToFieldMapScript(void);
void RunOnDiveWarpMapScript(void);
bool32 TryRunOnFrameMapScript(void);
void TryRunOnWarpIntoMapScript(void);
u32 CalculateRamScriptChecksum(void);
void ClearRamScript(void);
bool32 InitRamScript(u8 *script, u32 scriptSize, u32 mapGroup, u32 mapNum, u32 objectId);
const u8 *GetRamScript(u32 objectId, const u8 *script);
void ClearMsgBoxCancelableState(void);
void MsgSetSignPost(void);
void MsgSetNotSignPost(void);
bool32 IsMsgSignPost(void);
void ResetFacingNpcOrSignPostVars(void);
bool32 CanWalkAwayToCancelMsgBox(void);
void SetWalkingIntoSignVars(void);
bool32 IsMsgBoxWalkawayDisabled(void);

extern const u8 *gRAMScriptPtr;
extern u8 gWalkAwayFromSignInhibitTimer;

#endif // GUARD_SCRIPT_H
