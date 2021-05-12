#include "Listenner.h"
#include<iostream>

Listenner::Listenner(sf::TcpSocket* socket)
{
	this->client = socket;
}

bool Listenner::listen(const char* channel, void* data)
{
	sf::Packet packet;
	if (client->receive(packet) == sf::Socket::Status::Done)
	{
		ListennerResponse* response_data;
		response_data = (ListennerResponse*)packet.getData();
		if (response_data->channel == channel)
		{
			data = response_data->data;
		}
		return true;
	}
	return false;
}


bool Listenner::send(const char* channel, void* data)
{
	ListennerResponse send_data = { (char*)channel, data };
	sf::Packet packet;
	packet.append((void*)&send_data, sizeof(send_data));
	if(this->client->send(packet) == sf::Socket::Status::Done)
	{
		return true;
	}
	return false;
}

Listenner::~Listenner()
{

}