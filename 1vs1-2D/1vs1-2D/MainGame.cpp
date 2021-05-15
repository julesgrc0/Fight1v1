#include "MainGame.h"

MainGame::MainGame(std::string& input_ip, std::string& lua_file)
{
	if (lua_file.length() > 0)
	{
		this->f = lua_file;

		this->L = luaL_newstate();
		this->lua_enable = check_lua(L,luaL_dofile(L, lua_file.c_str()));

		 lua_close(L);
	}
	this->ip = input_ip;
	if (this->font.loadFromFile("C:/Windows/Fonts/calibri.ttf"))
	{
		loadfont = true;
	}
}

bool check_lua(lua_State* L, int result)
{
	if (result != LUA_OK)
	{
		std::cout << lua_tostring(L ,-1) << std::endl;
		exit(1);
		return false;
	}
	return true;
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

void MainGame::lua_player_table(Player& p)
{
	float dist = sqrt(pow((this->second_player.position.x + this->main_player.size.x / 2) - (this->main_player.position.x + this->main_player.size.x / 2), 2) + pow((this->second_player.position.y + this->main_player.size.x / 2) - (this->main_player.position.y + this->main_player.size.x / 2), 2));
	std::map<std::string, float> player = std::map<std::string, float> {
		{"x",p.position.x},
		{"y",p.position.y},
		{"life",p.life},
		{"size",p.size.x},
		{"distance",dist},
		{"hit",0}
	};

	lua_newtable(L);
	int top = lua_gettop(L);
	for (std::map<std::string, float>::iterator it = player.begin(); it != player.end(); ++it)
	{
		lua_pushstring(L, it->first.c_str());
		lua_pushnumber(L, it->second);
		lua_settable(L, top);
	}
}


bool MainGame::lua_update(float& deltatime)
{
	/* Lua memory bug after function */
	this->L = luaL_newstate();
	luaL_openlibs(L);
	
	luaL_dofile(L, f.c_str());

	lua_pushnumber(L, this->main_player.size.x);
	lua_setglobal(L, "HIT_DISTANCE");

	lua_pushnumber(L, this->main_player.move_speed);
	lua_setglobal(L, "PLAYER_SPEED");
	/**/

	lua_getglobal(L, "update");
	if (lua_isfunction(L, -1))
	{
		lua_pushnumber(L, deltatime * this->main_player.move_speed);
		
		this->lua_player_table(this->main_player);

		this->lua_player_table(this->second_player);

		if (check_lua(L, lua_pcall(L, 3, 1, 0)))
		{
			if (lua_istable(L, -1))
			{
				lua_pushstring(L, "x");
				lua_gettable(L, -2);
				this->main_player.position.x = (float)lua_tonumber(L, -1);
				lua_pop(L, 1);

				lua_pushstring(L, "y");
				lua_gettable(L, -2);
				this->main_player.position.y = (float)lua_tonumber(L, -1);
				lua_pop(L, 1);

				lua_pushstring(L, "life");
				lua_gettable(L, -2);
				this->main_player.life = (float)lua_tonumber(L, -1);
				lua_pop(L, 1);

				lua_pushstring(L, "direction");
				lua_gettable(L, -2);
				this->main_player.direction = (lua_tostring(L, -1) == "LEFT") ? LEFT : RIGHT;
				lua_pop(L, 1);

				lua_pushstring(L, "hit");
				lua_gettable(L, -2);
				this->second_player.life -= (float)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
		}
	}
	
	/**/
	lua_close(L);
	/**/
	return true;
}

int MainGame::run()
{
	this->main_player.position = sf::Vector2f(10, 225);
	this->main_player.last_position = main_player.position;
	this->second_player.position = sf::Vector2f(440, 225);
	second_player.last_position = second_player.position;
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
		
		if (windowFocus && !lua_enable)
		{
			if (this->main_player.update(deltatime, &second_player))
			{
				this->main_player.auto_collision(this->second_player.position, this->second_player.size);
				this->render_update = true;
			}

			if (serverReady)
			{
				this->client.send(this->main_player, this->second_player);
			}
		}
		else if (this->lua_enable)
		{
			if (this->lua_update(deltatime))
			{
				this->main_player.auto_collision(this->second_player.position, this->second_player.size);
				this->render_update = true;
			}

			if (serverReady)
			{
				this->client.send(this->main_player, this->second_player);
			}
		}
	}
	return 0;
}

MainGame::~MainGame()
{
}