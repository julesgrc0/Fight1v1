#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"


class GameClient
{
public:
	GameClient();
	~GameClient();
	bool connect(const char*);
	void send(Player&,Player&);
	bool listen(Player*, Player*);
private:
	sf::TcpSocket socket;
};

