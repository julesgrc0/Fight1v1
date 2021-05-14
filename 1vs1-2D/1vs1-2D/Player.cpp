#include "Player.h"
#include<iostream>

Player::Player()
{
	this->player_l.loadFromFile("C:/Users/jules/source/repos/1vs1-2D/1vs1-2D/x64/Debug/attack_l.png", sf::IntRect(0, 0, 100, 100));
	this->player_r.loadFromFile("C:/Users/jules/source/repos/1vs1-2D/1vs1-2D/x64/Debug/attack_r.png", sf::IntRect(0, 0, 100, 100));
}

void Player::draw(sf::RenderWindow& window, GameAssets& assets)
{
	sf::Sprite sprite;
	sprite.setPosition(this->position);

	if (this->direction == RIGHT)
	{
		sprite.setTexture(this->player_l);
	}
	else
	{
		sprite.setTexture(this->player_r);
	}

	sprite.setScale(sf::Vector2f(0.5, 0.5));
	window.draw(sprite);
	
	int y_life = 20;

	sf::RectangleShape lifeRect;
	lifeRect.setSize(sf::Vector2f(this->size.x, 8));
	lifeRect.setPosition(sf::Vector2f(this->position.x, this->position.y - y_life));
	lifeRect.setOutlineColor(sf::Color::Black);
	lifeRect.setOutlineThickness(1.0f);
	lifeRect.setFillColor(sf::Color::Transparent);
	window.draw(lifeRect);

	sf::RectangleShape lifeFill;
	lifeFill.setSize(sf::Vector2f(((this->size.x * this->life) / 100), 8));
	lifeFill.setPosition(sf::Vector2f(this->position.x, this->position.y - y_life));
	lifeFill.setFillColor(sf::Color::Green);
	window.draw(lifeFill);

}

bool Player::update(float deltatime,Player* player)
{
	this->last_delta = deltatime;
	bool has_move = false;
	bool up_active = false;
	bool down_active = false;
	bool attack = false;
	sf::Vector2f before_position = this->position;

	float move = this->move_speed * deltatime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
	{
		move /= 4;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		up_active = true;
		has_move = true;
		this->position.y -= move;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		down_active = true;
		has_move = true;
		this->position.y += move;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (up_active || down_active)
		{
			this->position.x -= move / 2;
			
			if (up_active)
			{
				this->position.y += move /2;
			}
			else if (down_active)
			{
				this->position.y -= move /2;
			}
		}else
		{
			has_move = true;
			this->position.x -= move;
		}

		this->direction = LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (up_active || down_active)
		{
			this->position.x += move / 2;

			if (up_active)
			{
				this->position.y += move / 2;
			}
			else if (down_active)
			{
				this->position.y -= move / 2;
			}
		}
		else
		{
			has_move = true;
			this->position.x += move;
		}
		this->direction = RIGHT;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		attack_wait += 90 * deltatime;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (attack_wait > 5.0f)
		{
			attack_wait = 0.0f;
			if (this->check_attack((*player)))
			{
				player->life -= 10;
				attack = true;
			}
		}
	}

	

	if (has_move)
	{
		this->last_position = before_position;
		return true;
	}
	else if (attack)
	{
		return true;
	}

	return false;
}

void Player::go_forward(bool isY,bool add)
{
	sf::Vector2f last = this->position;
	this->position = this->last_position;
	this->last_position = last;
	float move = (this->last_delta * this->move_speed) / 4;
	if (isY)
	{
		this->position.x += add ? move : move - move*2;
	}else
	{
		this->position.y += add ? move : move - move * 2;
	}
}

bool Player::auto_collision(sf::Vector2f item_position, sf::Vector2f item_size)
{
	bool hit = false;

	if (position.x < item_position.x + size.x &&
		position.x + size.x > item_position.x &&
		position.y < item_position.y + size.x &&
		size.x + position.y > item_position.y) 
	{
		hit = true;
	}

	if (hit)
	{
		if (this->last_position.x != this->position.x)
		{
			if (this->last_position.x > this->position.x)
			{
				this->go_forward(false, true);
			}
			else
			{
				this->go_forward(false, false);
			}
		}
		else
		{
			if (this->last_position.y > this->position.y)
			{
				this->go_forward(true, true);
			}
			else
			{
				this->go_forward(true, false);
			}
		}
	}

	return hit;
}

bool Player::check_attack(Player& player)
{
	float dist = sqrt(pow(player.position.x - position.x, 2) + pow(player.position.y - this->position.y, 2));
	if ((dist < this->size.x + (this->size.y / 4)) && player.direction != this->direction)
	{
		if ((this->position.y > player.position.y - player.size.y / 2) && (this->position.y < player.position.y + player.size.y / 2))
		{
			return true;
		}
	}
	return false;
}

Player::~Player()
{
	
}