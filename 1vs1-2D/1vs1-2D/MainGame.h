#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Player.h"
#include "GameClient.h"
#include "Background.h"

extern "C"
{
#include "lua5.4-include/lauxlib.h"
#include "lua5.4-include/lua.h"
#include "lua5.4-include/lualib.h"
}


class MainGame
{
public:
	MainGame(std::string&, std::string&);
	~MainGame();
	int run();

	bool render_update = true;
	bool dist_view = false;
	Player main_player;
	Player second_player;
	GameClient client;
	Background background;

	std::string ip;
	sf::Font font;
	bool loadfont = false;
private:
	lua_State* L;
	bool lua_enable = false;

	bool lua_update(float&);
	void lua_player_table(Player&);
	std::string f;
};

void render(sf::RenderWindow* window, MainGame* game);
bool check_lua(lua_State* L, int result);
