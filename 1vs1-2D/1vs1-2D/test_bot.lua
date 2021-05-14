-- test_bot.lua
-- JulesG10

-- enemy last coordinates
local last_x = 0
local last_y = 0

-- update every enemy or while update
function update(move,player,enemy)
	-- get deltatime
	local deltatime = move / PLAYER_SPEED	
	print(tostring(deltatime))

	-- enemy move
	if enemy.x == last_x and enemy.y == last_y then
		
		-- attack
		if player.dist <= HIT_DISTANCE then
			if enemy.life < 50 then
				hit(5)
			else
				hit(10)
			end
		else
			-- move x to enemy
			if enemy.x > player.x + player.size then
				direction(RIGHT)
				player.x += move
			else 
				direction(LEFT)
				player.x -= move
			end

			-- move y to enemy
			if enemy.y > player.y + player.size then
				direction(RIGHT)
				player.y += move
			else 
				direction(LEFT)
				player.y -= move
			end
		end
	else
	-- update coord
		last_x = enemy.x
		last_y = enemy.y
	end
	
	return player
end
