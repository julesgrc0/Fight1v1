#include "MainGame.h"
#include<Windows.h>

extern "C"
{
#include "lua5.4-include/lauxlib.h"
#include "lua5.4-include/lua.h"
#include "lua5.4-include/lualib.h"
}


#ifdef _WIN64
#pragma comment(lib,"lua-5.4-x64/liblua54.a")
#else
#pragma comment(lib,"lua-5.4-x86/liblua54.a")
#endif



int main(int argc, const char* argv[])
{
	ShowWindow(GetForegroundWindow(), SW_SHOW);
	std::cout << "Enter server ip:";
	std::string ip;
	std::getline(std::cin, ip);

	#ifndef _DEBUG
		ShowWindow(GetForegroundWindow(), SW_HIDE);
	#endif // !_DEBUG

	MainGame* game = new MainGame(ip);
	return game->run();
}