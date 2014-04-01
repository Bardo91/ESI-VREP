-----------------------------------------------------------------------------------------
--		V-REP Client Socket
--			Author: Pablo Ramón Soria.
--			Based on: V-REP code. 
--			Date: 2014-03-31
-----------------------------------------------------------------------------------------


-- Following function writes data to the socket (only single packet data for simplicity sake):
writeSocketData=function(client,data)
	client:send(data)	-- How to concatenate string: str1..str2..str3..str4
end

-- Following function reads data from the socket (only single packet data for simplicity sake):
readSocketData=function(client)
	-- 666 TODO

end

simSetThreadSwitchTiming(200) -- We wanna manually switch for synchronization purpose (and also not to waste processing time!)
simDelegateChildScriptExecution()

simSetThreadAutomaticSwitch(false)
local serverPort = 555;	-- Use this port for example

simSetThreadAutomaticSwitch(true)

local dlgHandle=-1
-- Change Rendering mode.
if (not simGetBooleanParameter(sim_boolparam_threaded_rendering_enabled)) then
	dlgHandle=simDisplayDialog('Threaded rendering',"Click ok to switch to the threaded rendering mode.",sim_dlgstyle_ok,false)
end

-- Create an auxiliary Console to display whatelse.
auxConsole=simAuxiliaryConsoleOpen("Cycle times",50,0)
while (simGetSimulationState()~=sim_simulation_advancing_abouttostop) do
	-- Now build a socket and connect to the server:
	local socket=require("socket")
	local client=socket.tcp()
	simSetThreadIsFree(true) -- To avoid a brief moment where the simulator would appear frozen.
	local result=client:connect('127.0.0.1',serverPort)		--	Connect to a server in a local machine with the chosen port.
	simSetThreadIsFree(false)

	if (result==1) then	-- If connected, then get in, else try until simulation stop.
		-- We could connect to the server
		local theLastTime=simGetSystemTimeInMilliseconds()
		while (simGetSimulationState()~=sim_simulation_advancing_abouttostop) do
			if (dlgHandle~=-1) then
				if (simGetDialogResult(dlgHandle)==sim_dlgret_ok) then
					simSetBooleanParameter(sim_boolparam_threaded_rendering_enabled,true)
					simEndDialog(dlgHandle)
					dlgHandle=-1
				end
			end

			local dt=simGetSystemTimeInMilliseconds(theLastTime)
			simAuxiliaryConsolePrint(auxConsole,"Cycle time (send request + wait for reply + simulate one step): "..dt.." ms\n")
			theLastTime=simGetSystemTimeInMilliseconds()
			
			-- 666 TODO: cambiar de aqui en adelante... Vamos ya queda poco! ;)
			-- Pack the data as a string:
			dataOut = "Step time: "..dt
			-- Send the data:
			writeSocketData(client,dataOut)
			
			-- Now don't waste time in this loop if the simulation time hasn't changed! This also synchronizes this thread with the main script
			simSwitchThread() -- This thread will resume just before the main script is called again
		end
	end
	writeSocketData(client, "QUIT");
	client:close()
end

