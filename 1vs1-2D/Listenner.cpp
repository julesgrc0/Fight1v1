#include "Listenner.h"

Listenner::Listenner()
{
	
}

void Listenner::set_client(sf::TcpSocket& socket)
{
	this->client = &socket;
}

bool Listenner::listen(const char* channel, void* data)
{
	sf::Packet packet;
	if (this->client->receive(packet) == sf::Socket::Status::Done)
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

void Listenner::send(const char* channel, void* data)
{
	ListennerResponse send_data = { (char*)channel, data };
	sf::Packet packet;
	packet.append((void*)&send_data, sizeof(send_data));
	this->client->send(packet);
}

Listenner::~Listenner()
{

}