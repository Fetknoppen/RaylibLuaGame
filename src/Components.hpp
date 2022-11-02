#pragma once
#include "raylib.h"
#include "lua.hpp"
#include<iostream>

struct PlayerComponent {
	int left, right, forward, back, jump;
	float roll, pitch, yaw;
	int look_left, lookRight, lookUp;
	float camDist;
	Vector3 forwardVec;
	PlayerComponent()
		:left(KEY_A), right(KEY_D), forward(KEY_W),  back(KEY_S), jump(KEY_SPACE), roll(0.0f), pitch(0.0f), yaw(0.0f), camDist(20.0f)
	{
	};
};

//health(int), speed(int)
struct LivingComponent {
	int health;
	int speed;
};

//velocitu(Vector3), gravity(float)
struct PhysichsComponent {
	Vector3 velocity;
	float gravitationalConstant;
	float jumpTimerOrig;
	float jumpTimer;
	bool grounded = false;

};

//BoundingBox
struct CollisionComponent {
	BoundingBox boundingBox;
};

//obj(Model), texture(Texture2D)
struct MeshComponent {
	Model model;
	Texture2D texture;
};


Vector3 lua_tovector3(lua_State* L, int index);
int PrintVector3(lua_State* L);
void lua_pushvector3(lua_State* L, const Vector3& vec);
int RandomVector3(lua_State* L);
Vector4 lua_tovector4(lua_State* L, int index);
int PrintVector4(lua_State* L);
void lua_pushvector4(lua_State* L, const Vector4& vec);
int RandomVector4(lua_State* L);
Transform lua_totransform(lua_State* L, int index);
void lua_pushtransform(lua_State* L, const Transform& transform);
int PrintTransform(lua_State* L);