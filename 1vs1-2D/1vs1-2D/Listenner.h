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
	Listenner();
	~Listenner();
	void set_client(sf::TcpSocket&);
	bool listen(const char*, void*);
	void send(const char*, void*);
private:
	sf::TcpSocket* client;
};

