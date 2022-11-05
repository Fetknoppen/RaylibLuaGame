#pragma once

#include "entt.hpp"
#include "Components.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

class System 
{
public:
	virtual ~System() = default;
	virtual bool OnUpdate(entt::registry& registry, float delta) = 0;
};

class BehaviourSystem : public System
{
	lua_State* L;
public:
	BehaviourSystem(lua_State* L)
		:L(L)
	{

	}


	bool OnUpdate(entt::registry& registry, float delta) final
	{
		auto view = registry.view<Behaviour>();

		view.each([&](Behaviour& script){
			lua_rawgeti(L, LUA_REGISTRYINDEX, script.luaRef);

			lua_getfield(L, -1, "update");
			lua_pushvalue(L, -2);
			lua_pushnumber(L, delta);

			if(lua_pcall(L, 2, 0, 0) != LUA_OK){
				std::cout<<"Failed to call\n";
			}

			lua_pop(L, 1);

		});

		return false;
	}
};