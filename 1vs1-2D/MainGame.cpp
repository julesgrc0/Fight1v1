#include "MainGame.h"

MainGame::MainGame()
{
	this->render_update = true;
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
			this->client.send_update(this->main_player);
			this->render_update = true;
		}

		if (this->client.listen_update(&this->second_player))
		{
			this->render_update = true;
		}
	}
	return 0;
}

MainGame::~MainGame()
{

}