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

class CollisionSystem : public System
{
	lua_State* L;
public:
	CollisionSystem(lua_State* L)
		:L(L)
	{

	}


	bool OnUpdate(entt::registry& registry, float delta) final
	{
		auto view = registry.view<TransformComponent, CollisionComp, Behaviour>();
		view.each([&](const TransformComponent& transform, CollisionComp& col, Behaviour& script)
		{

			float xSize = col.box.max.x - col.box.min.x;
			float ySize = col.box.max.y - col.box.min.y;
			float zSize = col.box.max.z - col.box.min.z;
			Vector3 minPosition = (Vector3){transform.position.x - xSize/2.0f, 
											transform.position.y, 
											transform.position.z - xSize/2.0f};
			Vector3 maxPosition = (Vector3){transform.position.x + xSize/2.0f, 
											transform.position.y + ySize, 
											transform.position.z + zSize/2.0f};
			col.box.min = minPosition;
			col.box.max = maxPosition;


			auto view2 = registry.view<TransformComponent, CollisionComp>();
			view2.each([&](const TransformComponent& transform2, CollisionComp& col2)
			{
				if(CheckCollisionBoxes(col.box, col2.box) && col.id != col2.id)
				{
					std::string where;
					if(col.box.min.y < col2.box.max.y && col.box.min.y > col2.box.min.y)
					{
						//Collision below player
						where = "below";
						
					}
					else if(col.box.min.y < col2.box.min.y && col.box.max.y > col2.box.min.y)
					{
						//Collision above player
						where = "above";
					}
					
					//std::cout<<"Collision!\n";
					lua_rawgeti(L, LUA_REGISTRYINDEX, script.luaRef);

					lua_getfield(L, -1, "onCollision");
					lua_pushvalue(L, -2);
					lua_pushstring(L, where.c_str());

					if(lua_pcall(L, 2, 0, 0) != LUA_OK){
						std::cout<<"Failed to call\n";
					}

					lua_pop(L, 1);
				}
				
			});
		});

		return false;
	}
};