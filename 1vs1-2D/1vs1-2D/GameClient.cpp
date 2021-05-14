#include "GameClient.h"

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

void GameClient::send(Player& player,Player& second)
{
	sf::Packet packet;
	packet << (int)player.state << (float)(440 - player.position.x) << (float)player.position.y << (int)player.life << (int)player.direction << (float)second.life;
	this->socket.send(packet);
}


bool GameClient::listen(Player* player,Player* second)
{
	sf::Packet packet;
	if (this->socket.receive(packet) == sf::Socket::Done)
	{
		int state;
		float x;
		float y;
		int life;
		int direction;
		float p2_life;

		packet >> state >> x >> y >> life >> direction >> p2_life;

		player->life = life;
		player->position.x = x;
		player->position.y = y;
		player->direction = (PlayerDirection)direction;
		player->state = (PlayerStates)state;
		second->life = p2_life;

		return true;
	}
	return false;
}

GameClient::~GameClient()
{

}