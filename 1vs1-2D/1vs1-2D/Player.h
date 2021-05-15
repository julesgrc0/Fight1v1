#pragma once
#include<SFML/Graphics.hpp>
#include<Windows.h>
#include<vector>

enum PlayerStates
{
	ATTACK,
	STATIC,
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
	sf::Vector2f last_position;

	PlayerStates state = STATIC;
	PlayerDirection direction = RIGHT;
	float life = 100.0f;
	const float move_speed = 400.0f;

	void draw(sf::RenderWindow&);
	bool update(float&,Player*);
	bool auto_collision(sf::Vector2f, sf::Vector2f);
	bool check_attack(Player& player);
	void go_forward(bool,bool);
	void hit_animation();
	bool hit_update(float&);

	const sf::Vector2f size = sf::Vector2f(50, 50);
private:
	
	float attack_wait = 0.0f;
	float last_delta;
	float hit_wait = 0.0f;
	
	sf::Texture player_r;
	sf::Texture player_l;
};

static std::vector<std::string> split(const std::string& txt, char ch);
std::string get_current_path();