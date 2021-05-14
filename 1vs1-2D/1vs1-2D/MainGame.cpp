#include "MainGame.h"
#include<iostream>

MainGame::MainGame()
{
}

void render(sf::RenderWindow* window, MainGame* game)
{
	window->setActive(true);
	window->setVerticalSyncEnabled(false);
	window->setFramerateLimit(0);
	
	while (window->isOpen())
	{
		if (game->render_update)
		{
			window->clear();

			game->main_player.draw((*window), game->assets);
			game->second_player.draw((*window), game->assets);

			window->display();
			game->render_update = false;
		}
	}
}

int MainGame::run()
{
	this->main_player.position = sf::Vector2f(10, 225);
	this->second_player.position = sf::Vector2f(440, 225);
	this->second_player.direction = LEFT;

	sf::RenderWindow window(sf::VideoMode(500,500), "1vs1", sf::Style::Close);
	window.setActive(false);

	std::thread(&render, &window, this).detach();

	sf::Event event;
	sf::Clock clock;
	float deltatime = 0.0f;
	bool serverReady = false;
	bool windowFocus = true;

	std::thread([&]() {
		if (this->client.connect("192.168.1.67"))
		{
			serverReady = true;

			std::thread([&]() {
				while (true)
				{
					if (this->client.listen(&this->second_player,&this->main_player))
					{
						this->render_update = true;
					}
				}
			}).detach();
		}
	}).detach();

	while (window.isOpen())
	{
		deltatime = clock.restart().asSeconds();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if (event.type == sf::Event::GainedFocus)
			{
				windowFocus = true;
			}
			
			if (event.type == sf::Event::LostFocus)
			{
				windowFocus = false;
			}
		}

		
		if (windowFocus && this->main_player.update(deltatime,&second_player))
		{
			this->main_player.auto_collision(this->second_player.position, this->second_player.size);
			if (serverReady)
			{
				this->client.send(this->main_player,this->second_player);
			}
			this->render_update = true;
		}
	}
	return 0;
}

MainGame::~MainGame()
{

}