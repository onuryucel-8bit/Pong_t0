local WINDOW_WIDTH = 800
local WINDOW_HEIGHT = 600

local x = 0
local y = 0
local dx, dy = 50, 50
local ballx = 100
local bally = 200

function update(dt)

	y = 0
	x = x + 360 * dt
	
	lu_setPlayer1Pos(x, y)

	

	ballx = ballx + 10 * dt
	lu_setBallPos(ballx, bally)
end