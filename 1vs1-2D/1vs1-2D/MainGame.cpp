#include "MainGame.h"

MainGame::MainGame(std::string& input_ip)
{
	this->ip = input_ip;
	if (this->font.loadFromFile("C:/Windows/Fonts/calibri.ttf"))
	{
		loadfont = true;
	}
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

			game->background.draw((*window));

			if (game->dist_view)
			{
				if (game->loadfont)
				{
					sf::Text text;
					text.setPosition(sf::Vector2f(game->main_player.position.x, game->main_player.position.y - 40));
					text.setFont(game->font);
					text.setCharacterSize(12);

					float dist = sqrt(pow((game->second_player.position.x + game->main_player.size.x / 2) - (game->main_player.position.x + game->main_player.size.x / 2), 2) + pow((game->second_player.position.y + game->main_player.size.x / 2) - (game->main_player.position.y + game->main_player.size.x / 2), 2));
					text.setString(std::to_string(dist));


					if (dist < game->main_player.size.x + (game->main_player.size.y / 4))
					{
						text.setFillColor(sf::Color::Red);
					}
					else
					{
						text.setFillColor(sf::Color::Black);
					}
					window->draw(text);

					text.setString(std::to_string((int)game->second_player.life)+"%");
					text.setFillColor(sf::Color::Black);
					text.setPosition(sf::Vector2f(game->second_player.position.x, game->second_player.position.y - 40));

					window->draw(text);
				}

				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(game->main_player.position.x + game->main_player.size.x / 2, game->main_player.position.y + game->main_player.size.y / 2),sf::Color::Black),
					sf::Vertex(sf::Vector2f(game->second_player.position.x + game->second_player.size.x / 2, game->second_player.position.y + game->second_player.size.y / 2),sf::Color::Black)
				};
				window->draw(line, 2, sf::Lines);
			}

			game->second_player.draw((*window));
			game->main_player.draw((*window));

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
		if (this->client.connect(this->ip.c_str()))
		{
			serverReady = true;

			std::thread([&]() {
				while (true)
				{
					int last_life = this->main_player.life;
					if (this->client.listen(&this->second_player,&this->main_player))
					{
						if (last_life != this->main_player.life)
						{
							this->main_player.hit_animation();
						}
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

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right)
			{
				dist_view = !dist_view;
				this->render_update = true;
			}
		}
		
		if (this->main_player.hit_update(deltatime))
		{
			this->render_update = true;
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