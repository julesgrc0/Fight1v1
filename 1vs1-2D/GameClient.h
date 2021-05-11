#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"
#include "Listenner.h"

struct GamePlayerData
{
	int state;
	sf::Vector2f coord;
	int life;
	int direction;
};

class GameClient
{
public:
	GameClient();
	~GameClient();
	bool connect(const char*);
	void send(Player&);
	bool listen(Player*);
	void ping();
private:
	sf::TcpSocket socket;
	Listenner listenner;
};

