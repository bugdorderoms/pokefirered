#ifndef GUARD_LUA_HANDLER_H
#define GUARD_LUA_HANDLER_H

enum
{
	LUA_HANDLER_STATE_NOTHING,
	LUA_HANDLER_STATE_PROCESS,
	LUA_HANDLER_STATE_PROCESSING,
	LUA_HANDLER_STATE_PROCESS_FINISHED,
	LUA_HANDLER_STATE_PROCESS_FAILED,
};

enum
{
	LUA_HANDLER_ACTION_TAKE_SCREENSHOT
};

struct LuaHandler
{
	u32 argument;
	u8 status;
	u8 action;
};

void ProcessLuaAction(u8 action, u32 argument);
u8 GetLuaActionProcessStatus(void);

#endif // GUARD_LUA_HANDLER_H