-- test_bot.lua
-- JulesG10


function update(move,player,enemy)

	-- get deltatime
	local deltatime = move / PLAYER_SPEED	
	move = deltatime * PLAYER_SPEED/2
	-- enemy move
		if player.distance <= HIT_DISTANCE then
			if enemy.life < 50 then
				player.hit = 5 * deltatime
			else
				player.hit = 10 * deltatime
			end
		else
			-- move x to enemy
			if enemy.x > player.x + player.size then
				player.x = player.x + move
			else 
				player.x = player.x - move
			end

			-- move y to enemy
			if enemy.y > player.y + player.size then
				player.y =  player.y +move
			else 
				player.y = player.y - move
			end
		end
	
	return player
end
