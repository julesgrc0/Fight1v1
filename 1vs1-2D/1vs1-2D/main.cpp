#define _CRT_SECURE_NO_WARNINGS

#include "MainGame.h"
#include <fstream>
#include <string>
#include <Windows.h>

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
	lua_State* L = luaL_newstate();
	if(argc > 1)
	{
			std::ifstream ifs(argv[1]);
			if (ifs)
			{
				std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
				luaL_openlibs(L);

				int lua_result = luaL_dostring(L, content.c_str());
				
				if (lua_result == LUA_OK)
				{
					
				}else
				{
					std::cout << "Fail " << lua_tostring(L,-1) << std::endl;
				}
			}
			else
			{
				std::cout << "Fail to read input file !" << std::endl;
			}
	}

	ShowWindow(GetForegroundWindow(), SW_SHOW);
	std::cout << "Enter server ip:";
	std::string ip;
	std::getline(std::cin, ip);

	#ifndef _DEBUG
		ShowWindow(GetForegroundWindow(), SW_HIDE);
	#endif // !_DEBUG

	MainGame* game = new MainGame(ip);
	int exit_code = game->run();
	
	lua_close(L);

	return exit_code;
}