#include "GameClient.h"
#include<iostream>

GameClient::GameClient()
{
}

bool GameClient::connect(const char* local)
{
	sf::Socket::Status status = socket.connect(local, 53000);
	if (status != sf::Socket::Done)
	{
		return false;
	}
	return true;
}

void GameClient::send(Player& player)
{
	sf::Packet packet;
	packet << (int)player.state << (float)player.position.x << (float)player.position.y << (int)player.life << (int)player.direction;
	this->socket.send(packet);
}


bool GameClient::listen(Player* player)
{
	sf::Packet packet;
	if (this->socket.receive(packet) == sf::Socket::Done)
	{
		int state;
		float x;
		float y;
		int life;
		int direction;

		packet >> state >> x >> y >> life >> direction;

		player->life = life;
		player->position.x = x;
		player->position.y = y;
		player->direction = (PlayerDirection)direction;
		player->state = (PlayerStates)state;

		return true;
	}
	return false;
}

GameClient::~GameClient()
{

}