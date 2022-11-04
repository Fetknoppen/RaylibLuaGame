#include "Game.hpp"

Game::Game( lua_State* L)
{
    this->scene = new Scene();
    this->scene->lua_openscene(L, this->scene);

    luaL_loadfile(L, "../scripts/vector.lua");
	lua_pcall(L, 0, 1, 0);
	lua_setglobal(L, "vector");
}

Game::~Game()
{
    
}

void Game::run()
{
    this->scene->UpdateSystems(GetFrameTime());

    while(true){};
}