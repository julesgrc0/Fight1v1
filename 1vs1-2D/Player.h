#pragma once
#include<SFML/Graphics.hpp>
#include "GameAssets.h"

enum PlayerSates
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
	PlayerSates state = STATIC;
	PlayerDirection direction = RIGHT;
	float life = 100.0f;

	void draw(sf::RenderWindow&, GameAssets&);
	bool update(float);
private:
	const float move_speed = 200.0f;
};

