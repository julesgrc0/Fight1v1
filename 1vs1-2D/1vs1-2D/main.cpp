#define _CRT_SECURE_NO_WARNINGS

#include "MainGame.h"
#include <fstream>
#include <string>
#include <Windows.h>

#ifdef _WIN64
#pragma comment(lib,"lua-5.4-x64/liblua54.a")
#else
#pragma comment(lib,"lua-5.4-x86/liblua54.a")
#endif



int main(int argc, const char* argv[])
{
	std::string lua_bot = "C:/Users/jules/source/repos/1vs1-2D/1vs1-2D/1vs1-2D/test_bot.lua";
	if (argc > 1)
	{
		lua_bot = std::string(argv[1]);
	}

	ShowWindow(GetForegroundWindow(), SW_SHOW);

	std::cout << "Enter server ip:";
	std::string ip;
	std::getline(std::cin, ip);

	#ifndef _DEBUG
		ShowWindow(GetForegroundWindow(), SW_HIDE);
	#endif // !_DEBUG

	MainGame* game = new MainGame(ip,lua_bot);
	return game->run();
}