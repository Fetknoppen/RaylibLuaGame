#pragma once
#include "raylib.h"
#include "lua.hpp"
#include<iostream>


struct TransformComponent
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

struct MeshComponent
{
	std::string name;
	MeshComponent(std::string name):name(name){}
};

Vector3 lua_tovector(lua_State* L, int index);
void lua_pushvector(lua_State* L, const Vector3& vec);	
int PrintVector(lua_State* L);
TransformComponent lua_totransform(lua_State* L, int index);
void lua_pushtransform(lua_State* L, const TransformComponent& transform);
int PrintTransform(lua_State* L);