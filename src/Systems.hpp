#pragma once

#include "entt.hpp"
#include "Components.hpp"
#include "raylib.h"

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
			Vector3 minPosition = {transform.position.x - xSize/2.0f, 
											transform.position.y, 
											transform.position.z - xSize/2.0f};
			Vector3 maxPosition = {transform.position.x + xSize/2.0f, 
											transform.position.y + ySize, 
											transform.position.z + zSize/2.0f};
			col.box.min = minPosition;
			col.box.max = maxPosition;


			auto view2 = registry.view<TransformComponent, CollisionComp, MeshComponent>();
			view2.each([&](entt:: entity entity ,const TransformComponent& transform2, CollisionComp& col2, MeshComponent& meshComp)
			{
				if(CheckCollisionBoxes(col.box, col2.box) && col.id != col2.id)
				{
					std::string what = "floor";
					int who = (int)entity;
					if(meshComp.name != "cube")
					{
						what = "player";				
					}
					
					//std::cout<<"Collision!\n";
					lua_rawgeti(L, LUA_REGISTRYINDEX, script.luaRef);

					lua_getfield(L, -1, "onCollision");
					lua_pushvalue(L, -2);
					lua_pushinteger(L, who);
					lua_pushstring(L, what.c_str());

					if(lua_pcall(L, 3, 0, 0) != LUA_OK){
						std::cout<<"Failed to call\n";
					}

					lua_pop(L, 1);
				}
				
			});
		});

		return false;
	}
};

class RayPhysSystem : public System
{
	lua_State* L;
public:
	RayPhysSystem(lua_State* L):L(L){}
	bool OnUpdate(entt::registry& registry, float delta) final
	{
		auto view = registry.view<TransformComponent, CollisionComp, Behaviour>();
		view.each([&](const TransformComponent& transform, CollisionComp& col, Behaviour& script)
		{
			Vector3 pos = col.box.min;
			Vector3 pos2 = col.box.min;
			Vector3 pos3 = col.box.min;
			pos.x += (col.box.max.x - col.box.min.x)/2.0f;
			pos2.x += 2.0f;
			pos3.x = col.box.max.x - 2.0f;
			Ray downRayMid;
			downRayMid.position = pos;
			downRayMid.direction = {0.0f, -1.0f, 0.0f};
			Ray downRayLeft;
			downRayLeft.position = pos2;
			downRayLeft.direction = {0.0f, -1.0f, 0.0f};
			Ray downRayRight;
			downRayRight.position = pos3;
			downRayRight.direction = {0.0f, -1.0f, 0.0f};

			auto view2 = registry.view<TransformComponent, CollisionComp>();
			view2.each([&](const TransformComponent& transform2, CollisionComp& col2)
			{
				if(col.id != col2.id)
				{
					bool grounded = false;
					RayCollision rayCol1 = GetRayCollisionBox(downRayMid, col2.box);
					RayCollision rayCol2 = GetRayCollisionBox(downRayLeft, col2.box);
					RayCollision rayCol3 = GetRayCollisionBox(downRayRight, col2.box);
					//Some ray hit
					if(rayCol1.hit || rayCol2.hit || rayCol3.hit)
					{
						//std::cout<<"Yes\nDistance: "<<rayCol.distance<<std::endl;
						
						if(rayCol1.hit && rayCol1.distance < 5.0f)
						{
							grounded = true;
						}
						if(rayCol2.hit && rayCol2.distance < 5.0f)
						{
							grounded = true;
						}
						if(rayCol3.hit && rayCol3.distance < 5.0f)
						{
							grounded = true;
						}
						if(grounded)
						{
							lua_rawgeti(L, LUA_REGISTRYINDEX, script.luaRef);

							lua_getfield(L, -1, "rayIntersect");
							lua_pushvalue(L, -2);

							if(lua_pcall(L, 1, 0, 0) != LUA_OK){
								std::cout<<"Failed to call\n";
							}

							lua_pop(L, 1);
						}
					}
				}
			});
		});

		return false;
	}
};