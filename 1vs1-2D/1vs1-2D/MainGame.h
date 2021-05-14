#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Player.h"
#include "GameClient.h"
#include "Background.h"

class MainGame
{
public:
	MainGame(std::string&);
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
};

