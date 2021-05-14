#include "MainGame.h"
#include<Windows.h>

int main(int argc, const char* argv[])
{
	ShowWindow(GetForegroundWindow(), SW_SHOW);
	std::cout << "Enter server ip:";
	std::string ip;
	std::getline(std::cin, ip);
	ShowWindow(GetForegroundWindow(), SW_HIDE);

	MainGame* game = new MainGame(ip);

	return game->run();
}