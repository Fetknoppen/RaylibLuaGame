#include "Components.hpp"

Vector3 lua_tovector3(lua_State* L, int index)
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

int PrintVector3(lua_State* L)
{
	Vector3 vec = lua_tovector3(L, 1);

	std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;

	return 0;
}

void lua_pushvector3(lua_State* L, const Vector3& vec)
{
	lua_newtable(L);

	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");
}

int RandomVector3(lua_State* L)
{
	int min, max, range;
	max = (int)lua_tonumber(L, 2);
	min = (int)lua_tonumber(L, 1);
	lua_pop(L, 2);
	range = max - min;

	int randX = (rand() % range + 1) + min;
	int randY = (rand() % range + 1) + min;
	int randZ = (rand() % range + 1) + min;
	Vector3 vec = Vector3({(float)randX, (float)randY, (float)randZ});

	lua_pushvector3(L, vec);


	return 1;
}

Vector4 lua_tovector4(lua_State* L, int index)
{
	if (!lua_istable(L, index)) {
		throw "lua_tovector: table expected.";
	}
	Vector4 ret;

	lua_getfield(L, index, "x");
	ret.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "y");
	ret.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "z");
	ret.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "w");
	ret.w = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);


	return ret;
}

int PrintVector4(lua_State* L)
{
	Vector4 vec = lua_tovector4(L, 1);

	std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", "<< vec.w << ")" << std::endl;

	return 0;
}

void lua_pushvector4(lua_State* L, const Vector4& vec)
{
	lua_newtable(L);

	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");
	lua_pushnumber(L, vec.w);
	lua_setfield(L, -2, "w");
}

int RandomVector4(lua_State* L)
{
	int min, max, range;
	max = (int)lua_tonumber(L, 2);
	min = (int)lua_tonumber(L, 1);
	lua_pop(L, 2);
	range = max - min;

	int randX = (rand() % range + 1) + min;
	int randY = (rand() % range + 1) + min;
	int randZ = (rand() % range + 1) + min;
	int randW = (rand() % range + 1) + min;
	Vector4 vec = Vector4({(float)randX, (float)randY, (float)randZ, (float)randW});

	lua_pushvector4(L, vec);


	return 1;
}

Transform lua_totransform(lua_State* L, int index)
{
	if (!lua_istable(L, index)) {
		throw "lua_totransform: Expected a table.";
	}

	Transform ret;

	lua_getfield(L, index, "position");
	ret.translation = lua_tovector3(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "rotation");
	ret.rotation = lua_tovector4(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "scale");
	ret.scale = lua_tovector3(L, -1);
	lua_pop(L, 1);

	return ret;
}

void lua_pushtransform(lua_State* L, const Transform& transform)
{
	lua_newtable(L);

	lua_pushvector3(L, transform.translation);
	lua_setfield(L, -2, "translation");

	lua_pushvector4(L, transform.rotation);
	lua_setfield(L, -2, "rotation");

	lua_pushvector3(L, transform.scale);
	lua_setfield(L, -2, "scale");
}

int PrintTransform(lua_State* L)
{
	Transform trans = lua_totransform(L, 1);

	std::cout << "Translation: (" << trans.translation.x << ", " << trans.translation.y << ", " << trans.translation.z << ")" << std::endl;
	std::cout << "Rotation: (" << trans.rotation.x << ", " << trans.rotation.y << ", " << trans.rotation.z << ", " << trans.rotation.w << ")" << std::endl;
	std::cout << "Scale: (" << trans.scale.x << ", " << trans.scale.y << ", " << trans.scale.z << ")" << std::endl;

	return 0;
}