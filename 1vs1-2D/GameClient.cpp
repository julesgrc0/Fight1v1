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
	this->listenner.set_client(socket);
	return true;
}

void GameClient::send(Player& player)
{
	GamePlayerData data = { player.state,player.position,player.life,player.direction };
	this->listenner.send("player:update",&data);
}

void GameClient::ping()
{
	time_t t = time(0);
	if (this->listenner.listen("ping", (void*)t))
	{
		time_t ti = time(0);
		std::cout << ti - t << "ms" << std::endl;
		this->listenner.send("ping", (void*)ti);
	}
}

bool GameClient::listen(Player* player)
{
	GamePlayerData data;
	if (this->listenner.listen("player:update", &data))
	{
		player->direction = (PlayerDirection)data.direction;
		player->life = data.life;
		player->position = data.coord;
		player->state = (PlayerStates)data.state;
		return true;
	}
	return false;
}

GameClient::~GameClient()
{

}