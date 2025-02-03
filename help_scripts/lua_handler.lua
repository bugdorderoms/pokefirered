-- CONFIGURATION --
-- define here the output path for the screenshots, like "C:/Users/"
-- otherwise leave it as "" to output in the same path as this script is
local screenshot_output_path = ""

-- Functions
function takeScreenshot(arg)
	if screenshot_output_path == "" then screenshot_output_path = debug.getinfo(2, "S").source:sub(2):match("(.*/)") end
	
	screenshot_output_path = screenshot_output_path..emu:getGameTitle().." "..os.date("%Y-%m-%d %H-%M-%S")..".png" -- format output file name
	emu:screenshot(screenshot_output_path)
	
	return true -- process successfully executed
end

functions = {
	[0] = takeScreenshot,
	-- and so on...
}

function runLua()
	if emu.memory.iwram:read8(0x03000004) == 1 then
		emu.memory.iwram:write8(0x03000004, 2) -- set state as processing
		if functions[emu.memory.iwram:read8(0x03000005)](emu.memory.iwram:read32(0x03000000)) == true then
			emu.memory.iwram:write8(0x03000004, 3) -- set state as process finished
		else
			emu.memory.iwram:write8(0x03000004, 4) -- set state as process failed
		end
	end
end

callbacks:add("frame", runLua)
console:log("Script loaded!")