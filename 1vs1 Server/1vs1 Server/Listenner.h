#pragma once

#include<SFML/Network.hpp>

struct ListennerResponse
{
	char* channel;
	void* data;
};

class Listenner
{
public:
	Listenner(sf::TcpSocket*);
	~Listenner();
	bool listen(const char*, void*);
	bool send(const char*, void*);
private:
	sf::TcpSocket* client;
};

