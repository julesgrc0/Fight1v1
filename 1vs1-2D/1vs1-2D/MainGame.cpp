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
	sf::RenderWindow window(sf::VideoMode(500,500), "1v1", sf::Style::Close);
	window.setActive(false);

	std::thread(&render, &window, this).detach();

	sf::Event event;
	sf::Clock clock;
	float deltatime = 0.0f;
	bool serverReady = false;

	std::thread([&]() {
		if (this->client.connect("192.168.1.67"))
		{
			serverReady = true;

			std::thread([&]() {
				while (true)
				{
					if (this->client.listen(&this->second_player))
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
		}

		if (this->main_player.update(deltatime))
		{
			if (serverReady)
			{
				this->client.send(this->main_player);
			}
			this->render_update = true;
		}
	}
	return 0;
}

MainGame::~MainGame()
{

}