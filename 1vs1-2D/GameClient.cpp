#include "GameClient.h"


GameClient::GameClient()
{
	const char* local = "192.168.112.1";
	sf::Socket::Status status = socket.connect(local, 53000);
	if (status != sf::Socket::Done)
	{
		exit(1);
	}
}

void GameClient::send_update(Player& main_player)
{
	sf::Packet packet;
	packet.append(&(Player)main_player,sizeof(Player));
	this->socket.send(packet);
}

bool GameClient::listen_update(Player* second_player)
{
	sf::Packet packet;
	this->socket.receive(packet);
	second_player = (Player*)packet.getData();
	return false;
}

GameClient::~GameClient()
{

}