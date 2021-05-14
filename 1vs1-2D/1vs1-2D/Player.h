#pragma once
#include<SFML/Graphics.hpp>
#include "GameAssets.h"

enum PlayerStates
{
	ATTACK,
	STATIC,
	DEFEND,
	HIT,
	DEAD,
};

enum PlayerDirection
{
	RIGHT,
	LEFT
};

class Player
{
public:
	Player();
	~Player();

	sf::Vector2f position;
	PlayerStates state = STATIC;
	PlayerDirection direction = RIGHT;
	float life = 100.0f;

	void draw(sf::RenderWindow&, GameAssets&);
	bool update(float,Player*);
	bool auto_collision(sf::Vector2f, sf::Vector2f);
	bool check_attack(Player& player);
	void go_forward(bool,bool);

	const sf::Vector2f size = sf::Vector2f(50, 50);
private:
	sf::Vector2f last_position;
	float attack_wait = 0.0f;
	float last_delta;

	const float move_speed = 400.0f;
	sf::Texture player_r;
	sf::Texture player_l;
};

