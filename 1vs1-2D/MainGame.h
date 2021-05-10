#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Player.h"
#include "GameAssets.h"
#include "HUD.h"
#include "GameClient.h"

class MainGame
{
public:
	MainGame();
	~MainGame();
	int run();

	bool render_update = false;
	Player main_player;
	Player second_player;
	HUD main_hud;
	GameAssets assets;
	GameClient client;
};

