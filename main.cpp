#include "raylib.h"
#include "lua.hpp"

#include <thread>
#include <string>
#include <iostream>

#include "Game.hpp"
#include "Scene.hpp"


void DumpError(lua_State* L) {
	if (lua_gettop(L) && lua_isstring(L, -1)) {
		std::cout << "Lua Error" << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

void ConsoleThreadFunction(lua_State* L) {
	std::string input;
	while (true) {
		std::cout << "> ";
		std::getline(std::cin, input);
		if (luaL_dostring(L, input.c_str()) != LUA_OK) {
			DumpError(L);
		}
	}
}

int main(void)
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    std::thread consoleThread(ConsoleThreadFunction, L);


    Scene scene;
	Scene::lua_openscene(L, &scene);

	while(true){};
	// Game game(&scene);
	// game.Run();

    consoleThread.detach();
    return 0;
}