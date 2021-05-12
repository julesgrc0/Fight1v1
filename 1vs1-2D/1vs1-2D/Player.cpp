#include "Player.h"

Player::Player()
{
	
}

void Player::draw(sf::RenderWindow& window, GameAssets& assets)
{
	sf::RectangleShape rect;
	if (this->direction == LEFT)
	{
		rect.setFillColor(sf::Color::Blue);
	}else
	{
		rect.setFillColor(sf::Color::Red);
	}
	rect.setSize(sf::Vector2f(10, 10));
	rect.setPosition(this->position);
	window.draw(rect);
}

bool Player::update(float deltatime)
{
	bool has_move = false;
	bool up_active = false;
	bool down_active = false;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		up_active = true;
		has_move = true;
		this->position.y -= this->move_speed * deltatime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		down_active = true;
		has_move = true;
		this->position.y += this->move_speed * deltatime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (up_active || down_active)
		{
			this->position.x -= (this->move_speed * deltatime) / 2;
			
			if (up_active)
			{
				this->position.y += (this->move_speed * deltatime)/2;
			}
			else if (down_active)
			{
				this->position.y -= (this->move_speed * deltatime)/2;
			}
		}else
		{
			has_move = true;
			this->position.x -= this->move_speed * deltatime;
		}

		this->direction = LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (up_active || down_active)
		{
			this->position.x += (this->move_speed * deltatime) / 2;

			if (up_active)
			{
				this->position.y += (this->move_speed * deltatime) / 2;
			}
			else if (down_active)
			{
				this->position.y -= (this->move_speed * deltatime) / 2;
			}
		}
		else
		{
			has_move = true;
			this->position.x += this->move_speed * deltatime;
		}
		this->direction = RIGHT;
	}
	
	return has_move;
}

Player::~Player()
{
	
}