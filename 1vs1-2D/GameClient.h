#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"


class GameClient
{
public:
	GameClient();
	~GameClient();
	bool listen_update(Player*);
	void send_update(Player&);
private:
	sf::TcpSocket socket;
};

