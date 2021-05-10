#include<iostream>
#include <SFML/Network.hpp>



int main(int argc, const char* argv[])
{
	sf::TcpListener listener;
	if (listener.listen(53000) != sf::Socket::Done)
	{
		return 1;
	}

	std::cout << "[READY] " << listener.getLocalPort() << "" << std::endl;

	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		return 1;
	}
	

	return 0;
}