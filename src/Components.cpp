#include "Components.hpp"

Vector3 lua_tovector(lua_State* L, int index)
{
	if (!lua_istable(L, index)) {
		throw "lua_tovector: table expected.";
	}
	Vector3 ret;

	lua_getfield(L, index, "x");
	ret.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "y");
	ret.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "z");
	ret.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);


	return ret;
}

void lua_pushvector(lua_State* L, const Vector3& vec)
{
	lua_newtable(L);

	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");
}

int PrintVector(lua_State* L)
{
	Vector3 vec = lua_tovector(L, 1);

	std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;

	return 0;
}


TransformComponent lua_totransform(lua_State* L, int index)
{
	if (!lua_istable(L, index)) {
		throw "lua_totransform: Expected a table.";
	}

	TransformComponent ret;

	lua_getfield(L, index, "position");
	ret.position = lua_tovector(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "rotation");
	ret.rotation = lua_tovector(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "scale");
	ret.scale = lua_tovector(L, -1);
	lua_pop(L, 1);

	return ret;
}

void lua_pushtransform(lua_State* L, const TransformComponent& transform)
{
	lua_newtable(L);

	lua_pushvector(L, transform.position);
	lua_setfield(L, -2, "translation");

	lua_pushvector(L, transform.rotation);
	lua_setfield(L, -2, "rotation");

	lua_pushvector(L, transform.scale);
	lua_setfield(L, -2, "scale");
}

int PrintTransform(lua_State* L)
{
	TransformComponent trans = lua_totransform(L, 1);

	std::cout << "Translation: (" << trans.position.x << ", " << trans.position.y << ", " << trans.position.z << ")" << std::endl;
	std::cout << "Rotation: (" << trans.rotation.x << ", " << trans.rotation.y << ", " << trans.rotation.z << ")" << std::endl;
	std::cout << "Scale: (" << trans.scale.x << ", " << trans.scale.y << ", " << trans.scale.z << ")" << std::endl;

	return 0;
}