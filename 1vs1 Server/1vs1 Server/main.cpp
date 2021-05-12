#include<iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "Listenner.h"

struct GamePlayerData
{
	int state = 0;
	sf::Vector2f coord = sf::Vector2f(0,0);
	int life = 100;
	int direction = 0;
};

void print_player(const char* msg, sf::TcpSocket* player)
{
	std::cout << msg << (*player).getRemoteAddress().toString() << ":" << (*player).getRemotePort() << std::endl;
}

void remove_from_players(sf::TcpSocket* player, std::vector<sf::TcpSocket*>* players)
{
	for (size_t i = 0; i < players->size(); i++)
	{
		if ((*player).getRemoteAddress() == (*(*players)[i]).getRemoteAddress() && (*player).getRemotePort() == (*(*players)[i]).getRemotePort())
		{
			players->erase(players->begin() + i);
			break;
		}
	}
}

void player_thread(sf::TcpSocket* player, std::vector<sf::TcpSocket*>* players)
{
	print_player("[new player] ", player);
	Listenner player_listenner = Listenner(player);
	
	while (true)
	{
		GamePlayerData player_data;
		if (player_listenner.listen("player:update", &player_data))
		{
			
			for (size_t i = 0; i < players->size(); i++)
			{
				if (!((*player).getRemoteAddress() == (*(*players)[i]).getRemoteAddress() && (*player).getRemotePort() == (*(*players)[i]).getRemotePort()))
				{
					Listenner temp_listenner = Listenner((*players)[i]);
					player_data.coord = sf::Vector2f(10, 10);
					if (!temp_listenner.send("player:update", &player_data))
					{
						remove_from_players((*players)[i], players);
						i--;
					}
				}
			}
		}
		else
		{
			remove_from_players(player, players);
			break;
		}
	}

	print_player("[player leave] ", player);
	delete player;
}

void players_socket_thread(sf::TcpListener& server)
{
	std::vector<sf::TcpSocket*> players = {};
	while (true)
	{
		sf::TcpSocket* player = new sf::TcpSocket();
		if (server.accept((*player)) == sf::Socket::Done)
		{
			players.push_back(player);
			std::thread([&]() {
				player_thread(players[players.size()-1], &players);
				}).detach();
		}
	}
}

int main(int argc, const char* argv[])
{
	sf::TcpListener server;
	if (server.listen(53000) != sf::Socket::Done)
	{
		return 1;
	}
	std::cout << "[READY] " << server.getLocalPort() << "" << std::endl;
	
	players_socket_thread(server);

	return 0;
}