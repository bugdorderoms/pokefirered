#include "global.h"
#include "lua_handler.h"

static struct LuaHandler sLuaHandler;

void ProcessLuaAction(u8 action, u32 argument)
{
	if (sLuaHandler.status == LUA_HANDLER_STATE_NOTHING)
	{
		sLuaHandler.status = LUA_HANDLER_STATE_PROCESS;
		sLuaHandler.action = action;
		sLuaHandler.argument = argument;
	}
}

u8 GetLuaActionProcessStatus(void)
{
	u8 status = sLuaHandler.status;
	
	if (status == LUA_HANDLER_STATE_PROCESS_FINISHED || status == LUA_HANDLER_STATE_PROCESS_FAILED)
		sLuaHandler.status = LUA_HANDLER_STATE_NOTHING;

	return status;
}
